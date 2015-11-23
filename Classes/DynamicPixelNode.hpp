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


//动态 PixelNode，每个 Pixel绑定到特定bone index，运行时操控 bone 使得在一个 draw call 内进行多部分 pixel 之间分离的动画，包括旋转、位移、缩放。同时支持 运行时增加 pixel。
//使得一个人物仅使用一个 drawcall，就有头、弓、身体、脚、箭袋、盾、血条，等分离的动画，继而解决性能问题。
class DynamicPixelNode:public cocos2d::Node
{
public:
    CREATE_FUNC(DynamicPixelNode);
    virtual bool init() override;

    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)override;

    void configAddSopx(const std::string& file, int boneIndex, cocos2d::Vec3 relativePosition); //加入某个 sopx 内的 vertex，并且设它的 Bone 为某个值

    /*
     绝妙的方法，用空的 Node 以及 c2dx 的 action 体系来为我们计算骨骼的 matrix
     */
    void configAction(int boneIndex, cocos2d::Action* action); //为某个骨骼配置动作，将取消该骨骼之前配置的动作

    void update(float dt) override;

protected:
    virtual ~DynamicPixelNode();
    void prepareVertexData();
    GLuint _vao;
    GLuint _vbo;
    void prepareShaders();
    cocos2d::GLProgramState* _programState = nullptr;
    cocos2d::CustomCommand _command;
    int _count = 0;

};


#endif /* DynamicPixelNode_hpp */
