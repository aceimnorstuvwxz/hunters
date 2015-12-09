//
//  PowerBar.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/22.
//
//

#ifndef PowerBar_hpp
#define PowerBar_hpp

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
    void configProtocals(HuntingHerosManageProtocal* huntingHerosManageProtocal, WindBarProtocal* windParProtocal,HuntingArrowManageProtocal* huntingArrowManageProtocal) {
        _huntingHerosManageProtocal = huntingHerosManageProtocal;
        _windBarProtocal = windParProtocal;
        _huntingArrowManageProtocal = huntingArrowManageProtocal;
    }
    virtual void op_configEnable(bool enable){_enable = enable;} //放箭触摸瞄准的开关
    virtual void op_show();
    virtual void op_dismiss();
    virtual void op_configPower(float power, int angle); //power [0-1]
    virtual void op_firstHeroPosition(HeroPositionType t);

    void update(float dt);

protected:
    HuntingArrowManageProtocal* _huntingArrowManageProtocal;
    HuntingHerosManageProtocal* _huntingHerosManageProtocal;
    WindBarProtocal* _windBarProtocal;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Node* _hubNode;
    void initHubThings();
    bool _enable = true;

    static constexpr int N_PATH_POINTS = 10;
    cocos2d::Node* _pathHubNode;
    PixelNode* _pxPathPoints[N_PATH_POINTS];
    void initPathThings();
    void showPath(float angle, float strenth);
    void hidePath();


    PixelNode* _pxRect;
    PixelNode* _pxContent;
    PixelNode* _pxArrow;
    PixelTextNode* _ptAngle;
    void initPowerThings();
    void initTouchThings();
};
#endif /* PowerBar_hpp */
