//
//  HuntingArrowManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/1.
//
//

#ifndef HuntingArrowManage_hpp
#define HuntingArrowManage_hpp

#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"
#include "HeroHeadAndUpgrade.hpp"


struct ArrowUnit
{
    PixelNode* _pxNode; //图形本体
    //BOX2D的本体？？？
};

class HuntingArrowManage:public HuntingArrowManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    virtual void op_shootArrow(HuntingArrowType arrowType, HeroPositionType position, float angle, float strenth) ; //放箭

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    
};
#endif /* HuntingArrowManage_hpp */
