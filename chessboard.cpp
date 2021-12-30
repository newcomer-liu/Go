#include "chessboard.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QDebug>
#include <QColor>
#include "algorithm.h"
ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent)
{
    init();
    setMouseTracking(true);
}

void ChessBoard::init()
{
    for(int i=0;i < boardsize;i++){
        for(int j=0;j < boardsize;j++){
            board[i][j] = NONE;
        }
    }
    currentstone = NONE;
    this->setEnabled(false);
    row = 20;
    column = 20;
    last_row = 20;
    last_column = 20;
}
//绘图
void ChessBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::black);
    QBrush brush(QColor(93,44,5));
    pen.setWidth(3);
    painter.setPen(pen);
    painter.setBrush(brush);
    // 画边框
    painter.drawRect(padding,padding,interval*(boardsize-1),interval*(boardsize-1));
    pen.setWidth(1);
    painter.setPen(pen);
    // 画细线
    for(int i=1;i < boardsize-1 ;i++){
        painter.drawLine(padding, padding+i*interval, interval*(boardsize-1) + padding ,padding + i*interval);
    }
    for(int j=1;j < boardsize-1 ;++j){
        painter.drawLine(padding+j*interval, padding, padding + j*interval, interval*(boardsize-1) + padding);
    }
    // 画天元
    brush.setColor(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    int center=(boardsize -1)/2;
    painter.drawEllipse(QPoint(padding+center*interval,padding+center*interval),4,4);
    painter.drawEllipse(QPoint(padding+3*interval,padding+3*interval),4,4);
    painter.drawEllipse(QPoint(padding+3*interval,padding+(boardsize-4)*interval),4,4);
    painter.drawEllipse(QPoint(padding+(boardsize-4)*interval,padding+(boardsize-4)*interval),4,4);
    painter.drawEllipse(QPoint(padding+(boardsize-4)*interval,padding+3*interval),4,4);
    painter.drawEllipse(QPoint(padding+3*interval,padding+center*interval),4,4);
    painter.drawEllipse(QPoint(padding+(boardsize-4)*interval,padding+center*interval),4,4);
    painter.drawEllipse(QPoint(padding+center*interval,padding+3*interval),4,4);
    painter.drawEllipse(QPoint(padding+center*interval,padding+(boardsize-4)*interval),4,4);
    pen.setWidth(2);
    //画棋子
    for(int i=0;i<boardsize ;i++){
        for(int j=0;j<boardsize ;j++){
            int current;
            current= checkStone(i,j);
            if(current==BLACK){
                pen.setColor(Qt::black);
                painter.setPen(pen);
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((i)*interval+padding,(j)*interval+padding),r,r);
            }
            else if(current==WHITE){
                pen.setColor(Qt::white);
                painter.setPen(pen);
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint((i)*interval+padding,(j)*interval+padding),r,r);
            }
        }
    }
    // 跟随鼠标的棋子
    if ( row < boardsize && column < boardsize)
        {
        if(currentstone == BLACK && board[row][column] == NONE){
            pen.setColor(Qt::black);
            painter.setPen(pen);
            brush.setColor(Qt::black);
            painter.setBrush(brush);
//            painter.drawEllipse(QPoint((row)*interval+padding,(row)*interval+padding),r,r);
            painter.drawRect(row*interval+padding-10,column*interval+padding-10,20,20);
        }
        else if(currentstone == WHITE && board[row][column] == NONE){
            pen.setColor(Qt::white);
            painter.setPen(pen);
            brush.setColor(Qt::white);
            painter.setBrush(brush);
//            painter.drawEllipse(QPoint((row)*interval+padding,(column)*interval+padding),r,r);
            painter.drawRect(row*interval+padding-10,column*interval+padding-10,20,20);
        }
    }
    pen.setWidth(2);
    pen.setColor(Qt::green);
    brush.setStyle(Qt::NoBrush);
    painter.setBrush(brush);
    painter.setPen(pen);
    if(last_row >=0 && last_row <=19 && last_column >=0 && last_column <=19)
    painter.drawEllipse(QPoint(padding+last_row*interval,padding+last_column*interval),r+1,r+1);
}

//鼠标点击动作
void ChessBoard::mouseReleaseEvent(QMouseEvent *event){
    row = getpos(event->x());
    column = getpos(event->y());
    if (putStone(row,column,currentstone))
    {

        isempty = false;
        emit myclick();
        if(!go_check(row,column))
        {
            if(AddState(&last_3,board))
            {
                pass_time = 0;
                update();
                regret_time++;
                emit reset_regret();
                InsertAfter(&L,row,column,currentstone);
                if(currentstone == WHITE) currentstone = BLACK;
                else if(currentstone == BLACK) currentstone = WHITE;
                last_row = row;
                last_column = column;
            }
            else emit WarnSame();
        }
        else
        {
            emit Warn();
        }
    }

}
//获得鼠标移动的坐标
void ChessBoard::mouseMoveEvent(QMouseEvent *event)
{
    row = getpos(event->x());
    column = getpos(event->y());
    update();
}
