glabel castle_grounds_geo_00073C # 0x0E00073C
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
         geo_camera_frustum 45, 100, 20000, geo_camera_fov
         geo_open_node
            geo_camera 16, 0, 1500, 2500, 0, 1500, -12000, geo_camera_preset_and_pos
            geo_open_node
               geo_display_list LAYER_OPAQUE, castle_grounds_seg7_dl_07006D70
               geo_display_list LAYER_OPAQUE, castle_grounds_seg7_dl_070095F0
               geo_display_list LAYER_ALPHA, castle_grounds_seg7_dl_0700A860
               geo_display_list LAYER_TRANSPARENT_DECAL, castle_grounds_seg7_dl_0700B1D0
               geo_display_list LAYER_ALPHA, castle_grounds_seg7_dl_0700BA20
               geo_display_list LAYER_OPAQUE_DECAL, castle_grounds_seg7_dl_0700C430
               geo_display_list LAYER_OPAQUE, castle_grounds_seg7_dl_0700C210
               geo_asm    0, Geo18_802D01E0
               geo_asm 5633, Geo18_802D1B70
               geo_asm 5633, Geo18_802D104C
               geo_render_obj
               geo_asm 0, geo_exec_level
            geo_close_node
         geo_close_node
      geo_close_node
      geo_zbuffer 0
      geo_open_node
         geo_asm 0, Geo18_802CD1E8
      geo_close_node
   geo_close_node
   geo_end
