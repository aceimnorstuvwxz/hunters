//
//  BattleState.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/8.
//
//

#include "BattleState.hpp"
#include "ProgressInfo.hpp"
#include "QuestDef.hpp"

USING_NS_CC;
BattleState BattleState::_instance;

void BattleState::configProtocals(BloodBarProtocal* bloodBarProtocal, BeatFieldProtocal* beatFieldProtocal, BattleRolesProtocal* battleRolesProtocal, BattleRoadProtocal* battleRoadProtocal, QuestCameraManageProtocal* cameraManageProtocal, cocos2d::Camera* mainCamera, QUestSceneProtocal* questSceneProtocal)
{
    _bloodBarProtocal = bloodBarProtocal;
    _beatFieldProtocal = beatFieldProtocal;
    _battleRolesProtocal = battleRolesProtocal;
    _battleRoadProtocal = battleRoadProtocal;
    _cameraManageProtocal = cameraManageProtocal;
    _mainCamera = mainCamera;
    _questSceneProtocal = questSceneProtocal;
}

void BattleState::initAsNewLevel() // 新关卡的初始化
{
    int levelIndex = ProgressInfo::s()->getLevelIndex();
    _levelInfo = StoryInfo::s()->getLevelConfig(levelIndex);

    _rivalIndex = 0;
    _heroBlood = ProgressInfo::s()->getInitBlood();
    _heroBloodMax = ProgressInfo::s()->getInitBloodMax();
    _heroAttackMin = ProgressInfo::s()->getInitAttackMin();
    _heroAttackMax = ProgressInfo::s()->getInitAttackMax();
    _heroPower = ProgressInfo::s()->getInitPower();
    _heroComboAmount = _heroShield = 0;

}
void BattleState::initAsNextRival() // 新战斗的初始化
{
    RivalInfoConfig config = _levelInfo[_rivalIndex];
    _rivalBlood = _rivalBloodMax = config.bloodMax;
    _rivalAttackMin = config.attackMin;
    _rivalAttackMax = config.attackMax;
    _rivalBloodLineCnt = config.bloodLineCnt;
    _rivalShield = 0;


    _battleRolesProtocal->op_rivalConfig();
    _battleRoadProtocal->op_moveForword(QuestDef::BATTLE_MOVE_IN_TIME); //道路移动
    _battleRolesProtocal->op_rivalMoveIn(); // rival 进场
    _cameraManageProtocal->op_startRunningShake();
    //进场完毕后显示血条
    _mainCamera->scheduleOnce([this](float dt){
        _beatFieldProtocal->op_show();
        BattleState::s()->rivalSetBloodBar();
        _cameraManageProtocal->op_stopRunningShake();
        _questSceneProtocal->op_showPauseShadow();

    }, QuestDef::BATTLE_MOVE_IN_TIME, "show bloodbar and taparea");
}
void BattleState::finishAsPassRival() // 过了一个怪之后
{
    _rivalIndex++;
    if (_rivalIndex == _levelInfo.size()) {
        //全部 boss 完毕
        finishAsPassLevel();
    } else {
        initAsNextRival();
    }
}
void BattleState::finishAsPassLevel() // 过了整个关卡
{
    ProgressInfo::s()->saveLevelResult(_heroBlood, _heroBloodMax, _heroAttackMin, _heroAttackMax, _heroPower);
}

// beat将会将操作发送到这里，然后由这里的逻辑，回头告诉beat自身的变更情况，同时更新状态，并且通知其它部分做出图形。

void BattleState::heroAddShield() // 新shield的抗血量由这里的数据来算出来
{
    // 我方加的护盾的量是对方attack 的平均值
    int amount = 0.5 * (_rivalAttackMin + _rivalAttackMax);

    if (_heroShield <= 0) {
        _beatFieldProtocal->op_showHeroShield();
    }

    _heroShield += amount;

    // TODO 数值toast 和 shield的数值更新
}
void BattleState::rivalAddShield()
{
    // 对方加的护盾的量是我方attack的平均值
    int amount = 0.5 * (_heroAttackMin + _heroAttackMax);

    if (_rivalShield <= 0) {
        _beatFieldProtocal->op_showRivalShield();
    }

    _rivalShield += amount;

    // TODO 数值toast 和 shield的数值更新
}

