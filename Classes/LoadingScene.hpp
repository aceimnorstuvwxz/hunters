//
//  LoadingScene.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/9.
//
//

#ifndef LoadingScene_hpp
#define LoadingScene_hpp


#include "TRBaseScene.h"
#include "PixelNode.h"
#include "PlanePixelNode.h"
#include "RoadPlane.h"

// 加载页
// 
class LoadingScene:public TRBaseScene
{
public:
    virtual bool init() override;
    CREATE_FUNC(LoadingScene);

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    void loadingTask0();
    void loadingTask1();

};
#endif /* LoadingScene_hpp */
