//
//  GameMenuScene.h
//  testgame4455
//
//  Created by stdio on 15/6/2.
//
//

#ifndef __testgame4455__GameMenuScene__
#define __testgame4455__GameMenuScene__

#include <stdio.h>

class GameMenuScene :public cocos2d::Layer{
private:

    cocos2d::Size visibleSize;
    cocos2d::Vec2 visibleOrigin;

public:
    static cocos2d::Scene*createScene();
    
    virtual bool init();
    
    void initEnvironment();
    void initBackground();
    void initMainMenu();
    void initAircraft();
    
    //菜单回调处理
    void menuCloseCallback(cocos2d::Ref* pSender);    
    void menuStartCallback(cocos2d::Ref* pSender);

    //定时器毁掉的背景滚动方法
    void backgroundMove(float f);
    CREATE_FUNC(GameMenuScene);
    
    
    
};



#endif /* defined(__testgame4455__GameMenuScene__) */
