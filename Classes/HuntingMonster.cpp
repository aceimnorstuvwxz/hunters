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
    _hubNode->setRotation3D({90,0,90});
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

    // 穿衣服
    for (int i = 0; i < BT_SWORD_MAX; i++) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/monsters/%d.tga.png.%d.so.png.sopx", level, boneIndexType2sopxId(i)), i, boneIndex2relativePosition(i), true, false);
    }

    // 穿上剑
    _dpxNode->configAddSopx(fmt::sprintf("hunters/swords/%d.tga.png.sopx", level), BT_SWORD_MAX, boneIndex2relativePosition(BT_SWORD_MAX), true, false);

    // 盾
    if (_hasShield) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/shields/%d.png.sopx", 0), BT_SHIELD, boneIndex2relativePosition(BT_SHIELD), true, false);
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

        case BT_SWORD_MAX:
            r = {10.f,10.f,0};
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
    float sx = 3.f;
    float sy = 3.f;
    float rz = 0;

    switch (boneIndexType) {

        case BT_STEADY:
            r =  {0,head_pos-100,-2};
            break;

        case BT_HEAD:
            r =  {0,head_pos-30,-1};
            break;

        case BT_HAIR:
            r = {-20,head_pos-15,-0.5};
            sx = sy = 0.9f*3.f;
            break;
        case BT_BODY:
            r = {-10,head_pos-47,0};
            rz = 3;
            break;

        case BT_LEG_L:
            r = {-13-2,head_pos-48,0};
            break;
        case BT_LEG_R:
            r = {-3-2,head_pos-48,0};
            break;
        case BT_HAND_L:
            r = {-10,head_pos-40,-1.0};
            rz = -70;
            break;
        case BT_SWORD_MAX:
            r ={-10+4*3,head_pos-35,1};
            break;
        case BT_SHIELD:
            r = {-1+4*3,head_pos-30-4*3,-1.5};
            break;

        default:
            assert(false);
            break;
    }
    return {r,sx,sy,Vec3{0,0,rz}};
}

void HuntingMonster::ani_moving(float radio)
{
    //设置各 bone 的动作
    float head_pos = 70;

    const float run_time = 2.0;
    const float run_length = 8;
    const float angle_forward = 25;
    const float angle_back = 25;
    auto bodyMove =Sequence::create(MoveBy::create(run_time*0.5, {0,2,0}), MoveBy::create(run_time*0.5, {0,-2,0}), NULL);

    auto cfg = help_calcBonePosition(BT_HEAD);
    _dpxNode->configAction(BT_HEAD, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Sequence::create(MoveBy::create(run_time*0.5, {-2,4,0}), MoveBy::create(run_time*0.5, {2,-4,0}), NULL)));

    cfg = help_calcBonePosition(BT_HAIR);
    _dpxNode->configAction(BT_HAIR, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Spawn::create( bodyMove->clone(), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,15}),RotateBy::create(run_time*0.5, Vec3{0,0,-15}), NULL), NULL)));

    cfg = help_calcBonePosition(BT_BODY);
    _dpxNode->configAction(BT_BODY, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(bodyMove->clone()));

    _dpxNode->configAction(BT_LEG_L, {-5+0.5f*run_length,head_pos-48,0}, {0,0,-angle_forward}, 1.f*3,1.f*3, RepeatForever::create(Spawn::create(
                                                                                                            Sequence::create(                                                                                                                             MoveBy::create(run_time, {-run_length,0,0}), MoveBy::create(run_time, {run_length,0,0}), NULL),
                                                                                                            Sequence::create(                                                                                                                             RotateBy::create(run_time, {0,0,(angle_back+angle_forward)}), RotateBy::create(run_time, {0,0,-(angle_back+angle_forward)}), NULL)
                                                                                                            ,NULL)));

    _dpxNode->configAction(BT_LEG_R, {-5-0.5f*run_length,head_pos-48,0}, {0,0,angle_back}, 1.f*3,1.f*3, RepeatForever::create(Spawn::create(
                                                                                                             Sequence::create(  MoveBy::create(run_time, {run_length,0,0}),                                                                                                                            MoveBy::create(run_time, {-run_length,0,0}), NULL),
                                                                                                             Sequence::create(   RotateBy::create(run_time, {0,0,-(angle_back+angle_forward)}),                                                                                                                           RotateBy::create(run_time, {0,0,(angle_back+angle_forward)}), NULL)
                                                                                                             ,NULL)));

    cfg = help_calcBonePosition(BT_HAND_L);
    _dpxNode->configAction(BT_HAND_L, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Spawn::create( Sequence::create(MoveBy::create(run_time*0.5, {2,4,0}), MoveBy::create(run_time*0.5, {-2,-4,0}), NULL), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,-10}),RotateBy::create(run_time*0.5, Vec3{0,0,10}), NULL), NULL)));

    cfg = help_calcBonePosition(BT_SWORD_MAX);
    _dpxNode->configAction(BT_SWORD_MAX, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(RepeatForever::create(Spawn::create( bodyMove->clone(), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,-3}),RotateBy::create(run_time*0.5, Vec3{0,0,3}), NULL), NULL))));

    cfg = help_calcBonePosition(BT_SHIELD);
    _dpxNode->configAction(BT_SHIELD, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Sequence::create(MoveBy::create(run_time*0.5, {2,4,0}), MoveBy::create(run_time*0.5, {-2,-4,0}), NULL)));
}

