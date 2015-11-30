//
//  HuntingHero.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/24.
//
//

#include "HuntingMonster.hpp"

#include "format.h"
#include "intersection.h"
#include "HuntingHeroMetaSuitManage.hpp"

USING_NS_CC;
void HuntingMonster::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initMonsterThings();
}

void HuntingMonster::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,0,0});
    _hubNode->setScale(0.15);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(9);
    _hubNode->setRotation3D({90,0,-90});
    _mainLayer->addChild(_hubNode);
    //    _hubNode->setVisible(false);
}

void HuntingMonster::initMonsterThings()
{
    _dpxNode = DynamicPixelNode::create();
    _dpxNode->setPosition3D({0,0,0});
    _hubNode->addChild(_dpxNode);
    _dpxNode->setCameraMask(_mainCamera->getCameraMask(), true);
}
int HuntingMonster::boneIndexType2sopxId(int boneIndexType)
{
    int r = 0;
    switch (boneIndexType) {
        case BT_HEAD:
            r = 2;
            break;

        case BT_HAIR:
            r = 0;
            break;

        case BT_BODY:
            r = 5;
            break;

        case BT_HAND_L:
            r = 4;
            break;

            //        case BT_HAND_R:
            //            r = 6;
            //            break;

        case BT_LEG_L:
            r = 8;
            break;

        case BT_LEG_R:
            r = 8;
            break;
            
        default:
            break;
    }
    return r;
}

void HuntingMonster::op_configType(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level) // 由英雄类型和等级 来指定穿着套装和弓
{

    _generalType = generalType;
    _specialType = specialType;
    _hasShield = hasShield;
    _level = level;

    //会清空 VBO 后，重新写数据
    _dpxNode->configClear();

    int suitId = 0;


    // 穿衣服
    for (int i = 0; i < BT_SWORD_MAX; i++) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/monsters/%d.png.%d.so.png.sopx", suitId, boneIndexType2sopxId(i)), i, boneIndex2relativePosition(i));
    }

    // 穿上剑
    _dpxNode->configAddSopx(fmt::sprintf("hunters/swords/%d.png.sopx", level), BT_SWORD_MAX, boneIndex2relativePosition(BT_SWORD_MAX));

    // 盾
    if (_hasShield) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/sopx/monster_shield.png.sopx"), BT_SHIELD, boneIndex2relativePosition(BT_SHIELD));
    }

    _hubNode->setCameraMask(_mainCamera->getCameraMask(), true);
    ani_moving(1);

}


cocos2d::Vec3 HuntingMonster::boneIndex2relativePosition(int boneIndexType)
{
    Vec3 r = {0,0,0};
    switch (boneIndexType) {

        case BT_HEAD:
            r = {0,10,0};
            break;

        case BT_HAIR:
            r = {0,-20.f/3,0};
            break;

        case BT_LEG_L:
        case BT_LEG_R:
            r = {2.f/3,-7.f/3,0};
            break;
        default:
            break;
    }
    return r;
}

ACPositionScaleRotation HuntingMonster::help_calcBonePosition(int boneIndexType)
{
    Vec3 r = {0,0,0};
    const float head_pos = 70;

    switch (boneIndexType) {

        case BT_HEAD:
            r =  {0,head_pos-30,0};
            break;

        case BT_HAIR:
            r = {-20,head_pos-15,-0.5};
            break;
        case BT_BODY:
            r = {-10,head_pos-47,0};
            break;

        case BT_LEG_L:
            r = {-13-2,head_pos-48,0};
            break;
        case BT_LEG_R:
            r = {-3-2,head_pos-48,0};
            break;
        case BT_HAND_L:
            r = {-10,head_pos-40,2.0};
            break;
        case BT_SWORD_MAX:
            r ={-10+4*3,head_pos-20-4*3,1};
            break;
        case BT_SHIELD:
            r = {-10+4*3,head_pos-20-4*3,1};
            break;

        default:
            assert(false);
            break;
    }
    return {r,1,1,Vec3{0,0,0}};
}

void HuntingMonster::ani_moving(float radio)
{
    //设置各 bone 的动作
    float head_pos = 70;

    const float run_time = 0.6;
    const float run_length = 8;
    const float angle_forward = 15;
    const float angle_back = 105;
    auto bodyMove =Sequence::create(MoveBy::create(run_time*0.5, {0,5,0}), MoveBy::create(run_time*0.5, {0,-5,0}), NULL);

    _dpxNode->configAction(BT_HEAD, help_calcBonePosition(BT_HEAD).position, {0,0,0}, 1.f*3, 1.f*3,RepeatForever::create(Sequence::create(MoveBy::create(run_time*0.5, {-2,6,0}), MoveBy::create(run_time*0.5, {2,-6,0}), NULL)));

    _dpxNode->configAction(BT_HAIR, help_calcBonePosition(BT_HAIR).position, {0,0,15}, 0.9f*3,0.9f*3, RepeatForever::create(Spawn::create( bodyMove->clone(), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,15}),RotateBy::create(run_time*0.5, Vec3{0,0,-15}), NULL), NULL)));

    _dpxNode->configAction(BT_BODY, help_calcBonePosition(BT_BODY).position, {0,0,10}, 1.f*3,1.f*3, RepeatForever::create(bodyMove->clone()));


    _dpxNode->configAction(BT_LEG_L, {-8,head_pos-48,0}, {0,0,-angle_forward}, 1.f*3,1.f*3, RepeatForever::create(Spawn::create(
                                                                                                            Sequence::create(                                                                                                                             MoveBy::create(run_time, {-run_length,0,0}), MoveBy::create(run_time, {run_length,0,0}), NULL),
                                                                                                            Sequence::create(                                                                                                                             RotateBy::create(run_time, {0,0,(angle_back+angle_forward)}), RotateBy::create(run_time, {0,0,-(angle_back+angle_forward)}), NULL)
                                                                                                            ,NULL)));

    _dpxNode->configAction(BT_LEG_R, {-8,head_pos-48,0}, {0,0,angle_back}, 1.f*3,1.f*3, RepeatForever::create(Spawn::create(
                                                                                                             Sequence::create(  MoveBy::create(run_time, {run_length,0,0}),                                                                                                                            MoveBy::create(run_time, {-run_length,0,0}), NULL),
                                                                                                             Sequence::create(   RotateBy::create(run_time, {0,0,-(angle_back+angle_forward)}),                                                                                                                           RotateBy::create(run_time, {0,0,(angle_back+angle_forward)}), NULL)
                                                                                                             ,NULL)));

    _dpxNode->configAction(BT_HAND_L, help_calcBonePosition(BT_HAND_L).position, {0,0,0}, 1.0f*3,1.0f*3, RepeatForever::create(Spawn::create( bodyMove->clone(), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,-35}),RotateBy::create(run_time*0.5, Vec3{0,0,35}), NULL), NULL)));

    _dpxNode->configAction(BT_SWORD_MAX, help_calcBonePosition(BT_SWORD_MAX).position, {0,0,100-45}, 3.f,3.f, RepeatForever::create(RepeatForever::create(Spawn::create( bodyMove->clone(), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,-15}),RotateBy::create(run_time*0.5, Vec3{0,0,15}), NULL), NULL))));
}
