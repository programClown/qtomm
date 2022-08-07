//
// Created by Administrator on 2022/8/7.
//

#include "QSkiaWidget.h"

#include "include/core/SkImageInfo.h"
#include "include/core/SkSurface.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkCanvas.h"

#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QPainter>
#include <QResizeEvent>

class QSkiaWidget::Private {
public:
    sk_sp<SkSurface> rasterSurface = nullptr;
    SkImageInfo info;
    QImage image;
    QByteArray data;
    QTimer timer;
    QTime lastTime;
};

QSkiaWidget::QSkiaWidget(QWidget *parent)
        : QWidget(parent), m_dptr(new Private) {
    init(this->width(), this->height());
    connect(&m_dptr->timer, &QTimer::timeout, this, QOverload<>::of(&QSkiaWidget::update));
    m_dptr->timer.start(1000 / 60);
}

QSkiaWidget::~QSkiaWidget() {
    delete m_dptr;
    m_dptr = nullptr;
}

void QSkiaWidget::init(int w, int h) {
    m_dptr->info = SkImageInfo::Make(w, h, SkColorType::kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
    size_t rowByts = m_dptr->info.minRowBytes();
    size_t size = m_dptr->info.computeByteSize(rowByts);
    m_dptr->data.resize(static_cast<int>(size));
    m_dptr->rasterSurface = SkSurface::MakeRasterDirect(m_dptr->info, m_dptr->data.data(), rowByts);
    if (!m_dptr->rasterSurface) {
        qDebug() << "SkSurface::MakeRasterN32Premul return null";
        return;
    }
}

void QSkiaWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    if (!this->isVisible()) {
        return;
    }
    if (!m_dptr->rasterSurface) {
        return;
    }
    auto canvas = m_dptr->rasterSurface->getCanvas();
    if (!canvas) {
        return;
    }
    const auto elapsed = m_dptr->lastTime.elapsed();
    m_dptr->lastTime = QTime::currentTime();
    canvas->save();
    this->draw(canvas, elapsed);
    canvas->restore();
    QPainter painter(this);
    m_dptr->image = QImage((uchar *) (m_dptr->data.data()), this->width(), this->height(), QImage::Format_RGBA8888);
    if (!m_dptr->image.isNull()) {
        painter.drawImage(0, 0, m_dptr->image);
    }
}

void QSkiaWidget::resizeEvent(QResizeEvent *event) {
    if (event->oldSize() == event->size()) {
        event->accept();
        return;
    }
    init(event->size().width(), event->size().height());
}
