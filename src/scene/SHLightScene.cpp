//
// Created by DELL on 2023/10/27.
//

#include "SHLightScene.h"
#include "common/Texture.h"
#include "common/Shader.h"
#include "common/FrameBuffer.h"
#include "common/EventSystem.h"
#include "camera/Camera3D.h"
#include "camera/Camera2D.h"
#include "common/Logger.h"
#include "common/Model.h"

SHLightScene::SHLightScene(int width, int height)
    : Scene(ID, width, height, 4, true)
{
    this->camera3d = Camera3D::create();
    this->camera3d->setLockUp(true);
    this->camera2d = Camera2D::create();

    this->shaderPicture = Shader::create("asset/shader/picture.vert", "asset/shader/picture.frag");
    this->shaderHdrToCubeMap = Shader::create("asset/shader/cubemap.vert", "asset/shader/cubemap_from_hdr.frag");
    this->shaderCubMap = Shader::create("asset/shader/cubemap.vert", "asset/shader/cubemap.frag");
    this->shaderShowSHLight = Shader::create("asset/shader/cubemap.vert", "asset/shader/02ShowSHLight.frag");
    this->shaderCreateSHLight = Shader::createComputeShader("asset/shader/02CreateSHLight.comp");

    this->sphere = Model::create("asset/model/sphere.obj");
    this->cube = Model::create("asset/model/cube.obj");

    this->roomHdr = Texture::createHDR("asset/room.hdr");
    this->roomCubeMap = this->createCubMap(roomHdr);
    this->roomShLight = createShLight(roomCubeMap);
    this->textureHdr = roomHdr;
    this->textureCubeMap = roomCubeMap;
    this->shLight = roomShLight;

    SHLightScene::reset();
}

SHLightScene::~SHLightScene()
{
    if (this->roomShLight != 0)
    {
        glDeleteBuffers(1, &roomShLight);
        roomShLight = 0;
    }

    if (this->skyShLight != 0)
    {
        glDeleteBuffers(1, &skyShLight);
        skyShLight = 0;
    }
}

SceneRef SHLightScene::create()
{
    struct enable_make_shared : public SHLightScene
    {
        enable_make_shared() : SHLightScene(0, 0) {}
    };
    return std::make_shared<enable_make_shared>();
}

void SHLightScene::reset()
{
    this->camera3d->resetView();
    this->camera2d->resetView();

    this->lightColor = {0.9, 0.9, 0.9};
    this->lightDir = {1, 1, 0.3};
}

void SHLightScene::draw()
{
    this->camera3d->setViewSize((float)this->width, (float)this->height);
    this->camera3d->update();
    this->camera2d->setViewSize((float)this->width, (float)this->height);
    this->camera2d->update();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    if (this->drawType == kHdrTexture)
    {
        shaderPicture->use();
        auto mat =math::scale({(float)textureHdr->getWidth() * 0.3f, -(float)textureHdr->getHeight() * 0.3f, 1.0f});
        shaderPicture->setUniform("mvp",  camera2d->getViewProj() * mat);
        shaderPicture->setUniform("color", math::Vec4{1, 1, 1, 1});
        shaderPicture->bindTexture(3, this->textureHdr);
        drawQuad();
    }
    else if(this->drawType == kHdrCubMap)
    {
        this->drawHdrCubeMap();
    }
    else if(this->drawType == kTextureCubeMap)
    {
        this->drawTextureCubeMap();
    }
    else if(this->drawType == kSHLight)
    {
        this->drawShLight();
    }

}

void SHLightScene::drawHdrCubeMap()
{
    drawAxis3D(camera3d->getViewProj(), 50, 2);
    shaderHdrToCubeMap->use();
    shaderHdrToCubeMap->bindTexture("sphericalMap", this->textureHdr);

    auto mat = math::translate({-150, 0, 0}) * math::scale({100, 100, 100});
    shaderHdrToCubeMap->setUniform("viewProj", this->camera3d->getViewProj() * mat);
    this->sphere->draw();


    mat = math::translate({150, 0, 0}) * math::scale({150, 150, 150});
    shaderHdrToCubeMap->setUniform("viewProj", this->camera3d->getViewProj() * mat);
    this->cube->draw();
}

void SHLightScene::drawTextureCubeMap()
{
    drawAxis3D(camera3d->getViewProj(), 50, 2);
    shaderCubMap->use();
    shaderCubMap->bindTexture("cubeMap", this->textureCubeMap);

    auto mat = math::translate({-150, 0, 0}) * math::scale({100, 100, 100});
    shaderCubMap->setUniform("viewProj", this->camera3d->getViewProj() * mat);
    this->sphere->draw();

    mat = math::translate({150, 0, 0}) * math::scale({150, 150, 150});
    shaderCubMap->setUniform("viewProj", this->camera3d->getViewProj() * mat);
    this->cube->draw();
}

