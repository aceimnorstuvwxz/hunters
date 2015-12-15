//
//  QuestCameraManage.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#ifndef QuestCameraManage_hpp
#define QuestCameraManage_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"

class QuestCameraManage:public QuestCameraManageProtocal
{
public:
    void init(cocos2d::Camera* mainCamera);
    virtual void op_switchToPosition(QuestCameraPosition p, bool direct = false); //切换镜头的不同位置, direct表示是否直接设置而非动画


    virtual void op_startRunningShake();
    virtual void op_stopRunningShake();

//    void configProtocals(BattleRoadProtocal* battleRoadProtocal, BattleRolesProtocal* battleRolesProtocal, BloodBarProtocal* bloodBarProtocal);

protected:
//    BattleRoadProtocal* _battleRoadProtocal;
//    BattleRolesProtocal* _battleRolesProtocal;
//    BloodBarProtocal* _bloodBarProtocal;
    cocos2d::Camera* _mainCamera;


};

#endif /* QuestCameraManage_hpp */
