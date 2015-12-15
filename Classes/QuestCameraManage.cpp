//
//  QuestCameraManage.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#include "QuestCameraManage.hpp"
#include "QuestDef.hpp"

USING_NS_CC;
void QuestCameraManage::init(cocos2d::Camera* mainCamera)
{
    _mainCamera = mainCamera;

}



void QuestCameraManage::op_startRunningShake()
{
    _mainCamera->runAction(RepeatForever::create(Sequence::create(MoveBy::create(QuestDef::HERO_RUN_UNIT_TIME*2.5, {0,0,QuestDef::HERO_RUN_HEIGHT*0.5f}), MoveBy::create(QuestDef::HERO_RUN_UNIT_TIME*5, {0,0,-QuestDef::HERO_RUN_HEIGHT}),MoveBy::create(QuestDef::HERO_RUN_UNIT_TIME*2.5, {0,0,QuestDef::HERO_RUN_HEIGHT*0.5f}),NULL)));
}

void QuestCameraManage::op_stopRunningShake()
{
    _mainCamera->stopAllActions();
    op_switchToPosition(QuestCameraPosition::BATTLE, true);
}

void QuestCameraManage::op_switchToPosition(QuestCameraPosition p, bool direct) //切换镜头的不同位置, direct表示是否直接设置而非动画
{
    Vec3 targetPosition;
    Vec3 targetRotation;

    switch (p) {
        case QuestCameraPosition::BATTLE:
            targetPosition = {0,-QuestDef::POS_Y_ROLE-8,45};
            targetRotation = {CC_RADIANS_TO_DEGREES(0.9607),0,0};
            break;
        case QuestCameraPosition::FORWARD:
            targetPosition = {QuestDef::CAMERA_FORWARD_EXPAND,0,QuestDef::CAMERA_FORWARD_EXPAND*0.25f};
            targetRotation = {90,0,-90};
            break;

        default:
            assert(false);
            break;
    }

    const float camera_move_time = QuestDef::CAMERA_MOVE_TIME;
    const float time_at_show_right_trees = 0.5f; //指从battle到forward的分布

    if (!direct) {
        _mainCamera->runAction(Spawn::create(MoveTo::create(camera_move_time, targetPosition),
                                             RotateTo::create(camera_move_time, targetRotation), NULL));
        _mainCamera->scheduleOnce([this,p](float dt){
            // 不只是右侧的树，hero的visibility也是要设置的，但是和tree相反, 还有 bloodbar 的 visibility
            if (p == QuestCameraPosition::BATTLE) {
            } else {
            }
        } , (p == QuestCameraPosition::BATTLE ? (1.f-time_at_show_right_trees) : time_at_show_right_trees)*camera_move_time, "right tree hide/show");

        _mainCamera->scheduleOnce([this,p](float dt){
        }, p != QuestCameraPosition::BATTLE ? 0 : camera_move_time, "left hills hide/show");
    } else {
        _mainCamera->setPosition3D(targetPosition);
        _mainCamera->setRotation3D(targetRotation);
    }
}