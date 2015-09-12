#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "GameMenuScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLView::create("My Game");
        // 设置游戏名称，就是窗口左上方显示的 title
        glview = GLView::create("飞机大战");
        // 设置窗口大小
        glview->setFrameSize(480, 800);
        director->setOpenGLView(glview);
    }
    // 为了适配手机端
    glview->setDesignResolutionSize(480, 800, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    auto scene = GameMenuScene::createScene();

    // run
    director->runWithScene(scene);
    
    // 初始化音乐
    preloadMusic();

    return true;
}

/* 初始化音乐 */
void AppDelegate::preloadMusic()
{
    // 预读背景音乐
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/game_music.wav");
    
    // 预读各种音效
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/game_over.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/bullet.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/use_bomb.wav");
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
