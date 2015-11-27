//
//  HeroHeadAndUpgrade.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/26.
//
//

#ifndef HeroHeadAndUpgrade_hpp
#define HeroHeadAndUpgrade_hpp


#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"
#include "HuntingHero.hpp"


class HeroHeadAndUpgrade:public HeroHeadAndUpgradeProtocal
{

public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    virtual void configProtocals(HuntingHerosManageProtocal* huntingHerosManageProtocal) { _huntingHerosManageProtocal = huntingHerosManageProtocal;}
    virtual void op_configPosition(HeroPositionType position, bool direct); //设置位置
    virtual HeroPositionType op_fetchPosition(); //获取位置
    virtual void op_tellGoldChange(); //被通知金币改变

    HuntingHero* op_fetchHero() { return &_huntingHero; }

    void update(float dt);

protected:
    HuntingHerosManageProtocal* _huntingHerosManageProtocal;
    HeroPositionType _heroPositionType;
    HeroHeadState _heroHeadState = HeroHeadState::EMPTY;
    HeroType _heroType = HeroType::HT_META;
    int _heroLevel = 0; // 1-2-3 基本升级 后转职 到4 ,5-6为后期高级升级

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;

    void initHubThings();

    PixelNode* _pxHeadIcon;
    PixelNode* _pxBuyConfirm;
    void initHeadThings();
    void initTouchThings();

    HuntingHero _huntingHero;
    void initHeroThings();
};
#endif /* HeroHeadAndUpgrade_hpp */
