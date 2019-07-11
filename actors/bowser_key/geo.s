glabel bowser_key2_geo # 0x16000A84
   geo_shadow SHADOW_CIRCLE_4_VERTS, 0x9B, 300
   geo_open_node
      geo_scale 0x00, 65536
      geo_open_node
         geo_display_list LAYER_OPAQUE, bowser_key_seg3_dl_030161F8
      geo_close_node
   geo_close_node
   geo_end

glabel bowser_key_geo # 0x16000AB0
   geo_scale 0x00, 16384
   geo_open_node
      geo_dl_translated LAYER_OPAQUE, 0, 0, 0
      geo_open_node
         geo_dl_translated LAYER_OPAQUE, 0, 0, 0
         geo_open_node
            geo_asm 0, Geo18_802BA2B0
            geo_scale 0x00, 65536
            geo_open_node
               geo_display_list LAYER_OPAQUE, bowser_key_seg3_dl_030161F8
            geo_close_node
         geo_close_node
      geo_close_node
   geo_close_node
geo_close_node #! more close than open nodes
geo_end
