#include <ultra64.h>

#include "sm64.h"
#include "game/level_update.h"
#include "game/debug.h"
#include "game/camera.h"
#include "game/mario.h"
#include "behavior_script.h"
#include "surface_collision.h"
#include "surface_load.h"
#include "game/object_list_processor.h"
#include "game/room.h"

struct FloorGeometry D_8038BE30;

static u8 unused8038BE50[0x40];


static s32 find_wall_collisions_from_list(
    struct SurfaceNode *surfaceNode, struct WallCollisionData *data)
{
    register f32 offset;
    register f32 radius = data->radius;
    register struct Surface *surf;
    register f32 x = data->x;
    register f32 y = data->y + data->offsetY;
    register f32 z = data->z;
    register f32 px, pz;
    register f32 w1, w2, w3;
    register f32 y1, y2, y3;
    s32 numCols = 0;

    // Max collision radius = 200
    if (radius > 200.0f) radius = 200.0f;

    while (surfaceNode != NULL)
    {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        if (y < surf->lowerY || y > surf->upperY)
            continue;

        offset = surf->normal.x * x + surf->normal.y * y + surf->normal.z * z + surf->originOffset;

        if (offset < -radius || offset > radius)
            continue;

        px = x;
        pz = z;

        if (surf->flags & SURFACE_FLAG_X_PROJECTION)
        {
            w1 = -surf->vertex1[2];
            w2 = -surf->vertex2[2];
            w3 = -surf->vertex3[2];
            y1 = surf->vertex1[1];
            y2 = surf->vertex2[1];
            y3 = surf->vertex3[1];

            if (surf->normal.x > 0.0f)
            {
                if ((y1 - y) * (w2 - w1) - (w1 - -pz) * (y2 - y1) > 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - -pz) * (y3 - y2) > 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - -pz) * (y1 - y3) > 0.0f) continue;
            }
            else
            {
                if ((y1 - y) * (w2 - w1) - (w1 - -pz) * (y2 - y1) < 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - -pz) * (y3 - y2) < 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - -pz) * (y1 - y3) < 0.0f) continue;
            }
        }
        else
        {
            w1 = surf->vertex1[0];
            w2 = surf->vertex2[0];
            w3 = surf->vertex3[0];
            y1 = surf->vertex1[1];
            y2 = surf->vertex2[1];
            y3 = surf->vertex3[1];

            if (surf->normal.z > 0.0f)
            {
                if ((y1 - y) * (w2 - w1) - (w1 - px) * (y2 - y1) > 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - px) * (y3 - y2) > 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - px) * (y1 - y3) > 0.0f) continue;
            }
            else
            {
                if ((y1 - y) * (w2 - w1) - (w1 - px) * (y2 - y1) < 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - px) * (y3 - y2) < 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - px) * (y1 - y3) < 0.0f) continue;
            }
        }

        if (gCheckingSurfaceCollisionsForCamera)
        {
            if (surf->flags & SURFACE_FLAG_1)
                continue;
        }
        else
        {
            if (surf->type == SURFACE_CAMERA_BOUNDARY)
                continue;

            if (surf->type == SURFACE_VANISH_CAP_PASSABLE)
            {
                if (gCurrentObject != NULL &&
                    (gCurrentObject->activeFlags & ACTIVE_FLAG_MOVE_THROUGH_GRATE))
                {
                    continue;
                }

                if (gCurrentObject != NULL && gCurrentObject == gMarioObject &&
                    (gMarioState->flags & MARIO_VANISH_CAP))
                {
                    continue;
                }
            }
        }

        //! Because this doesn't update the x and z local variables, multiple
        // walls can push mario more than is required.
        data->x += surf->normal.x * (radius - offset);
        data->z += surf->normal.z * (radius - offset);

        if (data->numWalls < 4)
            data->walls[data->numWalls++] = surf;

        numCols++;
    }

    return numCols;
}

s32 resolve_wall_collisions(f32 *xPtr, f32 *yPtr, f32 *zPtr, f32 offsetY, f32 radius)
{
    struct WallCollisionData collision;
    s32 numCollisions = 0;

    collision.offsetY = offsetY;
    collision.radius = radius;

    collision.x = *xPtr;
    collision.y = *yPtr;
    collision.z = *zPtr;

    collision.numWalls = 0;

    numCollisions = find_wall_collisions(&collision);
    *xPtr = collision.x;
    *yPtr = collision.y;
    *zPtr = collision.z;

    return numCollisions;
}

