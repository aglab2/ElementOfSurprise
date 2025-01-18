static struct Object* kTDMap[10][10];

enum TowerTypes
{
    TOWER_FIRE,
    TOWER_WATER,
    TOWER_CRYSTAL,
    TOWER_AIR,
};

struct TowerBeh
{
    int model;
    const BehaviorScript* bhv;
};

static struct TowerBeh kBasicTowerBehs[] = {
    { MODEL_FLYGUY       , bhvFireTower }, // AOE traps
    { MODEL_MR_BLIZZARD  , bhvWaterTower }, // Slowing down
    { MODEL_SNUFIT       , bhvCrystalTower }, // Very far range, slow, high damage
    { MODEL_MONTY_MOLE   , bhvAirTower }, // Very fast, low damage
};

static struct TowerBeh kComboTowerBehs[4][4] = {
    [TOWER_FIRE] = {
        { MODEL_BOWSER, bhvFireTower },
        { MODEL_BOO   , bhvSteamTower },
        { MODEL_BLARGG, bhvSpireTower },
        { MODEL_ENEMY_LAKITU, bhvInfernoTower },
    },
    [TOWER_WATER] = {
        { MODEL_BOO      , bhvSteamTower },
        { MODEL_PENGUIN  , bhvWaterTower },
        { MODEL_MANTA_RAY, bhvShardTower },
        { MODEL_YOSHI    , bhvHurricaneTower },
    },
    [TOWER_CRYSTAL] = {
        { MODEL_BLARGG   , bhvSpireTower },
        { MODEL_MANTA_RAY, bhvShardTower },
        { MODEL_HEAVE_HO  , bhvCrystalTower },
        { MODEL_MR_I     , bhvMrI },
    },
    [TOWER_AIR] = {
        { MODEL_ENEMY_LAKITU, bhvInfernoTower },
        { MODEL_YOSHI , bhvHurricaneTower },
        { MODEL_MR_I  , bhvMrI },
        { MODEL_UKIKI , bhvAirTower },
    },
};

static enum TowerTypes kSelectedTowerType = TOWER_FIRE;

#define TD_BUSY ((void*) 1)
#define TD_CAN_SELL(o) ((int)o < 0)

void bhv_td_init()
{
    for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
    {
        kTDMap[i][j] = NULL;
    }

    kTDMap[0][0] = TD_BUSY;
    for (int i = 3; i < 10; i++)
        kTDMap[i][0] = TD_BUSY;

    kTDMap[0][1] = TD_BUSY;
    for (int i = 3; i < 10; i++)
        kTDMap[i][1] = TD_BUSY;

    kTDMap[0][2] = TD_BUSY;
    kTDMap[9][2] = TD_BUSY;

    kTDMap[0][3] = TD_BUSY;
    for (int i = 3; i < 7; i++)
        kTDMap[i][3] = TD_BUSY;
    kTDMap[9][3] = TD_BUSY;

    kTDMap[0][4] = TD_BUSY;
    kTDMap[3][4] = TD_BUSY;
    kTDMap[6][4] = TD_BUSY;
    kTDMap[9][4] = TD_BUSY;

    for (int i = 0; i < 4; i++)
        kTDMap[i][5] = TD_BUSY;
    kTDMap[6][5] = TD_BUSY;
    kTDMap[9][5] = TD_BUSY;

    kTDMap[6][6] = TD_BUSY;
    kTDMap[9][6] = TD_BUSY;

    for (int i = 0; i < 7; i++)
        kTDMap[i][7] = TD_BUSY;
    kTDMap[9][7] = TD_BUSY;

    kTDMap[0][8] = TD_BUSY;
    kTDMap[9][8] = TD_BUSY;

    for (int i = 0; i < 10; i++)
        kTDMap[i][9] = TD_BUSY;

    gMarioStates->numCoins = 50;
}

void bhv_td_loop()
{
    gMarioStates->pos[1] = 200.f;
    gMarioStates->action = ACT_DEBUG_FREE_MOVE;

    gCamera->cutscene = CUTSCENE_TD;

    const int dist = 127.f;
    const int marioX = (int) gMarioStates->pos[0] + dist * 10;
    const int marioZ = (int) gMarioStates->pos[2] + dist * 10;

    int i = marioX / dist / 2;
    int j = marioZ / dist / 2;
 
#if 1
    o->oPosX = i * dist * 2 - dist * 9;
    o->oPosY = 0;
    o->oPosZ = j * dist * 2 - dist * 9;
#else
    o->oPosX = gMarioStates->pos[0];
    o->oPosY = gMarioStates->pos[1];
    o->oPosZ = gMarioStates->pos[2];
#endif

    if (TD_BUSY == kTDMap[i][j])
    {
        cur_obj_hide();
        return;
    }
    else
    {
        cur_obj_unhide();
    }


}
