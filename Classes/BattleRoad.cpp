// (C) 2015 Arisecbf


#include "BattleRoad.hpp"
#include "PixelNode.h"
#include "PlanePixelNode.h"
#include "PixelTextNode.hpp"
#include "format.h"

USING_NS_CC;
void BattleRoad::init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    _roadPlane = RoadPlane::create();
    _roadPlane->setCameraMask(_mainCamera->getCameraMask());
    _roadPlane->setPosition3D({0,50,0});
    _mainLayer->addChild(_roadPlane);
    _roadPlane->configImage("battle_road/tmp/road.png");


    _skyPlane = RoadPlane::create();
    _skyPlane->setCameraMask(_mainCamera->getCameraMask());
    _skyPlane->setPosition3D({-250,350,170});
    _skyPlane->setScale(2.f);
    _skyPlane->setRotation3D({90,0,-90});
    _mainLayer->addChild(_skyPlane);
    _skyPlane->configImage("battle_road/tmp/sky.png");

    {
        auto sp = RoadPlane::create();
        sp->setCameraMask(_mainCamera->getCameraMask());
        sp->setPosition3D({-29,0,0});
        sp->setScale(1000.f);
        sp->setRotation3D({90,0,-90});
        _mainLayer->addChild(sp);
        sp->configImage("images/dark_shadow.png");
        sp->configBlend(true);
        sp->setVisible(false);
        sp->setOpacity(100);
        sp->setZOrder(3);
        _leftShadow = sp;
    }
    {
        auto sp = RoadPlane::create();
        sp->setCameraMask(_mainCamera->getCameraMask());
        sp->setPosition3D({29,0,0});
        sp->setScale(1000.f);
        sp->setRotation3D({90,0,90});
        _mainLayer->addChild(sp);
        sp->configImage("images/dark_shadow.png");
        sp->configBlend(true);
        sp->setVisible(false);
        sp->setOpacity(100);
        sp->setZOrder(3);
        _rightShadow = sp;
    }

    {
        auto sp = RoadPlane::create();
        sp->setCameraMask(_mainCamera->getCameraMask());
        sp->setPosition3D({0,0,0.05});
        sp->setScale(1000.f);
//        sp->setRotation3D({90,0,90});
        _mainLayer->addChild(sp);
        sp->configImage("images/dark_shadow.png");
        sp->configBlend(true);
        sp->setVisible(false);
        sp->setOpacity(100);
        sp->setZOrder(3);
        _middleShadow = sp;
    }

    const float relative_pos = -10;

    // 草
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({relative_pos-5, scene_things_start_pos, 0});
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
        _leftGlass = node;
    }/*
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({30, scene_things_start_pos, 0});
        _mainLayer->addChild(node);
        node->setRotation3D({90,0,90});
        //    glassNode->setScale(j+1);
        auto pixelData = loadScatPixelFile("battle_road/tmp/glass.png.sopx");
        std::vector<PlanePixelBatchTuple> pixelBatchData;
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 10; i++) {
                pixelBatchData.push_back({{-scene_things_width + glass_width_step*i+random(-20.f, 10.f), 0.f, -10.f*j+random(-10.f, 0.f)},1.f + j*0.25f, &pixelData});
            }
        }
        node->configBatch(pixelBatchData, false);

        node->configMoveLine(-scene_things_width);
        node->configMoveWidth(scene_things_width);
        _rightGlass = node;
    }*/

    //树
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({relative_pos-5, scene_things_start_pos, 0});
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
        node->configXDiffAni(0.0, 0.0005, 2.f);
        _leftTrees = node;
    }

