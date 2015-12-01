//
//  HuntingHero.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/24.
//
//

#ifndef HuntingMonster_hpp
#define HuntingMonster_hpp

#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "SOCommon.h"
// 一个战斗英雄
// 地面单位都有一个 X 坐标


class HuntingMonster:HuntingMonsterProtocal
{
public:

    static std::shared_ptr<HuntingMonster> create(){
        return std::make_shared<HuntingMonster>();
    }

    enum BoneIndexType
    {
        BT_STEADY,
        BT_HEAD,
        BT_HAIR,
        BT_BODY,
        BT_HAND_L,
//        BT_HAND_R,
        BT_LEG_L,
        BT_LEG_R,
        BT_SWORD_MAX,
        BT_SHIELD,
        BT_MAX,
    };
    static int boneIndexType2sopxId(int boneIndexType);
    static cocos2d::Vec3 boneIndex2relativePosition(int boneIndexType);
    ACPositionScaleRotation help_calcBonePosition(int boneIndexType);
    ACPositionScaleRotation help_boneDeadGesture(int boneIndex);

    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);




    virtual void op_configType(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level);
    virtual void op_toastAttack(); //攻击
    virtual void op_toastUnderAttack(); //播放被攻击动画，变白
    virtual void op_toastDead(cocos2d::Vec2 direction); //播放死亡，散架了，坠落到地上，过一会儿后消失

protected:
    HuntingMonsterGeneralType _generalType;
    HuntingMonsterSpecialType _specialType;
    bool _hasShield;
    int _level;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();

    DynamicPixelNode* _dpxNode; //本体
    void initMonsterThings();

    //动作配置 以下动作，一旦执行，就会一直做下去，直到设置成其它动作。
    void ani_moving(float radio); //正常的往前移动的动作，radio 来控制减速
    void ani_attacking();
    void ani_dizzy(); //晕了，会晃头，有一圈星星

};


#endif /* HuntingMonster_hpp */
