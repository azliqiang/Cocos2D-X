//
//  GamePlayScene.h
//  AircraftBattle
//
//  Created by qianfeng on 15-3-18.
//
//

#ifndef __AircraftBattle__GamePlayScene__
#define __AircraftBattle__GamePlayScene__

#include <iostream>

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class GamePlayScene : public cocos2d::Layer
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 visibleOrigin;
    enum Game_State {
        Game_State_Running = 1,  // 1，游戏运行
        Game_State_Pause,        // 2，游戏暂停
        Game_State_Over          // 3，游戏结束
    } gameState;
    
    float fx, fy;
    
    cocos2d::Vector<cocos2d::Sprite *> bulletList;
    cocos2d::Vector<cocos2d::Sprite *> enemyList;
    
    // 游戏所得分数
    int score;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void initEnvironment();
    void initBackground();
    
    // 滚动背景定时器回调函数
    void backgroundMove(float f);
    
    void initMyPlayer();
    
    void touchEventRegistration();
    virtual bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
    virtual void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);
    
    // 游戏的逻辑
    void gameLogic();
    
    // 子弹创建的定时器回调函数
    void bulletCreate(float f);
    // 让子弹飞
    void bulletMove(float f);
    
    void enemyCreate(float f);
    // 敌机来袭
    void enemyMove(float f);
    
    // 层进入时调用的方法，碰撞事件监听在此处声明定义
    virtual void onEnter();
    
    // 初始化分数
    void initScoreLabel();
    
    // 飞机爆炸动画
    void explosionEfect(cocos2d::Vec2 vec, int tag);
    
    // 飞机爆炸动画播放完之后的回调函数
    void explosionRemove(Node * sprite);
    
    // 停止所有的定时器
    void stopAllSchedule();
    
    // 游戏结束逻辑
    void gameOver();
    
    CREATE_FUNC(GamePlayScene);
};

#endif /* defined(__AircraftBattle__GamePlayScene__) */
