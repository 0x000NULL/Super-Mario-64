glabel ssl_geo_0007CC # 0x0E0007CC
   geo_node_screen_area 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2
   geo_open_node
      geo_zbuffer 0
      geo_open_node
         geo_todo_09 100
         geo_open_node
            geo_background 0x0001
         geo_close_node
      geo_close_node
      geo_zbuffer 1
      geo_open_node
         geo_camera_frustum 45, 100, 12800, geo_camera_fov
         geo_open_node
            geo_camera 4, 0, 2000, 6000, 0, 0, 0, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_OPAQUE, ssl_seg7_dl_0701EE80
               geo_display_list LAYER_ALPHA, ssl_seg7_dl_0701F920
               geo_display_list LAYER_TRANSPARENT_DECAL, ssl_seg7_dl_0701FCE0
               geo_asm 2050, Geo18_802D2108
               geo_asm    0, Geo18_802D01E0
               geo_asm 2049, Geo18_802D1B70
               geo_asm 2050, Geo18_802D1B70
               geo_asm 2051, Geo18_802D1B70
               geo_render_obj
               geo_asm    0, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
   geo_close_node
   geo_end
