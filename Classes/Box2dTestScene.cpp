//
//  Box2dTestScene.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/1.
//
//

#include "Box2dTestScene.hpp"
#include "../cocos2d/external/Box2D/Box2D.h"
#include "cocos2d.h"


USING_NS_CC;

bool Box2dTestScene::init()
{
    assert(TRBaseScene::init());

    addCommonBtn({0.1,0.1}, "test", [this](){
        this->testAction();
    });


    initTestThings();

    return true;
}


void Box2dTestScene::initTestThings()
{
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

}


void Box2dTestScene::testAction()
{

}
