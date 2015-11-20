//
//  PixelDataCache.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#ifndef PixelDataCache_hpp
#define PixelDataCache_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ScatPixelDef.hpp"

struct PixelDataConfig
{
    GLuint vao;
    GLuint vbo;
    int count;
    int xMin;
    int xMax;
    int yMin;
    int yMax;
};

class PixelDataCache
{
public:
    static PixelDataCache* s(){return &_instance;};

    /* 所有使用的sopx都是需要preload的，而禁止在gaming时载入 */

    void preload(const std::string& sopxfile);

    PixelDataConfig get(const std::string& sopxfile); // VAO VBO

    // 除了普通的sopx缓存之外，还有字形的缓存，用来支持PixelTextNode

    void preloadChars(); //预加载字形文件（字母，数字，符号）
    std::vector<PixelUnit>* getChar(char c);

protected:
    virtual ~PixelDataCache();
    static PixelDataCache _instance;


    std::unordered_map<std::string, PixelDataConfig> _abmap;
    std::unordered_map<char, std::vector<PixelUnit>> _charMap;
};

#endif /* PixelDataCache_hpp */
