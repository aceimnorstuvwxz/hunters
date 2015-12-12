//
//  WindBar.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/22.
//
//

#ifndef WindBar_hpp
#define WindBar_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"

//风力指示牌
//风力图形，风向图形，风力数值
class WindBar:public WindBarProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(BattleRoadProtocal* battleRoadProtocal) {
        _battleRoadProtocal = battleRoadProtocal;
    }
    virtual void op_configWind(float wind); //设置风力数值，正表示往右（顺），负的表示往左（逆风）0无风
    virtual float op_fetchWind() {return _targetWind;}
    void update(float dt);

protected:
    BattleRoadProtocal* _battleRoadProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;


    cocos2d::Node* _hubNode;
    void initHubThings();

    float _currentWind = 0;
    float _targetWind = 0;

    PlanePixelNode* _pxBg;
    PlanePixelNode* _pxStrenth;
    PlanePixelNode* _pxDirection;
    PixelTextNode* _ptWind;
    PixelTextNode* _ptStrenth;
    void initWindThings();

};

#endif /* WindBar_hpp */
