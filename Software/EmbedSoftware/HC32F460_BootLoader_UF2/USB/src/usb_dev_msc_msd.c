/**
 *******************************************************************************
 * @file  usb/usb_dev_msc/source/usb_dev_msc_msd.c
 * @brief user MSC application layer.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Modify Vendor Identification
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
#include "usb_dev_msc_msd.h"
#include "usb_dev_msc_mem.h"

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
int8_t msc_init(uint8_t lun);
int8_t msc_getcapacity(uint8_t lun, uint32_t *block_num, uint32_t *block_size);
int8_t msc_ifready(uint8_t lun);
int8_t msc_ifwrprotected(uint8_t lun);
int8_t msc_read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t msc_write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t msc_getmaxlun(void);

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* Variable for Storage operation status */
__IO static uint8_t USB_STATUS_REG = 0U;

/* USB Mass storage querty data (36 bytes for each lun) */
const int8_t msc_inquirydata[] = {
    /* LUN 0 */
    0x00,   // Peripheral device type: 0x00 = Direct-access block device
    0x80,   // RMB: Removable Media Bit (0x80 = Removable)
    0x02,   // Version: SPC-2
    0x02,   // Response data format: 2
    (USB_DEV_INQUIRY_LENGTH - 4U), // Additional length: 32
    0x00,   // Reserved
    0x00,   // Reserved
    0x00,   // Reserved
    /* Vendor Identification */
    'X', 'H', 'S', 'C', ' ', 'M', 'C', 'U', ' ',    /* 9 bytes */
    /* Product Identification */
    'B', 'o', 'o', 't', 'L', 'o', 'a', 'd', 'e',    /* 15 bytes */
    'r', '_', 'U', 'F', '2', ' ',
    /* Product Revision Level */
    '1', '.', '0', ' ',                             /* 4 bytes */
};

static USB_DEV_MSC_cbk_TypeDef flash_fops = {
    &msc_init,
    &msc_getcapacity,
    &msc_getmaxlun,
    &msc_ifready,
    &msc_read,
    &msc_write,
    &msc_ifwrprotected,
    (int8_t *)msc_inquirydata
};

/* Pointer to flash_fops */
USB_DEV_MSC_cbk_TypeDef *msc_fops = &flash_fops;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
//static uint8_t u8CopybackBuf[W25Q64_SECTOR_SIZE];

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  initialize storage
 * @param  [in] lun          logic number
 * @retval status
 */
int8_t msc_init(uint8_t lun)
{
    //BSP_W25QXX_Init();
    return LL_OK;
}

/**
 * @brief  Get Storage capacity
 * @param  [in] lun          logic number
 * @param  [in] block_num    sector number
 * @param  [in] block_size   sector size
 * @retval status
 */
int8_t msc_getcapacity(uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
    (void)lun;
    //*block_size = 512U;
    //*block_num  = W25Q64_MAX_ADDR / 512U;
    *block_size = bootuf2_get_sector_size();
    *block_num = bootuf2_get_sector_count();
    return LL_OK;
}

/**
 * @brief  Check if storage is ready
 * @param  [in] lun          logic number
 * @retval status
 */
int8_t  msc_ifready(uint8_t lun)
{
    (void)lun;
    USB_STATUS_REG |= (uint8_t)0x10;
    return LL_OK;
}

/**
 * @brief  Check if storage is write protected
 * @param  [in] lun          logic number
 * @retval status
 */
int8_t msc_ifwrprotected(uint8_t lun)
{
    (void)lun;
    return LL_OK;
}

/**
 * @brief  read data from storage devices
 * @param  [in] lun          logic number
 * @param  [in] buf          data buffer be read
 * @param  [in] blk_addr     sector address
 * @param  [in] blk_len      sector count
 * @retval status
 */
int8_t msc_read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    (void)lun;
    boot2uf2_read_sector(blk_addr, buf, blk_len);
    return LL_OK;
}

/**
 * @brief  Write data to storage devices
 * @param  [in] lun          logic number
 * @param  [in] buf          data buffer be written
 * @param  [in] blk_addr     sector address
 * @param  [in] blk_len      sector count
 * @retval status
 */
int8_t msc_write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    (void)lun;
    bootuf2_write_sector(blk_addr, buf, blk_len);
    return LL_OK;
}

/**
 * @brief  Get supported logic number
 * @param  None
 * @retval 1
 */
int8_t msc_getmaxlun(void)
{
    return 1;
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
