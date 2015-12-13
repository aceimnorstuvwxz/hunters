// (C) 2015 Turnro.com

#include "TestGuideScene.h"
#include "TripleQuestScene.h"
#include "BeatQuestScene.h"
#include "ScatPixelScene.h"
#include "StealthTestScene.h"
#include "QuestScene.h"
#include "WelcoScene.h"


USING_NS_CC;

bool TestGuideScene::init()
{
    assert(TRBaseScene::init());



    addCommonBtn({0.5,0.9}, "Triple Quest", [](){
           Director::getInstance()->pushScene(TripleQuestScene::create());
    });


    addCommonBtn({0.5,0.8}, "Beat Quest", [](){
        Director::getInstance()->pushScene(BeatQuestScene::create());
    });


    addCommonBtn({0.5,0.2}, "Stealth", [](){
        Director::getInstance()->pushScene(StealthTestScene::create());
    });

    addCommonBtn({0.5,0.1}, "Scat Pixel 3D", [](){

        Director::getInstance()->pushScene(ScatPixelScene::create());
    });

    addCommonBtn({0.5,0.5}, "Quest", [](){

        Director::getInstance()->pushScene(QuestScene::create());
    });

    addCommonBtn({0.5,0.4}, "Welco", [](){

        Director::getInstance()->pushScene(WelcoScene::create());
    });

//    scheduleOnce([](float dt){
//        Director::getInstance()->pushScene(QuestScene::create());
//
//    }, 0.5, "sdgf");

    return true;
}