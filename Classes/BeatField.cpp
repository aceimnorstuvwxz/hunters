//
//  BeatField.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/7.
//
//

#include "BeatField.hpp"
#include "QuestDef.hpp"
#include "ShieldBeat.hpp"
#include "HealthBeat.hpp"
#include "LongBeat.hpp"
#include "CopyBeat.hpp"
#include "LaserBeat.hpp"
#include "BattleState.hpp"
USING_NS_CC;

void BeatField::init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera)
{
    _mainLayer = mainLayer;
    _mainCamera = mainCamera;

    _pxTapArea = PixelNode::create();
    _pxTapArea->setCameraMask(_mainCamera->getCameraMask());
    _pxTapArea->setPosition3D({0,QuestDef::BEAT_HERO_TAP_LINE, QuestDef::BEAT_TAP_AREA_HEIGHT});
    _pxTapArea->configSopx("sopx/taparea/full.png.sopx");
    _pxTapArea->configBlend(true);
    _mainLayer->addChild(_pxTapArea);


    for (int i = 0; i < 5; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({QuestDef::BEAT_LINE_WIDTH * (i-2),QuestDef::BEAT_HERO_TAP_LINE,QuestDef::BEAT_TAP_AREA_HEIGHT+0.1});
        node->configSopx("sopx/taparea/ok.png.sopx");
        node->configBlend(true);
        _mainLayer->addChild(node);
        _pxTapOk[i] = node;
        node->setVisible(false);
    }

    for (int i = 0; i < 5; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({QuestDef::BEAT_LINE_WIDTH * (i-2),QuestDef::BEAT_HERO_TAP_LINE,QuestDef::BEAT_TAP_AREA_HEIGHT+0.1});
        node->configSopx("sopx/taparea/fail.png.sopx");
        node->configBlend(true);
        _mainLayer->addChild(node);
        _pxTapFail[i] = node;
        node->setVisible(false);
    }

    for (int i = 0; i < 5; i++) {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({QuestDef::BEAT_LINE_WIDTH * (i-2),QuestDef::BEAT_HERO_TAP_LINE,QuestDef::BEAT_TAP_AREA_HEIGHT+0.1});
        node->configSopx("sopx/taparea/laser.png.sopx");
        node->configBlend(true);
        _mainLayer->addChild(node);
        _pxLaser[i] = node;
        node->setVisible(false);
    }


    initEffectsThings();
    initComboThings();
    initTouchThings();
    initUpgradeThings();


    op_dismiss();
}

void BeatField::configProtocals(BattleRolesProtocal* battleRolesProtocal, BattleRoadProtocal* battleRoadProtocal)
{
    _battleRolesProtocal = battleRolesProtocal;
    _battleRoadProtocal = battleRoadProtocal;
}

void BeatField::op_show() //出现
{
    _pxTapArea->setVisible(true);
}

void BeatField::op_dismiss() //消失
{
    _pxTapArea->setVisible(false);
}

void BeatField::op_startBeating() //开始出现beats 战斗开始
{
    _beatingPaused = false;
}

void BeatField::addSingleBeat()
{
    auto sp = SingleBeat::create();
    sp->init(this, _mainLayer, _mainCamera, static_cast<BeatFieldLineType>(random(-2, 2)), -80);
    _allBeats.push_back(sp);
}

void BeatField::addShieldBeat()
{
    auto sp = ShieldBeat::create();
    sp->init(this, _mainLayer, _mainCamera, static_cast<BeatFieldLineType>(random(-2, 2)), -80);
    _allBeats.push_back(sp);
}

void BeatField::addHealthBeat()
{
    auto sp = HealthBeat::create();
    sp->init(this, _mainLayer, _mainCamera, static_cast<BeatFieldLineType>(random(-2, 2)), -80);
    _allBeats.push_back(sp);
}
void BeatField::addLongBeat()
{
    auto sp = LongBeat::create();
    sp->init(this, _mainLayer, _mainCamera, static_cast<BeatFieldLineType>(random(-2, 2)), -80, 5);
    _allBeats.push_back(sp);
}

