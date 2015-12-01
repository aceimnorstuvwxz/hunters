//
//  Box2dTestScene.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/1.
//
//

#ifndef Box2dTestScene_hpp
#define Box2dTestScene_hpp

#include <stdio.h>
#include "TRBaseScene.h"
class Box2dTestScene:public TRBaseScene
{
public:
    CREATE_FUNC(Box2dTestScene);
    bool init();

protected:

    cocos2d::Layer* _mainLayer;

    void initTestThings();
    void testAction();
    
};

#endif /* Box2dTestScene_hpp */
