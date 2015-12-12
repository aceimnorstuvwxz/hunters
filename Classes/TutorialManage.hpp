//
//  TurotialManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/12.
//
//

#ifndef TurotialManage_hpp
#define TurotialManage_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "DynamicPixelNode.hpp"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "FlyingCrow.hpp"
#include "PixelParticleNode.hpp"
#include "RoadPlane.h"

class TutorialManage: public TutorialManageProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);


    virtual void op_toastStory();
    virtual void op_toastEnergy();


    void update(float dt);

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    PixelTextNode* _ptMessage;
    cocos2d::Node* _hubNode;
    void initHubThings();

    RoadPlane* _darkShadow;
    void showString(std::string str);

    void initTutorialThings();

};
#endif /* TurotialManage_hpp */
