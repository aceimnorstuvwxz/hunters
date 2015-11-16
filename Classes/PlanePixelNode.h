// (C) 2015 Turnro.com

#ifndef PlanePixelNode_hpp
#define PlanePixelNode_hpp

#include "cocos2d.h"
#include "ScatPixelDef.hpp"


struct PlanePixelBatchTuple
{
    cocos2d::Vec3 position;
    float scale;
    std::vector<PixelUnit>* pixels;
};

struct PlanePixelVertexPormat
{
    cocos2d::Vec3 position;
    cocos2d::Vec3 color;
    cocos2d::Vec3 normal;
};


class PlanePixelNode:public cocos2d::Node
{
public:
    CREATE_FUNC(PlanePixelNode);
    virtual bool init() override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)override;
    // 批量组合数据
    void configBatch(const std::vector<PlanePixelBatchTuple>& data, bool cutBack = true);


    // 为场景移动特别设计的功能
    void configMoveLine(float x) {_moveLine = x;};
    void configMoveWidth(float xwidth) {_moveWidth = xwidth;}
protected:
    virtual ~PlanePixelNode();

    bool _stencil = false;
    bool _stenciled = false;
    void prepareVertexData();
    GLuint _vao;
    GLuint _vbo;
    void prepareShaders();
    cocos2d::GLProgramState* _programState = nullptr;
    cocos2d::CustomCommand _command;
    int _count = 0;

    float _moveLine = -100;
    float _moveWidth = 700;

};

#endif /* PlanePixelNode_hpp */
