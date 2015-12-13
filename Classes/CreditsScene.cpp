//
//  CreditsScene.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/13.
//
//

#include "CreditsScene.hpp"
#include "PixelNode.h"
#include "PlanePixelNode.h"
#include "PixelTextNode.hpp"
#include "format.h"
#include "RoadPlane.h"
#include "QuestDef.hpp"
#include "LoadingScene.hpp"
#include "BattleState.hpp"
#include "QuestScene.h"

USING_NS_CC;


bool CreditsScene::init()
{
    assert(TRBaseScene::init());



    auto layer = Layer::create();
    this->addChild(layer);

    auto size = Director::getInstance()->getVisibleSize();

    auto camera = Camera::createPerspective(60, size.width/size.height, 0.1, 1000000);
    camera->setCameraFlag(CameraFlag::USER1);
    layer->addChild(camera);
    camera->setCameraMask((unsigned short)CameraFlag::USER1);
    camera->setPosition3D({0,0,50});
    camera->lookAt({0,0,0}, {0,1,0});
    _mainLayer = layer;
    _mainCamera = camera;


    auto hub = Node::create();
    hub->setPosition3D({0,0,0});
    hub->setCameraMask(_mainCamera->getCameraMask());
    _mainLayer->addChild(hub);
    _hub = hub;

     initCreditsThings();


    return true;
}

void CreditsScene::initCreditsThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0.2,-26,0});
        node->configSopx("sopx/welco/back.png.sopx");
        _hub->addChild(node);
        node->setScale(0.4);
        _pxBack = node;

    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.27);
        node->setPosition3D({0.4,-26,0});
        node->configText("back",1);
        node->configMixColor({1.f, 200.f/255.f, 51.f/255.f,1.f});
        _hub->addChild(node);
    }

    auto _image = RoadPlane::create();
    _image->configImage("images/credits.png");
    _image->configBlend(true);
    _image->setCameraMask(_mainCamera->getCameraMask());
    _image->runAction(ScaleTo::create(5, 0.08));
    _hub->addChild(_image);

}

void CreditsScene::initTouchThings()
{
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){

        return true;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){
        bool sfx = false;
        if (_pxBack->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation())) {

            Director::getInstance()->popScene();

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
