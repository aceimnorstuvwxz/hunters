//
//  TurotialManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/12.
//
//

#include "TutorialManage.hpp"
#include "ACSoundManage.hpp"


USING_NS_CC;


void TutorialManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
    initHubThings();
    initTutorialThings();
}

void TutorialManage::op_toastStory()
{
    _hubNode->setVisible(true);
    _darkShadow->setVisible(true);
    std::pair<float, std::string> texts[] = {
        {1.f, "a long time ago,"},
        {1.f, " you live in a castle."},
        {0.5f, "life is peace."},
        {0.5f, "but one day, "},
        {1.f, "monsters come in,"},
        {0.5f, "touch above to recruit a hero! hurry!"},
    };

    float t = 0;
    for (auto p : texts) {
        _ptMessage->scheduleOnce([this, p](float dt) {
            showString(p.second);
        }, t*3, fmt::sprintf("sgkfdlkedsf %s", p.second));
        t+=p.first;
    }

    _pxHand->scheduleOnce([this](float dt) {
        _pxHand->setPosition3D({-35,35,0});
        float t = 0.5f;
        _pxHand->runAction(RepeatForever::create(Sequence::create(Show::create(), DelayTime::create(t), Hide::create(), DelayTime::create(t), NULL)));
        ACSoundManage::s()->play(ACSoundManage::SN_M_1,true);
    }, t*3, "show hand");
    
}

void TutorialManage:: op_toastAttack()
{
    if (!_willShowAttack) {
        return;
    }
//    _touchEndSkipOnce = true;
    _willShowAttack = false;
    _hubNode->setVisible(true);
    _ptMessage->setVisible(true);
    _darkShadow->setVisible(true);
    _pxHand->setVisible(true);
    _pxHand->stopAllActions();
    _ptMessage->configText("swipe to aim and shoot!");
    _pxHand->setPosition3D({-30,15,0});
    _pxHand->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.f, {-30,15,0}), MoveTo::create(1.f, {-45,-15,0}), NULL)));
}

void TutorialManage::op_toastEnergy()
{
    _autoHide = true;
    if (!_energy) {
        return;
    }
    _energy = false;
    _autoHide = true;
    _hubNode->setVisible(true);
    _darkShadow->setVisible(false);
    _ptMessage->setVisible(false);
    _pxHand->setVisible(true);
    _pxHand->stopAllActions();
    _pxHand->setPosition3D({0, -50, 1});
    float t = 0.5f;
    _pxHand->runAction(RepeatForever::create(Sequence::create(Show::create(), DelayTime::create(t), Hide::create(), DelayTime::create(t), NULL)));
}

void TutorialManage::showString(std::string str)
{
    float time_step = 0.05f;
    for (int i = 0; i < str.size(); i++) {
        _ptMessage->scheduleOnce([this, str, i](float dt) {
            _ptMessage->configText(str.substr(0,i), 1);
            ACSoundManage::s()->play(ACSoundManage::SN_KEY);
        }, time_step*i, fmt::sprintf("show string message %d", i));
    }
}

void TutorialManage::op_toastAddMoreHeros(int p)
{
    if (!_addMore) {
        return;
    }
    _addMore = false;
    _autoHide = true;
    _hubNode->setVisible(true);
    _darkShadow->setVisible(false);
    _ptMessage->setVisible(false);
    _pxHand->setVisible(true);
    _pxHand->stopAllActions();
    _pxHand->setPosition3D({-15-p*15.f, 35, 0});
    float t = 0.5f;
    _pxHand->runAction(RepeatForever::create(Sequence::create(Show::create(), DelayTime::create(t), Hide::create(), DelayTime::create(t), NULL)));
}

void TutorialManage::op_toastAddMoreHerosDone()
{
    _autoHide = false;
    _hubNode->setVisible(false);
    _darkShadow->setVisible(false);
}

void TutorialManage::op_toastHeroUpgrade(int p)
{
    _upgWait--;
    if (_upgWait > 0) {
        return;
    }
    if (!_upgrade) {
        return;
    }
    _upgrade = false;
    _autoHide = true;
    _hubNode->setVisible(true);
    _darkShadow->setVisible(false);
    _ptMessage->setVisible(false);
    _pxHand->setVisible(true);
    _pxHand->stopAllActions();
    _pxHand->setPosition3D({-45+p*15.f, 35, 0});
    float t = 0.5f;
    _pxHand->runAction(RepeatForever::create(Sequence::create(Show::create(), DelayTime::create(t), Hide::create(), DelayTime::create(t), NULL)));
}

void TutorialManage::op_toastHeroUpgradeDone()
{
    _autoHide = false;
    _hubNode->setVisible(false);
    _darkShadow->setVisible(false);
}

void TutorialManage::op_toastEnergyDone()
{
    _autoHide = false;
    _hubNode->setVisible(false);
    _darkShadow->setVisible(false);
}

void TutorialManage::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,0,-10});
    _hubNode->setScale(0.1);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _mainCamera->addChild(_hubNode);

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

void TutorialManage::initTutorialThings()
{
    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8f,1.2f);
        node->setPosition3D({0,0,0});
        node->configText("message goes here",1);
        node->configMixColor({1,1,1,1.f});
        _hubNode->addChild(node);
        _ptMessage = node;
        node->setVisible(false);
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.0);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/tutorial/hand.png.sopx");
        _hubNode->addChild(node);
        _pxHand = node;
        node->setVisible(false);
    }

    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){

        if (_touchEndSkipOnce) {
            _touchEndSkipOnce = false;
            return false;
        }
        _hubNode->setVisible(false);
        _darkShadow->setVisible(false);

        return false;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){

    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);
}

void TutorialManage::update(float dt)
{
}