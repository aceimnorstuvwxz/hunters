// (C) 2015 Arisecbf
#ifndef QuestScene_hpp
#define QuestScene_hpp

#include <stdio.h>

#include "TRBaseScene.h"
#include "TripleDef.h"
#include "format.h"
#include "QuestProtocals.hpp"
#include "SOCommon.h"
#include "BattleRoad.hpp"
#include "QuestCameraManage.hpp"
#include "HeroGraphicNode.hpp"
#include "BattleRoles.hpp"
#include "BloodBar.hpp"
#include "SingleBeat.hpp"
#include "BeatField.hpp"
#include "TopIcons.hpp"
#include "EnergyBar.hpp"
#include "WindBar.hpp"
#include "PowerBar.hpp"
#include "HuntingHero.hpp"
// Beat Quest Game Scene


class QuestScene:public TRBaseScene, QUestSceneProtocal
{
public:
    CREATE_FUNC(QuestScene);
    virtual bool init() override;

    virtual void op_showPauseShadow() override;

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;


    QuestCameraManage _cameraManage;


    BattleRoad _battleRoad;


    TopIcons _topIcons;
    EnergyBar _energyBar;


    BattleRoles _battleRoles;

    BloodBar _bloodBar;

    BeatField _beatField;
    WindBar _windBar;
    PowerBar _powerBar;
    HuntingHero _huntingHero[4];

    cocos2d::Sprite* _pauseShadow;
    void initPauseShadowThings();
    bool _waiting2startBattle = false;

    void battleBegin();




    void update(float dt) override;
};

#endif /* QuestScene_hpp */
