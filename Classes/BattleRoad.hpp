// (C) 2015 Arisecbf

#ifndef BattleRoad_hpp
#define BattleRoad_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "SOCommon.h"
#include "QuestDef.hpp"
#include "QuestProtocals.hpp"
#include "RoadPlane.h"
#include "PlanePixelNode.h"
#include "PixelNode.h"

// 战斗场景
// 有多套战斗场景

class BattleRoad:public BattleRoadProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    virtual void op_config(int which) override; //用来设置战斗场景
    virtual void op_vibrate() override; //振动
    virtual void op_moveForword(float moveTime) override; //场景随hero前进，前进的时间和距离公共定义
    virtual void op_setRightTreesVisible(bool enable) override; //右侧树在镜头旋转的一定角度需要显示/隐藏
    virtual void op_setLeftHillsVisible(bool enable) override; //左侧山，减少 vertex 量
    void update(float dt);

    virtual void op_applyDarkShadow(float howdark) override; //场景变暗，用来处理 rival 的死亡，及 combo 大招
    virtual void op_dismissDarkShadow() override; //取消场景变暗
    virtual void op_toastDarkShadow(float howdark, float time) override; //toast 形式的接口

    virtual void op_configWind(float windDirection); //设置风的方向，表现为树的移动、云的飘动，正-向右，负-向左，大小表示风的强度


protected:

    constexpr static const float scene_things_start_pos = -300;
    constexpr static const float scene_things_width = 800;
    const float tree_width_step = 50;
    const float glass_width_step = 60;
    const float moutain_width_step = 70;
    const float hill_width_step = 90;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    PlanePixelNode* _leftGlass;
//    PlanePixelNode* _rightGlass;
    PlanePixelNode* _leftTrees;
//    PlanePixelNode* _rightTrees;
    PlanePixelNode* _moutains;
    PlanePixelNode* _hills;
    RoadPlane* _roadPlane;
    RoadPlane* _skyPlane;

    RoadPlane* _leftShadow;
    RoadPlane* _rightShadow;
    RoadPlane* _middleShadow;

    PixelNode* _pxCastle;
    PixelNode* _pxCantleBg;
    void initCastleThings();

};

#endif /* BattleRoad_hpp */
