//
//  DynamicPixelNode.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/23.
//
//

#ifndef DynamicPixelNode_hpp
#define DynamicPixelNode_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ScatPixelDef.hpp"

struct DynamicPixelVertexPormat
{
    cocos2d::Vec3 position;
    cocos2d::Vec4 color;
    cocos2d::Vec3 normal;
    float boneIndex; //实际上是 int，而 vertex attribute不支持 int
};


//动态 PixelNode，每个 Pixel绑定到特定bone index，运行时操控 bone 使得在一个 draw call 内进行多部分 pixel 之间分离的动画，包括旋转、位移、缩放。同时支持 运行时增加 pixel。
//使得一个人物仅使用一个 drawcall，就有头、弓、身体、脚、箭袋、盾、血条，等分离的动画，继而解决性能问题。
class DynamicPixelNode:public cocos2d::Node
{
public:
    CREATE_FUNC(DynamicPixelNode);
    virtual bool init() override;

    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)override;

    void configAddSopx(const std::string& file, int boneIndex, cocos2d::Vec3 relativePosition, bool cutInnerFace = true, bool cutBackFace = true, cocos2d::Vec3 baseRotation = {0,0,0}); //加入某个 sopx 内的 vertex，并且设它的 Bone 为某个值 这个 relativePosition 是此 vertex 序列相对 bone 的偏移 baseRotation 在基础上旋转后再 ADD

    /*
     绝妙的方法，用空的 Node 以及 c2dx 的 action 体系来为我们计算骨骼的 matrix
     */
    void configAction(int boneIndex, cocos2d::Vec3 position, cocos2d::Vec3 rotation, float scaleX, float scaleY, cocos2d::Action* action, bool set = true); //为某个骨骼配置动作，将取消该骨骼之前配置的动作，position 是该 bone 对整个 DynamicPixelNode 的相对位置


    void configMixColor(const cocos2d::Vec4& mixColor); //设置叠色，将用alpha与原色混合。默认alpha为0，显示原色。
    void configMixColorAni(const cocos2d::Vec4& mixColor, float fadeInOutTime, int repeat = 1); //混色动画，用来实现，rival受攻击 repeat -1时 无止尽
    void configClear(); //清空 VBO

    void configSpecialDoubleBoneRotateFirst(int boneIndex, cocos2d::Vec3 rotateion); //弓的特殊要求!!
    //TODO 提供普遍双层 BONE，来应对更高级的 archor 等效果，使得 Bone 控制更灵活

    void update(float dt) override;

protected:
    virtual ~DynamicPixelNode();
    GLuint _vao;
    GLuint _vbo;

    void prepareVertexData();
    void prepareShaders();
    static cocos2d::GLProgramState* _programState;
    cocos2d::CustomCommand _command;
    int _count = 0;
    bool _blend = false;
    int _bufferStoreSize = 50000;
    static constexpr const int BONE_NUM_MAX = 10; //与 VSH 同步
    cocos2d::Node* _bones[BONE_NUM_MAX];
    cocos2d::Mat4 _boneMatrixs[BONE_NUM_MAX];
    int _specialBoneIndex = -1;
    cocos2d::Node* _specialBonePreNode;

    cocos2d::Vec4 _mixColor = {0,0,0,0};
    int _aniMixColorTime = 0;
    float _aniMixColorAlphaStep;
    cocos2d::Vec4 _aniMixColorTarget;
};


#endif /* DynamicPixelNode_hpp */
