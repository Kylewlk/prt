//
// Created by wlk12 on 2023/8/6.
//

#include "Scene.hpp"
#include "common/FrameBuffer.h"

Scene::Scene(const char* name, int width, int height, int samples)
    :name(name), width(0), height(0), samples(samples)
{
    if (width > 0 && height > 0)
    {
        Scene::resize(width, height);
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
    if ( this->samples <= 1)
    {
        this->fbResolved = FrameBuffer::create(width, height, RenderTarget::kTexColor, RenderTarget::kRenderDepth);
    }
    else
    {
        this->fbResolved = FrameBuffer::create(width, height, RenderTarget::kTexColor, RenderTarget::kNone);
        this->fbDraw = FrameBuffer::createMultisample(width, height, 4, RenderTarget::kTexColor, true);
    }
}

void Scene::draw()
{

}

void Scene::drawProperty()
{

}

void Scene::render()
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


    this->drawProperty();
}

void Scene::onMouseEvent(const MouseEvent* e)
{

}

void Scene::onKeyEvent(const KeyEvent* e)
{

}
