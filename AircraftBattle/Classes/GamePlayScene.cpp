//
//  GamePlayScene.cpp
//  AircraftBattle
//
//  Created by qianfeng on 15-3-18.
//
//

#include "GamePlayScene.h"
#include "GameMenuScene.h"
//#include "GameOverScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene *GamePlayScene::createScene()
{
    // 创建物理世界的场景
    auto scene = Scene::createWithPhysics();
    
    //PhysicsWorld* phyWorld = scene->getPhysicsWorld();
    //phyWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto layer = GamePlayScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool GamePlayScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    initEnvironment();
    initBackground();
    initMyPlayer();
    touchEventRegistration();
    gameLogic();
    
    return true;
}

/* 层进入的时候会调用该函数，进行物理世界的碰撞检测 */
void GamePlayScene::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerPhysicsContact::create();
    
    listener -> onContactBegin = [=](PhysicsContact& contact) {
        auto spriteA = (Sprite *)contact.getShapeA()->getBody()->getNode();
        auto spriteB = (Sprite *)contact.getShapeB()->getBody()->getNode();
        int tag1 = spriteA->getTag();
        int tag2 = spriteB->getTag();
        Vec2 vec1 = spriteA->getPosition();
        Vec2 vec2 = spriteB->getPosition();
        
        // 敌机和子弹碰撞碰撞
        // 敌机 105 或 104，子弹 106，我机 103
        if (tag1 + tag2 == 210 || tag1 + tag2 == 211) {
            SimpleAudioEngine::getInstance()->playEffect("sounds/use_bomb.wav");
            // 加分，如果是 104 则是小敌机 500 分，105 是大敌机 1000 分
            if (tag1 == 104 || tag2 == 104) {
                score += 500;
            } else {
                score += 1000;
            }
            auto scoreSpire = (Label *)this->getChildByTag(100);
            scoreSpire->setString(String::createWithFormat("分数:%d", score)->_string);
            // 粒子特效
            auto *system = ParticleSystemQuad::create("particle_texture.plist");
            
            if (tag1 == 104 || tag1 == 105) {
                // 移除敌机和子弹
                enemyList.eraseObject(spriteA);
                bulletList.eraseObject(spriteB);
                system->setPosition(vec1);
                // 启动动画
                this->explosionEfect(vec1, tag1);
            } else {
                enemyList.eraseObject(spriteB);
                bulletList.eraseObject(spriteA);
                // 启动动画
                this->explosionEfect(vec2, tag2);
                system->setPosition(vec2);
            }
            
            // 粒子特效加入层中
            this->addChild(system);
            spriteA->removeFromParent();
            spriteB->removeFromParent();
            
            // 敌机与我机碰撞
        } else if ((tag1+tag2) == 207 || (tag1+tag2) == 208) {
            
            SimpleAudioEngine::getInstance()->playEffect("sounds/game_over.wav");
            // 修改游戏状态为结束状态 Game_State_Over
            gameState = Game_State_Over;
            // 停止所有的定时器
            stopAllSchedule();
            // 游戏结束逻辑
            if (tag1 == 103) {
                this->explosionEfect(vec2, tag2);
                this->explosionEfect(vec1, tag1);
            } else {
                this->explosionEfect(vec1, tag1);
                this->explosionEfect(vec2, tag2);
            }
            // 使用 UserDefault 存储些用户数据分数
            UserDefault *userDefault = UserDefault::getInstance();
            int topScore = userDefault->getIntegerForKey("topScore");
            if (topScore < score) {
                userDefault->setIntegerForKey("topScore", score);
            } else {
                userDefault->setIntegerForKey("topScore", topScore);
            }
            userDefault->setIntegerForKey("currentScore", score);
            
            spriteA->removeFromParent();
            spriteB->removeFromParent();
        } else{ }
        
        return true;
    };
    
    // 注册监听器
    EventDispatcher *eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

/* 停止所有的定时器 */
void GamePlayScene::stopAllSchedule()
{
    this->unscheduleAllSelectors();
}

