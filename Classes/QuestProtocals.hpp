// (C) 2015 arisecbf

#ifndef QuestProtocals_hpp
#define QuestProtocals_hpp

#include <stdio.h>
#include "HeroGraphicUnitAnimationDef.hpp"


class BattleRoadProtocal
{
public:
    //支持多个场景，比如普通的草地路、雪地路、夜间万圣节主题路、圣诞节主题路
    virtual void op_config(int which) = 0; //用来设置哪个场景
    virtual void op_vibrate() = 0; //振动
    virtual void op_moveForword(float moveTime) = 0; //场景随hero前进，前进的时间和距离公共定义
    virtual void op_setRightTreesVisible(bool enable) = 0; //右侧树在镜头旋转的一定角度需要显示/隐藏
    virtual void op_setLeftHillsVisible(bool enable) = 0; //左侧山，减少 vertex 量
    virtual void op_applyDarkShadow(float howdark) = 0; //场景变暗，用来处理 rival 的死亡，及 combo 大招
    virtual void op_dismissDarkShadow() = 0; //取消场景变暗
    virtual void op_toastDarkShadow(float howdark, float time) = 0; //toast 形式的接口
};

enum class QuestCameraPosition
{
    BATTLE,
    FORWARD
};

class QuestCameraManageProtocal
{
public:
    // 镜头的集中管理
    virtual void op_switchToPosition(QuestCameraPosition p, bool direct = false) = 0; //切换镜头的不同位置, direct表示是否直接设置而非动画
    virtual void op_startRunningShake() = 0;
    virtual void op_stopRunningShake() = 0;


    //    virtual void op_virbrateShort() = 0; //短促的振动
//    virtual void op_virbrateLong() = 0; //较长的振动
};

class HeroGraphicProtocal
{
public:
    // 默认状态下始终在执行idle动画，即各个部件的微动，由于战斗时不可见，所以不会有其它动作。
    virtual void op_config(std::vector<std::pair<HeroGraphicType, int>> bodyPartsConfig) = 0; //配置，必须被配置，如果init后，没被配置就上场，会错误。必须对所有类型进行配置
    virtual void op_setVisibility(bool enable) = 0; //设置显示与否 当退到幕后时需要
    virtual void op_run(float time) = 0; //播放一段时间的跑动动画
    virtual void op_move(const cocos2d::Vec3& pos,float time, bool direct = false) = 0; //位置移动
    virtual void op_rotate(const cocos2d::Vec3& rotation, float time, bool direct = false) = 0; //旋转

};

class RivalGraphicProtocal
{
public:
    // 有2个可以设置的状态，分别是idle和defense，同时还可以播放一个attack动作
    virtual void op_config(int whichRival) = 0; //配置，必须被配置，如果init后，没被配置就上场，会错误。必须对所有类型进行配置 设置哪一个套装
    virtual void op_setVisibility(bool enable) = 0; //设置显示与否 当退到幕后时需要
    virtual void op_attack() = 0; //播放一次attack动画
    virtual void op_startDefense() = 0; //进入defense状态并维持多久，在维持期间调用可以延长维持时间
    virtual void op_stopDefense() = 0;
    virtual void op_move(const cocos2d::Vec3& pos,float time, bool direct = false) = 0; //位置移动
    virtual void op_rotate(const cocos2d::Vec3& rotation, float time, bool direct = false) = 0; //旋转

    virtual void op_damage() = 0; //被攻击
    virtual void op_health() = 0; //被治疗
    virtual void op_die() = 0; //死亡
};

class BattleRolesProtocal
{
public:
    //每个单次战斗前要设置好hero和rival的图形属性（穿哪套衣服，打哪个怪）
    virtual void op_heroConfig() = 0; //hero 穿好衣服
    virtual void op_rivalConfig() = 0; //rival 穿好衣服
//    virtual void op_battleIn() = 0; //新战斗入场，初次入场需要hero跑入。这一判断从其它设置获取来判断入场形式。
    virtual void op_heroRunIn() = 0; //新关卡一开始，hero 从左边屏幕外跑入背景不动，跑到一定位置，停下，继续跑
    virtual void op_rivalMoveIn() = 0; //rival 进场
    virtual void op_switchToBattle() = 0; //告知镜头转向battle，此时rival需要转过身来。hero则要不再显示。
    virtual void op_switchToForward() = 0; //
    virtual void op_rivalDie() = 0; //播放rival的死亡动画
    virtual void op_rivalBeAttack() = 0; //播放rival被攻击的动作
    virtual void op_rivalAttack() = 0; //播放rival发动攻击的动作
    virtual void op_rivalBeHealth() = 0;
    virtual void op_rivalStartDefense() = 0; //rival举盾
    virtual void op_rivalStopDefense() = 0; //rival 取消举盾
    virtual void op_heroVisible(bool enable) = 0;

};

