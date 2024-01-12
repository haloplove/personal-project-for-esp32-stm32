/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 *
 *  Created on: 12.01.2019
 *      Author: Mateusz Salamon
 *		 www.msalamon.pl
 *
 *      Website: https://msalamon.pl/obsluga-przerwaniowa-czujnika-laserowego-vl53l0x/
 *      GitHub:  https://github.com/lamik/VL53L0X_API_INT_STM32_HAL
 *      Contact: mateusz@msalamon.pl
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "vl53l0x_api.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t Message[64];
uint8_t MessageLen;
uint8_t answer = 0;
uint8_t isAnswerReceived = 0;

VL53L0X_RangingMeasurementData_t RangingData;
VL53L0X_RangingMeasurementData_t RangingData_2;
VL53L0X_Dev_t vl53l0x_c; // center module
VL53L0X_DEV Dev = &vl53l0x_c;
VL53L0X_Dev_t vl53l0x_c_2; // center module
VL53L0X_DEV Dev_2 = &vl53l0x_c_2;

void My_receive(void);
void My_straight(void); // 直走
void My_left_p(void);	// 左平移
void My_right_p(void);
void My_turn_left(void); // 左转
void My_turn_right(void);
void My_left_vo(void); // 逆时针
void My_right_vo(void);
void My_stop(void); // 停
void My_back(void); // 后退

volatile uint8_t TofDataRead;
volatile uint8_t TofDataRead_2;
int cz = 0;
int distance_1 = 0;
int distance_2 = 0;
int bz = 0;

