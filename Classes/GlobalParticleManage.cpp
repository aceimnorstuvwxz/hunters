//
//  GlobalParticleManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/9.
//
//

#include "GlobalParticleManage.hpp"
#include "ACSoundManage.hpp"

USING_NS_CC;


void GlobalParticleManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
    initEffectsThings();
}

void GlobalParticleManage::initEffectsThings()
{
    _particleNode = PixelParticleNode::create();
    _particleNode->setCameraMask(_mainCamera->getCameraMask());
    _particleNode->setRotation3D({90,0,-90});
    _particleNode->setScale(1);
    _mainLayer->addChild(_particleNode);
}

void GlobalParticleManage::toastPartles(GlobalParticleType t, cocos2d::Vec2 pos, cocos2d::Vec2 dir, int degree)
{
    if (t == GlobalParticleType::MONSTER_ACCE) {
        _particleNode->addParticleBatch(random(1, 2), 1, 0.5, {pos.x+3,2,0}, {2,3,0}, {96/255.f,25/255.f,134/255.f,0.5f}, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
    if (t == GlobalParticleType::CROW_ACCE) {
        _particleNode->addParticleBatch(random(0, 1), 1, 0.5, {pos.x+2,pos.y,-1}, {2,3,0}, {96/255.f,25/255.f,134/255.f,0.5f}, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
}