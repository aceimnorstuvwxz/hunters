//
//  PowerBar.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/22.
//
//

#ifndef PowerBar_hpp
#define PowerBar_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"
#include "PlanePixelNode.h"

//力量和角度
class PowerBar:public PowerBarProtocal
{
public:
    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);

    virtual void op_show();
    virtual void op_dismiss();
    virtual void op_configPower(float power, int angle); //power [0-1]

    void update(float dt);

protected:
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();

    PixelNode* _pxRect;
    PixelNode* _pxContent;
    PixelTextNode* _ptAngle;
    void initPowerThings();
};
#endif /* PowerBar_hpp */
