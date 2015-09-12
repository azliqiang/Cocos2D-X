
#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    
    HelloWorld *layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    mScreenSize = CCDirector::sharedDirector()->getVisibleSize();
    initWorld();
    addBird();
    addBarContainer();
    
    setTouchEnabled(true);
    
    scheduleOnce(schedule_selector(HelloWorld::startGame), 3);
    
    return true;
}

void HelloWorld::startGame(float dt){
    scheduleUpdate();
    schedule(schedule_selector(HelloWorld::addBar), 1);
}

void HelloWorld::stopGame(){
    unscheduleUpdate();
    unschedule(schedule_selector(HelloWorld::addBar));
}

void HelloWorld::initWorld(){
    //初始化物理世界
    mWorld = new b2World(b2Vec2(0, -10));
    //设置物理世界的碰撞监听器
    mWorld->SetContactListener(this);
}

void HelloWorld::addBird(){
    //创建一个B2Sprite对象，该对象拥有精灵和Body的全部功能
    //当B2Sprite对象被初始化完成时，精灵的处理化工作已经完成，此时的Body对象依然为空
    mBird = B2Sprite::create("bird.png");
    
    //为B2Sprite对象准备与之对应的Body，此处需要注意的是Body和精灵的大小一定要匹配
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(mScreenSize.width/2.0f/RATIO, 9);
    b2Body *birdBody = mWorld->CreateBody(&bodyDef);
    
    CCSize birdSize = mBird->getContentSize();
    b2PolygonShape birdShape;
    birdShape.SetAsBox(birdSize.width/2.0f/RATIO, birdSize.height/2.0f/RATIO);
    b2FixtureDef birdFixtureDef;
    birdFixtureDef.shape = &birdShape;
    
    birdBody->CreateFixture(&birdFixtureDef);
    
    //设置PTM的值
    mBird->setPTMRatio(RATIO);
    //设置b2Sprite的body属性
    mBird->setB2Body(birdBody);
    addChild(mBird);
}

void HelloWorld::addGround() {
    B2Sprite *ground = B2Sprite::create("ground.png");
    CCSize groundSize = ground->getContentSize();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(groundSize.width/2.0f/RATIO,
                              groundSize.height/2.0f/RATIO);
    b2Body *groundBody = mWorld->CreateBody(&bodyDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(groundSize.width/2.0f/RATIO, groundSize.height/2.0f/RATIO);
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundBody->CreateFixture(&groundFixtureDef);
    
    ground->setB2Body(groundBody);
    ground->setPTMRatio(RATIO);
    addChild(ground);
    
}

void HelloWorld::addBarContainer() {
    mBarContainer = CCSprite::create();
    addChild(mBarContainer);
}

void HelloWorld::addBar(float dt){
    float offset = -rand()%5;
    
    B2Sprite *downBar = B2Sprite::create("down_bar.png");
    CCSize downBarSize = downBar->getContentSize();
    
    //生成下方的水管
    b2BodyDef downBarBodyDef;
    downBarBodyDef.type = b2_kinematicBody;
    downBarBodyDef.position = b2Vec2(mScreenSize.width/RATIO+2,
                                     downBarSize.height/RATIO/2+offset);
    //设置物体的速度
    downBarBodyDef.linearVelocity = b2Vec2(-5, 0);
    b2Body *downBarBody = mWorld->CreateBody(&downBarBodyDef);
    
    b2PolygonShape downBarShape;
    downBarShape.SetAsBox(downBarSize.width/2/RATIO,
                          downBarSize.height/2/RATIO);
    b2FixtureDef downBarFixtureDef;
    downBarFixtureDef.shape = &downBarShape;
    downBarBody->CreateFixture(&downBarFixtureDef);
    
    downBar->setB2Body(downBarBody);
    downBar->setPTMRatio(RATIO);
    mBarContainer-> addChild(downBar);

    //生成上方的水管
    B2Sprite *upBar = B2Sprite::create("up_bar.png");
    CCSize upBarSize = upBar->getContentSize();
    
    b2BodyDef upBarBodyDef;
    upBarBodyDef.type = b2_kinematicBody;
    upBarBodyDef.position = b2Vec2(mScreenSize.width/RATIO+2,
                                   downBarSize.height/RATIO+offset+2+upBarSize.height/2/RATIO);
    upBarBodyDef.linearVelocity = b2Vec2(-5, 0);
    b2Body *upBarDody = mWorld->CreateBody(&upBarBodyDef);
    
    b2PolygonShape upBarShape;
    upBarShape.SetAsBox(upBarSize.width/2/RATIO, upBarSize.height/2/RATIO);
    b2FixtureDef upBarFixtureDef;
    upBarFixtureDef.shape = &upBarShape;
    upBarDody->CreateFixture(&upBarFixtureDef);
    
    upBar->setB2Body(upBarDody);
    upBar->setPTMRatio(RATIO);
    mBarContainer-> addChild(upBar);
}

void HelloWorld::update(float dt){
    //update函数每一帧调用一次，在该函数当中调用GetContactList方法
    //该方法将会得到一个碰撞列表对象
    //每一个碰撞列表对象当中包含有两个Fixture对象，这两个对象，就是参与本次碰撞的对象，通过其getBody()方法可以得到参与
    //碰撞的body对象
    //得到body对象之后，就可以得到与之对应的CCSprite对象
    //通过碰撞对象（b2Contact）的getNext()方法，可以得到本次碰撞列表当中的下一个碰撞对象
//    b2Contact* pContact = mWorld->GetContactList();
//    pContact->GetFixtureA();
//    pContact->GetFixtureB();
    mWorld->Step(dt, 8, 3); // 8和3为官方推荐数据
    
    CCSprite *s;
    
    for(b2Body *b = mWorld->GetBodyList(); b != NULL; b = b->GetNext()){
        if(b->GetPosition().x < -3){
            s = (CCSprite *)b->GetUserData();
            if(s != NULL){
                s->removeFromParent();
            }
            mWorld->DestroyBody(b);
        }
    }
}

//使用world.GetContactList()方法进行碰撞检测
//

void HelloWorld::BeginContact(b2Contact *contact){
    if(contact->GetFixtureA()->GetBody()->GetUserData() == mBird ||
       contact->GetFixtureB()->GetBody()->GetUserData() == mBird){
        stopGame();
        CCMessageBox("Game Over!", "Game Over!");
    }
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    mBird->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, 5.0f));
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
