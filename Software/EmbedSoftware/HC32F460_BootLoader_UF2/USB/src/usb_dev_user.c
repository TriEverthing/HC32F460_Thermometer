/**
 *******************************************************************************
 * @file  usb/usb_dev_msc/source/usb_dev_user.c
 * @brief User callback function for USB example
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_user.h"
#include "usb_bsp.h"

/**
 * @addtogroup HC32F460_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Msc
 * @{
 */

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void usb_dev_user_init(void);
void usb_dev_user_rst(void);
void usb_dev_user_devcfg(void);
void usb_dev_user_devsusp(void);
void usb_dev_user_devresume(void);
void usb_dev_user_conn(void);
void usb_dev_user_disconn(void);

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
usb_dev_user_func user_cb = {
    &usb_dev_user_init,
    &usb_dev_user_rst,
    &usb_dev_user_devcfg,
    &usb_dev_user_devsusp,
    &usb_dev_user_devresume,
    &usb_dev_user_conn,
    &usb_dev_user_disconn,
};

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  usb_dev_user_init
 * @param  None
 * @retval None
 */
void usb_dev_user_init(void)
{

}

/**
 * @brief  usb_dev_user_rst
 * @param  None
 * @retval None
 */
void usb_dev_user_rst(void)
{

}

/**
 * @brief  usb_dev_user_devcfg
 * @param  None
 * @retval None
 */
void usb_dev_user_devcfg(void)
{

}

/**
 * @brief  usb_dev_user_conn
 * @param  None
 * @retval None
 */
void usb_dev_user_conn(void)
{

}

/**
 * @brief  USBD_USR_DeviceDisonnected
 * @param  None
 * @retval None
 */
void usb_dev_user_disconn(void)
{

}

/**
 * @brief  usb_dev_user_devsusp
 * @param  None
 * @retval None
 */
void usb_dev_user_devsusp(void)
{

}

/**
 * @brief  usb_dev_user_devresume
 * @param  None
 * @retval None
 */
void usb_dev_user_devresume(void)
{

}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF
 ******************************************************************************/
