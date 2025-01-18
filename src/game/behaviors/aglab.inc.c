void bhv_movie_loop()
{
    if (100 == o->oTimer)
    {
        gMarioStates->action = 0;
        play_sound(SOUND_MENU_THANK_YOU_PLAYING_MY_GAME, gGlobalSoundSource);
    }
}
