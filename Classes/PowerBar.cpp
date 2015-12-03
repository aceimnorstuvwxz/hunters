//
//  PowerBar.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/22.
//
//

#include "PowerBar.hpp"
#include "format.h"
#include "intersection.h"
#include "SOCommon.h"
#include "QuestDef.hpp"

USING_NS_CC;

void PowerBar::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initPowerThings();
    initTouchThings();
    initPathThings();
}


void PowerBar::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({-5,-3,-10});
    _hubNode->setScale(0.15);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(10);
    _mainCamera->addChild(_hubNode);
    _hubNode->setVisible(false);
}

void PowerBar::initPowerThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.2,1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/strenth_rect.png.sopx");
        node->configBlend(true);
        _hubNode->addChild(node);
        _pxRect = node;

    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.2,1);
        node->setPosition3D({0,0,0});
        node->configSopx("hunters/sopx/strenth_content.png.sopx");
        node->configBlend(true);

        _hubNode->addChild(node);
        _pxContent = node;
    }
    {
        auto node = PixelTextNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(1.1f,2.f);
        node->setPosition3D({-12,8,0});
        node->configText("00#",2.0);
        node->configMixColor({0.f/255.f, 183.f/255.f, 233.f/255.f,1.f});
        _hubNode->addChild(node);
        _ptAngle = node;
    }



    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setScale(0.15);
        node->setPosition3D({0,0,-10});
        node->configSopx("hunters/sopx/power_arrow.png.sopx");
        node->configBlend(true);
        _mainCamera->addChild(node);
        _pxArrow = node;
        node->setVisible(false);
    }
}


void PowerBar::op_show()
{
    _hubNode->setVisible(true);
}

void PowerBar::op_dismiss()
{
    _hubNode->setVisible(false);
}

void PowerBar::update(float dt)
{

}

void PowerBar::op_configPower(float power, int angle) //power [0-1]
{
    _pxContent->configYCut(power*45);
    _ptAngle->configText(fmt::sprintf("%2d#", angle));
}

void PowerBar::initTouchThings()
{
    auto listener = EventListenerTouchOneByOne::create();
    static bool moved = false;
    static Vec2 startPos;
    static float angle, strenth;
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        moved = false;
        startPos = touch->getLocation();
        if (_enable) {
            auto size = Director::getInstance()->getWinSize();
            float yr = touch->getLocation().y / size.height;
            float xr = touch->getLocation().x / size.width;
            CCLOG("power touch radio %f %f", xr, yr);
            // ban 掉一些区域
            if (xr > 0.9 && yr >0.85) { //暂停
            } else if (yr < 0.19) { //机能区域
            } else if (yr > 0.85 && xr > 0.17 && xr < 0.65) { //头像区域
            } else {
                // TODO 告诉 heros 开始 aiming
                _huntingHerosManageProtocal->op_aimingStart();
//                op_show();
                auto p = touch->getLocation();
                auto r = _mainCamera->unproject(Vec3{p.x,p.y,0});
                r.x = 40;
                r.y = 10*r.y;
                r.z = 10*r.z;
                Vec3 rr;
                _mainCamera->getWorldToNodeTransform().transformVector(r, &rr);
                CCLOG("r %f %f %f", r.x, r.y, r.z);
                //这里获得的是一个在镜头截面上的值，根据镜头的近切面和我们需要坐标所在平面来换算。
                rr.z = -10;
                rr.y = 100-rr.y;
                

                CCLOG("rr %f %f %f", rr.x, rr.y, rr.z);
                _pxArrow->stopAllActions();
                _pxArrow->setPosition3D(rr);
//                _pxArrow->setVisible(true);
                _pxArrow->setScale(0);
                return true;
            }
        }
        return false;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){
        moved = true;
        auto diff = (touch->getStartLocation() - touch->getLocation());
        angle = vector2angel(diff);
        if (angle>180) {
            angle = angle-360;
        }
        auto size = Director::getInstance()->getWinSize();
        strenth =  diff.length() / (size.height*0.35);
        _pxArrow->setScale(strenth*0.1);
        _pxArrow->setRotation3D({0,0,-(angle-45)});
        //TODO 告诉 heros
        strenth = std::min(1.f, strenth);
        _huntingHerosManageProtocal->op_configAiming(angle, strenth);
//        this->op_configPower(strenth, angle);
        showPath(angle, strenth);
    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){
        op_dismiss();
        _pxArrow->runAction(Sequence::create(ScaleTo::create(0.2, 0), Hide::create(), NULL));
        _huntingHerosManageProtocal->op_toastBow(angle, strenth);
        this->hidePath();
    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);

}

void PowerBar::initPathThings()
{

    _pathHubNode = Node::create();
    float y = heroPositionType2floatYposition(HeroPositionType::HPT_0);
    _pathHubNode->setPosition3D({0,y,QuestDef::ARROW_SHOOT_Z});
    _pathHubNode->setRotation3D({90,0,-90});
    _pathHubNode->setScale(1);
    _pathHubNode->setCameraMask(_mainCamera->getCameraMask());
    _pathHubNode->setZOrder(10);
    _mainLayer->addChild(_pathHubNode);
    _pathHubNode->setVisible(false);

    for (int i = 0; i < N_PATH_POINTS; i++) {
        auto n = PixelNode::create();
        n->setCameraMask(_mainCamera->getCameraMask());
        n->configSopx("hunters/sopx/path_point.png.sopx");
        n->configBlend(true);
        _pathHubNode->addChild(n);
        _pxPathPoints[i] = n;
    }
}

void PowerBar::showPath(float angle, float strenth)
{
    _pathHubNode->setVisible(true);
    const float time_step = 0.25;
    auto speed = huntingCalcSpeed(angle, strenth);
    auto acce = huntingCalcAcce(_windBarProtocal->op_fetchWind());
    for (int i = 0; i < N_PATH_POINTS; i++) {
        auto pos = movePathCalc({0,0}, speed, acce, time_step*(i+0.5));
        _pxPathPoints[i]->setPosition(pos.x,pos.y);
        _pxPathPoints[i]->setScale(0.2f-0.1f*i/N_PATH_POINTS);
    }

}

void PowerBar::hidePath()
{
    _pathHubNode->setVisible(false);
}

