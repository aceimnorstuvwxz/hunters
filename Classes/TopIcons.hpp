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
    virtual void op_configGold(int number) override; //设置金币数量

    void update(float dt);
protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;


    cocos2d::Node* _hubNode;
    cocos2d::Node* _heartHubNode;
    void initHubThings();

    PixelNode* _pxGoldBg;
    PixelNode* _pxGoldIcon;
    PixelTextNode* _pxGoldNumber;
    PixelTextNode* _ptxHeartNumber;
    int _currentGoldNumber = 0;
    int _targetGoldNumber = 0;
    void initGoldThings();


    PixelNode* _pxPauseIcon;
    void initPauseThings();
};

#endif /* TopIcons_hpp */
