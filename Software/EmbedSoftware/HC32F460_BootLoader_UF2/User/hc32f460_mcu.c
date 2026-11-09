/*
 * hc32f460_bsp.c
 *
 *  Created on: 2024 Dec 29
 *      Author: bluerose
 */
/* add user code end Header */
#include "hc32f460_mcu.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
#define HRC_FREQ_MON()                  (*((volatile uint32_t *)(0x40010684UL)))

/* private variables ---------------------------------------------------------*/
/* at-start led resouce array */


/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/* exported variables --------------------------------------------------------*/

/* private function prototypes --------------------------------------------*/

/* add user code begin function prototypes */

/* private user code ---------------------------------------------------------*/

/**
 * @brief  BSP clock initialize.
 *         Set board system clock to MPLL@120MHz
 * @param  None
 * @retval None
 */
void hc32_bsp_clocks_init(void)
{
  __IO uint16_t *PCRx;
  //stc_clock_xtal_init_t     stcXtalInit;
  stc_clock_pll_init_t      stcMpllInit;
  
  /* Peripheral registers write unprotected */
  WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_UNLOCK_KEY); //GPIO
  LL_PERIPH_WE( LL_PERIPH_EFM | LL_PERIPH_PWC_CLK_RMU );

  /* Enable GPIO Analog function */
  PCRx = &PCR_REG(BSP_XTAL_PORT, BSP_XTAL0_PIN_POS);
  SET_REG16_BIT(*PCRx, GPIO_PCR_DDIS);
  PCRx = &PCR_REG(BSP_XTAL_PORT, BSP_XTAL1_PIN_POS);
  SET_REG16_BIT(*PCRx, GPIO_PCR_DDIS);
  CLK_PLLxCmd(DISABLE); //PLLx Disbale

  /* Set bus clk div. */
  CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                                    CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));
#if 0
  /* Init Xtal initial structure with default value */
  CLK_XtalStructInit(&stcXtalInit);
  /* Config Xtal and enable Xtal */
  stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
  stcXtalInit.u8Drv = CLK_XTAL_DRV_ULOW;
  stcXtalInit.u8State = CLK_XTAL_ON;
  stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
  CLK_XtalInit(&stcXtalInit);
  /* Init PLL initial structure with default value */
  CLK_PLLStructInit(&stcMpllInit);
  /* MPLL config (HRC(16MHz) / pllmDiv * plln / PllpDiv = 120M). */
  stcMpllInit.PLLCFGR = 0UL;
  stcMpllInit.PLLCFGR_f.PLLM = 1UL - 1UL;
  stcMpllInit.PLLCFGR_f.PLLN = 60UL - 1UL;
  stcMpllInit.PLLCFGR_f.PLLP = 4UL - 1UL;
  stcMpllInit.PLLCFGR_f.PLLQ = 4UL - 1UL;
  stcMpllInit.PLLCFGR_f.PLLR = 10UL - 1UL;
  stcMpllInit.u8PLLState = CLK_PLL_ON;
  stcMpllInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
  CLK_PLLInit(&stcMpllInit);
#else
  /* HRC Enable */
  CLK_HrcCmd(ENABLE);
  /* Init PLL initial structure with default value */
  CLK_PLLStructInit(&stcMpllInit);
  /* Select proper HRC_VALUE according to ICG1.HRCFREQSEL bit */
  /* MPLL N=20~480, VCO 240MHz~480MHz*/
  if (1UL == (HRC_FREQ_MON() & 1UL)) {
    /* MPLL config (HRC(16MHz) / pllmDiv * plln / PllpDiv = 120M). */
    stcMpllInit.PLLCFGR = 0UL;
    stcMpllInit.PLLCFGR_f.PLLM = 2UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLN = 60UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLP = 4UL - 1UL; //16MHz/2*60/4=120MHz
    stcMpllInit.PLLCFGR_f.PLLQ = 4UL - 1UL; //16MHz/2*60/4=120MHz
    stcMpllInit.PLLCFGR_f.PLLR = 10UL - 1UL;//16MHz/2*10/4=48MHz
  } else {
    stcMpllInit.PLLCFGR = 0UL;
    stcMpllInit.PLLCFGR_f.PLLM = 5UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLN = 120UL - 1UL;//VCO=480MHz
    stcMpllInit.PLLCFGR_f.PLLP = 4UL - 1UL; //20MHz/5*120/4=120MHz
    stcMpllInit.PLLCFGR_f.PLLQ = 4UL - 1UL; //20MHz/5*120/4=120MHz
    stcMpllInit.PLLCFGR_f.PLLR = 10UL - 1UL;//20MHz/5*120/4=48MHz
  }

  stcMpllInit.u8PLLState = CLK_PLL_ON;
  stcMpllInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_HRC;
  CLK_PLLInit(&stcMpllInit);
#endif
  /* Wait MPLL ready. */
  while (SET != CLK_GetStableStatus(CLK_STB_FLAG_PLL)) 
  {

  }

  /* sram init include read/write wait cycle setting */
  SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
  SRAM_SetWaitCycle((SRAM_SRAM12 | SRAM_SRAM3 | SRAM_SRAMR), SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);

  /* flash read wait cycle setting */
  EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
  /* 3 cycles for 126MHz ~ 200MHz */
  MODIFY_REG16(CM_GPIO->PCCR, GPIO_PCCR_RDWT, GPIO_RD_WAIT3);
  /* Switch driver ability ， Power On default high speed mode for mian clocks < 168MHz */
  //PWC_HighSpeedToHighPerformance();
  /* Switch system clock source to MPLL. */
  CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
  /* Reset cache ram */
  EFM_CacheRamReset(ENABLE);
  EFM_CacheRamReset(DISABLE);
  /* Enable cache */
  EFM_CacheCmd(ENABLE);

  /* Peripheral registers write protected */
  LL_PERIPH_WP( LL_PERIPH_EFM | LL_PERIPH_PWC_CLK_RMU );
}


