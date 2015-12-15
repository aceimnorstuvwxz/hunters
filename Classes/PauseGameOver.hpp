//
//  PauseGameOver.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/15.
//
//

#ifndef PauseGameOver_hpp
#define PauseGameOver_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FlyingCrow.hpp"
#include "PixelParticleNode.hpp"
#include "RoadPlane.h"

struct CBFBuffton
{
    PixelNode* rect;
    PixelNode* icon;
};

class PauseGameOver :public PauseGameOverProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);
    void configProtocals(QUestSceneProtocal* questSceneProtocal, TopIconsProtocal* topIconsProtocal){
        _questSceneProtocal = questSceneProtocal;
        _topIconsProtocal = topIconsProtocal;
    }

    virtual void op_pause();
    virtual void op_gameOver();

    void update(float dt);

protected:
    QUestSceneProtocal* _questSceneProtocal;
    TopIconsProtocal* _topIconsProtocal;
    bool _paused = false;
    bool _goed = false;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();

    PixelTextNode* _ptTitle;

    CBFBuffton _btns[3];

    RoadPlane* _darkShadow;
    void initButtonsThings();
};
#endif /* PauseGameOver_hpp */
