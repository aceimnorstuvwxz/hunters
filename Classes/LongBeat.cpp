//
//  SingleBeat.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/6.
//
//

#include "LongBeat.hpp"
#include "QuestDef.hpp"
#include "SimpleAudioEngine.h"
#include "BattleState.hpp"
#include "format.h"

USING_NS_CC;

int LongBeat::_scheduleCnt = 0;

void LongBeat::init(BeatFieldProtocal* beatFieldProtocal, cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera, const BeatFieldLineType& line, float speed, int longNum)
{
    _beatFieldProtocal = beatFieldProtocal;
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
    _speed = speed;
    _longNum = longNum;

    assert(_longNum <= LONG_NUM_MAX);

    const float LONG_STEP = 4;
    for (int i = 0; i < _longNum; i++) {
        _pxContent[i] = PixelNode::create();
        _pxContent[i]->setCameraMask(_mainCamera->getCameraMask());
        _pxContent[i]->setPosition3D({static_cast<int>(line)*QuestDef::BEAT_LINE_WIDTH, QuestDef::BEAT_RIVAL_START_LINE + (i*LONG_STEP), QuestDef::BEAT_COME_HEIGHT});
        _pxContent[i]->configSopx("sopx/beats/long_beat_content.png.sopx");
        _mainLayer->addChild(_pxContent[i]);

        _pxContent[i]->setOpacity(0);
        _pxContent[i]->runAction(FadeIn::create(QuestDef::BEAT_SHOWUP_TIME));

    }
    _lineType = line;
    _beatType = BeatType::HEALTH;
}

BeatTouchResult LongBeat::op_touchBegin(const BeatFieldLineType& line)
{
    if (_lineType == line && _speed <= 0 ) {
        _tapping = true;
    }
    return {false, false, false, false};
}

BeatTouchResult LongBeat::op_touchEnd(const BeatFieldLineType& line)
{
    // 并不是长按beat
    if (_lineType == line) {
        _tapping = false;
    }
    return {false, false, false, false};
}

bool LongBeat::op_update(float dt)
{
    // 移动
    bool deleteAll = true;
    for (int i = 0; i < _longNum; i++) {
        if (_pxContent[i] == nullptr) {
            continue;
        }
        float oldPos = _pxContent[i]->getPositionY();
        _pxContent[i]->setPositionY(_pxContent[i]->getPositionY() + dt * _speed);
        float newPos = _pxContent[i]->getPositionY();
        if ( oldPos >= QuestDef::BEAT_HERO_TAP_LINE && newPos < QuestDef::BEAT_HERO_TAP_LINE && _tapping) {
            // 防住了
            _pxContent[i]->runAction(FadeOut::create(0.1));
            auto p = _pxContent[i];
            _pxContent[i]->scheduleOnce([p](float dt) {
                p->getParent()->removeChild(p);
            }, 0.1, fmt::sprintf("long beat remove from parent %d", _scheduleCnt++));

            _pxContent[i] = nullptr;
        } else if (newPos < QuestDef::BEAT_HERO_HIT_LINE) {
            // 击中Hero
            BattleState::s()->heroUnderAttack(BeatType::LONG);
            _pxContent[i]->runAction(FadeOut::create(0.1));
            auto p = _pxContent[i];
            _pxContent[i]->scheduleOnce([p](float dt) {
                p->getParent()->removeChild(p);
            }, 0.1, fmt::sprintf("long beat remove from parent %d", _scheduleCnt++));

            _pxContent[i] = nullptr;
        } else {
            deleteAll = false;
        }
    }
    return deleteAll;
}

void LongBeat::clear() //外部要求它从场景中删除
{
    for (int i = 0; i < _longNum; i++) {
        if (_pxContent[i] == nullptr) {
            continue;
        } else {
            // beat执行撞击后消散的动画
            _pxContent[i]->runAction(Spawn::create(FadeOut::create(QuestDef::BEAT_CLEAR_TIME), MoveBy::create(1, {0,_speed,0}), NULL));
            // 然后从layer删除
            auto p = _pxContent[i];
            _pxContent[i]->scheduleOnce([p](float dt) {
                p->getParent()->removeChild(p);
            }, 0.2, fmt::sprintf("long beat remove from parent %d", _scheduleCnt++));
        }
    }

}