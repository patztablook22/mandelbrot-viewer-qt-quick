#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QtCore>
#include <QWaitCondition>

#include "palette.h"

class Instructions
{
public:
        Instructions();
        ~Instructions();

        // getters
        // and setters:

        qreal scale() const;
        void setScale(qreal scale);

        QSize outSize() const;
        void setOutSize(QSize size);

        QSizeF calcSize() const;

        QPointF calcCenter() const;
        void setCalcCenter(QPointF center);

        qreal exponent() const;
        void setExponent(qreal exponent);

        Palette* palette() const;
        void setPalette(Palette* palette);

        bool changed() const;
        Instructions getChanges() const;

        bool imageOnly() const;

        void stop();
        bool shouldStop() const;

private:
        void change(bool imageOnly = false);

private:
        qreal m_scale;
        QPointF m_calcCenter;
        QSizeF m_calcSize;
        QSize m_outSize;
        qreal m_exponent;
        Palette* p_palette;
        bool m_stop;

        mutable bool m_changed;
        mutable bool m_imageOnly;
        QMutex* mutex;
        QWaitCondition* condition;

signals:

};

#endif // MANAGER_H
