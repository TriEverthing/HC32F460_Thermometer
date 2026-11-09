/*
 * hc32f460_bsp.h
 *
 *  Created on: 2024 Dec 29
 *      Author: bluerose
 */

#ifndef _HC32F460_BSP_H_
#define _HC32F460_BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* includes -----------------------------------------------------------------------*/
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "hc32_ll.h"
//#include "config_param.h"
/* private includes -------------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* exported types -------------------------------------------------------------*/
extern void (*Mcu_printf)( const uint8_t * format , ... ) ;
extern volatile uint32_t ticks_count_us ;
extern volatile uint64_t ticks_count_overflow ;
/* exported constants --------------------------------------------------------*/
/* add user code begin exported constants */

/* add user code end exported constants */

/* exported macro ------------------------------------------------------------*/
/**
 * @defgroup GPIO_Registers_Setting_definition GPIO Registers setting definition
 * @{
 */
#define GPIO_PSPCR_RST_VALUE            (0x001FU)
#define GPIO_PCCR_RST_VALUE             (0x4000U)
#define GPIO_PINAER_RST_VALUE           (0x0000U)
#define GPIO_PIN_NUM_MAX                (16U)
#define GPIO_PORT_OFFSET                (0x40UL)
#define GPIO_PIN_OFFSET                 (0x04UL)
#define GPIO_REG_OFFSET                 (0x10UL)
#define GPIO_REG_TYPE                   uint16_t
#define GPIO_PIDR_BASE                  ((uint32_t)(&CM_GPIO->PIDRA))
#define GPIO_PODR_BASE                  ((uint32_t)(&CM_GPIO->PODRA))
#define GPIO_POSR_BASE                  ((uint32_t)(&CM_GPIO->POSRA))
#define GPIO_PORR_BASE                  ((uint32_t)(&CM_GPIO->PORRA))
#define GPIO_POTR_BASE                  ((uint32_t)(&CM_GPIO->POTRA))
#define GPIO_POER_BASE                  ((uint32_t)(&CM_GPIO->POERA))
#define GPIO_PCR_BASE                   ((uint32_t)(&CM_GPIO->PCRA0))
#define GPIO_PFSR_BASE                  ((uint32_t)(&CM_GPIO->PFSRA0))

#define PIDR_REG(x)     (*(__IO GPIO_REG_TYPE *)(GPIO_PIDR_BASE + GPIO_REG_OFFSET * (x)))
#define PODR_REG(x)     (*(__IO GPIO_REG_TYPE *)(GPIO_PODR_BASE + GPIO_REG_OFFSET * (x)))
#define POSR_REG(x)     (*(__IO GPIO_REG_TYPE *)(GPIO_POSR_BASE + GPIO_REG_OFFSET * (x)))
#define PORR_REG(x)     (*(__IO GPIO_REG_TYPE *)(GPIO_PORR_BASE + GPIO_REG_OFFSET * (x)))
#define POTR_REG(x)     (*(__IO GPIO_REG_TYPE *)(GPIO_POTR_BASE + GPIO_REG_OFFSET * (x)))
#define POER_REG(x)     (*(__IO GPIO_REG_TYPE *)(GPIO_POER_BASE + GPIO_REG_OFFSET * (x)))
#define PCR_REG(x, y)   (*(__IO uint16_t *)(GPIO_PCR_BASE +  (uint32_t)((x) * GPIO_PORT_OFFSET) + (y) * GPIO_PIN_OFFSET))
#define PFSR_REG(x, y)  (*(__IO uint16_t *)(GPIO_PFSR_BASE + (uint32_t)((x) * GPIO_PORT_OFFSET) + (y) * GPIO_PIN_OFFSET))

#if LL_GPIO_ENABLE == DDL_OFF
#define GPIO_REG_LOCK_KEY           (0xA500U)
#define GPIO_REG_UNLOCK_KEY         (0xA501U)

