//
//  HuntingHerosManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/26.
//
//

#include "HuntingHerosManage.hpp"
#include "format.h"
#include "intersection.h"
#include "SOCommon.h"

USING_NS_CC;

void HuntingHerosManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHeadsThings();
}

void HuntingHerosManage::op_aimingStart() //power bar 通知开始瞄准
{

}

void HuntingHerosManage::op_aimingCancel() //power bar 通知瞄准取消
{

}

void HuntingHerosManage::op_configAiming(float angle, float strenth) //设置瞄准的参数
{

}

void HuntingHerosManage::op_toastBow(float angle, float strenth) //以这个角度开始放箭
{

}

void HuntingHerosManage::update(float dt)
{

}

void HuntingHerosManage::initHeadsThings()
{
    // hubnode
    _headsHubNode = Node::create();
    _headsHubNode->setPosition3D({-4,4.75,-10});
    _headsHubNode->setScale(0.08);
    _headsHubNode->setCameraMask(_mainCamera->getCameraMask());
    _headsHubNode->setZOrder(10);
    _mainCamera->addChild(_headsHubNode);
    //    _hubNode->setVisible(false);

    // enpty heads
    for (int i = 0; i < 4; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1,1);
        node->setPosition3D({26.f*i,0,0});
        node->configSopx("hunters/heros/head_add.png.sopx");
        node->configBlend(true);
        _headsHubNode->addChild(node);
        _pxHeads[i] = node;
    }

}
