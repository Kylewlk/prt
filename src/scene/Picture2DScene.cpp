//
// Created by DELL on 2023/10/26.
//

#include "Picture2DScene.h"
#include "common/Texture.h"
#include "common/Shader.h"
#include "common/FrameBuffer.h"
#include "common/EventSystem.h"
#include "camera/Camera2D.h"
#include "common/Logger.h"

Picture2DScene::Picture2DScene(int width, int height)
    : Scene(ID, width, height, 4)
{
    this->texture = Texture::create("asset/Lenna.png", false, true);
    this->shader = Shader::create("asset/shader/picture.vert", "asset/shader/picture.frag");
    this->camera = Camera2D::create();

    Picture2DScene::reset();
}

SceneRef Picture2DScene::create()
{
    struct enable_make_shared : public Picture2DScene
    {
        enable_make_shared() : Picture2DScene(0, 0) {}
    };
    return std::make_shared<enable_make_shared>();
}

void Picture2DScene::reset()
{
    color = {1.0f, 1.0f, 1.0f, 1.0f};
    rotation = 0.0f;
}

void Picture2DScene::draw()
{
    this->camera->setViewSize((float)this->width, (float)this->height);
    this->camera->update();

    auto mat = camera->getViewProj() * math::rotateDegree(rotation, math::Z_Axis);

    // Y 轴乘以 -1 是为了让 纹理坐标的原点在左上角，Y 轴向下
    mat = mat * math::scale({(float)texture->getWidth() * 0.5f, (float)texture->getHeight() * -0.5f, 1.0f});

    glEnable(GL_MULTISAMPLE);
    shader->use();
    shader->setUniform("mvp", mat);
    shader->setUniform("color", math::Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    shader->bindTexture("tex", this->texture);
    drawQuad();

    drawAxis2D(camera->getViewProj(), 100, 2);

    math::Vec2 points[2]{{0, 0}, {200, 0}};
    static float time = 0;
    time += (float)TimeSys::getDelta() * 0.2f;
    time = std::fmod(time, math::pi_2);
    points[1] = {std::cos(time) * 200, std::sin(time) * 200};
    drawPoints(points, 2, camera->getViewProj(), {0,1,1,1}, 10);
    drawLines(points, 2, LineType::strip, camera->getViewProj(), {1, 1, 0, 1}, 4);

    auto pos = this->camera->worldToScreen({points[1], 0});
    drawString(pos, {0.3, 0.5, 1, 1}, std::to_string(glm::degrees(time)));
}

void Picture2DScene::drawSettings()
{
    ImGui::ColorEdit4("Color", (float*)&color);
    ImGui::DragFloat("Rotate", &rotation);
}

void Picture2DScene::onMouseEvent(const MouseEvent* e)
{
    Scene::onMouseEvent(e);
    cameraMouseEvent(e, this->camera.get());
}
