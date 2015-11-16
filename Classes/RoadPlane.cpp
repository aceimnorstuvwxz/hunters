// (C) 2015 Arisecbf

#include "RoadPlane.h"

USING_NS_CC;

bool RoadPlane::init()
{
    assert(Node::init());

    if (_programState == nullptr) {
        prepareVertexData();
        prepareShaders();
    }

    return true;
}



void RoadPlane::prepareVertexData()
{
    _vertexData[0].position = {0.5,0.5,0.0};
    _vertexData[0].texcoord = {1.0,0.0};
    _vertexData[1].position = {-0.5,0.5,0.0};
    _vertexData[1].texcoord = {0.0,0.0};
    _vertexData[2].position = {-0.5,-0.5,0.0};
    _vertexData[2].texcoord = {0.0,1.0};
    _vertexData[3].position = {0.5,0.5,0.0};
    _vertexData[3].texcoord = {1.0,0.0};
    _vertexData[4].position = {-0.5,-0.5,0.0};
    _vertexData[4].texcoord = {0.0,1.0};
    _vertexData[5].position = {0.5,-0.5,0.0};
    _vertexData[5].texcoord = {1.0,1.0};
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        glGenVertexArrays(1, &_vao);
        GL::bindVAO(_vao);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(RoadPlaneVertexDataFormat)*_count, _vertexData, GL_STREAM_DRAW);
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RoadPlaneVertexDataFormat), (GLvoid *)offsetof(RoadPlaneVertexDataFormat, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(RoadPlaneVertexDataFormat), (GLvoid *)offsetof(RoadPlaneVertexDataFormat, texcoord));

        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
    else
    {
        //TODO
        assert(false);
    }

    CHECK_GL_ERROR_DEBUG();
}

void RoadPlane::prepareShaders()
{
    auto fileUtils = FileUtils::getInstance();
    auto vertSource = fileUtils->getStringFromFile("3d/road_plane.vsh");
    auto fragSource = fileUtils->getStringFromFile("3d/road_plane.fsh");

    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    glprogram->bindAttribLocation("a_positioin", 0);
    glprogram->bindAttribLocation("a_texcoord", 1);

    glprogram->link();
    glprogram->updateUniforms();

    _programState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _programState->retain();
}



void RoadPlane::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _command.init(_globalZOrder, transform, flags);
    _command.func = CC_CALLBACK_0(RoadPlane::onDraw, this, transform, flags);
    renderer->addCommand(&_command);
}

void RoadPlane::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{

    auto glProgram = _programState->getGLProgram();
    glProgram->use();

    auto loc = glProgram->getUniformLocation("u_size");
    glProgram->setUniformLocationWith2f(loc, _texture->getContentSize().width, _texture->getContentSize().height);

    loc = glProgram->getUniformLocation("u_cbf_opacity");
    glProgram->setUniformLocationWith1f(loc, getOpacity()*1.f/255);
    loc = glProgram->getUniformLocation("u_texture");
    glProgram->setUniformLocationWith1i(loc, 0);
    GL::bindTexture2DN(0, _texture->getName());



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
    if (_stenciled) {
        //潜行效果
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
        glStencilMask(0x00); // Don't write anything to stencil buffer
        glDepthMask(GL_TRUE); // Write to depth buffer
    }

    glDrawArrays(GL_TRIANGLES, 0, _count);


    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    glDisable(GL_STENCIL_TEST);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    glEnable (GL_BLEND);

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_count);
    CHECK_GL_ERROR_DEBUG();
}

void RoadPlane::configImage(const std::string file) // 可以实时
{
    _texture = Director::getInstance()->getTextureCache()->addImage(file);
}

RoadPlane::~RoadPlane()
{
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (cocos2d::Configuration::getInstance()->supportsShareableVAO())
    {
        glDeleteVertexArrays(1, &_vao);
        cocos2d::GL::bindVAO(0);
        _vao = 0;
    }
}

