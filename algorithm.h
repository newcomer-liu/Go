#include <string>
#include <stdio.h>
#include <string.h>
#ifndef ALGORITHM_H
#define ALGORITHM_H
#define MAXSIZE 362
#define MAXNAMESIZE 200
#define NONE 0
#define BLACK 1
#define WHITE 2
const int padding = 20;//相对边缘的距离
const int interval = 45;//间隔
const int boardsize = 19;//棋盘尺寸
const int r = 20; //棋子的半径
extern int regret_time;
extern int pass_time;
extern int isempty ; //棋盘是否为空
extern int step ;   // 记录步数
extern int currentstone; //记录当前下的棋子颜色
extern int  board[][19];
extern int vis[][19]; // 是否访问


typedef struct State {
    int board[19][19];
    State *next;
} State,*Statement;

extern Statement last_3;//保存棋盘的状态  倒数第三个

void InitState(Statement *last_3);
int compare(int **board1,int **board2);
int AddState(Statement *last_3,int board[][19]);

typedef struct Point{
    int x,y;
    int chess;
}Point;

extern int count;
typedef struct SeqNode{
    int x,y;
    SeqNode *next,*last;
    int now;
}SeqNode, *Sequence;

extern Sequence L;
void InitSeq(Sequence *L);
void InsertAfter(Sequence *L,int x,int y,int now);
void Delete(Sequence *L);

// 用于bfs
typedef struct Queue{
    Point a[MAXSIZE];
    int front,rear;
} Queue;

void Init(Queue **q);
int empty(Queue q);   //判空
int pop(Queue **q);    //出队
int push(Queue **q,Point p);   //入队
Point front(Queue q); //获得队头坐标

//extern Queue q; //


int putStone(int x, int y, int stone);
int removeStone(int x, int y);
int checkStone(int x, int y);
int getpos(int x);
int getpos_kboard(std::string,int a[]);
int bfs(int x, int y, int current, Queue *save,Queue *q); // 广度优先搜索
int go_check(int x,int y);  // 提子算法
void regret(); // 悔棋函数
void savefile(char *s);
void openfile(char *s);
#endif // ALGORITHM_H