/**
 * @defgroup GPIO_Port_Source GPIO Port Source
 * @{
 */
#define GPIO_PORT_A                 (0x00U)     /*!< Port A selected  */
#define GPIO_PORT_B                 (0x01U)     /*!< Port B selected  */
#define GPIO_PORT_C                 (0x02U)     /*!< Port C selected  */
#define GPIO_PORT_D                 (0x03U)     /*!< Port D selected  */
#define GPIO_PORT_E                 (0x04U)     /*!< Port E selected  */
#define GPIO_PORT_H                 (0x05U)     /*!< Port H selected  */
/**
 * @defgroup GPIO_Pins_Define GPIO Pin Source
 * @{
 */
#define GPIO_PIN_00                 (0x0001U)  /*!< Pin 00 selected   */
#define GPIO_PIN_01                 (0x0002U)  /*!< Pin 01 selected   */
#define GPIO_PIN_02                 (0x0004U)  /*!< Pin 02 selected   */
#define GPIO_PIN_03                 (0x0008U)  /*!< Pin 03 selected   */
#define GPIO_PIN_04                 (0x0010U)  /*!< Pin 04 selected   */
#define GPIO_PIN_05                 (0x0020U)  /*!< Pin 05 selected   */
#define GPIO_PIN_06                 (0x0040U)  /*!< Pin 06 selected   */
#define GPIO_PIN_07                 (0x0080U)  /*!< Pin 07 selected   */
#define GPIO_PIN_08                 (0x0100U)  /*!< Pin 08 selected   */
#define GPIO_PIN_09                 (0x0200U)  /*!< Pin 09 selected   */
#define GPIO_PIN_10                 (0x0400U)  /*!< Pin 10 selected   */
#define GPIO_PIN_11                 (0x0800U)  /*!< Pin 11 selected   */
#define GPIO_PIN_12                 (0x1000U)  /*!< Pin 12 selected   */
#define GPIO_PIN_13                 (0x2000U)  /*!< Pin 13 selected   */
#define GPIO_PIN_14                 (0x4000U)  /*!< Pin 14 selected   */
#define GPIO_PIN_15                 (0x8000U)  /*!< Pin 15 selected   */
#define GPIO_PIN_ALL                (0xFFFFU)  /*!< All pins selected */

/**
 * @defgroup GPIO_Function_Sel GPIO Function Selection
 * @{
 */
#define GPIO_FUNC_0               (0U)
#define GPIO_FUNC_1               (1U)
#define GPIO_FUNC_2               (2U)
#define GPIO_FUNC_3               (3U)
#define GPIO_FUNC_4               (4U)
#define GPIO_FUNC_5               (5U)
#define GPIO_FUNC_6               (6U)
#define GPIO_FUNC_7               (7U)
#define GPIO_FUNC_8               (8U)
#define GPIO_FUNC_9               (9U)
#define GPIO_FUNC_10              (10U)
#define GPIO_FUNC_11              (11U)
#define GPIO_FUNC_12              (12U)
#define GPIO_FUNC_13              (13U)
#define GPIO_FUNC_14              (14U)
#define GPIO_FUNC_15              (15U)
#define GPIO_FUNC_32              (32U)
#define GPIO_FUNC_33              (33U)
#define GPIO_FUNC_34              (34U)
#define GPIO_FUNC_35              (35U)
#define GPIO_FUNC_36              (36U)
#define GPIO_FUNC_37              (37U)
#define GPIO_FUNC_38              (38U)
#define GPIO_FUNC_39              (39U)
#define GPIO_FUNC_40              (40U)
#define GPIO_FUNC_41              (41U)
#define GPIO_FUNC_42              (42U)
#define GPIO_FUNC_43              (43U)
#define GPIO_FUNC_44              (44U)
#define GPIO_FUNC_45              (45U)
#define GPIO_FUNC_46              (46U)
#define GPIO_FUNC_47              (47U)
#define GPIO_FUNC_48              (48U)
#define GPIO_FUNC_49              (49U)
#define GPIO_FUNC_50              (50U)
#define GPIO_FUNC_51              (51U)
#define GPIO_FUNC_52              (52U)
#define GPIO_FUNC_53              (53U)
#define GPIO_FUNC_54              (54U)
#define GPIO_FUNC_55              (55U)
#define GPIO_FUNC_56              (56U)
#define GPIO_FUNC_57              (57U)
#define GPIO_FUNC_58              (58U)
#define GPIO_FUNC_59              (59U)

