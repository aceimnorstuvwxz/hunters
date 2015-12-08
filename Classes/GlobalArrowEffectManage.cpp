//
//  GlobalArrowEffectManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/7.
//
//

#include "GlobalArrowEffectManage.hpp"
#include "ACSoundManage.hpp"

USING_NS_CC;


void GlobalArrowEffectManage::init(cocos2d::Layer *mainLayer, cocos2d::Camera *mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;
    initEffectsThings();
}

void GlobalArrowEffectManage::initEffectsThings()
{
    {
        // thunder
        auto dx = DynamicPixelNode::create();
        dx->setPosition3D({0,0,0});
        _mainLayer->addChild(dx);
        dx->setPosition3D({5,0,0});
        dx->setScale(0.8, 1.3);
        dx->setZOrder(9);
        dx->setRotation3D({90,0,-90});
        dx->setCameraMask(_mainCamera->getCameraMask(), true);
        dx->setVisible(false);
        for (int i = 0; i < 2; i++) {
            dx->configAddSopx(fmt::sprintf("hunters/effects/thunder_%d.png.sopx", i), i, {0,0,0}, true, true);
        }
        dx->configBlend(true);

        _dxThunder = dx;

    }

    {
        // base bomb
        auto dx = DynamicPixelNode::create();
        dx->setPosition3D({0,0,0});
        _mainLayer->addChild(dx);
        dx->setPosition3D({5,0,0});
        dx->setScale(1.f, 1.f);
        dx->setZOrder(9);
        dx->setRotation3D({90,0,-90});
        dx->setCameraMask(_mainCamera->getCameraMask(), true);
        dx->setVisible(false);
        for (int i = 0; i < 3; i++) {
            dx->configAddSopx(fmt::sprintf("hunters/effects/new_base_bomb_%d.png.sopx", i), i, {0,0,0}, true, true);
        }
        dx->configBlend(true);

        _dxBaseBomb = dx;

    }

    {
        // super bomb
        auto dx = DynamicPixelNode::create();
        dx->setPosition3D({0,0,0});
        _mainLayer->addChild(dx);
        dx->setPosition3D({5,0,0});
        dx->setScale(1.f, 1.f);
        dx->setZOrder(9);
        dx->setRotation3D({90,0,-90});
        dx->setCameraMask(_mainCamera->getCameraMask(), true);
        dx->setVisible(false);
        for (int i = 0; i < 4; i++) {
            dx->configAddSopx(fmt::sprintf("hunters/effects/super_bomb_%d.png.sopx", i), i, {0,0,0}, true, true);
        }
        dx->configBlend(true);
        _dxSuperBomb = dx;

    }
}

void GlobalArrowEffectManage::op_bomb(int grade, cocos2d::Vec2 pos)
{
    if (grade == 0) {
        _dxBaseBomb->setPositionY(pos.x);
        _dxBaseBomb->setPositionZ(pos.y);

        _dxBaseBomb->setVisible(true);
        const float ani_step = 0.1;
        const int frame_num = 4;
        _dxBaseBomb->runAction(Sequence::create(DelayTime::create(ani_step*frame_num), Hide::create(), NULL));

        for (int i = 0; i < frame_num; i++) {
            _dxBaseBomb->configAction(i, {0,0,-1000}, {0,0,0}, 1, 1, Sequence::create(DelayTime::create(ani_step*i), MoveBy::create(0, Vec3{0,0,1000}), DelayTime::create(ani_step), MoveBy::create(0, Vec3{0,0,-1000}), DelayTime::create(ani_step*(frame_num-i)), NULL));
        }
        ACSoundManage::s()->play(ACSoundManage::SN_THUNDER_HIT);
    } else {
        float scale  = grade == 1 ? 1.f :1.5f;
        _dxSuperBomb->setPositionY(pos.x);
        _dxSuperBomb->setPositionZ(pos.y);

        _dxSuperBomb->setVisible(true);
        _dxSuperBomb->setScale(scale);
        const float ani_step = 0.1;
        const int frame_num = 4;
        _dxSuperBomb->runAction(Sequence::create(DelayTime::create(ani_step*frame_num), Hide::create(), NULL));

        for (int i = 0; i < frame_num; i++) {
            _dxSuperBomb->configAction(i, {0,0,-1000}, {0,0,0}, 1, 1, Sequence::create(DelayTime::create(ani_step*i), MoveBy::create(0, Vec3{0,0,1000}), DelayTime::create(ani_step), MoveBy::create(0, Vec3{0,0,-1000}), DelayTime::create(ani_step*(frame_num-i)), NULL));
        }
        ACSoundManage::s()->play(ACSoundManage::SN_THUNDER_HIT);
    }
    _monsterManageProtocal->op_bomb(pos.x, grade);


}

void GlobalArrowEffectManage::op_thunder(cocos2d::Vec2 pos)
{
    _dxThunder->setPositionY(pos.x);
    _dxThunder->setVisible(true);
    const float ani_step = 0.15;
    _dxThunder->runAction(Sequence::create(DelayTime::create(ani_step*2), Hide::create(), NULL));

    for (int i = 0; i < 2; i++) {
        _dxThunder->configAction(i, {0,0,-1000}, {0,0,0}, 1, 1, Sequence::create(DelayTime::create(ani_step*i), MoveBy::create(0, Vec3{0,0,1000}), DelayTime::create(ani_step), MoveBy::create(0, Vec3{0,0,-1000}), DelayTime::create(ani_step*(2-i)), NULL));
    }
    ACSoundManage::s()->play(ACSoundManage::SN_THUNDER_HIT);
    _monsterManageProtocal->op_thunder(pos.x);
}

void GlobalArrowEffectManage::update(float dt)
{

}