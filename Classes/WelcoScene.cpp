// (C) 2015 Turnro.com

#include "WelcoScene.h"
#include "PixelNode.h"
#include "PlanePixelNode.h"
#include "PixelTextNode.hpp"
#include "format.h"
#include "RoadPlane.h"
#include "QuestDef.hpp"
#include "LoadingScene.hpp"

#include "QuestScene.h"
#include "CreditsScene.hpp"
USING_NS_CC;

bool WelcoScene::init()
{
    assert(TRBaseScene::init());


//    addCommonBtn({0.9,0.9}, "rt", [](){ Director::getInstance()->replaceScene(WelcoScene::create()); });

    auto layer = Layer::create();
    this->addChild(layer);

    auto size = Director::getInstance()->getVisibleSize();

    auto camera = Camera::createPerspective(60, 1334.f/750/*size.width/size.height*/, 0.1, 1000000);
    camera->setCameraFlag(CameraFlag::USER1);
    layer->addChild(camera);
    camera->setCameraMask((unsigned short)CameraFlag::USER1);
    camera->setPosition3D({100,0,27});
    camera->setRotation3D({105,0,-90});
    _mainLayer = layer;
    _mainCamera = camera;

    initButtonsThings();
    initBackgroundThings();
    initExtraButtons();
    initTouchThings();

    scheduleUpdate();

    return true;
}


void WelcoScene::initBackgroundThings()
{
    _roadPlane = RoadPlane::create();
    _roadPlane->setCameraMask(_mainCamera->getCameraMask());
    _roadPlane->setPosition3D({0,550,0});
    _mainLayer->addChild(_roadPlane);
    _roadPlane->configImage("battle_road/tmp/road.png");
    _roadPlane->runAction(RepeatForever::create(MoveBy::create(QuestDef::ROLE_MOVE_TIME, {0, -QuestDef::POS_Y_ROLE_OUT+QuestDef::POS_Y_ROLE,0})));



    _skyPlane = RoadPlane::create();
    _skyPlane->setCameraMask(_mainCamera->getCameraMask());
    _skyPlane->setPosition3D({-250,350,170});
    _skyPlane->setScale(2.f);
    _skyPlane->setRotation3D({90,0,-90});
    _mainLayer->addChild(_skyPlane);
    _skyPlane->configImage("battle_road/tmp/sky.png");
    _skyPlane->runAction(RepeatForever::create(MoveBy::create(QuestDef::ROLE_MOVE_TIME, {0, -QuestDef::POS_Y_ROLE_OUT+QuestDef::POS_Y_ROLE,0})));


    // 草
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({-50, scene_things_start_pos, 0});
        _mainLayer->addChild(node);
        node->setRotation3D({90,0,-90});
        auto pixelData = loadScatPixelFile("battle_road/tmp/glass.png.sopx");
        std::vector<PlanePixelBatchTuple> pixelBatchData;
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 10; i++) {
                pixelBatchData.push_back({{glass_width_step*i+random(-20.f, 10.f), 0.f, -10.f*j+random(-10.f, 0.f)}, 1.f + j*0.25f, &pixelData});
            }
        }
        node->configBatch(pixelBatchData);
        node->configMoveLine(0);
        node->configMoveWidth(scene_things_width);
        node->runAction(RepeatForever::create(MoveBy::create(QuestDef::ROLE_MOVE_TIME, {0, -QuestDef::POS_Y_ROLE_OUT+QuestDef::POS_Y_ROLE,0})));
        _leftGlass = node;
    }

    //树
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({-50, scene_things_start_pos, 0});
        _mainLayer->addChild(node);
        node->setRotation3D({90,0,-90});
        auto pixelData = loadScatPixelFile("battle_road/tmp/tree.png.sopx");
        std::vector<PlanePixelBatchTuple> pixelBatchData;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < scene_things_width/tree_width_step; i++) {
                pixelBatchData.push_back({{tree_width_step*i+random(-10.f, 10.f), 0.f, -10.f*j+random(-10.f, 0.f)},1.f + j, &pixelData});
            }
        }
        node->configBatch(pixelBatchData);
        node->configMoveLine(0);
        node->configMoveWidth(scene_things_width);
        node->runAction(RepeatForever::create(MoveBy::create(QuestDef::ROLE_MOVE_TIME, {0, -QuestDef::POS_Y_ROLE_OUT+QuestDef::POS_Y_ROLE,0})));
        _leftTrees = node;
    }


    // 山
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({-80, scene_things_start_pos, 0});
        _mainLayer->addChild(node);
        node->setRotation3D({90,0,-90});
        auto pixelData = loadScatPixelFile("battle_road/tmp/moutain.png.sopx");
        std::vector<PlanePixelBatchTuple> pixelBatchData;
        for (int j = 0; j < 1; j++) {
            for (int i = 0; i < scene_things_width/moutain_width_step; i++) {
                float zp = -40.f*j +random(-40.f, 0.f);
                pixelBatchData.push_back({{moutain_width_step*i+random(-10.f, 20.f), 0.f, zp}, 0.65f*(zp-80)/-70, &pixelData});
            }
        }
        node->configBatch(pixelBatchData);
        node->configMoveLine(0);
        node->configMoveWidth(scene_things_width);
        node->runAction(RepeatForever::create(MoveBy::create(QuestDef::ROLE_MOVE_TIME, {0, -QuestDef::POS_Y_ROLE_OUT+QuestDef::POS_Y_ROLE,0})));
        _moutains = node;
    }

    // 远山
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({-110, scene_things_start_pos, 0});
        _mainLayer->addChild(node);
        node->setRotation3D({90,0,-90});
        auto pixelData = loadScatPixelFile("battle_road/tmp/hill.png.sopx");
        std::vector<PlanePixelBatchTuple> pixelBatchData;
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < scene_things_width/hill_width_step; i++) {
                float zp = - random(0.f, 55.f) - j *40;
                pixelBatchData.push_back({{hill_width_step*i+random(-10.f, 20.f), 0.f, zp}, j*0.6f+1, &pixelData});
            }
        }
        node->configBatch(pixelBatchData);

        node->configMoveLine(0);
        node->configMoveWidth(scene_things_width);
        node->runAction(RepeatForever::create(MoveBy::create(QuestDef::ROLE_MOVE_TIME, {0, -QuestDef::POS_Y_ROLE_OUT+QuestDef::POS_Y_ROLE,0})));
        _hills = node;
    }

}

