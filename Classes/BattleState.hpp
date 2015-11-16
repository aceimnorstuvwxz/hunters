//
//  BattleState.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/8.
//
//

#ifndef BattleState_hpp
#define BattleState_hpp

#include <stdio.h>
#include "StoryInfo.hpp"
#include "QuestProtocals.hpp"

class BattleState
{
public:
    static BattleState* s() {return &_instance;};

    void configProtocals(BloodBarProtocal* bloodBarProtocal, BeatFieldProtocal* beatFieldProtocal, BattleRolesProtocal* battleRolesProtocal, BattleRoadProtocal* battleRoadProtocal, QuestCameraManageProtocal* cameraManageProtocal, cocos2d::Camera* mainCamera, QUestSceneProtocal* questSceneProtocal);

    void initAsNewLevel(); // 新关卡的初始化
    void initAsNextRival(); // 新战斗的初始化
    void finishAsPassRival(); // 过了一个怪之后
    void finishAsPassLevel(); // 过了整个关卡

    // beat将会将操作发送到这里，然后由这里的逻辑，回头告诉beat自身的变更情况，同时更新状态，并且通知其它部分做出图形。

    void heroAddShield(); // 新shield的抗血量由这里的数据来算出来
    void rivalAddShield();

    void heroAddBlood(); // hero吃到加血beat，加血量由这里的数值计算出来。
    void rivalAddBlood(); // rival吃到加血beat，同上。

    bool heroUnderAttack(BeatType type); // hero遭到攻击，这里bool告诉beat是否是盾抗的。攻击力由beatType同时根据这里的attack数据算
    bool rivalUnderAttack(BeatType type); // rival遭到攻击，同上。
    bool rivalUnderComboAttack(int damage);


    // 在战斗胜利后的，能力提升阶段，
    void upgradeBlood(); // 加血量
    void upgradeAttack(); // 加attackMin
    void upgradePower();

    void comboAdd(); //当拦截成功时调用
    void comboFail(); //当失败是调用
    void comboTry2Toast(); //尝试发射 combo

    void heroSetBloodBar(); //请求设置一遍这些个数据
    void rivalSetBloodBar();

protected:
    static BattleState _instance;

    BloodBarProtocal* _bloodBarProtocal;
    BeatFieldProtocal* _beatFieldProtocal;
    BattleRolesProtocal* _battleRolesProtocal;
    BattleRoadProtocal* _battleRoadProtocal;
    QuestCameraManageProtocal* _cameraManageProtocal;
    cocos2d::Camera* _mainCamera; //用来 schedule
    QUestSceneProtocal* _questSceneProtocal;

    std::vector<RivalInfoConfig> _levelInfo;
    int _rivalIndex; //这个关卡进行到了第几个rival

    // Hero状态
    int _heroBlood;
    int _heroBloodMax;
    int _heroAttackMin;
    int _heroAttackMax;
    int _heroPower;

    int _heroComboAmount;
    int _heroShield; //表示盾的抗血量，为0时没有盾

    // Rival状态
    int _rivalBlood;
    int _rivalBloodMax;
    int _rivalAttackMin;
    int _rivalAttackMax;
    int _rivalBloodLineCnt;
    
    int _rivalShield;

    int calcRivalRandomDamage(BeatType beatType); //在min-max之间随机算基础攻击力，不同类型会伤害差异
    bool procedureOfHeroShieldDefenceHealth(int damage); //通用的hero盾情况处理，Bool表示此攻击是否有盾抵消，以及相关的血量计算

    int calcHeroRandomDamage(BeatType beatType); //在min-max之间随机算基础攻击力，不同类型会伤害差异
    bool procedureOfRivalShieldDefenceHealth(int damage); //通用的rival盾情况处理，Bool表示此攻击是否有盾抵消，以及相关的血量计算
    int calcComboDamage(int amount);

    void help_rivalDie();
    void help_heroDie();

};
#endif /* BattleState_hpp */
