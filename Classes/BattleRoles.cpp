//
//  BattleRoles.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/4.
//
//

#include "BattleRoles.hpp"
#include "QuestDef.hpp"


void BattleRoles::init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    _hero.init(_mainLayer, _mainCamera);



    _rival.init(_mainLayer, _mainCamera);
}



void BattleRoles::op_heroConfig()
{
    //TODO 从其它设置获取 这个战斗单元的hero及ravil的图形配置

    _hero.op_config({{HeroGraphicType::HEAD,0}, {HeroGraphicType::BODY,0},{HeroGraphicType::SWORD,0},{HeroGraphicType::SHIELD,0}});
    _hero.op_setVisibility(false);
}

void BattleRoles::op_rivalConfig()
{
    //TODO 从其它设置获取 这个战斗单元的hero及ravil的图形配置
    _rival.op_config(0);
    _rival.op_setVisibility(false);
}

void BattleRoles::op_rivalMoveIn()
{
    _rival.op_setVisibility(true);
    _rival.op_move({0,QuestDef::POS_Y_ROLE_OUT + QuestDef::BATTLE_MOVE_IN_TIME * QuestDef::SCENE_MOVE_SPEED,0}, 0, true);
    _rival.op_rotate({90,0,0}, 0, true);

    _rival.op_move({0,QuestDef::POS_Y_ROLE,0}, QuestDef::BATTLE_MOVE_IN_TIME, false);
}

void BattleRoles::op_heroRunIn() //新关卡一开始，hero 从左边屏幕外跑入背景不动，跑到一定位置，停下，继续跑
{
    _hero.op_setVisibility(true);

    // hero从视野外跑进来
    _hero.op_move({0,-QuestDef::HERO_STAND_POS-QuestDef::SCENE_MOVE_SPEED*QuestDef::HERO_RUN_IN_TIME,0}, 0, true);
    _hero.op_rotate({90,0,-90}, 0, true);
    _hero.op_move({0,-QuestDef::HERO_STAND_POS,0}, QuestDef::HERO_RUN_IN_TIME, false);

//
//    _rival.op_setVisibility(true);
//    bool isFirstIn = false; //从其它地方获取
//    if (isFirstIn) {
//        // hero从视野外跑进来
//        _hero.op_move({0,-QuestDef::POS_Y_ROLE_OUT,0}, 0, true);
//        _hero.op_rotate({90,0,-90}, 0, true);
//        _hero.op_move({0,-QuestDef::POS_Y_ROLE,0}, 3.f, false);
//        _hero.op_run(3.f);
//
//        // rival站那里不动，在视野内
//        _rival.op_move({0,QuestDef::POS_Y_ROLE,0}, 0, true);
//        _rival.op_rotate({90,0,-90}, 0, true);
//    } else {
//        // hero原地跑
//        _hero.op_run(3.f);
//        _hero.op_move({0,-QuestDef::POS_Y_ROLE,0}, 0, true);
//        _hero.op_rotate({90,0,-90}, 0, true);
//
//        // rival从右侧移进来，但不跑
//        _rival.op_move({0,QuestDef::POS_Y_ROLE_OUT,0}, 0, true);
//        _rival.op_rotate({90,0,-90}, 0, true);
//
//        _rival.op_move({0,QuestDef::POS_Y_ROLE,0}, 3.f, false);
//    }
}

void BattleRoles::op_switchToBattle() //告知镜头转向battle，此时rival需要转过身来。hero则要不再显示。
{
//    _rival.op_rotate({90,0,0}, QuestDef::CAMERA_MOVE_TIME);
    _hero.op_rotate({90,0,180}, QuestDef::CAMERA_MOVE_TIME);

}
void BattleRoles::op_switchToForward() //
{

}
void BattleRoles::op_rivalDie() //播放rival的死亡动画
{

    _rival.op_die();

}
void BattleRoles::op_rivalBeAttack() //播放rival被攻击的动作
{
    _rival.op_damage();

}
void BattleRoles::op_rivalBeHealth()
{
    _rival.op_health();
}
void BattleRoles::op_rivalAttack() //播放rival发动攻击的动作
{

}
void BattleRoles::op_rivalStartDefense() //rival举盾
{

}

void BattleRoles::op_rivalStopDefense()
{

}

void BattleRoles::op_heroVisible(bool enable)
{
    _hero.op_setVisibility(enable);
}