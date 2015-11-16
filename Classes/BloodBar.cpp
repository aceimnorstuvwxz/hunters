//
//  BloodBar.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/5.
//
//

#include "BloodBar.hpp"
#include "QuestDef.hpp"
#include "format.h"

USING_NS_CC;


void BloodBar::init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;


    // Hero
    _heroHubNode = Sprite3D::create();
    _heroHubNode->setCameraMask(_mainCamera->getCameraMask());
    _heroHubNode->setPosition3D({0,-QuestDef::POS_Y_ROLE,30});
    _heroHubNode->setScale(0.2f);
    _heroHubNode->setRotation3D({60,0,0});
    // TODO position rotation
    _mainLayer->addChild(_heroHubNode);


    _pxHeroOutline = PixelNode::create();
    _pxHeroOutline->setCameraMask(_mainCamera->getCameraMask());
    _pxHeroOutline->setPosition3D({0,0,0});
    _pxHeroOutline->configSopx("sopx/bloodbar/hero_outline.png.sopx");
    _heroHubNode->addChild(_pxHeroOutline);

    _pxHeroBlood = PixelNode::create();
    _pxHeroBlood->setCameraMask(_mainCamera->getCameraMask());
    _pxHeroBlood->setPosition3D({-74,-1,0});
    _pxHeroBlood->setScaleX(1);
    _pxHeroBlood->configSopx("sopx/bloodbar/hero_blood.png.sopx");
    _heroHubNode->addChild(_pxHeroBlood);

    _pxHeroIcons = PixelNode::create();
    _pxHeroIcons->setCameraMask(_mainCamera->getCameraMask());
    _pxHeroIcons->setScale(0.65f);
    _pxHeroIcons->setPosition3D({-35,7.5f,0});
    _pxHeroIcons->configSopx("sopx/bloodbar/hero_icons.png.sopx");
    _heroHubNode->addChild(_pxHeroIcons);


    _pxtHeroBloodNum = PixelTextNode::create();
    _pxtHeroBloodNum->setCameraMask(_mainCamera->getCameraMask());
    _pxtHeroBloodNum->setScale(1);
    _pxtHeroBloodNum->setPosition3D({-13,0,0.5});
    _pxtHeroBloodNum->configText("??\/??",1);
    _pxtHeroBloodNum->configMixColor({1,1,1,1});
    _heroHubNode->addChild(_pxtHeroBloodNum);

    _pxtHeroAttackPwr = PixelTextNode::create();
    _pxtHeroAttackPwr->setCameraMask(_mainCamera->getCameraMask());
    _pxtHeroAttackPwr->setScale(1);
    _pxtHeroAttackPwr->setPosition3D({-19,7.5,0.5});
    _pxtHeroAttackPwr->configText("?? - ??      ??",1);
    _pxtHeroAttackPwr->configMixColor({1,1,1,1});
    _heroHubNode->addChild(_pxtHeroAttackPwr);



    // Rival
    _rivalHubNode = Node::create();
    _rivalHubNode->setCameraMask(_mainCamera->getCameraMask());
    _rivalHubNode->setPosition3D({0,-QuestDef::POS_Y_ROLE+8,43.2});
    _rivalHubNode->setScale(0.16f);
    _rivalHubNode->setRotation3D({60,0,0});
    _rivalHubNode->setZOrder(2);
    // TODO position rotation
    _mainLayer->addChild(_rivalHubNode);

    _pxRivalOutline = PixelNode::create();
    _pxRivalOutline->setCameraMask(_mainCamera->getCameraMask());
    _pxRivalOutline->setPosition3D({0,0,0});
    _pxRivalOutline->configSopx("sopx/bloodbar/rival_outline.png.sopx");
    _rivalHubNode->addChild(_pxRivalOutline);

    _pxRivalHead = PixelNode::create();
    _pxRivalHead->setCameraMask(_mainCamera->getCameraMask());
    _pxRivalHead->setPosition3D({80,-4.5,0.1f});
    _pxRivalHead->configSopx(fmt::sprintf("sopx/bloodbar/rival_head/%d.png.sopx", _rivalHeadIndex));
    _rivalHubNode->addChild(_pxRivalHead);

    _pxRivalIcons = PixelNode::create();
    _pxRivalIcons->setCameraMask(_mainCamera->getCameraMask());
    _pxRivalIcons->setPosition3D({65,-8,0.1f});
    _pxRivalIcons->setScale(0.65f);
    _pxRivalIcons->configSopx("sopx/bloodbar/rival_icons.png.sopx");
    _rivalHubNode->addChild(_pxRivalIcons);

    // combo
    _pxComboRect = PixelNode::create();
    _pxComboRect->setCameraMask(_mainCamera->getCameraMask());
    _pxComboRect->setScale(1);
    _pxComboRect->setPosition3D({0,-20,0});
    _pxComboRect->configSopx("sopx/bloodbar/combo_rect.png.sopx");
    _rivalHubNode->addChild(_pxComboRect);
    _pxComboRect->configBlend(true);
    _pxComboRect->setZOrder(2);

    _pxComboContent = PixelNode::create();
    _pxComboContent->setCameraMask(_mainCamera->getCameraMask());
    _pxComboContent->setScale(1);
    _pxComboContent->setPosition3D({-28,-20,0});
    _pxComboContent->configSopx("sopx/bloodbar/combo_content.png.sopx");
    _rivalHubNode->addChild(_pxComboContent);
    _pxComboContent->configBlend(true);
    _pxComboContent->setZOrder(2);


    for (int i = 0; i < 5; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({74,-2,-0.2f+0.01f*i});
        node->setScaleX(1);
        node->configSopx(fmt::sprintf("sopx/bloodbar/rival_blood_%d.png.sopx", i));
//        node->setVisible(false);
        _rivalHubNode->addChild(node);
        _pxRivalBloods[i] = node;
    }

    _pxtRivalBloodNum = PixelTextNode::create();
    _pxtRivalBloodNum->setCameraMask(_mainCamera->getCameraMask());
    _pxtRivalBloodNum->setScale(1);
    _pxtRivalBloodNum->setPosition3D({13,-4,0.6});
    _pxtRivalBloodNum->configText("??\/??",1);
    _pxtRivalBloodNum->configMixColor({1,1,1,1});
    _rivalHubNode->addChild(_pxtRivalBloodNum);

    _pxtRivalAttack = PixelTextNode::create();
    _pxtRivalAttack->setCameraMask(_mainCamera->getCameraMask());
    _pxtRivalAttack->setScale(1);
    _pxtRivalAttack->setPosition3D({36,-7.5,0.5});
    _pxtRivalAttack->configText("?? - ??",1);
    _pxtRivalAttack->configMixColor({1,1,1,1});
    _rivalHubNode->addChild(_pxtRivalAttack);



    op_rival_reset();
    op_configCombo(0);
    op_dismiss();
}

