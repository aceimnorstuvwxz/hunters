//
//  EnergyBar.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/20.
//
//

#include "EnergyBar.hpp"

#include "format.h"

USING_NS_CC;

void EnergyBar::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initEnergyThings();
}

void EnergyBar::op_addEnergy(float howmuch)
{
    _targetEnergy = std::min(1.f, _targetEnergy + howmuch);
}

void EnergyBar::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,-5,-10});
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
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/icon_enegy_skill_a.png.sopx");
        _hubNode->addChild(node);

        node->setVisible(false);
        _pxSkillA = node;

    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/icon_enegy_skill_b.png.sopx");
        _hubNode->addChild(node);

        node->setVisible(false);
        _pxSkillB = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1);
        node->setPosition3D({0,0,0.1});
        node->configSopx("hunters/sopx/icon_enegy_skill_c.png.sopx");
        _hubNode->addChild(node);

        node->setVisible(false);
        _pxSkillC = node;
    }

    updateEnegyContent(0,0);
}

void EnergyBar::update(float dt)
{
    float last = _currentEnergy;
    const float speed = 0.5f;
    if (_targetEnergy == _currentEnergy) {
        return;
    }
    if (_targetEnergy > _currentEnergy) {
        //这里有个 bug，导致会很好看的抖动，所以不改
        _currentEnergy = std::min(1.f, _currentEnergy + speed*dt);
    } else {
        _currentEnergy = std::max(0.f, _currentEnergy - speed*dt);
    }
    updateEnegyContent(last, _currentEnergy);

}

void EnergyBar::updateEnegyContent(float last, float curr)
{
    _pxContent->setScale(66.f*curr, 1.f);
    _pxContent->setPositionX(-33.f + 66.f*curr*0.5f);
    const float in_time = 0.3;

    if (last < 1.f/3.f && curr >= 1.f/3.f) {
        //显示小技能
        auto ac = Sequence::create(Show::create(), NULL);
        _pxSkillA->runAction(ac->clone());
        _pxSkillB->runAction(ac);
    }

    if (last < 1.f && curr == 1.f) {
        //显示大技能
//        _pxSkillC->runAction(Sequence::create(Show::create(), NULL));
        _pxContent->configMixColorAni({1,1,1,1}, 1, -1);
    }
}