void BeatField::addLaserBeat()
{
    auto sp = LaserBeat::create();
    sp->init(this, _mainLayer, _mainCamera, static_cast<BeatFieldLineType>(random(-2, 2)), -80);
    _allBeats.push_back(sp);
}

void BeatField::addCopyBeat()
{
    //mother
    auto sp = CopyBeat::create();
    sp->init(this, _mainLayer, _mainCamera, static_cast<BeatFieldLineType>(random(-2, 2)), -80, true);
    _allBeats.push_back(sp);


    //son
    sp = CopyBeat::create();
    sp->init(this, _mainLayer, _mainCamera, static_cast<BeatFieldLineType>(random(-2, 2)), -80, false);
    _allBeats.push_back(sp);
}

void BeatField::putUpgradeBeats()
{
    CCLOG("put upgrade beats");
}
void BeatField::update(float dt)
{
    if (_beatingPaused) {
        return;
    }


    for (auto line :_lines2beclear){
        for (auto iter = _allBeats.begin(); iter != _allBeats.end(); ) {
            if ((*iter)->getLineType() == line) {
                // TODO应该只清除 tap Line 之前的
                (*iter)->clear();
                iter = _allBeats.erase(iter);
            } else {
                iter++;
            }
        }
    }
    _lines2beclear.clear();


    for (auto iter = _allBeats.begin(); iter != _allBeats.end(); ) {
        bool willDelete = (*iter)->op_update(dt);
        if (willDelete) {
            iter = _allBeats.erase(iter);
        } else {
            iter++;
        }
    }

    _time += dt;
    if (_time > _interval) {
        _time = 0;
        _interval =  std::max(0.25, _interval -0.002);

        int count = 1;
        if (rand_0_1() < 0.2) {
            count = 2;
        }
        for (int i = 0; i < count; i++) {
            float r = rand_0_1();
            if (r < 0.03) {
                this->addShieldBeat();
            } else if (r < 0.03 + 0.06) {
                this->addHealthBeat();
            } else if (r < 0.03 + 0.06 + 0.06) {
                this->addLongBeat();
            } else if (r < 0.03 + 0.06 + 0.06 + 0.06) {
                this->addCopyBeat();
            } else if (r < 0.03 + 0.06 + 0.06 + 0.06 + 0.06) {
                this->addLaserBeat();
            } else {
                this->addSingleBeat();
            }
        }
    }
}

void BeatField::initTouchThings()
{

    static bool touch_moved = false;
    auto listener = EventListenerTouchAllAtOnce::create();

    listener->onTouchesBegan = [this](const std::vector<Touch*>& touches, Event* event){

        auto size = Director::getInstance()->getWinSize();
        for (auto touch : touches) {
            Vec2 p = touch->getLocation();
            float yp = p.y / size.height;
            float xp = p.x / size.width;
            if (/*yp >= QuestDef::SCREED_RELATIVE_TAP_BOTTOM && */yp <= QuestDef::SCREEN_RELATIVE_TAP_TOP) {
                int which = xp / 0.2;
                BeatFieldLineType lt = static_cast<BeatFieldLineType>(which-2);
                this->tellAllBeatTapIn(lt);
//                CCLOG("tap begin @%d", which-2);
            } else {
                // 尝试发 combo
                BattleState::s()->comboTry2Toast();
            }
            // try hit at touch began
        }

    };

    listener->onTouchesMoved = [this](const std::vector<Touch*>& touches, Event* event){
        touch_moved = true;
    };

    listener->onTouchesEnded = [this](const std::vector<Touch*>& touches, Event* event){
        auto size = Director::getInstance()->getWinSize();
        for (auto touch : touches) {
            Vec2 p = touch->getLocation();
            float yp = p.y / size.height;
            float xp = p.x / size.width;
            if (yp >= QuestDef::SCREED_RELATIVE_TAP_BOTTOM && yp <= QuestDef::SCREEN_RELATIVE_TAP_TOP) {
                int which = xp / 0.2;
                BeatFieldLineType lt = static_cast<BeatFieldLineType>(which-2);
                this->tellAllBeatTapOut(lt);
//                CCLOG("tap end @%d", which-2);
            }
            // try hit at touch began
        }
    };

    listener->onTouchesCancelled = [this](const std::vector<Touch*>& touches, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);

}


