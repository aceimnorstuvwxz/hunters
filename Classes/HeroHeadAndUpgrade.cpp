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
    initTransferThings();

}


void HeroHeadAndUpgrade::initHubThings()
{
    {
        auto node = Node::create();
        node->setPosition3D({0,0,-10});
        node->setScale(0.08);
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setZOrder(10);
        _mainCamera->addChild(node);
        _hubNode = node;
    }

    {
        auto node = Node::create();
        node->setPosition3D({0,0,-10});
        node->setScale(0.1);
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setZOrder(10);
        _mainCamera->addChild(node);
        _transferHubNode = node;
    }
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


    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        float scaleBase = 0.6;
        node->setScale(scaleBase*1.f,scaleBase*1.f);
        node->setPosition3D({-3.5,-8,0.2});
        node->configText("lv1",1);
        node->configMixColor({257.f/255.f, 240.f/255.f, 0.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptxLevel = node;
        node->setVisible(false);
    }


    

    {
        auto node = RoadPlane::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,0,-11});
        node->setScale(1000.f);
        //    node->setRotation3D({90,0,-90});
        _mainCamera->addChild(node);
        node->configImage("hunters/upgrade/dark_shadow.png");
        _darkShadow = node;
        _darkShadow->setOpacity(100);
        _darkShadow->configBlend(true);
        _darkShadow->setVisible(false);
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
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8,0.8);
        node->setPosition3D({0,-24,0});
        node->configSopx("hunters/upgrade/icons/META_0.png.sopx");
        _hubNode->addChild(node);
        _pxUpgradeIcon = node;
        node->setVisible(false);
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        float scaleBase = 1.5;
        node->setScale(scaleBase*0.6f,scaleBase*1.2f);
        node->setPosition3D({1.5f,-45,1});
        node->configText("100",1);
        node->configMixColor({1.f, 255.f/255.f, 251.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptxUpgradeGold = node;
        node->setVisible(false);
    }

}

void HeroHeadAndUpgrade::initTransferThings()
{
    std::string textTitles[] = {"aela","ayne","yeta","fayne"};
    std::string descLine0[] = {    "slow",    "very",     "toast",   "binding"};
    std::string descLine1[] = {    "down",     "high",     "multiple", "with"};
    std::string descLine2[] = {    "enemies",       "damage",   "arrows",   "bombs"};
    std::vector<std::string*> descs = {descLine0, descLine1, descLine2};

    const float location_scale = 45.f;
    for (int i = 0; i < 4; i++) {
        {
            auto node = PixelNode::create();
            node->setCameraMask(_mainCamera->getCameraMask());
            node->setScale(1,1);
            node->setPosition3D({(1.f*i-1.5f)*location_scale,0.f,0.f});
            node->configSopx(fmt::sprintf("hunters/upgrade/transfer_rect_%d.png.sopx", i+4));
            _transferHubNode->addChild(node);
//            node->setVisible(false);
            _transfers[i]._pxRect = node;
        }

        {
            auto node = PixelTextNode::create();
            node->setCameraMask(_mainCamera->getCameraMask());
            float scaleBase = 1.0f;
            node->setScale(scaleBase*0.7f,scaleBase*1.f);
            node->setPosition3D({(i-1.5f)*location_scale,25,0});
            node->configText(textTitles[i],1);
            node->configMixColor({1.f, 255.f/255.f, 251.f/255.f,1.f});
            _transferHubNode->addChild(node);
            _transfers[i]._ptxTitle= node;
        }
        for (int j = 0; j < 3; j++) {
            {
                const float j_scale = 7;
                auto node = PixelTextNode::create();
                node->setCameraMask(_mainCamera->getCameraMask());
                float scaleBase = 1.0f;
                node->setScale(scaleBase*0.7f,scaleBase*1.f);
                node->setPosition3D({(i-1.5f)*location_scale,-10 - j * j_scale,0});
                node->configText(descs[j][i],1);
                node->configMixColor({1.f, 255.f/255.f, 251.f/255.f,1.f});
                _transferHubNode->addChild(node);
                _transfers[i]._ptxDesctibe[j]= node;
            }
        }
    }
    _transferHubNode->setVisible(false);
}


void HeroHeadAndUpgrade::showTransfer()
{
    _questSceneProtocal->op_configPaused(true);
    _darkShadow->setVisible(true);
    _transferHubNode->setVisible(true);
}