void BattleState::heroAddBlood() // hero吃到加血beat，加血量由这里的数值计算出来。
{
    int amount = 2.0 * (_rivalAttackMin + _rivalAttackMax) * 0.5; // 血瓶大概加2倍的敌人攻击

    _heroBlood = std::min(_heroBloodMax, _heroBlood + amount);

    // TODO 数值toast
    _bloodBarProtocal->op_hero_configBlood(_heroBloodMax, _heroBlood);

}
void BattleState::rivalAddBlood() // rival吃到加血beat，同上。
{
    int amount = 2.0 * (_heroAttackMin + _heroAttackMax) * 0.5; // 血瓶大概加2倍的对方攻击

    _rivalBlood = std::min(_rivalBloodMax, _rivalBlood + amount);

    // TODO 数值toast
    _bloodBarProtocal->op_rival_configBlood(_rivalBloodMax, _rivalBlood, _rivalBloodLineCnt);
}
bool BattleState::heroUnderAttack(BeatType beatType) // hero遭到攻击，这里bool告诉beat是否是盾抗的。攻击力由beatType同时根据这里的attack数据算
{
    // 先算攻击力
    int damage = calcRivalRandomDamage(beatType);

    // 盾及血的计算
    bool defence = procedureOfHeroShieldDefenceHealth(damage);

    return defence;
}
bool BattleState::rivalUnderAttack(BeatType beatType) // rival遭到攻击，同上。
{
    // 先算攻击力
    int damage = calcHeroRandomDamage(beatType);

    // 盾及血的计算
    bool defence = procedureOfRivalShieldDefenceHealth(damage);


    return defence;
}

bool BattleState::rivalUnderComboAttack(int damage)
{
    // 盾及血的计算
    bool defence = procedureOfRivalShieldDefenceHealth(damage);


    return defence;
}


// 在战斗胜利后的，能力提升阶段，
void BattleState::upgradeBlood() // 加血量
{

}
void BattleState::upgradeAttack()
{

}
void BattleState::upgradePower(){

}

int BattleState::calcRivalRandomDamage(BeatType beatType) //在min-max之间随机算基础攻击力
{
    int base =  random(_rivalAttackMin, _rivalAttackMax);
    int r  = base;
    switch (beatType) {
        case BeatType::SINGLE:
            r = base;
            break;

        case BeatType::LONG:
            r = 0.5*base;
            break;

        case BeatType::COPY:
            r = base;
            break;


        case BeatType::LASER:
            r = 2*base;
            break;


        default:
            assert(false);
            break;
    }
    return r;
}

int BattleState::calcHeroRandomDamage(BeatType beatType) //在min-max之间随机算基础攻击力
{
    int base =  random(_heroAttackMin, _heroAttackMax);
    int r  = base;
    switch (beatType) {
        case BeatType::SINGLE:
            r = base;
            break;

        case BeatType::COPY:
            r = base;
            break;

        case BeatType::LASER:
            r = 2*base;
            break;

        default:
            assert(false);
            break;
    }
    return r;
}


bool BattleState::procedureOfHeroShieldDefenceHealth(int damage) //通用的hero盾情况处理，Bool表示此攻击是否有盾抵消
{
    bool defence = false;
    if (_heroShield > 0) {
        _heroShield -= damage;
        if (_heroShield < 0) {
            _heroShield = 0;
        }
        if (_heroShield == 0) {
            // 失去盾
            _beatFieldProtocal->op_dismissHeroShield();
        } else {
            // 更新盾的抗值
            // TODO
        }
        defence = true;
    }

    if (!defence) {
        comboFail(); //受到伤害，combo 断开
        _heroBlood -= damage;
        if (_heroBlood  < 0) {
            _heroBlood = 0;
        }
        // 受到伤害
        _beatFieldProtocal->op_toastHeroDamage();

        // 血条更新
        _bloodBarProtocal->op_hero_configBlood(_heroBloodMax, _heroBlood);

        // TODO Blood diff的数字toast

        if (_heroBlood == 0) {
            // Hero死亡
            help_heroDie();
        }
    }

    return defence;
}

