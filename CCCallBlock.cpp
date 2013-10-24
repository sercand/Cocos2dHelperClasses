//
//  CCCallBlock.cpp
//  CCCallBlock
//
//  Created by YangLe on 12-8-26.
//
//

#include "CCCallBlock.h"

using namespace std;
USING_NS_CC;

bool CCCallBlock::initWithBlock(const function<void()> &block) {
    _block = block;
    return true;
}

CCCallBlock * CCCallBlock::create(const function<void()> &block) {
    CCCallBlock *pRet = new CCCallBlock();
    if (pRet && pRet->initWithBlock(block)) {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCObject * CCCallBlock::copyWithZone(CCZone *pZone) {
    CCZone* pNewZone = NULL;
    CCCallBlock* pRet = NULL;
    
    if (pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallBlock*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCCallBlock();
        pZone = pNewZone = new CCZone(pRet);
    }
    
    CCActionInstant::copyWithZone(pZone);
    pRet->initWithBlock(_block);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCCallBlock::update(float time) {
    CC_UNUSED_PARAM(time);
    this->execute();
}

void CCCallBlock::execute() {
    _block();
}