s32 find_wall_collisions(struct WallCollisionData *colData)
{
    struct SurfaceNode *node;
    s16 cellX, cellZ;
    s32 numCollisions = 0;
    s16 x = colData->x;
    s16 z = colData->z;

    colData->numWalls = 0;

    if (x <= -0x2000 || x >= 0x2000) return numCollisions;
    if (z <= -0x2000 || z >= 0x2000) return numCollisions;

    // World (level) consists of a 16x16 grid. Find where the collision is on
    // the grid (round toward -inf)
    cellX = ((x + 0x2000) / 0x400) & 0x0F;
    cellZ = ((z + 0x2000) / 0x400) & 0x0F;

    node = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    numCollisions += find_wall_collisions_from_list(node, colData);

    node = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next;
    numCollisions += find_wall_collisions_from_list(node, colData);

    gNumFindWallCalls += 1;
    return numCollisions;
}

static struct Surface *find_ceil_from_list(
    struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight)
{
    register struct Surface *surf;
    register s32 x1, z1, x2, z2, x3, z3;
    struct Surface *ceil = NULL;

    ceil = NULL;
    while (surfaceNode != NULL)
    {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        x1 = surf->vertex1[0];
        z1 = surf->vertex1[2];
        z2 = surf->vertex2[2];
        x2 = surf->vertex2[0];

        if ((z1 - z) * (x2 - x1) - (x1 - x) * (z2 - z1) > 0) continue;
        x3 = surf->vertex3[0];
        z3 = surf->vertex3[2];
        if ((z2 - z) * (x3 - x2) - (x2 - x) * (z3 - z2) > 0) continue;
        if ((z3 - z) * (x1 - x3) - (x3 - x) * (z1 - z3) > 0) continue;

        if (gCheckingSurfaceCollisionsForCamera != 0)
        {
            if (surf->flags & SURFACE_FLAG_1)
                continue;
        }
        else if (surf->type == SURFACE_CAMERA_BOUNDARY)
        {
            continue;
        }

        {
            f32 nx = surf->normal.x;
            f32 ny = surf->normal.y;
            f32 nz = surf->normal.z;
            f32 oo = surf->originOffset;
            f32 height;

            if (ny == 0.0f) continue;

            height = -(x * nx + nz * z + oo) / ny;
            if (y - (height - -78.0f) > 0.0f) continue;

            *pheight = height;
            ceil = surf;
            break;
        }
    }

    return ceil;
}

f32 find_ceil(f32 posX, f32 posY, f32 posZ, struct Surface **pceil)
{
    s16 cellZ, cellX;
    struct Surface *ceil, *dynamicCeil;
    struct SurfaceNode *surfaceList;
    f32 height = 20000.0f;
    f32 dynamicHeight = 20000.0f;
    s16 x, y, z;

    //! PUs
    x = (s16) posX;
    y = (s16) posY;
    z = (s16) posZ;
    *pceil = NULL;

    if (x <= -0x2000 || x >= 0x2000) return height;
    if (z <= -0x2000 || z >= 0x2000) return height;

    cellX = ((x + 0x2000) / 0x400) & 0xF;
    cellZ = ((z + 0x2000) / 0x400) & 0xF;

    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next;
    dynamicCeil = find_ceil_from_list(surfaceList, x, y, z, &dynamicHeight);

    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_CEILS].next;
    ceil = find_ceil_from_list(surfaceList, x, y, z, &height);

    if (dynamicHeight < height)
    {
        ceil = dynamicCeil;
        height = dynamicHeight;
    }

    *pceil = ceil;

    gNumFindCeilCalls += 1;
    return height;
}

static f32 unused_find_floor_height(struct Object *obj)
{
    struct Surface *floor;
    f32 floorHeight = find_floor(obj->oPosX, obj->oPosY, obj->oPosZ, &floor);
    return floorHeight;
}

/**
 * Return the floor height underneath (xPos, yPos, zPos) and populate `floorGeo`
 * with data about the floor's normal vector and origin offset. Also update 
 * D_8038BE30.
 */
f32 find_floor_height_and_data(f32 xPos, f32 yPos, f32 zPos, struct FloorGeometry **floorGeo)
{
    struct Surface *floor;
    f32 floorHeight = find_floor(xPos, yPos, zPos, &floor);

    *floorGeo = NULL;

    if (floor != NULL)
    {
        D_8038BE30.normalX = floor->normal.x;
        D_8038BE30.normalY = floor->normal.y;
        D_8038BE30.normalZ = floor->normal.z;
        D_8038BE30.originOffset = floor->originOffset;

        *floorGeo = &D_8038BE30;
    }
    return floorHeight;
}

