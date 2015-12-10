//
//  PixelParticleNode.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/9.
//
//

#ifndef PixelParticleNode_hpp
#define PixelParticleNode_hpp

#include <stdio.h>
#include "cocos2d.h"

//简单的3D 粒子系统
/*以 battleRoad 的战斗坐标系作为固定平面，粒子释放出来后就会在这个空间内活动，指定粒子的生存时间，色彩，速度，方向，大小，方向变化，色彩变化，数量。粒子发射器始终会把粒子放到这个固定的空间内活动。预支 buffer 空间，使用双 Buffer，满了后，清空另外一个，然后用另外一个，。交替。*/

struct PixelParticleVertexFormal
{
    cocos2d::Vec3 centerPos;
    cocos2d::Vec3 diffPos;
    cocos2d::Vec4 scaleLife;
    cocos2d::Vec4 color;
    cocos2d::Vec3 speed;
    cocos2d::Vec3 normal;
};

struct PixelParticleBuffer
{
    GLuint _vao;
    GLuint _vbo;
    int _count = 0;
    float _time = 0;
};

class PixelParticleNode:public cocos2d::Node
{
public:
    CREATE_FUNC(PixelParticleNode);
    virtual bool init() override;

    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)override;
    void update(float dt) override;

    void addParticleBatch(int count, float time, float timeVar, cocos2d::Vec3 position, cocos2d::Vec3 positionVar, cocos2d::Vec4 color, cocos2d::Vec4 colorVar, cocos2d::Vec3 speed, cocos2d::Vec3 speedVar, float beginScale, float beginScaleVar, float endScale, float endScaleVar);

protected:

    virtual ~PixelParticleNode();
    void prepareVertexData();
    void prepareShaders();
    PixelParticleBuffer _ppbuffer[2];
    int _activeBufferIndex = 0; //buffer定时切换，在 add batch 时，如果超过预留空间，则直接丢弃
    cocos2d::GLProgramState* _programState = nullptr;
    cocos2d::CustomCommand _command;
    static constexpr int BUFFER_STORGE_SIZE = 50000; //指顶点数目

    static constexpr int N_TMP_BUFFER = 128; //指可容纳粒子数 大于一个帧会产生的粒子数目
    PixelParticleVertexFormal _tmpVertexBuffer[N_TMP_BUFFER*36];
    int _tmpLocalCount = 0;

    void switchBuffer();
    void uploadBuffer();



    void addPerParticle(float time, float timeVar, cocos2d::Vec3 position, cocos2d::Vec3 positionVar, cocos2d::Vec4 color, cocos2d::Vec4 colorVar, cocos2d::Vec3 speed, cocos2d::Vec3 speedVar, float beginScale, float beginScaleVar, float endScale, float endScaleVar);
    float _timeCount = 0;

};

#endif /* PixelParticleNode_hpp */
