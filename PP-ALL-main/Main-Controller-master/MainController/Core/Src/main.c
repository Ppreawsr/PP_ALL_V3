/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "subroutine.h"
#include "ModBusRTU.h"
#include "arm_math.h"
#include <math.h>
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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim16;
DMA_HandleTypeDef hdma_tim3_ch2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
////*JOYSTICK PART
// @User: Joystick receiving buffer
uint8_t joystickPayload[1] = {0};

// @User: Encoder's raw angle from QEI
uint32_t qeiRaw = 0;
float qeifloat = 0.0;
float TT;
uint8_t C;
uint8_t Va;
uint16_t testing = 0;
// @User: Base system receiving buffer


// @User: Motor's parameter
volatile uint8_t zStop = 0;

///*MODBUS PART
ModbusHandleTypedef hmodbus;

//flag
uint8_t flagShelf = 0; //set shelf
uint16_t piingpong; //movement

uint8_t vacuum;
uint8_t gripper;
uint8_t reed;
uint16_t rState1;
uint16_t rState2;
float linear_velocity;
float linear_accel;
float linear_position;

u16u8_t registerFrame[200]; //middle between base n z axis (they will see the same.)
uint16_t shelfPos[5];
uint16_t setPos;
uint16_t x_pos; // x - axis
uint8_t deb = 0;
uint8_t mode;

uint16_t temPick; // temporary shelves order
uint16_t temPlace;
uint16_t pick[5]; // shelves order array
uint16_t place[5];
uint8_t rnd = 0; // jog counter
uint8_t rnd2;
uint8_t load;

///*Trajectory PART
uint64_t x = 0;
uint8_t xx;
float time_op = 2;    // stop time in seconds
float position_goal = 0.0;   // stop position in meters
float position_now = 0;
float total_displacement = 0;
float total_time = 0;
float t_acc =0;
float t_const = 0;
float Peak = 0;
uint64_t i = 0;
int num_points = 2000;
float t = 0;
float velocity;
float position;
float startTime;
uint16_t Mode = 0;
uint16_t St;
uint16_t Home = 0;
//uint64_t _micros = 0;
float Vfeedback = 0.0;
float VInM = 0;
arm_pid_instance_f32 PID = {0};
uint16_t dir;
float BTempV;
int pwmM;
uint32_t qeirawread;
uint32_t spd;
uint32_t pos;
float err = 0.008733333333;
float pos_err;
float err_g;
uint16_t rou = 0;
uint16_t Box = 0;
uint16_t Ho = 0;
uint16_t PP = 0;
uint64_t Time_R = 0;
uint64_t Rout;

uint8_t buf[3] = {2,2,3};
uint8_t L_State;
uint8_t L;
uint8_t fin;
int16_t VT;
uint64_t currentTime;
uint8_t end = 0;
uint8_t X1 = 0;
uint8_t X2 = 0;
/////
typedef struct
{
// for record New / Old value to calculate dx / dt
uint64_t Positions[2];
uint64_t TimeStamps[2];
float QEIPostion_1turn;
float QEIAngularVelocity[2];
float QEILinearAcc;
}QEI_StructureTypeDef;
QEI_StructureTypeDef QEIdata = {0};
uint64_t _micros = 0;
enum
{
NEW,OLD
};
//////

int64_t diffPosition;
float diffTime;
float diffVelocity;
float diffVelocitys;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */
void BaseAction(void);
void OrderSeparate(void);
void ControlJoy(void);
void ACC_Update(void);
void generate_trapezoidal_velocity_profile(double t2, double x2);
float PlantSimulation(float VIn) ;
uint64_t micros();
float QEIEncoderPosVel_Update();
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM16_Init();
  MX_TIM4_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  ///TRAJECTORY
  PID.Kp = 6.1;
  PID.Ki = 0.001;
  PID.Kd = 0.01;

  arm_pid_init_f32(&PID, 0);
  // @User: Setup UART 1 for communication with joy stick
  HAL_UART_Receive_DMA(&huart1, joystickPayload, 1);

  // @User: Setup UART 2 for communication with Base system
  HAL_TIM_Base_Start_IT(&htim5); //open Timer 5 (interrupt)
  hmodbus.huart = &huart2;
  hmodbus.htim = &htim16;
  hmodbus.slaveAddress = 0x15;
  hmodbus.RegisterSize =200;
  Modbus_init(&hmodbus, registerFrame);
  reed = 0;
  gripper = 0;
  vacuum = 0;

  shelfPos[0] = 400;
  shelfPos[1] = 1641;
  shelfPos[2] = 3236;
  shelfPos[3] = 4442;
  shelfPos[4] = 5850;

  // @User: Setup Timer 1 for Motor drive
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

  // @User: Setup Timer 2 as encoder interface
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);


  // @User: Setup Timer 3 for sending led data
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_Base_Start_IT(&htim7);
//
  // TODO: Test subroutine
//  uint16_t result = retractX();
//  uint16_t result = extendX();
//  HAL_TIM_Base_Start_IT(&htim4);
//  uint8_t result = HomeZ();
//  __HAL_TIM_SET_COUNTER(&htim2, 0);


