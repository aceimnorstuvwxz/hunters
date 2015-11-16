//
//  WildHuntingScene.cpp
//  WildingHunting
//
//  Created by chenbingfeng on 15/11/15.
//
//

#include "WildHuntingScene.hpp"
#include "SOCommon.h"

USING_NS_CC;

bool WildHuntingTestScene::init()
{
    assert(TRBaseScene::init());



    _mainLayer = Layer::create();
    this->addChild(_mainLayer);


    _spBackground = Sprite::create("hunting/bg.png");
    _spBackground->setPosition(genPos({0.5,0.5}));
    _spBackground->setScale(1.1);
    _mainLayer->addChild(_spBackground);

    _spHeroDir = Sprite::create("hunting/herodir.png");
    _spHeroDir->setPosition(genPos({0.5,0.5}));
    _mainLayer->addChild(_spHeroDir);
    _spHeroDir->setVisible(false);

    _spHero = Sprite::create("hunting/shooter.png");
    _spHero->setPosition(genPos({0.5,0.5}));
    _mainLayer->addChild(_spHero);

    _spRival = Sprite::create("hunting/rival.png");
    _spRival->setPosition(genPos({0.6,0.5}));
    _mainLayer->addChild(_spRival);

    initTouchThings();

    scheduleUpdate();

    return true;
}

void WildHuntingTestScene::initTouchThings()
{
    _spDir = Sprite::create("hunting/dir.png");
    _spDir->setPosition(genPos({0.2,0.3}));
    _spDir->setZOrder(99);
    _spDir->setScale(3.0);
    _mainLayer->addChild(_spDir);

    _spDirBg = Sprite::create("hunting/dirbg.png");
    _spDirBg->setPosition(genPos({0.2,0.3}));
    _spDirBg->setZOrder(99);
    _spDirBg->setScale(3.0);
    _mainLayer->addChild(_spDirBg);


    _spTriger = Sprite::create("hunting/triger.png");
    _spTriger->setPosition(genPos({0.8,0.3}));
    _spTriger->setZOrder(99);
    _spTriger->setScale(3.0);
    _mainLayer->addChild(_spTriger);

    _pxTunnel = PixelNode::create();
    _pxTunnel->configSopx("hunting/tunnel.png.sopx");
    _pxTunnel->setPosition(genPos({0.5,0.5}));
    _mainLayer->addChild(_pxTunnel);

    static bool touch_moved = false;
    auto listener = EventListenerTouchAllAtOnce::create();

    listener->onTouchesBegan = [this](const std::vector<Touch*>& touches, Event* event){

        auto size = Director::getInstance()->getWinSize();
        for (auto touch : touches) {
            Vec2 p = touch->getLocation();
            if (p.x / size.width < 0.5) {
                //左边，是方向按钮的控制
                _diring = true;
                _dirCenter = p;
                _spDir->setPosition(p);
                _spDirBg->setPosition(p);
            } else {
                //右侧按钮

            }

        }

    };

    listener->onTouchesMoved = [this](const std::vector<Touch*>& touches, Event* event){
        auto size = Director::getInstance()->getWinSize();

        for (auto touch : touches) {
            Vec2 p = touch->getLocation();
            if (p.x / size.width < 0.5) {
                //左边，是方向按钮的控制
                _spDir->setPosition(p);
                _dirTarget = p;
            } else {
                //右侧按钮

            }
            
        }
    };

    listener->onTouchesEnded = [this](const std::vector<Touch*>& touches, Event* event){
        auto size = Director::getInstance()->getWinSize();
        for (auto touch : touches) {
            Vec2 p = touch->getLocation();
            if (p.x / size.width < 0.5) {
                //左边，是方向按钮的控制
                _diring = false;
                auto ac = MoveTo::create(0.3, genPos({0.2,0.3}));
                _spDir->runAction(ac->clone());
                _spDirBg->runAction(ac);
            } else {
                //右侧按钮
                //射击
                shoot();
            }
        }
    };

    listener->onTouchesCancelled = [this](const std::vector<Touch*>& touches, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);


}

void WildHuntingTestScene::update(float dt)
{

    //dir
    if (_diring) {
        _spHeroDir->setRotation(vector2angel(_dirTarget-_dirCenter));

        _pxTunnel->setRotation(vector2angel(_dirTarget-_dirCenter));

        if ((_dirTarget-_dirCenter).length() > 80) {
            CCLOG("dir move");

            Vec2 dir = _dirTarget-_dirCenter;
            dir = dir*(1.f/dir.length());
            const float moveSpeed = 200;
            Vec2 oldPos = _spHero->getPosition();
            Vec2 newPos = oldPos + dir*moveSpeed*dt;
            _spHero->setPosition(newPos);
            _spHeroDir->setPosition(newPos);
            _pxTunnel->setPosition(newPos);

            //TODO 打地图的相对移动
        } else {
            CCLOG("only dir");
        }
    }

}

void WildHuntingTestScene::shoot()
{
    CCLOG("shoot");
    //后坐力
    Vec2 dir = _dirTarget-_dirCenter;
//    dir.y = 0;
    dir = dir*(1.f/dir.length());
    Vec2 backMove = -dir * 30;
    auto ac = MoveBy::create(0.1, backMove);
    _spHeroDir->runAction(ac->clone());
    _spHero->runAction(ac->clone());
    _pxTunnel->runAction(ac);

    //弹道
    const float in_time = 0.05;
    _pxTunnel->setScaleX(0.01);
    _pxTunnel->runAction(Sequence::create(Show::create(), Spawn::create(FadeIn::create(in_time),ScaleTo::create(in_time, 0.5, 1), NULL), FadeOut::create(0.1), NULL));

    const float shake_time = 0.06;
    const float shake_length = 5;
    _mainLayer->runAction(Sequence::create(MoveBy::create(shake_time, {shake_length, shake_length}),MoveBy::create(shake_time, {-shake_length, -shake_length}),MoveBy::create(shake_time, {shake_length, shake_length}),MoveBy::create(shake_time, {-shake_length, -shake_length}), NULL));
}