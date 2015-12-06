//
//  FlyingCrow.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/6.
//
//

#ifndef FlyingCrow_hpp
#define FlyingCrow_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"

class FlyingCrow:public FlyingCrowProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);


    virtual void op_config(FlyingCrowType type, cocos2d::Vec2 relativePosition);
    virtual bool op_dealWithArrow(ArrowUnit& arrow);


    void update(float dt);


    void toastDead();

protected:
    
    FlyingCrowType _crowType;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();
    bool _alive = true;

    DynamicPixelNode* _dxCrow;
    PixelNode* _pxShield;
    float _speed;
    float _acce;


    void initCrowThings();

};


#endif /* FlyingCrow_hpp */
