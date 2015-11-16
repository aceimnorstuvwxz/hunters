//
//  SingleBeat.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/6.
//
//

#ifndef LongBeat_hpp
#define LongBeat_hpp

#include <stdio.h>
#include "QuestProtocals.hpp"
#include "PixelNode.h"



// 普通剑
// 按一定速度前进，tap-ok会被反射，tap-fail会受到攻击，可设置初始速度，不会变速
class LongBeat: public BeatProtocal
{
public:
    static std::shared_ptr<LongBeat> create(){ return std::make_shared<LongBeat>();}
    void init(BeatFieldProtocal* beatFieldProtocal, cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera, const BeatFieldLineType& line, float speed, int longNum);

    virtual BeatTouchResult op_touchBegin(const BeatFieldLineType& line) override;
    virtual BeatTouchResult op_touchEnd(const BeatFieldLineType& line) override;
    virtual bool op_update(float dt) override;
    virtual void clear() override; //外部要求它从场景中删除

protected:
    static int _scheduleCnt;
    int _longNum;
    float _speed;
    bool _tapping = false;

    BeatFieldProtocal* _beatFieldProtocal;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;
    constexpr static int LONG_NUM_MAX = 10;
    PixelNode* _pxContent[LONG_NUM_MAX];

};
#endif /* LongBeat_hpp */
