#include "AppDelegate.h"
#include "TestGuideScene.h"
#include "ProgressInfo.hpp"
#include "StoryInfo.hpp"
#include "ACSoundManage.hpp"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(true);

    
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    ProgressInfo::s()->load(); // 读出进度信息
    StoryInfo::s()->loadConfig(); // 加载关卡配置数据

    ACSoundManage::s()->load(); // preload sounds

    Director::getInstance()->setClearColor(Color4F::WHITE);

    director->runWithScene(TestGuideScene::create());

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    ProgressInfo::s()->save(); // 保存进度信息
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

}
