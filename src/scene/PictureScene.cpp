//
// Created by wlk12 on 2023/8/6.
//

#include "PictureScene.h"
#include "common/Texture.h"
#include "common/Shader.h"
#include "common/FrameBuffer.h"
#include "common/EventSystem.h"
#include "camera/Camera2D.h"
#include "common/Logger.h"

PictureScene::PictureScene(int width, int height)
    : Scene(ID, width, height, 4)
{
    this->texture = Texture::create("asset/Lenna.png", false, true);
    this->shader = Shader::createByPath("asset/shader/picture.vert", "asset/shader/picture.frag");
    this->camera = Camera2D::create();
}

SceneRef PictureScene::create()
{
    struct enable_make_shared : public PictureScene
    {
        enable_make_shared() : PictureScene(0, 0) {}
    };
    return std::make_shared<enable_make_shared>();
}

PictureSceneRef PictureScene::create(int width, int height)
{
    struct enable_make_shared : public PictureScene
    {
        enable_make_shared(int width, int height)
            : PictureScene(width, height) {}
    };

    return std::make_shared<enable_make_shared>(width, height);
}

void PictureScene::draw()
{
    this->camera->setViewSize((float)this->width, (float)this->height);
    this->camera->update();

    auto mat = camera->getViewProj() * math::rotateDegree(rotation, math::Z_Axis);

    // Y 轴乘以 -1 是为了让 纹理坐标的原点在左上角，Y 轴向下
    mat = mat * math::scale({(float)texture->getWidth() * 0.5f, (float)texture->getHeight() * -0.5f, 1.0f});

    shader->use();
    glUniformMatrix4fv(1, 1, false, (float*)&mat);
    glUniform4fv(2, 1, (float*)&color);
    shader->bindTexture(3, this->texture);
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


void PictureScene::drawProperty()
{
    if (!showPropertyWindow)
    {
        return;
    }

    if(ImGui::Begin(Scene::PropertyWindow, &showPropertyWindow, 0))
    {
        ImGui::SetWindowSize({300, 400}, ImGuiCond_FirstUseEver);
        if (ImGui::Button("Reset", {100.0f, 0}))
        {
            this->color = { 1.0f, 1.0f, 1.0f, 1.0f};
            this->rotation = 0.0f;
            this->camera->resetView();
        }

        ImGui::ColorEdit4("Color", (float*)&color);
        ImGui::DragFloat("Rotate", &rotation);

        ImGui::Separator();

        if (ImGui::Button("Save", {100.0f, 0}))
        {
            const auto& pixels = this->fbResolved->readPixel();
            std::string path = ".data/";
            path += this->name;
            path += ".png";
            stbi_write_png(path.c_str(), width, height, 4, pixels.data(), width * 4);

            auto workingDir = std::filesystem::current_path().u8string();
            LOGI("Save to picture: {}/{}", (const char*)workingDir.data(), path);
        }

    }
    ImGui::End();
}

void PictureScene::onMouseEvent(const MouseEvent* e)
{
    if (e->mouseButton == MouseEvent::kButtonLeft)
    {
        if (e->mouseEventType == MouseEvent::kMousePress)
        {
            this->holdLeftButton = true;
        }
        else if (e->mouseEventType == MouseEvent::kMouseRelease)
        {
            this->holdLeftButton = false;
        }
    }

    if (e->mouseEventType == MouseEvent::kMouseScroll)
    {
        float scale = this->camera->getViewScale();
        scale = (e->scrollY > 0) ? scale * 0.8f : scale * 1.25f;
        this->camera->setViewScale(scale);
    }
    else if (e->mouseEventType == MouseEvent::kMouseMove)
    {
        if (this->holdLeftButton)
        {
            auto delta = e->posDelta;
            delta *= this->camera->getViewScale();
            this->camera->move({delta.x, -delta.y, 0});
        }
    }
}