// 0x0C0001E4
extern Gfx yoshi_egg_seg5_dl_draw[];
const GeoLayout yoshi_egg_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_YOSHIEGG, yoshi_egg_seg5_dl_draw),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
