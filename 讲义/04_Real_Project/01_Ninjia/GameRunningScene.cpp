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

/* �����¼�ע�� */
void GameRunningScene::touchEventRegistration()
{
    //setTouchEnabled(true);  // deprecated

    // �õ���ǰ�ڵ㣨�������㡢����ȣ��������¼��ַ���
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    // �������㴥��������
    // ��ֻ��ע��� Layer ���ܽ��մ����¼�
    auto listener = EventListenerTouchOneByOne::create();
    // ��Ӽ����¼���������
    listener->onTouchBegan = CC_CALLBACK_2(GameRunningScene::onTouchBegan, this);
    //listener->onTouchMoved = CC_CALLBACK_2(GameRunningScene::onTouchMoved, this);
    // ���´��ݴ���
    listener->setSwallowTouches(false);
    // ����¼����������¼��ַ���
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

/* ��Ϸ�߼� */
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

            // �ж��ӵ���ǿ���Ƿ�λ���غ�
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
            // ɾ���ƶ�������Ļ�ķ�����
            if (projectile.at(i)->getPosition().x > visibleSize.width) {
                //std::cout << "==================== remove projectile ====================" << std::endl;
                this->removeChild(projectile.at(i));
                std::vector<Sprite*>::iterator Iter1 = projectile.begin() + i;
                projectile.erase(Iter1);
                break;
            }
        }
    }

    // ɾ�������ӵ�ʱ���ƶ�������Ļ��ǿ��
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

/* ��ʼ��ͼƬ */
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

/* ��ʼ������������� */
void GameRunningScene::initEnvironment()
{
    // ��ʾ�����ͼ���������򣩵Ĵ�С
    // ��� DesignResolutionSize ����Ļ�ߴ�һ����
    // �� getVisibleSize ���� getWinSize
    visibleSize = Director::getInstance()->getVisibleSize();

    // ��ʾ���������������꣬���ڴ������λ�õ�ʱ��ǳ�����
    // ȷ���ڵ��ڲ�ͬ�ֱ����µ�λ��һ��
    visibleOrigin = Director::getInstance()->getVisibleOrigin();
}

/* ��ʼ������˵� */
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

/* ��ʼ����ǩ */
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
/* ��ʼ��ͼƬ */
void GameRunningScene::initPicture()
{
    colorLayer = (LayerColor *)Sprite::create("Genesis_800x800.png");
    colorLayer->setPosition(visibleSize.width/8,
        visibleSize.height/2);
    this->addChild(colorLayer);
}
#endif

/* �˳���Ϸ��ϵͳĬ���˳��Ļص����� */
void GameRunningScene::menuCloseCallback(Ref* pSender)
{
    // ��������ʵ��
    Director::getInstance()->end();

    // �˳�����
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