//    {
//        auto node = PlanePixelNode::create();
//        node->setCameraMask(_mainCamera->getCameraMask());
//        node->setPosition3D({30, scene_things_start_pos, 0});
//        _mainLayer->addChild(node);
//        node->setRotation3D({90,0,90});
//        auto pixelData = loadScatPixelFile("battle_road/tmp/tree.png.sopx");
//        std::vector<PlanePixelBatchTuple> pixelBatchData;
//        for (int j = 0; j < 3; j++) {
//            for (int i = 0; i < scene_things_width/tree_width_step; i++) {
//                pixelBatchData.push_back({{-scene_things_width + tree_width_step*i+random(-10.f, 10.f), 0.f, -10.f*j+random(-10.f, 0.f)},1.f + j, &pixelData});
//            }
//        }
//        node->configBatch(pixelBatchData);
//
//        node->configMoveLine(-scene_things_width);
//        node->configMoveWidth(scene_things_width);
//        _rightTrees = node;
//        _rightTrees->setVisible(false); //初始不显示
//    }


    // 山
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({relative_pos-25, scene_things_start_pos, 0});
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
        _moutains = node;
//        node->setVisible(false);

    }

    // 远山
    {
        auto node = PlanePixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({relative_pos-65, scene_things_start_pos, 0});
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
        _hills = node;
//        node->setVisible(false);

    }

    // 测试
/*
    for (int i = 0; i < 1; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0, 40+i*20.f, 10});
        _mainLayer->addChild(node);
        node->configSopx("beats/a.png.sopx");
        node->runAction(RepeatForever::create(Sequence::create(FadeOut::create(2.f), FadeIn::create(2.f), NULL)));
    }

    // 测试chars
    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0, -50, 10});
        _mainLayer->addChild(node);
//        node->configSopx("beats/a.png.sopx");
        node->configText("1234567890abcdefghijklmnopqrstuvwxyz",1);
        node->configMixColor({rand_0_1(),rand_0_1(),rand_0_1(),1});
        node->schedule([node](float dt){
            node->configMixColor({rand_0_1(),rand_0_1(),rand_0_1(),1});
            node->configText(fmt::sprintf("%02d%02d%02d%02d", random(0, 999999),random(0, 999999),random(0, 999999),random(0, 999999)), 1);
        }, 0.5, 100, 0.5, "dsfd");
//        node->runAction(RepeatForever::create(Sequence::create(FadeOut::create(2.f), FadeIn::create(2.f), NULL)));
    }*/
    initCastleThings();

}

void BattleRoad::initCastleThings()
{
    const float scale = 1.6;
    {
        auto px = PixelNode::create();
        px->setCameraMask(_mainCamera->getCameraMask());
        px->configSopx("hunters/sopx/castle.png.sopx");
        px->setPosition3D({-2,QuestDef::CASTLE_POS,0});
        px->setRotation3D({90,0,-90});
        px->setScale(QuestDef::ARROW_SCALE);
        _mainLayer->addChild(px);
        _pxCastle = px;
        _pxCastle->setScale(scale);
    }

    {
        auto px = PixelNode::create();
        px->setCameraMask(_mainCamera->getCameraMask());
        px->configSopx("hunters/sopx/castle_bg.png.sopx");
        px->setPosition3D({-3,QuestDef::CASTLE_POS,0});
        px->setRotation3D({90,0,-90});
        px->setScale(QuestDef::ARROW_SCALE);
        _mainLayer->addChild(px);
        _pxCantleBg = px;
        _pxCantleBg->setScale(scale);
    }

    const float heart_pos = 23;
    const float heart_hei = 80;
    {
        auto px = PixelNode::create();
        px->setCameraMask(_mainCamera->getCameraMask());
        px->configSopx("hunters/sopx/castle_heart.png.sopx");
        px->setPosition3D({1,QuestDef::CASTLE_POS+heart_pos,heart_hei});
        px->setRotation3D({90,0,-90});
        px->setScale(QuestDef::ARROW_SCALE);
        px->setScale(scale);
        _mainLayer->addChild(px);
    }
    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.8f,1.f);
        node->setPosition3D({3,QuestDef::CASTLE_POS+heart_pos+2.6f,heart_hei+0.1f});
        node->setRotation3D({90,0,-90});
        node->configText(fmt::sprintf("%02d", _heart),1);
        node->configMixColor({50.f/255.f, 50.f/255.f, 50.f/255.f,1.f});
        _mainLayer->addChild(node);
        _ptxHeart = node;
    }
}