int speed_m = 15000;
int speed_sizing = 9000;
int maxsizing = 1100;
int minisizing = 1050;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	//
	// VL53L0X initialisation stuff
	//
	uint32_t refSpadCount;
	uint8_t isApertureSpads;
	uint8_t VhvSettings;
	uint8_t PhaseCal;

	uint32_t refSpadCount_2;
	uint8_t isApertureSpads_2;
	uint8_t VhvSettings_2;
	uint8_t PhaseCal_2;

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	MX_I2C3_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();

	/* Initialize interrupts */
	MX_NVIC_Init();
	/* USER CODE BEGIN 2 */
	//  MessageLen = sprintf((char*)Message, "msalamon.pl VL53L0X Continuous mode\n\r");
	//  HAL_UART_Transmit(&huart1, Message, MessageLen, 100);

	Dev->I2cHandle = &hi2c1;
	Dev->I2cDevAddr = 0x52;
	Dev_2->I2cHandle = &hi2c3;
	Dev_2->I2cDevAddr = 0x52;

	HAL_GPIO_WritePin(TOF2_XSHUT_GPIO_Port, TOF2_XSHUT_Pin, GPIO_PIN_RESET); // Disable XSHUT
	HAL_Delay(20);
	HAL_GPIO_WritePin(TOF2_XSHUT_GPIO_Port, TOF2_XSHUT_Pin, GPIO_PIN_SET); // Enable XSHUT
	HAL_Delay(20);

	HAL_NVIC_DisableIRQ(EXTI2_IRQn);

	VL53L0X_WaitDeviceBooted(Dev_2);
	VL53L0X_DataInit(Dev_2);
	VL53L0X_StaticInit(Dev_2);
	VL53L0X_PerformRefCalibration(Dev_2, &VhvSettings_2, &PhaseCal_2);
	VL53L0X_PerformRefSpadManagement(Dev_2, &refSpadCount_2, &isApertureSpads_2);
	VL53L0X_SetDeviceMode(Dev_2, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
	VL53L0X_StartMeasurement(Dev_2);

	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	//     VL53L0X_SetDeviceAddress(Dev_2, 0x54);
	//
	//    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
	//
	//          VL53L0X_WaitDeviceBooted( Dev_2 );
	//          VL53L0X_DataInit( Dev_2 );
	//          VL53L0X_StaticInit( Dev_2 );
	//          VL53L0X_PerformRefCalibration(Dev_2, &VhvSettings_2, &PhaseCal_2);
	//          VL53L0X_PerformRefSpadManagement(Dev_2, &refSpadCount_2, &isApertureSpads_2);
	//          VL53L0X_SetDeviceMode(Dev_2, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
	//          VL53L0X_StartMeasurement(Dev_2);
	//          //HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	//
	//
	//
	//
	HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_RESET); // Disable XSHUT
	HAL_Delay(20);
	HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_SET); // Enable XSHUT
	HAL_Delay(20);

	//
	// VL53L0X init for Single Measurement
	//

	HAL_NVIC_DisableIRQ(EXTI3_IRQn);

	VL53L0X_WaitDeviceBooted(Dev);
	VL53L0X_DataInit(Dev);
	VL53L0X_StaticInit(Dev);
	VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
	VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
	VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
	VL53L0X_StartMeasurement(Dev);

	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	// 电机部分
	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK)
		Error_Handler();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		// if (TofDataRead == 1)
		// {
		// 	MessageLen = sprintf((char *)Message, "Measured distance: %i\n\r", RangingData.RangeMilliMeter);
		// 	HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		// 	TofDataRead = 0;
		// 	HAL_Delay(500);
		// }
		//
		//
		//
		// if (TofDataRead_2 == 1)
		// {
		// 	MessageLen = sprintf((char *)Message, "Measured distance2: %i\n\r", RangingData_2.RangeMilliMeter + 40);
		// 	HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		// 	TofDataRead_2 = 0;
		// 	HAL_Delay(500);
		// }
		// RangingData.RangeMilliMeter

		//	  while(1)
		//	  {
		//
		//		  distance_1=RangingData.RangeMilliMeter;
		//		  distance_2=RangingData_2.RangeMilliMeter+35;
		//		  cz=distance_1-distance_2;
		//
		//		  if(distance_1>maxsizing)//&&distance_2>maxsizing
		//		  {
		//			  MessageLen = sprintf((char*)Message, "超出距离20厘米\n\r");
		//			  HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		//			  HAL_Delay(50);//左平移调�?
		//	          __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed_m);//左移
		//	          __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);//�?1
		//	          __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_m);
		//	          __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//	          __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//	          __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,speed_m);//�?3
		//	          __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
		//	          __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,speed_m);//�?4
		//
		//
		//		  }
		// else if (distance_1 < minisizing) //&&distance_2<minisizing
		// {
		// 	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);	   //右移
		// 	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_m); //�?1
		// 	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
		// 	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed_m); //�?2
		// 	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_m);
		// 	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); //�?3
		// 	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_m);
		// 	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); //�?4
		// 	HAL_Delay(50);
		// }
		// if (cz < -15 && cz > -40)
		//		  {
		//			  MessageLen = sprintf((char*)Message, "车辆�?左转\n\r");
		//			  HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		//			  //HAL_Delay(500);//车辆�?左走
		////	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//左转�?
		////	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_sizing);//�?1
		////	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_sizing);
		////	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		////	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		////	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,speed_m);//�?3
		////	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,speed_m);
		////	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//
		//
		//	          __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//逆时针走
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�?1
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_m);
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,speed_m);
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);//�?3
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,speed_m);//�?4
		//			  HAL_Delay(100);
		//		  }
		//		  else if(cz>15&&cz<40)
		//		  {
		//		  	  MessageLen = sprintf((char*)Message, "车辆�?右转\n\r");
		//		  	  HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		//		  	  //HAL_Delay(500);//车辆右转
		////	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//右转�?
		////	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�?1
		////	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_m);
		////	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		////	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		////	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,14000);//�?3
		////	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,14000);
		////	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//
		//	          __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,(speed_m+2000));//顺时针走
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);//�?1
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,(speed_m+2000));//�?2
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,(speed_m+2000));//�?3
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,(speed_m+2000));
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//			  HAL_Delay(100);
		//		  }
		//		  else
		//		  {
		//			  MessageLen = sprintf((char*)Message, "车辆直走\n\r");
		//			 HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		//			 //HAL_Delay(50);
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//停止
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);//�?1
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);//�?3
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//		      HAL_Delay(50);
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//直走
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_sizing);//�?1
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_sizing);
		//		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,(speed_sizing+2000));//�?3
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,(speed_sizing+2000));
		//		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//		  }
		//	  }

		//
		//
		//
		////距离调试
		//		MessageLen = sprintf((char*)Message, "Measured distance: %i\n\rMeasured distance2: %i\n\r", RangingData.RangeMilliMeter,RangingData_2.RangeMilliMeter+40);
		//		HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		//		HAL_Delay(500);
		//
		//		cz=RangingData.RangeMilliMeter-(RangingData_2.RangeMilliMeter+25);
		//
		//		MessageLen = sprintf((char*)Message, "差�?�为: %i\n\r",cz);
		//		HAL_UART_Transmit(&huart1, Message, MessageLen, 100);
		//		HAL_Delay(300);
		//

		//		  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//右移
		//		  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�?1
		//		  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		//		  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speed_m);//�?2
		//		  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,speed_m);
		//		  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);//�?3
		//		  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,speed_m);
		//		  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//		  		    HAL_Delay(3000);
		//
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//停止
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);//�?1
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);//�?3
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
		//		  		    		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//		  		    		    HAL_Delay(5000);

		//	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//右转�?
		//	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_sizing);//�?1
		//	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_sizing);
		//	  		      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,speed_m);//�?3
		//	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,speed_m);
		//	  		      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4

		//	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//左转�?
		//	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�?1
		//	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_m);
		//	      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,speed_sizing);//�?3
		//	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,speed_sizing);
		//	      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4

		//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//直走
		//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�???1
		// HAL_Delay(4000);
		//	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//直走
		//	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�?1
		//	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		//	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speed_m);//�?2
		//	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,speed_m);//�?3
		//	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
		//	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,speed_m);//�?4
		//	  HAL_Delay(4000);

		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//右平�?
		//  	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�?1
		//  	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_m);
		//  	  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//  	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//  	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,speed_m);//�?3
		//  	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,speed_m);
		//  	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4
		//
		//      HAL_Delay(3000);
		//
		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed_m);//左平�?
		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);//�?1
		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speed_m);//�?2
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,speed_m);
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);//�?3
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,speed_m);//�?4

		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);//左转�?  校准电压差后数�??
		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed_m);//�?1
		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed_m);
		//      __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);//�?2
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,10000);//�?3
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,10000);
		//      __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,0);//�?4

		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			//逆时针走
		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); //�?1
		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); //�?2
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_sizing);
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); //�?3
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed_sizing); //�?4

		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed_sizing); //顺时针走
		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);			//�?1
		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
		// __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed_sizing); //�?2
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_sizing); //�?3
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_sizing);
		// __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); //�?4

		/*HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, RESET);
				HAL_Delay(5000);
			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, SET);
						HAL_Delay(5000);*/

		HAL_UART_Receive_IT(&huart1, &answer, 1); // 中断接收函数
		while (isAnswerReceived == 0)
		{
			My_stop();
		}					  // waiting for answer
		isAnswerReceived = 0; // answer received, reset flag

		if (answer == '5' || answer == '6') // 对接收字符进行判断
		{
			My_straight();
			HAL_Delay(6000);
			bz = answer;
		}

		if (answer == '8')
		{
			My_straight();
			HAL_Delay(2300);

			if (bz == '5')
			{
				My_left_vo();
				HAL_Delay(1575);
				My_straight();
				HAL_Delay(4050);
			}
			else if (bz == '6')
			{
				My_right_vo();
				HAL_Delay(1575);
				My_straight();
				HAL_Delay(4050);
			}
		}

		if (answer == '9')
		{
			My_right_vo();
			HAL_Delay(3000);

			if (bz == '5')
			{
				My_straight();
				HAL_Delay(4050);
				My_right_vo();
				HAL_Delay(1600);
			}
			else if (bz == '6')
			{
				My_straight();
				HAL_Delay(4050);
				My_left_vo();
				HAL_Delay(1600);
			}

			My_straight();
			HAL_Delay(7000);

		} // 小车调试

		//	  if(bz==1)
		//	  {
		//		  My_straight();
		//		  HAL_Delay(5200);
		//		  My_stop();
		//		  HAL_Delay(2000);
		//		  My_straight();
		//		  HAL_Delay(1500);
		//		  My_left_vo();
		//		  HAL_Delay(1050);
		//		  My_straight();
		//		  HAL_Delay(3500);
		//		  bz=0;
		//	  }

		// My_stop();

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief NVIC Configuration.
 * @retval None
 */
