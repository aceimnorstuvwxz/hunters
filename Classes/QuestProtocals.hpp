// (C) 2015 arisecbf

#ifndef QuestProtocals_hpp
#define QuestProtocals_hpp

#include <stdio.h>
#include "HeroGraphicUnitAnimationDef.hpp"
#include "PixelNode.h"
#include "ACSoundManage.hpp"
#include "format.h"
#include "QuestDef.hpp"

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

    virtual void op_configWind(float windDirection) = 0; //设置风的方向，表现为树的移动、云的飘动，正-向右，负-向左，大小表示风的强度
    virtual void op_hitCastle(bool dead) = 0;
    virtual void op_configCastle(int grade) = 0; // 0 没有损伤 5完全毁灭


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
    virtual void op_minusHeart() = 0;
    virtual void op_addWave() = 0;
};

enum class EnergySkillType:int
{
    FLOAT_LASER //空中的激光器支援
//    SHIELD, //护盾
//    CRITICAL, //暴击加成
//    TIMESTOP, //时光暂停
};

class EnergyBarProtocal
{
public:
    virtual void op_addEnergy(float howmuch) = 0; //通知怪物死亡，加气
};

class WindBarProtocal
{
public:
    virtual void op_configWind(float wind) = 0; //设置风力数值，正表示往右（顺），负的表示往左（逆风）0无风

    virtual float op_fetchWind() = 0;
};

class PowerBarProtocal
{
public:
    virtual void op_configEnable(bool enable) = 0; //放箭触摸瞄准的开关
    virtual void op_show() = 0;
    virtual void op_dismiss() = 0;
    virtual void op_configPower(float power, int angle) = 0; //power [0-1]
};



enum class HeroPositionType:int
{
        HPT_0 = 0, //最前排
        HPT_1 = 1,
        HPT_2 = 2,
        HPT_3 = 3, //最后
        HPT_OUT = 4 //视野外
};

float heroPositionType2floatYposition(HeroPositionType positionType);


class HuntingHerosManageProtocal
{
public:
    virtual void op_aimingStart() = 0; //power bar 通知开始瞄准
    virtual void op_aimingCancel() = 0; //power bar 通知瞄准取消
    virtual void op_configAiming(float angle, float strenth) = 0; //设置瞄准的参数
    virtual void op_toastBow(float angle, float strenth) = 0; //以这个角度开始放箭

};

enum class HeroHeadState:int
{
    EMPTY = 0, //空的，可购买英雄到此单位
    ALIVE = 1, //满的，已经有英雄到此单位了
};

enum class HeroType:int
{
    HT_META = 0, //基本
    HT_SLOW_DOWN = 1, //减速
    HT_HIGH_ATTACK = 2, //高功
    HT_MULTI_ATTACK = 3, //群攻
    HT_BOMB_ATTACK = 4, //炸弹
    HT_NONE,
};

inline std::string heroType2string(HeroType t )
{
    switch (t) {

        case HeroType::HT_META:
            return "META";
        case HeroType::HT_SLOW_DOWN:
            return "SLOW";
        case HeroType::HT_HIGH_ATTACK:
            return "HIGH";
        case HeroType::HT_MULTI_ATTACK:
            return "MULTI";
        case HeroType::HT_BOMB_ATTACK:
            return "BOMB";

        default:
            assert(false);
            break;
    }
    return "";
}

    //是否可以转职
inline bool heroCouldTransfer(HeroType heroType, int grade) {
    return heroType == HeroType::HT_META && grade == 2;
}
    //是否可以升级
inline bool heroCouldUpgrade(HeroType heroType, int grade) {
    return  !heroCouldTransfer(heroType, grade) && grade < 2;
}
    //升级的金钱
inline int heroUpgradeGold(HeroType heroType, int grade) {
    return (grade+1)*100 + (heroType == HeroType::HT_META ? 0 : 200);
}

inline std::string heroTypeGrade2levelString(HeroType t, int g)
{
    int r = 0;
    switch (t) {
        case HeroType::HT_META:
            r = 0;
            break;


        default:
            r = 3;
            break;
    }
    r = r+g+1;
    if (r == 6) {
        return "max";
    } else {
        return fmt::sprintf("lv%d", r);
    }
}

    //转职的金钱
inline int heroTransferGold() {
    return 300;
}





