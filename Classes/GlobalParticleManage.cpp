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
    /*int count, float time, float timeVar, cocos2d::Vec3 position, cocos2d::Vec3 positionVar, cocos2d::Vec4 color, cocos2d::Vec4 colorVar, cocos2d::Vec3 speed, cocos2d::Vec3 speedVar, float beginScale, float beginScaleVar, float endScale, float endScaleVar*/
    if (t == GlobalParticleType::MONSTER_ACCE) {
        _particleNode->addParticleBatch(random(1, 2), 1, 0.5, {pos.x,2,0}, {2,3,0}, {96/255.f,25/255.f,134/255.f,0.5f}, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
    else if (t == GlobalParticleType::CROW_ACCE) {
        _particleNode->addParticleBatch(random(0, 1), 1, 0.5, {pos.x+2,pos.y,-1}, {2,3,0}, {96/255.f,25/255.f,134/255.f,0.5f}, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
    else if (t == GlobalParticleType::ARROW_NORMAL) {
        Vec4 color = degree == 0 ? Vec4{200/255.f,200/255.f,200/255.f,0.5f} :
        degree == 1 ? Vec4{240/255.f,200/255.f,100/255.f,0.5f} : Vec4{250/255.f,200/255.f,50/255.f,0.5f};
        _particleNode->addParticleBatch(random(0, 1), 0.5+0.15*degree, 0.5, {pos.x+2,pos.y,-1}, {1,2,0}, color, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
    else if (t == GlobalParticleType::ARROW_BOMB) {
        Vec4 color = Vec4{200/255.f,50/255.f,50/255.f,0.5f};
        _particleNode->addParticleBatch(random(0, 1), 0.5+0.15*degree, 0.5, {pos.x+2,pos.y,-1}, {2,3,0}, color, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
    else if (t == GlobalParticleType::ARROW_MULTI) {
        Vec4 color = Vec4{230/255.f,100/255.f,150/255.f,0.5f};
        _particleNode->addParticleBatch(random(0, 1), 0.5+0.15*degree, 0.5, {pos.x+2,pos.y,-1}, {2,3,0}, color, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
    else if (t == GlobalParticleType::ARROW_HIGH) {
        Vec4 color = Vec4{60/255.f,60/255.f,60/255.f,0.5f};
        _particleNode->addParticleBatch(random(0, 1), 0.5+0.15*degree, 0.5, {pos.x+2,pos.y,-1}, {2,3,0}, color, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
    else if (t == GlobalParticleType::ARROW_SLOW) {
        Vec4 color = Vec4{60/255.f,250/255.f,250/255.f,0.5f};
        _particleNode->addParticleBatch(random(0, 1), 0.5+0.15*degree, 0.5, {pos.x+2,pos.y,-1}, {2,3,0}, color, {0.2,0.2,0.2,0.2}, {0,0,0}, {3,3,3}, 2, 0.5, 0.5, 0.25);
    }
}