//  if (position_goal >= 300)
//  {
//	  err_g =  (position_goal * err);
//
//	  pos_err = (position_goal - err_g)+2;
//  }
//
//  else if (position_goal < 300)
//  {
//	  pos_err = position_goal;
//  }
  registerFrame[0x00].U16 = 22881;
  Modbus_Protocal_Worker();

  buf[0] = 3;
  L_State = updateLED(buf,&htim3,TIM_CHANNEL_2);
  piingpong = 0;
  uint8_t result = HomeZ();
  HAL_Delay(2000);
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  generate_trapezoidal_velocity_profile(time_op,16.0);
  Home = 0;
  rou = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  piingpong =1;
//	  Mode = 1;

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Modbus_Protocal_Worker();
	  BaseAction();
	  OrderSeparate();
	  ControlJoy();
//	  uint8_t result = HomeZ();

//	  static uint64_t timestamps =0;
//	  if (rou >= 2 && end == 0)
//	  {
//		  currentTime = micros();
//	  }
//	  else if (end == 1)
//	  {
//		  __HAL_TIM_SET_COUNTER(&htim7, 0);
//		  currentTime = 0;
//		  _micros = 0;
////		  end = 0;

	  static uint64_t timestampqei =0;
	  int64_t currentTimeqei = micros();
	  if(currentTimeqei > timestampqei)
	  {
		  timestampqei = currentTimeqei + 100000;//us
	  	  ACC_Update();
	  }

//	  }
	  if(fin == 1 && fabs((( (setPos+160)/10.0) - qeifloat))  >= 0.8)
	  {
		  generate_trapezoidal_velocity_profile(time_op,(setPos/10.0)+16.0);

	  }
	  xx = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
//	  currentTime = __HAL_TIM_GET_COUNTER(&htim15);

//	  generate_trapezoidal_velocity_profile(time_op,setPos/10.0);

//	  if ( St == 1)
//	  {
//		  if (qeirawread <= (8192*5))
////		  	  {
//		  		  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 30000);
//		  		  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
//		  	  }
//	  }
	  //TRAJECTORY


	  // TODO: Test encoder QEI, remove later
	  qeiRaw  = __HAL_TIM_GET_COUNTER(&htim2);
	  qeifloat = (__HAL_TIM_GET_COUNTER(&htim2))*(60.00/8192);


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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIMEx_BreakInputConfigTypeDef sBreakInputConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakInputConfig.Source = TIM_BREAKINPUTSOURCE_BKIN;
  sBreakInputConfig.Enable = TIM_BREAKINPUTSOURCE_ENABLE;
  sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_HIGH;
  if (HAL_TIMEx_ConfigBreakInput(&htim1, TIM_BREAKINPUT_BRK, &sBreakInputConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;
  sBreakDeadTimeConfig.BreakFilter = 15;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 2;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 2;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 106;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 169;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 16999;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1999;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 169;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 169;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 999;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA2_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB10 PB4 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_10|GPIO_PIN_4|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void ControlJoy(){

	  if ( joystickPayload[0] == 0x80 && registerFrame[0x10].U16 == 0x0)
	  {
		  generate_trapezoidal_velocity_profile(time_op,16.0);
		  joystickPayload[0] = 0;

	  }

	  if ( joystickPayload[0] == 0x80 && registerFrame[0x10].U16 == 0x1)
	  				{
	  					if (TT == 0)
	  					{
	  						shelfPos[0] = (qeifloat*10.0)-160;

	  					}

	  					else if (TT == 1)
	  							{
	  						shelfPos[1] = (qeifloat*10.0) -160;

	  							}

	  					else if (TT == 2)
	  							{
	  						shelfPos[2] = (qeifloat*10.0)-160;

	  							}

	  					else if (TT == 3)
	  							{
	  						shelfPos[3] = (qeifloat*10.0)-160;

	  							}

	  					else if (TT == 4)
	  							{
	  						shelfPos[4] = (qeifloat*10.0)-160;

	  							}
	  					else if ( TT >=  5)
	  					{
	  						flagShelf = 1;
	  					}
	  					TT += 1;
	  					joystickPayload[0] = 0;
	  				}

	  else if ( joystickPayload[0] == 0x40)
		{
//		  //generate_trapezoidal_velocity_profile(time_op, ((setPos/10.0) + 0.1));
		  setPos += 1;
		  joystickPayload[0] = 0 ;
		}
	  else if ( joystickPayload[0] == 0x41)
	  		{
		  setPos += 10;
		  joystickPayload[0] = 0 ;
	  		}
	  else if ( joystickPayload[0] == 0x42)
	  		{
		  setPos += 100;
		  joystickPayload[0] = 0 ;
	  		}
	  else if ( joystickPayload[0] == 0x43)
	  		{
		  setPos += 1000;
		  joystickPayload[0] = 0 ;
	  		}
//	  else if ( joystickPayload[0] == 0x00)
//	  		{
//		  generate_trapezoidal_velocity_profile(time_op, (position - 0.1));
//		  joystickPayload[0] = 0 ;
//	  		}
	  	  else if ( joystickPayload[0] == 0x01 && (setPos-10) >= 0)
	  	  		{
	  		setPos -= 10;
	  		joystickPayload[0] = 0 ;
	  	  		}
	  	  else if ( joystickPayload[0] == 0x02 && (setPos-100) >= 0)
	  	  		{
	  		setPos -= 100;
	  		joystickPayload[0] = 0 ;
	  	  		}
	  	  else if ( joystickPayload[0] == 0x03 && (setPos-1000) >= 0)
	  	  		{
	  		setPos -= 1000;
	  		joystickPayload[0] = 0 ;
	  	  		}

	  	  if ( joystickPayload[0] == 0x82 )
	  	  {
	  		  if (Va == 0)
	  		  {
	  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
	  			Va = 1;
		  		joystickPayload[0] = 0 ;
	  		  }
	  		  else if ( Va == 1)
	  		  {
	  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);
	  			Va = 0;
	  			joystickPayload[0] = 0 ;
	  		  }
	  	  }

	  	  if ( joystickPayload[0] == 0x81 )
	  	  {
	  		 if (C == 0)
	  		 {
	  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
	  			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
	  			if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET )
	  				{
	  				  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
	  				  	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
	  				}
	  			joystickPayload[0] = 0 ;
	  			C = 1;
	  		 }
	  		 else if ( C == 1)
	  		 {
	  			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
	  			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 0 );
	  			if ( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET )
	  			{
	  				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
	  				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
	  			}
	  			joystickPayload[0] = 0 ;
	  			C = 0;
	  		 }
	  	  }

}
///TRAJECTORY
void generate_trapezoidal_velocity_profile(double t2, double x2) {
  // Total displacement and time interval
	total_displacement = x2 - qeifloat;
	total_time = t2;

  // Calculate optimal acceleration time (t_acc) - Assume a reasonable value
	t_acc = total_time / 4;  // This is an assumption; you can adjust it

  // Remaining time for constant velocity phase
	t_const = total_time - 2 * t_acc;

  // Calculate peak velocity
	Peak = total_displacement / (t_acc + t_const);

//	HAL_TIM_Base_Start_IT(&htim4);

	piingpong = 0;
//	if (total_displacement <= 0.1)
//	{
//		piingpong = 1;
//	}
//	else
//	{
	HAL_TIM_Base_Start_IT(&htim4);
//		piingpong = 0;
//	}
	deb++;
	fin = 0;
	Mode = 0;

  }

