//
//  WindBar.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/22.
//
//

#include "WindBar.hpp"
#include "format.h"
#include "intersection.h"

USING_NS_CC;

void WindBar::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initWindThings();
}


void WindBar::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({-8,2.5,-10});
    _hubNode->setScale(0.1);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _mainCamera->addChild(_hubNode);
}

void WindBar::initWindThings()
{
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0,0});
        auto pixelData = loadScatPixelFile("hunters/sopx/wind_pillar.png.sopx");
        node->configBatch(PlanePixelBatchTuple{Vec3{0,0,0}, 1, &pixelData}, true);
        _hubNode->addChild(node);
        _pxBg = node;
    }


    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8);
        node->setPosition3D({0,2,0});
        auto pixelData = loadScatPixelFile("hunters/sopx/wind_strenth.png.sopx");
        node->configBatch(PlanePixelBatchTuple{Vec3{0,0,0}, 1, &pixelData}, false);
        _hubNode->addChild(node);
        _pxStrenth = node;
    }
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8);
        node->setPosition3D({0,0.5,0});
        auto pixelData = loadScatPixelFile("hunters/sopx/wind_direction.png.sopx");
        node->configBatch(PlanePixelBatchTuple{Vec3{0,0,0}, 1, &pixelData}, false);
        _hubNode->addChild(node);
        _pxDirection = node;
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.6f,0.9f);
        node->setPosition3D({1.5,27,0.5});
        node->configText("wind",1);
        node->configMixColor({49.f/255.f, 36.f/255.f, 33.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptWind = node;
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8f,0.9f);
        node->setPosition3D({2,19,0.5});
        node->configText("0",1);
        node->configMixColor({49.f/255.f, 36.f/255.f, 33.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptStrenth = node;
    }
}
void WindBar::op_configWind(float wind) //设置风力数值，正表示往右（顺），负的表示往左（逆风）0无风
{
    _targetWind = wind;
}


void WindBar::update(float dt)
{
    if (_targetWind == _currentWind) {
        return;
    }
    const float speed = 2;
    float oldWind = _currentWind;
    int oldWindInt = _currentWind/0.2;
    if (_currentWind > _targetWind) {
        _currentWind = std::max(_currentWind-speed*dt, _targetWind);
    } else if(_currentWind < _targetWind) {
        _currentWind = std::min(_currentWind+speed*dt, _targetWind);
    }
    _ptStrenth->configText(fmt::sprintf("%s%.0f", _currentWind > 0 ? "+" : _currentWind < 0 ? "-" : "", std::abs(_currentWind)),1);

    if (oldWind <= 0 && _currentWind >0 ) {
        _pxDirection->runAction(RotateTo::create(1.f, {0,0,0}));
    } else if (oldWind >= 0 && _currentWind < 0) {
        _pxDirection->runAction(RotateTo::create(1.f, {0,180,0}));
    }
    int newWindInt = _currentWind/0.2;
    if (oldWindInt != newWindInt) {
        _pxStrenth->stopAllActions();
        _pxStrenth->runAction(RepeatForever::create(RotateBy::create(1.f, {0,  _currentWind*180.f, 0})));
    }
}