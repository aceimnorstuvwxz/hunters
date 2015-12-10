//
//  HuntingHero.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/24.
//
//

#ifndef HuntingHero_hpp
#define HuntingHero_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
// 一个战斗英雄
// 地面单位都有一个 X 坐标

class HuntingHero:HuntingHeroProtocal
{
public:
    enum BoneIndexType
    {
        BT_HEAD,
        BT_HAIR,
        BT_BODY,
        BT_HAND_L,
//        BT_HAND_R,
        BT_LEG_L,
        BT_LEG_R,
        BT_BOW_MAX,
        BT_ARROW
    };
    static int boneIndexType2sopxId(int boneIndexType);
    static cocos2d::Vec3 boneIndex2relativePosition(int boneIndexType);
    cocos2d::Vec3 help_calcBonePosition(int boneIndexType);

    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(HuntingArrowManageProtocal* huntingArrowManageProtocal) {
        _huntingArrowManageProtocal = huntingArrowManageProtocal;
    }


    virtual void op_configHeroTypeAndGrade(HeroType heroType, int grade); // 由英雄类型和等级 来指定穿着套装和弓
    virtual void op_configRelativeAngle(float angle); //逆时针变大，设置单个英雄的相对设计角度
    virtual void op_startAiming(); //开始瞄准，会举起弓到水平位置
    virtual void op_stopAiming(); //取消瞄准，会放下弓
    virtual void op_toastShoot(float angle, float strenth); //瞄准完毕后的发射动作，发射后会放下弓
    virtual void op_configAiming(float angle, float strenth);
    virtual void op_toastUnderAttack(); //播放被攻击动画
    virtual void op_toastDead(); //播放死亡
    virtual void op_toastShield(float interval); //播放一段时间护盾
    virtual void op_toastBoost(float interval); //播放一段时间的暴击增益
    virtual void op_toastUpgrade(); //播放升级动画
    virtual void op_show();
    virtual void op_hide();
    virtual void op_move(HeroPositionType desPositionType, bool direct = false);
    int op_fetchSuitId() { return _suidId; }
    virtual void op_configPaused(bool pause);


protected:
    HuntingArrowManageProtocal* _huntingArrowManageProtocal;
    HeroType _heroType = HeroType::HT_NONE;
    int _heroGrade = 0;
    int _suidId;
    HeroPositionType _heroPositionType = HeroPositionType::HPT_OUT;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();

    DynamicPixelNode* _dpxNode; //本体
    void initHeroThings();

    //动作配置 以下动作，一旦执行，就会一直做下去，直到设置成其它动作。
    void ani_idle(); //空闲动作
    void ani_run(); //跑步动作


};


#endif /* HuntingHero_hpp */
