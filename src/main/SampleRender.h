//
// Created by Administrator on 2022/8/7.
//

#ifndef QTOMM_SAMPLERENDER_H
#define QTOMM_SAMPLERENDER_H

#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkCanvas.h"

class SampleRender {
public:
    SampleRender();

    void draw(SkCanvas *canvas, int elapsed, int w, int h);

    void init(int w, int h);

    void resize(int w, int h);

private:
    SkPaint m_paint;
    SkFont m_font;
    float m_rotateSpeed = 90.0f / 1000;
    int m_rotateAngle = 0;
};


#endif //QTOMM_SAMPLERENDER_H
