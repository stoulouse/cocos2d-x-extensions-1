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

#include "CCSwipeGestureRecognizer.h"
#include <iostream>

USING_NS_CC;

bool CCSwipeGestureRecognizer::init()
{
    direction = 0;
	minDistance = kSwipeMinDistance;
	maxDuration = kSwipeMaxDuration;
    return true;
}

CCSwipeGestureRecognizer::~CCSwipeGestureRecognizer()
{
    
}

CCSwipeGestureRecognizerDirection CCSwipeGestureRecognizer::swipeDirection(CCPoint p1, CCPoint p2)
{
    float right = p2.x-p1.x;
    float left = p1.x-p2.x;
    float down = p1.y-p2.y;
    float up = p2.y-p1.y;
    
    if (right > left && right > up && right > down) {
		return kSwipeGestureRecognizerDirectionRight;
    }
    else if (left > up && left > down) {
            return kSwipeGestureRecognizerDirectionLeft;
    }
    else if (up > down) {
            return kSwipeGestureRecognizerDirectionUp;
    }
    else {
            return kSwipeGestureRecognizerDirectionDown;
    }
    
    return kSwipeGestureRecognizerDirectionLeft;
}

bool CCSwipeGestureRecognizer::checkSwipeDirection(CCPoint p1, CCPoint p2, int & dir)
{
    bool right = p2.x-p1.x>=minDistance;
    bool left = p1.x-p2.x>=minDistance;
    bool down = p1.y-p2.y>=minDistance;
    bool up = p2.y-p1.y>=minDistance;
    
    if (right) {
        if ((direction & kSwipeGestureRecognizerDirectionRight) == kSwipeGestureRecognizerDirectionRight) {
            dir = kSwipeGestureRecognizerDirectionRight;
            return true;
        }
    }
    else if (left) {
        if ((direction & kSwipeGestureRecognizerDirectionLeft) == kSwipeGestureRecognizerDirectionLeft) {
            dir = kSwipeGestureRecognizerDirectionLeft;
            return true;
        }
    }
    else if (up) {
        if ((direction & kSwipeGestureRecognizerDirectionUp) == kSwipeGestureRecognizerDirectionUp) {
            dir = kSwipeGestureRecognizerDirectionUp;
            return true;
        }
    }
    else if (down) {
        if ((direction & kSwipeGestureRecognizerDirectionDown) == kSwipeGestureRecognizerDirectionDown) {
            dir = kSwipeGestureRecognizerDirectionDown;
            return true;
        }
    }
    
    return false;
}

bool CCSwipeGestureRecognizer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent)
{
    if (isRecognizing) {
        isRecognizing = false;
        return false;
    }
    
    initialPosition = pTouch->getLocation();
    if (!isPositionBetweenBounds(initialPosition)) return false;
    
    CCTime::gettimeofdayCocos2d(&startTime, NULL);
    
    isRecognizing = true;
    return true;
}

void CCSwipeGestureRecognizer::ccTouchMoved(CCTouch * pTouch, CCEvent * pEvent)
{
    CCPoint finalPosition = pTouch->getLocation();
//    if (!isPositionBetweenBounds(finalPosition)) {
//        isRecognizing = false;
//        return;
//    }
    
    //distance between initial and final point of touch
    float distance = distanceBetweenPoints(initialPosition, finalPosition);
    
    struct cc_timeval currentTime;
    CCTime::gettimeofdayCocos2d(&currentTime, NULL);
    
    double duration = CCTime::timersubCocos2d(&startTime, &currentTime); //duration in milliseconds
    
    //check that minimum distance has been reached
    //check that maximum duration hasn't been reached
    //check if the direction of the swipe is correct
    CCSwipeGestureRecognizerDirection dir = swipeDirection(initialPosition, finalPosition);
	//    if (distance>=minDistance && duration<=maxDuration && checkSwipeDirection(initialPosition,finalPosition,dir))
	{
        CCSwipe * swipe = CCSwipe::create();
        swipe->direction = (CCSwipeGestureRecognizerDirection)dir;
        swipe->location = initialPosition;
		swipe->distance = distance;
		swipe->duration = duration;
		swipe->final = false;
		swipe->completion = distance / minDistance;
        
        gestureRecognized(swipe);
//        if (cancelsTouchesInView) stopTouchesPropagation(createSetWithTouch(pTouch), pEvent); //cancel touch over other views
    }
    
//    isRecognizing = false;
}

void CCSwipeGestureRecognizer::ccTouchEnded(CCTouch * pTouch, CCEvent * pEvent)
{
    CCPoint finalPosition = pTouch->getLocation();
    if (!isPositionBetweenBounds(finalPosition)) {
        isRecognizing = false;
        return;
    }
    
    //distance between initial and final point of touch
    float distance = distanceBetweenPoints(initialPosition, finalPosition);
    
    struct cc_timeval currentTime;
    CCTime::gettimeofdayCocos2d(&currentTime, NULL);
    
    double duration = CCTime::timersubCocos2d(&startTime, &currentTime); //duration in milliseconds
    
    //check that minimum distance has been reached
    //check that maximum duration hasn't been reached
    //check if the direction of the swipe is correct
    CCSwipeGestureRecognizerDirection dir = swipeDirection(initialPosition, finalPosition);
//    if (distance>=minDistance && duration<=maxDuration && checkSwipeDirection(initialPosition,finalPosition,dir))
	{
        CCSwipe * swipe = CCSwipe::create();
        swipe->direction = (CCSwipeGestureRecognizerDirection)dir;
        swipe->location = initialPosition;
		swipe->distance = distance;
		swipe->duration = duration;
		swipe->final = true;
		swipe->completion = distance / minDistance;
        
        gestureRecognized(swipe);
        if (cancelsTouchesInView) stopTouchesPropagation(createSetWithTouch(pTouch), pEvent); //cancel touch over other views
    }
    
    isRecognizing = false;
}
