//
//  EnergyBar.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/20.
//
//

#ifndef EnergyBar_hpp
#define EnergyBar_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"

// 气的聚集 3个气技能
class EnergyBar:public EnergyBarProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    virtual void op_addEnergy(float howmuch); //通知怪物死亡，加气

    void update(float dt);
protected:

    float _targetEnergy = 0; //[0-1]
    float _currentEnergy = 0;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;


    cocos2d::Node* _hubNode;
    void initHubThings();

    PixelNode* _pxBg;
    PixelNode* _pxScope;
    PixelNode* _pxContent;
//    PixelNode* _pxSkillA;
//    PixelNode* _pxSkillB;
//    PixelNode* _pxSkillC;
    void initEnergyThings();
    void updateEnegyContent(float last, float curr);
    void initInteractiveThings();
    void tryReleaseSkill(EnergySkillType st);

};

#endif /* EnergyBar_hpp */
