// (C) 2015 Turnro.com

#ifndef WelcoScene_hpp
#define WelcoScene_hpp

#include "TRBaseScene.h"
#include "PixelNode.h"
#include "PlanePixelNode.h"
#include "RoadPlane.h"


// 欢迎页
// 包括 音乐、音效的开关， 游戏开始/继续、排行、credits按钮
class WelcoScene:public TRBaseScene
{
public:
    virtual bool init() override;
    CREATE_FUNC(WelcoScene);

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    constexpr static const float scene_things_start_pos = -300;
    constexpr static const float scene_things_width = 800;
    const float tree_width_step = 50;
    const float glass_width_step = 60;
    const float moutain_width_step = 70;
    const float hill_width_step = 90;


    PlanePixelNode* _leftGlass;
    PlanePixelNode* _leftTrees;
    PlanePixelNode* _moutains;
    PlanePixelNode* _hills;
    RoadPlane* _roadPlane;
    RoadPlane* _skyPlane;
    void initBackgroundThings();

    PixelNode* _pxTitle;
    PixelNode* _pxPlay;
    PixelNode* _pxLeaderboard;
    PixelNode* _pxCredits;

    PixelNode* _pxMusicSwitch;
    PixelNode* _pxSoundSwitch;

    void initButtonsThings();
    void initExtraButtons();
    void initTouchThings();

    void update(float dt) override;


};

#endif /* WelcoScene */
