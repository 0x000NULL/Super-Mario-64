glabel jrb_geo_000AFC # 0x0E000AFC
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
         geo_camera_frustum 45, 100, 10000, geo_camera_fov
         geo_open_node
            geo_camera 16, 0, 2000, 6000, 0, 0, 0, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_OPAQUE, jrb_seg7_dl_0700EF00
               geo_display_list LAYER_ALPHA, jrb_seg7_dl_0700FE48
               geo_display_list LAYER_OPAQUE, jrb_seg7_dl_07010548
               geo_asm    0, Geo18_802D01E0
               geo_asm 4610, Geo18_802D104C
               geo_render_obj
               geo_asm    2, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
   geo_close_node
   geo_end
