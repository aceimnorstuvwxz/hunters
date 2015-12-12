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
    _darkShadow->setVisible(true);
    std::pair<float, std::string> texts[] = {
        {1.f, "a long time ago,"},
        {1.f, " you live in a castle."},
        {0.5f, "one day, "},
        {1.5f, "you killed the prince for a girl."},
        {1.5f, "today, the emperor attracted "},
        {1.5f, "many monsters to attack the castle!"},
        {1.5f, "!recruit a hero to defend the castle"},
        {0.5f, "hurry!"},
    };

    float t = 0;
    for (auto p : texts) {
        _ptMessage->scheduleOnce([this, p](float dt) {
            showString(p.second);
        }, t*3, fmt::sprintf("sgkfdlkedsf %s", p.second));
        t+=p.first;
    }

    

    
}

void TutorialManage::op_toastEnergy()
{

}

void TutorialManage::showString(std::string str)
{
    float time_step = 0.05f;
    for (int i = 0; i < str.size(); i++) {
        _ptMessage->scheduleOnce([this, str, i](float dt) {
            _ptMessage->configText(str.substr(0,i), 0.5);
            ACSoundManage::s()->play(ACSoundManage::SN_KEY);
        }, time_step*i, fmt::sprintf("show string message %d", i));
    }
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
    }
}

void TutorialManage::update(float dt)
{
}