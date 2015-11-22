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
    virtual void op_configWind(int wind); //设置风力数值，正表示往右（顺），负的表示往左（逆风）0无风

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;


    cocos2d::Node* _hubNode;
    void initHubThings();

    int _currentWind = 0;
    int _targetWind = 0;

    PlanePixelNode* _pxBg;
    PlanePixelNode* _pxStrenth;
    PlanePixelNode* _pxDirection;
    PixelTextNode* _ptWind;
    PixelTextNode* _ptStrenth;
    void initWindThings();

};

#endif /* WindBar_hpp */
