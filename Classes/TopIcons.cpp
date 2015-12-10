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
//
//    _heartHubNode = Node::create();
//    _heartHubNode->setPosition3D({0,5,-10});
//    _heartHubNode->setScale(0.1);
//    _heartHubNode->setCameraMask(_mainCamera->getCameraMask());
//    _mainCamera->addChild(_heartHubNode);
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
    const float heart_pos = -15;
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.0);
        node->setPosition3D({-70+heart_pos,0,0});
        node->configSopx("hunters/sopx/heart_bg.png.sopx");
        _hubNode->addChild(node);
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.0);
        node->setPosition3D({-70+heart_pos,0,0.5});
        node->configSopx("hunters/sopx/heart_icon.png.sopx");
        _hubNode->addChild(node);
                _pxHeart = node;
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.f,1.2f);
        node->setPosition3D({-62+heart_pos,0.5,0.5});
        node->configText("20",1);
        node->configMixColor({0.f/255.f, 0.f/255.f, 0.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptxHeartNumber = node;
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


    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.2f,1.2f);
        node->setPosition3D({89,-14,0});
        node->configText("wave",1);
        node->configMixColor({1.f, 200.f/255.f, 51.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptxWaveTitle = node;
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.2f,1.2f);
        node->setPosition3D({96,-22,0});
        node->configText("00",1);
        node->configMixColor({1.f, 200.f/255.f, 51.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptxWaveNumber = node;
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

void TopIcons::op_minusHeart()
{
    _heart--;
    _ptxHeartNumber->configText(fmt::sprintf("%d", _heart));
    _battleRoadProtocal->op_hitCastle(_heart == 0);
    _pxHeart->configMixColorAni({1,1,1,1}, 0.2);

    int grade = (QuestDef::INIT_HEART - _heart)/4;
     _battleRoadProtocal->op_configCastle(grade); // 0 没有损伤 5完全毁灭

    if (_heart == 0) {
        //game over
    }

}

void TopIcons::op_addWave()
{
    _currentWave++;
    _ptxWaveNumber->configText(fmt::sprintf("%d", _currentWave));
    auto ac =  EaseInOut::create(Sequence::create(ScaleTo::create(0.15f, 1.5f),ScaleTo::create(0.15f, 1.2f), NULL), 2.f);

    _ptxWaveTitle->runAction(ac->clone());
    _ptxWaveNumber->runAction(ac->clone());
    ACSoundManage::s()->play(ACSoundManage::SN_NEW_WAVE);
}
