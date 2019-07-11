#include <ultra64.h>

#include "sm64.h"
#include "audio/interface_2.h"
#include "game.h"
#include "memory.h"
#include "sound_init.h"
#include "profiler.h"
#include "game.h"
#include "segments.h"
#include "main.h"

// Message IDs
#define MESG_SP_COMPLETE      100
#define MESG_DP_COMPLETE      101
#define MESG_VI_VBLANK        102
#define MESG_START_GFX_SPTASK 103
#define MESG_NMI_REQUEST      104

OSThread D_80339210;  // unused?
OSThread gIdleThread;
OSThread gMainThread;
OSThread gGameLoopThread;
OSThread gSoundThread;
OSMesgQueue gPIMesgQueue;
OSMesgQueue gIntrMesgQueue;
OSMesgQueue gSPTaskMesgQueue;
OSMesg gDmaMesgBuf[1];
OSMesg gPIMesgBuf[32];
OSMesg gSIEventMesgBuf[1];
OSMesg gIntrMesgBuf[16];
OSMesg gUnknownMesgBuf[16];
OSIoMesg gDmaIoMesg;
OSMesg D_80339BEC;
OSMesgQueue gDmaMesgQueue;
OSMesgQueue gSIEventMesgQueue;

struct VblankHandler *gVblankHandler1 = NULL;
struct VblankHandler *gVblankHandler2 = NULL;
struct SPTask *gActiveSPTask = NULL;
struct SPTask *sCurrentAudioSPTask = NULL;
struct SPTask *sCurrentDisplaySPTask = NULL;
struct SPTask *sNextAudioSPTask = NULL;
struct SPTask *sNextDisplaySPTask = NULL;
s8 sAudioEnabled = 1;
u32 sNumVblanks = 0;
s8 gResetTimer = 0;
s8 D_8032C648 = 0;
s8 gDebugLevelSelect = 0;
s8 D_8032C650 = 0;

s8 gShowProfiler = FALSE;
s8 gShowDebugText = FALSE;
static u16 sProfilerKeySequence[] = {U_JPAD, U_JPAD, D_JPAD, D_JPAD, L_JPAD, R_JPAD, L_JPAD, R_JPAD};
static u16 sDebugTextKeySequence[]     = {D_JPAD, D_JPAD, U_JPAD, U_JPAD, L_JPAD, R_JPAD, L_JPAD, R_JPAD};
static s16 sProfilerKey = 0;
static s16 sDebugTextKey = 0;

// unused
void handle_debug_key_sequences(void)
{
    if (gPlayer3Controller->buttonPressed != 0)
    {
        if (sProfilerKeySequence[sProfilerKey++] == gPlayer3Controller->buttonPressed)
        {
            if (sProfilerKey == ARRAY_COUNT(sProfilerKeySequence))
                sProfilerKey = 0, gShowProfiler ^= 1;
        }
        else
        {
            sProfilerKey = 0;
        }

        if (sDebugTextKeySequence[sDebugTextKey++] == gPlayer3Controller->buttonPressed)
        {
            if (sDebugTextKey == ARRAY_COUNT(sDebugTextKeySequence))
                sDebugTextKey = 0, gShowDebugText ^= 1;
        }
        else
        {
            sDebugTextKey = 0;
        }
    }
}

static void Unknown80246170(void)
{
    // uninitialized
    OSTime time;
    u32 b;

    osSetTime(time);
    osMapTLB(0, b, NULL, 0, 0, 0);
    osUnmapTLBAll();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull"
    sprintf(NULL, NULL);
#pragma GCC diagnostic pop
}

static void Dummy802461CC(void)
{
}

static void Dummy802461DC(void)
{
}

static void Dummy802461EC(void)
{
}

static void setup_mesg_queues(void)
{
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, ARRAY_COUNT(gDmaMesgBuf));
    osCreateMesgQueue(&gSIEventMesgQueue, gSIEventMesgBuf, ARRAY_COUNT(gSIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &gSIEventMesgQueue, NULL);

    osCreateMesgQueue(&gSPTaskMesgQueue, gUnknownMesgBuf, ARRAY_COUNT(gUnknownMesgBuf));
    osCreateMesgQueue(&gIntrMesgQueue, gIntrMesgBuf, ARRAY_COUNT(gIntrMesgBuf));
    osViSetEvent(&gIntrMesgQueue, (OSMesg)MESG_VI_VBLANK, 1);

    osSetEventMesg(OS_EVENT_SP, &gIntrMesgQueue, (OSMesg)MESG_SP_COMPLETE);
    osSetEventMesg(OS_EVENT_DP, &gIntrMesgQueue, (OSMesg)MESG_DP_COMPLETE);
    osSetEventMesg(OS_EVENT_PRENMI, &gIntrMesgQueue, (OSMesg)MESG_NMI_REQUEST);
}

