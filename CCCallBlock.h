//
//  CCCallBlock.h
//  CCCallBlock
//
//  Created by YangLe on 12-8-26.
//
//

#ifndef __CCCallBlock__
#define __CCCallBlock__

#include "cocos2d.h"

class CCCallBlock : public cocos2d::CCActionInstant {
public:
    static CCCallBlock * create(const std::function<void()> &block);
    cocos2d::CCObject * copyWithZone(cocos2d::CCZone *pZone);
    void update(float time);
    void execute();
private:
    bool initWithBlock(const std::function<void()> &block);
private:
    std::function<void()> _block;
};

#endif