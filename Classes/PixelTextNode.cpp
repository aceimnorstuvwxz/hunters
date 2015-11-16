//
//  PixelTextNode.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/6.
//
//

#include "PixelTextNode.hpp"
#include "PixelDataCache.hpp"
bool PixelTextNode::init()
{
    assert(PixelNode::init());
    return true;
}

void PixelTextNode::configText(const std::string& text, float splitWidth)
{
    const float char_width = 3;
    if (text.size() == 0) {
        this->setVisible(false);
    } else {
        this->setVisible(true);
    }
    float totalWidth = text.size()*char_width + (text.size()-1)*splitWidth;
    float posNow = -0.5f * totalWidth;
    std::vector<PixelBatchTuple> batchData;
    for (char c : text) {
        batchData.push_back({{posNow,0,0}, 1.f, PixelDataCache::s()->getChar(c)});
        posNow += char_width + splitWidth;
    }
    this->configBatch(batchData);
}