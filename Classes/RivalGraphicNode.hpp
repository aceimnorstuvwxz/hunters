//
//  RivalGraphicNode.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/4.
//
//

#ifndef RivalGraphicNode_hpp
#define RivalGraphicNode_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "RivalGraphicAnimationDef.hpp"


class RivalGraphicNode:public RivalGraphicProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    // 有2个可以设置的状态，分别是idle和defense，同时还可以播放一个attack动作
    virtual void op_config(int whichRival) override; //配置，必须被配置，如果init后，没被配置就上场，会错误。必须对所有类型进行配置 设置哪一个套装
    virtual void op_setVisibility(bool enable) override; //设置显示与否 当退到幕后时需要
    virtual void op_attack() override; //播放一次attack动画
    virtual void op_startDefense() override; //进入defense状态并维持多久，在维持期间调用可以延长维持时间
    virtual void op_stopDefense() override;
    virtual void op_move(const cocos2d::Vec3& pos,float time, bool direct = false) override; //位置移动
    virtual void op_rotate(const cocos2d::Vec3& rotation, float time, bool direct = false) override; //旋转

    virtual void op_damage() override;
    virtual void op_health() override;
    virtual void op_die() override;

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    cocos2d::Sprite3D* _hubNode;
    PixelNode* _bodyParts[static_cast<int>(RivalGraphicType::MAX)];

    constexpr static const int NUM_ATK = 3;
    PixelNode* _atks[NUM_ATK];
    int _currentAtkIndex  = 0;
};

#endif /* RivalGraphicNode_hpp */
