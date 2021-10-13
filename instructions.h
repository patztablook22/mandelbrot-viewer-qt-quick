#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QtCore>
#include <QWaitCondition>

class Instructions
{
public:
    Instructions();

    qreal scale() const;
    void setScale(qreal scale);

    QSize outSize() const;
    void setOutSize(QSize size);

    QSizeF calcSize() const;

    QPointF calcCenter() const;
    void setCalcCenter(QPointF center);

    qreal exponent() const;
    void setExponent(qreal exponent);

    bool changed() const;
    const Instructions getChanges();

    void stop();
    bool shouldStop() const;

private:
    qreal m_scale;
    QPointF m_calcCenter;
    QSizeF m_calcSize;
    QSize m_outSize;
    qreal m_exponent;
    bool m_changed;
    bool m_stop;
    QMutex* mutex;
    QWaitCondition* condition;
    void change();

signals:

};

#endif // MANAGER_H
