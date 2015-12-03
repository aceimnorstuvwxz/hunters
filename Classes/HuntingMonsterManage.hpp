//
//  HuntingMonsterManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/2.
//
//

#ifndef HuntingMonsterManage_hpp
#define HuntingMonsterManage_hpp


#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"
#include "HeroHeadAndUpgrade.hpp"
#include "HuntingMonster.hpp"


class HuntingMonsterManage:public HuntingMonsterManageProtocal
{
public:

    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    virtual void op_dealCollision(ArrowUnit& arrow); 

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    std::list<std::shared_ptr<HuntingMonster>> _monsters;
    int _currentWaveMonsterCnt = 7;
    int _currentWave = 0;
    float _timeLeft = 10; //初始的5秒休息

    void addMonster(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level);

    bool dealWithPerMonster(std::shared_ptr<HuntingMonster> monster, ArrowUnit& arrow);
};

#endif /* HuntingMonsterManage_hpp */
