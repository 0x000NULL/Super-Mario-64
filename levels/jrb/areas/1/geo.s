glabel jrb_geo_000A18 # 0x0E000A18
   geo_node_screen_area 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2
   geo_open_node
      geo_zbuffer 0
      geo_open_node
         geo_todo_09 100
         geo_open_node
            geo_background BACKGROUND_ABOVE_CLOUDS, geo_skybox_main
         geo_close_node
      geo_close_node
      geo_zbuffer 1
      geo_open_node
         geo_camera_frustum 45, 100, 25000, geo_camera_fov
         geo_open_node
            geo_camera 16, 0, 2000, 6000, 0, 0, 0, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_ALPHA, jrb_seg7_dl_07002FD0
               geo_display_list LAYER_OPAQUE, jrb_seg7_dl_07004940
               geo_display_list LAYER_OPAQUE, jrb_seg7_dl_07004C78
               geo_display_list LAYER_OPAQUE, jrb_seg7_dl_070058C8
               geo_display_list LAYER_OPAQUE, jrb_seg7_dl_070069B0
               geo_display_list LAYER_OPAQUE, jrb_seg7_dl_07007570
               geo_display_list LAYER_ALPHA, jrb_seg7_dl_07007718
               geo_asm    0, Geo18_802D01E0
               geo_asm 4609, Geo18_802D104C
               geo_asm 4613, Geo18_802D104C
               geo_render_obj
               geo_asm   14, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
      geo_zbuffer 0
      geo_open_node
         geo_asm 0, Geo18_802CD1E8
      geo_close_node
   geo_close_node
   geo_end
