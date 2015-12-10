//
//  GlobalParticleManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/9.
//
//

#ifndef GlobalParticleManage_hpp
#define GlobalParticleManage_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FlyingCrow.hpp"
#include "PixelParticleNode.hpp"

class GlobalParticleManage: public GlobalParticleManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

//    void configProtocals(HuntingMonsterManageProtocal* monsterManageProtocal) {
//        _monsterManageProtocal = monsterManageProtocal;
//    }
    virtual void toastPartles(GlobalParticleType t, cocos2d::Vec2 pos, cocos2d::Vec2 dir, int degree);
    virtual void op_configPaused(bool pause) ;

protected:
//    HuntingMonsterManageProtocal* _monsterManageProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    PixelParticleNode* _particleNode;

    void initEffectsThings();
};

#endif /* GlobalParticleManage_hpp */
