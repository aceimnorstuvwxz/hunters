// (C) 2015 Turnro.com

#ifndef PixelNode_hpp
#define PixelNode_hpp

#include "cocos2d.h"
#include "ScatPixelDef.hpp"


struct PixelBatchTuple
{
    cocos2d::Vec3 position;
    float scale;
    std::vector<PixelUnit>* pixels;
};

struct PixelVertexPormat
{
    cocos2d::Vec3 position;
    cocos2d::Vec4 color;
    cocos2d::Vec3 normal;
    cocos2d::Vec2 posdiff;
    float seed;
};


class PixelNode:public cocos2d::Node
{
public:
    CREATE_FUNC(PixelNode);
    virtual bool init() override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)override;
    // 从sopx文件加载数据
    void configSopx(const std::string& file);
    // 批量组合数据 有需要再支持，因为会造成有2个地方配置vbo
    void configBatch(const std::vector<PixelBatchTuple>& data);
    void configMixColor(const cocos2d::Vec4& mixColor); //设置叠色，将用alpha与原色混合。默认alpha为0，显示原色。
    void configMixColorAni(const cocos2d::Vec4& mixColor, float fadeInOutTime, int repeat = 1); //混色动画，用来实现，rival受攻击 repeat -1时 无止尽
    void configBlend(bool enable) {_blend = enable;}
    void configOpacityAni(bool alpha, bool posExpand, float posexpandLength=1) {//设置 opacity 动作的表现，默认是仅 alpha
        _opacityAsAlpha = alpha;
        _opacityAsPosExpand = posExpand;
        _posexpand = posexpandLength;
    }
    cocos2d::Rect fetchScreenRect(float offset, cocos2d::Camera* camera); //获取映射到屏幕空间的区域，用于 touch 的事件判定，offset 用来扩展区域，更容易点中
    void configYCut(float cutLine); //用于实现 strenth bar
    void update(float dt) override;
protected:
    virtual ~PixelNode();
    bool _batched = false; //如果是batch，那么vao/vbo是自有的，需要管理好。
    bool _stencil = false;
    bool _stenciled = false;
    void prepareVertexData();
    GLuint _vao;
    GLuint _vbo;
    void prepareShaders();
    cocos2d::GLProgramState* _programState = nullptr;
    cocos2d::CustomCommand _command;
    int _count = 0;

    bool _opacityAsAlpha = true;
    bool _opacityAsPosExpand = false;
    float _posexpand = 0; //散开聚合度，0的时候不散开，1的时候散开1倍，递增


    cocos2d::Vec4 _mixColor = {0,0,0,0};
    bool _blend = false;

    int _aniMixColorTime = 0;
    float _aniMixColorAlphaStep;
    cocos2d::Vec4 _aniMixColorTarget;

    float _xMin;
    float _xMax;
    float _yMin;
    float _yMax;

    float _yCut = std::numeric_limits<float>::max();

};

#endif /* PixelNode_hpp */
