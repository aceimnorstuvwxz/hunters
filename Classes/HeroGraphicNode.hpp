//
//  HeroGraphicNode.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#ifndef HeroGraphicNode_hpp
#define HeroGraphicNode_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "HeroGraphicUnitAnimationDef.hpp"
//我的战士的图形

class HeroGraphicNode:public HeroGraphicProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    // 默认状态下始终在执行idle动画，即各个部件的微动，由于战斗时不可见，所以不会有其它动作。
    virtual void op_config(std::vector<std::pair<HeroGraphicType, int>> bodyPartsConfig) override; //配置，必须被配置，如果init后，没被配置就上场，会错误。必须对所有类型进行配置
    virtual void op_setVisibility(bool enable) override; //设置显示与否 当退到幕后时需要
    virtual void op_run(float time) override; //播放一段时间的跑动动画 * 不再需要，hero 一直在跑即可。新设计，hero 不再需要其它动作（其它不可见）
    virtual void op_move(const cocos2d::Vec3& pos,float time, bool direct = false) override; //位置移动
    virtual void op_rotate(const cocos2d::Vec3& rotation, float time, bool direct = false) override; //旋转

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    cocos2d::Sprite3D* _hubNode;
    PixelNode* _bodyParts[static_cast<int>(HeroGraphicType::MAX)];
    PixelNode* _foot0;
    PixelNode* _foot1;
    PixelNode* _foot2;
    int _currentLevels[static_cast<int>(HeroGraphicType::MAX)];
};

#endif /* HeroGraphicNode_hpp */
