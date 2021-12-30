#include "algorithm.h"
#include <QDebug>
int board[19][19];
int currentstone = NONE;
int isempty = 1;
int regret_time = 0;
int pass_time = 0;
int vis[19][19];
Statement last_3 = (State *)malloc(sizeof(State));
Sequence L = (SeqNode *)malloc(sizeof(SeqNode));
int count = 0;
int step = 0;

void InitState(Statement *last_3)
{
    State *last_2 = (State *)malloc(sizeof(State)),*last_1 = (State*)malloc(sizeof(State));
    for (int i= 0;i<boardsize;i++)
    {
        for (int j= 0;j<boardsize;j++)
        {
            last_2->board[i][j] = NONE;
            last_1->board[i][j] = NONE;
            (*last_3)->board[i][j] = NONE;
        }
    }
    (*last_3)->next = last_2;
    last_2->next = last_1;
    last_1->next = NULL;
}

int Compare(int board1[][19],int board2[][19])
{
    for (int i= 0;i<boardsize;i++)
    {
        for (int j= 0;j<boardsize;j++)
        {
            if(board1[i][j] == board2[i][j]) continue;
            else return 1;
        }
    }
    return  0;
}

int AddState(Statement *last_3,int board[][19])
{
    State *tem = (State *)malloc(sizeof(State));
    State *p = (*last_3);
    for (int i= 0;i<boardsize;i++)
    {
        for (int j= 0;j<boardsize;j++)
        {
            tem->board[i][j] = board[i][j];
        }
    }
    tem->next = NULL;
    if (Compare((*last_3)->next->board,tem->board))
    {

        (*last_3) = (*last_3)->next;
        free(p);
        (*last_3)->next->next = tem;
        return 1;
    }
    else
    {
        for (int i= 0;i<boardsize;i++)
        {
            for (int j= 0;j<boardsize;j++)
            {
                board[i][j] = (*last_3)->next->next->board[i][j];
            }
        }
        free(tem);
        return 0;
    }

}

// 保存有效的落子序列  以便用文件保存
void InitSeq(Sequence *L)
{
    (*L)->x = -1;
    (*L)->y = -1;
    (*L)->now = NONE;
    (*L)->last = NULL;
    (*L)->next = NULL;
}

void InsertAfter(Sequence *L,int x,int y,int now)
{
    SeqNode *q = (*L);
    SeqNode *p = (SeqNode *)malloc(sizeof(SeqNode));
    p->x = x;
    p->y = y;
    p->now = now;

    p->next = NULL;
    while (q->next) {
        q = q->next;
    }
    p->last = q;
    q->next = p;
}

void Delete(Sequence *L)
{
    if ((*L)->next == NULL) return;
    else
    {
        SeqNode *q = (*L),*r = (*L)->next;
        while (r->next) {
            q = q->next;
            r = r->next;
        }
        q->next = NULL;
        free(r);
    }
}

void Init(Queue **q)
{
    *q = (Queue *) malloc(sizeof(Queue));
    (*q)->front = 0;
    (*q)->rear = 0;
}

int empty(Queue q)   //判空
{
    if (q.front == q.rear) return 1;
    else return 0;
}

int pop(Queue **q)   //出队
{
    if (empty(**q)) return -1;
    else
    {
        (*q)->front = ((*q)->front + 1)%MAXSIZE;
        return 0;
    }
}
int push(Queue **q,Point p)   //入队
{
    if (((*q)->rear + 1)%MAXSIZE == (*q)->front) return -1;
    else
    {
        (*q)->a[(*q)->rear] = p;
        (*q)->rear = ((*q)->rear + 1)%MAXSIZE;
        return 0;
    }
}

Point front(Queue q) //获得队头坐标
{
    Point p = q.a[q.front];
    return p;
}

int putStone(int x, int y, int stone){
    if(x < boardsize && y < boardsize && stone != NONE){
        if(checkStone(x,y) == NONE){
            board[x][y] = stone;
            return 1;
        }
    }
    return 0;
}

int removeStone(int x, int y){
    if(x < boardsize && y < boardsize){
        board[x][y] = NONE;
        return 1;
    }
    return 0;
}

//悔棋
void regret()
{
    regret_time = 0;
    State * last_one = last_3->next->next;
    for (int i= 0;i<boardsize;i++)
    {
        for (int j= 0;j<boardsize;j++)
        {
            last_one->board[i][j] = last_3->board[i][j];
            board[i][j] = last_one->board[i][j];
            last_3->board[i][j] = NONE;
            last_3->next->board[i][j] = NONE;
        }
    }

}
// 返回x，y处棋子的状态
int checkStone(int x, int y){
    return board[x][y];
}