void HeroHeadAndUpgrade::dismissTransfer()
{
    _questSceneProtocal->op_configPaused(false);
    _darkShadow->setVisible(false);
    _transferHubNode->setVisible(false);
}

void HeroHeadAndUpgrade::initHeroThings()
{
    _huntingHero.init(_mainLayer, _mainCamera);
    _huntingHero.op_configHeroTypeAndGrade(HeroType::HT_META, 0);
    _huntingHero.op_hide();
    _huntingHero.op_move(HeroPositionType::HPT_OUT, true);
}


void HeroHeadAndUpgrade::initTouchThings()
{
    auto listener = EventListenerTouchOneByOne::create();
    static bool moved = false;
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        moved = false;/*
        auto rect = _pxHeadIcon->fetchScreenRect(0, _mainCamera);
        if (rect.containsPoint(touch->getLocation())) {
            CCLOG("head icon touch in");
            return true;
        } else if (_pxUpgradeRect->isVisible() && _pxUpgradeRect->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation())) {
            return true;
        }*/
        return true;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){
        moved = true;

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){
        if (!moved) {
            //没有移动，则是普通点击
            if (_pxHeadIcon->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation()) && _heroHeadState == HeroHeadState::EMPTY && MoneyManager::s()->get() >= MoneyCostDef::C_ADD_HERO) {
                if (_pxBuyConfirm->isVisible()) {
                    //确认购买
                    _heroHeadState = HeroHeadState::ALIVE;
                    MoneyManager::s()->cost(MoneyCostDef::C_ADD_HERO);
                    _huntingHero.op_show();
                    _huntingHero.op_move(_heroPositionType, false);
                    int suitId = _huntingHero.op_fetchSuitId();
                    _pxHeadIcon->stopAllActions();
                    _pxBuyConfirm->stopAllActions();
                    _pxBuyConfirm->setVisible(false);
                    _pxHeadIcon->setVisible(true);
                    _pxHeadIcon->configSopx(fmt::sprintf("hunters/heros/%d.png.head.png.sopx", suitId));
                    ACSoundManage::s()->play(ACSoundManage::SN_ADD_HERO);
                    _ptxLevel->setVisible(true);
                    _ptxLevel->configText(heroTypeGrade2levelString(_heroType,_heroLevel));
                    _powerBarProtocal->op_firstHeroPosition(_heroPositionType);
                    ACSoundManage::s()->play(ACSoundManage::SN_NEW_HERO);
                    _tutorialManageProtocal->op_toastAttack();
                } else {
                    //显示确认购买
                    const float delay_time = 2.5;
                    _pxHeadIcon->runAction(Sequence::create(Hide::create(), DelayTime::create(delay_time), Show::create(), NULL));
                    _pxBuyConfirm->runAction(Sequence::create(Show::create(), DelayTime::create(delay_time), Hide::create(), NULL));
                    _pxBuyConfirm->configMixColor((MoneyManager::s()->get() >= MoneyCostDef::C_ADD_HERO) ? Vec4{0,0,0,0} : Vec4{0.5f,0.5f,0.5f,1.f});
                    _tutorialManageProtocal->op_toastAddMoreHerosDone();

                }
            } else if (_heroHeadState == HeroHeadState::ALIVE) {
                if (_transferHubNode->isVisible()) {
                    for (int i = 0; i < 4; i++) {
                        if (_transfers[i]._pxRect->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation()) )
                        {

                            _heroType = i == 0 ? HeroType::HT_SLOW_DOWN : i == 1 ? HeroType::HT_HIGH_ATTACK : i == 2 ? HeroType::HT_MULTI_ATTACK:HeroType::HT_BOMB_ATTACK;
                            _heroLevel = 0;
                            _huntingHero.op_configHeroTypeAndGrade(_heroType, _heroLevel);
                            _huntingHero.op_toastUpgrade();
                            int suitId = _huntingHero.op_fetchSuitId();
                            _pxHeadIcon->configSopx(fmt::sprintf("hunters/heros/%d.png.head.png.sopx", suitId));
                            _ptxLevel->configText(heroTypeGrade2levelString(_heroType,_heroLevel));

                            this->dismissTransfer();
                            ACSoundManage::s()->play(ACSoundManage::SN_HERO_UPGRADE);

                        }
                    }

                } else if (!_pxUpgradeRect->isVisible() && _pxHeadIcon->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation())) {
                    //显示升级
                    if (heroCouldUpgrade(_heroType, _heroLevel)) {
                        int howMuch = heroUpgradeGold(_heroType, _heroLevel);
                        showUpgradeRect(MoneyManager::s()->get() >= howMuch, heroUpgradeGold(_heroType, _heroLevel));
                    } else if (heroCouldTransfer(_heroType, _heroLevel)) {
                        int howMuch = heroTransferGold();
                        showUpgradeRect(MoneyManager::s()->get() >= howMuch, heroTransferGold());
                    }
                    _tutorialManageProtocal->op_toastHeroUpgradeDone();
                } else {
                    if (_pxUpgradeButton->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation())) {
                        CCLOG("upgrade comfirm");
                        if (heroCouldUpgrade(_heroType, _heroLevel)) {
                            int m = heroUpgradeGold(_heroType, _heroLevel);
                            if (m <= MoneyManager::s()->get()) {
                                MoneyManager::s()->cost(m);
                                _heroLevel++;
                                _huntingHero.op_configHeroTypeAndGrade(_heroType, _heroLevel);
                                _huntingHero.op_toastUpgrade();
                                _ptxLevel->configText(heroTypeGrade2levelString(_heroType,_heroLevel));

                                //TODO 升级声音和效果
                                ACSoundManage::s()->play(ACSoundManage::SN_HERO_UPGRADE);

                            } else {
                                //钱不够
                            }
                        } else if (heroCouldTransfer(_heroType, _heroLevel)) {
                            int m =  heroTransferGold();
                            if (m <= MoneyManager::s()->get()) {
                                MoneyManager::s()->cost(m);
                                _heroLevel = 0;
                                //打开转职面板
                                this->showTransfer();
                                //TODO 升级声音和效果
                            } else {
                                //钱不够
                            }
                        }

                    }
                    dismissUpgradeRect();
                }
            }

        }
    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);

}

