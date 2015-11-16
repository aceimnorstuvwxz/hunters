//
//  SingleBeat.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/6.
//
//

#ifndef CopyBeat_hpp
#define CopyBeat_hpp

#include <stdio.h>
#include "QuestProtocals.hpp"
#include "PixelNode.h"



// 普通剑
// 按一定速度前进，tap-ok会被反射，tap-fail会受到攻击，可设置初始速度，不会变速
class CopyBeat: public BeatProtocal
{
public:
    static std::shared_ptr<CopyBeat> create(){ return std::make_shared<CopyBeat>();}
    void init(BeatFieldProtocal* beatFieldProtocal, cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera, const BeatFieldLineType& line, float speed, bool isMother);

    virtual BeatTouchResult op_touchBegin(const BeatFieldLineType& line) override;
    virtual BeatTouchResult op_touchEnd(const BeatFieldLineType& line) override;
    virtual bool op_update(float dt) override;
    virtual void clear() override; //外部要求它从场景中删除

protected:
    const float copy_wait_time = 1.f;
    bool _isMother; //如果是母亲，跑到come的中间会抖一下，如果是儿子，跑道come的中间会出现。
    static int _scheduleCnt;
    BeatFieldProtocal* _beatFieldProtocal;

    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    PixelNode* _pxContent;
    float _speed;

};
#endif /* CopyBeat_hpp */
