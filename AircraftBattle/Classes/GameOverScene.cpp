//
//  GameOverScene.cpp
//  AircraftBattle
//
//  Created by qianfeng on 15-3-18.
//
//

#include "GameOverScene.h"
#include "GamePlayScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GameOverScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    initEnvironment();
    initScoreLabel();
    initGameMenu();
    
    return true;
}

void GameOverScene::initEnvironment()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    // 创建背景
    auto bg = Sprite::create("gameover.png");
    bg->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,
                         visibleSize.height/2));
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(bg, 0);
}

void GameOverScene::initScoreLabel()
{
    int currentScore = UserDefault::getInstance()->getIntegerForKey("currentScore");
    int topScore = UserDefault::getInstance()->getIntegerForKey("topScore");
    
    // 显示最终分数
    auto label = cocos2d::Label::createWithSystemFont(__String::createWithFormat("%d",
                                                                                 currentScore)->getCString(), "Arial", 24);
    label->setPosition(Vec2(visibleSize.width/2,
                            visibleOrigin.y + visibleSize.height/2 + 60));
    label->setHorizontalAlignment(TextHAlignment::RIGHT);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label, 1);
    
    // 显示最高分数
    auto label2 = cocos2d::Label::createWithSystemFont(__String::createWithFormat("%d",
                                                                                  topScore)->getCString(), "Arial", 24);
    label2->setPosition(Vec2(visibleSize.width/2,
                             visibleOrigin.y + visibleSize.height-label2->getContentSize().height));
    label2->setHorizontalAlignment(TextHAlignment::RIGHT);
    label2->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(label2, 1);
}

void GameOverScene:: initGameMenu()
{
    // 继续游戏菜单项
    auto startItem = MenuItemImage::create("game_start.png", "game_start2.png",
                                           CC_CALLBACK_1(GameOverScene::continueGame, this));
    startItem->setPosition(Vec2(visibleSize.width/2 + visibleOrigin.x,
                                visibleSize.height/2 + visibleOrigin.y));
    
    // 退出游戏菜单项
    auto closeItem = MenuItemImage::create( "game_exit.png", "game_exit2.png",
                                           CC_CALLBACK_1(GameOverScene::quitGame, this));
    closeItem->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,
                                visibleSize.height/2 + visibleOrigin.y - startItem->getContentSize().height));
    
    // 把菜单项添加到菜单精灵中
    auto menu = Menu::create(startItem, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    
    // 把菜单精灵添加到当前的层中
    this->addChild(menu, 1);
}

/* 继续游戏菜单项的回调函数 */
void GameOverScene::continueGame(Ref *pSender)
{
    auto scene = GamePlayScene::createScene();
    auto gameScene = TransitionSlideInR::create(1.0f, scene);
    Director::getInstance()->replaceScene(gameScene);
}

/* 退出游戏，系统默认退出的回调函数 */
void GameOverScene::quitGame(Ref *pSender)
{
    // 结束导演实例
    Director::getInstance()->end();
    
    // 退出程序
    exit(0);
}