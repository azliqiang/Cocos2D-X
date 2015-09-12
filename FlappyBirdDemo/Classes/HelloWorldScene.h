/*
 The MIT License (MIT)
 
 Copyright (c) 2013 SuXinDe (Email: suxinde2009@126.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */



#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D.h"
#include "B2Sprite.h"

#define RATIO 48.0f // 比例 48

class HelloWorld : public cocos2d::CCLayer,
public b2ContactListener
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    ////////////////////////////////////////////////////
    // 下面添加自己的代码
    ////////////////////////////////////////////////////
    
    b2World     *mWorld;                // box2D world
    B2Sprite    *mBird;                 // bird的sprite
    CCSize       mScreenSize;           // 屏幕尺寸
    CCSprite    *mBarContainer;         //
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    //该方法就是碰撞检测的回调方法
    virtual void BeginContact(b2Contact* contact);
    virtual void update(float dt);
    
private:
    //向游戏当中添加一只小鸟
    void addBird();
    //添加背景
    void addGround();
    //初始化世界
    void initWorld();
    //添加水管
    void addBar(float dt);
    void addBarContainer();
    //开始游戏
    void startGame(float dt);
    //停止游戏
    void stopGame();
    
};

#endif // __HELLOWORLD_SCENE_H__