void generate_Velocity()
{
//////acc//////////////

////////////////////
		t = (time_op) * i / num_points;
		if (t < t_acc) {
			velocity = (Peak * (t / t_acc));
		} else if (t >= t_acc && t <+ t_acc + t_const) {
			velocity = Peak;
		} else {
			velocity = (Peak * ((time_op - t) / t_acc));
		}
		position += velocity/1000.0;

		position_now = position;

		Vfeedback = arm_pid_f32(&PID, position_now - qeifloat);



		BTempV = Vfeedback;

		VInM = Vfeedback * (24.0/65535.0);

		//		        pwmM = Vfeedback * (65535.0/24.0);


		//		        if (Vfeedback < 9830)
		//		        {
		//		        	Vfeedback = 9830;
		//		        }

		zStop = 0;
		pwmM = Vfeedback * (65535.0/24.0);

		if(Vfeedback >= 0)
		{
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwmM);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);


		}

		else if (Vfeedback < 0)
		{

			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwmM*-1);
		}



		//		        qei = PlantSimulation(Vfeeback);

		if ( i >= time_op*1000)
		{
			__HAL_TIM_SET_COUNTER(&htim4, 0);
			HAL_TIM_Base_Stop_IT(&htim4);
			rou += 1;
			i = 0;
			buf[0] = 2;
			L_State = updateLED(buf,&htim3,TIM_CHANNEL_2);
			if ( registerFrame[0x10].U16 != 0)
			{
				piingpong  = 1;

			}
			fin = 1;
//					        	Vfeedback = 0;
//
//					        	total_displacement = 0;
//					        	t_acc = 0;
//				        	t_const = 0;
//				        	Peak = 0;
//				        	t = 0;
//				        	velocity = 0;
//				        	position = 0;
//
//				        	pwmM = 0;

			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 3000);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);

				        }

}

float PlantSimulation(float VIn) // run with fix frequency
{
	static float speed =0;
	static float qei =0;
	float current= VIn - speed * 0.0123;
	float torque = current * 0.456;
	float acc = torque * 0.789;
	speed += acc;
	qei += speed;
	return qei;
}

//uint64_t micros()
//{
//return __HAL_TIM_GET_COUNTER(&htim2)+_micros;
//}

