//
// Created by Administrator on 2022/8/7.
//

#ifndef QTOMM_QSKIAWIDGET_H
#define QTOMM_QSKIAWIDGET_H


#include <QWidget>

class SkCanvas;

class QSkiaWidget : public QWidget {

Q_OBJECT
public:
    QSkiaWidget(QWidget *parent = nullptr);

    ~QSkiaWidget() override;

    virtual void draw(SkCanvas *canvas, int elapsed) = 0;

protected:

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void init(int w, int h);

private:
    class Private;

    Private *m_dptr = nullptr;
};


#endif //QTOMM_QSKIAWIDGET_H
