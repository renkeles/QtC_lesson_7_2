#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , grScene(0,0,640,480)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&grScene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->installEventFilter(this);

    filter = new ItemEventFilter{};
    grScene.addItem(filter);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != ui->graphicsView) {
        return false;
    }

    if (event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {

            int changer = count++ % 3;
            auto ltop = ui->graphicsView->mapToScene(mouseEvent->x() - 50 + 1, mouseEvent->y() - 50);
            auto rbottom = ui->graphicsView->mapToScene(mouseEvent->x() + 50, mouseEvent->y() + 50);

            switch (changer)
            {
            case 0:
            {
            auto item = grScene.addRect(QRectF(ltop, rbottom), QPen{Qt::NoPen} ,getColorBrush());
            item->installSceneEventFilter(filter);
            }

                break;
            case 1:
            {
               auto item = grScene.addEllipse(QRectF(ltop, rbottom), QPen{Qt::NoPen} ,getColorBrush());
               item->installSceneEventFilter(filter);
            }

                break;
            case 2:
            {
                auto item = grScene.addPolygon(createStar(mouseEvent->x(), mouseEvent->y()), QPen{Qt::NoPen} ,getColorBrush());
                item->installSceneEventFilter(filter);
            }

                break;
            default:
                qDebug() << "Ошибка счётчика!";
                break;
            }

        }

        if(mouseEvent->button() == Qt::RightButton)
        {
            auto figure = grScene.itemAt(ui->graphicsView->mapToScene(mouseEvent->x(), mouseEvent->y()), QTransform{});
            if (figure) {
                grScene.removeItem(figure);
            }
        }
    }

    double scale = 1;
    double scaleStep = 0.01;
    if(event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        if(wheelEvent->angleDelta().y() > 0)
        {
            scale += scaleStep;
        }else
        {
            scale -= scaleStep;
        }
        ui->graphicsView->scale(scale, scale);
    }

    if(event->type() == QEvent::KeyPress)
    {
        int keyEvent = static_cast<QKeyEvent*>(event)->key();
        if(keyEvent == Qt::Key_Plus)
        {
            scale += scaleStep;
        }
        if(keyEvent == Qt::Key_Minus)
        {
            scale -= scaleStep;
        }
        ui->graphicsView->scale(scale, scale);
    }



    return false;
}

QBrush MainWindow::getColorBrush()
{
    return QBrush(QColor(rand() % 256, rand() % 256, rand() % 256));
}

QPolygonF MainWindow::createStar(int x, int y)
{
    auto centerStar = ui->graphicsView->mapToScene(x, y);
    QPolygonF polygon;
    polygon     << QPointF{centerStar.x(), centerStar.y() + 80}
                << QPointF{centerStar.x() + 25, centerStar.y() + 35}
                << QPointF{centerStar.x() + 75, centerStar.y() + 25}
                << QPointF{centerStar.x() + 40, centerStar.y() - 10}
                << QPointF{centerStar.x() + 50, centerStar.y() - 60}
                << QPointF{centerStar.x(), centerStar.y() - 40}
                << QPointF{centerStar.x() - 50, centerStar.y() - 60}
                << QPointF{centerStar.x() - 40, centerStar.y() - 10}
                << QPointF{centerStar.x() - 75, centerStar.y() + 25}
                << QPointF{centerStar.x() - 25, centerStar.y() + 35};
    return polygon;
}

MainWindow::~MainWindow()
{
    delete ui;
}

