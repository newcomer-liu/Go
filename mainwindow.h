#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "chessboard.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    SeqNode *p ;
protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::MainWindow *ui;
    QPixmap *img_w;
    QPixmap *img_b;

};
#endif // MAINWINDOW_H
