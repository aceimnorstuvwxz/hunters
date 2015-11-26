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
#include "MoneyCostDef.hpp"
#include "MoneyManager.hpp"

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
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1,1);
        node->setPosition3D({0,0,1});
        node->configSopx("hunters/heros/head_confirm.png.sopx");
        _hubNode->addChild(node);
        _pxBuyConfirm = node;
        _pxBuyConfirm->setVisible(false);
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
        if (!moved) {
            //没有移动，则是普通点击
            if (_heroHeadState == HeroHeadState::EMPTY && MoneyManager::s()->get() >= MoneyCostDef::C_ADD_HERO) {
                if (_pxBuyConfirm->isVisible()) {
                    //确认购买
                    _heroHeadState = HeroHeadState::ALIVE;

                } else {
                    //显示确认购买
                    const float delay_time = 2.5;
                    _pxHeadIcon->runAction(Sequence::create(Hide::create(), DelayTime::create(delay_time), Show::create(), NULL));
                    _pxBuyConfirm->runAction(Sequence::create(Show::create(), DelayTime::create(delay_time), Hide::create(), NULL));
                }
            }

        }
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

void HeroHeadAndUpgrade::op_tellGoldChange() //被通知金币改变
{
    if (_heroHeadState == HeroHeadState::EMPTY) {
        if (MoneyManager::s()->get() >= MoneyCostDef::C_ADD_HERO) {
            _pxHeadIcon->configMixColor({0.5,0.5,0.5,1.0});
        } else {
            _pxHeadIcon->configMixColor({0,0,0,0});
        }
    }
}

HeroPositionType HeroHeadAndUpgrade::op_fetchPosition() //获取位置
{
    return _heroPositionType;
}

void HeroHeadAndUpgrade::update(float dt)
{

}