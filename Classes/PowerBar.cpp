//
//  PowerBar.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/22.
//
//

#include "PowerBar.hpp"
#include "format.h"
#include "intersection.h"

USING_NS_CC;

void PowerBar::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initPowerThings();
}


void PowerBar::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({-5,-3,-10});
    _hubNode->setScale(0.15);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(10);
    _mainCamera->addChild(_hubNode);
//    _hubNode->setVisible(false);
}

void PowerBar::initPowerThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.2,1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/strenth_rect.png.sopx");
        node->configBlend(true);
        _hubNode->addChild(node);
        _pxRect = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.2,1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/strenth_content.png.sopx");
        node->configBlend(true);

        _hubNode->addChild(node);
        _pxContent = node;
    }
    
    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.5f,2.f);
        node->setPosition3D({-12,8,0});
        node->configText("00#",0.5);
        node->configMixColor({0.f/255.f, 183.f/255.f, 233.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptAngle = node;
    }
}


void PowerBar::op_show()
{
    _hubNode->setVisible(true);
}

void PowerBar::op_dismiss()
{
    _hubNode->setVisible(false);
}

void PowerBar::update(float dt)
{

}

void PowerBar::op_configPower(float power, int angle) //power [0-1]
{
    _pxContent->configYCut(power*45);
    _ptAngle->configText(fmt::sprintf("%02d#", angle));
}