class HuntingHeroProtocal
{
public:
    virtual void op_configHeroTypeAndGrade(HeroType heroType, int grade) = 0; // 由英雄类型和等级 来指定穿着套装和弓
    virtual void op_configRelativeAngle(float angle) = 0; //逆时针变大，设置单个英雄的相对设计角度
    virtual void op_startAiming() = 0; //开始瞄准，会举起弓到水平位置
    virtual void op_stopAiming() = 0; //取消瞄准，会放下弓
    virtual void op_toastShoot(float angle, float strenth) = 0; //瞄准完毕后的发射动作，发射后会放下弓
    virtual void op_configAiming(float angle, float strenth) = 0;
    virtual void op_toastUnderAttack() = 0; //播放被攻击动画
    virtual void op_toastDead() = 0; //播放死亡
    virtual void op_toastShield(float interval) = 0; //播放一段时间护盾
    virtual void op_toastBoost(float interval) = 0; //播放一段时间的暴击增益
    virtual void op_toastUpgrade() = 0; //播放升级动画
    virtual void op_show() = 0;
    virtual void op_hide() = 0;
    virtual void op_move(HeroPositionType desPositionType, bool direct = false) = 0;
 };

enum class HuntingMonsterGeneralType:int
    //在图形上有大小区分，普通，大，极大，同时在攻击力、移动速度、血量上有不同配置
{
    NORMAL=1,
    BIG=4,
    GIANT=8,
};

inline float calcBloodMax(HuntingMonsterGeneralType generalType, int level)
{
    return 0.5f*100*(1.f+level*1.f/4.f)*static_cast<int>(generalType);
}

inline int calcMonsterGold(HuntingMonsterGeneralType generalType, int level)
{
    int r =  (generalType == HuntingMonsterGeneralType::NORMAL ? 1 : generalType == HuntingMonsterGeneralType::BIG ? 2 : 3);
    return r * (5 + level/4);
}

inline float calcMonsterEnergy(HuntingMonsterGeneralType generalType)
{
    float r =  (generalType == HuntingMonsterGeneralType::NORMAL ? 1 : generalType == HuntingMonsterGeneralType::BIG ? 2 : 3);
    return 0.33f/20*r; //要20个普通的才能换1格能量
}

inline float huntingMonsterGeneralType2scale(HuntingMonsterGeneralType generalType){
    return  generalType ==  HuntingMonsterGeneralType::NORMAL ? 1.f :
        generalType == HuntingMonsterGeneralType::BIG ? 2.f:
    3.f;
}

enum class HuntingMonsterSpecialType
    //特效类别，特效表示这个怪有特技，同时图形上有粒子效果
{
    METAL, //速度慢，血量高，有紫色粒子效果
    WOOD, //
    WATER, //能够自愈
    FIRE, //攻击力加强
    EARTH, //移动速度快
    NONE,
};
struct ArrowUnit;
class HuntingMonsterProtocal
{
public:
    virtual void op_configType(HuntingMonsterGeneralType generalType, HuntingMonsterSpecialType specialType, bool hasShield, int level, int shieldCount) = 0; //level 决定了其基础的等级，决定了套装样子和基础血量和攻击力
    virtual void op_toastAttack() = 0; //攻击
    virtual void op_toastUnderAttack() = 0; //播放被攻击动画，变白
    virtual void op_toastDead(cocos2d::Vec2 direction) = 0; //播放死亡，散架了，坠落到地上。从哪个方向的箭杀死的，头会按那个方向飞走，然后其它的散开落地
    virtual int op_getId() = 0;
    virtual void op_dealWithArrow(ArrowUnit& arrow) = 0;
    virtual void op_thunderTest(float pos) = 0;
    virtual void op_bombTest(float pos, int grade) = 0;
};


class HeroHeadAndUpgradeProtocal
{
public:
    virtual void op_configPosition(HeroPositionType position, bool direct) = 0; //设置位置
    virtual HeroPositionType op_fetchPosition() = 0; //获取位置
    virtual void op_tellGoldChange() = 0; //被通知金币改变


};

enum class HuntingArrowType:int
{
    META_0 = 00,
    META_1 = 01,
    META_2 = 02,
    SLOW_0 = 10,
    SLOW_1 = 11,
    SLOW_2 = 12,
    HIGH_0 = 20,
    HIGH_1 = 21,
    HIGH_2 = 22,
    MULTI_0 = 30,
    MULTI_1 = 31,
    MULTI_2 = 32,
    BOMB_0 = 40,
    BOMB_1 = 41,
    BOMB_2 = 42
};

    /*
inline HuntingArrowType heroTyoeGrade2arrowType(HeroType heroType, int grade)
{

}*/

