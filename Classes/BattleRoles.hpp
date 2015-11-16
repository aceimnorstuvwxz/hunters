//
//  BattleRoles.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/4.
//
//

#ifndef BattleRoles_hpp
#define BattleRoles_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "HeroGraphicNode.hpp"
#include "RivalGraphicNode.hpp"
#include "QuestProtocals.hpp"
// 双方战士，战士的各种效果和动作，

class BattleRoles:public BattleRolesProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    //每个单次战斗前要设置好hero和rival的图形属性（穿哪套衣服，打哪个怪）
    virtual void op_heroConfig() override;
    virtual void op_rivalConfig() override;
    virtual void op_heroRunIn() override; //新关卡一开始，hero 从左边屏幕外跑入背景不动，跑到一定位置，停下，继续跑
    virtual void op_rivalMoveIn() override; //rival 进场

    virtual void op_switchToBattle()override; //告知镜头转向battle，此时rival需要转过身来。hero则要不再显示。
    virtual void op_switchToForward()override; //
    virtual void op_rivalDie()override; //播放rival的死亡动画
    virtual void op_rivalBeAttack()override; //播放rival被攻击的动作
    virtual void op_rivalBeHealth()override; //播放 rival 被治疗的动作
    virtual void op_rivalAttack()override; //播放rival发动攻击的动作
    virtual void op_rivalStartDefense()override; //rival 举盾
    virtual void op_rivalStopDefense() override; //rival 取消举盾
    virtual void op_heroVisible(bool enable)override;

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    HeroGraphicNode _hero;
    RivalGraphicNode _rival;
};

#endif /* BattleRoles_hpp */
