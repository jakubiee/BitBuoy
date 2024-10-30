#ifndef HC12_H
#define HC12_H

#include "stm32l1xx_hal.h"
#include <string.h>
#include "queue.h"

void HC12_Init(UART_HandleTypeDef *huart_hc12, UART_HandleTypeDef *huart_debug);
void HC12_Send_Message(const char *message);
void HC12_Send_Data_With_Confirmation(const char *confirmation_request, const char *message, Queue *q);

#endif
