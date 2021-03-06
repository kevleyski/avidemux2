/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************///

#include <QPainter>
#include <QRubberBand>
#include <QBoxLayout>
#include <QSizeGrip>

#include "ADM_default.h"
#include "DIA_flyDialogQt4.h"

ADM_QRubberBand::ADM_QRubberBand(QWidget *parent) : QRubberBand(QRubberBand::Rectangle, parent)
{
}

ADM_QRubberBand::~ADM_QRubberBand()
{
}

/**
    \fn paintEvent
    \brief Override platform-dependent appearance of QRubberBand
*/
void ADM_QRubberBand::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(2);
    QColor color(Qt::red);
    pen.setColor(color);
    painter.setPen(pen);
    QRect adjustedRect = rect().adjusted(1,1,-1,-1);
    painter.drawRect(adjustedRect);
    color.setAlpha(80);
    QBrush brush = QBrush(color, Qt::DiagCrossPattern);
    adjustedRect = adjustedRect.adjusted(1,1,-1,-1);
    painter.fillRect(adjustedRect, brush);
    painter.end();
}

/**
    \fn Ctor
*/
ADM_rubberControl::ADM_rubberControl(ADM_flyDialog *fly, QWidget *parent) : QWidget(parent)
{
    nestedIgnore=0;
    flyParent=fly;
    // tell QSizeGrip to resize this widget instead of top-level window
    setWindowFlags(Qt::SubWindow);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    QSizeGrip *grip1 = new QSizeGrip(this);
    QSizeGrip *grip2 = new QSizeGrip(this);
#ifdef __APPLE__
    // work around grips not shown on macOS
    grip1->setFixedSize(10,10);
    grip2->setFixedSize(10,10);
#endif
    grip1->setVisible(true);
    grip2->setVisible(true);
    layout->addWidget(grip1, 0, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(grip2, 0, Qt::AlignRight | Qt::AlignBottom);
    rubberband = new ADM_QRubberBand(this);
    rubberband->move(0, 0);
    rubberband->show();
    show();
}

/**
    \fn resizeEvent
*/
void ADM_rubberControl::resizeEvent(QResizeEvent *)
{
    int x, y, w, h;
    x = pos().x();
    y = pos().y();
    w = size().width();
    h = size().height();
    rubberband->resize(size());
    if(!nestedIgnore)
        flyParent->bandResized(x, y, w, h);
}
//EOF
