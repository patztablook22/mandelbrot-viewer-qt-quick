#ifndef RENDERER_H
#define RENDERER_H

#include <QThread>
#include <QObject>
#include <QAbstractVideoSurface>
#include "manager.h"

class Renderer : public QThread
{
        Q_OBJECT
        Q_PROPERTY(QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface)
        Q_PROPERTY(Manager* manager READ manager WRITE setManager)
public:
        Renderer(QObject* parent = nullptr);
        Manager* manager() const;
        QAbstractVideoSurface* videoSurface() const;
        void setManager(Manager* manager);
        void setVideoSurface(QAbstractVideoSurface* surface);
private:
        Manager* p_manager;
        QAbstractVideoSurface* p_surface;
};

#endif // RENDERER_H
