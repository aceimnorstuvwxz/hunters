//
//  GlobalArrowEffectManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/7.
//
//

#ifndef GlobalArrowEffectManage_hpp
#define GlobalArrowEffectManage_hpp


#include <stdio.h>
#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FlyingCrow.hpp"

class GlobalArrowEffectManage: public GlobalArrowEffectManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    void configProtocals(HuntingMonsterManageProtocal* monsterManageProtocal) {
        _monsterManageProtocal = monsterManageProtocal;
    }

    virtual void op_bomb(int grade, cocos2d::Vec2 pos);
    virtual void op_thunder(cocos2d::Vec2 pos);

    void update(float dt);

protected:
    HuntingMonsterManageProtocal* _monsterManageProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

//    PixelNode* _pxThunder;
//    PixelNode*
    DynamicPixelNode* _dxBaseBomb;
    DynamicPixelNode* _dxSuperBomb;
    DynamicPixelNode* _dxThunder;

    void initEffectsThings();

};

#endif /* GlobalArrowEffectManage_hpp */
