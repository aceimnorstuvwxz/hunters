//
//  HuntingArrowManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/1.
//
//

#include "HuntingArrowManage.hpp"


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
    auto px = PixelNode::create();
    px->setCameraMask(_mainCamera->getCameraMask());
    px->configSopx("hunters/arrows/00.png.sopx");
    px->setPosition3D({0,0,0});
    px->setRotation3D({90,0,-90});
    _mainLayer->addChild(px);

    float spx = strenth * std::cos(angle/180*3.1415926);
    float spy = strenth * std::sin(angle/180*3.1415926);
    const float speed_scale = 10;
    cocos2d::Vec2 speed = speed_scale*Vec2{spx,spy};

    _arrowUnits.push_back({px,speed});
}

bool HuntingArrowManage::dealWithUnit(ArrowUnit& unit, float dt)
{
    unit._pxNode->setPositionY(unit._speed.x * dt + unit._pxNode->getPositionY()); //x

    unit._pxNode->setPositionZ(unit._speed.y * dt + unit._pxNode->getPositionZ()); //y
    return false;
}
