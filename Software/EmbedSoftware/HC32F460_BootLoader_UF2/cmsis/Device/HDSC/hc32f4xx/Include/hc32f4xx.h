/**
 *******************************************************************************
 * @file  hc32f4xx.h
 * @brief This file contains the common part of the HC32 series.
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
#ifndef __HC32F4XX_H__
#define __HC32F4XX_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
/**
 * @brief HC32F4xx Device Include
 */
#if defined(HC32F460)
#include "hc32f460.h"
#include "system_hc32f460.h"
#else
#error "Please select first the target HC32xxxx device used in your application (in hc32xxxx.h file)"
#endif


#define LL_USB_ENABLE DDL_ON
#define LL_EFM_ENABLE DDL_ON

/* You can add your own assert functions by implement the function DDL_AssertHandler
   definition follow the function DDL_AssertHandler declaration */
#ifdef __DEBUG
#define DDL_ASSERT(x)                                                          \
do {                                                                           \
    ((x) ? (void)0 : DDL_AssertHandler(__FILE__, __LINE__));                   \
} while (0)
/* Exported function */
void DDL_AssertHandler(const char *file, int line);
#else
#define DDL_ASSERT(x)                   ((void)0U)
#endif /* __DEBUG */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __HC32F4XX_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