void SHLightScene::drawShLight()
{
    drawAxis3D(camera3d->getViewProj(), 50, 2);
    shaderShowSHLight->use();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->shLight);
    shaderShowSHLight->setUniform("shCount", shLevel*shLevel);

    auto mat = math::translate({-150, 0, 0}) * math::scale({100, 100, 100});
    shaderShowSHLight->setUniform("viewProj", this->camera3d->getViewProj() * mat);
    this->sphere->draw();

    mat = math::translate({150, 0, 0}) * math::scale({150, 150, 150});
    shaderShowSHLight->setUniform("viewProj", this->camera3d->getViewProj() * mat);
    this->cube->draw();
}

void SHLightScene::drawSettings()
{
    if(ImGui::RadioButton("HDR Room", &hdrType, 0))
    {
        if (roomHdr == nullptr)
        {
            this->roomHdr = Texture::createHDR("asset/room.hdr");
            this->roomCubeMap = this->createCubMap(roomHdr);
            this->roomShLight =  this->createShLight(roomHdr);
        }
        this->textureHdr = roomHdr;
        this->textureCubeMap = roomCubeMap;
        this->shLight = roomShLight;
    }
    if(ImGui::RadioButton("HDR Sky", &hdrType, 1))
    {
        if (skyHdr == nullptr)
        {
            this->skyHdr = Texture::createHDR("asset/sky.hdr");
            this->skyCubeMap = this->createCubMap(skyHdr);
            this->skyShLight = this->createShLight(skyHdr);
        }
        this->textureHdr = skyHdr;
        this->textureCubeMap = skyCubeMap;
        this->shLight = skyShLight;
    }
    ImGui::Separator();

    ImGui::ColorEdit3("Light Color", (float*)&lightColor, ImGuiColorEditFlags_Float);
    ImGui::DragFloat3("Light Direction", (float*)&lightDir);

    ImGui::Separator();
    ImGui::RadioButton("HDR Texture", &drawType, kHdrTexture);
    ImGui::RadioButton("HDR Cube Map", &drawType, kHdrCubMap);
    ImGui::RadioButton("Texture Cube Map", &drawType, kTextureCubeMap);
    ImGui::RadioButton("SH Light", &drawType, kSHLight);
    ImGui::SliderInt("SH Level", &shLevel, 1, 4);
}

void SHLightScene::onMouseEvent(const MouseEvent* e)
{
    Scene::onMouseEvent(e);
    if (drawType == kHdrTexture)
    {
        cameraMouseEvent(e, this->camera2d.get());
    }
    else
    {
        cameraMouseEvent(e, this->camera3d.get());
    }
}

TextureRef SHLightScene::createCubMap(const TextureRef& hdr)
{
    const int mapSize = 256;
    TextureRef cubeMap = Texture::createCubemap(GL_RGBA16F, mapSize, mapSize, 1);

    FrameBufferRef frameBuffer = FrameBuffer::create(mapSize, mapSize, RenderTarget::kNone, RenderTarget::kNone);

    using namespace math;

    // 参考 https://blog.csdn.net/wlk1229/article/details/85077819
    Mat4 project = math::perspective(glm::radians(90.0f), 1, 0.1, 10.0);
    Mat4 views[]{
        lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 1.0f,  0.0f,  0.0f), Vec3(0.0f, -1.0f,  0.0f)),
        lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(-1.0f,  0.0f,  0.0f), Vec3(0.0f, -1.0f,  0.0f)),
        lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f,  1.0f,  0.0f), Vec3(0.0f,  0.0f,  1.0f)),
        lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f, -1.0f,  0.0f), Vec3(0.0f,  0.0f, -1.0f)),
        lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f,  0.0f,  1.0f), Vec3(0.0f, -1.0f,  0.0f)),
        lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3( 0.0f,  0.0f, -1.0f), Vec3(0.0f, -1.0f,  0.0f))
    };

    glEnable(GL_DEPTH_TEST);
    shaderHdrToCubeMap->use();
    shaderHdrToCubeMap->bindTexture("sphericalMap", hdr);

    for (int i = 0; i < 6; ++i)
    {
        frameBuffer->bind(cubeMap, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+i);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderHdrToCubeMap->setUniform("viewProj", project*views[i]);
        this->cube->draw();
        frameBuffer->unbind();
    }

    cubeMap->setSampler(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
    return cubeMap;
}

GLuint SHLightScene::createShLight(const TextureRef& cubeMap)
{
    GLuint light = 0;

    math::Vec4 lightData[16]{{0, 0, 0, 0}};

    glGenBuffers(1, &light);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, light);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(lightData), lightData, GL_STREAM_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, light);

    shaderCreateSHLight->use();
    shaderHdrToCubeMap->bindTexture("cubeMap", cubeMap);
    glDispatchCompute(64, 64, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS );

    auto data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(lightData, data, sizeof(lightData));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    std::cout << "SH Light: ";
    for (auto l : lightData)
    {
        std::cout << "(" << l.x << ", " << l.y << ", "<< l.z << ")\n";
    }
    std::cout << std::endl;

    return light;
}

