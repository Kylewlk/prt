//
// Created by DELL on 2023/10/27.
//

#include "PolarPlotsScene.h"
#include "common/Texture.h"
#include "common/Shader.h"
#include "common/FrameBuffer.h"
#include "common/EventSystem.h"
#include "camera/Camera3D.h"
#include "common/Logger.h"
#include "common/Model.h"

PolarPlotsScene::PolarPlotsScene(int width, int height)
    : Scene(ID, width, height, 4)
{
    this->camera = Camera3D::create();
    this->camera->setLockUp(true);

    this->shader = Shader::createByPath("asset/shader/01PolarPlots.vert", "asset/shader/01PolarPlots.frag");

    this->sphere = Model::create("asset/model/sphere.obj");

    PolarPlotsScene::reset();
}

SceneRef PolarPlotsScene::create()
{
    struct enable_make_shared : public PolarPlotsScene
    {
        enable_make_shared() : PolarPlotsScene(0, 0) {}
    };
    return std::make_shared<enable_make_shared>();
}

void PolarPlotsScene::reset()
{
    this->camera->resetView();

    this->lightColor = {0.9, 0.9, 0.9};
    this->lightDir = {1, 1, 0.3};

    this->sphereColor = math::Vec3{0.3, 0.7, 0.5};
}

void PolarPlotsScene::draw()
{
    this->camera->setViewSize((float)this->width, (float)this->height);
    this->camera->update();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //    glDisable(GL_MULTISAMPLE);

    shader->use();
    shader->setUniform("viewProj", camera->getViewProj());
    shader->setUniform("lightColor", lightColor);
    shader->setUniform("lightDir", glm::normalize(lightDir));
    shader->setUniform("cameraPos", camera->getViewPosition());

    auto matScale = math::scale({40, 40, 40});
    auto normalMat = glm::transpose(glm::inverse(math::Mat3{matScale}));
    shader->setUniform("positive", math::Vec3{1, 0, 0});
    shader->setUniform("negative", math::Vec3{0, 1, 1});
    shader->setUniform("normalMatrix", normalMat);

    math::Mat4 model;
    math::Vec2i lm{0, 0};

    shader->setUniform("lm", lm);
    model = math::translate(math::Vec3{0, 150, 0}) ;
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {1, 0};
    model = math::translate(math::Vec3{0, 50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {1, 1};
    model = math::translate(math::Vec3{100, 50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {1, -1};
    model = math::translate(math::Vec3{-100, 50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {2, 0};
    model = math::translate(math::Vec3{0, -50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {2, 1};
    model = math::translate(math::Vec3{100, -50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {2, 2};
    model = math::translate(math::Vec3{200, -50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {2, -1};
    model = math::translate(math::Vec3{-100, -50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {2, -2};
    model = math::translate(math::Vec3{-200, -50, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {3, 0};
    model = math::translate(math::Vec3{0, -150, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {3, 1};
    model = math::translate(math::Vec3{100, -150, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {3, 2};
    model = math::translate(math::Vec3{200, -150, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {3, 3};
    model = math::translate(math::Vec3{300, -150, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {3, -1};
    model = math::translate(math::Vec3{-100, -150, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {3, -2};
    model = math::translate(math::Vec3{-200, -150, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

    lm = {3, -3};
    model = math::translate(math::Vec3{-300, -150, 0}) ;
    shader->use();
    shader->setUniform("lm", lm);
    shader->setUniform("model", model * matScale);
    this->sphere->draw();
    drawAxis3D(camera->getViewProj()*model, 50, 2);

}

void PolarPlotsScene::drawSettings()
{
    ImGui::ColorEdit3("Light Color", (float*)&lightColor, ImGuiColorEditFlags_Float);
    ImGui::DragFloat3("Light Direction", (float*)&lightDir);

    ImGui::Separator();
    ImGui::ColorEdit3("Sphere Color", (float*)&sphereColor, ImGuiColorEditFlags_Float);
}

void PolarPlotsScene::onMouseEvent(const MouseEvent* e)
{
    Scene::onMouseEvent(e);
    cameraMouseEvent(e, this->camera.get());
}
