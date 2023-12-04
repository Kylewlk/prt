//
// Created by wlk12 on 2023/8/6.
//

#include "Scene.h"
#include "common/FrameBuffer.h"
#include "common/Shader.h"
#include "common/Texture.h"
#include "camera/Camera2D.h"
#include "camera/Camera3D.h"
#include "common/EventSystem.h"
#include "common/Logger.h"

Scene::Scene(const char* name, int width, int height, int samples, bool hdr /*= false*/)
    :name(name), width(0), height(0), samples(samples), hdr(hdr)
{
    if (width > 0 && height > 0)
    {
        Scene::resize(width, height);
    }

    if (hdr)
    {
        if (samples > 1)
        {
            this->toneMappingShader =
                Shader::create("asset/shader/tone_mapping.vert", "asset/shader/tone_mapping_multisample.frag");
        }
        else
        {
            this->toneMappingShader =
                Shader::create("asset/shader/tone_mapping.vert", "asset/shader/tone_mapping.frag");
        }
    }

    this->mouseListener = MouseListener::create();
    this->mouseListener->onMouseEvent = [this](auto e){ this->onMouseEvent(e); };
    EventSystem::get()->subscribe(this->mouseListener);

    this->keyListener = KeyListener::create();
    this->keyListener->onKey = [this](auto e){ this->onKeyEvent(e); };
    EventSystem::get()->subscribe(this->keyListener);
}

TextureRef Scene::getColorTexture()
{
    return this->fbResolved->getColor();
}


void Scene::resize(int width_, int height_)
{
    if (this->width == width_ && this->height == height_)
    {
        return;
    }

    this->width = width_;
    this->height = height_;
    if ( this->samples <= 1 && !hdr)
    {
        this->fbResolved = FrameBuffer::create(width, height, RenderTarget::kTexColor, RenderTarget::kRenderDepth);
    }
    else
    {
        this->fbResolved = FrameBuffer::create(width, height, RenderTarget::kTexColor, RenderTarget::kNone);
        this->fbDraw = FrameBuffer::createMultisample(width, height, samples,
                                                      hdr ? RenderTarget::kTexColorFloat : RenderTarget::kTexColor, true);
    }
}

void Scene::draw()
{

}

void Scene::drawProperty()
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
            this->reset();
        }

        ImGui::SameLine(0, 20);

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
        if (this->hdr)
        {
            ImGui::SliderFloat("exposure", &this->toneMappingExposure, 0.01f, 100.0f);
        }

        ImGui::Separator();

        this->drawSettings();
    }
    ImGui::End();
}

void Scene::render()
{
    if (hdr)
    {
        fbDraw->bind();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->draw();
        fbDraw->unbind();

        this->fbResolved->bind();
        this->toneMappingShader->use();
        if (this->samples > 1)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbDraw->getColor()->getHandle());
            glUniform1i(0, 0);
            this->toneMappingShader->setUniform("sampleCount", fbDraw->getSamples());
        }
        else
        {
            this->toneMappingShader->bindTexture(0, this->fbDraw->getColor());
        }
        this->toneMappingShader->setUniform("exposure", toneMappingExposure);
        drawQuad();
        this->fbResolved->unbind();
    }
    else
    {
        if (samples <= 1)
        {
            fbResolved->bind();
            glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->draw();
            fbResolved->unbind();
        }
        else
        {
            fbDraw->bind();
            glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->draw();
            fbDraw->unbind();

            fbDraw->blitFramebuffer(0, 0, width, height, this->fbResolved);
        }
    }

    this->drawProperty();
}

void Scene::onMouseEvent(const MouseEvent* e)
{
    if (e->mouseButton == MouseEvent::kButtonLeft)
    {
        this->holdLeftButton = e->mouseEventType == MouseEvent::kMousePress;
    }
    else if (e->mouseButton == MouseEvent::kButtonRight)
    {
        this->holdRightButton = e->mouseEventType == MouseEvent::kMousePress;
    }
    else if (e->mouseButton == MouseEvent::kButtonMiddle)
    {
        this->holdMidButton = e->mouseEventType == MouseEvent::kMousePress;
    }
}

void Scene::onKeyEvent(const KeyEvent* e)
{

}


void Scene::cameraMouseEvent(const MouseEvent* e, Camera2D* camera)
{
    if (e->mouseEventType == MouseEvent::kMouseScroll)
    {
        float scale = camera->getViewScale();
        scale = (e->scrollY > 0) ? scale * 0.8f : scale * 1.25f;
        camera->setViewScale(scale);
    }
    else if (e->mouseEventType == MouseEvent::kMouseMove)
    {
        if (this->holdLeftButton)
        {
            auto delta = e->posDelta;
            delta *= camera->getViewScale();
            camera->move({delta.x, -delta.y, 0});
        }
    }
}

void Scene::cameraMouseEvent(const MouseEvent* e, Camera3D* camera)
{
    if (e->mouseEventType == MouseEvent::kMouseScroll)
    {
        camera->forward((float)e->scrollY*20.0f);
    }
    else if (e->mouseEventType == MouseEvent::kMouseMove)
    {
        auto delta = e->posDelta;
        if (this->holdLeftButton)
        {
            camera->round(delta.x, -delta.y);
        }
        else if(this->holdMidButton)
        {
            camera->move({delta.x, -delta.y, 0});
        }
    }
}
