//
//  HuntingHero.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/24.
//
//

#include "HuntingHero.hpp"

#include "format.h"
#include "intersection.h"
#include "HuntingHeroMetaSuitManage.hpp"

USING_NS_CC;
void HuntingHero::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    initHubThings();
    initHeroThings();
}

void HuntingHero::initHubThings()
{
    _hubNode = Node::create();
    _hubNode->setPosition3D({0,0,-10});
    _hubNode->setScale(0.15);
    _hubNode->setCameraMask(_mainCamera->getCameraMask());
    _hubNode->setZOrder(9);
    _mainCamera->addChild(_hubNode);
    //    _hubNode->setVisible(false);
}

void HuntingHero::initHeroThings()
{
    _dpxNode = DynamicPixelNode::create();
    _dpxNode->setPosition3D({0,0,0});
    _hubNode->addChild(_dpxNode);
    _dpxNode->setCameraMask(_mainCamera->getCameraMask(), true);
}

int HuntingHero::boneIndexType2sopxId(int boneIndexType)
{
    int r = 0;
    switch (boneIndexType) {
        case BT_HEAD:
            r = 2;
            break;

        case BT_HAIR:
            r = 0;
            break;

        case BT_BODY:
            r = 5;
            break;

        case BT_HAND_L:
            r = 4;
            break;

        case BT_HAND_R:
            r = 6;
            break;

        case BT_LEG_L:
            r = 8;
            break;

        case BT_LEG_R:
            r = 8;
            break;

        default:
            break;
    }
    return r;
}

void HuntingHero::op_configHeroTypeAndDegree(int heroType, int grade) // 由英雄类型和等级 来指定穿着套装和弓
{
    //会清空 VBO 后，重新写数据
    _dpxNode->configClear();
    assert( grade >= 0 && grade <3);

    int suitId = 0;
    char bowType = 'm';
    if (heroType == HT_META) {
        suitId = HuntingHeroMetaSuitManage::s()->borrow();
        bowType = 'm';
    } else {
        switch (heroType) {
            case HT_SLOW_DOWN:
                suitId = 4;
                bowType = 'b';
                break;

            case HT_HIGH_ATTACK:
                suitId = 5;
                bowType = 'e';
                break;

            case HT_MULTI_ATTACK:
                suitId = 6;
                bowType = 'a';
                break;

            case HT_BOMB_ATTACK:
                suitId = 7;
                bowType = 'f';
                break;
                
            default:
                assert(false);
                break;
        }
    }

    // 穿衣服
    for (int i = 0; i < BT_BOW_MAX; i++) {
        _dpxNode->configAddSopx(fmt::sprintf("hunters/heros/%d.png.%d.png.sopx", suitId, boneIndexType2sopxId(i)), i, {0,0,0});


        _dpxNode->configAction(i, {0,0,0}, RepeatForever::create(RotateBy::create(1, {0,0,90})));
    }


}
void HuntingHero::op_configRelativeAngle(float angle) //逆时针变大，设置单个英雄的相对设计角度
{

}
void HuntingHero::op_startAiming() //开始瞄准，会举起弓到水平位置
{

}
void HuntingHero::op_stopAiming() //取消瞄准，会放下弓
{

}
void HuntingHero::op_toastShoot() //瞄准完毕后的发射动作，发射后会放下弓
{

}
void HuntingHero::op_configAiming(float angle, float strenth)
{

}
void HuntingHero::op_toastUnderAttack() //播放被攻击动画
{

}
void HuntingHero::op_toastDead() //播放死亡
{

}
void HuntingHero::op_toastShield(float interval) //播放一段时间护盾
{

}
void HuntingHero::op_toastBoost(float interval) //播放一段时间的暴击增益
{

}
void HuntingHero::op_toastUpgrade() //播放升级动画
{

}


void HuntingHero::op_show()
{
    _hubNode->setVisible(true);
}

void HuntingHero::op_hide()
{
    _hubNode->setVisible(false);
}

float HuntingHero::heroPositionType2floatYposition(int positionType)
{
    const float start_pos = 0;
    const float pos_step = 20;
    const float out_pos_n = 5;
    float r;
    switch (positionType) {
        case HPT_3:
            r = start_pos;
            break;
        case HPT_2:
            r = start_pos + pos_step;
            break;
        case HPT_1:
            r = start_pos + 2*pos_step;
            break;
        case HPT_0:
            r = start_pos + 3*pos_step;
            break;
        case HPT_OUT:
            r = start_pos - out_pos_n*pos_step;
            break;

        default:
            assert(false);
            break;
    }
    return r;

}

void HuntingHero::op_move(int desPositionType, bool direct)
{
    float ypos = heroPositionType2floatYposition(desPositionType);
    const float move_speed = 10;
    if (direct) {
        _hubNode->setPosition3D({0, ypos, 0});
    } else {
        _hubNode->stopAllActions();
        _hubNode->runAction(MoveTo::create(std::abs(_hubNode->getPositionY() - ypos) / move_speed, Vec3{0, ypos, 0}));
    }
}