static struct Surface *find_floor_from_list(
    struct SurfaceNode *surfaceNode, s32 x, s32 y, s32 z, f32 *pheight)
{
    register struct Surface *surf;
    register s32 x1, z1, x2, z2, x3, z3;
    f32 nx, ny, nz;
    f32 oo;
    f32 height;
    struct Surface *floor = NULL;

    while (surfaceNode != NULL)
    {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        x1 = surf->vertex1[0];
        z1 = surf->vertex1[2];
        x2 = surf->vertex2[0];
        z2 = surf->vertex2[2];

        if ((z1 - z) * (x2 - x1) - (x1 - x) * (z2 - z1) < 0) continue;
        x3 = surf->vertex3[0];
        z3 = surf->vertex3[2];
        if ((z2 - z) * (x3 - x2) - (x2 - x) * (z3 - z2) < 0) continue;
        if ((z3 - z) * (x1 - x3) - (x3 - x) * (z1 - z3) < 0) continue;

        if (gCheckingSurfaceCollisionsForCamera != 0)
        {
            if (surf->flags & 0x02)
                continue;
        }
        else if (surf->type == SURFACE_CAMERA_BOUNDARY)
        {
            continue;
        }

        nx = surf->normal.x;
        ny = surf->normal.y;
        nz = surf->normal.z;
        oo = surf->originOffset;

        if (ny == 0.0f) continue;

        height = -(x * nx + nz * z + oo) / ny;
        if (y - (height + -78.0f) < 0.0f) continue;

        *pheight = height;
        floor = surf;
        break;
    }

    return floor;
}

f32 find_floor_height(f32 x, f32 y, f32 z)
{
    struct Surface *floor;
    f32 floorHeight = find_floor(x, y, z, &floor);
    return floorHeight;
}

static f32 find_dynamic_floor(f32 xPos, f32 yPos, f32 zPos, struct Surface **pfloor)
{
    struct SurfaceNode *surfaceList;
    struct Surface *floor;
    f32 floorHeight = -11000.0f;

    //! PUs
    s16 x = (s16) xPos;
    s16 y = (s16) yPos;
    s16 z = (s16) zPos;

    s16 cellX = ((x + 0x2000) / 0x400) & 0x0F;
    s16 cellZ = ((z + 0x2000) / 0x400) & 0x0F;

    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS].next;
    floor = find_floor_from_list(surfaceList, x, y, z, &floorHeight);

    *pfloor = floor;
    return floorHeight;
}

f32 find_floor(f32 xPos, f32 yPos, f32 zPos, struct Surface **pfloor)
{
    s16 cellZ, cellX;
    struct Surface *floor, *dynamicFloor;
    struct SurfaceNode *surfaceList;
    f32 height = -11000.0f;
    f32 dynamicHeight = -11000.0f;

    //! PUs
    s16 x = (s16) xPos;
    s16 y = (s16) yPos;
    s16 z = (s16) zPos;

    *pfloor = NULL;

    if (x <= -0x2000 || x >= 0x2000) return height;
    if (z <= -0x2000 || z >= 0x2000) return height;

    cellX = ((x + 0x2000) / 0x400) & 0xF;
    cellZ = ((z + 0x2000) / 0x400) & 0xF;

    surfaceList = gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS].next;
    dynamicFloor = find_floor_from_list(surfaceList, x, y, z, &dynamicHeight);

    surfaceList = gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_FLOORS].next;
    floor = find_floor_from_list(surfaceList, x, y, z, &height);

    if (!gFindFloorIncludeSurface0012)
    {
        if (floor != NULL && floor->type == SURFACE_0012)
            floor = find_floor_from_list(surfaceList, x, (s32) (height - 200.0f), z, &height);
    }
    else
    {
        gFindFloorIncludeSurface0012 = FALSE;
    }

    if (floor == NULL)
        gNumFindFloorMisses += 1;

    if (dynamicHeight > height)
    {
        floor = dynamicFloor;
        height = dynamicHeight;
    }

    *pfloor = floor;

    gNumFindFloorCalls += 1;
    return height;
}

f32 find_water_level(f32 x, f32 z)
{
    s32 i;
    s32 numRegions;
    s16 val;
    f32 loX, hiX, loZ, hiZ;
    f32 waterLevel = -11000.0f;
    s16 *p = gWaterRegions;

    if (p != NULL)
    {
        numRegions = *p++;

        for (i = 0; i < numRegions; i++)
        {
            val = *p++;
            loX = *p++;
            loZ = *p++;
            hiX = *p++;
            hiZ = *p++;

            if (loX < x && x < hiX && loZ < z && z < hiZ && val < 50)
            {
                waterLevel = *p;
                break;
            }
            p++;
        }
    }

    return waterLevel;
}

