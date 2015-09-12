//
//  nijia.h
//  testgame4455
//
//  Created by stdio on 15/7/27.
//
//

#ifndef __testgame4455__nijia__
#define __testgame4455__nijia__

#include <stdio.h>

#include "cocos2d.h"

USING_NS_CC;

class nijia : public Layer
{
private:
    Size visibleSize;
    
    Vec2 visibleOrigin;
    
    LayerColor*colorLayer;
    
    std::vector<cocos2d::Sprite*>Robber;
    
    int numRobber;
    
    std::vector<cocos2d::Sprite*> projectile;
    int numProjectile;
    
public:
    
    virtual bool init();
    
    static Scene* createScene();
    
    CREATE_FUNC(nijia);
    
    void initEnvironment();
    
    void initBgColor();
    
    void initMainMenu();
    // 初始化标签
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void addHero();
    
    void addRobber(float dt);
    
    void touchEventRegistration();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    
    
    void checkDetection(float dt);


    
};



#endif /* defined(__testgame4455__nijia__) */
