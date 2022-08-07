#include <QApplication>
#include <QPushButton>
#include "QSkiaWidget.h"
#include "SampleRender.h"

class SkiaWidget : public QSkiaWidget {
Q_OBJECT
public:
    void draw(SkCanvas *canvas, int elapsed) override {
        m_renderer.draw(canvas, elapsed, this->width(), this->height());
    }

private:
    SampleRender m_renderer;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SkiaWidget w;
    w.show();
    return QApplication::exec();
}
