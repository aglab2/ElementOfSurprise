// This is a weird file where it cross-references another actor.
// Most likely, it was a compile time optimization due to the
// exact same DL being referenced.

// 0x0F000640

extern const Gfx bobomb_seg8_dl_08022D08_draw[];
const GeoLayout bowling_ball_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_SNUF_BALL, bobomb_seg8_dl_08022D08_draw),
   GEO_CLOSE_NODE(),
   GEO_END(),
};

// 0x0F00066C
const GeoLayout bowling_ball_track_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 170393),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_ALPHA, bobomb_seg8_dl_08022D08),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
