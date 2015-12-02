//
//  HuntingArrowManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/1.
//
//

#include "HuntingArrowManage.hpp"
#include "SOCommon.h"
#include "QuestDef.hpp"


USING_NS_CC;



void HuntingArrowManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
}

void HuntingArrowManage::update(float dt)
{
    for (auto iter =  _arrowUnits.begin(); iter != _arrowUnits.end(); ) {

        if (dealWithUnit(*iter, dt)) {
            iter = _arrowUnits.erase(iter);
        } else {
            iter++;
        }
    }
}

void HuntingArrowManage::op_shootArrow(HuntingArrowType arrowType, HeroPositionType position, float angle, float strenth) //放箭

{
    float y = heroPositionType2floatYposition(position) + QuestDef::ARROW_SHOOT_Y;
    auto px = PixelNode::create();
    px->setCameraMask(_mainCamera->getCameraMask());
    px->configSopx("hunters/arrows/00.png.sopx");
    px->setPosition3D({0,y,QuestDef::ARROW_SHOOT_Z});
    px->setRotation3D({90,0,-90});
    px->setScale(QuestDef::ARROW_SCALE);
    _mainLayer->addChild(px);

    float spx = strenth * std::cos(angle/180*3.1415926);
    float spy = strenth * std::sin(angle/180*3.1415926);
    const float speed_scale = 100;
    cocos2d::Vec2 speed = speed_scale*Vec2{spx,spy};

    _arrowUnits.push_back({px,speed});
}

bool HuntingArrowManage::dealWithUnit(ArrowUnit& unit, float dt)
{
    const float gravity = 10;
    const float windpower = 2;

    cocos2d::Vec2 acce = {windpower*_windBarProtocal->op_fetchWind(), -gravity};

    unit._pxNode->setPositionY(unit._speed.x * dt + unit._pxNode->getPositionY()); //x

    unit._pxNode->setPositionZ(unit._speed.y * dt + unit._pxNode->getPositionZ()); //y

    unit._pxNode->setRotation3D({90, 45-vector2angel(unit._speed), -90});


    unit._speed += acce*dt;
    
    return false;
}