//
void BaseAction(void){

	static uint64_t timestamp1 = 0;
	  static uint64_t timestamp2 = 0;
	  static uint64_t timestamp3 = 0;
	  static uint64_t timestamp4 = 0;

	  position_goal = setPos/10.0;
	  if (velocity < 0)
	  {
		  linear_velocity = QEIdata.QEIAngularVelocity[NEW] * -1;
	  }

	  else if (velocity >= 0)
	  {
		  linear_velocity = QEIdata.QEIAngularVelocity[NEW];
	  }

	  if (QEIdata.QEILinearAcc < 0)
	  	  {
		  	  linear_accel = QEIdata.QEILinearAcc * -1;
	  	  }

	  	  else if (QEIdata.QEILinearAcc >= 0)
	  	  {
	  		  linear_accel = QEIdata.QEILinearAcc;
	  	  }


	  //linear_accel = accerelometer;
	  linear_position = qeifloat-16.0 ;

	  rState1=  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
	  rState2=  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);


	// Vacuum & Gripper
	  vacuum = registerFrame[0x02].U16;
	  gripper = registerFrame[0x03].U16;	  //1 forward 0 backward
	  reed = rState1 + (2 * rState2); //for 2 back 1
	  registerFrame[0x04].U16 = (reed*2%3);


	  registerFrame[0x11].U16 = (float)(linear_position * 10); //Position
	  registerFrame[0x12].U16 = (float)(linear_velocity * 10); //Velocity
	  registerFrame[0x13].U16 = (float)(linear_accel * 10)	; //Acceleration
	  registerFrame[0x40].U16 = x_pos; // X-axis Position

	  if (vacuum == 1)
	  {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
	  }
	  else
	  {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);
	  }

	  if (gripper == 0 && reed != 1)
	  {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );

		  //HAL_TIM_Base_Start(&htim1);
//		  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//		  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
//		  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 6000);
//		  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);

//		  if ( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET )
//		  {
//	  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
//	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
//		  }
	  }
	  else if (gripper == 1 && reed != 2)
	  {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 0 );
