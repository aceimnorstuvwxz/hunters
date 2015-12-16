//
//  PauseGameOver.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/15.
//
//

#include "PauseGameOver.hpp"
#include "ACSoundManage.hpp"
#include "LoadingScene.hpp"
#include "MoneyManager.hpp"
#include "WelcoScene.h"


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
    _hubNode->setVisible(false);

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
            node->setPosition3D({-55.f+i*55.f,0.f,0});
            node->configSopx("hunters/go/button.png.sopx");
            _hubNode->addChild(node);
            _btns[i].rect = node;
        }

        {
            auto node = PixelNode::create();
            node->setCameraMask(_mainCamera->getCameraMask());
            node->setScale(1.4);
            node->setPosition3D({-55.f+i*55.f,0.f,1});
            node->configSopx("hunters/go/button.png.sopx");
            _hubNode->addChild(node);
            _btns[i].icon = node;
        }

    }

    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){

        if (_hubNode->isVisible()) {
            return true;
        }
        return false;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){
        bool sfx = false;

        if (_btns[0].rect->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation())) {
            if (_paused) {
                //continue
                _hubNode->setVisible(false);
                _darkShadow->setVisible(false);
                _questSceneProtocal->op_configPaused(false);
            }
            else if (_goed) {
                //restart
                Director::getInstance()->replaceScene(LoadingScene::create());
            }
            sfx = true;
        }
        else  if (_btns[1].rect->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation())) {
            if (_paused) {
                //rate to add gold
                _hubNode->setVisible(false);
                _darkShadow->setVisible(false);

                _questSceneProtocal->op_configPaused(false);
                MoneyManager::s()->add(500);
            }
            else if (_goed) {
                //add health
                _hubNode->setVisible(false);
                _darkShadow->setVisible(false);
                _questSceneProtocal->op_configPaused(false);
                _topIconsProtocal->op_addHeart();
            }
            Application::getInstance()->openURL("itms-apps://itunes.apple.com/app/id1068411583");
            sfx = true;

        }
        else if (_btns[2].rect->fetchScreenRect(0, _mainCamera).containsPoint(touch->getLocation())) {
            Director::getInstance()->replaceScene(WelcoScene::create());
            sfx = true;

        }

        if (sfx) {
            ACSoundManage::s()->play(ACSoundManage::SN_CLICK);
        }
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

    _ptTitle->configText("",1);
    _ptTitle->configMixColor(col2vec4(96, 151, 50, 1));

    _paused = true;
    _hubNode->setVisible(true);
    _darkShadow->setVisible(true);
    _btns[0].icon->configSopx("hunters/go/icon_continue.png.sopx");
    _btns[1].icon->configSopx("hunters/go/icon_rate2gold.png.sopx");
    _btns[2].icon->configSopx("hunters/go/icon_back.png.sopx");
//    _btns[2].icon->configMixColor(col2vec4(85, 61, 0, 1));
    _questSceneProtocal->op_configPaused(true);
}

void PauseGameOver::op_gameOver()
{

    _ptTitle->configText("game over",1);
    _ptTitle->configMixColor(col2vec4(220, 19, 19, 1));
    _goed = true;
    _hubNode->setVisible(true);
    _darkShadow->setVisible(true);
    _btns[0].icon->configSopx("hunters/go/icon_restart.png.sopx");
    _btns[1].icon->configSopx("hunters/go/icon_rate2health.png.sopx");
    _btns[2].icon->configSopx("hunters/go/icon_back.png.sopx");
//    _btns[2].icon->configMixColor(col2vec4(85, 61, 0, 1));
    _questSceneProtocal->op_configPaused(true);

}