void BeatField::tellAllBeatTapIn(BeatFieldLineType line)
{


    bool hasTaped = false;
    for (auto iter = _allBeats.begin(); iter != _allBeats.end(); ) {
        auto tapResult = (*iter)->op_touchBegin(line);
        if (tapResult.showTapOk) {
            op_toastTap(line, true);
        }
        if (tapResult.showTapFail) {
            op_toastTap(line, false);
        }

        if (tapResult.willDelete) {
            iter = _allBeats.erase(iter);
        } else {
            iter++;
        }
        if (tapResult.tapOk) {
            BattleState::s()->comboAdd();
            hasTaped = true;
            break;
        }
    }
    if (!hasTaped) {
        // 空tap，TODO 造成伤害
        op_toastTap(line, false);
    }

}

void BeatField::tellAllBeatTapOut(BeatFieldLineType line)
{
//    for (auto iter = _allBeats.begin(); iter != _allBeats.end(); ) {
//        if ((*iter)->op_touchEnd(line)) {
//            iter = _allBeats.erase(iter);
//        } else {
//            iter++;
//        }
//    }
}

void BeatField::op_toastTap(BeatFieldLineType line, bool isok) //toast是短暂的，包括出现和消失
{
    int index = static_cast<int>(line)+2;
    PixelNode* node = isok ? _pxTapOk[index] : _pxTapFail[index];

    node->setVisible(true);
    node->setOpacity(0);
    node->runAction(Sequence::create(FadeIn::create(QuestDef::TAP_TOAST_TIME), FadeOut::create(QuestDef::TAP_TOAST_TIME), Hide::create(), NULL));

}


void BeatField::op_toastLaser(BeatFieldLineType line) //Laser beat 的效果
{
    int index = static_cast<int>(line)+2;
    PixelNode* node = _pxLaser[index];

    node->setVisible(true);
    node->setOpacity(128);
    node->runAction(Sequence::create(FadeIn::create(0.05), FadeOut::create(0.2), Hide::create(), NULL));

    _lines2beclear.push_back(line);
}


void BeatField::op_showComboCore() //显示可以发射 combo 的状态
{
    _pxComboCore->setVisible(true);
    _pxComboCore->setPosition3D({0,-QuestDef::POS_Y_ROLE+40, -0.1});
    _pxComboCore->configMixColorAni({1,1,1,1}, 0.2); // 闪一下
    // TODO 使用 pos_expand 效果 聚合
}
void BeatField::op_dismissComboCore() //取消可以发射 combo 的状态
{
    _pxComboCore->setVisible( false);
//    _pxComboCore->configMixColorAni({1,1,1,1}, 0.2); // 闪一下
    // TODO 使用 pos_expand 效果 散开
}
void BeatField::op_toastComboAttack(int damage) //进行一次 combo 攻击的显示，同时 推后场上的 beats
{
    _pxComboWing->setVisible(true);
    _pxComboWing->setScaleX(1.f/20);
    _pxComboWing->setPosition3D({0,-QuestDef::POS_Y_ROLE+40, -0.1});
    _pxComboWing->runAction(Spawn::create(
                                          ScaleTo::create(0.2, 1, 1, 1),//张开翅膀
                                          Sequence::create(MoveTo::create(1.0, {0,QuestDef::POS_Y_ROLE,0})
                                          , Hide::create(),NULL),NULL
                                          ));
    // TODO 飞到位置之后，用 mixcolorAni 把 飞机染红，然后 pos_expand 散开结束
    _pxComboCore->runAction(Sequence::create(MoveTo::create(1.0, {0,QuestDef::POS_Y_ROLE,0})
                                             , Hide::create(),NULL));

    _pxComboCore->scheduleOnce([this, damage](float dt) {
        BattleState::s()->rivalUnderComboAttack(damage);
    }, 1.0, "combo damage");
}

