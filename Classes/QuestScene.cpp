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

/*
    addCommonBtn({0.35,0.95}, "battle cam", [this](){
        _cameraManage.op_switchToPosition(QuestCameraPosition::BATTLE);
    });
    addCommonBtn({0.2,0.95}, "forward cam", [this](){
        _cameraManage.op_switchToPosition(QuestCameraPosition::FORWARD);
    });

    addCommonBtn({0.9,0.9}, "roles newbatt", [this](){
        _battleRoles.op_newBattleConfig();
    });
    addCommonBtn({0.75,0.9}, "roles battleIn", [this](){
        _battleRoles.op_battleIn();
    });

    addCommonBtn({0.6,0.9}, "battle begin", [this](){
        battleBegin();
    });
*/
    addCommonBtn({0.9,0.2}, "power test", [this](){
        _powerBar.op_configPower(random(0.f, 1.f), random(0, 180));
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
    addCommonBtn({0.9,0.7}, "enegy test", [this](){
        _energyBar.op_addEnergy(0.1);
    });
    addCommonBtn({0.9,0.6}, "gold number", [this](){
        _topIcons.op_configGold(random(0, 99999));
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


    // config
    _cameraManage.configProtocals(&_battleRoad, &_battleRoles, &_bloodBar);
    _beatField.configProtocals(&_battleRoles, &_battleRoad);
    BattleState::s()->configProtocals(&_bloodBar, &_beatField,
                                      &_battleRoles, &_battleRoad, &_cameraManage, _mainCamera, this);

    MoneyManager::s()->configProtocals(&_topIcons);
    _powerBar.configProtocals(&_huntingHerosManage);

    // testing
    _huntingMonster.init(_mainLayer, _mainCamera);
    _huntingMonster.op_configType(HuntingMonsterGeneralType::NORMAL, HuntingMonsterSpecialType::NONE, true, 0);

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
}