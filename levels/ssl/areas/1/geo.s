glabel ssl_geo_000648 # 0x0E000648
   geo_node_screen_area 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2
   geo_open_node
      geo_zbuffer 0
      geo_open_node
         geo_todo_09 100
         geo_open_node
            geo_background BACKGROUND_DESERT, geo_skybox_main
         geo_close_node
      geo_close_node
      geo_zbuffer 1
      geo_open_node
         geo_camera_frustum 45, 100, 20000, geo_camera_fov
         geo_open_node
            geo_camera 1, 0, 2000, 6000, -2048, 0, -1024, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_OPAQUE, ssl_seg7_dl_07009F48
               geo_display_list LAYER_OPAQUE, ssl_seg7_dl_0700BA78
               geo_display_list LAYER_ALPHA, ssl_seg7_dl_0700BC18
               geo_display_list LAYER_TRANSPARENT_DECAL, ssl_seg7_dl_0700BD00
               geo_asm 2049, Geo18_802D2108
               geo_asm    0, Geo18_802D01E0
               geo_asm 2049, Geo18_802D104C
               geo_asm 2129, Geo18_802D104C
               geo_asm 2049, Geo18_802D1CDC
               geo_asm 2050, Geo18_802D1CDC
               geo_asm 2051, Geo18_802D1CDC
               geo_render_obj
               geo_asm    0, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
      geo_zbuffer 0
      geo_open_node
         geo_asm 0, Geo18_802CD1E8
      geo_close_node
   geo_close_node
   geo_end
