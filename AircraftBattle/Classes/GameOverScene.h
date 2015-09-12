//
//  GameOverScene.h
//  AircraftBattle
//
//  Created by qianfeng on 15-3-18.
//
//

#ifndef __AircraftBattle__GameOverScene__
#define __AircraftBattle__GameOverScene__

#include <iostream>

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 visibleOrigin;
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void initEnvironment();
    void initScoreLabel();
    void initGameMenu();
    void continueGame(cocos2d::Ref* pSender);
    void quitGame(cocos2d::Ref* pSender);
    
    CREATE_FUNC(GameOverScene);
};

#endif /* defined(__AircraftBattle__GameOverScene__) */