f32 func_80381D3C(f32 x, f32 z)
{
    s32 i;
    s32 numRegions;
    UNUSED s32 unused;
    s16 val;
    f32 loX, hiX, loZ, hiZ;
    f32 waterLevel = -11000.0f;
    s16 *p = gWaterRegions;

    if (p != NULL)
    {
        numRegions = *p++;

        for (i = 0; i < numRegions; i++)
        {
            val = *p;

            if (val >= 50)
            {
                loX = *(p + 1);
                loZ = *(p + 2);
                hiX = *(p + 3);
                hiZ = *(p + 4);

                if (loX < x && x < hiX && loZ < z && z < hiZ && val % 10 == 0)
                {
                    waterLevel = *(p + 5);
                    break;
                }
            }

            p += 6;
        }
    }

    return waterLevel;
}

static s32 surface_list_length(struct SurfaceNode *list)
{
    s32 count = 0;

    while (list != NULL)
    {
        list = list->next;
        count++;
    }

    return count;
}

void debug_surface_list_info(f32 xPos, f32 zPos)
{
    struct SurfaceNode *list;
    s32 numFloors = 0;
    s32 numWalls = 0;
    s32 numCeils = 0;

    s32 cellX = (xPos + 0x2000) / 0x400;
    s32 cellZ = (zPos + 0x2000) / 0x400;

    list = gStaticSurfacePartition[cellZ & 0x0F][cellX & 0x0F][SPATIAL_PARTITION_FLOORS].next;
    numFloors += surface_list_length(list);

    list = gDynamicSurfacePartition[cellZ & 0x0F][cellX & 0x0F][SPATIAL_PARTITION_FLOORS].next;
    numFloors += surface_list_length(list);

    list = gStaticSurfacePartition[cellZ & 0x0F][cellX & 0x0F][SPATIAL_PARTITION_WALLS].next;
    numWalls += surface_list_length(list);

    list = gDynamicSurfacePartition[cellZ & 0x0F][cellX & 0x0F][SPATIAL_PARTITION_WALLS].next;
    numWalls += surface_list_length(list);

    list = gStaticSurfacePartition[cellZ & 0x0F][cellX & 0x0F][SPATIAL_PARTITION_CEILS].next;
    numCeils += surface_list_length(list);

    list = gDynamicSurfacePartition[cellZ & 0x0F][cellX & 0x0F][SPATIAL_PARTITION_CEILS].next;
    numCeils += surface_list_length(list);

    print_debug_top_down_mapinfo("area   %x", cellZ * 16 + cellX);

    print_debug_top_down_mapinfo("dg %d", numFloors);
    print_debug_top_down_mapinfo("dw %d", numWalls);
    print_debug_top_down_mapinfo("dr %d", numCeils);

    set_text_array_x_y(80, -3);

    print_debug_top_down_mapinfo("%d", gNumFindFloorCalls);
    print_debug_top_down_mapinfo("%d", gNumFindWallCalls);
    print_debug_top_down_mapinfo("%d", gNumFindCeilCalls);

    set_text_array_x_y(-80, 0);

    print_debug_top_down_mapinfo("listal %d", gSurfaceNodesAllocated);
    print_debug_top_down_mapinfo("statbg %d", gNumStaticSurfaces);
    print_debug_top_down_mapinfo("movebg %d", gSurfacesAllocated - gNumStaticSurfaces);

    gNumFindFloorCalls = 0;
    gNumFindCeilCalls = 0;
    gNumFindWallCalls = 0;
}

static s32 unused_resolve_floor_or_ceil_collisions(
    s32 checkCeil,
    f32 *px, f32 *py, f32 *pz,
    f32 radius,
    struct Surface **psurface,
    f32 *surfaceHeight)
{
    f32 nx, ny, nz, oo;
    f32 x = *px, y = *py, z = *pz;
    f32 offset, distance;

    *psurface = NULL;

    if (checkCeil)
        *surfaceHeight = find_ceil(x, y, z, psurface);
    else
        *surfaceHeight = find_floor(x, y, z, psurface);

    if (*psurface == NULL)
        return -1;

    nx = (*psurface)->normal.x;
    ny = (*psurface)->normal.y;
    nz = (*psurface)->normal.z;
    oo = (*psurface)->originOffset;

    offset = nx * x + ny * y + nz * z + oo;
    distance = offset >= 0 ? offset : -offset;

    if (distance < radius)
    {
        *px += nx * (radius - offset);
        *py += ny * (radius - offset);
        *pz += nz * (radius - offset);

        return 1;
    }

    return 0;
}
