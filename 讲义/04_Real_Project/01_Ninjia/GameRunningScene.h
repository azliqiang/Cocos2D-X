#ifndef __GAMERUNNING_SCENE_H__
#define __GAMERUNNING_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameRunningScene : public cocos2d::Layer
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 visibleOrigin;
    LayerColor *colorLayer;
    std::vector<cocos2d::Sprite*> Robber;
    std::vector<cocos2d::Sprite*> projectile;
    int numRobber;
    int numProjectile;

public:
    // there's no 'id' in cpp,
    // so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference.
    // Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void initBgColor();

    // 初始化程序基本属性
    void initEnvironment();
    // 初始化程序菜单
    void initMainMenu();
    // 初始化标签
    void initLabel();
    // 初始化图片
    //void initPicture();

    // 添加强盗
    void addRobber(float dt);

    // 添加英雄
    void addHero();

    // 游戏逻辑
    void gameLogic();

    // 触摸事件注册
    void touchEventRegistration();
    // 触摸开始事件
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    void checkDetection(float dt);

    //bool onContactBegin(const PhysicsContact& contact);

    //void initPhysicsBody();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameRunningScene);
};

#endif // __GAMERUNNING_SCENE_H__
