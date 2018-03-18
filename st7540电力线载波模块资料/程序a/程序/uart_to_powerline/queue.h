/*
文件名：queue.h
时间：2011-1-22

功能：

*/
#ifndef QUEUE_H
#define QUEUE_H

#include "main.h"

typedef struct
{
	u8 start;
	u8 end;
	u8 len;
	u8 *q_data;
	u8 isFull;	//为1标志队列满,为0表示没有填满，此时若非空，start和end一定不等
	u8 data_cnt;
} Queue;

void Queue_ini(u8 *d,u8 num,Queue *q);

//成功返回0，失败返回错误号
s8 Queue_set(s8 d,Queue *q);	
s8 Queue_get(s8 *d,Queue *q);
s8 Queue_pre_read(s8 *d,Queue *q);
u8 Queue_cnt(Queue *q);


#endif