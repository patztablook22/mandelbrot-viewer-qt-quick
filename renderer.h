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
        Q_PROPERTY(qreal exponent        READ exponent   WRITE setExponent       NOTIFY exponentChanged);
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
        qreal exponent() const;

        void setOutSize(QSize size);
        void setScale(qreal scale);
        void setCalcCenter(QPointF center);
        void setThreads(int threads);
        void setPalette(Palette* palette);
        void setExponent(qreal exponent);

        Q_INVOKABLE void exportTo(QString path);

signals:
        void rendered(const QImage& image, int precision);
        void precisionChanged();
        void outSizeChanged();
        void calcSizeChanged();
        void scaleChanged();
        void calcCenterChanged();
        void threadsChanged();
        void exponentChanged();

protected:
        void run();
private:
        void waitForChanges();
        int m_threads;
        int m_precision;
        Instructions instructions;
        QAbstractVideoSurface* p_surface;
        void updateImage(const QImage& image, int precision);
        QImage activeImage;
};

#endif // RENDERER_H
