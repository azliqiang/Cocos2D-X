#ifndef __PHYSICSNODES_CCPHYSICSSPRITE_H__
#define __PHYSICSNODES_CCPHYSICSSPRITE_H__

#include "cocos2d.h"
#include "ExtensionMacros.h"

USING_NS_CC;

class b2Body;


//该类的作用是将Box2D引擎当中的Body和Cocos2dx当中的CCSprite进行整合，结合两者的功能
class B2Sprite : public CCSprite
{
protected:
    bool    m_bIgnoreBodyRotation;
    
    b2Body  *m_pB2Body;
    
    float   m_fPTMRatio;
public:
    B2Sprite();
    
    //一下的静态方法用于创建一个B2Sprite对象
    static B2Sprite* create();
    static B2Sprite* createWithTexture(CCTexture2D *pTexture);
    
    static B2Sprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
    
    static B2Sprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
    static B2Sprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    
    static B2Sprite* create(const char *pszFileName);
    
    static B2Sprite* create(const char *pszFileName, const CCRect& rect);
    
    virtual bool isDirty();
    
    bool isIgnoreBodyRotation() const;
    void setIgnoreBodyRotation(bool bIgnoreBodyRotation);
    
    virtual const CCPoint& getPosition();
    virtual void getPosition(float* x, float* y);
    virtual float getPositionX();
    virtual float getPositionY();
    virtual void setPosition(const CCPoint &position);
    virtual float getRotation();
    virtual void setRotation(float fRotation);
    virtual CCAffineTransform nodeToParentTransform();
    
    b2Body* getB2Body() const;
    void setB2Body(b2Body *pBody);
    
    float getPTMRatio() const;
    void setPTMRatio(float fPTMRatio);
    
protected:
    void updatePosFromPhysics();
};

#endif