bool BattleState::procedureOfRivalShieldDefenceHealth(int damage) //通用的hero盾情况处理，Bool表示此攻击是否有盾抵消
{
    bool defence = false;
    if (_rivalShield > 0) {
        _rivalShield -= damage;
        if (_rivalShield < 0) {
            _rivalShield = 0;
        }
        if (_rivalShield == 0) {
            // 失去盾
            _beatFieldProtocal->op_dismissRivalShield();
        } else {
            // 更新盾的抗值
            // TODO
        }
        defence = true;
    }

    if (!defence) {
        _rivalBlood -= damage;
        if (_rivalBlood  < 0) {
            _rivalBlood = 0;
        }
        // 受到伤害
        _battleRolesProtocal->op_rivalBeAttack();

        // 血条更新
        _bloodBarProtocal->op_rival_configBlood(_rivalBloodMax, _rivalBlood, _rivalBloodLineCnt);

        // TODO Blood diff的数字toast

        if (_rivalBlood == 0) {
            // rival死亡
            help_rivalDie();
        }
    }
    
    return defence;
}

void BattleState::comboAdd() //当拦截成功时调用
{
    _heroComboAmount++;
    if (_heroComboAmount == QuestDef::COMBO_BASE_AMOUNT) {
        //刚满足的时候 show
        _beatFieldProtocal->op_showComboCore();
        _bloodBarProtocal->op_toastComboFull();
    }
    _bloodBarProtocal->op_configCombo(std::min(1.f, _heroComboAmount*1.f/QuestDef::COMBO_BASE_AMOUNT));
}

void BattleState::comboFail() //当失败是调用
{
    bool failFromComplete =  _heroComboAmount >= QuestDef::COMBO_BASE_AMOUNT;
    bool need2dismiss = _heroComboAmount >0;
    _heroComboAmount = 0;
    if (need2dismiss) {
        _beatFieldProtocal->op_dismissComboCore();
    }
    if (failFromComplete) {
        _bloodBarProtocal->op_toastComboReset();
    }
    _bloodBarProtocal->op_configCombo(std::min(1.f, _heroComboAmount*1.f/QuestDef::COMBO_BASE_AMOUNT));
}

int BattleState::calcComboDamage(int amount)
{
    return amount*_heroPower*random(0.8, 1.0);
}

void BattleState::comboTry2Toast() //尝试发射 combo
{
    if (_heroComboAmount >= QuestDef::COMBO_BASE_AMOUNT) {
        //发射 combo
        int damage = calcComboDamage(_heroComboAmount);
        _heroComboAmount = 0;
        _beatFieldProtocal->op_toastComboAttack(damage);
    }
}

void BattleState::heroSetBloodBar() //请求设置一遍这些个数据
{
    _bloodBarProtocal->op_hero_configAttack(_heroAttackMin, _heroAttackMax, _heroPower);
    _bloodBarProtocal->op_hero_configBlood(_heroBloodMax, _heroBlood);
}
void BattleState::rivalSetBloodBar()
{
    _bloodBarProtocal->op_rival_configAttack(_rivalAttackMin, _rivalAttackMax);
    _bloodBarProtocal->op_rival_configBlood(_rivalBloodMax, _rivalBlood, _rivalBloodLineCnt);
    _bloodBarProtocal->op_rival_configHead(_levelInfo[_rivalIndex].rivalId);
}


void BattleState::help_rivalDie()
{
    //通知 beatField，它会清空 Beats,然后释放 upgrade 的 beats
    _beatFieldProtocal->op_toastRivalDieEffects();
    _battleRolesProtocal->op_rivalDie();

}

void BattleState::help_heroDie()
{
    //TODO

}