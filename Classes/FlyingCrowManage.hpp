//
//  FlyingCrowManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/7.
//
//

#ifndef FlyingCrowManage_hpp
#define FlyingCrowManage_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FloatingLaser.hpp"

class FlyingCrowManage: public FlyingCrowManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

}
#endif /* FlyingCrowManage_hpp */