void HeroHeadAndUpgrade::showUpgradeRect(bool enable, int howMuch)
{
    const float delay_time = 2.2;
    auto ac = Sequence::create(Show::create(), DelayTime::create(delay_time), Hide::create(), NULL);
    _pxUpgradeRect->runAction(ac->clone());
    _pxUpgradeButton->runAction(ac->clone());
    _pxUpgradeIcon->runAction(ac->clone());
    _ptxUpgradeGold->runAction(ac);
    _ptxUpgradeGold->configText(fmt::sprintf("%d", howMuch), 0.8);

//    _pxUpgradeRect->configMixColor(MoneyManager::s()->get() >= howMuch ? Vec4{0,0,0,0} : Vec4{0.5,0.5,0.5,1.0});


    Vec4 enableColor = {1.f, 200.f/255.f, 51.f/255.f,1.f};
    Vec4 disableCOlor = {0.5f,0.5f,0.5f,1.f};
    auto col = enable ? enableColor : disableCOlor;
    _pxUpgradeButton->configMixColor(col);
    _ptxUpgradeGold->configMixColor(col);

    std::string sopxfile = fmt::sprintf("hunters/upgrade/icons/%s_%d.png.sopx", heroType2string(_heroType), _heroLevel);
    _pxUpgradeIcon->configSopx(sopxfile);

}

void HeroHeadAndUpgrade::dismissUpgradeRect()
{
    auto clo = [](Node* n) {
        n->stopAllActions();
        n->setVisible(false);
    };
    clo(_pxUpgradeRect);
    clo(_pxUpgradeButton);
    clo(_pxUpgradeIcon);
    clo(_ptxUpgradeGold);
}


void HeroHeadAndUpgrade::op_configPosition(HeroPositionType position, bool direct) //设置位置
{
    const float x_start = -6;
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
            _pxHeadIcon->configMixColor({0,0,0,0});
            _tutorialManageProtocal->op_toastAddMoreHeros(static_cast<int>(_heroPositionType));
        } else {
            _pxHeadIcon->configMixColor({0.5,0.5,0.5,1.0});
        }
    } else {
        _pxHeadIcon->configMixColor({0,0,0,0});
        if (MoneyManager::s()->get() >= 100) {
            _tutorialManageProtocal->op_toastHeroUpgrade(static_cast<int>(_heroPositionType));
        }
    }
}

bool HeroHeadAndUpgrade::op_isHeadBusy()
{
    return _pxUpgradeRect->isVisible() || _transferHubNode->isVisible();
}

HeroPositionType HeroHeadAndUpgrade::op_fetchPosition() //获取位置
{
    return _heroPositionType;
}

void HeroHeadAndUpgrade::update(float dt)
{

}
