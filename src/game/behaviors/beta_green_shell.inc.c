// beta_green_shell.c.inc

void bhv_beta_green_shell_init(void)
{
    o->oGravity = 2.5;
    o->oFriction = 0.8;
    o->oBuoyancy = 1.3;
}

void BetaGreenShellDropped(void)
{
    obj_enable_rendering();
    
    obj_get_dropped();
    
    o->oHeldState = HELD_FREE;
    o->oForwardVel = 0;
    o->oVelY = 0;
}

void BetaGreenShellThrown(void)
{
    obj_enable_rendering_2();
    
    obj_enable_rendering();
    
    o->oHeldState = HELD_FREE;
    o->oFlags &= ~0x8; /* bit 3 */
    o->oForwardVel = 40.0;
    o->oVelY = 20.0;
}

void bhv_beta_green_shell_loop(void)
{
    switch (o->oHeldState)
    {
        case HELD_FREE:
            ObjectStep();
            break;
            
        case HELD_HELD:
            obj_disable_rendering();
            break;
            
        case HELD_THROWN:
            BetaGreenShellThrown();
            break;
            
        case HELD_DROPPED:
            BetaGreenShellDropped();
            break;
    }
}
