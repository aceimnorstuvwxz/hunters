//
//  TornadoManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/11.
//
//

#include "TornadoManage.hpp"
#include "ACSoundManage.hpp"

USING_NS_CC;


void TornadoManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
    initEffectsThings();
}

void TornadoManage::initEffectsThings()
{
    _particleNode = PixelParticleNode::create();
    _particleNode->setCameraMask(_mainCamera->getCameraMask());
    _particleNode->setRotation3D({90,0,-90});
    _particleNode->setScale(1);
    _mainLayer->addChild(_particleNode);
}

std::pair<float, float> TornadoManage::op_getTornados() //当不存在时返回的位置为一个-MAX的值，保证箭不会碰到
{
    return {_posT0, _posT1};
}

void TornadoManage::op_configTornado(bool t0, float t0Pos, bool t1, float t1Pos)//最多两个 tornado
{
    _posT0 = t0 ? t0Pos : -1000;
    _posT1 = t1 ? t1Pos : -1000;
    //取消之前的所有
    for (auto& t : _tornados) {
        t.enable = false;
    }
    //加入新的
    if (t0) {
        _tornados.push_back({t0Pos, 0.f, 0.f, true});
    }
    if (t1) {
        _tornados.push_back({t1Pos, 0.f, 0.f, true});
    }
    if (t0 || t1) {
        ACSoundManage::s()->play(ACSoundManage::SN_TORNADO, true);
    } else {
        ACSoundManage::s()->stop(ACSoundManage::SN_TORNADO);
    }

}

void TornadoManage::dealWithEveryTornado(TornadoConfig& cfg, float dt)
{
    const float move_speed = 100;
    const float step = 5;
    const float time = 0.5f;
    const float xspeed = 15.f;
    const float xdiff = time*xspeed*0.5f;

    if (cfg.enable && cfg.top < TOP) {
        cfg.top += move_speed*dt;
    } else if (!cfg.enable) {
        cfg.bottom += move_speed*dt;
    }

    for (float now = cfg.bottom; now < cfg.top; now += step) {
        _particleNode->addParticleBatch(random(0, 1), time, 0.1f, {cfg.pos - xdiff, now, -1}, {0,step,0}, {0.8,0.8,0.8,1}, {0.2,0.2,0.2,0}, {xspeed,5,0}, {1,0.5,0}, 3, 0.2, 3, 0.2);


//        _particleNode->addParticleBatch(random(0, 1), time, 0.1f, {cfg.pos + xdiff, now, -3}, {0,step,0}, {0.7,0.7,0.99,1}, {0.2,0.2,0.2,0}, {-xspeed,5,0}, {1,0.5,0}, 3, 0.2, 3, 0.2);
    }

}

void TornadoManage::update(float dt)
{
    static int count = 0;
    count ++;
    if (count == 3) {
        count = 0;
        for (auto iter = _tornados.begin(); iter != _tornados.end();) {
            dealWithEveryTornado(*iter, dt);
            if (!iter->enable && iter->top <= iter->bottom) {
                iter = _tornados.erase(iter);
            } else {
                iter++;
            }
        }

    }
}

void TornadoManage::op_configPaused(bool pause)
{
    if (pause) {
        _particleNode->pause();
    } else {
        _particleNode->resume();
    }
}