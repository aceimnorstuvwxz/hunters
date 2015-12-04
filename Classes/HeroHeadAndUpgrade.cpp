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
    initHeroThings();
    initTouchThings();
    initUpgradeThings();
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

void HeroHeadAndUpgrade::initUpgradeThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1,1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/upgrade/upgrade_rect.png.sopx");
        _hubNode->addChild(node);
        _pxUpgradeRect = node;
        node->setVisible(false);
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1,1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/upgrade/upgrade_btn.png.sopx");
        _hubNode->addChild(node);
        _pxUpgradeButton = node;
        node->setVisible(false);
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        float scaleBase = 1.5;
        node->setScale(scaleBase*0.6f,scaleBase*1.2f);
        node->setPosition3D({0,-45,1});
        node->configText("100",1);
        node->configMixColor({1.f, 255.f/255.f, 251.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptxUpgradeGold = node;
        node->setVisible(true);
    }

}

void HeroHeadAndUpgrade::initHeroThings()
{
    _huntingHero.init(_mainLayer, _mainCamera);
    _huntingHero.op_configHeroTypeAndDegree(HeroType::HT_META, 0);
    _huntingHero.op_hide();
    _huntingHero.op_move(HeroPositionType::HPT_OUT, true);
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
        } else if (_pxUpgradeRect->isVisible() && _pxUpgradeRect->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation())) {
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
                    MoneyManager::s()->cost(MoneyCostDef::C_ADD_HERO);
                    _heroHeadState = HeroHeadState::ALIVE;
                    _huntingHero.op_show();
                    _huntingHero.op_move(_heroPositionType, false);
                    int suitId = _huntingHero.op_fetchSuitId();
                    _pxHeadIcon->stopAllActions();
                    _pxBuyConfirm->stopAllActions();
                    _pxBuyConfirm->setVisible(false);
                    _pxHeadIcon->setVisible(true);
                    _pxHeadIcon->configSopx(fmt::sprintf("hunters/heros/%d.png.head.png.sopx", suitId));
                    ACSoundManage::s()->play(ACSoundManage::SN_ADD_HERO);

                } else {
                    //显示确认购买
                    const float delay_time = 2.5;
                    _pxHeadIcon->runAction(Sequence::create(Hide::create(), DelayTime::create(delay_time), Show::create(), NULL));
                    _pxBuyConfirm->runAction(Sequence::create(Show::create(), DelayTime::create(delay_time), Hide::create(), NULL));
                }
            } else if (_heroHeadState == HeroHeadState::ALIVE) {
                if (!_pxUpgradeRect->isVisible()) {
                    //显示升级
                    showUpgradeRect(true, 100, "sdfds");
                } else {

                }
            }

        }
    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);

}

void HeroHeadAndUpgrade::showUpgradeRect(bool enable, int howMuch, std::string iconSopx)
{
    const float delay_time = 2.2;
    auto ac = Sequence::create(Show::create(), DelayTime::create(delay_time), Hide::create(), NULL);
    _pxUpgradeRect->runAction(ac->clone());
    _pxUpgradeButton->runAction(ac->clone());
    _ptxUpgradeGold->runAction(ac);
    _ptxUpgradeGold->configText(fmt::sprintf("%d", howMuch), 0.8);

    Vec4 enableColor = {1.f, 200.f/255.f, 51.f/255.f,1.f};
    Vec4 disableCOlor = {0.5f,0.5f,0.5f,1.f};
    auto col = enable ? enableColor : disableCOlor;
    _pxUpgradeButton->configMixColor(col);
    _ptxUpgradeGold->configMixColor(col);
}


void HeroHeadAndUpgrade::op_configPosition(HeroPositionType position, bool direct) //设置位置
{
    const float x_start = -5;
    const float x_step = 2.2;
    float y = 4.7;
    float x = x_start + (3-static_cast<int>(position)) * x_step;
    if (direct) {
        _hubNode->setPosition3D({x,y,-10});
    } else {
        _hubNode->runAction(MoveTo::create(0.3, {x,y,-10}));

    }
    _heroPositionType = position;

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
