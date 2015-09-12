#include "GameRunningScene.h"

#include <iostream>

USING_NS_CC;

Scene* GameRunningScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    // 'layer' is an autorelease object
    auto layer = GameRunningScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

/* on "init" you need to initialize your instance */
bool GameRunningScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

    initEnvironment();
    initMainMenu();
    initLabel();
    //initPicture();
    initBgColor();
    gameLogic();
    addHero();
    touchEventRegistration();
    //initPhysicsBody();

    return true;
}

/* 触摸事件注册 */
void GameRunningScene::touchEventRegistration()
{
    //setTouchEnabled(true);  // deprecated

    // 得到当前节点（场景、层、精灵等）的所有事件分发器
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    // 创建单点触摸监听器
    // 即只有注册的 Layer 才能接收触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    // 添加监听事件到监听器
    listener->onTouchBegan = CC_CALLBACK_2(GameRunningScene::onTouchBegan, this);
    //listener->onTouchMoved = CC_CALLBACK_2(GameRunningScene::onTouchMoved, this);
    // 向下传递触摸
    listener->setSwallowTouches(false);
    // 添加事件监听器到事件分发器
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#if 0
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameRunningScene::onContactBegin, this);
    dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
#endif
}

bool GameRunningScene::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    Point touchLocation = pTouch->getLocationInView();
    touchLocation = Director::getInstance()->convertToGL(touchLocation);
    Sprite *projectileExmaple = Sprite::create("projectile.png");
    ++numProjectile;

    auto OneBody = PhysicsBody::createBox(projectileExmaple->getContentSize());
    OneBody->applyImpulse(Vect(100, 500));
    OneBody->setContactTestBitmask(0x04);
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

#if 0
void GameRunningScene::initPhysicsBody()
{
    auto body = PhysicsBody::createEdgeBox(visibleSize);
    auto edgeNode = Node::create();
    edgeNode->setPosition(visibleSize.width/2, visibleSize.height/2);
    edgeNode->setPhysicsBody(body);

    this->addChild(edgeNode);
}

bool GameRunningScene::onContactBegin(const PhysicsContact& contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();

    this->removeChild(a->getNode());
    this->removeChild(b->getNode());

    return true;
}
#endif

/* 游戏逻辑 */
void GameRunningScene::gameLogic()
{
    this->schedule(schedule_selector(GameRunningScene::addRobber), 1);
    this->schedule(schedule_selector(GameRunningScene::checkDetection));
}

void GameRunningScene::checkDetection(float dt)
{
    for (int i = 0; i < projectile.size(); ++i) {

        for (int j = 0; j < Robber.size(); ++j) {

            float x = abs(projectile.at(i)->getPosition().x -
                Robber.at(j)->getPosition().x);
            float y = abs(projectile.at(i)->getPosition().y -
                Robber.at(j)->getPosition().y);
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

            //std::cout << "projectile Postion = " << projectile.at(i)->getPosition().x << std::endl;
            // 删除移动超过屏幕的发射体
            if (projectile.at(i)->getPosition().x > visibleSize.width) {
                //std::cout << "==================== remove projectile ====================" << std::endl;
                this->removeChild(projectile.at(i));
                std::vector<Sprite*>::iterator Iter1 = projectile.begin() + i;
                projectile.erase(Iter1);
                break;
            }
        }
    }

    // 删除不发子弹时，移动超过屏幕的强盗
    for (int j = 0; j < Robber.size(); ++j) {
        //std::cout << "Robber Postion = " << Robber.at(j)->getPosition().x << std::endl;
        if (Robber.at(j)->getPosition().x < 0) {
            //std::cout << "==================== remove Robber ====================" << std::endl;
            this->removeChild(Robber.at(j));
            std::vector<Sprite*>::iterator Iter2 = Robber.begin() + j;
            Robber.erase(Iter2);
            break;
        }
    }
}

/* 初始化图片 */
void GameRunningScene::initBgColor()
{
    colorLayer = LayerColor::create(Color4B(255, 255, 255, 255),
        visibleSize.width,visibleSize.height);
    this->addChild(colorLayer);
#if 0
    colorLayer = (LayerColor *)Sprite::create("Genesis_800x800.png");
    colorLayer->setPosition(Vec2(visibleSize.width/2 + visibleOrigin.x,
                  visibleSize.height/2 + visibleOrigin.y));
    this->addChild(colorLayer);
#endif
}

