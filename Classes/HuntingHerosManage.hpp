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
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"
#include "HeroHeadAndUpgrade.hpp"

//这里包括我方英雄的头像和英雄本体
class HuntingHerosManage:public HuntingHerosManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(HuntingArrowManageProtocal* huntingArrowManageProtocal) {

        for (HeroHeadAndUpgrade& head :_headIcons) {
            head.op_fetchHero()->configProtocals(huntingArrowManageProtocal);
        }
    }



    virtual void op_aimingStart(); //power bar 通知开始瞄准
    virtual void op_aimingCancel(); //power bar 通知瞄准取消
    virtual void op_configAiming(float angle, float strenth); //设置瞄准的参数
    virtual void op_toastBow(float angle, float strenth); //以这个角度开始放箭

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    // 头像部分
    HeroHeadAndUpgrade _headIcons[4];
    std::unordered_map<int, HeroHeadAndUpgrade*> _positionMap;
    void initHeadsThings();

};

#endif /* HuntingHerosManage_hpp */