void BeatField::op_toastRivalDieEffects() //rival 死了后，所有 Beat 减速运动，后消失，然后释放升级 beat
{
    _battleRoadProtocal->op_toastDarkShadow(0.5, 2);
    _isDieingSlow = true; //设置 flag，该 flag 会导致 Update 的dt变小，从而减速
    _beatingPaused = true; //暂停常规的 Beat 的放置
    _mainCamera->scheduleOnce([this](float dt){
        //一段时间后clear场上的 beat
        for (auto b : _allBeats) {
            b->clear();
        }
        _allBeats.clear();
    }, 0.3, "rival die clear beats");

    _mainCamera->scheduleOnce([this](float dt){
        //然后恢复正常速度，出现升级选项
        _isDieingSlow = false;
        this->op_showUpgrade();

    }, 0.3+0.1, "rival die put upgrade beats");

}

void BeatField::initEffectsThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,QuestDef::BEAT_HERO_TAP_LINE, QuestDef::BEAT_TAP_AREA_HEIGHT});
        node->configSopx("sopx/taparea/hero_damage.png.sopx");
        node->configBlend(true);
        _mainLayer->addChild(node);
        node->setVisible(false);
        _pxHeroDamageHealth = node;
    }
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,QuestDef::BEAT_HERO_TAP_LINE, QuestDef::BEAT_TAP_AREA_HEIGHT+0.1});
        node->configSopx("sopx/taparea/hero_shield.png.sopx");
        node->configBlend(true);
        _mainLayer->addChild(node);
        node->setVisible(false);
        _pxHeroShield = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,QuestDef::POS_Y_ROLE-2, 0});
        node->setRotation3D({90,0,0});
        node->configSopx("sopx/taparea/rival_shield.png.sopx");
        node->configBlend(true);
        _mainLayer->addChild(node);
        node->setVisible(false);
        _pxRivalShield = node;
    }
}

void BeatField::initComboThings()
{
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,-QuestDef::POS_Y_ROLE+40, -0.1});
//        node->setRotation3D({90,0,0});
        node->configSopx("sopx/combo/combo_core.png.sopx");
        node->configBlend(true);
//        node->setScale(0.5);
        _mainLayer->addChild(node);
        node->setVisible(false);
        _pxComboCore = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,-QuestDef::POS_Y_ROLE+40, -0.1});
        //        node->setRotation3D({90,0,0});
        node->configSopx("sopx/combo/combo_wing.png.sopx");
        node->configBlend(true);
        //        node->setScale(0.5);
        _mainLayer->addChild(node);
                node->setVisible(false);
        _pxComboWing = node;
    }
}



void BeatField::op_toastHeroDamage() //被攻击
{
    _pxHeroDamageHealth->setOpacity(0);
    _pxHeroDamageHealth->stopAllActions();
    _pxHeroDamageHealth->configMixColor(_mixColorDamage);
    _pxHeroDamageHealth->runAction(Sequence::create(Show::create(), FadeIn::create(0.10), FadeOut::create(0.1), Hide::create(), NULL));
}
void BeatField::op_toastHeroHealth() //加血
{
    _pxHeroDamageHealth->setOpacity(0);
    _pxHeroDamageHealth->stopAllActions();
    _pxHeroDamageHealth->configMixColor(_mixColorHealth);
    _pxHeroDamageHealth->runAction(Sequence::create(Show::create(), FadeIn::create(0.10), FadeOut::create(0.1), Hide::create(), NULL));
}
void BeatField::op_showHeroShield() //举盾牌
{
    _pxHeroShield->setOpacity(0);
    _pxHeroShield->stopAllActions();
    // 用Pos extand TODO 配合音效
    _pxHeroShield->runAction(Sequence::create(Show::create(), FadeIn::create(0.1), NULL));

}
void BeatField::op_dismissHeroShield() //盾牌被击毁
{
    _pxHeroShield->stopAllActions();
    _pxHeroShield->runAction(Sequence::create(FadeOut::create(0.1), Hide::create(), NULL));
}

