#include "palette.h"
#include <QFile>
#include <QDebug>
#include <string>
#include <sstream>

Palette::Palette(QObject *parent)
    : QObject(parent)
{

}

bool Palette::valid() const
{
    return m_valid;
}

QString Palette::source() const
{
    return m_source;
}

void Palette::setSource(QString source)
{
    using namespace std;

    QFile fd(source);

    if (!fd.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "unable to read file:" << source;
        return;
    }

    QString content = fd.readAll();
    stringstream ss(content.toStdString());

    for (int line = 0; line < 256; line++) {
        string buffer;

        // red
        ss >> buffer;
        uchar r = stoi(buffer);

        // green
        ss >> buffer;
        uchar g = stoi(buffer);

        // blue
        ss >> buffer;
        uchar b = stoi(buffer);

        m_colors[line] = qRgb(r,g,b);
    }

    m_valid  = true;
    m_source = source;
    emit sourceChanged();
}

const QRgb& Palette::getColor(uchar index)
{
    return m_colors[index];
}
