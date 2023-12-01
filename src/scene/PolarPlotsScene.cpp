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

    this->shaderColor = Shader::createByPath("asset/shader/01PolarPlots.vert", "asset/shader/01PolarPlots.frag");
    this->shaderShape = Shader::createByPath("asset/shader/01PolarPlotsShape.vert", "asset/shader/01PolarPlotsShape.frag");

//    this->sphere = Model::create("asset/model/sphere.obj");
    this->sphere = Model::create();
    this->updateSphere(64);

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

    this->lightIntensity = 0.8f;
    this->lightDir = {1, 1, 0.3};

    this->positiveColor = math::Vec3{0.3f, 0.62f, 0.7f};
    this->negativeColor = math::Vec3{0.95f, 0.82f, 0.5f};
}

void PolarPlotsScene::draw()
{
    this->camera->setViewSize((float)this->width, (float)this->height);
    this->camera->update();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //    glDisable(GL_MULTISAMPLE);

    auto& shader = magnitudeAsRadius ? this->shaderShape : this->shaderColor;

    shader->use();
    shader->setUniform("viewProj", camera->getViewProj());
    shader->setUniform("lightColor", math::Vec3{lightIntensity, lightIntensity, lightIntensity});
    shader->setUniform("lightDir", glm::normalize(lightDir));
    shader->setUniform("cameraPos", camera->getViewPosition());

    auto matScale = math::scale({40, 40, 40});
    auto normalMat = glm::transpose(glm::inverse(math::Mat3{matScale}));
    shader->setUniform("positive", positiveColor);
    shader->setUniform("negative", negativeColor);
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
    ImGui::SliderFloat("Light Intensity", (float*)&lightIntensity, 0.1, 1.0);
    ImGui::DragFloat3("Light Direction", (float*)&lightDir);

    ImGui::Separator();
    ImGui::ColorEdit3("Positive Color", (float*)&positiveColor, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Negative Color", (float*)&negativeColor, ImGuiColorEditFlags_Float);

    ImGui::Separator();
    ImGui::Checkbox("Magnitude As Radius", &magnitudeAsRadius);
}

void PolarPlotsScene::onMouseEvent(const MouseEvent* e)
{
    Scene::onMouseEvent(e);
    cameraMouseEvent(e, this->camera.get());
}

void PolarPlotsScene::updateSphere(int segment)
{
    auto& vertices = this->sphere->getVertices();
    auto& indices = this->sphere->getIndices();

    vertices.clear();
    vertices.reserve((segment + 1) * (segment + 1));
    float xSegment = math::pi_2 / (float)segment;
    float ySegment = math::pi / (float)segment;
    for (unsigned int y = 0; y <= segment; ++y)
    {
        for (unsigned int x = 0; x <= segment; ++x)
        {
            auto& vertex = vertices.emplace_back();

            vertex.position.x = std::cos((float)x * xSegment) * std::sin((float)y * ySegment);
            vertex.position.y = std::cos((float)y * ySegment);
            vertex.position.z = std::sin((float)x * xSegment) * std::sin((float)y * ySegment);
            vertex.normal = vertex.position;
            vertex.texCoord.x = float(x) / float(segment);
            vertex.texCoord.y = float(y) / float(segment);
        }
    }

    indices.clear();
    indices.reserve(2 * 3 * segment * segment);
    for (unsigned int y = 0; y < segment; ++y)
    {
        auto r1 = y * (segment + 1);
        auto r2 = (y + 1) * (segment + 1);
        for (unsigned int x = 0; x < segment; ++x)
        {
            indices.push_back(r1 + x);
            indices.push_back(r1 + x + 1);
            indices.push_back(r2 + x);

            indices.push_back(r1 + x + 1);
            indices.push_back(r2 + x + 1);
            indices.push_back(r2 + x);
        }
    }

    this->sphere->updateGL();
}
