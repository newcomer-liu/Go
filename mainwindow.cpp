#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QSize>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("围棋");
    ui->regret->setEnabled(false);
    ui->restart->setEnabled(false);
    ui->giveup->setEnabled(false);
    ui->pass->setEnabled(false);
    ui->last->setEnabled(false);
    ui->next->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    // 太极图片
    img_b = new QPixmap(":/Image/B.png");
    *img_b = img_b->scaled(200,200);
    img_w = new QPixmap(":/Image/W.png");
    *img_w = img_w->scaled(200,200);
    //打开文件
    connect(ui->actionopen,&QAction::triggered,this,[=](){
        QString s = QFileDialog::getOpenFileName(this,"打开文件","./") ;   // 若取消打开  返回空串
        if (s != "")
        {
            char *openfilename = s.toLatin1().data();
            ui->start->setEnabled(false);
            InitSeq(&L);
            InitState(&last_3);
            ui->chessboard->init();
            openfile(openfilename);
            ui->last->setEnabled(true);
            ui->next->setEnabled(true);
            p = L;
            qDebug() << openfilename  ;
        }
    });

    //开始按钮功能
    connect(ui->start,&QPushButton::clicked,ui->chessboard,[=](){
        ui->chessboard->setEnabled(true);
        ui->pass->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        currentstone = BLACK;
        ui->start->setEnabled(false);
        ui->restart->setEnabled(true);
        InitState(&last_3);
        InitSeq(&L);
        update();
    });
    //过一手按钮功能
    connect(ui->pass,&QPushButton::clicked,this,[=](){
        pass_time++;
        InsertAfter(&L,-1,-1,4);
        if (pass_time == 2)
        {
            ui->chessboard->row = 20;
            ui->chessboard->column = 20;
            QMessageBox::information(this,"对局结束","谁赢了呢?");
            QMessageBox::StandardButton response_btn = QMessageBox::question(this,"保存","是否保存?");
            if (response_btn == QMessageBox::Yes)
            {
                QString s = QFileDialog::getSaveFileName(this,"打开文件","./",".txt") + ".txt";
                char *savefilename = s.toLatin1().data();
                savefile(savefilename);
            }
            ui->chessboard->setEnabled(false);
            ui->regret->setEnabled(false);
            ui->pass->setEnabled(false);
            ui->giveup->setEnabled(false);
            ui->restart->setEnabled(true);
        }
        else
        {
            regret_time++;
            if (regret_time >= 2) emit ui->chessboard->reset_regret();
            AddState(&last_3,board);
            if (currentstone == BLACK) currentstone = WHITE;
            else currentstone = BLACK;
            update();
        }
    });
    // 认输
    connect(ui->giveup,&QPushButton::clicked,this,[=](){
       if (currentstone == BLACK) {
           QMessageBox::information(this,"对局结束","黑方认输,白方胜");
       }
       else {
           QMessageBox::information(this,"对局结束","白方认输,黑方胜");
       }
       ui->chessboard->row = 20;
       ui->chessboard->column = 20;
       ui->chessboard->setEnabled(false);
       ui->regret->setEnabled(false);
       ui->pass->setEnabled(false);
       ui->giveup->setEnabled(false);
       ui->restart->setEnabled(true);
    });
    // 重新开始按钮功能
    connect(ui->restart,&QPushButton::clicked,this,[=](){
        ui->chessboard->init();
        ui->chessboard->update();
        ui->start->setEnabled(true);
        ui->regret->setEnabled(false);
        ui->pass->setEnabled(false);
        ui->giveup->setEnabled(false);
        ui->restart->setEnabled(false);
        ui->lineEdit->setEnabled(false);
    });
    // 悔棋按钮功能
    connect(ui->regret,&QPushButton::clicked,this,[=](){
        InsertAfter(&L,-1,-1,3);
        ui->regret->setEnabled(false);
        regret();
        ui->chessboard->last_row  = 20;
        ui->chessboard->last_column = 20;
        ui->chessboard->update();
    });
    // 重置悔棋按钮
    connect(ui->chessboard,&ChessBoard::reset_regret,this,[=](){
        if (regret_time >= 2)
        {
            ui->regret->setEnabled(true);
            //qDebug() << regret_time;
        }
    });
    // 退出按钮功能初始化
    connect(ui->end,&QPushButton::clicked,this,&QWidget::close);

    connect(ui->chessboard,&ChessBoard::myclick,this,[=](){
        if (!isempty)
        {
            ui->giveup->setEnabled(true);
            ui->restart->setEnabled(true);
        }
        update();
    });
    //   无气时弹出警告
    connect(ui->chessboard,&ChessBoard::Warn,this,[=](){
        QMessageBox::warning(this,"禁着点!","无气,此处不能落子!");
    });
    // 上一步
    connect(ui->last,&QPushButton::clicked,this,[=](){
        step--;
        InitState(&last_3);
        p = L;
        for (int i = 0;i<boardsize;i++)
            for (int j = 0;j<boardsize;j++)
                board[i][j] = NONE;
        for (int i = 0;i<step;i++){
            emit ui->next->clicked();
            step--;
        }
    });
    // 下一步
    connect(ui->next,&QPushButton::clicked,this,[=](){
            p = p->next;
            if(p->now == 3) {
                regret();
                ui->chessboard->last_row  = 20;
                ui->chessboard->last_column = 20;
                ui->chessboard->update();
            }
            else if (p->now == 4) {
                pass_time++;
                if (pass_time == 2)
                {
                    ui->chessboard->row = 20;
                    ui->chessboard->column = 20;
                    QMessageBox::information(this,"对局结束","谁赢了呢?");
                    ui->next->setEnabled(false);
                    ui->chessboard->setEnabled(false);
                    ui->regret->setEnabled(false);
                    ui->pass->setEnabled(false);
                    ui->giveup->setEnabled(false);
                    ui->restart->setEnabled(true);
                }
                else
                {
                    AddState(&last_3,board);
                    if (currentstone == BLACK) currentstone = WHITE;
                    else currentstone = BLACK;
                }
            }
            else {
                putStone(p->x,p->y,p->now);
                go_check(p->x,p->y);
                AddState(&last_3,board);
                ui->chessboard->update();
                ui->chessboard->last_row = p->x;
                ui->chessboard->last_column = p->y;
                if(currentstone != WHITE) currentstone = WHITE;
                else if(currentstone != BLACK) currentstone = BLACK;
            }
            step++;
        update();
    });
    // 打劫时弹出警告
    connect(ui->chessboard,&ChessBoard::WarnSame,this,[=](){
        QMessageBox::warning(this,"打劫!","请从别处落子!");
    });
    // 键盘输入确认按钮
    ui->ensure->setEnabled(false);
    connect(ui->lineEdit,&QLineEdit::textChanged,this,[=](){
        ui->ensure->setEnabled(!(ui->lineEdit->text().isEmpty()));
    });
    //  从键盘输入棋子
    connect(ui->ensure,&QPushButton::clicked,this,[=](){
        QString s = ui->lineEdit->text();
        std::string str = s.toStdString();
        int a[2] = {-1,-1};
        if (getpos_kboard(str,a))
        {
            if(putStone(a[0],a[1],currentstone))
            {

                isempty = false;
                emit ui->chessboard->myclick();
                if(!go_check(a[0],a[1]))
                {
                    if(AddState(&last_3,board))
                    {
                        pass_time = 0;
                        update();
                        regret_time++;
                        emit ui->chessboard->reset_regret();
                        InsertAfter(&L,ui->chessboard->row,ui->chessboard->column,currentstone);
                        if(currentstone != BLACK ) currentstone = BLACK;
                        else if(currentstone != WHITE) currentstone = WHITE;
                        ui->chessboard->last_row = a[0];
                        ui->chessboard->last_column = a[1];
                    }
                    else emit ui->chessboard->WarnSame();
                }
                else
                {
                    emit ui->chessboard->Warn();
                }
            }
            else QMessageBox::warning(this,"落子无效","此处已有棋子!");
        }
        else
        {
            QMessageBox::warning(this,"错误输入!","请输入正确的坐标");
        }
        ui->lineEdit->clear();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow:: paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPoint p(ui->chessboard->x()+padding,ui->chessboard->y()+padding+10);
    QBrush brush(QColor(34,142,204));
    painter.setBrush(brush);
    painter.drawRect(ui->chessboard->x()+ui->chessboard->width()+35,ui->chessboard->y()+45,230,810);  //  画蓝框
    // 画太极
    if (currentstone == BLACK)
    {
        painter.drawPixmap(ui->chessboard->x()+ui->chessboard->width()+48,ui->chessboard->y()+55,*img_b);

    }
    else if (currentstone == WHITE)
    {
        painter.drawPixmap(ui->chessboard->x()+ui->chessboard->width()+48,ui->chessboard->y()+55,*img_w);
    }
    else
    {
        painter.drawPixmap(ui->chessboard->x()+ui->chessboard->width()+48,ui->chessboard->y()+55,*img_b);
        painter.drawPixmap(ui->chessboard->x()+ui->chessboard->width()+48,ui->chessboard->y()+55,*img_w);
    }
    for (int i = 0;i<19;i++)
    {
        QString s('A'+i);
        painter.drawText(p.x()+ i*interval,p.y()-r/2-2,s);
        painter.drawText(p.x()+ i*interval,p.y()+ui->chessboard->height()+r/2+2,s);
        s = tr("%1").arg(i+1);
        painter.drawText(p.x()-2*r,p.y()+i*interval+20,s);
        painter.drawText(p.x()+ui->chessboard->width()-r+5,p.y()+i*interval+20,s);
    }
}
