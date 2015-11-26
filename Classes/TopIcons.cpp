//
//  TopIcons.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/20.
//
//

#include "TopIcons.hpp"
#include "format.h"

USING_NS_CC;

void TopIcons::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initGoldThings();
    initPauseThings();
}



void TopIcons::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,5,-10});
    _hubNode->setScale(0.1);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _mainCamera->addChild(_hubNode);
}



void TopIcons::initGoldThings()
{

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({60,0,0});
        node->configSopx("hunters/sopx/icon-gold-bg.png.sopx");
        _hubNode->addChild(node);
        _pxGoldBg = node;
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8);
        node->setPosition3D({45,0,0.5});
        node->configSopx("hunters/sopx/icon-gold.png.sopx");
        _hubNode->addChild(node);
        _pxGoldIcon = node;
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8f,1.f);
        node->setPosition3D({68,0.5,0.5});
        node->configText("00000",1);
        node->configMixColor({1.f, 200.f/255.f, 51.f/255.f,1.f});
        _hubNode->addChild(node);
        _pxGoldNumber = node;
    }
}


void TopIcons::initPauseThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.7);
        node->setPosition3D({94,-1,0});
        node->configSopx("hunters/sopx/icon-pause.png.sopx");
        _hubNode->addChild(node);
        _pxPauseIcon = node;
    }
}


void TopIcons::op_configGold(int number)
{
    _targetGoldNumber = number;
}

void TopIcons::update(float dt)
{
    const int step = 7;
    if (_targetGoldNumber != _currentGoldNumber) {
        if (_targetGoldNumber > _currentGoldNumber) {
            _currentGoldNumber = std::min(_currentGoldNumber += step, _targetGoldNumber);
        } else {
            _currentGoldNumber = std::max(_currentGoldNumber -= step, _targetGoldNumber);
        }
        _pxGoldNumber->configText(fmt::sprintf("%05d", _currentGoldNumber),1);
    }
}