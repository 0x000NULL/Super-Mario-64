glabel wdw_geo_000724 # 0x0E000724
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
            geo_camera 1, 0, 2000, 6000, -4352, 0, -4352, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_OPAQUE, wdw_seg7_dl_07011E48
               geo_display_list LAYER_ALPHA, wdw_seg7_dl_07012258
               geo_display_list LAYER_TRANSPARENT_DECAL, wdw_seg7_dl_07012908
               geo_asm    0, Geo18_802D01E0
               geo_asm 4354, Geo18_802D104C
               geo_render_obj
               geo_asm    0, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
   geo_close_node
   geo_end