void BeatField::op_toastRivalDamage() //被攻击
{
    _battleRolesProtocal->op_rivalBeAttack();
}
void BeatField::op_toastRivalHealth() //加血
{
    _battleRolesProtocal->op_rivalBeHealth();
}
void BeatField::op_showRivalShield() //举盾牌
{
    _pxRivalShield->setOpacity(0);
    _pxRivalShield->stopAllActions();
    // 用Pos extand TODO 配合音效
    _pxRivalShield->runAction(Sequence::create(Show::create(), FadeIn::create(0.1), NULL));
}
void BeatField::op_dismissRivalShield() //盾牌被击毁
{
    _pxRivalShield->stopAllActions();
    _pxRivalShield->runAction(Sequence::create(FadeOut::create(0.1), Hide::create(), NULL));
}

void BeatField::initUpgradeThings()
{

    const float z = -10;
    const float c = 0.15;
    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,0, z});
        //        node->setRotation3D({90,0,0});
        node->configSopx("sopx/upgrade/title.png.sopx");
                node->setScale(c);
        _mainCamera->addChild(node);
        node->setVisible(false);
        _pxUpgradeTitle = node;
    }

    {
        auto node = PixelNode::create();
        node->setCameraMask(_mainCamera->getCameraMask());
        node->setPosition3D({0,0, z});
        //        node->setRotation3D({90,0,0});
        node->configSopx("sopx/upgrade/content.png.sopx");
                node->setScale(c);
        _mainCamera->addChild(node);
                node->setVisible(false);
        _pxUpgradeContent = node;
    }



    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){

        if (_waitUpgrading) {
            float bot = 183.f/750;
            float top = (750.f-256)/750;
            float xbeg = 400/1334.f;
            float xend = (1344.f-400)/1334.f;

            auto size = Director::getInstance()->getWinSize();
            float yNow = touch->getLocation().y/size.height;
            float xNow = touch->getLocation().x/size.width;
            if (xNow > xbeg && xNow < xend && yNow > bot && yNow < top) {

                int which = (yNow - bot)/(top-bot)*3;
                if (which < 1)  {
                    //health
                    BattleState::s()->upgradeBlood();
                } else if (which <2) {
                    //attack
                    BattleState::s()->upgradeAttack();
                } else {
                    //combo
                    BattleState::s()->upgradePower();
                }
                this->op_dismissUpgrade();
                BattleState::s()->finishAsPassRival();
            }

        }

        return false;
    };

    listener->onTouchMoved = [this](Touch* touch, Event* event){

    };

    listener->onTouchEnded = [this](Touch* touch, Event* event){

    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event){
    };

    _mainLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _mainLayer);
}

void BeatField::op_showUpgrade()
{
    _waitUpgrading = true;
    const float t = 0.5;
    const float y = 20;
    _pxUpgradeTitle->setPositionY(y);
    _pxUpgradeTitle->runAction(Sequence::create(Show::create(), MoveTo::create(t, {0.f,0.f,_pxUpgradeTitle->getPositionZ()}), NULL));


    _pxUpgradeContent->setPositionY(-y);
    _pxUpgradeContent->runAction(Sequence::create(Show::create(), MoveTo::create(t, {0.f,0.f, _pxUpgradeContent->getPositionZ()}), NULL));
}

void BeatField::op_dismissUpgrade()
{
    _waitUpgrading = false;
    const float t = 0.5;
    const float y = 20;
//    _pxUpgradeTitle->setPositionY(y);
    _pxUpgradeTitle->runAction(Sequence::create(MoveTo::create(t, {0.f,y,_pxUpgradeTitle->getPositionZ()}), Hide::create(), NULL));


//    _pxUpgradeContent->setPositionY(-y);
    _pxUpgradeContent->runAction(Sequence::create(
                                                  MoveTo::create(t, {0.f,-y, _pxUpgradeContent->getPositionZ()}), Hide::create(), NULL));
}

