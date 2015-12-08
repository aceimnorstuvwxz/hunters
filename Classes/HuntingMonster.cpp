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
#include "QuestDef.hpp"
#include "MoneyManager.hpp"

USING_NS_CC;

int HuntingMonster::_idGen = 0;

void HuntingMonster::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initMonsterThings();
    initGoldThings();
    _id = _idGen++;
}

void HuntingMonster::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,QuestDef::MONSTER_APPEAR_POS,0});
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

void HuntingMonster::initGoldThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(4.f);
        node->setPosition3D({0,100,0});
        node->configSopx("hunters/sopx/icon-gold.png.sopx");
        _hubNode->addChild(node);
        _pxGold = node;
        node->setVisible(false);
    }

}

void HuntingMonster::toastGold()
{
    _pxGold->runAction(Sequence::create(Show::create(), Spawn::create(MoveBy::create(0.5, {0,30.f/huntingMonsterGeneralType2scale(_generalType),0}), RotateBy::create(0.5, {0,360,0}),  NULL),Hide::create(), NULL));
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

void HuntingMonster::op_configType(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level, int shieldCount) // 由英雄类型和等级 来指定穿着套装和弓
{

    _generalType = generalType;
    _specialType = specialType;
    _hasShield = hasShield;
    _shieldCount = shieldCount;
    _level = level;

    //会清空 VBO 后，重新写数据
    _dpxNode->configClear();

    _hubNode->setScale(huntingMonsterGeneralType2scale(generalType)*0.15f);

    // 穿衣服
    for (int i = 0; i < BT_SWORD_MAX; i++) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/monsters/%d.tga.png.%d.so.png.sopx", level, boneIndexType2sopxId(i)), i, boneIndex2relativePosition(i), true, false);
    }

    // 穿上剑
    _dpxNode->configAddSopx(fmt::sprintf("hunters/swords/%d.tga.png.sopx", level), BT_SWORD_MAX, boneIndex2relativePosition(BT_SWORD_MAX), true, false);

    // 盾
    if (_hasShield) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/monsters/shield.png.sopx", 0), BT_SHIELD, boneIndex2relativePosition(BT_SHIELD), true, false);
        _dpxNode->configBlend(true);
    }

    _steadyScale = QuestDef::ARROW_SCALE/0.15f/huntingMonsterGeneralType2scale(generalType);
    _dpxNode->configAction(BT_STEADY, {0,0,0}, {0,180,0}, _steadyScale, _steadyScale, DelayTime::create(0.5));

    //blood
    _dpxNode->configAddSopx("hunters/sopx/blood_line.png.sopx", BT_BLOOD, {0,0,0}, true, false);
    _dpxNode->configAction(BT_BLOOD, {0,120,0}, {0,0,0}, 60, 3/huntingMonsterGeneralType2scale(generalType), DelayTime::create(0));

//    //circle
//    _dpxNode->configAddSopx(fmt::sprintf("hunters/sopx/foot_circle_monster.png.sopx", 0), BT_CIRCLE, {0,0,0}, true, false);
//    _dpxNode->configAction(BT_CIRCLE, {0,0,0}, {90,0,0}, 2, 2, DelayTime::create(0));


    _hubNode->setCameraMask(_mainCamera->getCameraMask(), true);
    ani_moving(1);

    _bloodMax = _bloowNow = calcBloodMax(generalType, level);
    refreshBloodLine();

    _pxGold->setScale(4.f/huntingMonsterGeneralType2scale(_generalType));


}

void HuntingMonster::refreshBloodLine()
{
    float radio = _bloowNow/_bloodMax;
    _dpxNode->configAction(BT_BLOOD, {+60*(1.f-radio),120,0}, {0,0,0}, 60*radio, 3/huntingMonsterGeneralType2scale(_generalType), DelayTime::create(0));

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
        case BT_SHIELD:
            r = {0,10,0};
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
            r =  {0,0,0};
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
            r = {0,head_pos-30-4*3,-1.5};
            sx = sy = 2.5f;
            break;

        case BT_BLOOD:
            r = {0,0,0};
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

    _steadyScale = QuestDef::ARROW_SCALE/0.15f/huntingMonsterGeneralType2scale(_generalType);
    _dpxNode->configAction(BT_STEADY, {0,0,0}, {0,180,0}, _steadyScale, _steadyScale, RepeatForever::create(bodyMove->clone()));

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
    _dpxNode->configAction(BT_SHIELD, _hasShield ? cfg.position : Vec3{0,0,1000}, cfg.rotation, cfg.scaleX, cfg.scaleY, RepeatForever::create(Sequence::create(MoveBy::create(run_time*0.5, {2,4,0}), MoveBy::create(run_time*0.5, {-2,-4,0}), NULL)));
}

