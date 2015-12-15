#include "playerwindow.h"
#include <QWidget>
#include <cmath>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_math.h>
#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

void PlayerWindow::initGraph(){

    plot = new QwtPlot();
    plot->setContentsMargins(0,-10,5,0);

    plot->setFixedSize(280, 120);

    //plot->setMaximumSize(940, 120);

    //setCentralWidget(plot);


    QwtPlotGrid *grid = new QwtPlotGrid(); //
    x = 0;
    y = 0;
    z = 0;
        grid->setMajorPen(QPen( Qt::gray, 1 )); // цвет линий и толщина
        grid->enableX(false);
        grid->attach( plot );
        //plot->setAxisScale(0, -3000, 3000, 2000);
        plot->setAxisAutoScale(0);
        //plot->setAxisAutoScale(plot->xBottom, true);

        plot->setAxisScale(plot->xBottom, x-20, x, 0);
        plot->enableAxis(plot->xBottom, false);
        plot->enableAxis(0, false);

        curveX = new QwtPlotCurve();
        curveY = new QwtPlotCurve();
        curveZ = new QwtPlotCurve();

        curveX->setPen( Qt::green, 1 );
        curveY->setPen( Qt::red, 1 );
        curveZ->setPen( Qt::blue, 1 );

        curveX->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // сглаживание
        curveY->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curveZ->setRenderHint( QwtPlotItem::RenderAntialiased, true );

            // Маркеры кривой
            // #include <qwt_symbol.h>
            QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
                QBrush( Qt::green ), QPen( Qt::green, 2 ), QSize( 5, 5 ) );
            QwtSymbol *symbolY = new QwtSymbol( QwtSymbol::Ellipse,
                QBrush( Qt::red ), QPen( Qt::red, 2 ), QSize( 5, 5 ) );
            QwtSymbol *symbolZ = new QwtSymbol( QwtSymbol::Ellipse,
                QBrush( Qt::blue ), QPen( Qt::blue, 2 ), QSize( 5, 5 ) );



            curveX->setSymbol( symbol );
            curveX->setCurveAttribute( QwtPlotCurve::Fitted );
            curveY->setSymbol( symbolY );
            curveY->setCurveAttribute( QwtPlotCurve::Fitted );
            curveZ->setSymbol( symbolZ );
            curveZ->setCurveAttribute( QwtPlotCurve::Fitted );

            // Добавить точки на ранее созданную кривую


            pointsX = new QPolygonF();
            pointsY = new QPolygonF();
            pointsZ = new QPolygonF();

            curveX->setSamples(*pointsX);
            curveX->attach( plot );
            curveY->attach( plot );
            curveZ->attach( plot );


              // отобразить кривую на графике

}