////		  HAL_TIM_Base_Start(&htim1);
//		  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//		  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
//		  if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET )
//		  {
//			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
//			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
//		  }

	  }
	  if((gripper == 1 && reed == 2) || (gripper == 0 && reed == 1)){
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
	  }



	  //heartbeat protocal & debug
	  //registerFrame[0x00].U16 = 22881;
	  if(registerFrame[0x00].U16 != 22881){
		  registerFrame[0x00].U16 = 22881;
		  //deb++;
	  }

	  /////Set shelves
	  if(registerFrame[0x01].U16 == 1) // order mode 1 -> open setshelf
	  {
		  registerFrame[0x01].U16 = 0; //change to idle mode
		  registerFrame[0x10].U16 = 1; //current state set shelf mode
//		  set_position_box();
	  }
	  if(registerFrame[0x10].U16 == 1 && flagShelf == 1)
	  {
		  registerFrame[0x23].U16 = shelfPos[0];
		  registerFrame[0x24].U16 = shelfPos[1];
		  registerFrame[0x25].U16 = shelfPos[2];
		  registerFrame[0x26].U16 = shelfPos[3];
		  registerFrame[0x27].U16 = shelfPos[4];
		  registerFrame[0x10].U16 = 0; //
		  flagShelf = 0;
	  }

		///////1
	  else if((registerFrame[0x01].U16 == 2)) //go to mode 2: Home
	  {
		  (registerFrame[0x01].U16) = 0; //reset status
		  (registerFrame[0x10].U16) = 2; //Z-home
		  		//setPos = shelfPos[0]; // set goal to home
		  setPos = 0;
//		  		  uint8_t result = HomeZ();
//		  		  HAL_Delay(2000);
//		  		  generate_trapezoidal_velocity_profile(time_op,setPos/10.0);
		  Home = 0;
		  rou = 0;
	  }
	/////point mode
	  else if((registerFrame[0x01].U16) == 8)
	  {
		  (registerFrame[0x01].U16) = 0; //reset status
		  (registerFrame[0x10].U16) = 16; // Z-go point

		  setPos = ((registerFrame[0x30].U16)); // goal = point 30->base system 4 point mode
				//generate_trapezoidal_velocity_profile(time_op,setPos);
	  }
	/////jog mode
	  //read (convert to array)
	  else if((registerFrame[0x01].U16 ==4))
	  {
		  (registerFrame[0x01].U16) = 0; //reset status

		  temPick = (registerFrame[0x21].U16);
		  temPlace = (registerFrame[0x22].U16);

		  OrderSeparate();
	  }
			//run
	  else if(rnd > 0)
	  {
				//first round
		  if(registerFrame[0x10].U16 == 0 && rnd== 5 && gripper == 0 && reed == 1 && vacuum == 0) // first rev
		  {
			  mode = 9; // for debug - pick
			  gripper = 0;
			  vacuum = 0;
			  (registerFrame[0x10].U16) = 4; // Z-go pick
			  setPos = shelfPos[pick[5-rnd]-1];
		  }
		  if((piingpong && registerFrame[0x10].U16 == 8)) // prev mode: pick, do place
		  {
				///////place down
			  if(mode == 6){
				  timestamp1 = HAL_GetTick() + 400; // delay before gripper move
				  mode = 60;
			  }
			  if(reed != 2 && rnd> 0 && vacuum == 1 && gripper == 0 && HAL_GetTick() >= timestamp1){
				  registerFrame[0x03].U16 = 1; // gripper forward
				  timestamp2 = HAL_GetTick() + 100; //delay before release box
				  mode = 61;
			  }
			  else if(reed == 2 && vacuum == 1 && HAL_GetTick() >= timestamp2)//reached reed vacuum not off
			  {
				  registerFrame[0x02].U16 = 0; //vacuum off
				  timestamp3 = HAL_GetTick() + 100; // delay before retract gripper
				  mode = 62;
			  }
			  else if(vacuum == 0 && gripper == 1 && HAL_GetTick() >= timestamp3)
			  {
				  registerFrame[0x03].U16 = 0; //gripper backward
				  load = 0;
				  timestamp4 = HAL_GetTick() + 50; // delay before moving to pick
				  mode = 63;
			  }
				///////finish place -> move on
			  if(gripper == 0 && reed == 1 && vacuum == 0 && HAL_GetTick() >= timestamp4)
			  {
				  rnd--;
				  if(rnd>0)
				  {
					  (registerFrame[0x10].U16) = 4; // Z-go pick
					  setPos = shelfPos[pick[5-rnd]-1];
					  mode = 9;
				  }
			  }
		  }
		  else if(piingpong && registerFrame[0x10].U16 == 4)// prev mode: place, do pick
		  {
				//////pick up
			  if(mode == 9){
				  timestamp1 = HAL_GetTick() + 50; // delay before gripper go pick
				  mode = 90;
			  }
			  if(reed == 1 && gripper == 0 && vacuum == 0 && HAL_GetTick() >= timestamp1)
			  {
				  registerFrame[0x03].U16 = 1; //gripper forward
				  timestamp2 = HAL_GetTick() + 200; // delay before vacuum on
				  mode = 91;
			  }
			  else if(gripper == 1 && vacuum == 0 && HAL_GetTick() >= timestamp2) //if vacuum off
			  {
				  registerFrame[0x02].U16 = 1; //vacuum on
				  timestamp3 = HAL_GetTick() + 400; // delay before pulling box back
				  mode = 92;

			  }
			  else if(reed == 2 && gripper == 1 && vacuum == 1 && HAL_GetTick() >= timestamp3)
			  {
				  registerFrame[0x03].U16 = 0; //gripper backward
				  load = 1;
				  timestamp4 = HAL_GetTick() + 400; // delay before moving again
				  mode = 93;
			  }
				///////finish pick -> move on
			  if(gripper == 0 && reed == 1 && vacuum == 1 && HAL_GetTick() >= timestamp4)
			  {
				  (registerFrame[0x10].U16) = 8; // Z-go place
				  if(place[5-rnd] == 5)
				  {
					  setPos = shelfPos[place[5-rnd]-1] + 100; //bon  goal + 5mm
				  }
				  else
				  {
					  setPos = shelfPos[place[5-rnd]-1] + 150; // goal + 5mm
				  }
				  mode = 6;
			  }
		  }
	  }

	  //end jog
	  else if(registerFrame[0x10].U16 == 8 && rnd== 0)
	  {
		  (registerFrame[0x10].U16 = 0); // End Jogs
		  mode = 255;
	  }
		/////End point & Home
	  else if(piingpong && (registerFrame[0x10].U16 == 2 || registerFrame[0x10].U16 == 16))
	  {
		  //finish point & home mode
		  static uint64_t Timestamp = 0;
		  if(rnd2 == 0){
			  Timestamp = HAL_GetTick() + 1000; //delay
			  rnd2 = 1;
		  }
		  else if(HAL_GetTick() >= Timestamp)
		  {
			  registerFrame[0x10].U16 = 0; // finish point & home mode
			  rnd2 = 0;
		  }
	  }
	  if(registerFrame[0x10].U16 == 0){
		  piingpong = 0;
	  }
}

