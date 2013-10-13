/****************************************************************************
Copyright (c) 2013 Artavazd Barseghyan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCGestureRecognizer.h"

USING_NS_CC;

CCGestureRecognizer::CCGestureRecognizer()
{
    isRecognizing = false;
    
    dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    
    setTouchEnabled(true);
    setCancelsTouchesInView(false);
}

CCGestureRecognizer::~CCGestureRecognizer()
{
    dispatcher->removeDelegate(this);
}

void CCGestureRecognizer::setTarget(CCObject * tar, SEL_CallFuncO sel)
{
    target = tar;
    selector = sel;
}

float CCGestureRecognizer::distanceBetweenPoints(CCPoint p1, CCPoint p2)
{
    float deltaX = p2.x-p1.x;
    float deltaY = p2.y-p1.y;
    return fabs(sqrtf(deltaX*deltaX+deltaY*deltaY));
}

void CCGestureRecognizer::stopTouchesPropagation(CCSet * pTouches, CCEvent * pEvent)
{
    //hack! cancel touch so it won't propagate
    dispatcher->touchesCancelled(pTouches, pEvent);
}

void CCGestureRecognizer::setParent(CCNode*p)
{
    CCLayer::setParent(p);
    
    if (p!=NULL) {
        CCSize size = p->getContentSize();
        setContentSize(size);
        setPosition(p->getPosition());
        frame = p->boundingBox();
    }
}

void CCGestureRecognizer::setPosition(const CCPoint& p)
{
    CCLayer::setContentSize(p);
//    
//	if (getParent())
//		frame = getParent()->boundingBox();
}

void CCGestureRecognizer::setContentSize(const CCSize& size)
{
    CCLayer::setContentSize(size);
//    
//	if (getParent())
//		frame = getParent()->boundingBox();
}

CCSet * CCGestureRecognizer::createSetWithTouch(CCTouch * pTouch)
{
    CCSet * set = new CCSet();
    set->addObject(pTouch);
    return set;
}

void CCGestureRecognizer::registerWithTouchDispatcher()
{
    dispatcher->addTargetedDelegate(this, -256, false);
}

bool CCGestureRecognizer::isPositionBetweenBounds(CCPoint pos)
{
	if (getParent() && getParent()->getParent()) {
		frame = getParent()->boundingBox();
		frame.origin = getParent()->getParent()->convertToWorldSpace(frame.origin);
	}
    return frame.containsPoint(pos);
}

void CCGestureRecognizer::gestureRecognized(CCGesture * gesture)
{
	gesture->userObject = getUserObject();
	gesture->userData = getUserData();
    if (target && selector) (target->*selector)(gesture); //call selector
}