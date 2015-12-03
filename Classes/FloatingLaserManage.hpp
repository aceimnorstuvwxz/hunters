//
//  FloatingLaserManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/4.
//
//

#ifndef FloatingLaserManage_hpp
#define FloatingLaserManage_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FloatingLaser.hpp"

class FloatingLaserManage: public FloatingLaserManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    virtual void op_launchOneLaser(); //释放一个激光器
    virtual int op_fetchLaserNumber(); //获取天上有几个激光器
    virtual void op_toastLaser(cocos2d::Vec2 target); //放激光

    void update(float dt);

protected:

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    static constexpr int N_LASER = 4; //最多同时4个
    FloatingLaser _laserMachines[N_LASER];
    std::deque<FloatingLaser*> _laserQueue;
    void initLaserThings();


};

#endif /* FloatingLaserManage_hpp */
