//
//  CreditsScene.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/13.
//
//

#ifndef CreditsScene_hpp
#define CreditsScene_hpp

#include <stdio.h>
#include "TRBaseScene.h"
#include "PixelNode.h"
#include "PlanePixelNode.h"
#include "RoadPlane.h"
#include "PixelTextNode.hpp"


//
class CreditsScene:public TRBaseScene
{
public:
    virtual bool init() override;
    CREATE_FUNC(CreditsScene);

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    PixelNode* _pxBack;
    Node* _hub;

    void initCreditsThings();
    void initTouchThings();

};

#endif /* CreditsScene_hpp */
