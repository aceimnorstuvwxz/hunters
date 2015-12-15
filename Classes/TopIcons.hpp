//
//  TopIcons.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/20.
//
//

#ifndef TopIcons_hpp
#define TopIcons_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"

//界面上方的图标按钮    金币，暂停


class TopIcons:public TopIconsProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(BattleRoadProtocal* battleRoadProtocal, PauseGameOverProtocal* pauseGameOverProtocal) {
        _battleRoadProtocal = battleRoadProtocal;
        _pauseGameOverProtocal = pauseGameOverProtocal;
    }

    virtual void op_configGold(int number); //设置金币数量
    virtual void op_minusHeart();
    virtual void op_addHeart();
    virtual void op_addWave();

    void update(float dt);
protected:
    int _cnt = 0;
    PauseGameOverProtocal* _pauseGameOverProtocal;
    BattleRoadProtocal* _battleRoadProtocal;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;


    cocos2d::Node* _hubNode;
    cocos2d::Node* _heartHubNode;
    void initHubThings();

    PixelNode* _pxGoldBg;
    PixelNode* _pxGoldIcon;
    PixelNode* _pxHeart;
    PixelTextNode* _pxGoldNumber;
    PixelTextNode* _ptxHeartNumber;
    PixelTextNode* _ptxWaveTitle;
    PixelTextNode* _ptxWaveNumber;
    int _currentWave = 0;
    int _currentGoldNumber = 0;
    int _targetGoldNumber = 0;
    void initGoldThings();

    int _heart = QuestDef::INIT_HEART;

    PixelNode* _pxPauseIcon;
    void initPauseThings();
};

#endif /* TopIcons_hpp */
