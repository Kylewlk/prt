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
    : Scene(ID, width, height, 1, true)
{
    this->camera3d = Camera3D::create();
    this->camera3d->setLockUp(true);
    this->camera2d = Camera2D::create();

    this->shaderPicture = Shader::createByPath("asset/shader/picture.vert", "asset/shader/picture.frag");
    this->shader = Shader::createByPath("asset/shader/model.vert", "asset/shader/model.frag");

    this->roomHdr = Texture::createHDR("asset/room.hdr");

    this->textureHdr = roomHdr;

    this->sphere = Model::create("asset/model/sphere.obj");

    SHLightScene::reset();
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
    else if(this->drawType == kCubeEnvironment)
    {
        drawAxis3D(camera3d->getViewProj(), 100, 2);
        shader->use();
        shader->setUniform("viewProj", camera3d->getViewProj());

        shader->setUniform("lightColor", lightColor);
        shader->setUniform("lightDir", glm::normalize(lightDir));
        shader->setUniform("cameraPos", camera3d->getViewPosition());



        auto mat = math::translate({-120, 0, 0}) * math::scale({100, 100, 100});
        auto normalMat = glm::transpose(glm::inverse(math::Mat3{mat}));
        shader->setUniform("albedo", math::Vec3{1, 1, 1});
        shader->setUniform("model", mat);
        shader->setUniform("normalMatrix", normalMat);
        this->sphere->draw();


        mat = math::translate({120, 0, 0}) * math::scale({100, 100, 100});
        shader->setUniform("albedo", math::Vec3{1, 1, 1});
        shader->setUniform("model", mat);
        shader->setUniform("normalMatrix", normalMat);
        this->sphere->draw();
    }

}

void SHLightScene::drawSettings()
{
    ImGui::ColorEdit3("Light Color", (float*)&lightColor, ImGuiColorEditFlags_Float);
    ImGui::DragFloat3("Light Direction", (float*)&lightDir);

    ImGui::Separator();
    ImGui::RadioButton("HDR Texture", &drawType, kHdrTexture);
    ImGui::RadioButton("Cube Map", &drawType, kCubeEnvironment);
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
