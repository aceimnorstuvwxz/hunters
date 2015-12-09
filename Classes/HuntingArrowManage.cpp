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
#include "format.h"


USING_NS_CC;



void HuntingArrowManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
}

void HuntingArrowManage::update(float dt)
{
    for (auto& arrow : _arrowUnits) {
        dealWithUnit(arrow, dt);
    }
    for (auto iter = _arrowUnits.begin(); iter != _arrowUnits.end();) {
        if (iter->_leftHitTimes <= 0) {
            iter->_pxNode->removeFromParent();
            iter = _arrowUnits.erase(iter);
        } else {
            iter++;
        }
    }

}
void HuntingArrowManage::op_shootArrow(HuntingArrowType arrowType, HeroPositionType position, float angle, float strenth)  //放箭
{
    int arrowCount = 1;
    if (arrowType == HuntingArrowType::MULTI_0) {
        arrowCount = 2;
    } else if (arrowType == HuntingArrowType::MULTI_1) {
        arrowCount = 3;
    } else if (arrowType == HuntingArrowType::MULTI_2) {
        arrowCount = 4;
    }
    for (int i = 0; i < arrowCount; i++) {
        op_shootArrowAst(arrowType, position, angle+(i-arrowCount*0.5f)*5.f, strenth);
    }
    ACSoundManage::s()->play(ACSoundManage::SN_ARROW_SHOOT);
}

void HuntingArrowManage::op_shootArrowAst(HuntingArrowType arrowType, HeroPositionType position, float angle, float strenth) //放箭

{
    float y = heroPositionType2floatYposition(position) + QuestDef::ARROW_SHOOT_Y;
    auto px = PixelNode::create();
    px->setCameraMask(_mainCamera->getCameraMask());
    px->configSopx(fmt::sprintf("hunters/arrows/%02d.png.sopx", static_cast<int>(arrowType)));
    px->setPosition3D({0,y,QuestDef::ARROW_SHOOT_Z});
    px->setRotation3D({90,0,-90});
    px->setScale(QuestDef::ARROW_SCALE);
    _mainLayer->addChild(px);

    cocos2d::Vec2 speed = huntingCalcSpeed(angle, strenth);

    _arrowUnits.push_back({px,speed,arrowType,hitTimesOfArrow(arrowType),{}});
}

 void HuntingArrowManage::dealWithUnit(ArrowUnit& unit, float dt)
{

    cocos2d::Vec2 acce = huntingCalcAcce(_windBarProtocal->op_fetchWind());

    unit._pxNode->setPositionY(unit._speed.x * dt + unit._pxNode->getPositionY()); //x

    unit._pxNode->setPositionZ(unit._speed.y * dt + unit._pxNode->getPositionZ()); //y

    unit._pxNode->setRotation3D({90, 45-vector2angel(unit._speed), -90});

    auto at = arrowType2particleType(unit._type);
    _particleManageProtocal->toastPartles(at.first, {unit._pxNode->getPositionY(),unit._pxNode->getPositionZ()}, {0,0}, at.second);


    unit._speed += acce*dt;


    _huntingMonsterManageProtocal->op_dealCollision(unit);
    _flyingCrowManageProtocal->op_dealCollision(unit);

}
