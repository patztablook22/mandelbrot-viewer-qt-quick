#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    explicit Manager(QObject *parent = nullptr);

    qreal scale() const;
    void setScale(qreal scale);

signals:
    void scaleChanged();

private:
    qreal m_scale;

signals:

};

#endif // MANAGER_H
