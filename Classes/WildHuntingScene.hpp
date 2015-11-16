//
//  WildHuntingScene.hpp
//  WildingHunting
//
//  Created by chenbingfeng on 15/11/15.
//
//

#ifndef WildHuntingScene_hpp
#define WildHuntingScene_hpp

#include <stdio.h>
#include "TRBaseScene.h"
#include "PixelNode.h"

class WildHuntingTestScene: public TRBaseScene
{
public:
    CREATE_FUNC(WildHuntingTestScene);
    virtual bool init() override;

    void update(float dt)override;

protected:
    cocos2d::Layer* _mainLayer;


    cocos2d::Sprite* _spBackground;
    cocos2d::Sprite* _spHero;
    cocos2d::Sprite* _spHeroDir;
    cocos2d::Sprite* _spRival;


    cocos2d::Sprite* _spDir;
    cocos2d::Sprite* _spDirBg;
    cocos2d::Sprite* _spTriger;
    PixelNode* _pxTunnel;//弹道



    cocos2d::Vec2 _dirCenter;
    cocos2d::Vec2 _dirTarget;
    bool _diring = false;
    void initTouchThings();
    void shoot();



};
#endif /* WildHuntingScene_hpp */
