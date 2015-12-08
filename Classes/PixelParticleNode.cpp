//
//  PixelParticleNode.cpp
//  hunters
//
//  Created by chenbingfeng on 15/12/9.
//
//

#include "PixelParticleNode.hpp"
USING_NS_CC;

bool PixelParticleNode::init()
{
    assert(Node::init());

    if (_programState == nullptr) {
        prepareVertexData();
        prepareShaders();
    }

    return true;
}



void PixelParticleNode::prepareVertexData()
{
    for (int i = 0; i < 2; i++) {
        auto& buffer = _ppbuffer[i];
        buffer._count = 0;

        glGenVertexArrays(1, &buffer._vao);
        GL::bindVAO(buffer._vao);
        glGenBuffers(1, &buffer._vbo);
        glBindBuffer(GL_ARRAY_BUFFER, buffer._vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(PixelParticleVertexFormal) * BUFFER_STORGE_SIZE, nullptr, GL_STREAM_DRAW); //预支空间
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, srcPos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, desPos));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, scaleLife));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, color));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, speed));

        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR_DEBUG();
    }
}


void PixelParticleNode::prepareShaders()
{
    auto fileUtils = FileUtils::getInstance();
    auto vertSource = fileUtils->getStringFromFile("3d/pixel_particle.vsh");
    auto fragSource = fileUtils->getStringFromFile("3d/pixel_particle.fsh");

    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    glprogram->bindAttribLocation("a_cbf_src_pos", 0);
    glprogram->bindAttribLocation("a_cbf_des_pos", 1);
    glprogram->bindAttribLocation("a_cbf_scale_life", 2);
    glprogram->bindAttribLocation("a_cbf_color", 3);
    glprogram->bindAttribLocation("a_cbf_speed", 4);

    glprogram->link();
    glprogram->updateUniforms();

    _programState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _programState->retain();
}

void PixelParticleNode::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
    auto glProgram = _programState->getGLProgram();
    glProgram->use();

    for (int i = 0; i < 2; i++) {
        auto& buffer = _ppbuffer[i];

        auto loc = glProgram->getUniformLocation("u_cbf_time");
        glProgram->setUniformLocationWith1f(loc, buffer._time);

        glProgram->setUniformsForBuiltins(transform);

        glBindBuffer(GL_ARRAY_BUFFER, buffer._vbo);
        GL::bindVAO(buffer._vao);
//
//        if (_blend) {
//            glEnable (GL_BLEND);
//            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        } else {
            glDisable(GL_BLEND);
//        }

        glEnable(GL_CULL_FACE);
        // shadow cover打开depth test同时在fsh中对a为0的进行discard，以保证重合交叠处不会交叠而加深。
        glEnable(GL_DEPTH_TEST);
        glDepthMask(true);
//        if (_stencil) {
//
//            glEnable(GL_STENCIL_TEST);
//            // Draw floor
//            glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
//            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//            glStencilMask(0xFF); // Write to stencil buffer
//            glDepthMask(GL_FALSE); // Don't write to depth buffer
//            glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
//        }
//        if (_stenciled) {
//
//            glEnable(GL_STENCIL_TEST);
//            glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
//            glStencilMask(0x00); // Don't write anything to stencil buffer
//            glDepthMask(GL_TRUE); // Write to depth buffer
//        }

        glDrawArrays(GL_TRIANGLES, 0, buffer._count);


        GL::bindVAO(0);

//        glDisable(GL_STENCIL_TEST);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(false);
        glEnable (GL_BLEND);
        
        CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,buffer._count);
        CHECK_GL_ERROR_DEBUG();
    }
}

void PixelParticleNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _command.init(_globalZOrder, transform, flags);
    _command.func = CC_CALLBACK_0(PixelParticleNode::onDraw, this, transform, flags);
    renderer->addCommand(&_command);
}

void PixelParticleNode::update(float dt)
{

}

PixelParticleNode::~PixelParticleNode()
{
    for (int i = 0; i < 2; i++) {
        auto& buffer = _ppbuffer[i];
        glDeleteBuffers(1, &buffer._vbo);
        glDeleteVertexArrays(1, &buffer._vao);
        cocos2d::GL::bindVAO(0);
    }
}

void PixelParticleNode::addParticleBatch(int count, float time, float timeVar, cocos2d::Vec3 position, cocos2d::Vec3 positionVar, cocos2d::Vec4 color, cocos2d::Vec4 colorVar, cocos2d::Vec3 speed, cocos2d::Vec3 speedVar, float beginScale, float beginScaleVar, float endScale, float endScaleVar)
{
    assert(count <= N_TMP_BUFFER);
    auto& buffer = _ppbuffer[_activeBufferIndex];
    if (buffer._count + count*36 > BUFFER_STORGE_SIZE) {
        //超载，丢弃
        return;
    }
    int localCount = 0;
    auto genCube = [&localCount,=this](float time, cocos2d::Vec3 position, cocos2d::Vec4 color, cocos2d::Vec3 speed, float beginScale, float endScale) {
        
    }
    for (int i = 0; i < count; i++) {
    }

}
