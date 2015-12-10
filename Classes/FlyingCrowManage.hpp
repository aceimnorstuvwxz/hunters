//
//  FlyingCrowManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/7.
//
//

#ifndef FlyingCrowManage_hpp
#define FlyingCrowManage_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FlyingCrow.hpp"

class FlyingCrowManage: public FlyingCrowManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    void configProtocals(TopIconsProtocal* topIconsProtocal, GlobalParticleManageProtocal* particleManageProtocal) {
        _topIconsProtocal = topIconsProtocal;
        _particleManageProtocal = particleManageProtocal;
    }
    virtual void op_dealCollision(ArrowUnit& arrow);
    virtual void op_configPaused(bool pause);


    void update(float dt);

protected:
    GlobalParticleManageProtocal* _particleManageProtocal;
    TopIconsProtocal* _topIconsProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    std::list<std::shared_ptr<FlyingCrow>> _crows;
    int _batchCount = 0;
    float _timeLeft = 1;

    void addBatchOfCrows();
    void addCrow(int x, int y, int h, bool cry);

};

#endif /* FlyingCrowManage_hpp */
