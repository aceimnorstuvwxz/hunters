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
        _dxCrow->configAction(i, {0,0,-100}, {0,0,0}, 1, 1, RepeatForever::create(Sequence::create(DelayTime::create(ani_step*i), MoveBy::create(0, Vec3{0,0,100}), DelayTime::create(ani_step), MoveBy::create(0, Vec3{0,0,-100}), DelayTime::create(ani_step*(7-i)), NULL)));
    }
}


void FlyingCrow::op_configRelativePosition(cocos2d::Vec2 relativePosition)
{
    
}

bool FlyingCrow::op_dealWithArrow(ArrowUnit& arrow)
{
    return false;
}

void FlyingCrow::update(float dt)
{

}