/* 飞机爆炸动画 */
void GamePlayScene::explosionEfect(Vec2 vec, int tag)
{
    float timeDelay = 0.1;
    Vector<SpriteFrame*> animationframe;
    // 小敌机动画帧
    if (tag == 104) {
        for (int i = 1; i < 5; i++) {
            auto string = cocos2d::__String::createWithFormat("enemy1_down%d.png", i);
            SpriteFrame * sf=SpriteFrame::create(string->getCString(),
                                                 Rect(0, 0, 57, 43));
            animationframe.pushBack(sf);
        }
    } else if (tag == 105) {
        for(int i = 1; i < 5; i++) {
            auto string = cocos2d::__String::createWithFormat("enemy2_down%d.png", i);
            SpriteFrame * sf=SpriteFrame::create(string->getCString(),
                                                 Rect(0, 0, 69, 95));
            animationframe.pushBack(sf);
        }
    } else {
        timeDelay = 0.5;
        for(int i=1;i<5;i++) {
            auto string = cocos2d::__String::createWithFormat("hero_blowup_n%d.png", i);
            SpriteFrame * sf = SpriteFrame::create(string->getCString(),
                                                   Rect(0, 0, 102, 126));
            animationframe.pushBack(sf);
        }
    }
    Animation * ani = Animation::createWithSpriteFrames(animationframe, timeDelay);
    auto blanksprite = Sprite::create();
    blanksprite->setTag(tag);
    Action *act = Sequence::create(Animate::create(ani),
                                   CCCallFuncN::create(blanksprite, callfuncN_selector(GamePlayScene::explosionRemove)),
                                   NULL);
    this->addChild(blanksprite);
    blanksprite->setPosition(vec);
    blanksprite->runAction(act);
}

/* 删除爆炸动画本身 */
void GamePlayScene::explosionRemove(Node * sprite)
{
    sprite->removeFromParentAndCleanup(true);
    if (sprite->getTag() == 103) {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic("sounds/game_music.wav");
        // 游戏结束
        gameOver();
    }
}

/* 游戏结束 */
void GamePlayScene::gameOver()
{
    //CCLOG("游戏结束");
//    auto scene = GameOverScene::createScene();
//    auto gameOverScene = TransitionTurnOffTiles::create(1.0f, scene);
//    Director::getInstance()->replaceScene(gameOverScene);
    
    
    auto scene = GameMenuScene::createScene();
    auto gameOverScene = TransitionTurnOffTiles::create(1.0f, scene);
    Director::getInstance()->replaceScene(gameOverScene);
}

void GamePlayScene::gameLogic()
{
    // 我机发射子弹创建
    this->schedule(schedule_selector(GamePlayScene::bulletCreate), 0.3);
    // 让子弹飞
    this->schedule(schedule_selector(GamePlayScene:: bulletMove), 0.01);
    this->schedule(schedule_selector(GamePlayScene::enemyCreate), 0.5);
    this->schedule(schedule_selector(GamePlayScene:: enemyMove), 0.01);
}

void GamePlayScene::initEnvironment()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
    gameState = Game_State_Running ;  // 初始化游戏状态
    
    // 初始化分数标签
    initScoreLabel();
}

void GamePlayScene::initScoreLabel()
{
    // 初始化分数为 0
    score = 0;
    
    // 添加分数
    //auto label = Label::createWithTTF("", "fonts/MarkerFelt.ttf", 24);
    auto label = cocos2d::Label::createWithSystemFont("分数:0", "Arial", 24);
    label->setTag(100);
    label->setPosition(Vec2(0, visibleOrigin.y + visibleSize.height -
                            label->getContentSize().height/2));
    label->setHorizontalAlignment(TextHAlignment::RIGHT);
    label->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(label, 1);
}

/* 初始化游戏背景 */
void GamePlayScene::initBackground()
{
    // 创建背景精灵
    auto bg1 = Sprite::create("background.png");
    bg1->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,0));
    // AnchorPoint 的默认位置 Layer 的是左下，而其他的 node 是中心点
    // 节点的原点是父节点的左下角
    bg1->setAnchorPoint(Vec2(0.5, 0));
    bg1->setTag(101);
    this->addChild(bg1, 0);
    
    // 第二张背景图，是跟在第一张的上面，无缝连接，两张图形成不间断的地图滚动
    auto bg2 = Sprite::create("background.png");
    bg2->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,
                              bg1->getPositionY()+bg1->getContentSize().height));
    bg2->setAnchorPoint(Vec2(0.5, 0));
    bg2->setTag(102);
    this->addChild(bg2, 0);
    
    // 背景滚动的定时器，0.01 执行一次传入的函数
    this->schedule(schedule_selector(GamePlayScene::backgroundMove), 0.01);
}

