// (C) 2015 arisecbf

#ifndef QuestDef_hpp
#define QuestDef_hpp

#include <stdio.h>
#include "cocos2d.h"


class QuestDef
{
public:
    constexpr static const float POS_Y_ROLE = 100; //战斗时Hero/Rival的战位置离中心的距离
    constexpr static const float POS_Y_ROLE_OUT = 110; //视野外位置

    constexpr static const float HERO_STAND_POS = 60;
    constexpr static const float SCENE_MOVE_SPEED = 50;

    constexpr static const float ROLE_MOVE_TIME = 3.f; //hero跑入或者rival移入的时间

    constexpr static const float CAMERA_MOVE_TIME = 1.f; //镜头旋转的时间
    constexpr static const float CAMERA_FORWARD_EXPAND = 150; //forward状态镜头在X方向离中间的距离

    constexpr static const float BEAT_LINE_WIDTH = 20; //战区每条栈道的宽度
    constexpr static const float BEAT_RIVAL_START_LINE = 80; //新beat出现的位置
    constexpr static const float BEAT_RIVAL_HIT_LINE = 80; //beat击中rival的位置
    constexpr static const float BEAT_HERO_HIT_LINE = -85; //beat击中hero的位置
    constexpr static const float BEAT_HERO_TAP_LINE = -75; //beat被tap的位置
    constexpr static const float BEAT_COME_HEIGHT = 5; //在过来的剑的高度
    constexpr static const float BEAT_TAP_AREA_HEIGHT = 5;
    constexpr static const float BEAT_BACK_HEIGHT = 4; //击回的剑的高度

    constexpr static const float BEAT_SHOWUP_TIME = 0.5f; //beat出现时的出现过程时间


    constexpr static const float SCREEN_RELATIVE_TAP_TOP = 0.33;//触摸区域在屏幕2D上的位置
    constexpr static const float SCREED_RELATIVE_TAP_BOTTOM = 0.19;
    constexpr static const float TAP_AREA_HALF_WIDTH = 2.5;

    constexpr static const float TAP_TOAST_TIME = 0.05f;
    constexpr static const float BEAT_CLEAR_TIME = 0.35;

    constexpr static const int COMBO_BASE_AMOUNT = 20;
    constexpr static const float HERO_RUN_IN_TIME = 2.0;
    constexpr static const float HERO_RUN_IN_TIME2 = 1.0; //原地跑，树后退

    constexpr static const float BATTLE_MOVE_IN_TIME = 2.0; //第一人称视角，跑这个距离，怪物就到位了
    constexpr static const float HERO_RUN_UNIT_TIME = 0.033;
    constexpr static const float HERO_RUN_HEIGHT = 5;
    constexpr static const float BEAT_BACK_RADIO = 2.0;

    constexpr static const float HUNTING_X_START = -140;
    constexpr static const float HUNTING_X_HEROPOS_BEGIN = -125;
    constexpr static const float HUNTING_X_HEROPOS_STEP = 10;
    constexpr static const float HUNTING_X_END = 140;
    constexpr static const float MONSTER_APPEAR_POS = 145;
    constexpr static const float ARROW_SHOOT_Z = 10;
    constexpr static const float ARROW_SHOOT_Y = 5;
    constexpr static const float ARROW_SCALE = 0.5;
    constexpr static const float BOW_TOAST_STEP = 0.5;
    constexpr static const float CASTLE_POS = -150;

    constexpr static const int INIT_HEART = 20;
    constexpr static const float GRAVITY = 50;
    
    


};


#endif /* QuestDef_hpp */
