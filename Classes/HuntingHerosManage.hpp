//
//  HuntingHerosManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/26.
//
//

#ifndef HuntingHerosManage_hpp
#define HuntingHerosManage_hpp

#include "cocos2d.h"
#include "QuestProtocals.hpp"

//这里包括我方英雄的头像和英雄本体
class HuntingHerosManage:public HuntingHerosManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    //头像区域
    void initHeadsThings();

};

#endif /* HuntingHerosManage_hpp */