/* 背景滚动的回调方法 */
void GamePlayScene::backgroundMove(float f)
{
    // 背景滚动逻辑
    auto bg1 = this->getChildByTag(101);
    auto bg2 = this->getChildByTag(102);
    
    // 当第二张图片退出屏幕时，把第一张图片设置到屏幕中
    // 由于背景图片的高度是大于屏幕的高度的，所以判断逻辑要复杂点
    if(bg2 -> getPositionY() + bg2->getContentSize().height <=
        Director::getInstance()->getVisibleSize().height) {
        bg1->setPositionY(-bg1->getContentSize().height +
                                  Director::getInstance()->getVisibleSize().height);
    }
    bg1->setPositionY(bg1->getPositionY() - 4);
    bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
}

/* 初始化我方战机 */
void GamePlayScene::initMyPlayer()
{
    // 创建我方战机
    auto aircraft = Sprite::create("hero1.png");
    aircraft->setPosition(visibleSize.width/2+visibleOrigin.x, 200);
    aircraft->setTag(103);
    
    // 设置碰撞属性
    // 碰撞掩码，用于判断是否可以与其他物体碰撞，
    // 判断逻辑为两者的碰撞掩码进行逻辑与，如为零则不会碰撞，否则会
    // 这里只有我机、敌机、子弹
    // 子弹和敌机可以碰撞，敌机和我机可以碰撞，我机和子弹不可以碰撞
    // 依次可以设置它们的掩码分别为：0x01(0001)、0x03(0011)、0x02(0010)
    auto aircraftBody = PhysicsBody::createBox(aircraft->getContentSize());
    aircraftBody->setContactTestBitmask(0x0003);
    aircraftBody->setCategoryBitmask(0x0001);
    aircraftBody->setCollisionBitmask(0x0007);
    aircraftBody->setGravityEnable(false);
    aircraft->setPhysicsBody(aircraftBody);
    this->addChild(aircraft);
    
    Animation *animation = Animation::create();
    SpriteFrame *spriteFrame1 = SpriteFrame::create("hero1.png",
                                                         Rect(0, 0, 102, 126));
    SpriteFrame *spriteFrame2 = SpriteFrame::create("hero2.png",
                                                         Rect(0, 0, 102, 126));
    animation->addSpriteFrame(spriteFrame1);
    animation->addSpriteFrame(spriteFrame2);
    animation->setDelayPerUnit(0.15f);
    Animate * animate = Animate::create(animation);
    
    aircraft->runAction(RepeatForever::create(animate));
}

