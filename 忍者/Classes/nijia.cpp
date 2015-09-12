//
//  nijia.cpp
//  testgame4455
//
//  Created by stdio on 15/7/27.
//
//

#include "nijia.h"

USING_NS_CC;



bool nijia::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    initEnvironment();
    initBgColor();
    
    initMainMenu();
    addHero();
    touchEventRegistration();

    this->schedule(schedule_selector(nijia::addRobber), 1);
    
    this->schedule(schedule_selector(nijia::checkDetection));
    
    return true;
}

Scene* nijia::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));
    
    // 'layer' is an autorelease object
    auto layer = nijia::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}
void nijia::initEnvironment(){

    visibleSize = Director::getInstance()->getVisibleSize();
    
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
}
//初始化背景颜色
void nijia::initBgColor(){

    colorLayer = LayerColor::create(Color4B(200,200,200,200),visibleSize.width,visibleSize.height);

    this->addChild(colorLayer);
}
void nijia::addRobber(float dt)
{    Sprite *robberExample = Sprite::create("robber.png");
    ++numRobber;
    
    auto OneBody = PhysicsBody::createBox(robberExample->getContentSize());
    OneBody->setContactTestBitmask(0x04);
    robberExample->setPhysicsBody(OneBody);
    
    Size targetSize = robberExample->getContentSize();
    float targetInitX = getContentSize().width;
    //CCRANDOM_0_1在屏幕高度0 和1 之间随机
    float targetInitY = CCRANDOM_0_1() * (visibleSize.height -targetSize.height) +
    targetSize.height/2;
    //初始化点
    Point initPoint = Point(targetInitX, targetInitY);
    robberExample->setPosition(initPoint);
    
    
    Robber.push_back(robberExample);
    //把器里面最后的一个元素 加入到容器里面  Robber 是当前屏幕内的强盗对象的数量，numRobber是总共强盗的数量
    this->addChild(Robber.back(), 0);
    
    
    float time = CCRANDOM_0_1() * 2 + 2;
    MoveTo *Move = MoveTo::create(time, Vec2(-Robber.back()->getContentSize().width/2,Robber.back()->getPosition().y));
    
    
    Robber.back()->runAction(Move);
    
}

void nijia::addHero(){
    
    colorLayer = (LayerColor *)Sprite::create("player.png");
    colorLayer->setPosition(visibleSize.width/8, visibleSize.height/2);
    this->addChild(colorLayer);

    
    
}
void nijia::initMainMenu()
{
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(nijia::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(
                                visibleOrigin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                visibleOrigin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

/* 触摸事件注册 */
void nijia::touchEventRegistration()
{
    //setTouchEnabled(true);  // deprecated
    
    // 得到当前节点（场景、层、精灵等）的所有事件分发器
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    // 创建单点触摸监听器
    // 即只有注册的 Layer 才能接收触摸事件
    
    auto listener = EventListenerTouchOneByOne::create();
    
    // 添加监听事件到监听器
    listener->onTouchBegan = CC_CALLBACK_2(nijia::onTouchBegan, this);
    // 向下传递触摸
    listener->setSwallowTouches(false);
    // 添加事件监听器到事件分发器
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}

//判断子弹是否碰撞
void nijia::checkDetection(float dt)
{
    for (int i = 0; i < projectile.size(); ++i) {
        
        for (int j = 0; j < Robber.size(); ++j) {
            //at 获取字符串下标
            //abs 取对象的int整形
            float x = abs(projectile.at(i)->getPosition().x -Robber.at(j)->getPosition().x);
            float y = abs(projectile.at(i)->getPosition().y -Robber.at(j)
                          ->getPosition().y);
            float length = sqrt(x * x + y * y);
            
            // 判断子弹和强盗是否位置重合
            
            if (length < Robber.at(j)->getContentSize().width/2 +
                projectile.at(i)->getContentSize().width/2) {
                
                this->removeChild(projectile.at(i));
                this->removeChild(Robber.at(j));
                std::vector<Sprite*>::iterator Iter1 = projectile.begin() + i;
                projectile.erase(Iter1);
                
                std::vector<Sprite*>::iterator Iter2 = Robber.begin() + j;
                Robber.erase(Iter2);
                break;
            }
            if (projectile.at(i)->getPosition().x > visibleSize.width) {
                this->removeChild(projectile.at(i));
                //Iterator ：迭代器角色负责定义访问和遍历元素的接口
                std::vector<Sprite*>::iterator Iter1 = projectile.begin() + i;
                //erase 是擦除
                projectile.erase(Iter1);
                break;
            }
        }
    }
    for (int j = 0; j < Robber.size(); ++j) {
        if (Robber.at(j)->getPosition().x < 0) {
            this->removeChild(Robber.at(j));
            std::vector<Sprite*>::iterator Iter2 = Robber.begin() + j;
            Robber.erase(Iter2);
            break;
        }
    }
}


bool nijia::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    Point touchLocation = pTouch->getLocationInView();
    touchLocation = Director::getInstance()->convertToGL(touchLocation);
    Sprite *projectileExmaple = Sprite::create("ball.png");
    ++numProjectile;
    
    auto OneBody = PhysicsBody::createBox(projectileExmaple->getContentSize());
    OneBody->applyImpulse(Vect(100, 500));
    OneBody->setContactTestBitmask(0x01);
    projectileExmaple->setPhysicsBody(OneBody);
    
    projectileExmaple->setPosition(visibleSize.width/8, visibleSize.height/2);
    projectile.push_back(projectileExmaple);
    
    this->addChild(projectile.back(), numProjectile);
    
    Point offset = ccpSub(touchLocation, colorLayer->getPosition());
    
    float ratio  = offset.y / offset.x;
    
    int targetX  = colorLayer->getContentSize().width/2 + visibleSize.width;
    
    int targetY  = (targetX*ratio) + colorLayer->getPosition().y;
    
    Vec2 targetPosition = Vec2(targetX, targetY);
    
    MoveTo* Move = MoveTo::create(2, targetPosition);
    projectile.back()->runAction(Move);
    
    return true;
}

void nijia::menuCloseCallback(Ref* pSender)
{
    
    Director::getInstance()->end();
    
    exit(0);
}
