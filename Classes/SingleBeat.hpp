//
//  SingleBeat.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/6.
//
//

#ifndef SingleBeat_hpp
#define SingleBeat_hpp

#include <stdio.h>
#include "QuestProtocals.hpp"
#include "PixelNode.h"



// 普通剑
// 按一定速度前进，tap-ok会被反射，tap-fail会受到攻击，可设置初始速度，不会变速
class SingleBeat: public BeatProtocal
{
public:
    static std::shared_ptr<SingleBeat> create(){ return std::make_shared<SingleBeat>();}
    void init(BeatFieldProtocal* beatFieldProtocal, cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera, const BeatFieldLineType& line, float speed);

    virtual BeatTouchResult op_touchBegin(const BeatFieldLineType& line) override;
    virtual BeatTouchResult op_touchEnd(const BeatFieldLineType& line) override;
    virtual bool op_update(float dt) override;
    virtual void clear() override; //外部要求它从场景中删除

protected:
    static int _scheduleCnt;
    BeatFieldProtocal* _beatFieldProtocal;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    PixelNode* _pxContent;
    float _speed;

};
#endif /* SingleBeat_hpp */