/* 触摸事件注册，要通过回调函数来控制飞机的坐标 */
void GamePlayScene:: touchEventRegistration()
{
    //setTouchEnabled(true);  // deprecated
    
    // 得到当前节点（场景、层、精灵等）的所有事件分发器
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    // 创建单点触摸监听器
    // 即只有注册的 Layer 才能接收触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    // 添加监听事件到监听器
    listener->onTouchBegan = CC_CALLBACK_2(GamePlayScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GamePlayScene::onTouchMoved, this);
    // 不向下传递触摸
    listener->setSwallowTouches(true);
    // 添加事件监听器到事件分发器
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

/* 手指点击下时，记录该点的位置，该点为起点 */
bool GamePlayScene::onTouchBegan(Touch * touch, Event * event)
{
    if (Game_State_Running == gameState) {
        fx = touch->getLocation().x;
        fy = touch->getLocation().y;
    }
    
    return true;
}

/*
 * 每次移动把移动的位置（终点）记录下来，并与之前记录下的位置相减
 * 得到飞机该位移的相对量（x、y轴移动多少），并刷新起点位置
 */
void GamePlayScene::onTouchMoved(Touch * touch, Event * event)
{
    if (Game_State_Running == gameState) {
        int mx = touch->getLocation().x - fx;
        int my = touch->getLocation().y - fy;
        auto spaircraft = this->getChildByTag(103);
        spaircraft->runAction(MoveBy::create(0, Point(mx, my)));
        fx = touch->getLocation().x;
        fy = touch->getLocation().y;
    }
}

/* 子弹创建 */
void GamePlayScene::bulletCreate(float f)
{
    SimpleAudioEngine::getInstance()->playEffect("sounds/bullet.wav");
    auto aircraft = this->getChildByTag(103);
    Sprite *bullet=Sprite::create("bullet.png");
    bullet->setPosition(aircraft->getPosition().x,
                             aircraft->getPosition().y+60);
    bullet->setTag(106);
    auto bulletBody = PhysicsBody::createBox(bullet->getContentSize());
    bulletBody->setContactTestBitmask(0x0002);
    bulletBody->setCategoryBitmask(0x0005);
    bulletBody->setCollisionBitmask(0x0002);
    bulletBody->setGravityEnable(false);
    bullet->setPhysicsBody(bulletBody);
    this->addChild(bullet);
    this->bulletList.pushBack(bullet);
}

/* 让子弹飞 */
void GamePlayScene::bulletMove(float f)
{
    // 遍历 vector 取出所有的子弹，让子弹的位置往上移，移动快了就是飞
    for (int i = 0; i < bulletList.size() ; i++) {
        auto bullet = bulletList.at(i);
        bullet->setPositionY(bullet->getPositionY() + 3);
        // 如果该子弹已经超出屏幕范围，则移除它
        if (bullet->getPositionY() > Director::getInstance()->getWinSize().height) {
            // 从层中移除
            bullet->removeFromParent();
            // 从记录所有子弹的 vector 中移除
            bulletList.eraseObject(bullet);
            // 移除后上一个对象会移到当前这个对象的位置
            // 实际还是当前这个 i，所以要 --i 才能访问到下一个对象
            --i;
        }
    }
}

/* 敌机创建 */
void GamePlayScene::enemyCreate(float f)
{
    // 随机出现敌机 1 或敌机 2
    int ranDom = rand() % 2 + 1;
    auto string = cocos2d::__String::createWithFormat("enemy%d.png", ranDom);
    //CCLOG("%s", string->getCString());
    auto enemy = Sprite::create(string->getCString());
    if (ranDom == 1) {
        // 敌机的类型，由这个来判断，用于分数计算
        enemy->setTag(104);
    } else {
        enemy->setTag(105);
    }
    
    // 随机在屏幕最上方的出现敌机
    auto width = rand()%(int)(Director::getInstance()->getVisibleSize().width);
    auto height = Director::getInstance()->getVisibleSize().height +
    enemy->getContentSize().height;
    enemy->setPosition(Vec2(width, height));
    auto enemyBody = PhysicsBody::createBox(enemy->getContentSize());
    enemyBody->setContactTestBitmask(0x0003);
    enemyBody->setCategoryBitmask(0x0002);
    enemyBody->setCollisionBitmask(0x0001);
    enemyBody->setGravityEnable(false);
    enemy->setPhysicsBody(enemyBody);
    this->addChild(enemy);
    this->enemyList.pushBack(enemy);
}

/* 让敌人来得更猛烈些吧 */
void GamePlayScene::enemyMove(float f)
{
    // 取出所有的敌机，让敌机往下移动
    for (int i = 0; i < enemyList.size(); ++i) {
        auto enemy = enemyList.at(i);
        enemy->setPositionY(enemy->getPositionY() - 5);
        // 如果该子弹已经超出屏幕范围，则移除它
        if (enemy->getPositionY() < -enemy->getContentSize().height) {
            // 从层中移除
            enemy->removeFromParent();
            // 从记录所有子弹的 vector 中移除
            enemyList.eraseObject(enemy);
            // 移除后上一个对象会移到当前这个对象的位置，
            // 实际还是当前这个 i，所以要 --i 才能访问到下一个对象
            --i;
        }
    }
}