#include <QtGui>
#include "numbereditor.h"

NumberEditor::NumberEditor(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    curColor = Qt::black;
    zoom = 16;
    dim = 7;
    image = QImage(dim, dim, QImage::Format_ARGB32);
    image.fill(qRgba(0, 0, 0, 0));
}

void NumberEditor::clearImage() {
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            setImagePixel(QPoint(i*zoom,j*zoom), false);
        }
    }
}
void NumberEditor::setImage(bool **matrix) {
    clearImage();
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            if ( matrix[i][j] ) {
                setImagePixel(QPoint(i*zoom,j*zoom), true);
            }
        }
    }
}

bool **NumberEditor::getImage() {

}

void NumberEditor::setDim(int newDim) {
    if ( newDim != dim ) {
        dim = newDim;
        image = QImage(dim, dim, QImage::Format_ARGB32);
        image.fill(qRgba(0, 0, 0, 0));
    }
}

int NumberEditor::getDim() {
    return dim;
}

QSize NumberEditor::sizeHint() const
{
    QSize size = zoom * image.size();
    if (zoom >= 3)
        size += QSize(1, 1);
    return size;
}

void NumberEditor::setPenColor(const QColor &newColor)
{
    curColor = newColor;
}

void NumberEditor::setIconImage(const QImage &newImage)
{
    if (newImage != image) {
        image = newImage.convertToFormat(QImage::Format_ARGB32);
        update();
        updateGeometry();
    }
}

void NumberEditor::setZoomFactor(int newZoom)
{
    if (newZoom < 1)
        newZoom = 1;
    if (newZoom != zoom) {
        zoom = newZoom;
        update();
        updateGeometry();
    }
}

void NumberEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (zoom >= 3) {
        painter.setPen(palette().foreground().color());
        for (int i = 0; i <= image.width(); ++i)
            painter.drawLine(zoom * i, 0, zoom * i, zoom * image.height());
        for (int j = 0; j <= image.height(); ++j)
            painter.drawLine(0, zoom * j, zoom * image.width(), zoom * j);
    }
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRect rect = pixelRect(i, j);
            if ( !(event->region().intersected(rect).isEmpty()) ) {
                QColor color = QColor::fromRgba(image.pixel(i, j));
                painter.fillRect(rect, color);
            }
        }
    }
}

QRect NumberEditor::pixelRect(int i, int j) const
{
    if (zoom >= 3) {
        return QRect(zoom * i + 1, zoom * j + 1, zoom - 1, zoom - 1);
    } else {
        return QRect(zoom * i, zoom * j, zoom, zoom);
    }
}

void NumberEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setImagePixel(event->pos(), true);
    } else if (event->button() == Qt::RightButton) {
        setImagePixel(event->pos(), false);
    }
}

void NumberEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        setImagePixel(event->pos(), true);
    } else if (event->buttons() & Qt::RightButton) {
        setImagePixel(event->pos(), false);
    }
}

void NumberEditor::setImagePixel(const QPoint &pos, bool opaque)
{
    int i = pos.x() / zoom;
    int j = pos.y() / zoom;
    if (image.rect().contains(i, j)) {
        if (opaque) {
            image.setPixel(i, j, penColor().rgba());
        } else {
            image.setPixel(i, j, qRgba(0, 0, 0, 0));
        }
        update(pixelRect(i, j));
    }
}