void BattleRoad::op_config(int which)
{
}
void BattleRoad::op_vibrate()
{
}
void BattleRoad::op_moveForword(float moveTime)
{
    float move_time = moveTime;
    float move_length = -moveTime* QuestDef::SCENE_MOVE_SPEED;
    _leftTrees->runAction(MoveBy::create(move_time, {0, move_length,0}));
//    _rightTrees->runAction(MoveBy::create(move_time, {0, move_length,0}));
    _leftGlass->runAction(MoveBy::create(move_time, {0, move_length,0}));
//    _rightGlass->runAction(MoveBy::create(move_time, {0, move_length,0}));
    _moutains->runAction(MoveBy::create(move_time, {0, move_length,0}));
    _hills->runAction(MoveBy::create(move_time, {0, move_length,0}));
    _roadPlane->runAction(MoveBy::create(move_time, {0, move_length,0}));

}

void BattleRoad::op_setRightTreesVisible(bool enable)
{
//    _rightTrees->setVisible(enable);
}

void BattleRoad::op_setLeftHillsVisible(bool enable) //左侧山，减少 vertex 量
{
    _hills->setVisible(enable);
    _moutains->setVisible(enable);
}

void BattleRoad::update(float dt)
{
    _leftTrees->configMoveLine(scene_things_start_pos - _leftTrees->getPosition3D().y);
//    _rightTrees->configMoveLine(-scene_things_width+(_rightTrees->getPosition3D().y - scene_things_start_pos));

    _leftGlass->configMoveLine(scene_things_start_pos - _leftGlass->getPosition3D().y);
//    _rightGlass->configMoveLine(-scene_things_width+(_rightTrees->getPosition3D().y - scene_things_start_pos));

    _moutains->configMoveLine(scene_things_start_pos - _moutains->getPosition3D().y);
    _hills->configMoveLine(scene_things_start_pos - _hills->getPosition3D().y);

}

void BattleRoad::op_applyDarkShadow(float howdark) //场景变暗，用来处理 rival 的死亡，及 combo 大招
{
    _rightShadow->setOpacity(0);
    _rightShadow->setVisible(true);
    _rightShadow->runAction(FadeTo::create(0.2, 255*howdark));

    _leftShadow->setOpacity(0);
    _leftShadow->setVisible(true);
    _leftShadow->runAction(FadeTo::create(0.2, 255*howdark));
}
void BattleRoad::op_dismissDarkShadow() //取消场景变暗
{
    _leftShadow->runAction(Sequence::create(FadeOut::create(0.15), Hide::create(), NULL));
    _rightShadow->runAction(Sequence::create(FadeOut::create(0.15), Hide::create(), NULL));
}

void BattleRoad::op_toastDarkShadow(float howdark, float time) //toast 形式的接口
{
    auto ac = Sequence::create(Show::create(), FadeTo::create(0.3, howdark*255), DelayTime::create(time), FadeOut::create(1.0), NULL);
    _leftShadow->runAction(ac->clone());
    _middleShadow->runAction(ac->clone());
//    _rightShadow->runAction(ac);
}

void BattleRoad::op_configWind(float windDirection) //设置风的方向，表现为树的移动、云的飘动，正-向右，负-向左，大小表示风的强度
{
    _skyPlane->stopAllActions();
    _leftTrees->configXDiffAni(0, 0, 0);
    if (windDirection != 0) {
        _skyPlane->runAction(RepeatForever::create(MoveBy::create(1.f, Vec3{0, windDirection,0})));
        _leftTrees->configXDiffAni(windDirection*0.001, windDirection*0.0003 + 0.0003, std::min(3.f,std::max(5.f/std::abs(windDirection), 1.5f)));
    }
}

void BattleRoad::op_minusHeart()
{
    _heart--;



    if (_heart == 0) {
        // game over
    }
}


