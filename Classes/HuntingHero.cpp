//
//  HuntingHero.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/24.
//
//

#include "HuntingHero.hpp"

#include "format.h"
#include "intersection.h"

USING_NS_CC;
void HuntingHero::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initHeroThings();
}

void HuntingHero::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,0,-10});
    _hubNode->setScale(0.15);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(9);
    _mainCamera->addChild(_hubNode);
    //    _hubNode->setVisible(false);
}
void HuntingHero::initHeroThings()
{
    _dpxNode = DynamicPixelNode::create();
    _dpxNode->setPosition3D({0,0,0});
    _hubNode->addChild(_dpxNode);
    _dpxNode->setCameraMask(_mainCamera->getCameraMask(), true);
}
void HuntingHero::op_configSuitBow(int suitId, int bowId) //指定穿着套装和弓
{
    //会清空 VBO 后，重新写数据


}
void HuntingHero::op_configRelativeAngle(float angle) //逆时针变大，设置单个英雄的相对设计角度
{

}
void HuntingHero::op_startAiming() //开始瞄准，会举起弓到水平位置
{

}
void HuntingHero::op_stopAiming() //取消瞄准，会放下弓
{

}
void HuntingHero::op_toastShoot() //瞄准完毕后的发射动作，发射后会放下弓
{

}
void HuntingHero::op_configAiming(float angle, float strenth)
{

}
void HuntingHero::op_toastUnderAttack() //播放被攻击动画
{

}
void HuntingHero::op_toastDead() //播放死亡
{

}
void HuntingHero::op_toastShield(float interval) //播放一段时间护盾
{

}
void HuntingHero::op_toastBoost(float interval) //播放一段时间的暴击增益
{

}
void HuntingHero::op_toastUpgrade() //播放升级动画
{

}
