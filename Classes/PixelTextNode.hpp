//
//  PixelTextNode.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/6.
//
//

#ifndef PixelTextNode_hpp
#define PixelTextNode_hpp

#include <stdio.h>
#include "PixelNode.h"

class PixelTextNode: public PixelNode
{
public:
    CREATE_FUNC(PixelTextNode);
    virtual bool init() override;

    void configText(const std::string& text, float splitWidth = 0); //设置字符串和间距

    // 大小通过node的scale控制，颜色通过PixelNode的api控制。
protected:
};


#endif /* PixelTextNode_hpp */
