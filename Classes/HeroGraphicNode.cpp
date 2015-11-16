//
//  HeroGraphicNode.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#include "HeroGraphicNode.hpp"
#include "format.h"
#include "QuestDef.hpp"

USING_NS_CC;
void HeroGraphicNode::init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera)
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

        _currentLevels[i] = -1;
    }

    // run foots
    _foot0 = PixelNode::create();
    _foot0->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->addChild(_foot0);

    _foot1 = PixelNode::create();
    _foot1->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->addChild(_foot1);

    _foot2 = PixelNode::create();
    _foot2->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->addChild(_foot2);
}

void HeroGraphicNode::op_config(std::vector<std::pair<HeroGraphicType, int>> bodyPartsConfig) //配置，必须被配置
{
    assert(bodyPartsConfig.size() == static_cast<int>(HeroGraphicType::MAX));

    for (auto p : bodyPartsConfig) {
        if (_currentLevels[static_cast<int>(p.first)] == p.second) {
            //与之前相同
            continue;
        }
        _currentLevels[static_cast<int>(p.first)] = p.second;


        auto animationConfig = HeroGraphicAnimationDef::fetchAnimationConfig(p.first, p.second);
        auto pixelNode = _bodyParts[static_cast<int>(p.first)];
        pixelNode->configSopx(fmt::sprintf("sopx/hero/%s-%d.png.sopx", heroGraphicType2string(p.first), p.second));
        pixelNode->setPosition3D(animationConfig.back().position);
        pixelNode->setRotation3D(animationConfig.back().rotation);
        pixelNode->setScale(animationConfig.back().scaler);
        pixelNode->stopAllActions();

        if (p.first == HeroGraphicType::BODY) {

            //隐藏原来的 body
            pixelNode->setVisible(false);
            //foot的位置始终与 body 相同
            _foot0->configSopx("sopx/hero/FOOT_a.png.sopx");
            _foot0->setPosition3D(animationConfig.back().position);
            _foot0->setRotation3D(animationConfig.back().rotation);
            _foot0->setScale(animationConfig.back().scaler);
            _foot0->setVisible(false);
            _foot0->stopAllActions();

            _foot1->configSopx("sopx/hero/FOOT_b.png.sopx");
            _foot1->setPosition3D(animationConfig.back().position);
            _foot1->setRotation3D(animationConfig.back().rotation);
            _foot1->setScale(animationConfig.back().scaler);
            _foot1->stopAllActions();
            _foot1->setVisible(false);

            _foot2->configSopx("sopx/hero/FOOT_c.png.sopx");
            _foot2->setPosition3D(animationConfig.back().position);
            _foot2->setRotation3D(animationConfig.back().rotation);
            _foot2->setScale(animationConfig.back().scaler);
            _foot2->stopAllActions();
            _foot2->setVisible(false);

            // foot 跑起来
            const float run_unit_time = QuestDef::HERO_RUN_UNIT_TIME;
            const float z_run_length = QuestDef::HERO_RUN_HEIGHT;
            _hubNode->runAction(RepeatForever::create(Sequence::create(MoveBy::create(2.5*run_unit_time, {0,0,-z_run_length*0.5f}),MoveBy::create(5*run_unit_time, {0,0,z_run_length}),MoveBy::create(2.5*run_unit_time, {0,0,-z_run_length*0.5f}), NULL)));

            _foot0->runAction(RepeatForever::create(Sequence::create(
                                                                     DelayTime::create(5*run_unit_time),
                                                                     Show::create(),
                                                                     DelayTime::create(5*run_unit_time),
                                                                     Hide::create(),
                                                                     DelayTime::create(10*run_unit_time),
                                                                     NULL)));
            _foot1->runAction(RepeatForever::create(Sequence::create(
                                                                     Show::create(),
                                                                     DelayTime::create(5*run_unit_time),
                                                                     Hide::create(),
                                                                     DelayTime::create(5*run_unit_time),
                                                                     Show::create(),
                                                                     DelayTime::create(5*run_unit_time),
                                                                     Hide::create(),
                                                                     DelayTime::create(5*run_unit_time),
                                                                     NULL)));
            _foot2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(15*run_unit_time),
                                                                     Show::create(), DelayTime::create(5*run_unit_time), Hide::create(), NULL)));
        }

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
}

void HeroGraphicNode::op_setVisibility(bool enable) //设置显示与否 当退到幕后时需要
{
    _hubNode->setVisible(enable);
}

void HeroGraphicNode::op_run(float time) //播放一段时间的跑动动画
{

}

void HeroGraphicNode::op_move(const cocos2d::Vec3& pos,float time, bool direct) //位置移动
{
    if (direct) {
        _hubNode->setPosition3D(pos);
    } else {
        _hubNode->runAction(MoveTo::create(time, pos));
    }
}

void HeroGraphicNode::op_rotate(const cocos2d::Vec3& rotation, float time, bool direct) //旋转
{
    if (direct) {
        _hubNode->setRotation3D(rotation);
    } else {
        _hubNode->runAction(RotateTo::create(time, rotation));
    }
}