/**
 * @defgroup GPIO_ReadCycle_Sel GPIO Pin Read Wait Cycle Selection
 * @{
 */
#define GPIO_RD_WAIT0              (0x00U << GPIO_PCCR_RDWT_POS)
#define GPIO_RD_WAIT1              (0x01U << GPIO_PCCR_RDWT_POS)
#define GPIO_RD_WAIT2              (0x02U << GPIO_PCCR_RDWT_POS)
#define GPIO_RD_WAIT3              (0x03U << GPIO_PCCR_RDWT_POS)

#endif

#if LL_FCG_ENABLE == DDL_OFF
/**
 * @defgroup FCG_FCG1_Peripheral FCG FCG1 peripheral
 * @{
 */
#define FCG1_PERIPH_CAN                 (PWC_FCG1_CAN)
#define FCG1_PERIPH_QSPI                (PWC_FCG1_QSPI)
#define FCG1_PERIPH_I2C1                (PWC_FCG1_I2C1)
#define FCG1_PERIPH_I2C2                (PWC_FCG1_I2C2)
#define FCG1_PERIPH_I2C3                (PWC_FCG1_I2C3)
#define FCG1_PERIPH_USBFS               (PWC_FCG1_USBFS)
#define FCG1_PERIPH_SDIOC1              (PWC_FCG1_SDIOC1)
#define FCG1_PERIPH_SDIOC2              (PWC_FCG1_SDIOC2)
#define FCG1_PERIPH_I2S1                (PWC_FCG1_I2S1)
#define FCG1_PERIPH_I2S2                (PWC_FCG1_I2S2)
#define FCG1_PERIPH_I2S3                (PWC_FCG1_I2S3)
#define FCG1_PERIPH_I2S4                (PWC_FCG1_I2S4)
#define FCG1_PERIPH_SPI1                (PWC_FCG1_SPI1)
#define FCG1_PERIPH_SPI2                (PWC_FCG1_SPI2)
#define FCG1_PERIPH_SPI3                (PWC_FCG1_SPI3)
#define FCG1_PERIPH_SPI4                (PWC_FCG1_SPI4)
#define FCG1_PERIPH_USART1              (PWC_FCG1_USART1)
#define FCG1_PERIPH_USART2              (PWC_FCG1_USART2)
#define FCG1_PERIPH_USART3              (PWC_FCG1_USART3)
#define FCG1_PERIPH_USART4              (PWC_FCG1_USART4)

#endif

#if LL_PWC_ENABLE == DDL_OFF
/**
 * @brief PWC FCG0 Unlock/Lock code
 */
#define PWC_FCG0_REG_UNLOCK_KEY          (0xA5A50001UL)
#define PWC_FCG0_REG_LOCK_KEY            (0xA5A50000UL)
#endif
/**
 * @defgroup XTAL port/pin definition
 * @{
 */
#define BSP_XTAL_PORT                 (GPIO_PORT_H)
#define BSP_XTAL_PIN                  (GPIO_PIN_00 | GPIO_PIN_01)
#define BSP_XTAL0_PIN_POS             (0)
#define BSP_XTAL1_PIN_POS             (1)


/* add user code begin exported functions */
void hc32_bsp_clocks_init(void);


/* add user code end exported functions */


#ifdef __cplusplus
}
#endif


#endif /* _AT32F405_HAL_H_ */
