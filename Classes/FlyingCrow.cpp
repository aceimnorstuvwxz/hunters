//
//  FlyingCrow.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/6.
//
//

#include "FlyingCrow.hpp"
#include "format.h"
#include "intersection.h"
#include "HuntingHeroMetaSuitManage.hpp"
#include "QuestDef.hpp"
#include "SOCommon.h"
#include "ACSoundManage.hpp"

USING_NS_CC;
void FlyingCrow::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initCrowThings();
}

void FlyingCrow::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,0,30});
    _hubNode->setScale(0.5);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(9);
    _hubNode->setRotation3D({90,0,-90});
    _mainLayer->addChild(_hubNode);
//    _hubNode->setVisible(false);
}

void FlyingCrow::initCrowThings()
{

    _dxCrow = DynamicPixelNode::create();
    _dxCrow->setPosition3D({0,0,0});
    _hubNode->addChild(_dxCrow);
    _dxCrow->setCameraMask(_mainCamera->getCameraMask(), true);
    _dxCrow->configBlend(true);

    for (int i = 0; i < 8; i++) {
        _dxCrow->configAddSopx(fmt::sprintf("hunters/flycrow/%d.png.ps.png.sopx", i), i, {0,0,0}, true, true);
    }

    const float ani_step = 0.1;
    for (int i = 0; i < 8; i++) {
        _dxCrow->configAction(i, {0,0,-1000}, {0,0,0}, 1, 1, RepeatForever::create(Sequence::create(DelayTime::create(ani_step*i), MoveBy::create(0, Vec3{0,0,1000}), DelayTime::create(ani_step), MoveBy::create(0, Vec3{0,0,-1000}), DelayTime::create(ani_step*(7-i)), NULL)));
    }


    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/flycrow/shield.png.sopx");
        node->configBlend(true);
        node->setVisible(false);
        _hubNode->addChild(node);
        _pxShield = node;
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,-5,0});
        node->configSopx("hunters/flycrow/crow_shit.png.sopx");
        node->configBlend(true);
        node->setVisible(false);
        _hubNode->addChild(node);
        _pxCrowShit = node;
    }
}

void FlyingCrow::op_config(bool acce, bool shield, cocos2d::Vec2 relativePosition)
{
    const Vec2 init_pos = {170,60};
    Vec2 pos = init_pos + relativePosition;
    _hubNode->setPosition3D({0, pos.x, pos.y});

    if (shield) {
        _pxShield->setVisible(true);
    }
    _speed = 10;
    _acce = acce ? 5 : 0;
}

bool FlyingCrow::op_dealWithArrow(ArrowUnit& arrow)
{
    Vec2 pos_arrow = {arrow._pxNode->getPositionY(), arrow._pxNode->getPositionZ()};
    Vec2 pos_crow =  {_hubNode->getPositionY(), _hubNode->getPositionZ()};
    Vec2 len_diff = pos_crow-pos_arrow;
    float crow_rlen = _pxShield->isVisible() ? 14:8;
    if (len_diff.length() < crow_rlen) {
        arrow._leftHitTimes--;
        //hit
        if (_pxShield->isVisible()) {
            _pxShield->configMixColorAni({1,1,1,1}, 0.2, 2);
            _pxShield->runAction(Sequence::create(DelayTime::create(0.2*2), Hide::create(), NULL));
            ACSoundManage::s()->play(ACSoundManage::SN_HIT_SHIELD);
        } else {
            _dxCrow->configMixColorAni({1,1,1,1}, 0.2, 2);
            this->toastDead();
            ACSoundManage::s()->play(ACSoundManage::SN_ARROW_NORMAL_HIT);

            return true;
        }
    }

    return false;
}

void FlyingCrow::op_configPaused(bool pause)
{
    if (pause) {
        _dxCrow->pause();
    } else {
        _dxCrow->resume();
    }
}

void FlyingCrow::update(float dt)
{
    if (_alive) {
        _speed += _acce*dt;
        _hubNode->setPositionY(_hubNode->getPositionY() - _speed*dt);
        if (_need2cry && _hubNode->getPositionY() < 100) {
            _need2cry = false;
            ACSoundManage::s()->play(ACSoundManage::SN_CROW_CRY);
        }
        if (_acce > 0) {
            _particleManage->toastPartles(GlobalParticleType::CROW_ACCE, {_hubNode->getPositionY(), _hubNode->getPositionZ()}, {0,0}, 0);
        }

        //不下落
//        if (_hubNode->getPositionY() < -100) {
//            _pxCrowShit->setVisible(true);
//            _shitSpeed += dt*QuestDef::GRAVITY;
//            _pxCrowShit->setPositionY(_pxCrowShit->getPositionY()-_shitSpeed*dt);
//        }
        if (_hubNode->getPositionY() < QuestDef::EMEMY_ATTACK_POS) {
            //hit!
            _topIconsProtocal->op_minusHeart();
            toastDead(false);
        }

    }
}

void FlyingCrow::toastDead(bool needDown)
{
    _alive = false;
    for (int i = 0; i < 8; i++) {
        _dxCrow->configStopActions(i);
    }
    const float dead_time = 1.f;
    const float fout = 0.5f;
    if (needDown) {
    _hubNode->runAction(EaseIn::create(Spawn::create(RotateBy::create(dead_time, Vec3{0,-90,0}), MoveBy::create(dead_time, {0,0, -(_hubNode->getPositionZ())}), NULL), 2));
    }
    auto p = _hubNode;
    _hubNode->scheduleOnce([p](float dt) {p->removeFromParent();}, dead_time+fout, fmt::sprintf("crow dead %d", random(0, 99999)));
}