void WelcoScene::initButtonsThings()
{
    const float shake_time = 0.05;
    const float stay_time = 0.3;
    const float move_time = 1.0;

    auto hub = Sprite3D::create();
    hub->setPosition3D({0,0,65});
    hub->setRotation3D({105,0,-90});
    hub->setCameraMask(_mainCamera->getCameraMask());
    hub->setScale(2.0);
    _mainLayer->addChild(hub);

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({1.0,-0.9,0});
        node->configSopx("sopx/welco/arrow.png.sopx");
        hub->addChild(node);
        node->setOpacity(0);
        node->setScale(1.1,1.5);
        node->runAction(Sequence::create(
                                         FadeIn::create(1.5f),
                                         DelayTime::create(0.3),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(shake_time, {2,0,0}),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(shake_time, {2,0,0}),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(0.5f, {0,4,0}),
                                         DelayTime::create(stay_time),

                                         MoveBy::create(move_time, {0,10,0}),
                                         NULL));
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0.7,-0.6,0});
        node->configSopx("sopx/welco/rush.png.sopx");
        hub->addChild(node);
        node->setOpacity(0);
        node->setScale(1.1,1.5);
        node->runAction(Sequence::create(
                                         FadeIn::create(1.5f),
                                         DelayTime::create(0.3),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(shake_time, {2,0,0}),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(shake_time, {2,0,0}),
                                         MoveBy::create(shake_time, {-1,0,0}),
                                         MoveBy::create(0.5f, {0,-4,0}),
                                         DelayTime::create(stay_time),

                                         MoveBy::create(move_time, {0,10,0}),
                                         NULL));
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({50,50,0});
        node->configSopx("sopx/welco/sword.png.sopx");
        hub->addChild(node);
        node->setScale(0.8);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         MoveTo::create(0.3, {5,1,0}),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time),

                                         MoveBy::create(move_time, {0,10,0}),
                                         NULL));
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,-10,0});
        node->configSopx("sopx/welco/play.png.sopx");
        hub->addChild(node);
        node->setScale(0.4);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.1),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),
                                         DelayTime::create(1.f),
                                         ScaleTo::create(0.15, 0.5), ScaleTo::create(0.15, 0.4),
                                         DelayTime::create(3.f),
                                         ScaleTo::create(0.15, 0.5), ScaleTo::create(0.15, 0.4),
                                         DelayTime::create(3.f),
                                         ScaleTo::create(0.15, 0.5), ScaleTo::create(0.15, 0.4),
                                         DelayTime::create(3.f),
                                         ScaleTo::create(0.15, 0.5), ScaleTo::create(0.15, 0.4),
                                         DelayTime::create(3.f),
                                         NULL));
        _pxPlay = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0.2,-19,0});
        node->configSopx("sopx/welco/rank.png.sopx");
        hub->addChild(node);
        node->setScale(0.4);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.2),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),
                                         NULL));
        _pxLeaderboard = node;
    }


    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.27);
        node->setPosition3D({0.2,-19,0});
        node->configText("leaderboard",1);
        node->configMixColor({1.f, 200.f/255.f, 51.f/255.f,1.f});
        hub->addChild(node);
        node->setVisible(false);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.2),
                                         Show::create(),
                                         Spawn::create(
                                                       Show::create(), NULL),
                                         NULL));
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0.2,-26,0});
        node->configSopx("sopx/welco/credits.png.sopx");
        hub->addChild(node);
        node->setScale(0.4);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),
                                         NULL));
        _pxCredits = node;

    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.27);
        node->setPosition3D({0.4,-26,0});
        node->configText("credits",1);
        node->configMixColor({1.f, 200.f/255.f, 51.f/255.f,1.f});
        hub->addChild(node);
        node->setVisible(false);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3),
                                         Show::create(),
                                         Spawn::create(
                                                       Show::create(), NULL),
                                         NULL));
    }


    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({-49,20,0});
        node->configSopx(ACSoundManage::s()->getSoundState() ? "sopx/welco/sound.png.sopx" : "sopx/welco/sound_x.png.sopx");
        hub->addChild(node);
        node->setScale(0.4);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3+0.4),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),
                                         NULL));
        _pxSoundSwitch = node;
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({-40,20,0});
        node->configSopx(ACSoundManage::s()->getMusicState() ? "sopx/welco/music.png.sopx":"sopx/welco/music_x.png.sopx");
        hub->addChild(node);
        node->setScale(0.4);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3+0.4),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),
                                         NULL));
        _pxMusicSwitch = node;
    }


    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({49,20,0});
        node->configSopx("sopx/welco/twitter.png.sopx");
        hub->addChild(node);
        node->setScale(0.3);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3+0.4),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),
                                         NULL));
        _pxTwitter = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({49,10,0});
        node->configSopx("sopx/welco/weibo.png.sopx");
        hub->addChild(node);
        node->setScale(0.3);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3+0.4),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),
                                         NULL));
        _pxWeibo = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({49,-23,0});
        node->configSopx("sopx/welco/moregame.png.sopx");
        hub->addChild(node);
        node->setScale(0.3);
        node->setVisible(false);
        node->setOpacity(0);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3+0.4),
                                         Show::create(),
                                         Spawn::create(
                                                       FadeIn::create(0.3), NULL),

                                         DelayTime::create(5.f),

                                         RotateBy::create(0.15, {0,0,5}),                                        RotateBy::create(0.15, {0,0,-5}),
                                         RotateBy::create(0.15, {0,0,-5}),                                        RotateBy::create(0.15, {0,0,5}),

                                         DelayTime::create(5.f),


                                         RotateBy::create(0.15, {0,0,5}),                                        RotateBy::create(0.15, {0,0,-5}),
                                         RotateBy::create(0.15, {0,0,-5}),                                        RotateBy::create(0.15, {0,0,5}),

                                         DelayTime::create(5.f),


                                         RotateBy::create(0.15, {0,0,5}),                                        RotateBy::create(0.15, {0,0,-5}),
                                         RotateBy::create(0.15, {0,0,-5}),                                        RotateBy::create(0.15, {0,0,5}),

                                         DelayTime::create(5.f),

                                         NULL));
        _pxMoreGame = node;
    }

    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.27);
        node->setPosition3D({45,-28,1});
        node->configText("more game",1);
        node->configMixColor({1.f, 200.f/255.f, 51.f/255.f,1.f});
        hub->addChild(node);
        node->setVisible(false);
        node->runAction(Sequence::create(
                                         DelayTime::create(1.5f),
                                         DelayTime::create(0.3),
                                         DelayTime::create(shake_time*6+0.5),
                                         DelayTime::create(stay_time+move_time+0.3+0.3+0.3),
                                         Show::create(),
                                         Spawn::create(
                                                       Show::create(), NULL),
                                         NULL));
    }




    _mainCamera->scheduleOnce([this](float dt){
        ACSoundManage::s()->play(ACSoundManage::SN_M_WELCO);
    }, 5.f, "sdfsdf}3");

    _mainCamera->scheduleOnce([this](float dt){
        ACSoundManage::s()->play(ACSoundManage::SN_ARROW_NORMAL_HIT);
    }, 2.f, "sdfsdf}dsf33");
}

