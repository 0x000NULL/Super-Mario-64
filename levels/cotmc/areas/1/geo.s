glabel cotmc_geo_0001A0 # 0x0E0001A0
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
            geo_camera 16, 0, 2000, 6000, 0, 0, 0, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_OPAQUE, cotmc_seg7_dl_07007D48
               geo_display_list LAYER_OPAQUE, cotmc_seg7_dl_0700A160
               geo_asm     0, Geo18_802D01E0
               geo_asm 10241, Geo18_802D1B70
               geo_display_list LAYER_TRANSPARENT, cotmc_seg7_dl_0700A4B8
               geo_render_obj
               geo_asm 0, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
   geo_close_node
   geo_end
