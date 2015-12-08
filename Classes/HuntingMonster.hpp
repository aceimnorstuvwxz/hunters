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
        BT_BLOOD,
        BT_MAX,
    };
    static int boneIndexType2sopxId(int boneIndexType);
    static cocos2d::Vec3 boneIndex2relativePosition(int boneIndexType);
    ACPositionScaleRotation help_calcBonePosition(int boneIndexType);
    ACPositionScaleRotation help_boneDeadGesture(int boneIndex);

    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocal(EnergyBarProtocal* energyBarProtocal, FloatingLaserManageProtocal *floatingLaserManageProtocal, GlobalArrowEffectManageProtocal* effetcManageProtocal, TopIconsProtocal* topIconsProtocal) {
        _topIconsProtocal = topIconsProtocal;
        _energyBarProtocal = energyBarProtocal;
        _floatingLaserManageProtocal = floatingLaserManageProtocal;
        _effetcManageProtocal = effetcManageProtocal;
    }



    virtual void op_configType(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level, int shieldCount);
    virtual void op_toastAttack(); //攻击
    virtual void op_toastUnderAttack(); //播放被攻击动画，变白
    virtual void op_toastDead(cocos2d::Vec2 direction); //播放死亡，散架了，坠落到地上，过一会儿后消失
    virtual int op_getId();
    virtual void op_dealWithArrow(ArrowUnit& arrow);

    virtual void op_thunderTest(float pos) ;
    virtual void op_bombTest(float pos, int grade);
    void update(float dt);
     bool isDead(){return !_alive;};

protected:
    TopIconsProtocal* _topIconsProtocal;

    GlobalArrowEffectManageProtocal* _effetcManageProtocal;
    EnergyBarProtocal* _energyBarProtocal;
    FloatingLaserManageProtocal* _floatingLaserManageProtocal;
    HuntingMonsterGeneralType _generalType;
    HuntingMonsterSpecialType _specialType;
    bool _hasShield;
    int _shieldCount;
    int _level;
    int _id;
    static int _idGen;
    float _steadyScale;
    float _slowDownTime = 0;
    float _slowDownRate = 1;
    float _poisonTime = 0;
    float _poisonDamage;
    bool _alive = true;

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

    void applyEffectArrow(ArrowUnit& arrow, bool isThrough);

    float _bloodMax;
    float _bloowNow;
    void refreshBloodLine();

    PixelNode* _pxGold;
    void initGoldThings();
    void toastGold();
    void closeShield();

    void damage(float damage, cocos2d::Vec2 dir);

};


#endif /* HuntingMonster_hpp */