void WelcoScene::initExtraButtons()
{

}

void WelcoScene::update(float dt)
{
    _leftTrees->configMoveLine(scene_things_start_pos - _leftTrees->getPosition3D().y);
    _leftGlass->configMoveLine(scene_things_start_pos - _leftGlass->getPosition3D().y);
    _moutains->configMoveLine(scene_things_start_pos - _moutains->getPosition3D().y);
    _hills->configMoveLine(scene_things_start_pos - _hills->getPosition3D().y);
}

void WelcoScene::initTouchThings()
{
    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){

        return true;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){
        bool sfx = false;
        if (_pxSoundSwitch->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation())) {
            CCLOG("sound switch");
            ACSoundManage::s()->soundSwitch();

            _pxSoundSwitch->configSopx(ACSoundManage::s()->getSoundState() ? "sopx/welco/sound.png.sopx" : "sopx/welco/sound_x.png.sopx");
            sfx = true;
        } else if (_pxMusicSwitch->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation())) {
            CCLOG("musci switch");
            ACSoundManage::s()->musicSwitch();
            _pxMusicSwitch->configSopx(ACSoundManage::s()->getMusicState() ? "sopx/welco/music.png.sopx":"sopx/welco/music_x.png.sopx");
            sfx = true;

        } else if (_pxTwitter->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation())) {

            Application::getInstance()->openURL("https://twitter.com/arisecbf");
            sfx = true;

        } else if (_pxWeibo->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation())) {
            Application::getInstance()->openURL("http://weibo.com/u/1750764372");
            sfx = true;

        } else if (_pxPlay->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation())) {
            ACSoundManage::s()->stopMusic();
            Director::getInstance()->replaceScene(LoadingScene::create());
            sfx = true;
        } else if (_pxLeaderboard->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation()) ) {
            //TODO open leaderboards
            sfx = true;

        } else if (_pxCredits->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation()) ){
            //todo credits
            Director::getInstance()->pushScene(CreditsScene::create());
            sfx = true;

        }else if (_pxMoreGame->fetchScreenRect(5, _mainCamera).containsPoint(touch->getLocation())) {
            Application::getInstance()->openURL("https://itunes.apple.com/developer/bingfeng-chen/id1041292698");
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