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
        node->setScale(1);
        node->setPosition3D({0,0,0.1});
        auto pixelData = loadScatPixelFile("hunters/sopx/wind_direction.png.sopx");
        node->configBatch(PlanePixelBatchTuple{Vec3{0,0,0}, 1, &pixelData}, false);
        _hubNode->addChild(node);
        _pxDirection = node;
    }

    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0,0.1});
        auto pixelData = loadScatPixelFile("hunters/sopx/wind_strenth.png.sopx");
        node->configBatch(PlanePixelBatchTuple{Vec3{0,0,0}, 1, &pixelData}, false);
        _hubNode->addChild(node);
        _pxStrenth = node;
    }

}
void WindBar::op_configWind(int wind) //设置风力数值，正表示往右（顺），负的表示往左（逆风）0无风
{
    _targetWind = wind;
}


void WindBar::update(float dt)
{

}