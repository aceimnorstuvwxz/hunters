//
//  HeroHeadAndUpgrade.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/26.
//
//

#include "HeroHeadAndUpgrade.hpp"

#include "format.h"
#include "intersection.h"
#include "SOCommon.h"

USING_NS_CC;

void HeroHeadAndUpgrade::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initHeadThings();
    initTouchThings();
}


void HeroHeadAndUpgrade::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,0,-10});
    _hubNode->setScale(0.08);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(10);
    _mainCamera->addChild(_hubNode);
}

void HeroHeadAndUpgrade::initHeadThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1,1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/heros/head_add.png.sopx");
        _hubNode->addChild(node);
        _pxHeadIcon = node;
    }
}

void HeroHeadAndUpgrade::initTouchThings()
{
    auto listener = EventListenerTouchOneByOne::create();
    static bool moved = false;
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        moved = false;
        auto rect = _pxHeadIcon->fetchScreenRect(0, _mainCamera);
        if (rect.containsPoint(touch->getLocation())) {
            CCLOG("head icon touch in");
            return true;
        }
        return false;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){
        moved = true;

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){
    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);

}

void HeroHeadAndUpgrade::op_configPosition(HeroPositionType position, bool direct) //设置位置
{
    const float x_start = -5;
    const float x_step = 2.2;
    float y = 4.7;
    float x = x_start + static_cast<int>(position) * x_step;
    if (direct) {
        _hubNode->setPosition3D({x,y,-10});
    } else {
        _hubNode->runAction(MoveTo::create(0.3, {x,y,-10}));

    }

}

HeroPositionType HeroHeadAndUpgrade::op_fetchPosition() //获取位置
{
    return _heroPositionType;
}

void HeroHeadAndUpgrade::update(float dt)
{

}