inline float calcArrowDamage(HuntingArrowType arrowType)
{
    float r = 0;
    switch (arrowType) {
        case HuntingArrowType::META_0:
            r = 50;
            break;
        case HuntingArrowType::META_1:
            r = 75;
            break;
        case HuntingArrowType::META_2:
            r = 130;
            break;

        case HuntingArrowType::SLOW_0:
            r = 150;
            break;
        case HuntingArrowType::SLOW_1:
            r = 180;
            break;
        case HuntingArrowType::SLOW_2:
            r = 250;
            break;

        case HuntingArrowType::HIGH_0:
            r = 150;
            break;
        case HuntingArrowType::HIGH_1:
            r = 200;
            break;
        case HuntingArrowType::HIGH_2:
            r = 300;
            break;

        case HuntingArrowType::MULTI_0:
            r = 100;
            break;
        case HuntingArrowType::MULTI_1:
            r = 110;
            break;
        case HuntingArrowType::MULTI_2:
            r = 120;
            break;

        case HuntingArrowType::BOMB_0:
            r = 100;
            break;
        case HuntingArrowType::BOMB_1:
            r = 100;
            break;
        case HuntingArrowType::BOMB_2:
            r = 110;
            break;

        default:
            assert(false);
            break;
    }

    return r*0.5f;
}

inline int hitTimesOfArrow(HuntingArrowType type)
{
    return type == HuntingArrowType::HIGH_1 ? 2: type == HuntingArrowType::HIGH_2 ? 3 : 1;
}

HuntingArrowType huntingHeroType2huntingArrowType(HeroType heroType, int grade);

class HuntingArrowManageProtocal
{
public:
    virtual void op_shootArrow(HuntingArrowType arrowType, HeroPositionType position, float angle, float strenth) = 0; //放箭
};

struct ArrowUnit
{
    PixelNode* _pxNode; //图形本体
    cocos2d::Vec2 _speed; //速度
    HuntingArrowType _type; //弓箭类型
    int _leftHitTimes; //剩余击中次数 一般的为1 能够穿透的>1
    std::vector<int> _hitedMonsterIds; //已经击中的怪物
};

class HuntingMonsterManageProtocal
{
public:
    virtual void op_dealCollision(ArrowUnit& arrow) = 0;
    virtual void op_thunder(float pos) = 0;
    virtual void op_bomb(float pos, int grade) = 0;
};

inline cocos2d::Vec2 huntingCalcSpeed(float angle, float strenth){
    float spx = strenth * std::cos(angle/180*3.1415926);
    float spy = strenth * std::sin(angle/180*3.1415926);
    const float speed_scale = 100*1.5;
    cocos2d::Vec2 speed = speed_scale*cocos2d::Vec2{spx,spy};
    return speed;
}

inline cocos2d::Vec2 huntingCalcAcce(float wind){
    const float gravity = QuestDef::GRAVITY;
    const float windpower = 4;

    cocos2d::Vec2 acce = 1.5f*cocos2d::Vec2{windpower*wind, -gravity};
    return acce;
}

class FloatingLaserProtocal
{
public:

    virtual void op_appear(cocos2d::Vec2 pos, float time) = 0; //出现的位置 从上方降下来 time 有效时间，时间一到 自动消失
    virtual void op_toastLaser(cocos2d::Vec2 target) = 0; //释放一个激光，转动角度，释放激光，有后坐力表现

    virtual bool op_isEnable() = 0;
};


class FloatingLaserManageProtocal
{
public:
    virtual void op_launchOneLaser() = 0; //释放一个激光器
    virtual int op_fetchLaserNumber() = 0; //获取天上有几个激光器
    virtual void op_toastLaser(cocos2d::Vec2 target) = 0; //放激光
};


//enum class FlyingCrowType{
//        CT_NORMAL, //普通
//        CT_ACCE, //在中间会突然加速
//        CT_SHIELD, //身上背个盾，需要打两次
//};

class FlyingCrowProtocal
{
public:
    virtual void op_config(bool acce, bool shield, cocos2d::Vec2 relativePosition) = 0;
    virtual bool op_dealWithArrow(ArrowUnit& arrow) = 0;
};

    class FlyingCrowManageProtocal
    {
    public:
        virtual void op_dealCollision(ArrowUnit& arrow) = 0;

    };

    class GlobalArrowEffectManageProtocal
    {
    public:
        virtual void op_bomb(int grade, cocos2d::Vec2 pos) = 0;
        virtual void op_thunder(cocos2d::Vec2 pos) = 0;
    };

#endif /* QuestProtocals_hpp */
