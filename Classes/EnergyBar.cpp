//
//  EnergyBar.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/20.
//
//

#include "EnergyBar.hpp"

#include "format.h"
#include "intersection.h"

USING_NS_CC;

void EnergyBar::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initEnergyThings();
    initInteractiveThings();
}

void EnergyBar::op_addEnergy(float howmuch)
{
    _targetEnergy = std::min(1.f, _targetEnergy + howmuch);
    if (_targetEnergy > 0.35) {
        _tutorialManageProtocal->op_toastEnergy();
    }
}

void EnergyBar::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,-4.35f,-10});
    _hubNode->setScale(0.1);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _mainCamera->addChild(_hubNode);
}

void EnergyBar::initEnergyThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/icon_enegy_bg.png.sopx");
        _hubNode->addChild(node);
        _pxBg = node;
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0.01,0.01});
        node->configSopx("hunters/sopx/icon_enegy_content.png.sopx");
        _hubNode->addChild(node);
        _pxContent = node;
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0.02,0.02});
        node->configSopx("hunters/sopx/icon_enegy_scope.png.sopx");
        _hubNode->addChild(node);
        _pxScope = node;
    }
//    {
//        auto node = PixelNode::create();
//        node->setCameraMask(_mainCamera->getCameraMask());
//        node->setScale(1);
//        node->setPosition3D({0,0,0});
//        node->configSopx("hunters/sopx/icon_enegy_skill_a.png.sopx");
//        _hubNode->addChild(node);
//
//        node->setVisible(false);
//        _pxSkillA = node;
//
//    }
//
//    {
//        auto node = PixelNode::create();
//        node->setCameraMask(_mainCamera->getCameraMask());
//        node->setScale(1);
//        node->setPosition3D({0,0,0});
//        node->configSopx("hunters/sopx/icon_enegy_skill_b.png.sopx");
//        _hubNode->addChild(node);
//
//        node->setVisible(false);
//        _pxSkillB = node;
//    }
//
//    {
//        auto node = PixelNode::create();
//        node->setCameraMask(_mainCamera->getCameraMask());
//        node->setScale(1);
//        node->setPosition3D({0,0,0.1});
//        node->configSopx("hunters/sopx/icon_enegy_skill_c.png.sopx");
//        _hubNode->addChild(node);
//
//        node->setVisible(false);
//        _pxSkillC = node;
//    }

//    _pxSkillA->setOpacity(0);
//    _pxSkillB->setOpacity(0);

//    _pxSkillA->configOpacityAni(true, true, 1);
//    _pxSkillB->configOpacityAni(true, true, 1);

    updateEnegyContent(0,0);
}

void EnergyBar::update(float dt)
{
    float last = _currentEnergy;
    const float speed = 1.f;
    if (_targetEnergy == _currentEnergy) {
        return;
    }
    if (_targetEnergy > _currentEnergy) {
        _currentEnergy = std::min(_targetEnergy, _currentEnergy + speed*dt);
    } else {
        _currentEnergy = std::max(_targetEnergy, _currentEnergy - speed*dt*2);
    }
    updateEnegyContent(last, _currentEnergy);

}

void EnergyBar::updateEnegyContent(float last, float curr)
{
    _pxContent->setScale(66.f*curr, 1.f);
    _pxContent->setPositionX(-33.f + 66.f*curr*0.5f);
//    const float in_time = 0.3;

//    if (last < 1.f/3.f && curr >= 1.f/3.f) {
//        //显示小技能
//        _pxSkillA->stopAllActions();
//        _pxSkillB->stopAllActions();
//
//        auto ac = Sequence::create(Show::create(), FadeIn::create(in_time), NULL);
//        _pxSkillA->runAction(ac->clone());
//        _pxSkillB->runAction(ac);
//    }
//
//    if (last < 1.f && curr == 1.f) {
//        //显示大技能
//        _pxContent->configMixColorAni({1,1,1,1}, 1, -1);
//    }
//
//    if (last >= 1.f/3.f && curr <= 1.f/3.f) {
//
//        _pxSkillA->stopAllActions();
//        _pxSkillB->stopAllActions();
//        auto ac = Sequence::create(FadeOut::create(in_time), Hide::create(), NULL);
//
//        _pxSkillA->runAction(ac->clone());
//        _pxSkillB->runAction(ac);
//    }
//
//    if (last >= 1.f && curr <= 1.f) {
//        _pxContent->configMixColorAni({1,1,1,1}, 1, 0);
//    }
}

void EnergyBar::initInteractiveThings()
{
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){


        return true;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){
        auto p = touch->getLocation();
       /* if (_pxSkillA->fetchScreenRect(0, _mainCamera).containsPoint(p)) {
            CCLOG("skill a");
            tryReleaseSkill(EnergySkillType::SHIELD);
        } else if (_pxSkillB->fetchScreenRect(0, _mainCamera).containsPoint(p)) {
            CCLOG("skill b");
            tryReleaseSkill(EnergySkillType::CRITICAL);
        } else */
        if (_pxScope->fetchScreenRect(10, _mainCamera).containsPoint(p)) {
            CCLOG("skill c");
            tryReleaseSkill(EnergySkillType::FLOAT_LASER);
            _tutorialManageProtocal->op_toastEnergyDone();

        }
    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);
}

void EnergyBar::tryReleaseSkill(EnergySkillType st)
{
    if (st == EnergySkillType::FLOAT_LASER &&
        _targetEnergy >= 1.f/3.f) {
        //API 释放激光器

        CCLOG("floating laser");
        _floatingLaserManageProtocal->op_launchOneLaser();

        _targetEnergy -= 1.f/3.f;
    }

    /*else if (st == EnergySkillType::CRITICAL &&
               _targetEnergy >= 1.f/3.f) {
        //API 释放暴击加成

        _targetEnergy -= 1.f/3.f;
    } else if (st == EnergySkillType::TIMESTOP && _targetEnergy >= 1.f) {
        //API 释放时光停止
        _targetEnergy = 0;
    }*/
}