void HuntingMonster::ani_attacking()
{
    //设置各 bone 的动作
    float head_pos = 70;

    const float run_time = 2.0;
    const float run_length = 8;
    const float angle_forward = 25;
    const float angle_back = 25;
    const float attack_interval = 1.f;
    auto bodyMove =Sequence::create(MoveBy::create(run_time*0.5, {0,2,0}), MoveBy::create(run_time*0.5, {0,-2,0}), NULL);

    auto cfg = help_calcBonePosition(BT_HEAD);
    _dpxNode->configAction(BT_HEAD, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Sequence::create(MoveBy::create(run_time*0.5, {-2,4,0}), MoveBy::create(run_time*0.5, {2,-4,0}), NULL)));

    cfg = help_calcBonePosition(BT_HAIR);
    _dpxNode->configAction(BT_HAIR, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Spawn::create( bodyMove->clone(), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,15}),RotateBy::create(run_time*0.5, Vec3{0,0,-15}), NULL), NULL)));

    cfg = help_calcBonePosition(BT_BODY);
    _dpxNode->configAction(BT_BODY, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(bodyMove->clone()));

    cfg = help_calcBonePosition(BT_LEG_L);
    _dpxNode->configAction(BT_LEG_L, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, DelayTime::create(0.1));
    cfg = help_calcBonePosition(BT_LEG_R);
    _dpxNode->configAction(BT_LEG_R, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, DelayTime::create(0.1));

    cfg = help_calcBonePosition(BT_HAND_L);
    _dpxNode->configAction(BT_HAND_L, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Spawn::create( Sequence::create(MoveBy::create(run_time*0.5, {2,4,0}), MoveBy::create(run_time*0.5, {-2,-4,0}), NULL), Sequence::create( RotateBy::create(run_time*0.5, Vec3{0,0,-10}),RotateBy::create(run_time*0.5, Vec3{0,0,10}), NULL), NULL)));

    cfg = help_calcBonePosition(BT_SWORD_MAX);
    _dpxNode->configAction(BT_SWORD_MAX, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Sequence::create(RotateBy::create(0.2, 90),RotateBy::create(0.2, -90), DelayTime::create(attack_interval - 0.4), NULL)));

    cfg = help_calcBonePosition(BT_SHIELD);
    _dpxNode->configAction(BT_SHIELD, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Sequence::create(MoveBy::create(run_time*0.5, {2,4,0}), MoveBy::create(run_time*0.5, {-2,-4,0}), NULL)));
}
void HuntingMonster::op_toastAttack() //攻击
{
    ani_attacking();
}
void HuntingMonster::op_toastUnderAttack() //播放被攻击动画，变白
{
    _dpxNode->configMixColorAni({1,1,1,1}, 0.2);
}

ACPositionScaleRotation HuntingMonster::help_boneDeadGesture(int boneIndex)
{

    Vec3 r = {0,0,0};
    float sx = 3.f;
    float sy = 3.f;
    float rx = 0;
    float ry = 0;
    float rz = 0;

    switch (boneIndex) {

        case BT_STEADY:
            r =  {0,0,-2};
            break;

        case BT_HEAD:
            r =  {0,10,-1};
            rz = 90;
            break;

        case BT_HAIR:
            r = {0,-100,-0.5};
            sx = sy = 0.9f*3.f;
            break;
        case BT_BODY:
            r = {0,10,0};
            rz = 3;
            break;

        case BT_LEG_L:
            r = {-10,20,0};
            break;

        case BT_LEG_R:
            r = {10,20,0};
            break;
        case BT_HAND_L:
            r = {50,10,-1.0};
            rz = -90;
            break;
        case BT_SWORD_MAX:
            r ={70,50,1};
            rz = 45+90;
            break;
        case BT_SHIELD:
            r = {0,0,1};
            break;

        default:
            assert(false);
            break;
    }
    r.x = r.x-15;
    return {r,sx,sy,Vec3{rx,ry,rz}};

}
void HuntingMonster::op_toastDead(cocos2d::Vec2 direction) //播放死亡，散架了，坠落到地上，过一会儿后消失
{
    float xradio = direction.x/direction.y;
    float ground_y = 0;
    //散架处理
    for (int i = 0; i < BT_MAX; i++) {
        auto cfg = help_calcBonePosition(i);
        auto dfg = help_boneDeadGesture(i);
        _dpxNode->configAction(i, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, EaseIn::create( Spawn::create(MoveTo::create(0.5, {dfg.position.x + cfg.position.x+xradio*(cfg.position.y-ground_y), dfg.position.y, dfg.position.z}), RotateTo::create(0.5, dfg.rotation), NULL), 1.f));
    }
}

void HuntingMonster::update(float dt)
{
    const float move_speed = 2;
    _hubNode->setPositionY(_hubNode->getPositionY() - move_speed*dt);
}
