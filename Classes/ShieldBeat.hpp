//
//  ShieldBeat.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/8.
//
//

#ifndef ShieldBeat_hpp
#define ShieldBeat_hpp


#include <stdio.h>
#include "QuestProtocals.hpp"
#include "PixelNode.h"

// 盾

class ShieldBeat: public BeatProtocal
{
public:
    static std::shared_ptr<ShieldBeat> create() { return std::make_shared<ShieldBeat>(); }
    void init(BeatFieldProtocal* beatFieldProtocal, cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera, const BeatFieldLineType& line, float speed); //盾 的速度是恒定的，但所有盾的速度可以按游戏进行的阶段进行一定的整体调整

    virtual BeatTouchResult op_touchBegin(const BeatFieldLineType& line) override;
    virtual BeatTouchResult op_touchEnd(const BeatFieldLineType& line) override;
    virtual bool op_update(float dt) override;
    virtual void clear() override; //外部要求它从场景中删除

protected:
    static int _scheduleCnt;
    BeatFieldProtocal* _beatFieldProtocal;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    PixelNode* _pxTapRect;
    PixelNode* _pxContent;
    float _speed;

};
#endif /* ShieldBeat_hpp */
