// (C) 2015 Arisecbf

#ifndef RoadPlane_hpp
#define RoadPlane_hpp

#include "cocos2d.h"

struct RoadPlaneVertexDataFormat
{
    cocos2d::Vec3 position;
    cocos2d::Vec2 texcoord;
};


class RoadPlane:public cocos2d::Node
{
public:
    CREATE_FUNC(RoadPlane);
    virtual bool init() override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)override;

    // 注意create后，必须马上configImage
    void configImage(const std::string file); // 可以实时
    void configStenciled(bool enable); // 用来实现stealth潜行，stenciled是潜行背景的机能开关。
    void configBlend(bool enable) {_blend = enable;}

    cocos2d::Size getContentSize() {return _texture->getContentSize();}

protected:
//    bool _stencil = false;
    bool _blend = false;
    virtual ~RoadPlane();
    bool _stenciled = false;
    void prepareVertexData();
    GLuint _vao;
    GLuint _vbo;
    void prepareShaders();
    cocos2d::GLProgramState* _programState = nullptr;
    cocos2d::CustomCommand _command;
    cocos2d::Texture2D* _texture = nullptr;

    //暂时按简单的实现，不实现网格化
    RoadPlaneVertexDataFormat _vertexData[6];
    const int _count = 6;
};

#endif /* RoadPlane_hpp */
