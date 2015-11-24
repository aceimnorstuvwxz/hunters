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

// 一个战斗英雄
// 地面单位都有一个 X 坐标

class HuntingHero
{
public:
    enum BoneIndexType
    {
        BT_HEAD,
        BT_HAIR,
        BT_BODY,
        BT_HAND_L,
        BT_HAND_R,
        BT_LEG_L,
        BT_LEG_R,
        BT_BOW
    };
    enum HeroPositionType
    {
        HPT_0, //最前排
        HPT_1,
        HPT_2,
        HPT_3, //最后
        HPT_OUT //视野外
    };

    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);


    void op_configSuitBow(int suitId, int bowId); //指定穿着套装和弓
    void op_configRelativeAngle(float angle); //逆时针变大，设置单个英雄的相对设计角度
    void op_startAiming(); //开始瞄准，会举起弓到水平位置
    void op_stopAiming(); //取消瞄准，会放下弓
    void op_toastShoot(); //瞄准完毕后的发射动作，发射后会放下弓
    void op_configAiming(float angle, float strenth);
    void op_toastUnderAttack(); //播放被攻击动画
    void op_toastDead(); //播放死亡
    void op_toastShield(float interval); //播放一段时间护盾
    void op_toastBoost(float interval); //播放一段时间的暴击增益
    void op_toastUpgrade(); //播放升级动画
    void op_show();
    void op_dismiss();
    void op_move(int desPosition, bool direct = false);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();

    DynamicPixelNode* _dpxNode; //本体
    void initHeroThings();


};


#endif /* HuntingHero_hpp */
