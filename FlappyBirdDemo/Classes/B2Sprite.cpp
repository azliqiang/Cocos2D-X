
#include "B2Sprite.h"
#include "support/CCPointExtension.h"

#include "Box2D/Box2D.h"

B2Sprite::B2Sprite(): m_bIgnoreBodyRotation(false), m_pB2Body(NULL), m_fPTMRatio(0.0f)
{}

B2Sprite* B2Sprite::create()
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

B2Sprite* B2Sprite::createWithTexture(CCTexture2D *pTexture)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithTexture(pTexture))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

B2Sprite* B2Sprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithTexture(pTexture, rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

B2Sprite* B2Sprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithSpriteFrame(pSpriteFrame))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

B2Sprite* B2Sprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithSpriteFrameName(pszSpriteFrameName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

B2Sprite* B2Sprite::create(const char *pszFileName)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithFile(pszFileName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

B2Sprite* B2Sprite::create(const char *pszFileName, const CCRect& rect)
{
    B2Sprite* pRet = new B2Sprite();
    if (pRet && pRet->initWithFile(pszFileName, rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool B2Sprite::isDirty()
{
    return true;
}

bool B2Sprite::isIgnoreBodyRotation() const
{
    return m_bIgnoreBodyRotation;
}

void B2Sprite::setIgnoreBodyRotation(bool bIgnoreBodyRotation)
{
    m_bIgnoreBodyRotation = bIgnoreBodyRotation;
}

const CCPoint& B2Sprite::getPosition()
{
    updatePosFromPhysics();
    return CCNode::getPosition();
}

void B2Sprite::getPosition(float* x, float* y)
{
    updatePosFromPhysics();
    return CCNode::getPosition(x, y);
}

float B2Sprite::getPositionX()
{
    updatePosFromPhysics();
    return m_obPosition.x;
}

float B2Sprite::getPositionY()
{
    updatePosFromPhysics();
    return m_obPosition.y;
}

b2Body* B2Sprite::getB2Body() const
{
    return m_pB2Body;
}

void B2Sprite::setB2Body(b2Body *pBody)
{
    m_pB2Body = pBody;
    pBody->SetUserData(this);
}

float B2Sprite::getPTMRatio() const
{
    return m_fPTMRatio;
}

void B2Sprite::setPTMRatio(float fRatio)
{
    m_fPTMRatio = fRatio;
}

void B2Sprite::updatePosFromPhysics()
{
    b2Vec2 pos = m_pB2Body->GetPosition();
    float x = pos.x * m_fPTMRatio;
    float y = pos.y * m_fPTMRatio;
    m_obPosition = ccp(x,y);
}

void B2Sprite::setPosition(const CCPoint &pos)
{
    float angle = m_pB2Body->GetAngle();
    m_pB2Body->SetTransform(b2Vec2(pos.x / m_fPTMRatio, pos.y / m_fPTMRatio), angle);
}

float B2Sprite::getRotation()
{
    return (m_bIgnoreBodyRotation ? CCSprite::getRotation() :
            CC_RADIANS_TO_DEGREES(m_pB2Body->GetAngle()));
}

void B2Sprite::setRotation(float fRotation)
{
    if (m_bIgnoreBodyRotation)
    {
        CCSprite::setRotation(fRotation);
    }
    else
    {
        b2Vec2 p = m_pB2Body->GetPosition();
        float radians = CC_DEGREES_TO_RADIANS(fRotation);
        m_pB2Body->SetTransform(p, radians);
    }
}

CCAffineTransform B2Sprite::nodeToParentTransform()
{
    b2Vec2 pos  = m_pB2Body->GetPosition();
	
	float x = pos.x * m_fPTMRatio;
	float y = pos.y * m_fPTMRatio;
	
	if (m_bIgnoreAnchorPointForPosition)
    {
		x += m_obAnchorPointInPoints.x;
		y += m_obAnchorPointInPoints.y;
	}
	
	float radians = m_pB2Body->GetAngle();
	float c = cosf(radians);
	float s = sinf(radians);
	
	if (!m_obAnchorPointInPoints.equals(CCPointZero))
    {
		x += ((c * -m_obAnchorPointInPoints.x * m_fScaleX) + (-s * -m_obAnchorPointInPoints.y * m_fScaleY));
		y += ((s * -m_obAnchorPointInPoints.x * m_fScaleX) + (c * -m_obAnchorPointInPoints.y * m_fScaleY));
	}
    
	m_sTransform = CCAffineTransformMake( c * m_fScaleX,	s * m_fScaleX,
									     -s * m_fScaleY,	c * m_fScaleY,
									     x,	y );
	
	return m_sTransform;
}
