#include "palette.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Palette::Palette(QObject *parent)
    : QObject(parent)
    , m_valid(false)
{
}

bool Palette::valid() const {
    return m_valid;
}

QString Palette::source() const {
    return m_source;
}

void Palette::setSource(const QString& source) {
    m_valid = false;
    QFile fd(source);
    QTextStream stream(&fd);

    if (!fd.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "unable to read file:" << source;
        return;
    }

    for (size_t line = 0; line < 256; line++) {
        if (stream.atEnd()) return;
        uint r,g,b;
        char semicol;
        stream >> r >> semicol >> g >> semicol >> b >> semicol;
        m_colors[line] = qRgb(r,g,b);
    }

    m_valid  = true;
    m_source = source;
    emit sourceChanged();
}

QRgb Palette::getColor(uchar index) const {
    return m_colors[index];
}
