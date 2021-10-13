#ifndef PALETTE_H
#define PALETTE_H

#include <QObject>
#include <QRgb>
#include <array>

class Palette : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool    valid  READ valid)

public:
    explicit Palette(QObject *parent = nullptr);
    QString source() const;
    void setSource(QString source);
    bool valid() const;
    const QRgb& getColor(uchar index);

signals:
    void sourceChanged();

private:
    QString m_source;
    bool    m_valid;
    std::array<QRgb, 256> m_colors;
};

#endif // PALETTE_H
