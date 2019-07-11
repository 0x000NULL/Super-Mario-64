// moat_drainer.c.inc

void bhv_invisible_objects_under_bridge_init(void)
{
    if(save_file_get_flags() & SAVE_FLAG_MOAT_DRAINED)
    {
        gWaterRegions[6]    = -800;
        gWaterRegions[12] = -800;
    }
}
