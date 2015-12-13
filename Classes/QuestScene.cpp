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

    /*
    addCommonBtn({0.9,0.5}, "reset", [](){Director::getInstance()->replaceScene(QuestScene::create());});



    addCommonBtn({0.1,0.7}, "pause", [this](){
        op_configPaused(!_paused);
    });


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
        MoneyManager::s()->add(10000);
    });

    addCommonBtn({0.9,0.4}, "20 wave", [this](){
        _huntingMonsterManage.op_testSetWave(20);
    });

    addCommonBtn({0.9,0.3}, "tornado", [this](){
        _tornadoManage.op_configTornado(true, random(-100, 0), true, random(0, 100));
    });

    addCommonBtn({0.9,0.05}, "tuto", [this](){
        _tutorialManage.op_toastStory();
    });

*/

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
    _particleManage.init(_mainLayer, _mainCamera);
    _tornadoManage.init(_mainLayer, _mainCamera);
    _tutorialManage.init(_mainLayer, _mainCamera);

    // config
    _cameraManage.configProtocals(&_battleRoad, &_battleRoles, &_bloodBar);
    _beatField.configProtocals(&_battleRoles, &_battleRoad);
    BattleState::s()->configProtocals(&_bloodBar, &_beatField,
                                      &_battleRoles, &_battleRoad, &_cameraManage, _mainCamera, this);

    MoneyManager::s()->configProtocals(&_topIcons, &_huntingHerosManage);
    _powerBar.configProtocals(&_huntingHerosManage, &_windBar, &_huntingArrowManage);
    _huntingArrowManage.configProtocals(&_windBar, &_huntingMonsterManage, &_flyCrowManage, &_particleManage, &_huntingHerosManage, &_tornadoManage);
    _huntingHerosManage.configProtocals(&_huntingArrowManage, &_powerBar, this, &_tutorialManage);
    _huntingMonsterManage.configProtocal(&_energyBar, &_floatingLaserManage, &_globalArrowEffectManage, &_topIcons, &_particleManage, &_tornadoManage, &_windBar);
    _energyBar.configProtocals(&_floatingLaserManage, &_tutorialManage);
    _topIcons.configProtocals(&_battleRoad);
    _flyCrowManage.configProtocals(&_topIcons, &_particleManage);
    _globalArrowEffectManage.configProtocals(&_huntingMonsterManage);
    _windBar.configProtocals(&_battleRoad);


    // init actions
    _cameraManage.op_switchToPosition(QuestCameraPosition::FORWARD, true);


    // test


    scheduleUpdate();
    _tutorialManage.op_toastStory();
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

void QuestScene::op_configPaused(bool pause)
{
    _paused = pause;
    _flyCrowManage.op_configPaused(pause);
    _huntingMonsterManage.op_configPaused(pause);
    _huntingHerosManage.op_configPaused(pause);
    _particleManage.op_configPaused(pause);
}

void QuestScene::update(float dt)
{
    _topIcons.update(dt);
    _energyBar.update(dt);
    _windBar.update(dt);
    if (!_paused) {
        _huntingArrowManage.update(dt);
        _huntingMonsterManage.update(dt);
        _flyCrowManage.update(dt);
        _huntingHerosManage.update(dt);
        _tornadoManage.update(dt);
    }
}