Vtx vcutm_dl_Cube_mesh_layer_1_vtx_cull[8] = {
	{{ {-100, -100, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 100, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, 100, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-100, -100, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, -100, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 100, 100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, 100, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {100, -100, -100}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx vcutm_dl_Cube_mesh_layer_1_vtx_0[24] = {
	{{ {100, 100, -100}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {-100, 100, -100}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {-100, 100, 100}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {100, 100, 100}, 0, {624, 240}, {0, 127, 0, 255} }},
	{{ {100, -100, 100}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {100, 100, 100}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {-100, 100, 100}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {-100, -100, 100}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {-100, -100, 100}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {-100, 100, 100}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {-100, 100, -100}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {-100, -100, -100}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {-100, -100, -100}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {100, -100, -100}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {100, -100, 100}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {-100, -100, 100}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {100, -100, -100}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {100, 100, -100}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {100, 100, 100}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {100, -100, 100}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {-100, -100, -100}, 0, {368, 752}, {0, 0, 129, 255} }},
	{{ {-100, 100, -100}, 0, {624, 752}, {0, 0, 129, 255} }},
	{{ {100, 100, -100}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {100, -100, -100}, 0, {368, 496}, {0, 0, 129, 255} }},
};

Gfx vcutm_dl_Cube_mesh_layer_1[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(vcutm_dl_Cube_mesh_layer_1_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
	gsSPLightColor(LIGHT_1, 0xFFFFFFFF),
	gsSPLightColor(LIGHT_2, 0x7F7F7FFF),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPVertex(vcutm_dl_Cube_mesh_layer_1_vtx_0 + 0, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsDPPipeSync(),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_CD_MAGICSQ | G_AD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

