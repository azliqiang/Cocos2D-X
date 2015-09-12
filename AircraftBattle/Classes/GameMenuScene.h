//
//  GameMenuScene.h
//  AircraftBattle
//
//  Created by qianfeng on 15-3-18.
//
//

#ifndef __AircraftBattle__GameMenuScene__
#define __AircraftBattle__GameMenuScene__

#include <iostream>

#include "cocos2d.h"

class GameMenuScene : public cocos2d::Layer
{
private:
        cocos2d::Size visibleSize;
        cocos2d::Vec2 visibleOrigin;
    
public:
    // there's no 'id' in cpp
    // so we recommend returning the class instance pointer
        static cocos2d::Scene *createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone
        virtual bool init();
    
        void initEnvironment();
        void initBackground();
        void initMainMenu();
        void initAircraft();
    
    // 菜单项的回调处理方法
        void menuCloseCallback(cocos2d::Ref* pSender);
        void menuStartCallback(cocos2d::Ref* pSender);
    
    // 定时器回调的背景滚动方法
        void backgroundMove(float f);
    
    // CREATE_FUNC 先调用构造函数（CCLayer类node方法中调用构造函数）
    // 再调用 init 方法进行初始化（子类中先调用父类的 init 方法）
    // 最后又设置创建出的对象为自动释放内存
    // 这样其他人在使用这个类的时候，只要是用 create() 函数创建出来的对象
    // 不用费心去管理释放内存了
    CREATE_FUNC(GameMenuScene);
};

#endif /* defined(__AircraftBattle__GameMenuScene__) */
