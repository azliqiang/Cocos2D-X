//
//  GameMenuScene.cpp
//  testgame4455
//
//  Created by stdio on 15/6/2.
//
//

#include "GameMenuScene.h"
#include "GamePlayScene.h"
#include "nijia.h"
USING_NS_CC;

Scene*GameMenuScene::createScene(){

    auto scene = Scene::create();
    auto layer = GameMenuScene::create();
    
    scene->addChild(layer);
    
    
    return scene;


};
bool GameMenuScene::init()
{

    if (!Layer::init()) {
        return false;
    }
    initEnvironment();
    initBackground();
    initMainMenu();
    initAircraft();
   
    return true;
}

/* 初始化程序基本属性 */
void GameMenuScene:: initEnvironment()
{
    // 表示获得视图（可视区域）的大小
    // 如果 DesignResolutionSize 跟屏幕尺寸一样大
    // 则 getVisibleSize 等于 getWinSize
     visibleSize = Director::getInstance()->getVisibleSize();
    // 表示可视区域的起点坐标，这在处理相对位置的时候非常有用
    // 确保节点在不同分辨率下的位置一致
     visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    // 播放背景音乐，循环播放
//     CocosDenshion::SimpleAudioEngine::getInstance()->
//    playBackgroundMusic("sounds/game_music.wav");
}

void GameMenuScene::initBackground()
{
    // 创建背景精灵
     auto bg1 = Sprite::create("background.png");
     bg1->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,0));
    // AnchorPoint 的默认位置 Layer 的是左下，而其他的 node 是中心点
    // 节点的原点是父节点的左下角
     bg1->setAnchorPoint(Vec2(0.5, 0));
     bg1->setTag(101);
     this->addChild(bg1, 0);
    
    //第二张背景图，是跟在第一张的上面，无缝连接，两张图形成不间断的地图滚动
    auto bg2 = Sprite::create("background.png");
    bg2->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,
                          bg1->getPositionY()+bg1->getContentSize().height));
    bg2->setAnchorPoint(Vec2(0.5, 0));
     bg2->setTag(102);
     this->addChild(bg2, 0);
    
    // 背景滚动的定时器，0.01 执行一次传入的函数
     this->schedule(schedule_selector(GameMenuScene::backgroundMove), 0.01);
}
/* 背景滚动的回调方法 */
void GameMenuScene::backgroundMove(float f)
{
    // 背景滚动逻辑
    auto bg1 = this->getChildByTag(101);
    auto bg2 = this->getChildByTag(102);
    // 当第二张图片退出屏幕时，把第一张图片设置到屏幕中，
    // 由于背景图片的高度是大于屏幕的高度的，所以判断逻辑要复杂点
    if (bg2 -> getPositionY() + bg2->getContentSize().height <=
        Director::getInstance()->getVisibleSize().height) {
        bg1->setPositionY(-bg1->getContentSize().height +
                          Director::getInstance()->getVisibleSize().height);
    }
    bg1->setPositionY(bg1->getPositionY() - 3);
    bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
}

void GameMenuScene::initAircraft(){

    auto aircraft = Sprite::create("ball.png");
    aircraft->setPosition(visibleSize.width/2+visibleOrigin.x,200);
    this->addChild(aircraft);
    
    Animation *animation = Animation::create();
    SpriteFrame*spriteFrame1 = SpriteFrame::create("hero1.png", Rect(0, 0, 102, 126));
    SpriteFrame*spriteFrame2 = SpriteFrame::create("hero2.png", Rect(0, 0, 102, 126));

    animation->addSpriteFrame(spriteFrame1);
    animation->addSpriteFrame(spriteFrame2);
    
    animation->setDelayPerUnit(0.15);
    
    
    Animate*animate = Animate::create(animation);
    
    aircraft->runAction(RepeatForever::create(animate));
    
}



void GameMenuScene::initMainMenu()
{
    // 这个是开始游戏菜单选项
    // GameMenuScene::menuStartCallback 是按钮的回调函数
    // 即点击这个菜单按钮的时候会执行该函数
    auto startItem = MenuItemImage::create("game_start.png",
                                           "game_start2.png",
                                           CC_CALLBACK_1(GameMenuScene::menuStartCallback,
                                                         this));
    // 设置开始游戏菜单按钮的位置
    startItem->setPosition(Vec2(visibleSize.width/2 + visibleOrigin.x,
                                visibleSize.height/2 + visibleOrigin.y));
    
    // 这个是结束游戏菜单选项
    auto closeItem = MenuItemImage::create("game_exit.png",
                                           "game_exit2.png",
                                           CC_CALLBACK_1(GameMenuScene::menuCloseCallback,
                                                         this));
    // 设置结束游戏菜单按钮的位置
    closeItem->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,
                                visibleSize.height/2 + visibleOrigin.y - startItem->getContentSize().height));
    
    // 把菜单项添加到菜单精灵中
    auto menu = Menu::create(startItem, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    
    // 把菜单精灵添加到当前的层中
    this->addChild(menu, 1);
}

/* 退出游戏，系统默认退出的回调函数 */
void GameMenuScene::menuCloseCallback(Ref* pSender)
{
    // 结束导演实例
    Director::getInstance()->end();
    
    // 退出程序
    exit(0);
}


void GameMenuScene::menuStartCallback(Ref* pSender)
{
    // 开始游戏
    //CCLOG("开始游戏");
    
    auto scene = GamePlayScene::createScene();
    auto GamePlayScene = TransitionSlideInR::create(1.0f, scene);
    Director::getInstance()->replaceScene(GamePlayScene);
    
    
    
    
    
 
    
    
    
}