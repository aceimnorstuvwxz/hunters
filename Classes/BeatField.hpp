//
//  BeatField.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/7.
//
//

#ifndef BeatField_hpp
#define BeatField_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "PixelNode.h"
#include "QuestProtocals.hpp"
#include "SingleBeat.hpp"

class BeatField :public BeatFieldProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    void configProtocals(BattleRolesProtocal* battleRolesProtocal, BattleRoadProtocal* battleRoadProtocal);
    virtual void op_show(); //出现
    virtual void op_dismiss(); //消失
    virtual void op_startBeating(); //开始出现beats 战斗开始
    virtual void op_toastTap(BeatFieldLineType line, bool isok); //toast是短暂的，包括出现和消失

    virtual void op_toastHeroDamage(); //被攻击
    virtual void op_toastHeroHealth(); //加血
    virtual void op_showHeroShield(); //举盾牌
    virtual void op_dismissHeroShield(); //盾牌被击毁

    virtual void op_toastRivalDamage(); //被攻击
    virtual void op_toastRivalHealth(); //加血
    virtual void op_showRivalShield(); //举盾牌
    virtual void op_dismissRivalShield(); //盾牌被击毁

    virtual void op_toastLaser(BeatFieldLineType line); //Laser beat 的效果，会有光束，同时 line 上的所有 beat 消亡


    virtual void op_showComboCore(); //显示可以发射 combo 的状态
    virtual void op_dismissComboCore(); //取消可以发射 combo 的状态
    virtual void op_toastComboAttack(int damage); //进行一次 combo 攻击的显示，同时 推后场上的 beats
    virtual void op_toastRivalDieEffects(); //rival 死了后，所有 Beat 减速运动，后消失，然后释放升级 beat

    virtual void op_showUpgrade();
    virtual void op_dismissUpgrade();


    void update(float dt);
protected:
    bool _isDieingSlow = false;
    bool _beatingPaused = true;
    BattleRolesProtocal* _battleRolesProtocal;
    BattleRoadProtocal* _battleRoadProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    PixelNode* _pxTapArea;
    PixelNode* _pxTapOk[5];
    PixelNode* _pxTapFail[5];
    PixelNode* _pxLaser[5];


    const cocos2d::Vec4 _mixColorDamage = {1,0,0,1};
    const cocos2d::Vec4 _mixColorHealth = {0,1,0,1};
    PixelNode* _pxHeroDamageHealth;
    PixelNode* _pxRivalDamageHealth;
    PixelNode* _pxHeroShield;
    PixelNode* _pxRivalShield;


    void initEffectsThings();


    void initTouchThings();


    PixelNode* _pxComboCore;
    PixelNode* _pxComboWing;
    void initComboThings();


    void tellAllBeatTapIn(BeatFieldLineType line);
    void tellAllBeatTapOut(BeatFieldLineType line);
    void addSingleBeat();
    void addShieldBeat();
    void addHealthBeat();
    void addLongBeat();
    void addCopyBeat();
    void addLaserBeat();

    void putUpgradeBeats();

    std::vector<BeatFieldLineType> _lines2beclear;

    // 所有beat在一起的指针序列
    std::list<std::shared_ptr<BeatProtocal>> _allBeats;

    float _time = 0;
    float _interval = 0.5;

    PixelNode* _pxUpgradeTitle;
    PixelNode* _pxUpgradeContent;
    bool _waitUpgrading = false;
    void initUpgradeThings();
};

#endif /* BeatField_hpp */
