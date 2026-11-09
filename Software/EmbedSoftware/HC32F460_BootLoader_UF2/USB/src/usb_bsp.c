/**
 *******************************************************************************
 * @file  usb/usb_dev_msc/source/usb_bsp.c
 * @brief BSP function for USB example
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Add USB core ID select function
   2023-09-30       CDT             Rename UsbClockIni() as UsbClockInit()
   2024-11-08       CDT             Optimize print information
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
#include "usb_bsp.h"
#include "hc32_ll.h"
#include "usb_dev_int.h"

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

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* unlock/lock peripheral */
#define EXAMPLE_PERIPH_WE               ( LL_PERIPH_EFM | LL_PERIPH_PWC_CLK_RMU )
#define EXAMPLE_PERIPH_WP               ( LL_PERIPH_EFM | LL_PERIPH_PWC_CLK_RMU )


/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
extern usb_core_instance usb_dev;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
void IRQ030_Handler(void);

/**
 * @brief  handle the USB interrupt
 * @param  None
 * @retval None
 */
#if LL_INTERRUPTS_ENABLE == DDL_ON
static void USB_IRQ_Handler(void)
#else
void IRQ030_Handler(void)
#endif
{
    usb_isr_handler(&usb_dev);
}

/**
 * @brief  handle the USB interrupt
 * @param  None
 * @retval None
 */
int bootuf2_flash_write(uint32_t address , const uint8_t *data, size_t size)
{
    uint32_t u32TimesOut;

    /* Peripheral registers write unprotected */
    LL_PERIPH_WE(LL_PERIPH_EFM );

    //address = address + FLASH_BASE ;
    if( address >= UF2_APP_CODE_ADDRESS && address <= ( UF2_BOOT_FLASH_SIZE + UF2_BOOT_EFM_BASE )) {
        /* Embedded Flash */
        u32TimesOut = 0xFFFFFF;

        /* Wait flash ready. */
        while (SET != EFM_GetStatus(EFM_FLAG_RDY)) {
            u32TimesOut --;
            if( u32TimesOut == 0 ) { return LL_ERR; } 
        }

        /* EFM_FWMC write enable */
        EFM_FWMC_Cmd(ENABLE);
        /* Hold bus while erase & program */
        EFM_SetBusStatus(EFM_BUS_HOLD);

        /* Erase Sector */
        EFM_SectorErase(address);

        /* Program Flash */
        //EFM_SequenceProgram(address, data, size);
        EFM_Program(address, data, size);

        /* EFM_FWMC write Disable */
        EFM_FWMC_Cmd(DISABLE);  

        /* Peripheral registers write protected */
        LL_PERIPH_WP(LL_PERIPH_EFM );
    } else if( address >= UF2_BOOT_QSPIROM_BASE ) {
        /* QSPI ROM Area */
        
    }
    else
    {
        return LL_ERR;
    }
 
    
    return LL_OK;
}

/**
 * @brief  USB clock initial
 * @param  None
 * @retval None
 */