void HuntingMonster::ani_attacking()
{
    //设置各 bone 的动作
//    float head_pos = 70;

    const float run_time = 2.0;
//    const float run_length = 8;
//    const float angle_forward = 25;
//    const float angle_back = 25;
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

void HuntingMonster::closeShield()
{
    _dpxNode->configAction(BT_SHIELD, {0,0,0}, {0,0,0}, 0, 0, DelayTime::create(0));
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
            r =  {0,-1000,200};
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


        case BT_BLOOD:
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
    if (direction.y == 0) {
        direction.y = 0.01;
    }
    if (direction.y > 0) {
        direction.y = -direction.y;
    }
    if (std::abs(direction.x) > std::abs(direction.y)) {
        direction.x = (direction.x < 0 ? -1 : 1)* std::abs(direction.y);
    }
    float xradio = direction.x/direction.y;
    float ground_y = 0;
    //散架处理
    for (int i = 0; i < BT_MAX; i++) {
        auto cfg = help_calcBonePosition(i);
        auto dfg = help_boneDeadGesture(i);
        if (i != BT_SHIELD) {


        _dpxNode->configAction(i, cfg.position, cfg.rotation, cfg.scaleX, cfg.scaleY, EaseIn::create( Spawn::create(MoveTo::create(0.5, {dfg.position.x + cfg.position.x+xradio*(cfg.position.y-ground_y), dfg.position.y, dfg.position.z}), RotateTo::create(0.5, dfg.rotation), NULL), 1.f));
            }
    }
}

int HuntingMonster::op_getId()
{
    return _id;
}

void HuntingMonster::op_dealWithArrow(ArrowUnit& arrow)
{
    Vec2 pos_arrow = {arrow._pxNode->getPositionY(), arrow._pxNode->getPositionZ()};
    float pos_monster = _hubNode->getPositionY();

    float typescale = huntingMonsterGeneralType2scale(_generalType);
    float x_expand = 0.5f*20*typescale*0.15;
    float y_height = 20*4*typescale*0.15f;
    float shiled_r = 0.9f*y_height;
    Vec2 centerPos = {pos_monster,  y_height*0.6f};
    if ( (_hasShield &&  (centerPos-pos_arrow).length() < shiled_r) || //盾碰撞
        (pos_arrow.x > pos_monster-x_expand && pos_arrow.x < pos_monster+x_expand && pos_arrow.y > 0 &&
        pos_arrow.y < y_height) //普通碰撞
        ) {
        //击中
        //检查是否重复
        bool duplicate = false;
        for (auto id : arrow._hitedMonsterIds) {
            if (id == this->_id) {
                //重复击中
                duplicate = true;break;
            }
        }

        if (!duplicate) {
            bool needApplyArrow = true; /*!(arrow._type == HuntingArrowType::BOMB_0 ||
                                    arrow._type == HuntingArrowType::BOMB_1 ||
                                    arrow._type == HuntingArrowType::BOMB_2 ||
                                    arrow._type == HuntingArrowType::SLOW_2);*/
            bool isThrough = arrow._leftHitTimes > 1;
            //真的击中了
            arrow._leftHitTimes--;
            arrow._hitedMonsterIds.push_back(_id);

            //护盾
            bool shielded = false;
            if (_hasShield) {
                if (arrow._type == HuntingArrowType::SLOW_2 ||
                    arrow._type == HuntingArrowType::BOMB_0 ||
                     arrow._type == HuntingArrowType::BOMB_1 ||
                    arrow._type == HuntingArrowType::BOMB_2){}
                else {
                    _shieldCount--;
                }
                if (_shieldCount == 0) {
                    _hasShield = false;
                    closeShield();
                }
                shielded = true;
            }


            //附箭或穿透效果
            if (needApplyArrow && !shielded) {
                applyEffectArrow(arrow, isThrough);
            }

            //通用效果（根据 arrow 速度和方向的击退效果，受伤闪白）
            op_toastUnderAttack();
            float pushBackDiff = arrow._speed.x*0.01f;
            _hubNode->setPositionY(_hubNode->getPositionY() + pushBackDiff);

            float dama = calcArrowDamage(arrow._type);
            bool nodamage = false;

            //特殊效果
            if (arrow._type == HuntingArrowType::SLOW_0 && !shielded) {
                _slowDownTime = 3.f;
                _slowDownRate = 0.65f;
            } else if (arrow._type == HuntingArrowType::SLOW_1 && !shielded) {
                _slowDownTime = 4.f;
                _slowDownRate = 0.45f;
            } else if (arrow._type == HuntingArrowType::SLOW_2) {
                // 特殊 雷击 范围
                _effetcManageProtocal->op_thunder(pos_arrow);
                nodamage = true;
            } else if (arrow._type == HuntingArrowType::MULTI_2 && !shielded) {
                _poisonTime = 8.f;
                _poisonDamage = 0.5f * dama;
            } else if (arrow._type == HuntingArrowType::BOMB_0 ||
                       arrow._type == HuntingArrowType::BOMB_1 ||
                       arrow._type == HuntingArrowType::BOMB_2) {

                _effetcManageProtocal->op_bomb(arrow._type == HuntingArrowType::BOMB_0 ? 0 :
                                               arrow._type == HuntingArrowType::BOMB_1 ? 1 : 2, pos_arrow);
                nodamage = true;
            }



            //激光
            int num = _floatingLaserManageProtocal->op_fetchLaserNumber();
            _floatingLaserManageProtocal->op_toastLaser(pos_arrow);

            //声音
            if (num > 0) {
                ACSoundManage::s()->play(ACSoundManage::SN_LASER_HIT);
            }
            if (shielded) {
                ACSoundManage::s()->play(ACSoundManage::SN_HIT_SHIELD);
            } else {
                ACSoundManage::s()->play(ACSoundManage::SN_ARROW_NORMAL_HIT);
            }
            //伤害计算
            if (!nodamage && !shielded) {
                damage( (1+num)*dama, arrow._speed);
            }

        }
    }
}

void HuntingMonster::damage(float damage, cocos2d::Vec2 dir)
{
    if (_bloowNow <= 0 ) return;
    //伤害计算
    _bloowNow -= damage;
    if (_bloowNow <= 0) {
        _alive = false;
        _bloowNow = 0;

        //金币
        toastGold();
        MoneyManager::s()->add(calcMonsterGold(_generalType, _level));

        //能量
        _energyBarProtocal->op_addEnergy(calcMonsterEnergy(_generalType));

        //死亡
        op_toastDead(dir);
        auto p = _hubNode;
        _mainCamera->scheduleOnce([p](float dt){
            p->removeFromParent();
        }, 1.f, fmt::sprintf("monster dead %d", random(0, 999999999)));
    }

    refreshBloodLine();
}


void HuntingMonster::op_thunderTest(float pos)
{
    if (std::abs(_hubNode->getPositionY() - pos) < 30) {

        if (_hasShield) {
            _shieldCount--;
            if (_shieldCount == 0) {
                _hasShield = false;
                closeShield();
            }
            ACSoundManage::s()->play(ACSoundManage::SN_HIT_SHIELD);
        } else {
            damage(calcArrowDamage(HuntingArrowType::SLOW_2), {0, -1});
            _slowDownTime = 3.f;
            _slowDownRate = 0.f;
            _dpxNode->configMixColor({102.f/255.f,0.f/255.f,139.f/255.f,0.5f});
            auto p = _dpxNode;
            _dpxNode->scheduleOnce([p](float dt) {p->configMixColor({0,0,0,0,});}, _slowDownTime, fmt::sprintf("sdfs %d", random(0, 99999)));
        }

    }
}

void HuntingMonster::op_bombTest(float pos, int grade)
{
    if (std::abs(_hubNode->getPositionY() - pos) < (grade <= 1 ? 30 : 45)) {
        if (_hasShield) {
            _shieldCount--;

            if (_shieldCount == 0) {
                _hasShield = false;
                closeShield();
            }
            ACSoundManage::s()->play(ACSoundManage::SN_HIT_SHIELD);
        } else {
            damage(calcArrowDamage(grade == 0 ? HuntingArrowType::BOMB_0 : grade == 1 ? HuntingArrowType::BOMB_1 :HuntingArrowType::BOMB_2), {0, -1});

            if (grade >= 1) {
                _hubNode->setPositionY(_hubNode->getPositionY() + 0.5f*(pos - _hubNode->getPositionY()));
            }
        }
    }
}

void HuntingMonster::applyEffectArrow(ArrowUnit& arrow, bool isThrough)
{
    //算位置和方向
    Vec2 pos_arrow = {arrow._pxNode->getPositionY(), arrow._pxNode->getPositionZ()};
    float pos_monster = _hubNode->getPositionY();

    float pxScale = huntingMonsterGeneralType2scale(_generalType);
    float x_pos = (pos_arrow.x - pos_monster)/0.15f/pxScale/_steadyScale;
    float y_pos = pos_arrow.y/0.15f/pxScale/_steadyScale;


    //附箭或穿透效果
    if (!isThrough) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/arrows/c%02d.png.sopx", static_cast<int>(arrow._type)), BT_STEADY, {x_pos,y_pos,5},true, false, {0,0,45-vector2angel(arrow._speed)});
    } else {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/arrows/through.png.sopx", static_cast<int>(arrow._type)), BT_STEADY, {x_pos,y_pos,5},true, false, {0,0,45-vector2angel(arrow._speed)});
    }

}

void HuntingMonster::update(float dt)
{
    float move_speed = 4;
    if (_slowDownTime >0) {
        _slowDownTime -= dt;
        move_speed = move_speed*_slowDownRate;
    }
    _hubNode->setPositionY(_hubNode->getPositionY() - move_speed*dt);

    if (_hubNode->getPositionY() < QuestDef::EMEMY_ATTACK_POS) {
        //hit!
        _topIconsProtocal->op_minusHeart();
        _alive = false;
        _hubNode->removeFromParent();
    }
}


