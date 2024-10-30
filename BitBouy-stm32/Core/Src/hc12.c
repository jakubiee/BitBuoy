#include "hc12.h"

static UART_HandleTypeDef *huart_hc12;
static UART_HandleTypeDef *huart_debug;


void HC12_Init(UART_HandleTypeDef *uart_hc12, UART_HandleTypeDef *uart_debug) {
    huart_hc12 = uart_hc12;
    huart_debug = uart_debug;
}

void HC12_Send_Message(const char *message) {
    HAL_UART_Transmit(huart_hc12, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

    if (huart_debug != NULL) {
        char confirm[] = "Message sent via HC-12.\r\n";
        HAL_UART_Transmit(huart_debug, (uint8_t*)confirm, strlen(confirm), HAL_MAX_DELAY);
    }

    HAL_Delay(2000);
}


void HC12_Send_Data_With_Confirmation(const char *confirmation_request, const char *message, Queue *q) {
    int attempts = 0;
    int max_attempts = 3;
    char received_char;
    char response[20] = {0};

    while (attempts < max_attempts) {
        HAL_UART_Transmit(huart_hc12, (uint8_t*)confirmation_request, strlen(confirmation_request), HAL_MAX_DELAY);

        int i = 0;
        memset(response, 0, sizeof(response));

        while (HAL_UART_Receive(huart_hc12, (uint8_t*)&received_char, 1, 2000) == HAL_OK) {
            response[i++] = received_char;

            if (huart_debug != NULL) {
                HAL_UART_Transmit(huart_debug, (uint8_t*)&received_char, 1, HAL_MAX_DELAY);
            }

            if (received_char == '\n' || i >= sizeof(response) - 1) {
                break;
            }
        }

        int len = strlen(response);
        while (len > 0 && (response[len - 1] == '\r' || response[len - 1] == '\n')) {
            response[--len] = '\0';
        }

        if (strcmp(response, "OK ABCD1234") == 0) {
            HAL_UART_Transmit(huart_hc12, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

            if (huart_debug != NULL) {
                char confirm[] = "Message sent via HC-12.\r\n";
                HAL_UART_Transmit(huart_debug, (uint8_t*)confirm, strlen(confirm), HAL_MAX_DELAY);
            }
            return;
        }

        attempts++;

        if (huart_debug != NULL) {
            char retry[] = "Retrying...\r\n";
            HAL_UART_Transmit(huart_debug, (uint8_t*)retry, strlen(retry), HAL_MAX_DELAY);
        }

        HAL_Delay(5000);
    }

    if (!enqueue(q, message)) {
        if (huart_debug != NULL) {
            char queue_full[] = "Queue is full, cannot add message.\r\n";
            HAL_UART_Transmit(huart_debug, (uint8_t*)queue_full, strlen(queue_full), HAL_MAX_DELAY);
        }
    } else {
        if (huart_debug != NULL) {
            char message_in_queue[] = "Message added to queue after failed attempts.\r\n";
            HAL_UART_Transmit(huart_debug, (uint8_t*)message_in_queue, strlen(message_in_queue), HAL_MAX_DELAY);
        }
    }

    if (huart_debug != NULL) {
        char error[] = "Failed to send message after 3 attempts, added to queue.\r\n";
        HAL_UART_Transmit(huart_debug, (uint8_t*)error, strlen(error), HAL_MAX_DELAY);
    }
}
