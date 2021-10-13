#include "renderer.h"

Renderer::Renderer(QObject* parent)
        :QThread(parent)
{
}

void Renderer::setManager(Manager *manager)
{
    if (manager == nullptr || manager == p_manager)
        return;
    p_manager = manager;
}

Manager* Renderer::manager() const
{
    return p_manager;
}

QAbstractVideoSurface* Renderer::videoSurface() const
{
    return p_surface;
}

void Renderer::setVideoSurface(QAbstractVideoSurface *surface)
{
    if (surface == p_surface || surface == nullptr)
        return;
    p_surface = surface;
}