void AllocPool(void)
{
    void *start = (void *)SEG_POOL_START;
    void *end   = (void *)SEG_POOL_END;

    main_pool_init(start, end);
    D_8033A124 = mem_pool_init(0x4000, MEMORY_POOL_LEFT);
}

void create_thread(OSThread *thread, OSId id, void (*entry)(void *), void *arg,
    void *sp, OSPri pri)
{
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}

void handle_nmi_request(void)
{
    gResetTimer = 1;
    D_8032C648 = 0;
    func_80320890();
    func_803208C0(2, 890);
    func_802491FC(90);
}

static void receive_new_tasks(void)
{
    struct SPTask *spTask;

    while (osRecvMesg(&gSPTaskMesgQueue, (OSMesg *)&spTask, OS_MESG_NOBLOCK) != -1)
    {
        spTask->state = SPTASK_STATE_NOT_STARTED;
        switch (spTask->task.t.type)
        {
        case 2:
            sNextAudioSPTask = spTask;
            break;
        case 1:
            sNextDisplaySPTask = spTask;
            break;
        }
    }

    if (sCurrentAudioSPTask == NULL && sNextAudioSPTask != NULL)
    {
        sCurrentAudioSPTask = sNextAudioSPTask;
        sNextAudioSPTask = NULL;
    }

    if (sCurrentDisplaySPTask == NULL && sNextDisplaySPTask != NULL)
    {
        sCurrentDisplaySPTask = sNextDisplaySPTask;
        sNextDisplaySPTask = NULL;
    }
}

static void start_sptask(s32 taskType)
{
    UNUSED s32 pad;  // needed to pad the stack

    if (taskType == M_AUDTASK)
        gActiveSPTask = sCurrentAudioSPTask;
    else
        gActiveSPTask = sCurrentDisplaySPTask;

    osSpTaskLoad(&gActiveSPTask->task);
    osSpTaskStartGo(&gActiveSPTask->task);
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
}

static void interrupt_gfx_sptask(void)
{
    if (gActiveSPTask->task.t.type == M_GFXTASK)
    {
        gActiveSPTask->state = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

static void start_gfx_sptask(void)
{
    if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL && sCurrentDisplaySPTask->state == SPTASK_STATE_NOT_STARTED)
    {
        profiler_log_gfx_time(TASKS_QUEUED);
        start_sptask(M_GFXTASK);
    }
}

static void pretend_audio_sptask_done(void)
{
    gActiveSPTask = sCurrentAudioSPTask;
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
    osSendMesg(&gIntrMesgQueue, (OSMesg)MESG_SP_COMPLETE, OS_MESG_NOBLOCK);
}

static void handle_vblank(void)
{
    UNUSED s32 pad;  // needed to pad the stack

    Dummy802461EC();
    sNumVblanks++;
    if (gResetTimer > 0)
        gResetTimer++;

    receive_new_tasks();

    // First try to kick off an audio task. If the gfx task is currently
    // running, we need to asychronously interrupt it -- handle_sp_complete
    // will pick up on what we're doing and start the audio task for us.
    // If there is already an audio task running, there is nothing to do.
    // If there is no audio task available, try a gfx task instead.
    if (sCurrentAudioSPTask != NULL)
    {
        if (gActiveSPTask != NULL)
        {
            interrupt_gfx_sptask();
        }
        else
        {
            profiler_log_vblank_time();
            if (sAudioEnabled != 0)
                start_sptask(M_AUDTASK);
            else
                pretend_audio_sptask_done();
        }
    }
    else
    {
        if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL && sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED)
        {
            profiler_log_gfx_time(TASKS_QUEUED);
            start_sptask(M_GFXTASK);
        }
    }

    // Notify the game loop about the vblank.
    if (gVblankHandler1 != NULL)
        osSendMesg(gVblankHandler1->queue, gVblankHandler1->msg, OS_MESG_NOBLOCK);
    if (gVblankHandler2 != NULL)
        osSendMesg(gVblankHandler2->queue, gVblankHandler2->msg, OS_MESG_NOBLOCK);
}