static void MX_NVIC_Init(void)
{
	/* EXTI3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == TOF_INT_Pin)
	{
		VL53L0X_GetRangingMeasurementData(Dev, &RangingData);
		VL53L0X_ClearInterruptMask(Dev, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
		TofDataRead = 1;
	}
	else if (GPIO_Pin == TOF2_INT_Pin)
	{
		VL53L0X_GetRangingMeasurementData(Dev_2, &RangingData_2);
		VL53L0X_ClearInterruptMask(Dev_2, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
		TofDataRead_2 = 1;
	}
}

void My_straight(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			// 直走
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); // �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_sizing + 2400); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_sizing + 2400);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
}

void My_receive(void)
{

	HAL_UART_Receive(&huart1, &answer, 1, HAL_MAX_DELAY);

	if (answer == '1')
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			// 直走
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); // �?1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_sizing); // �?3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_sizing);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
		HAL_Delay(50);
	}
	else if (answer == '2')
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			// 左转�?
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); // �?1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_m); // �?3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_m);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
		HAL_Delay(50);
	}
	else if (answer == '3')
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			// 右转�?
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); // �?1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_m); // �?3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_m);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
		HAL_Delay(50);
	}
	else if (answer == '4')
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed_m); // 左移
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);	   // �?1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_m);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_m); // �?3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed_m); // �?4
		HAL_Delay(50);
	}
	else if (answer == '5')
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);	   // 右移
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_m); // �?1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed_m); // �?2
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_m);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_m);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
		HAL_Delay(50);
	}
	else if (answer == '6')
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			// 逆时针走180�?
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); // �?1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_sizing);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed_sizing); // �?4
		HAL_Delay(2400);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); // 停止
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); // �?1
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
		HAL_Delay(50);
	}
}

void My_left_p(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed_m); // 左平�?
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);	   // �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed_m); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_m);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed_m); // �?4
}

void My_right_p(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);	   // 右移
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_m); // �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed_m); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_m);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_m);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
}

void My_turn_left(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);	   // 左转�?  校准电压差后数�??
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_m); // �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_m);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_m); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_m);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
}

void My_turn_right(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			// 右转�?
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); // �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_m); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_m);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
}

void My_left_vo(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);			// 逆时针走
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed_sizing); // �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed_sizing);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_sizing);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed_sizing); // �?4
}

void My_right_vo(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed_sizing); // 顺时针走
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);			// �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed_sizing); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed_sizing); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_sizing);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
}

void My_stop(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); // 停止
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); // �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); // �?4
}

void My_back(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed_sizing); // 直走
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);			// �?1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed_sizing); // �?2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_sizing + 1500);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); // �?3
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed_sizing + 1500); // �?4
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
		isAnswerReceived = 1;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   tex: printf_uart("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
