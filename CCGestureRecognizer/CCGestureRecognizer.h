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

#ifndef CCGestureRecognizer_h
#define CCGestureRecognizer_h

#include "cocos2d.h"

class CCGesture : public cocos2d::CCObject
{
public:
	CCGesture() : userObject(0), userData(0) {}
    bool init() {return true;}
	cocos2d::CCObject* userObject;
	int userData;
};

class CCGestureRecognizer : public cocos2d::CCLayer
{
public:
    CCGestureRecognizer();
    ~CCGestureRecognizer();
    void setTarget(cocos2d::CCObject * tar, cocos2d::SEL_CallFuncO sel);
    
    //setParent is called after the layer is added as a child
    virtual void setParent(cocos2d::CCNode*p);
	
	virtual void setPosition(const cocos2d::CCPoint& p);
	virtual void setContentSize(const cocos2d::CCSize& size);
	
protected:
    cocos2d::CCRect frame;
    bool isRecognizing;
    
    void gestureRecognized(CCGesture * gesture);
    void stopTouchesPropagation(cocos2d::CCSet * pTouches, cocos2d::CCEvent * pEvent);
    
    //utility methods
    bool isPositionBetweenBounds(cocos2d::CCPoint pos);
    float distanceBetweenPoints(cocos2d::CCPoint p1, cocos2d::CCPoint p2);
    cocos2d::CCSet * createSetWithTouch(cocos2d::CCTouch * pTouch);
    
    virtual void registerWithTouchDispatcher(void);
    
    //mandatory methods for subclasses
    virtual bool ccTouchBegan(cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)=0;
    virtual void ccTouchMoved(cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)=0;
    virtual void ccTouchEnded(cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)=0;
    
    //if gesture is correctly recognized, cancel touch over other views (default: false)
    //NOTE: subclasses must check the value and implement it correctly
    CC_SYNTHESIZE(bool, cancelsTouchesInView, CancelsTouchesInView);

    CC_SYNTHESIZE(cocos2d::CCObject*, userObject, UserObject);
    CC_SYNTHESIZE(int, userData, UserData);

private:
    cocos2d::SEL_CallFuncO selector;
    cocos2d::CCObject * target;
    cocos2d::CCTouchDispatcher * dispatcher;
};

#endif