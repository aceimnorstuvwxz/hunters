//
//  DynamicPixelNode.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/23.
//
//

#include "DynamicPixelNode.hpp"
#include "PixelDataCache.hpp"

USING_NS_CC;


cocos2d::GLProgramState* DynamicPixelNode::_programState = nullptr;

bool DynamicPixelNode::init()
{
    assert(Node::init());

    prepareVertexData();
    if (_programState == nullptr) {
        prepareShaders();
    }

    return true;
}


void DynamicPixelNode::prepareVertexData()
{
    // 主要任务就是产生 VAO/VBO
    _count = 0;

     glGenVertexArrays(1, &_vao);
     GL::bindVAO(_vao);
     glGenBuffers(1, &_vbo);
     glBindBuffer(GL_ARRAY_BUFFER, _vbo);
     //        glBufferData(GL_ARRAY_BUFFER, sizeof(SimplePixelVertexPormat)*_count, _vertexData, GL_STREAM_DRAW);

     // position
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, position));
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, color));
     glEnableVertexAttribArray(2);
     glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, normal));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, boneIndex));


     GL::bindVAO(0);
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     CHECK_GL_ERROR_DEBUG();
}

void DynamicPixelNode::prepareShaders()
{
    auto fileUtils = FileUtils::getInstance();
    auto vertSource = fileUtils->getStringFromFile("3d/dynamic_pixel.vsh");
    auto fragSource = fileUtils->getStringFromFile("3d/dynamic_pixel.fsh");

    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    glprogram->bindAttribLocation("a_positioin", 0);
    glprogram->bindAttribLocation("a_color", 1);
    glprogram->bindAttribLocation("a_cbf_normal", 2);
    glprogram->bindAttribLocation("a_bone_index", 3);

    glprogram->link();
    glprogram->updateUniforms();

    _programState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _programState->retain();
}



void DynamicPixelNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _command.init(_globalZOrder, transform, flags);
    _command.func = CC_CALLBACK_0(DynamicPixelNode::onDraw, this, transform, flags);
    renderer->addCommand(&_command);
}

void DynamicPixelNode::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{

    auto glProgram = _programState->getGLProgram();
    glProgram->use();

//
//    auto loc = glProgram->getUniformLocation("u_cbf_opacity");
//    glProgram->setUniformLocationWith1f(loc, _opacityAsAlpha ? getOpacity()/255.f : 1.f);
//    loc = glProgram->getUniformLocation("u_posexpand");
//    glProgram->setUniformLocationWith1f(loc, _opacityAsPosExpand ? _posexpand*(1.f - getOpacity()/255.f) : 0.f);
//    loc = glProgram->getUniformLocation("u_mixcolor");
//    glProgram->setUniformLocationWith4fv(loc, &(_mixColor.x), 1);
//
//    loc = glProgram->getUniformLocation("u_y_cut");
//    glProgram->setUniformLocationWith1f(loc, _yCut);

    // TODO bone matrix uniform array

    glProgram->setUniformsForBuiltins(transform);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vao);
    }
    else
    {
        // TODO
    }

    if (_blend) {
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);

    glDrawArrays(GL_TRIANGLES, 0, _count);

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    glEnable (GL_BLEND);

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_count);
    CHECK_GL_ERROR_DEBUG();
}

DynamicPixelNode::~DynamicPixelNode()
{
    glDeleteBuffers(1, &_vbo);
    _vbo = 0;

    glDeleteVertexArrays(1, &_vao);
    cocos2d::GL::bindVAO(0);
    _vao = 0;
}

void DynamicPixelNode::update(float dt)
{
}


void DynamicPixelNode::configAddSopx(const std::string& file, int boneIndex, cocos2d::Vec3 relativePosition) //加入某个 sopx 内的 vertex，并且设它的 Bone 为某个值
{

}

void DynamicPixelNode::configAction(int boneIndex, cocos2d::Action* action) //为某个骨骼配置动作，将取消该骨骼之前配置的动作
{

}