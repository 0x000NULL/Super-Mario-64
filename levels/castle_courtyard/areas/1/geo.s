glabel castle_courtyard_geo_000218 # 0x0E000218
   geo_node_screen_area 10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2
   geo_open_node
      geo_zbuffer 0
      geo_open_node
         geo_todo_09 100
         geo_open_node
            geo_background BACKGROUND_OCEAN_SKY, geo_skybox_main
         geo_close_node
      geo_close_node
      geo_zbuffer 1
      geo_open_node
         geo_camera_frustum 45, 100, 12800, geo_camera_fov
         geo_open_node
            geo_camera 16, 0, 2000, 6000, 0, 0, 0, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_OPAQUE, castle_courtyard_seg7_dl_070048B8
               geo_display_list LAYER_TRANSPARENT_DECAL, castle_courtyard_seg7_dl_07005698
               geo_display_list LAYER_ALPHA, castle_courtyard_seg7_dl_07005938
               geo_asm    0, Geo18_802D01E0
               geo_asm 9729, Geo18_802D104C
               geo_render_obj
               geo_asm 0, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
   geo_close_node
   geo_end