void GameRunningScene::addHero()
{
    colorLayer = (LayerColor *)Sprite::create("player.png");
    colorLayer->setPosition(visibleSize.width/8, visibleSize.height/2);
    this->addChild(colorLayer);
}

/* 初始化程序基本属性 */
void GameRunningScene::initEnvironment()
{
    // 表示获得视图（可视区域）的大小
    // 如果 DesignResolutionSize 跟屏幕尺寸一样大
    // 则 getVisibleSize 等于 getWinSize
    visibleSize = Director::getInstance()->getVisibleSize();

    // 表示可视区域的起点坐标，这在处理相对位置的时候非常有用
    // 确保节点在不同分辨率下的位置一致
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
}

/* 初始化程序菜单 */
void GameRunningScene::initMainMenu()
{
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(GameRunningScene::menuCloseCallback, this));

    closeItem->setPosition(Vec2(
        visibleOrigin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
        visibleOrigin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}

/* 初始化标签 */
void GameRunningScene::initLabel()
{
    // add a label shows "Hello World"
    // create and initialize a label

    auto label = LabelTTF::create("Programming is Romantic",
        "Arial", 24);
    label->setColor(ccc3(0, 0, 0));

    // position the label on the center of the screen
    label->setPosition(Vec2(visibleOrigin.x + visibleSize.width/2,
        visibleOrigin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
}

#if 0
/* 初始化图片 */
void GameRunningScene::initPicture()
{
    colorLayer = (LayerColor *)Sprite::create("Genesis_800x800.png");
    colorLayer->setPosition(visibleSize.width/8,
        visibleSize.height/2);
    this->addChild(colorLayer);
}
#endif

/* 退出游戏，系统默认退出的回调函数 */
void GameRunningScene::menuCloseCallback(Ref* pSender)
{
    // 结束导演实例
    Director::getInstance()->end();

    // 退出程序
    exit(0);
}

#if 0

void GameRunningScene::addRobber(float dt)
{
    Sprite* robberExample = Sprite::create("robber.png");
    ++numRobber;
    auto OneBody = PhysicsBody::createBox(robberExample->getContentSize());
    OneBody->setContactTestBitmask(0x08);
    robberExample->setPhysicsBody(OneBody);

    robberExample->setPosition(
        visibleSize.width + robberExample->getContentSize().width,
        robberExample->getContentSize().height/2+
            CCRANDOM_0_1()*(visibleSize.height-
            robberExample->getContentSize().height/2));
    Robber.push_back(robberExample);
    this->addChild(Robber.back(), i);
    MoveTo *Move = MoveTo::create(3, Vec2(-Robber.back()->getContentSize().width/2,
            Robber.back()->getPosition().y));
    Robber.back()->runAction(Move);
}

#else

void GameRunningScene::addRobber(float dt)
{
    Sprite *robberExample = Sprite::create("robber.png");
    ++numRobber;
    auto OneBody = PhysicsBody::createBox(robberExample->getContentSize());
    OneBody->setContactTestBitmask(0x08);
    robberExample->setPhysicsBody(OneBody);

    Size targetSize = robberExample->getContentSize();
    float targetInitX = visibleSize.width + robberExample->getContentSize().width/2;
    //float targetInitY = random.nextInt((int) (winSize.height - targetSize.height)) + targetSize.height / 2;
    float targetInitY = CCRANDOM_0_1() * (visibleSize.height -targetSize.height) +
        targetSize.height/2;
    Point initPoint = Point(targetInitX, targetInitY);
    robberExample->setPosition(initPoint);

    Robber.push_back(robberExample);
    this->addChild(Robber.back(), numRobber);

    float targetEndX = -targetSize.width / 2;
    float targetEndY = targetInitY;
    Point endPoint   = Point(targetEndX, targetEndY);

    float time = CCRANDOM_0_1() * 2 + 2;
    MoveTo *Move = MoveTo::create(time, Vec2(-Robber.back()->getContentSize().width/2,
        Robber.back()->getPosition().y));
    Robber.back()->runAction(Move);
}

#endif
