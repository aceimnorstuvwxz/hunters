//
//  LoadingScene.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/9.
//
//

#include "LoadingScene.hpp"
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


bool LoadingScene::init()
{
    assert(TRBaseScene::init());


//    addCommonBtn({0.9,0.9}, "bk", [](){ Director::getInstance()->popScene(); });

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

    const float loading_time = 1.5;

    auto hub = Sprite3D::create();
    hub->setPosition3D({0,0,0});
    hub->setCameraMask(_mainCamera->getCameraMask());
//    hub->setScale(2.0);
    _mainLayer->addChild(hub);
    {

        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,0,0});
        node->configSopx("sopx/loading/rect.png.sopx");
        hub->addChild(node);
//        node->setScale(0.3);
//        node->setVisible(false);
//        node->setOpacity(0);
//        node->runAction();
    }

    {

        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({-25,0,0});
        node->configSopx("sopx/loading/content.png.sopx");
        hub->addChild(node);
//        node->setScaleX(25);
        node->runAction(Spawn::create(ScaleTo::create(loading_time, 50, 1, 1), MoveTo::create(loading_time, {0,0,0}), NULL));
        //        node->setScale(0.3);
        //        node->setVisible(false);
        //        node->setOpacity(0);
        //        node->runAction();
    }

    scheduleOnce([this](float dt) {

        this->loadingTask0();
    }, loading_time*0.2, "loading task 0");
    scheduleOnce([this](float dt) {

        this->loadingTask1();
    }, loading_time*0.5, "loading task 1");
    scheduleOnce([this](float dt){
        Director::getInstance()->replaceScene(QuestScene::create());
    },loading_time,"jump to quest");

    return true;
}

void LoadingScene::loadingTask0()
{

}

void LoadingScene::loadingTask1(){

}