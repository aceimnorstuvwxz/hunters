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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, centerPos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, diffPos));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, scaleLife));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, color));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, speed));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(PixelParticleVertexFormal), (GLvoid *)offsetof(PixelParticleVertexFormal, normal));

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
    glprogram->bindAttribLocation("a_cbf_center_pos", 0);
    glprogram->bindAttribLocation("a_cbf_diff_pos", 1);
    glprogram->bindAttribLocation("a_cbf_scale_life", 2);
    glprogram->bindAttribLocation("a_cbf_color", 3);
    glprogram->bindAttribLocation("a_cbf_speed", 4);
    glprogram->bindAttribLocation("a_cbf_normal", 5);

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
    _tmpLocalCount = 0;
    for (int i = 0; i < count; i++) {
        addPerParticle(time, timeVar, position, positionVar, color, colorVar, speed, speedVar, beginScale, beginScaleVar, endScale, endScaleVar);
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer._vbo);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(PixelParticleVertexFormal)*buffer._count, sizeof(PixelParticleVertexFormal)*_tmpLocalCount, _tmpVertexBuffer);
    buffer._count += _tmpLocalCount;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PixelParticleNode::addPerParticle(float time, float timeVar, cocos2d::Vec3 position, cocos2d::Vec3 positionVar, cocos2d::Vec4 color, cocos2d::Vec4 colorVar, cocos2d::Vec3 speed, cocos2d::Vec3 speedVar, float beginScale, float beginScaleVar, float endScale, float endScaleVar)
{

    // 8 point
    const float half_step = 0.5f;

    cocos2d::Vec3 cornors[8] = {{1,1,1}, {-1,1,1}, {-1,-1,1}, {1,-1,1}, {1,1,-1}, {-1,1,-1}, {-1,-1,-1}, {1,-1,-1}};
    Node* node = Node::create();
    node->setRotation3D({random(0.f, 360.f),random(0.f, 360.f),random(0.f, 360.f)});
    auto mat = node->getNodeToParentTransform();

    auto genface = [this, &cornors, half_step, &mat](int a, int b, int c, int d, const cocos2d::Vec4& color, const cocos2d::Vec3& centerPos, const cocos2d::Vec4& scaleLife, const Vec3& speed, const Vec3& normal){
        _tmpVertexBuffer[_tmpLocalCount].centerPos = centerPos;
        _tmpVertexBuffer[_tmpLocalCount].diffPos = cornors[a] * half_step;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].diffPos);
        _tmpVertexBuffer[_tmpLocalCount].normal = normal;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].normal);
        _tmpVertexBuffer[_tmpLocalCount].color = color;
        _tmpVertexBuffer[_tmpLocalCount].speed = speed;
        _tmpVertexBuffer[_tmpLocalCount++].scaleLife = scaleLife;


        _tmpVertexBuffer[_tmpLocalCount].centerPos = centerPos;
        _tmpVertexBuffer[_tmpLocalCount].diffPos = cornors[b] * half_step;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].diffPos);
        _tmpVertexBuffer[_tmpLocalCount].normal = normal;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].normal);
        _tmpVertexBuffer[_tmpLocalCount].color = color;
        _tmpVertexBuffer[_tmpLocalCount].speed = speed;
        _tmpVertexBuffer[_tmpLocalCount++].scaleLife = scaleLife;


        _tmpVertexBuffer[_tmpLocalCount].centerPos = centerPos;
        _tmpVertexBuffer[_tmpLocalCount].diffPos = cornors[c] * half_step;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].diffPos);
        _tmpVertexBuffer[_tmpLocalCount].normal = normal;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].normal);
        _tmpVertexBuffer[_tmpLocalCount].color = color;
        _tmpVertexBuffer[_tmpLocalCount].speed = speed;
        _tmpVertexBuffer[_tmpLocalCount++].scaleLife = scaleLife;

        _tmpVertexBuffer[_tmpLocalCount].centerPos = centerPos;
        _tmpVertexBuffer[_tmpLocalCount].diffPos = cornors[a] * half_step;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].diffPos);
        _tmpVertexBuffer[_tmpLocalCount].normal = normal;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].normal);
        _tmpVertexBuffer[_tmpLocalCount].color = color;
        _tmpVertexBuffer[_tmpLocalCount].speed = speed;
        _tmpVertexBuffer[_tmpLocalCount++].scaleLife = scaleLife;


        _tmpVertexBuffer[_tmpLocalCount].centerPos = centerPos;
        _tmpVertexBuffer[_tmpLocalCount].diffPos = cornors[c] * half_step;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].diffPos);
        _tmpVertexBuffer[_tmpLocalCount].normal = normal;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].normal);
        _tmpVertexBuffer[_tmpLocalCount].color = color;
        _tmpVertexBuffer[_tmpLocalCount].speed = speed;
        _tmpVertexBuffer[_tmpLocalCount++].scaleLife = scaleLife;


        _tmpVertexBuffer[_tmpLocalCount].centerPos = centerPos;
        _tmpVertexBuffer[_tmpLocalCount].diffPos = cornors[d] * half_step;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].diffPos);
        _tmpVertexBuffer[_tmpLocalCount].normal = normal;
        mat.transformVector(&_tmpVertexBuffer[_tmpLocalCount].normal);
        _tmpVertexBuffer[_tmpLocalCount].color = color;
        _tmpVertexBuffer[_tmpLocalCount].speed = speed;
        _tmpVertexBuffer[_tmpLocalCount++].scaleLife = scaleLife;
    };

    auto dColor = color + random(-1.f, 1.f)*colorVar;
    auto dPos = position + Vec3{random(-1.f, 1.f)*positionVar.x,random(-1.f, 1.f)*positionVar.y,random(-1.f, 1.f)*positionVar.z};
    auto dSpeed = speed + random(-1.f, 1.f)*speedVar;
    Vec4 dScleLife = {beginScale + random(-1.f, 1.f)*beginScaleVar, endScale + random(-1.f, 1.f)*endScaleVar, _ppbuffer[_activeBufferIndex]._time, _ppbuffer[_activeBufferIndex]._time + time + random(0.f, 1.f)*timeVar};

    genface(0,1,2,3,dColor, dPos, dScleLife, dSpeed, {0,0,1});
    genface(5,4,7,6,dColor, dPos, dScleLife, dSpeed, {0,0,-1});

    genface(4,0,3,7,dColor, dPos, dScleLife, dSpeed, {1,0,0});
    genface(1,5,6,2,dColor, dPos, dScleLife, dSpeed,{-1,0,0});
    genface(4,5,1,0,dColor, dPos, dScleLife, dSpeed, {0,1,0});
    genface(6,7,3,2,dColor, dPos, dScleLife, dSpeed, {0,-1,0});
}
