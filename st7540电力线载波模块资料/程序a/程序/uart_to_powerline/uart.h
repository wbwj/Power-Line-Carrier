/*
ÀÖµçĞÂÄÏ

*/
#ifndef UART_H
#define UART_H

#include "main.h"
#include "queue.h"

void uart_ini(u16 b);

char data_to_uart(void);

extern Queue uart_rx_queue;
extern Queue uart_tx_queue;

#endif