//
//
////void BaseActionV2(void){
//
//	static uint16_t timestamp1 = 0;
//	  static uint16_t timestamp2 = 0;
//	  static uint16_t timestamp3 = 0;
//	  static uint16_t timestamp4 = 0;
//
//	  position_goal = setPos/10.0;
//	  if (velocity < 0)
//	  {
//		  linear_velocity = QEIdata.QEIAngularVelocity[NEW] * -1;
//	  }
//
//	  else if (velocity >= 0)
//	  {
//		  linear_velocity = QEIdata.QEIAngularVelocity[NEW];
//	  }
//
//	  if (QEIdata.QEILinearAcc < 0)
//	  	  {
//		  	  linear_accel = QEIdata.QEILinearAcc * -1;
//	  	  }
//
//	  	  else if (QEIdata.QEILinearAcc >= 0)
//	  	  {
//	  		  linear_accel = QEIdata.QEILinearAcc;
//	  	  }
//
//
//	  //linear_accel = accerelometer;
//	  linear_position = qeifloat-16.0 ;
//
//	  rState1=  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
//	  rState2=  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
//
//
//	// Vacuum & Gripper
//	  vacuum = registerFrame[0x02].U16;
//	  gripper = registerFrame[0x03].U16;	  //1 forward 0 backward
//	  reed = rState1 + (2 * rState2); //for 2 back 1
//	  registerFrame[0x04].U16 = (reed*2%3);
//
//
//	  registerFrame[0x11].U16 = (float)(linear_position * 10); //Position
//	  registerFrame[0x12].U16 = (float)(linear_velocity * 10); //Velocity
//	  registerFrame[0x13].U16 = (float)(linear_accel * 10)	; //Acceleration
//	  registerFrame[0x40].U16 = x_pos; // X-axis Position
//
//	  if (vacuum == 1)
//	  {
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
//	  }
//	  else
//	  {
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);
//	  }
//
//	  if (gripper ==1)
//	  {
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
//		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
//
////
////		  if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET )
////		  {
////			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
////			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
////		  }
//	  }
//	  else if (gripper == 0)
//	  {
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
//		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 0 );
////		  if ( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET )
////		  {
////	  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
////	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
////		  }
//	  }
//	  if((gripper == 1 && reed == 2) || (gripper == 0 && reed == 1)){
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
//		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1 );
//	  }
//
//
//
//	  //heartbeat protocal & debug
//	  //registerFrame[0x00].U16 = 22881;
//	  if(registerFrame[0x00].U16 != 22881){
//		  registerFrame[0x00].U16 = 22881;
//		  //deb++;
//	  }
//
//	  /////Set shelves
//	  if(registerFrame[0x01].U16 == 1) // order mode 1 -> open setshelf
//	  {
//		  registerFrame[0x01].U16 = 0; //change to idle mode
//		  registerFrame[0x10].U16 = 1; //current state set shelf mode
////		  set_position_box();
//	  }
//	  if(registerFrame[0x10].U16 == 1 && flagShelf == 1)
//	  {
//		  registerFrame[0x23].U16 = shelfPos[0];
//		  registerFrame[0x24].U16 = shelfPos[1];
//		  registerFrame[0x25].U16 = shelfPos[2];
//		  registerFrame[0x26].U16 = shelfPos[3];
//		  registerFrame[0x27].U16 = shelfPos[4];
//		  registerFrame[0x10].U16 = 0; //
//		  flagShelf = 0;
//	  }
//
//		///////1
//	  else if((registerFrame[0x01].U16 == 2)) //go to mode 2: Home
//	  {
//		  (registerFrame[0x01].U16) = 0; //reset status
//		  (registerFrame[0x10].U16) = 2; //Z-home
//		  		//setPos = shelfPos[0]; // set goal to home
//		  setPos = 0;
////		  		  uint8_t result = HomeZ();
////		  		  HAL_Delay(2000);
////		  		  generate_trapezoidal_velocity_profile(time_op,setPos/10.0);
//		  Home = 0;
//		  rou = 0;
//	  }
//	/////point mode
//	  else if((registerFrame[0x01].U16) == 8)
//	  {
//		  (registerFrame[0x01].U16) = 0; //reset status
//		  (registerFrame[0x10].U16) = 16; // Z-go point
//
//		  setPos = (registerFrame[0x30].U16); // goal = point 30->base system 4 point mode
//				//generate_trapezoidal_velocity_profile(time_op,setPos);
//	  }
//	/////jog mode
//	  //read (convert to array)
//	  else if((registerFrame[0x01].U16 ==4))
//	  {
//		  (registerFrame[0x01].U16) = 0; //reset status
//
//		  temPick = (registerFrame[0x21].U16);
//		  temPlace = (registerFrame[0x22].U16);
//
//		  OrderSeparate();
//	  }
//			//run
//	  else if(rnd > 0)
//	  {
//				//first round
//		  if(registerFrame[0x10].U16 == 0 && rnd== 5 && gripper == 0 && reed == 1 && vacuum == 0) // first rev
//		  {
//			  mode = 9; // for debug - pick
//			  (registerFrame[0x10].U16) = 4; // Z-go pick
//			  setPos = shelfPos[pick[5-rnd]-1];
//		  }
//		  if((piingpong && registerFrame[0x10].U16 == 8)) // prev mode: pick, do place
//		  {
//				///////place down
//			  if(mode == 6){
//				  timestamp1 = HAL_GetTick() + 100; // delay before gripper move
//				  registerFrame[0x03].U16 = 1; // gripper
//				  registerFrame[0x02].U16 = 2; // vacuum
//				  mode = 60;
//			  }
//			  if(reed != 2 && rnd> 0 && vacuum == 1 && gripper == 0 && HAL_GetTick() >= timestamp1){
//				  registerFrame[0x03].U16 = 1; // gripper forward
//				  timestamp2 = HAL_GetTick() + 100; //delay before release box
//				  mode = 61;
//			  }
//			  else if(reed == 2 && vacuum == 1 && HAL_GetTick() >= timestamp2)//reached reed vacuum not off
//			  {
//				  registerFrame[0x02].U16 = 0; //vacuum off
//				  timestamp3 = HAL_GetTick() + 300; // delay before retract gripper
//				  mode = 62;
//			  }
//			  else if(vacuum == 0 && gripper == 1 && HAL_GetTick() >= timestamp3)
//			  {
//				  registerFrame[0x03].U16 = 0; //gripper backward
//				  load = 0;
//				  timestamp4 = HAL_GetTick() + 100; // delay before moving to pick
//				  mode = 63;
//			  }
//				///////finish place -> move on
//			  if(gripper == 0 && reed == 1 && vacuum == 0 && HAL_GetTick() >= timestamp4)
//			  {
//				  rnd--;
//				  if(rnd>0)
//				  {
//					  (registerFrame[0x10].U16) = 4; // Z-go pick
//					  setPos = shelfPos[pick[5-rnd]-1];
//					  mode = 9;
//				  }
//			  }
//		  }
//		  else if(piingpong && registerFrame[0x10].U16 == 4)// prev mode: place, do pick
//		  {
//				//////pick up
//			  if(mode == 9){
//				  timestamp1 = HAL_GetTick() + 50; // delay before gripper go pick
//					//mode = 90;
//			  }
//			  if(reed == 1 && gripper == 0 && vacuum == 0 && HAL_GetTick() >= timestamp1)
//			  {
//				  timestamp2 = HAL_GetTick() + 100; // delay before picking box
//				  registerFrame[0x03].U16 = 1; //gripper forward
//					//mode = 91;
//			  }
//			  else if(gripper == 1 && vacuum == 0 && HAL_GetTick() >= timestamp2) //if vacuum off
//			  {
//				  registerFrame[0x02].U16 = 1; //vacuum on
//				  timestamp3 = HAL_GetTick() + 700; // delay before pulling box back
//					//mode = 92;
//
//			  }
//			  else if(reed == 2 && gripper == 1 && vacuum == 1 && HAL_GetTick() >= timestamp3)
//			  {
//				  registerFrame[0x03].U16 = 0; //gripper backward
//				  load = 1;
//				  timestamp4 = HAL_GetTick() + 200; // delay before moving again
//					//mode = 93;
//			  }
//				///////finish pick -> move on
//			  if(gripper == 0 && reed == 1 && vacuum == 1 && HAL_GetTick() >= timestamp4)
//			  {
//				  (registerFrame[0x10].U16) = 8; // Z-go place
//				  if(place[5-rnd] == 5)
//				  {
//					  setPos = shelfPos[place[5-rnd]-1] + 150; // goal + 5mm
//				  }
//				  else
//				  {
//					  setPos = shelfPos[place[5-rnd]-1] + 150; // goal + 5mm
//				  }
//					//mode = 6;
//			  }
//		  }
//	  }
//
//	  //end jog
//	  else if(registerFrame[0x10].U16 == 8 && rnd== 0)
//	  {
//		  (registerFrame[0x10].U16 = 0); // End Jogs
//		  mode = 255;
//	  }
//		/////End point & Home
//	  else if(piingpong && (registerFrame[0x10].U16 == 2 || registerFrame[0x10].U16 == 16))
//	  {
//		  //finish point & home mode
//		  static uint64_t Timestamp = 0;
//		  if(rnd2 == 0){
//			  Timestamp = HAL_GetTick() + 1000; //delay
//			  rnd2 = 1;
//		  }
//		  else if(HAL_GetTick() >= Timestamp)
//		  {
//			  registerFrame[0x10].U16 = 0; // finish point & home mode
//			  rnd2 = 0;
//		  }
//	  }
//	  if(registerFrame[0x10].U16 == 0){
//		  piingpong = 0;
//	  }
//}