// 根据鼠标位置获得坐标
int getpos(int x)
{
    x = x - padding + interval/2;
    int pos = x/interval;
    return pos;
}

//将自输入的字符串转成坐标
int getpos_kboard(std::string s,int a[])
{
    int num = 0;
    a[1] = -1;
    for (int i = 0;i < s.length();i++)
    {
        if (s[i] == ' ' && (num == 0 || num == 1)) continue;
        else if (num == 2 && s[i] != ' ' ) return   0;
        else if(num == 0)
        {
            if (s[i] >= 'a' && s[i] <= 'a'+18)
            {
                a[0] = s[i] - 'a';
                num = 1;
            }
            else if (s[i] >= 'A' && s[i] <= 'A'+18)
            {
                a[0] = s[i] - 'A';
                num = 1;
            }
            else return 0;
        }
        else if (num == 1)
        {
             if (s[i] > '1' && s[i] <= '9')
             {
                 a[1] = s[i] - '1';
                 num++;
             }
             else if((i == s.length() - 1 || s[i+1] == ' ' ) && s[i] == '1' )
             {
                 a[1] = 0;
                 num++;
             }
             else if( s[i] == '1' && s[i+1] <= '9' && s[i+1] >= '0')
             {
                 a[1] = 10 + s[i+1] - '1';
                 num++;
                 i++;
             }
             else return  0;

        }
    }
    // 未获得第二维坐标返回0
    if (a[1] == -1) return 0;
    else return 1;
}

// 有气返回1,没气返回0
int bfs(int x, int y, int current, Queue *save,Queue *q)
{
    //广度优先搜索,如果当前一片区域的棋子没有气,则remove
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //记录四个方向
    Point temp;
    memset(vis,0,sizeof(vis));
    //把当前坐标加入队列
    temp.x = x, temp.y = y;
    push(&q, temp);
    push(&save, temp);
    while (!empty(*q))
    {
        //获取队头坐标
        temp = front(*q);
        pop(&q);
        x = temp.x;
        y = temp.y;
        vis[x][y] = 1;
        //对当前坐标的周围进行查看
        for (int i = 0; i < 4; i++)
        {
            int u, v;
            u = x + dir[i][0];
            v = y + dir[i][1];
            if (u < 0 || u >= 19 || v < 0 || v >= 19)
                continue;
            //坐标 u,v 没有棋子 代表有气存在 返回true
            if (checkStone(u, v) == NONE)
                return 1;
            //否则 将坐标u,v加入队列
            else if (checkStone(u, v) == current && !vis[u][v])
            {
                temp.x = u;
                temp.y = v;
                push(&q, temp);
                push(&save, temp);
            }
        }
    }
    return 0;
}

int go_check(int x,int y)
{

    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //记录四个方向
    int u, v;
    //初始化vis
    memset(vis,0,sizeof(vis));

    Queue *save,*q; //save保存要拿走的棋子 q广度搜索队列
    Init(&save);
    Init(&q);
    // 1.先看周围的对手的棋子是否有气, 没有气的区域,有的话则拿走
    for (int i = 0; i < 4; i++)
    {
        u = x + dir[i][0];
        v = y + dir[i][1];
        if (u < 0 || u >= 19 || v < 0 || v >= 19)
            continue;
        //如果是没有访问过的对手的棋子,进行广搜
        if (checkStone(u, v) != NONE && checkStone(x, y) != checkStone(u, v) && !vis[u][v])
        {

            if (bfs(u, v, checkStone(u, v), save,q))
            { //当前区域有气,则把save和q清空
                while (!empty(*save))
                    pop(&save);
                while (!empty(*q))
                    pop(&q);
            }
            else
            { //当前区域没有气,则将save中的坐标拿走棋子

                while (!empty(*save))
                {
                    Point temp = front(*save);
                    pop(&save);
                    removeStone(temp.x, temp.y);
                }
            }
        }
    }
    // 2.再查看当前棋子的区域有没有气,没有气则移除
    if (!bfs(x, y, checkStone(x, y), save,q))
    {
        board[x][y] = NONE;
        return 1;
    }
    else return 0;
}

void openfile(char *s)
{
    FILE *fp = fopen(s,"r");
    InitSeq(&L);
    int x,y,now;
    while (fscanf(fp,"%d%d%d",&x,&y,&now) != -1) {
        InsertAfter(&L,x,y,now);
    }
    fclose(fp);
}

void savefile(char *s)
{
    FILE *fp = fopen(s,"w");
    SeqNode *p = L->next;
    while (p)
    {
        fprintf(fp,"%d %d %d\n",p->x,p->y,p->now);
        p = p->next;
    }
    fclose(fp);
}
