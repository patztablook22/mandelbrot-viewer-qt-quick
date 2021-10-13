#ifndef RENDERER_H
#define RENDERER_H

#include <QThread>
#include <QObject>
#include <QAbstractVideoSurface>
#include "instructions.h"
#include "palette.h"

class Renderer : public QThread
{
        Q_OBJECT
        Q_PROPERTY(QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface)

        Q_PROPERTY(QSize  outSize        READ outSize WRITE setOutSize   NOTIFY outSizeChanged)
        Q_PROPERTY(QSizeF calcSize       READ calcSize  NOTIFY calcSizeChanged)
        Q_PROPERTY(qreal scale           READ scale      WRITE setScale          NOTIFY scaleChanged)
        Q_PROPERTY(QPointF calcCenter    READ calcCenter WRITE setCalcCenter     NOTIFY calcCenterChanged)
        Q_PROPERTY(int threads           READ threads    WRITE setThreads        NOTIFY threadsChanged)
        Q_PROPERTY(int precision         READ precision                          NOTIFY precisionChanged)
        Q_PROPERTY(Palette* palette      READ palette    WRITE setPalette)
public:
        Renderer(QObject* parent = nullptr);
        ~Renderer();
        QAbstractVideoSurface* videoSurface() const;
        void setVideoSurface(QAbstractVideoSurface* surface);

        QSize outSize() const;
        QSizeF calcSize() const;
        qreal scale() const;
        QPointF calcCenter() const;
        int threads() const;
        int precision() const;
        Palette* palette() const;

        void setOutSize(QSize size);
        void setScale(qreal scale);
        void setCalcCenter(QPointF center);
        void setThreads(int threads);
        void setPalette(Palette* palette);

signals:
        void rendered(const QImage& image, int precision);
        void precisionChanged();
        void outSizeChanged();
        void calcSizeChanged();
        void scaleChanged();
        void calcCenterChanged();
        void threadsChanged();

protected:
        void run();
private:
        void waitForChanges();
        int m_threads;
        int m_precision;
        Instructions instructions;
        QAbstractVideoSurface* p_surface;
        Palette* p_palette;
        void updateImage(const QImage& image, int precision);
};

#endif // RENDERER_H