#if 0
static void UsbClockInit(void)
{
    stc_clock_pllx_init_t stcUpllInit;

    (void)CLK_PLLxStructInit(&stcUpllInit);
    stcUpllInit.u8PLLState = CLK_PLLX_ON;
    stcUpllInit.PLLCFGR = 0UL;
    stcUpllInit.PLLCFGR_f.PLLM = (2UL  - 1UL);
    stcUpllInit.PLLCFGR_f.PLLN = (84UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLR = (7UL  - 1UL);
    stcUpllInit.PLLCFGR_f.PLLQ = (7UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLP = (7UL - 1UL); //48M
    (void)CLK_PLLxInit(&stcUpllInit);

    /* Set USB clock source */
    CLK_SetUSBClockSrc(CLK_USBCLK_PLLXP);
}
#else
#define UsbClockInit()    CLK_SetUSBClockSrc(CLK_USBCLK_PLLR)
#endif

/**
 * @brief  initialize configurations for the BSP
 * @param  [in] pdev                device instance
 * @param  [in] pstcPortIdentify    usb core and phy select
 * @retval None
 */
void usb_bsp_init(usb_core_instance *pdev, stc_usb_port_identify *pstcPortIdentify)
{
    __IO uint16_t *PCRx;
    __IO uint16_t *PFSRx;

    bootuf2_init();

    /* Unlock peripherals or registers */
    LL_PERIPH_WE(EXAMPLE_PERIPH_WE);

    //BSP_CLK_Init();
    //BSP_LED_Init();
#if (LL_PRINT_ENABLE == DDL_ON)
    DDL_PrintfInit(BSP_PRINTF_DEVICE, BSP_PRINTF_BAUDRATE, BSP_PRINTF_Preinit);
#endif
    /* USB clock source configure */
    UsbClockInit();

#if (LL_PRINT_ENABLE == DDL_ON)
    DDL_Printf("USB start !!\r\n");
#endif

    /* Register write enable for some required peripherals. */
    WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_UNLOCK_KEY);

    PCRx = &PCR_REG(USB_DP_PORT, USB_DP_PIN_POS);
    WRITE_REG16(*PCRx, GPIO_PCR_DDIS); 
    PCRx = &PCR_REG(USB_DM_PORT, USB_DM_PIN_POS);
    WRITE_REG16(*PCRx, GPIO_PCR_DDIS); 

    /* Set specified Port Pin function */
    PFSRx = &PFSR_REG(USB_VBUS_PORT, USB_VBUS_PIN_POS);/* VBUS */
    MODIFY_REG16(*PFSRx, GPIO_PFSR_FSEL, GPIO_FUNC_10);

    /* Register write protected for some required peripherals. */
    WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_LOCK_KEY);

    WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_UNLOCK_KEY);
    CLR_REG32_BIT(CM_PWC->FCG1, FCG1_PERIPH_USBFS);
    WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_LOCK_KEY);
}

#define INTSEL_REG              (uint32_t)(&CM_INTC->SEL0)
#define INTSEL_RST_VALUE        (0x1FFUL)
/**
 * @brief  configure the NVIC of USB
 * @param  [in] pdev                    device instance
 * @retval None
 */
void usb_bsp_nvicconfig(usb_core_instance *pdev)
{
#if LL_INTERRUPTS_ENABLE == DDL_ON
    ////stc_irq_signin_config_t stcIrqRegiConf;
    /* Register INT_SRC_USBFS_GLB Int to Vect.No.030 */
    //stcIrqRegiConf.enIRQn = INT030_IRQn;
    /* Select interrupt function */
    //stcIrqRegiConf.enIntSrc = INT_SRC_USBFS_GLB;
    /* Callback function */
    //stcIrqRegiConf.pfnCallback = &USB_IRQ_Handler;
    /* Registration IRQ */
    //(void)INTC_IrqSignIn(&stcIrqRegiConf);
    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, 15U);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
#else
    __IO uint32_t *INTC_SELx;
    /* INT_SEL30 Register, 0x40051000+0x00D4 */
    INTC_SELx = (__IO uint32_t *)(INTSEL_REG + (4U * (uint32_t)(INT030_IRQn)));
    /* for MISRAC2004-12.4 */
    if (INTSEL_RST_VALUE == ((*INTC_SELx) & INTSEL_RST_VALUE)) {
        WRITE_REG32(*INTC_SELx, INT_SRC_USBFS_GLB);
    }
    /* Clear Pending */
    NVIC_ClearPendingIRQ(INT030_IRQn);
    /* Set priority */
    NVIC_SetPriority(INT030_IRQn, 15U);
    /* Enable NVIC */
    NVIC_EnableIRQ(INT030_IRQn);
#endif
}


/**
 * @brief  This function provides delay time in micro sec
 * @param  [in] usec         Value of delay required in micro sec
 * @retval None
 */
void usb_udelay(const uint32_t usec)
{
    __IO uint32_t i;
    uint32_t j;
    j = (HCLK_VALUE + 1000000UL - 1UL) / 1000000UL * usec;
    for (i = 0UL; i < j; i++) {
    }
}

/**
 * @brief  This function provides delay time in milli sec
 * @param  [in] msec         Value of delay required in milli sec
 * @retval None
 */
void usb_mdelay(const uint32_t msec)
{
    usb_udelay(msec * 1000UL);
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
