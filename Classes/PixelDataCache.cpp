//
//  PixelDataCache.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/3.
//
//

#include "PixelDataCache.hpp"
#include "PixelNode.h"
#include "format.h"

USING_NS_CC;

PixelDataCache PixelDataCache::_instance;


void PixelDataCache::preload(const std::string& sopxfile)
{
    auto pixels = loadScatPixelFile(sopxfile);

    int count = 0;
    const float half_step = 0.5f;

    auto vertexData = (PixelVertexPormat*)malloc(sizeof(PixelVertexPormat)*36*pixels.size());

    cocos2d::Vec3 cornors[8] = {{1,1,1}, {-1,1,1}, {-1,-1,1}, {1,-1,1}, {1,1,-1}, {-1,1,-1}, {-1,-1,-1}, {1,-1,-1}};

    auto genface = [this, &cornors, half_step, &vertexData, &count](int a, int b, int c, int d, const cocos2d::Color4B& color, const cocos2d::Vec3& relativePos, const cocos2d::Vec3& normal2, float seed){
        Vec2 posdiff = {relativePos.x, relativePos.y};
        vertexData[count].position = cornors[a] * half_step + relativePos;
        vertexData[count].normal = normal2;
        vertexData[count].seed = seed;
        vertexData[count].posdiff = posdiff;
        vertexData[count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[count].position = cornors[b] * half_step + relativePos;
        vertexData[count].normal = normal2;
        vertexData[count].seed = seed;
        vertexData[count].posdiff = posdiff;
        vertexData[count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[count].position = cornors[c]  * half_step + relativePos;
        vertexData[count].normal = normal2;
        vertexData[count].seed = seed;
        vertexData[count].posdiff = posdiff;
        vertexData[count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[count].position = cornors[a] * half_step + relativePos;
        vertexData[count].normal = normal2;
        vertexData[count].seed = seed;
        vertexData[count].posdiff = posdiff;
        vertexData[count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[count].position = cornors[c] * half_step + relativePos;
        vertexData[count].normal = normal2;
        vertexData[count].seed = seed;
        vertexData[count].posdiff = posdiff;
        vertexData[count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[count].position = cornors[d] * half_step + relativePos;
        vertexData[count].normal = normal2;
        vertexData[count].seed = seed;
        vertexData[count].posdiff = posdiff;
        vertexData[count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};
    };

    for (auto pix : pixels) {
        cocos2d::Vec3 relativePos = {static_cast<float>(pix.pos.x), static_cast<float>(pix.pos.y), 0.f};
        float seed = rand_0_1();
        genface(0,1,2,3,pix.color, relativePos, {0,0,1}, seed);
        genface(4,0,3,7,pix.color, relativePos, {1,0,0}, seed);
        genface(5,4,7,6,pix.color, relativePos, {0,0,-1}, seed);
        genface(1,5,6,2,pix.color, relativePos, {-1,0,0}, seed);
        genface(4,5,1,0,pix.color, relativePos, {0,1,0}, seed);
        genface(6,7,3,2,pix.color, relativePos, {0,-1,0}, seed);
    }

    int xMin = std::numeric_limits<int>::max();
    int xMax = std::numeric_limits<int>::min();
    int yMin = std::numeric_limits<int>::max();
    int yMax = std::numeric_limits<int>::min();
    for (auto pix : pixels) {
        //Rect 计算
        xMin = std::min(xMin, pix.pos.x);
        yMin = std::min(yMin, pix.pos.y);
        xMax = std::max(xMax, pix.pos.x);
        yMax = std::max(yMax, pix.pos.y);
    }


    GLuint VAO,VBO;
    glGenVertexArrays(1, &VAO);
    cocos2d::GL::bindVAO(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PixelVertexPormat)*count, vertexData, GL_STREAM_DRAW);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, normal));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, posdiff));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, seed));
    
    cocos2d::GL::bindVAO(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(vertexData);
    CCLOG("pixel data preload %s VAO=%d VBo=%d cnt=%d", sopxfile.c_str(), VAO, VBO, count);
    _abmap[sopxfile] = {VAO,VBO,count,xMin,xMax,yMin,yMax};
}

PixelDataConfig PixelDataCache::get(const std::string& sopxfile)
{
    if (_abmap.count(sopxfile)) {
        return _abmap[sopxfile];
    } else {
        // 禁止动态载入
//        assert(false);//TODO
        preload(sopxfile);
        CCLOG("--->>>dynamic load sopx=%s", sopxfile.c_str());
        return _abmap[sopxfile];
    }
}

PixelDataCache::~PixelDataCache()
{
    for (auto p : _abmap) {
            glDeleteBuffers(1, &p.second.vbo);
            glDeleteVertexArrays(1, &p.second.vao);
            cocos2d::GL::bindVAO(0);
    }
}

void PixelDataCache::preloadChars() //预加载字形文件（字母，数字，符号）
{
    //数字
    for (char c = '0'; c <= '9'; c++) {
        CCLOG("load char %c", c);
        auto pixels = loadScatPixelFile(fmt::sprintf("sopx/chars/0/%c.png.sopx", c));
        _charMap[c] = pixels;
    }

    //小写字母
    for (char c = 'a'; c <= 'z'; c++) {
        CCLOG("load char %c", c);
        auto pixels = loadScatPixelFile(fmt::sprintf("sopx/chars/0/%c.png.sopx", c));
        _charMap[c] = pixels;
    }

    //特殊符号
    _charMap[' '] = loadScatPixelFile("sopx/chars/0/sp_space.png.sopx");
    _charMap['+'] = loadScatPixelFile("sopx/chars/0/sp_add.png.sopx");
    _charMap['$'] = loadScatPixelFile("sopx/chars/0/sp_dollar.png.sopx");
    _charMap['='] = loadScatPixelFile("sopx/chars/0/sp_equal.png.sopx");
    _charMap['!'] = loadScatPixelFile("sopx/chars/0/sp_excla.png.sopx");
    _charMap['-'] = loadScatPixelFile("sopx/chars/0/sp_minus.png.sopx");
    _charMap['%'] = loadScatPixelFile("sopx/chars/0/sp_perscent.png.sopx");
    _charMap['/'] = loadScatPixelFile("sopx/chars/0/sp_slash.png.sopx");
    _charMap['?'] = loadScatPixelFile("sopx/chars/0/sp_why.png.sopx");
    _charMap['#'] = loadScatPixelFile("sopx/chars/0/sp_degree.png.sopx");

}

std::vector<PixelUnit>* PixelDataCache::getChar(char c)
{
    if (_charMap.count(c)) {
        return &_charMap[c];
    } else {
        CCLOG("----->>>> char '%c' is not int charmap  <<<<----------",c);
        assert(false);
    }
}



// RAW pixel Data cache
void PixelDataCache::loadRawPixels(const std::string& sopxfile)
{
    _rawPixelData[sopxfile] = loadScatPixelFile(sopxfile);

}

void PixelDataCache::clearRawCache()
{
    _rawPixelData.clear();
}

std::vector<PixelUnit>* PixelDataCache::getRawPixels(const std::string& sopxfile)
{
    if (_rawPixelData.count(sopxfile) == 0) {
        loadRawPixels(sopxfile);
    }

    return &(_rawPixelData[sopxfile]);
}