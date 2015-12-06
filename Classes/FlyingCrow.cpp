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
}

void FlyingCrow::op_config(FlyingCrowType type, cocos2d::Vec2 relativePosition)
{
    _crowType = type;
    const Vec2 init_pos = {0,100};
    Vec2 pos = init_pos + relativePosition;
    _hubNode->setPosition(pos.x, pos.y);

    if (type == FlyingCrowType::CT_SHIELD) {
        _pxShield->setVisible(true);
    }
    _speed = 10;
    _acce = type == FlyingCrowType::CT_ACCE ? 5 : 0;
}

bool FlyingCrow::op_dealWithArrow(ArrowUnit& arrow)
{
    return false;
}

void FlyingCrow::update(float dt)
{
    if (_alive) {
        _speed += _acce*dt;
        _hubNode->setPositionY(_hubNode->getPositionY() - _speed*dt);

    }
}

void FlyingCrow::toastDead()
{
    _alive = false;
    for (int i = 0; i < 8; i++) {
        _dxCrow->configStopActions(i);
    }
    const float dead_time = 1.f;
    const float fout = 0.5f;
    _hubNode->runAction(EaseIn::create(Spawn::create(RotateBy::create(dead_time, Vec3{0,-90,0}), MoveBy::create(dead_time, {0,0, -(_hubNode->getPositionZ())}), NULL), 2));
    auto p = _hubNode;
    _hubNode->scheduleOnce([p](float dt) {p->removeFromParent();}, dead_time+fout, fmt::sprintf("crow dead %d", random(0, 99999)));
}
