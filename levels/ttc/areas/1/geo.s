glabel ttc_geo_0003B8 # 0x0E0003B8
   geo_node_screen_area 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2
   geo_open_node
      geo_zbuffer 0
      geo_open_node
         geo_todo_09 100
         geo_open_node
            geo_background 0xC7FF
         geo_close_node
      geo_close_node
      geo_zbuffer 1
      geo_open_node
         geo_camera_frustum 45, 100, 12800, geo_camera_fov
         geo_open_node
            geo_camera 2, 0, 2000, 6000, 0, 0, 0, geo_camera_preset_and_pos
            geo_open_node
               geo_asm    0, Geo18_802D01E0
               geo_asm 5120, Geo18_802D2108
               geo_asm 5121, Geo18_802D2108
               geo_display_list LAYER_OPAQUE, ttc_seg7_dl_0700AD38
               geo_display_list LAYER_TRANSPARENT, ttc_seg7_dl_0700B1D8
               geo_display_list LAYER_ALPHA, ttc_seg7_dl_0700E878
               geo_render_obj
               geo_asm    0, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
   geo_close_node
   geo_end
