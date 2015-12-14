//
//  PauseGameOver.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/15.
//
//

#include "PauseGameOver.hpp"
#include "ACSoundManage.hpp"


USING_NS_CC;


void PauseGameOver::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
    initHubThings();
    initButtonsThings();
}

void PauseGameOver::initHubThings()
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

void PauseGameOver::initButtonsThings()
{
    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.2f,1.2f);
        node->setPosition3D({0,30,0});
        node->configText("game over",1);
        node->configMixColor({0.8,0,0,1.f});
        _hubNode->addChild(node);
        _ptTitle = node;
    }
    for (int i = 0; i < 3; i++)
    {
        {
            auto node = PixelNode::create();
            node->setCameraMask(_mainCamera->getCameraMask());
            node->setScale(1.0);
            node->setPosition3D({0,15-i*15.f,0});
            node->configSopx("hunters/go/button.png.sopx");
            _hubNode->addChild(node);
            _btns[i].rect = node;
        }

        {
            auto node = PixelNode::create();
            node->setCameraMask(_mainCamera->getCameraMask());
            node->setScale(1.0);
            node->setPosition3D({0,0,0});
            node->configSopx("hunters/go/button.png.sopx");
            _hubNode->addChild(node);
            _btns[i].icon = node;
        }

    }

    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){


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

void PauseGameOver::update(float dt)
{
}

void PauseGameOver::op_pause()
{

}

void PauseGameOver::op_gameOver()
{

}