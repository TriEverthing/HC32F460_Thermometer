/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BOOTUF2_CONFIG_H
#define BOOTUF2_CONFIG_H

#define CONFIG_PRODUCT                      "HC32"
#define CONFIG_BOARD                        "HC32_BootLaoder"
#define CONFIG_BOOTUF2_INDEX_URL            "https://github.com/cherry-embedded"
#define CONFIG_BOOTUF2_JOIN_URL             "http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=GyH2M5XfWTHQzmZis4ClpgvfdObPrvtk&authKey=LmcLhfno%2BiW51wmgVC%2F8WoYwUXqiclzWDHMU1Jy1d6S8cECJ4Q7bfJ%2FTe67RLakI&noverify=0&group_code=642693751"

#define CONFIG_BOOTUF2_CACHE_SIZE           8192    //Erase Sector 8kB for HC32F460
#define CONFIG_BOOTUF2_SECTOR_SIZE          512     //512B for Composite 
#define CONFIG_BOOTUF2_SECTOR_PER_CLUSTER   16      //Erase Sector 8kB for HC32F460
#define CONFIG_BOOTUF2_SECTOR_RESERVED      1
#define CONFIG_BOOTUF2_NUM_OF_FAT           2
#define CONFIG_BOOTUF2_ROOT_ENTRIES         64

/* HC32F460 Memrory Map 
 * 0x00000000~0x0007FFFF: Embedded Flash 
 * 0x98000000~0x9C000000: QSPI ROM Area
*/
#define UF2_BOOT_EFM_BASE                   0x00000000
#define UF2_BOOT_QSPIROM_BASE               0x98000000
#define UF2_BOOT_FLASH_SIZE                 (512*1024)  //HC32F460xE=512kB,HC32F460xC=256kB
//APP code address , 16kB Flash for UF2_BootLoaderF
#define UF2_APP_CODE_ADDRESS                0x00004000  //16kB

#define CONFIG_BOOTUF2_FAMILYID             0x12345678

#define CONFIG_BOOTUF2_FLASHMAX             ( UF2_BOOT_FLASH_SIZE + UF2_BOOT_EFM_BASE - UF2_APP_CODE_ADDRESS )  //Flash
#define CONFIG_BOOTUF2_PAGE_COUNTMAX        ( CONFIG_BOOTUF2_FLASHMAX / 256 )

#define CONFIG_BOOTUF2_FLASH_DMA            0

#endif