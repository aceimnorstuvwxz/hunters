//
//  TornadoManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/11.
//
//

#ifndef TornadoManage_hpp
#define TornadoManage_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FlyingCrow.hpp"
#include "PixelParticleNode.hpp"

struct TornadoConfig
{
    float pos;
    float top;
    float bottom;
    bool enable;
};

class TornadoManage: public TornadoManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    //    void configProtocals(HuntingMonsterManageProtocal* monsterManageProtocal) {
    //        _monsterManageProtocal = monsterManageProtocal;
    //    }
    virtual void op_configTornado(bool t0, float t0Pos, bool t1, float t1Pos);//最多两个 tornado
    virtual void op_configPaused(bool pause);
    virtual std::pair<float, float> op_getTornados(); //当不存在时返回的位置为一个-MAX的值，保证箭不会碰到

    void update(float dt);

protected:
    //    HuntingMonsterManageProtocal* _monsterManageProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    static constexpr float TOP = 110;

    PixelParticleNode* _particleNode;

    std::deque<TornadoConfig> _tornados;
    void dealWithEveryTornado(TornadoConfig& cfg, float dt);

    void initEffectsThings();

    float _posT0;
    float _posT1;
};
#endif /* TornadoManage_hpp */
