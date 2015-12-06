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


    virtual void op_configRelativePosition(cocos2d::Vec2 relativePosition);
    virtual bool op_dealWithArrow(ArrowUnit& arrow);


    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;


    cocos2d::Node* _hubNode;
    void initHubThings();

    DynamicPixelNode* _dxCrow;
    void initCrowThings();

};


#endif /* FlyingCrow_hpp */