class BloodBarProtocal
{
public:
    // 包括敌对双方的战斗信息，血条，攻击力min-max，Hero独有的pwr(？连击攻击力)，血条及血量，攻击力等数值变化的表现
    // 战时的数值信息：加血，少血，加战斗力，加连击力及其图形表现不在此。

    virtual void op_hero_configBlood(int bloodMax, int blood, bool direct = false) = 0;
    virtual void op_hero_configAttack(int attack_min, int attack_max, int pwr, bool direct = false) = 0;

    virtual void op_rival_configHead(int which) = 0; //配置敌人头像
    virtual void op_rival_configBlood(int bloodMax, int blood, int cnt = 1, bool direct = false) = 0; //cnt 1-5，血条数目
    virtual void op_rival_configAttack(int attack_min, int attack_max, bool direct = false) = 0;
    virtual void op_rival_reset() = 0;//恢复 rival 的信息到?状态
    virtual void op_show() = 0;
    virtual void op_dismiss() = 0;

    virtual void op_configCombo(float radio) = 0; //设置 combo 进度
    virtual void op_toastComboFull() = 0; //满了后闪亮一下，出来一个 combo 攻击
    virtual void op_toastComboReset() = 0; //断连后红一下，表示事务了
};

enum class BeatFieldLineType:int
{
    //从左到右5条线
    L2 = -2,
    L1 = -1,
    MM = 0,
    R1 = 1,
    R2 = 2
};

struct BeatTouchResult
{
    bool tapOk;
    bool willDelete;
    bool showTapOk;
    bool showTapFail;
};

enum class BeatType:int
{
    SINGLE,
    SHIELD,
    HEALTH,
    LONG,
    COPY,
    LASER,
    BOMB,
    COMBO,
    STEALTH
};

class BeatProtocal
{
public:
    // beat 的通用接口。输入事件，自行判断，然后处理。同时update驱动运动及相关逻辑，其中各种结果调用其它模块完成。
    virtual BeatTouchResult op_touchBegin(const BeatFieldLineType& line) = 0;// bool通知上游删除
    virtual BeatTouchResult op_touchEnd(const BeatFieldLineType& line) = 0;// bool通知上游删除
    virtual bool op_update(float dt) = 0;// bool通知上游删除
    BeatType getBeatType(){ return _beatType; }
    BeatFieldLineType getLineType(){ return _lineType;}
    virtual void clear() = 0; //外部要求它从场景中删除

protected:
    BeatFieldLineType _lineType;
    BeatType _beatType;
};

class BeatFieldProtocal
{
public:
//    virtual void op_config // 本战斗beats序列的配置
    virtual void op_show() = 0; //出现
    virtual void op_dismiss() = 0; //消失
    virtual void op_startBeating() = 0; //开始出现beats 战斗开始
    virtual void op_toastTap(BeatFieldLineType line, bool isok) = 0; //toast是短暂的，包括出现和消失

    virtual void op_toastHeroDamage() = 0; //被攻击
    virtual void op_toastHeroHealth() = 0; //加血
    virtual void op_showHeroShield() = 0; //举盾牌
    virtual void op_dismissHeroShield() = 0; //盾牌被击毁

    virtual void op_toastRivalDamage() = 0; //被攻击
    virtual void op_toastRivalHealth() = 0; //加血
    virtual void op_showRivalShield() = 0; //举盾牌
    virtual void op_dismissRivalShield() = 0; //盾牌被击毁

    virtual void op_toastLaser(BeatFieldLineType line) = 0; //Laser beat 的效果
    virtual void op_showComboCore() = 0; //显示可以发射 combo 的状态
    virtual void op_dismissComboCore() = 0; //取消可以发射 combo 的状态
    virtual void op_toastComboAttack(int damage) = 0; //进行一次 combo 攻击的显示，同时 推后场上的 beats
    virtual void op_toastRivalDieEffects() = 0; //rival 死了后，所有 Beat 减速运动，后消失，然后释放升级 beat

    virtual void op_showUpgrade() = 0;
    virtual void op_dismissUpgrade() = 0;

};



class QUestSceneProtocal
{
public:
    virtual void op_showPauseShadow() = 0;
};


class TopIconsProtocal
{
public:
    virtual void op_configGold(int number) = 0; //设置金币数量
};


#endif /* QuestProtocals_hpp */
