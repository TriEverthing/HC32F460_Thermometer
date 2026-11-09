/**
 *******************************************************************************
 * @file  usb/usb_dev_msc/source/main.c
 * @brief Main file for USB example
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f460_mcu.h"

#include "usb_bsp.h"

#include "usb_dev_user.h"
#include "usb_dev_desc.h"
#include "usb_dev_msc_class.h"

#include "bootuf2.h"

/**
 * @addtogroup HC32F460_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Msc
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);
/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* KEY0 Port/Pin definition */
#define BOOTUF2_PORT         (GPIO_PORT_B)
#define BOOTUF2_PIN          (GPIO_PIN_07)
#define BOOTUF2_PIN_POS      (7)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
usb_core_instance usb_dev;
uint8_t uf2_bootloader_flag = 0;
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
uint8_t Boot_Btn_Is_Press(void);
void Jump_To_Application(uint32_t app_addr);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @brief ICG parameters configuration
 * 0x00000400~0x0000041F
 */
/* The ICG area filled with default value, Please modify this value required */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
const uint32_t u32ICG[] __attribute__((section(".ARM.__at_0x400"))) =
#elif defined (__GNUC__) && !defined (__CC_ARM)
const uint32_t u32ICG[] __attribute__((section(".icg_sec"))) =
#elif defined (__CC_ARM)
const uint32_t u32ICG[] __attribute__((at(0x400))) =
#elif defined (__ICCARM__)
#pragma location = 0x400
__root static const uint32_t u32ICG[] =
#else
#error "unsupported compiler!!"
#endif
{
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,//ICG1[0]:HRCFREQSEL, 0-20MHz, 1-16MHz
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,
    0xFFFFFFFFUL,

};
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
int main(void)
{
    uf2_bootloader_flag = 0;

    /* Check Boot Botton */
    if( !Boot_Btn_Is_Press() ) {
        Jump_To_Application(UF2_APP_CODE_ADDRESS);
    }

    uf2_bootloader_flag = 1;

    /* Set board system clock to MPLL@120MHz */
    hc32_bsp_clocks_init();
    SystemCoreClockUpdate();
    
    /* Initializes the device stack and load the class driver */
    stc_usb_port_identify stcPortIdentify;
    stcPortIdentify.u8CoreID = USBFS_CORE_ID;
    usb_dev_init(&usb_dev, &stcPortIdentify, &user_desc, &usb_dev_msc_cbk, &user_cb);
    for (;;) {
        if ( bootuf2_is_write_done() ) {
            Jump_To_Application(UF2_APP_CODE_ADDRESS);
            //bootuf2_clear_state();
        }        
    }
    return 0;
}

/**
 * @}
 */
void Jump_To_Application(uint32_t app_addr)
{
    __IO uint16_t *PCRx;
    __IO uint16_t *PFSRx;
    
    pFunction Application;
    uint32_t stack_top = *(volatile uint32_t *)app_addr;          /* Frist Word: Initial MSP */
    uint32_t reset_vec = *(volatile uint32_t *)(app_addr + 4U);   /* Second Word: Reset_Handler */

    if ((stack_top < SRAM_BASE ) || (stack_top > ( stack_top + 188*1024))) {
        return;
    }

    if ((reset_vec & 0x1U) == 0U) {
        return;   /* Thumb must be 1 */
    }

    if (reset_vec < app_addr || reset_vec > (UF2_BOOT_FLASH_SIZE + EFM_BASE)) {
        return; 
    }

    /* Register write enable for some required peripherals. */
    WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_UNLOCK_KEY);
    /* GPIO_Deinit */
    PCRx = &PCR_REG(BOOTUF2_PORT, BOOTUF2_PIN_POS);
    WRITE_REG16(*PCRx, 0x0000); 
    PCRx = &PCR_REG(USB_VBUS_PORT, USB_VBUS_PIN_POS);
    WRITE_REG16(*PCRx, 0x0000); 
    PCRx = &PCR_REG(USB_DP_PORT, USB_DP_PIN_POS);
    WRITE_REG16(*PCRx, 0x0000); 
    PCRx = &PCR_REG(USB_DM_PORT, USB_DM_PIN_POS);
    WRITE_REG16(*PCRx, 0x0000); 
    /* Set specified Port Pin function */
    PFSRx = &PFSR_REG(USB_VBUS_PORT, USB_VBUS_PIN_POS);/* VBUS */
    MODIFY_REG16(*PFSRx, GPIO_PFSR_FSEL, GPIO_FUNC_0);
    /* GPIO global register reset */
    WRITE_REG16(CM_GPIO->PSPCR, GPIO_PSPCR_RST_VALUE);
    WRITE_REG16(CM_GPIO->PCCR, GPIO_PCCR_RST_VALUE);
    WRITE_REG16(CM_GPIO->PINAER, GPIO_PINAER_RST_VALUE);
    /* Register write protected for some required peripherals. */
    WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_LOCK_KEY);

    if( uf2_bootloader_flag )
    {
        /* Register write enable for some required peripherals. */
        LL_PERIPH_WE(LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_EFM);
        /* Disable the USB peripheral clock. */
        WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_UNLOCK_KEY);
        SET_REG32_BIT(CM_PWC->FCG1, FCG1_PERIPH_USBFS);
        WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_LOCK_KEY);
        /* De-Initializes the device stack and load the class driver */
        usb_dev_deinit(&usb_dev);
        /* Disable mPLL */
        CLK_PLLCmd(DISABLE);
        /* Register write protected for some required peripherals. */
        LL_PERIPH_WP(LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_EFM);

        /* Clear All NVIC */
        for (uint32_t i = 0; i < 8U; i++) {
            NVIC->ICER[i] = 0xFFFFFFFFU;
            NVIC->ICPR[i] = 0xFFFFFFFFU;
        }

        /* Switch-Off SysTick */
        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL  = 0;
    }
    /* Disable Interrupt */
    __disable_irq();
    __DSB();
    __ISB();
    
    /* Set interrupt vector to app code */
    SCB->VTOR = (uint32_t)(app_addr & SCB_VTOR_TBLOFF_Msk);
    __DSB();
    __ISB();

    /* Initialize user application's Stack Pointer */
    __set_MSP(stack_top);
    __DSB();
    __ISB();

    /* Jump to application */
    Application = (pFunction)reset_vec;
    Application();
}
/**
 * @}
 */
uint8_t Boot_Btn_Is_Press(void)
{
    static uint8_t boot_btn_is_initialized = 0;

    if( boot_btn_is_initialized == 0 )
    {
        __IO uint16_t *PCRx;
        /* Register write enable for some required peripherals. */
        WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_UNLOCK_KEY);
        /* Pull-up Input */
        PCRx = &PCR_REG(BOOTUF2_PORT, BOOTUF2_PIN_POS);
        WRITE_REG16(*PCRx, GPIO_PCR_PUU); 
        /* Register write protected for some required peripherals. */
        WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_LOCK_KEY);
        boot_btn_is_initialized = 1;
    } 

    if((READ_REG(PIDR_REG(BOOTUF2_PORT)) & (BOOTUF2_PIN)) == 0 ) //UF2_BOOT Press Down
    {
        //soft_delay_ms(10);
        /* Key filter */
        DDL_DelayMS(20);
        if((READ_REG(PIDR_REG(BOOTUF2_PORT)) & (BOOTUF2_PIN)) == 0 )
        {
            return 1;
        }
    }

    return 0;
}
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