//

void OrderSeparate(void)
{
	//rnd = 0;
	////// Convert to string
	for(uint16_t p = 10000;p>=1 && temPick != 0;p/=10)
	{
		if(temPick/p <= 0 || temPick/p > 5 || temPlace/p <= 0 || temPlace/p > 5) // check if 0 or > 5
		{
			//rnd = 0;
			//mode = 0;
			break;
		}
		pick[rnd] = temPick/p; // use this for pick
		place[rnd] = temPlace/p; // use this for place
		temPick = temPick%p;
		temPlace = temPlace%p;
		rnd++;
		rou = 20;
	}
}


/* 	@User : Move motor
 *
 * 	Function: serviceMotor
 * 	Description: Low level API to control motor
 *	Return type : void
 *
 *	Parameter:
 *		pwm: (unsigned 32-bit integer) Motor pwm value
 *		dir: (unsigned 8-bit integer) Motor direction
 *
 */

void serviceMotor(uint32_t pwm, uint8_t dir){

	zStop = 0;
	if(dir){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	}
	else{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm);
	}
}

/* 	@User : Get encoder value
 *
 * 	Function: getEncoderValue
 * 	Description: Low level API to get qei's counter
 *	Return type : uint32_t
 *
 *	Parameter:
 *		void
 *
 */


uint32_t getEncoderValue(){
	return __HAL_TIM_GET_COUNTER(&htim2);
}


