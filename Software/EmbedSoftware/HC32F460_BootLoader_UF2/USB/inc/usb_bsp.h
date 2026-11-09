/**
 *******************************************************************************
 * @file  usb/usb_dev_msc/source/usb_bsp.h
 * @brief Head file for usb_bsp.c
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Add USB core ID select function
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#ifndef __USB_BSP_H__
#define __USB_BSP_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_lib.h"
#include "hc32f460_mcu.h"

/**
 * @addtogroup HC32F460_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Msc
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_DP_PORT                     (GPIO_PORT_A)
#define USB_DP_PIN                      (GPIO_PIN_12)
#define USB_DP_PIN_POS                  (12)
#define USB_DM_PORT                     (GPIO_PORT_A)
#define USB_DM_PIN                      (GPIO_PIN_11)
#define USB_DM_PIN_POS                  (11)
#define USB_VBUS_PORT                   (GPIO_PORT_A)
#define USB_VBUS_PIN                    (GPIO_PIN_09)
#define USB_VBUS_PIN_POS                (9)
#define USB_SOF_PORT                    (GPIO_PORT_A)
#define USB_SOF_PIN                     (GPIO_PIN_08)
#define USB_SOF_PIN_POS                 (8)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void usb_bsp_init(usb_core_instance *pdev, stc_usb_port_identify *pstcPortIdentify);
void usb_udelay(const uint32_t usec);
void usb_mdelay(const uint32_t msec);
void usb_bsp_nvicconfig(usb_core_instance *pdev);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __USB_BSP_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