void BloodBar::op_hero_configBlood(int bloodMax, int blood, bool direct)
{
    float radio = blood*1.f/bloodMax;
    CCLOG("hero blood radio=%f",radio);
    const float start_pos = -74;
    const float total_width = 120;
    float scale = total_width*radio/1;
    float pos = total_width*radio/2+start_pos;
    if (direct) {
        _pxHeroBlood->setPositionX(pos);
        _pxHeroBlood->setScaleX(scale);
    } else {
        _pxHeroBlood->runAction(Spawn::create(MoveTo::create(blood_change_time, {pos, _pxHeroBlood->getPositionY(), _pxHeroBlood->getPositionZ()}),ScaleTo::create(blood_change_time, scale, _pxHeroBlood->getScaleY(), _pxHeroBlood->getScaleZ()), NULL));
    }

    _pxtHeroBloodNum->configText(fmt::sprintf("%d\/%d", blood, bloodMax), 1);
}
void BloodBar::op_hero_configAttack(int attack_min, int attack_max, int pwr, bool direct)
{
    _pxtHeroAttackPwr->configText(fmt::sprintf("%02d - %02d      %02d", attack_min, attack_max, pwr), 1);
}

void BloodBar::op_rival_configHead(int which)
{
    if (which != _rivalHeadIndex) {
        _rivalHeadIndex = which;
        _pxRivalHead->configSopx(fmt::sprintf("sopx/bloodbar/rival_head/%d.png.sopx", _rivalHeadIndex));
    }
}
void BloodBar::op_rival_configBlood(int bloodMax, int blood, int cnt, bool direct) //cnt 1-5，血条数目
{
    assert(cnt >= 1 && cnt <= 5);

    int whichColor = std::floor(blood/(bloodMax*1.f/cnt));
    float radio = (blood/(bloodMax*1.f/cnt)) - whichColor;
    if (radio == 0 && whichColor > 0) {
        radio = 1.f;
        whichColor = whichColor-1;
    }
    const float start_pos = 74;
    const float start_scale = 1;
    const float total_width = 128;
    float scale = total_width*radio/1;
    float pos = start_pos - total_width*radio/2;
    float scale_total = total_width/1;
    float pos_total = start_pos - total_width/2;



    if (whichColor == _oldRivalBloodIndex) {
        auto node = _pxRivalBloods[whichColor];
        if (direct) {
            node->setScaleX(scale);
            node->setPositionX(pos);
        } else {
            node->runAction(Spawn::create(MoveTo::create(blood_change_time, {pos, node->getPositionY(), node->getPositionZ()}),ScaleTo::create(blood_change_time, scale, node->getScaleY(), node->getScaleZ()), NULL));
        }
    } else if (whichColor < _oldRivalBloodIndex) {
        // 级别降低
        float delay_time = 0;
        for (int lv = _oldRivalBloodIndex; lv >= whichColor; lv--) {

            auto node = _pxRivalBloods[lv];
            //设初始
            if (lv != _oldRivalBloodIndex) {
                node->setPositionX(pos_total);
                node->setScaleX(scale_total);
            }

            //动作
            if (lv != whichColor) {
                if (direct) {
                    node->setPositionX(start_pos);
                    node->setScaleX(start_scale);
                } else {
                    node->runAction(Sequence::create(DelayTime::create(delay_time),  Spawn::create(MoveTo::create(blood_change_time_over_level, {start_pos, node->getPositionY(), node->getPositionZ()}),ScaleTo::create(blood_change_time_over_level, start_scale, node->getScaleY(), node->getScaleZ()), NULL), NULL));
                }
            } else {
                if (direct) {
                    node->setPositionX(pos);
                    node->setScaleX(scale);
                } else {
                    node->runAction(Sequence::create(DelayTime::create(delay_time), Spawn::create(MoveTo::create(blood_change_time_over_level, {pos, node->getPositionY(), node->getPositionZ()}),ScaleTo::create(blood_change_time_over_level, scale, node->getScaleY(), node->getScaleZ()), NULL), NULL));
                }
            }
            delay_time += blood_change_time_over_level;
        }
    } else if (whichColor > _oldRivalBloodIndex) {
        //级别提高
        float delay_time = 0;
        for (int lv = _oldRivalBloodIndex; lv <= whichColor; lv++) {

            auto node = _pxRivalBloods[lv];
            //设初始
            if (lv != _oldRivalBloodIndex) {
                node->setPositionX(start_pos);
                node->setScaleX(start_scale);
            }

            //动作
            if (lv != whichColor) {
                if (direct) {
                    node->setPositionX(pos_total);
                    node->setScaleX(scale_total);
                } else {
                    node->runAction(Sequence::create(DelayTime::create(delay_time), Spawn::create(MoveTo::create(blood_change_time_over_level, {pos_total, node->getPositionY(), node->getPositionZ()}),ScaleTo::create(blood_change_time_over_level, scale_total, node->getScaleY(), node->getScaleZ()), NULL), NULL));
                }
            } else {
                if (direct) {
                    node->setPositionX(pos);
                    node->setScaleX(scale);
                } else {
                    node->runAction(Sequence::create(DelayTime::create(delay_time), Spawn::create(MoveTo::create(blood_change_time_over_level, {pos, node->getPositionY(), node->getPositionZ()}),ScaleTo::create(blood_change_time_over_level, scale, node->getScaleY(), node->getScaleZ()), NULL), NULL));
                }
            }
            delay_time += blood_change_time_over_level;
        }

    }


    _oldRivalBloodIndex = whichColor;

    _pxtRivalBloodNum->configText(fmt::sprintf("%d\/%d", blood, bloodMax), 1);

}

