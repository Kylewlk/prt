//
// Created by DELL on 2023/10/27.
//

#include "Model3DScene.h"
#include "common/Texture.h"
#include "common/Shader.h"
#include "common/FrameBuffer.h"
#include "common/EventSystem.h"
#include "camera/Camera3D.h"
#include "common/Logger.h"
#include "common/Model.h"

Model3DScene::Model3DScene(int width, int height)
    : Scene(ID, width, height, 4)
{
    this->camera = Camera3D::create();
    this->camera->setLockUp(true);

    this->shader = Shader::create("asset/shader/model.vert", "asset/shader/model.frag");

    this->sphere = Model::create("asset/model/sphere.obj");
    this->cube = Model::create("asset/model/cube.obj");
    this->plane = Model::create("asset/model/plane.obj");

    Model3DScene::reset();
}

SceneRef Model3DScene::create()
{
    struct enable_make_shared : public Model3DScene
    {
        enable_make_shared() : Model3DScene(0, 0) {}
    };
    return std::make_shared<enable_make_shared>();
}

void Model3DScene::reset()
{
    this->camera->resetView();
    this->camera->round(-20, 0);
    this->camera->round(0, -60);

    this->lightColor = {0.9, 0.9, 0.9};
    this->lightDir = {1, 1, 0.3};

    this->sphereColor = math::Vec3{0.3, 0.7, 0.5};
    this->cubeColor = math::Vec3{0.2, 0.6, 0.8};
    this->planeColor = math::Vec3{0.4, 0.4, 0.4};
}

void Model3DScene::draw()
{
    this->camera->setViewSize((float)this->width, (float)this->height);
    this->camera->update();

    shader->use();
    shader->setUniform("viewProj", camera->getViewProj());

    shader->setUniform("lightColor", lightColor);
    shader->setUniform("lightDir", glm::normalize(lightDir));
    shader->setUniform("cameraPos", camera->getViewPosition());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //    glDisable(GL_MULTISAMPLE);

    auto mat = math::translate({120, 1, 0}) * math::scale({100, 100, 100});
    auto normalMat = glm::transpose(glm::inverse(math::Mat3{mat}));
    shader->setUniform("albedo", cubeColor);
    shader->setUniform("model", mat);
    shader->setUniform("normalMatrix", normalMat);
    this->cube->draw();


    mat = math::translate({-120, 50, 0}) * math::scale({100, 100, 100});
    normalMat = glm::transpose(glm::inverse(math::Mat3{mat}));
    shader->setUniform("albedo", sphereColor);
    shader->setUniform("model", mat);
    shader->setUniform("normalMatrix", normalMat);
    this->sphere->draw();


    mat = math::translate({0, -50, 0}) * math::scale({500, 500, 500});
    normalMat = glm::transpose(glm::inverse(math::Mat3{mat}));
    shader->setUniform("albedo", planeColor);
    shader->setUniform("model", mat);
    shader->setUniform("normalMatrix", normalMat);
    this->plane->draw();

    drawAxis3D(camera->getViewProj(), 100, 2);

    math::Vec3 points[2]{{0, 0, 0}, {200, 0, 0}};
    static float time = 0;
    time += (float)TimeSys::getDelta() * 0.5f;
    time = std::fmod(time, math::pi_2);
    points[1] = {std::sin(time) * 200, 0, std::cos(time) * 200};
    drawPoints(points, 2, camera->getViewProj(), {0,1,1,1}, 10);
    drawLines(points, 2, LineType::strip, camera->getViewProj(), {1, 1, 0, 1}, 4);

    std::vector<math::Vec2> points2D;
    this->camera->worldToScreen(points, sizeof(points)/sizeof(points[2]), points2D);
    drawIndicator(points2D.data(), (int)points2D.size(), {1.0, 0.4, 0.3, 1});
}

void Model3DScene::drawSettings()
{
    ImGui::ColorEdit3("Light Color", (float*)&lightColor, ImGuiColorEditFlags_Float);
    ImGui::DragFloat3("Light Direction", (float*)&lightDir);

    ImGui::Separator();
    ImGui::ColorEdit3("Sphere Color", (float*)&sphereColor, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Cube Color", (float*)&cubeColor, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Plane Color", (float*)&planeColor, ImGuiColorEditFlags_Float);
}

void Model3DScene::onMouseEvent(const MouseEvent* e)
{
    Scene::onMouseEvent(e);
    cameraMouseEvent(e, this->camera.get());
}
