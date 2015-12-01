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

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    std::list<std::shared_ptr<HuntingMonster>> _monsters;
    int _currentWaveMonsterCnt = 5;
    int _currentWave = 0;
    float _timeLeft = 10; //初始的5秒休息

    void addMonster(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level);
};

#endif /* HuntingMonsterManage_hpp */