void BloodBar::op_rival_reset()//恢复 rival 的信息到?状态
{
    //头像
    _rivalHeadIndex = 99;
    _pxRivalHead->configSopx(fmt::sprintf("sopx/bloodbar/rival_head/%d.png.sopx", _rivalHeadIndex));


    //血条
    op_rival_configBlood(1,0,1,true);

    //attack 等数值
    op_rival_configAttack(-1, -1);

}



void BloodBar::op_rival_configAttack(int attack_min, int attack_max, bool direct)
{
    if (attack_max == -1) {
        _pxtRivalAttack->configText("?? - ??", 1);
    } else {
        _pxtRivalAttack->configText(fmt::sprintf("%02d - %02d",attack_min, attack_max), 1);
    }
}

void BloodBar::op_show()
{
    _rivalHubNode->setVisible(true);
    _heroHubNode->setVisible(true);
}

void BloodBar::op_dismiss()
{
    _heroHubNode->setVisible(false);
    _rivalHubNode->setVisible(false);
}

void BloodBar::op_configCombo(float radio) //设置 combo 进度
{
    _pxComboContent->runAction(Spawn::create(ScaleTo::create(0.2, radio*55, 1, 1), MoveTo::create(0.2, {-28+radio*55/2, _pxComboContent->getPositionY(), _pxComboContent->getPositionZ() }), NULL));
}
void BloodBar::op_toastComboFull() //满了后闪亮一下，出来一个 combo 攻击
{
    _pxComboRect->configMixColorAni({1,1,1,1}, 0.2);
}
void BloodBar::op_toastComboReset() //断连后红一下，表示事务了
{
    _pxComboRect->configMixColorAni({0.7,0,0,1}, 0.2);

}