/* 	@User : Zero encoder value
 *
 * 	Function: zeroEncoderValue
 * 	Description: Low level API to reset encoder's qei counter to 10,000 (this is for overhead)
 *	Return type : void
 *
 *	Parameter:
 *		void
 *
 */


void zeroEncoderValue(){
	__HAL_TIM_SET_COUNTER(&htim2, 10000);
}

/* 	@User : Get zStop
 *
 * 	Function: getZStop
 * 	Description: Low level API to get ZStop flag
 *	Return type : uint8_t
 *
 *	Parameter:
 *		void
 *
 */


uint8_t getZStop(){
	return zStop;
}

// @User : Stop motor when hit the end stop
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if((GPIO_Pin == GPIO_PIN_10 ) && zStop== 0){
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
        zStop = 1;
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        qeifloat = 0;
    }
    if ((GPIO_Pin == GPIO_PIN_5))
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    }

}


// @User : Stop timer3 that send data to led
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == TIM3){
		HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_2);
	}

}

//Callback -> Timer finish
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim5)
	{
		//registerFrame[0x00].U16 = 22881; //send "Ya" // dont use interrupt cuz unstable
	}
	else if(htim == &htim4)
	{
		i+=1;
		//piingpong = 0;
		fin = 0;
		generate_Velocity();

	}

	else if(htim == &htim7)
	{
	_micros += UINT16_MAX;
	}

}

uint64_t micros()
{
return __HAL_TIM_GET_COUNTER(&htim7)+_micros;
}

float QEIEncoderPosVel_Update()
{
//collect data
QEIdata.TimeStamps[NEW] = micros();
QEIdata.Positions[NEW] = __HAL_TIM_GET_COUNTER(&htim2);
//QEIdata.QEIAngularVelocity[NEW]=QEIdata.QEIAngularVelocity[OLD];

//Postion 1 turn calculation
//QEIdata.QEIPostion_1turn = QEIdata.Positions[NEW] % 3072;
//calculate dx
diffPosition = QEIdata.Positions[NEW] - QEIdata.Positions[OLD];
//diffVelocity = QEIdata.QEIAngularVelocity[NEW] - QEIdata.QEIAngularVelocity[OLD];
//Handle Warp around
//if(diffPosition > 2147483647)
//diffPosition -=4294967295 ;
//if(diffPosition < 2147483647)
//diffPosition +=4294967295;
//calculate dt
diffTime = (QEIdata.TimeStamps[NEW]-QEIdata.TimeStamps[OLD]) * 0.000001;
//calculate anglar velocity
diffVelocity = (diffPosition / diffTime)*(60.0/8192)*100.0;
//QEIdata.QEILinearAcc = (diffVelocity / diffTime)*(60.0/8192)*100.0;

//store value for next loop
QEIdata.Positions[OLD] = QEIdata.Positions[NEW];
QEIdata.TimeStamps[OLD]=QEIdata.TimeStamps[NEW];

return diffVelocity;
//QEIdata.QEIAngularVelocity[OLD]=QEIdata.QEIAngularVelocity[NEW];
}

void ACC_Update()
{
//collect data
QEIdata.TimeStamps[NEW] = micros();
QEIdata.QEIAngularVelocity[NEW] =  QEIEncoderPosVel_Update();
//QEIdata.QEIAngularVelocity[NEW]=QEIdata.QEIAngularVelocity[OLD];

//Postion 1 turn calculation
//QEIdata.QEIPostion_1turn = QEIdata.Positions[NEW] % 3072;
//calculate dx
//diffPosition = QEIdata.Positions[NEW] - QEIdata.Positions[OLD];
diffVelocitys = QEIdata.QEIAngularVelocity[NEW] - QEIdata.QEIAngularVelocity[OLD];
//Handle Warp around
//if(diffPosition > 2147483647)
//diffPosition -=4294967295 ;
//if(diffPosition < 2147483647)
//diffPosition +=4294967295;
//calculate dt
//calculate anglar velocity
QEIdata.QEILinearAcc = (diffVelocitys / diffTime);
//QEIdata.QEILinearAcc = (diffVelocity / diffTime)*(60.0/8192)*100.0;

//store value for next loop
QEIdata.QEIAngularVelocity[OLD] = QEIdata.QEIAngularVelocity[NEW];
QEIdata.TimeStamps[OLD]=QEIdata.TimeStamps[NEW];

//QEIdata.QEIAngularVelocity[OLD]=QEIdata.QEIAngularVelocity[NEW];
}


void set_position_box()
{
	if ( joystickPayload[0] == 0x80 )
	{
		if (TT == 0)
		{
			shelfPos[0] = qeifloat;
		}

		else if (TT == 1)
				{
			shelfPos[1] = qeifloat;
				}

		else if (TT == 2)
				{
			shelfPos[2] = qeifloat;
				}

		else if (TT == 3)
				{
			shelfPos[3] = qeifloat;
				}

		else if (TT == 4)
				{
			shelfPos[4] = qeifloat;
				}
		else if ( TT >=  5)
		{
			flagShelf = 1;
		}
		TT += 1;
	}
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

#ifdef  USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */