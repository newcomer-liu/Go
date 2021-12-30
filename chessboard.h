#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <algorithm.h>
#include <QWidget>


class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    void init();
    int row ,column;
    int last_row,last_column;
signals:
    void myclick();
    void Warn();
    void WarnSame();
    void reset_regret();
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CHESSBOARD_H
