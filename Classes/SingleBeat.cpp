//
//  SingleBeat.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/6.
//
//

#include "SingleBeat.hpp"
#include "QuestDef.hpp"
#include "SimpleAudioEngine.h"
#include "BattleState.hpp"
#include "format.h"

USING_NS_CC;
int SingleBeat::_scheduleCnt = 0;

void SingleBeat::init(BeatFieldProtocal* beatFieldProtocal, cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera, const BeatFieldLineType& line, float speed)
{
    _beatFieldProtocal = beatFieldProtocal;
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
    _speed = speed;

    _pxContent = PixelNode::create();
    _pxContent->setCameraMask(_mainCamera->getCameraMask());
    _pxContent->setPosition3D({static_cast<int>(line)*QuestDef::BEAT_LINE_WIDTH, QuestDef::BEAT_RIVAL_START_LINE, QuestDef::BEAT_COME_HEIGHT});
    _pxContent->configSopx("sopx/beats/single_beat_content.png.sopx");
    _mainLayer->addChild(_pxContent);

    _pxContent->setOpacity(0);
    _pxContent->runAction(FadeIn::create(QuestDef::BEAT_SHOWUP_TIME));

    _lineType = line;
    _beatType = BeatType::SINGLE;
}

BeatTouchResult SingleBeat::op_touchBegin(const BeatFieldLineType& line)
{
    if (_lineType == line && _speed <= 0 && std::abs(_pxContent->getPositionY() - QuestDef::BEAT_HERO_TAP_LINE) <= (8+QuestDef::TAP_AREA_HALF_WIDTH)) {

        CCLOG("tap single beat");
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/test.wav");

        _pxContent->runAction(RotateBy::create(0.2, {180,0,0}));
        _speed = -_speed*QuestDef::BEAT_BACK_RADIO;
        return {true, false, true, false};
    }
    return {false, false, false, false};
}

BeatTouchResult SingleBeat::op_touchEnd(const BeatFieldLineType& line)
{
    // 并不是长按beat
    return {false, false, false, false};
}

bool SingleBeat::op_update(float dt)
{
    // 移动
    _pxContent->setPositionY(_pxContent->getPositionY() + dt * _speed);

    // 检查碰撞
    // single beat的hit 检查是简单的
    if (_speed <= 0 && _pxContent->getPositionY() < QuestDef::BEAT_HERO_HIT_LINE) {
        CCLOG("single beat hit hero");
        // 击中hero

        bool back = BattleState::s()->heroUnderAttack(getBeatType());
        if (back) {
            CCLOG("single beat been shield back");
            //被盾反
            _pxContent->runAction(RotateBy::create(0.2, {180,0,0}));
            _speed = -_speed*QuestDef::BEAT_BACK_RADIO;
            return false;
        } else {
            // beat执行撞击后消散的动画
            _pxContent->runAction(Spawn::create(FadeOut::create(0.2), MoveBy::create(1, {0,_speed,0}), NULL));
            // 然后从layer删除
            auto p = _pxContent;
            _pxContent->scheduleOnce([p](float dt) {
                p->getParent()->removeChild(p);
            }, 0.2, fmt::sprintf("single beat remove from parent %d", _scheduleCnt++));

            return true; // 告诉BeatField我已经删除
        }

    }
    if (_speed >= 0 && _pxContent->getPositionY() > QuestDef::BEAT_RIVAL_HIT_LINE) {
        CCLOG("single beat hit rival");
        // 击中rival
        bool back = BattleState::s()->rivalUnderAttack(getBeatType());
        if (back) {
            _pxContent->runAction(RotateBy::create(0.2, {180,0,0}));
            _speed = -_speed*QuestDef::BEAT_BACK_RADIO;
            return false;
        } else {
            _pxContent->runAction(FadeOut::create(0.2));

            auto p = _pxContent;
            _pxContent->scheduleOnce([p](float dt) {
                p->getParent()->removeChild(p);
            }, 0.2, fmt::sprintf("single beat remove from parent %d", _scheduleCnt++));
        }

        return true;
    }
    return false;
}

void SingleBeat::clear() //外部要求它从场景中删除
{
    // beat执行撞击后消散的动画
    _pxContent->runAction(Spawn::create(FadeOut::create(QuestDef::BEAT_CLEAR_TIME), MoveBy::create(1, {0,_speed,0}), NULL));
    // 然后从layer删除
    auto p = _pxContent;
    _pxContent->scheduleOnce([p](float dt) {
        p->getParent()->removeChild(p);
    }, 0.2, fmt::sprintf("health beat remove from parent %d", _scheduleCnt++));
}