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
    void configBatch(PlanePixelBatchTuple data, bool CutBack);


    // 为场景移动特别设计的功能
    void configMoveLine(float x) {_moveLine = x;};
    void configMoveWidth(float xwidth) {_moveWidth = xwidth;}

    // 为了树木摇晃而设计的，功能
    void configXDiff(float diffRadio); //在 x 方向上，偏移的距离=diffRadio*Y
    void configXDiffAni(float baseDiffRadio, float varyDiffRadio, float interval); //对应的动画，先由当前偏转移动到baseDiff，然后在 base 的地方在一定范围内摇曳， interval 摆动一回合的时间, speed <= 0时取消

    void update(float dt);
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

    float _baseXDiff_target = 0;
    float _baseXDiff_current = 0;
    float _varyXDiff = 0;
};

#endif /* PlanePixelNode_hpp */
