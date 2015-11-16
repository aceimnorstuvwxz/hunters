//
//  RivalGraphicNode.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/4.
//
//

#include "RivalGraphicNode.hpp"
#include "format.h"


USING_NS_CC;

void RivalGraphicNode::init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera)
// 有2个可以设置的状态，分别是idle和defense，同时还可以播放一个attack动作
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    // hubnode
    _hubNode = Sprite3D::create(); // empty node
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _mainLayer->addChild(_hubNode);

    // body's parts
    for (int i = 0; i < static_cast<int>(HeroGraphicType::MAX); i++) {
        _bodyParts[i] = PixelNode::create();
        _bodyParts[i]->setCameraMask(_mainCamera->getCameraMask());
        _hubNode->addChild(_bodyParts[i]);

    }


    // atks
    for (int i = 0; i < NUM_ATK; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        _hubNode->addChild(node);
        node->configSopx(fmt::sprintf("sopx/rival/atk/%d.png.sopx", i));
        node->setVisible(false);
        node->setPositionZ(5);
        _atks[i] = node;
    }

}
void RivalGraphicNode::op_config(int whichRival) //配置，必须被配置，如果init后，没被配置就上场，会错误。必须对所有类型进行配置 设置哪一个套装
{

    for (int type = 0; type < static_cast<int>(RivalGraphicType::MAX); type++) {

        auto animationConfig = RivalGraphicAnimationDef::fetchAnimationConfig(static_cast<RivalGraphicType>(type), RivalGraphicMode::IDLE, whichRival);
        assert(animationConfig.size());
        auto pixelNode = _bodyParts[type];
        pixelNode->configSopx(fmt::sprintf("sopx/rival/%d-%s.png.sopx", whichRival, rivalGraphicType2string(static_cast<RivalGraphicType>(type))));
        pixelNode->setPosition3D(animationConfig.back().position);
        pixelNode->setRotation3D(animationConfig.back().rotation);
        pixelNode->setScale(animationConfig.back().scaler);
        pixelNode->stopAllActions();
        pixelNode->setOpacity(255);
        pixelNode->setVisible(true);

        if (animationConfig.size() > 1) {
            Vector<FiniteTimeAction*> actionArr; // *注意这里的vector是CC的！
            for (int i = 0; i < animationConfig.size(); i++) {
                auto& ani = animationConfig[i];
                actionArr.pushBack( Spawn::create(MoveTo::create(ani.moveTime, ani.position), RotateTo::create(ani.moveTime, ani.rotation), ScaleTo::create(ani.moveTime, ani.scaler), NULL));
                actionArr.pushBack(DelayTime::create(ani.stayTime));
            }

            pixelNode->runAction(RepeatForever::create( Sequence::create(actionArr) ));
        }
    }
    _hubNode->setOpacity(255);

}

void RivalGraphicNode::op_setVisibility(bool enable) //设置显示与否 当退到幕后时需要
{
    _hubNode->setVisible(enable);
}
void RivalGraphicNode::op_attack() //播放一次attack动画
{

}

void RivalGraphicNode::op_startDefense() //进入defense状态并维持多久，在维持期间调用可以延长维持时间
{

}

void RivalGraphicNode::op_stopDefense()
{
    
}

void RivalGraphicNode::op_move(const cocos2d::Vec3& pos,float time, bool direct) //位置移动
{
    if (direct) {
        _hubNode->setPosition3D(pos);
    } else {
        _hubNode->runAction(MoveTo::create(time, pos));
    }
}

void RivalGraphicNode::op_rotate(const cocos2d::Vec3& rotation, float time, bool direct)  //旋转
{
    if (direct) {
        _hubNode->setRotation3D(rotation);
    } else {
        _hubNode->runAction(RotateTo::create(time, rotation));
    }
}

void RivalGraphicNode::op_damage()
{
    for (auto p : _bodyParts) {
        p->configMixColorAni({0.7f,0,0,1}, 0.2);
    }
    _hubNode->runAction(Sequence::create(MoveBy::create(0.033, {-1,0,0}), MoveBy::create(0.033, {2,0,0}), MoveBy::create(0.033, {-1,0,0}), NULL) );

    _atks[_currentAtkIndex]->setOpacity(255);
    _atks[_currentAtkIndex]->setPosition(random(-10, 10), random(3,20));
    _atks[_currentAtkIndex]->runAction(
        Sequence::create(Show::create(), DelayTime::create(0.1), Hide::create(), NULL));

    _currentAtkIndex ++;
    if (_currentAtkIndex >= NUM_ATK) {
        _currentAtkIndex = 0;
    }
}

void RivalGraphicNode::op_health()
{
    // TODO 改成治疗幕从下往上
    for (auto p : _bodyParts) {
        p->configMixColorAni({0,0.7f,0,1}, 0.2);
    }
}

void RivalGraphicNode::op_die()
{
    for (auto p : _bodyParts) {
        p->configMixColorAni({0.7f,0,0,1}, 0.2, 5);
    }
    _hubNode->runAction(Sequence::create(Repeat::create(Sequence::create(MoveBy::create(0.1, {0,2,0}), MoveBy::create(0.1, {0,2,0}), NULL), 4), FadeOut::create(0.5), NULL));


    _mainCamera->schedule([this](float dt){
        this->op_damage();
    }, 0.15, 5, 0.05, "rival die sequence");

    // 然后爆炸 pos_expand TODO
        _mainCamera->scheduleOnce([this](float dt){
            for (auto p : _bodyParts) {
                p->runAction(FadeOut::create(0.2));
            }
            for (int i = 0; i < NUM_ATK; i++) {
                _atks[i]->runAction(FadeOut::create(0.2));
            }
        }, 5*0.15*1.3, "rival die dismiss");

}