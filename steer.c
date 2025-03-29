#include "main.h"
#include <string.h>
#include <stdio.h>

UART_HandleTypeDef huart2;

void setMotorSpeed(uint8_t motor, int8_t speed);
uint8_t UART_Receive(void);

int main(void) {
    HAL_Init();
    
    // Initialize peripherals
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    uint8_t receivedData;

    while (1) {
        receivedData = UART_Receive();
        if (receivedData != 0) {
            switch (receivedData) {
                case 'S':  
                    setMotorSpeed(0, -50);  
                    break;
                case 'W':  
                    setMotorSpeed(0, 50);  
                    break;
                case 'D': 
                    setMotorSpeed(1, 50);  
                    break;
                case 'A':
                    setMotorSpeed(1, -50); 
                    break;
                case 'X':  
                    setMotorSpeed(0, 0);
                    setMotorSpeed(1, 0);
                    break;
                default:
                    break;
            }
        }
    }
}

// Function to set motor speed
void setMotorSpeed(uint8_t motor, int8_t speed) {
    uint8_t direction = (speed >= 0) ? 0 : 1;
    uint8_t absSpeed = (speed >= 0) ? speed : -speed;
    uint8_t data = (motor << 7) | (direction << 6) | (absSpeed & 0x3F);
    
    // Transmit motor control data
    HAL_UART_Transmit(&huart2, &data, 1, HAL_MAX_DELAY);

    // Feedback message
    char feedback[50];
    snprintf(feedback, sizeof(feedback), "Motor %d set to speed %d\r\n", motor, speed);
    HAL_UART_Transmit(&huart2, (uint8_t*)feedback, strlen(feedback), HAL_MAX_DELAY);
}

// Function to receive UART data
uint8_t UART_Receive(void) {
    uint8_t data;
    if (HAL_UART_Receive(&huart2, &data, 1, 100) == HAL_OK) {
        return data;
    }
    return 0;  // No valid data received
}

//do system intializations 
