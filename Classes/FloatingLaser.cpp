//
//  FloatingLaser.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/4.
//
//

#include "FloatingLaser.hpp"
#include "format.h"
#include "intersection.h"
#include "HuntingHeroMetaSuitManage.hpp"
#include "QuestDef.hpp"
#include "SOCommon.h"

USING_NS_CC;
void FloatingLaser::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initLaserThings();
}

void FloatingLaser::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,0,30});
    _hubNode->setScale(0.15);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(9);
    _hubNode->setRotation3D({90,0,-90});
    _mainLayer->addChild(_hubNode);
    //    _hubNode->setVisible(false);
}

void FloatingLaser::initLaserThings()
{
    //机器
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(2);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/float_laser.png.sopx");
        node->configBlend(true);
        _hubNode->addChild(node);
        _pxMachine = node;
    }


    for (int i = 0; i < N_NUM_LASER; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(2,5);
        node->setPosition3D({0,0,-2});
        node->configSopx("hunters/sopx/laser_light.png.sopx");
        node->setVisible(false);
        _hubNode->addChild(node);
        _pxLasers.push_back(node);
    }
    
}

void FloatingLaser::op_appear(cocos2d::Vec2 pos, float time) //出现的位置 从上方降下来 time 有效时间，时间一到 自动消失
{
    _position = pos;
    const float in_time = 1.5f;
    const float east_rate = 2.f;
    _hubNode->setVisible(true);
    _hubNode->setPosition3D({0,pos.x-150,pos.y+100});
    _hubNode->runAction(EaseOut::create(Spawn::create( RotateBy::create(in_time*1.1f, {0,360*10,0}), MoveTo::create(in_time, {0,pos.x,pos.y}), NULL), east_rate));

    _hubNode->scheduleOnce([this](float dt){
        const float time = 1.5f;
        const float length = 2;

        _hubNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(time, {0,0,length}), MoveBy::create(time, {0,0,-length}), NULL)));
        _enable = true;
    }, 1.5f, "continue wave");

    _hubNode->scheduleOnce([this, pos,east_rate](float dt){
        _enable = false;
        const float out_time = 1.5f;
        _hubNode->stopAllActions();
        _hubNode->runAction(EaseIn::create(Spawn::create( RotateBy::create(out_time, {0,360*10,0}),Sequence::create(MoveTo::create(out_time, {0,pos.x + 200,pos.y+100}), Hide::create(), NULL), NULL), east_rate));
    }, in_time+time, "laser gone");
    
}

void FloatingLaser::op_toastLaser(cocos2d::Vec2 target) //释放一个激光，转动角度，释放激光，有后坐力表现
{
//    assert(_enable);

    Vec2 vec = target - _position;
    float angle = vector2angel(vec);

    _pxMachine->setRotation3D({0,0,-90-angle});

    auto light = _pxLasers.front();
    _pxLasers.pop_front();
    _pxLasers.push_back(light);
    light->setRotation3D({0,0,-90-angle});
    light->stopAllActions();
    light->setOpacity(255);
    light->runAction(Sequence::create(Show::create(), DelayTime::create(0.2), FadeOut::create(0.2), Hide::create(), NULL));
}

void FloatingLaser::update(float dt)
{

}

