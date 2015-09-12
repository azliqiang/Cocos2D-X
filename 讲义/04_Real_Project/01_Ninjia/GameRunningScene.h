#ifndef __GAMERUNNING_SCENE_H__
#define __GAMERUNNING_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameRunningScene : public cocos2d::Layer
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 visibleOrigin;
    LayerColor *colorLayer;
    std::vector<cocos2d::Sprite*> Robber;
    std::vector<cocos2d::Sprite*> projectile;
    int numRobber;
    int numProjectile;

public:
    // there's no 'id' in cpp,
    // so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference.
    // Method 'init' in cocos2d-x returns bool,
    // instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void initBgColor();

    // ��ʼ�������������
    void initEnvironment();
    // ��ʼ������˵�
    void initMainMenu();
    // ��ʼ����ǩ
    void initLabel();
    // ��ʼ��ͼƬ
    //void initPicture();

    // ���ǿ��
    void addRobber(float dt);

    // ���Ӣ��
    void addHero();

    // ��Ϸ�߼�
    void gameLogic();

    // �����¼�ע��
    void touchEventRegistration();
    // ������ʼ�¼�
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    void checkDetection(float dt);

    //bool onContactBegin(const PhysicsContact& contact);

    //void initPhysicsBody();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameRunningScene);
};

#endif // __GAMERUNNING_SCENE_H__
