// (C) 2015 Arisecbf


#include "QuestScene.h"
#include "format.h"
#include "PixelDataCache.hpp"
#include "HeroGraphicUnitAnimationDef.hpp"
#include "RivalGraphicAnimationDef.hpp"
#include "StoryInfo.hpp"
#include "BattleState.hpp"
#include "MoneyManager.hpp"
USING_NS_CC;


bool QuestScene::init()
{
    assert(TRBaseScene::init());

    // preload  TODO 移到load界面中

    StoryInfo::s()->loadConfig();
    HeroGraphicAnimationDef::loadConfig();
    RivalGraphicAnimationDef::loadConfig();
    PixelDataCache::s()->preload("beats/a.png.sopx");
    PixelDataCache::s()->preload("beats/b.png.sopx");
    PixelDataCache::s()->preloadChars();
    addCommonBtn({0.9,0.5}, "reset", [](){Director::getInstance()->replaceScene(QuestScene::create());});


//    addCommonBtn({0.1,0.1}, "monster dead", [this](){
//        _huntingMonster.op_toastDead({1,-1});
//    });
//    addCommonBtn({0.1,0.2}, "monster underat", [this](){
//        _huntingMonster.op_toastUnderAttack();
//    });
//
//    addCommonBtn({0.1,0.3}, "monster attack", [this](){
//        _huntingMonster.op_toastAttack();
//    });

    addCommonBtn({0.1,0.9}, "bomb base", [this](){
        _globalArrowEffectManage.op_bomb(0, {random(-100.f, 100.f), 20.f});
    });
    addCommonBtn({0.1,0.8}, "thunder", [this](){
        _globalArrowEffectManage.op_thunder({random(-100.f, 100.f),0});
    });
    addCommonBtn({0.1,0.7}, "bomb s", [this](){
        _globalArrowEffectManage.op_bomb(2, {random(-100.f, 100.f), 20.f});
    });
//    static int iid = 0;
//
//    addCommonBtn({0.1,0.4}, "next monster", [this](){
//        _huntingMonster.op_configType(HuntingMonsterGeneralType::NORMAL, HuntingMonsterSpecialType::NONE, true, iid++);
//    });
//
//    addCommonBtn({0.1,0.5}, "next monster", [this](){
//        _huntingMonster.op_configType(HuntingMonsterGeneralType::NORMAL, HuntingMonsterSpecialType::NONE, true, iid--);
//    });
  


    static float wind = 0;
    addCommonBtn({0.9,0.1}, "road wind", [this](){
        wind += 1;
        if (wind > 5) {
            wind = -5;
        }
        CCLOG("wind = %f", wind);
        _battleRoad.op_configWind(wind);
        _windBar.op_configWind(wind);
    });

    addCommonBtn({0.9,0.8}, "road shadow", [this](){
        _battleRoad.op_toastDarkShadow(1.0, 2);
    });
    addCommonBtn({0.9,0.7}, "enegy add", [this](){
        _energyBar.op_addEnergy(0.1);
    });
    addCommonBtn({0.9,0.6}, "gold add", [this](){
        MoneyManager::s()->add(100);
    });
    addCommonBtn({0.9,0.6}, "gold add", [this](){
        MoneyManager::s()->add(100);
    });

    addCommonBtn({0.9,0.4}, "20 wave", [this](){
        _huntingMonsterManage.op_testSetWave(20);
    });

    static float xpos = -100;
    addCommonBtn({0.9,0.3}, "laser toast", [this](){
        _floatingLaserManage.op_toastLaser({xpos, 0});
        xpos += 10;
    });


    auto layer = Layer::create();
    this->addChild(layer);

    auto size = Director::getInstance()->getVisibleSize();

    auto camera = Camera::createPerspective(60, size.width/size.height, 1, 1000000);
    camera->setCameraFlag(CameraFlag::USER1);
    layer->addChild(camera);
    camera->setCameraMask((unsigned short)CameraFlag::USER1);

    _mainLayer = layer;
    _mainCamera = camera;
    _mainCamera->setZOrder(10);

    // init
    _cameraManage.init(_mainCamera);
    _battleRoad.init(_mainLayer, _mainCamera);
    _battleRoles.init(_mainLayer, _mainCamera);
    _energyBar.init(_mainLayer, _mainCamera);
    _windBar.init(_mainLayer, _mainCamera);
    _bloodBar.init(_mainLayer, _mainCamera);
    _beatField.init(_mainLayer, _mainCamera);
    initPauseShadowThings();
    _topIcons.init(_mainLayer, _mainCamera);
    _powerBar.init(_mainLayer, _mainCamera);
    _huntingHerosManage.init(_mainLayer, _mainCamera);
    _huntingArrowManage.init(_mainLayer, _mainCamera);
    _huntingMonsterManage.init(_mainLayer, _mainCamera);
    _floatingLaserManage.init(_mainLayer, _mainCamera);
    _flyCrowManage.init(_mainLayer, _mainCamera);
    _globalArrowEffectManage.init(_mainLayer, _mainCamera);

    // config
    _cameraManage.configProtocals(&_battleRoad, &_battleRoles, &_bloodBar);
    _beatField.configProtocals(&_battleRoles, &_battleRoad);
    BattleState::s()->configProtocals(&_bloodBar, &_beatField,
                                      &_battleRoles, &_battleRoad, &_cameraManage, _mainCamera, this);

    MoneyManager::s()->configProtocals(&_topIcons);
    _powerBar.configProtocals(&_huntingHerosManage, &_windBar);
    _huntingArrowManage.configProtocals(&_windBar, &_huntingMonsterManage, &_flyCrowManage);
    _huntingHerosManage.configProtocals(&_huntingArrowManage);
    _huntingMonsterManage.configProtocal(&_energyBar, &_floatingLaserManage, &_globalArrowEffectManage, &_topIcons);
    _energyBar.configProtocals(&_floatingLaserManage);
    _topIcons.configProtocals(&_battleRoad);
    _flyCrowManage.configProtocals(&_topIcons);
    _globalArrowEffectManage.configProtocals(&_huntingMonsterManage);

    // init actions
    _cameraManage.op_switchToPosition(QuestCameraPosition::FORWARD, true);

    scheduleUpdate();
    return true;
}

void QuestScene::initPauseShadowThings()
{
    _pauseShadow = Sprite::create("images/pause_shadow.png");
    _pauseShadow->setPosition(genPos({0.5,0.5}));
    _pauseShadow->setScale(1000);
    _defaultLayer->addChild(_pauseShadow);
    _pauseShadow->setVisible(false);

    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [this](Touch* touch, Event* event){

        if (_waiting2startBattle && _pauseShadow->isVisible()) {
            _waiting2startBattle = false;
            _pauseShadow->setVisible(false);
            _beatField.op_startBeating();
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

void QuestScene::op_showPauseShadow()
{
    _pauseShadow->setVisible(true);
    _waiting2startBattle = true;
}

void QuestScene::battleBegin()
{
    BattleState::s()->initAsNextRival(); //初始化首个rival的数据
}

void QuestScene::update(float dt)
{
//    _battleRoad.update(dt);
//    _beatField.update(dt);
    _topIcons.update(dt);
    _energyBar.update(dt);
    _windBar.update(dt);
    _huntingArrowManage.update(dt);
    _huntingMonsterManage.update(dt);
    _flyCrowManage.update(dt);
}