static void handle_sp_complete(void)
{
    struct SPTask *curSPTask = gActiveSPTask;

    gActiveSPTask = NULL;

    if (curSPTask->state == SPTASK_STATE_INTERRUPTED)
    {
        // handle_vblank tried to start an audio task while there was already a
        // gfx task running, so it had to interrupt the gfx task. That interruption
        // just finished.
        if (osSpTaskYielded(&curSPTask->task) == 0)
        {
            // The gfx task completed before we had time to interrupt it.
            // Mark it finished, just like below.
            curSPTask->state = SPTASK_STATE_FINISHED;
            profiler_log_gfx_time(RSP_COMPLETE);
        }

        // Start the audio task, as expected by handle_vblank.
        profiler_log_vblank_time();
        if (sAudioEnabled != 0)
            start_sptask(M_AUDTASK);
        else
            pretend_audio_sptask_done();
    }
    else
    {
        curSPTask->state = SPTASK_STATE_FINISHED;
        if (curSPTask->task.t.type == M_AUDTASK)
        {
            // After audio tasks come gfx tasks.
            profiler_log_vblank_time();
            if (sCurrentDisplaySPTask != NULL && sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED)
            {
                if (sCurrentDisplaySPTask->state != SPTASK_STATE_INTERRUPTED)
                    profiler_log_gfx_time(TASKS_QUEUED);
                start_sptask(M_GFXTASK);
            }
            sCurrentAudioSPTask = NULL;
            if (curSPTask->msgqueue != NULL)
                osSendMesg(curSPTask->msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
        }
        else
        {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the task completely finished and
            // null out sCurrentDisplaySPTask. That happens in handle_dp_complete.
            profiler_log_gfx_time(RSP_COMPLETE);
        }
    }
}

static void handle_dp_complete(void)
{
    // Gfx SP task is completely done.
    if (sCurrentDisplaySPTask->msgqueue != NULL)
        osSendMesg(sCurrentDisplaySPTask->msgqueue, sCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    profiler_log_gfx_time(RDP_COMPLETE);
    sCurrentDisplaySPTask->state = SPTASK_STATE_FINISHED_DP;
    sCurrentDisplaySPTask = NULL;
}

static void thread3_main(UNUSED void *arg)
{
    setup_mesg_queues();
    AllocPool();
    load_engine_code_segment();

    create_thread(&gSoundThread, 4, thread4_sound, NULL, gThread4Stack + 0x2000, 20);
    osStartThread(&gSoundThread);

    create_thread(&gGameLoopThread, 5, thread5_game_loop, NULL, gThread5Stack + 0x2000, 10);
    osStartThread(&gGameLoopThread);

    while (1)
    {
        OSMesg msg;

        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((u32)msg)
        {
        case MESG_VI_VBLANK:
            handle_vblank();
            break;
        case MESG_SP_COMPLETE:
            handle_sp_complete();
            break;
        case MESG_DP_COMPLETE:
            handle_dp_complete();
            break;
        case MESG_START_GFX_SPTASK:
            start_gfx_sptask();
            break;
        case MESG_NMI_REQUEST:
            handle_nmi_request();
            break;
        }
        Dummy802461DC();
    }
}

void set_vblank_handler(s32 index, struct VblankHandler *handler, OSMesgQueue *queue, OSMesg *msg)
{
    handler->queue = queue;
    handler->msg = msg;

    switch (index)
    {
    case 1:
        gVblankHandler1 = handler;
        break;
    case 2:
        gVblankHandler2 = handler;
        break;
    }
}

static void SendMessage(OSMesg *msg)
{
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, msg, OS_MESG_NOBLOCK);
}

void dispatch_audio_sptask(struct SPTask *spTask)
{
    if (sAudioEnabled != 0 && spTask != NULL)
    {
        osWritebackDCacheAll();
        osSendMesg(&gSPTaskMesgQueue, (OSMesg) spTask, OS_MESG_NOBLOCK);
    }
}

void send_display_list(struct SPTask *spTask)
{
    if (spTask != NULL)
    {
        osWritebackDCacheAll();
        spTask->state = SPTASK_STATE_NOT_STARTED;
        if (sCurrentDisplaySPTask == NULL)
        {
            sCurrentDisplaySPTask = spTask;
            sNextDisplaySPTask = NULL;
            osSendMesg(&gIntrMesgQueue, (OSMesg)MESG_START_GFX_SPTASK, OS_MESG_NOBLOCK);
        }
        else
        {
            sNextDisplaySPTask = spTask;
        }
    }
}

static void turn_on_audio(void)
{
    sAudioEnabled = 1;
}

static void turn_off_audio(void)
{
    sAudioEnabled = 0;
    while (sCurrentAudioSPTask != NULL)
        ;
}

/**
 * Initialize hardware, start main thread, then idle.
 */
static void thread1_idle(UNUSED void *arg)
{
#if VERSION_US
    s32 sp24 = osTvType;
#endif

    osCreateViManager(OS_PRIORITY_VIMGR);
#if VERSION_US
    if (sp24 == TV_TYPE_NTSC)
        osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    else
        osViSetMode(&osViModeTable[OS_VI_PAL_LAN1]);
#else
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
#endif
    osViBlack(TRUE);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    create_thread(&gMainThread, 3, thread3_main, NULL, gThread3Stack + 0x2000, 100);
    if (D_8032C650 == 0)
        osStartThread(&gMainThread);
    osSetThreadPri(NULL, 0);

    // halt
    while (1)
        ;
}

void Main(void)
{
    UNUSED u8 pad[64];  // needed to pad the stack

    osInitialize();
    Dummy802461CC();
    create_thread(&gIdleThread, 1, thread1_idle, NULL, gIdleThreadStack + 0x800, 100);
    osStartThread(&gIdleThread);
}
