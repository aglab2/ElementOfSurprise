// 0x17000230
extern const Gfx water_splash_seg4_dl_render[];
const GeoLayout water_splash_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_SPLASH, water_splash_seg4_dl_render),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
