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
#define PLL_SRC_REG                     (CM_CMU->PLLCFGR)
#define PLL_EN_REG                      (CM_CMU->PLLCR)
#define HRC_FREQ_MON                    (*((volatile uint32_t *)(0x40010684UL)))

/**
 * @brief Be able to modify TIMEOUT according to board condition.
 */
#define CLK_TIMEOUT                     ((uint32_t)0x1000UL)
#define CLK_LRC_TIMEOUT                 (160U)
#define CLK_MRC_TIMEOUT                 (1U)
#define CLK_XTAL32_TIMEOUT              (160U)


#define PWC_MD_SWITCH_TIMEOUT           (30UL)
#define PWC_MD_SWITCH_TIMEOUT2          (0x1000UL)
#define PWC_MD_SWITCH_CMD               (0x10U)


/**
 * @brief Clk FCG Default Value
 */
#define CLK_FCG0_DEFAULT                (0xFFFFFAEEUL)
#define CLK_FCG1_DEFAULT                (0xFFFFFFFFUL)
#define CLK_FCG2_DEFAULT                (0xFFFFFFFFUL)
#define CLK_FCG3_DEFAULT                (0xFFFFFFFFUL)

/**
 * @defgroup EFM_Timeout_Definition EFM timeout definition
 * @{
 */
#define EFM_TIMEOUT                     (HCLK_VALUE / 20000UL)  /* EFM wait read timeout */
#define EFM_PGM_TIMEOUT                 (HCLK_VALUE / 20000UL)  /* EFM Program timeout max 53us */
#define EFM_ERASE_TIMEOUT               (HCLK_VALUE / 50UL)     /* EFM Erase timeout max 20ms */
#define EFM_SEQ_PGM_TIMEOUT             (HCLK_VALUE / 62500UL)  /* EFM Sequence Program timeout max 16us */

/**
 * @defgroup CLK_Bus_Clock_Sel Clock Bus Clock Category Selection
 * @{
 */
#define CLK_BUS_PCLK0                   (CMU_SCFGR_PCLK0S)
#define CLK_BUS_PCLK1                   (CMU_SCFGR_PCLK1S)
#define CLK_BUS_PCLK2                   (CMU_SCFGR_PCLK2S)
#define CLK_BUS_PCLK3                   (CMU_SCFGR_PCLK3S)
#define CLK_BUS_PCLK4                   (CMU_SCFGR_PCLK4S)
#define CLK_BUS_EXCLK                   (CMU_SCFGR_EXCKS)
#define CLK_BUS_HCLK                    (CMU_SCFGR_HCLKS)
#define CLK_BUS_CLK_ALL                 (CLK_BUS_PCLK0 | CLK_BUS_PCLK1 | CLK_BUS_PCLK2 | CLK_BUS_PCLK3 | \
                                         CLK_BUS_PCLK4 | CLK_BUS_EXCLK | CLK_BUS_HCLK)
                                         
#define CLK_SYSCLK_SRC_PLL              (0x05U)
/* private variables ---------------------------------------------------------*/
/* at-start led resouce array */


/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/* exported variables --------------------------------------------------------*/

/* private function prototypes --------------------------------------------*/
int32_t Wait_ClkStable(uint8_t u8Flag, uint32_t u32Time);
void Set_SysClockSrc(uint8_t u8Src);
void Set_SysClockDiv(uint32_t u32Clock, uint32_t u32Div);
int32_t Set_EFMWaitCycle(uint32_t u32WaitCycle);

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
  //__IO uint16_t *PCRx;
  //stc_clock_xtal_init_t stcXtalInit;
  
  /* GPIO registers write unprotected */
  WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_UNLOCK_KEY);
  /* FCG registers write unprotected */
  WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_UNLOCK_KEY);
  /* EFM registers write unprotected */
  WRITE_REG32(CM_EFM->FAPRT, EFM_REG_UNLOCK_KEY1);
  WRITE_REG32(CM_EFM->FAPRT, EFM_REG_UNLOCK_KEY2);
  /* SRAM registers write unprotected */
  WRITE_REG32(CM_SRAMC->WTPR, SRAM_REG_UNLOCK_KEY);
  WRITE_REG32(CM_SRAMC->CKPR, SRAM_REG_UNLOCK_KEY);
  /* PWC, CLK, RMU registers write unprotected */
  SET_REG16_BIT(CM_PWC->FPRC, PWC_UNLOCK_CODE0 | PWC_UNLOCK_CODE1);

  /* Set bus clk divider, HCLKS_DIV1, EXCKS_DIV2, PCLK0_DIV1, PCLK1_DIV2, PCLK2_DIV4, PCLK3_DIV4, PCLK4_DIV2 */
  Set_SysClockDiv(CLK_BUS_CLK_ALL, ( (0<<CMU_SCFGR_HCLKS_POS) | (1<<CMU_SCFGR_EXCKS_POS) | (0 <<CMU_SCFGR_PCLK0S_POS) \
                                  | (1<<CMU_SCFGR_PCLK1S_POS)| (3<<CMU_SCFGR_PCLK2S_POS)| (3<<CMU_SCFGR_PCLK3S_POS) \
                                  | (1<<CMU_SCFGR_PCLK4S_POS)));

  //PLLx Disbale
  WRITE_REG8(CM_CMU->UPLLCR, CMU_UPLLCR_UPLLOFF);
  
#if 0
  /* Enable GPIO Analog function */
  PCRx = &PCR_REG(BSP_XTAL_PORT, BSP_XTAL0_PIN_POS);
  SET_REG16_BIT(*PCRx, GPIO_PCR_DDIS);
  PCRx = &PCR_REG(BSP_XTAL_PORT, BSP_XTAL1_PIN_POS);
  SET_REG16_BIT(*PCRx, GPIO_PCR_DDIS);

  /* Init Xtal initial structure with default value */
  CLK_XtalStructInit(&stcXtalInit);
  /* Config Xtal and enable Xtal */
  stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
  stcXtalInit.u8Drv = CLK_XTAL_DRV_ULOW;
  stcXtalInit.u8State = CLK_XTAL_ON;
  stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
  CLK_XtalInit(&stcXtalInit);

  /* set PLL source in advance */
  CLR_REG32_BIT(CM_CMU->PLLCFGR, CMU_PLLCFGR_PLLSRC);//MPLL source HSE
  /* Init PLL initial structure with default value */
  /* MPLL_VCO = PLL_IN / PLLM * PLLN：240MHz~480MHz
    * f_MPLL_P = MPLL_VCO / PLLP, 192MHz
    * f_MPLL_Q = MPLL_VCO / PLLQ, 192MHz 
    * f_MPLL_R = MPLL_VCO / PLLR,  48MHz */
  WRITE_REG32(CM_CMU->PLLCFGR, (( 1UL-1UL)<<CMU_PLLCFGR_MPLLM_POS) |  //PLLM
                               ((48UL-1UL)<<CMU_PLLCFGR_MPLLN_POS) |  //PLLN
                               (( 2UL-1UL)<<CMU_PLLCFGR_MPLLP_POS) |  //PLLP
                               (( 2UL-1UL)<<CMU_PLLCFGR_MPLLQ_POS) |  //PLLQ
                               (( 8UL-1UL)<<CMU_PLLCFGR_MPLLR_POS));  //PLLR
#else
  /* HRC Enable */
  WRITE_REG8(CM_CMU->HRCCR, 0);
  Wait_ClkStable(CMU_OSCSTBSR_HRCSTBF, CLK_TIMEOUT);
  /* set PLL source in advance */
  SET_REG32_BIT(CM_CMU->PLLCFGR, CMU_PLLCFGR_PLLSRC);//MPLL source HRC
  /* Init PLL initial structure with default value */
  /* MPLL_VCO = PLL_IN / PLLM * PLLN：240MHz~480MHz
    * f_MPLL_P = MPLL_VCO / PLLP, 192MHz
    * f_MPLL_Q = MPLL_VCO / PLLQ, 192MHz 
    * f_MPLL_R = MPLL_VCO / PLLR,  48MHz */
  if (1UL == (HRC_FREQ_MON & 1UL)) { //HRC=16MHz
      WRITE_REG32(CM_CMU->PLLCFGR, (( 1UL-1UL)<<CMU_PLLCFGR_MPLLM_POS) |  //PLLM
                                   ((24UL-1UL)<<CMU_PLLCFGR_MPLLN_POS) |  //PLLN
                                   (( 2UL-1UL)<<CMU_PLLCFGR_MPLLP_POS) |  //PLLP
                                   (( 2UL-1UL)<<CMU_PLLCFGR_MPLLQ_POS) |  //PLLQ
                                   (( 8UL-1UL)<<CMU_PLLCFGR_MPLLR_POS) |  //PLLR
                                   CMU_PLLCFGR_PLLSRC );
  } else {  //HRC=20MHz
      WRITE_REG32(CM_CMU->PLLCFGR, (( 5UL-1UL)<<CMU_PLLCFGR_MPLLM_POS) |  //PLLM
                                   ((96UL-1UL)<<CMU_PLLCFGR_MPLLN_POS) |  //PLLN
                                   (( 2UL-1UL)<<CMU_PLLCFGR_MPLLP_POS) |  //PLLP
                                   (( 2UL-1UL)<<CMU_PLLCFGR_MPLLQ_POS) |  //PLLQ
                                   (( 8UL-1UL)<<CMU_PLLCFGR_MPLLR_POS) |  //PLLR
                                   CMU_PLLCFGR_PLLSRC );
  }
#endif
  //MPLL Enable
  WRITE_REG8(PLL_EN_REG, 0);
  /* Wait MPLL ready. */
  while ( READ_REG8_BIT(CM_CMU->OSCSTBSR, CMU_OSCSTBSR_MPLLSTBF) == 0 ) {

  }

  /* sram init include read/write wait cycle setting */
  WRITE_REG32(CM_SRAMC->WTCR, (0<<SRAMC_WTCR_SRAMH_RWT_POS)  | (0<<SRAMC_WTCR_SRAMH_WWT_POS)  |   /* SRAMH */
                              (1<<SRAMC_WTCR_SRAM12_RWT_POS) | (1<<SRAMC_WTCR_SRAM12_WWT_POS) |   /* SRAM12 */
                              (1<<SRAMC_WTCR_SRAM3_RWT_POS)  | (1<<SRAMC_WTCR_SRAM3_WWT_POS)  |   /* SRAM3 */
                              (1<<SRAMC_WTCR_SRAMR_RWT_POS)  | (1<<SRAMC_WTCR_SRAMR_WWT_POS));    /* SRAMR */

  /* flash read wait cycle setting */
  Set_EFMWaitCycle(EFM_WAIT_CYCLE5);

  /* 3 cycles for 126MHz ~ 200MHz */
  MODIFY_REG16(CM_GPIO->PCCR, GPIO_PCCR_RDWT, GPIO_RD_WAIT3);

  /* Switch driver ability ， Power On default high speed mode for mian clocks < 168MHz */
  MODIFY_REG8(CM_PWC->PWRC2, PWC_PWRC2_DDAS | PWC_PWRC2_DVS, PWC_PWRC2_DDAS);
  WRITE_REG8(CM_PWC->MDSWCR, PWC_MD_SWITCH_CMD);
  /* Delay 30uS*/
  DelayUS(PWC_MD_SWITCH_TIMEOUT);

  /* Switch system clock source to MPLL. */
  Set_SysClockSrc(CLK_SYSCLK_SRC_PLL);

  /* Reset cache ram */
  WRITE_REG32(bCM_EFM->FRMC_b.CRST, ENABLE);
  WRITE_REG32(bCM_EFM->FRMC_b.CRST, DISABLE);
  /* Enable cache */
  WRITE_REG32(bCM_EFM->FRMC_b.CACHE, ENABLE);

  /* GPIO registers write protected */
  WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_LOCK_KEY);
  /* FCG registers write protected */
  WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_LOCK_KEY);
  /* EFM registers write protected */
  WRITE_REG32(CM_EFM->FAPRT, EFM_REG_LOCK_KEY);
  /* SRAM registers write protected */
  WRITE_REG32(CM_SRAMC->WTPR, SRAM_REG_LOCK_KEY);
  WRITE_REG32(CM_SRAMC->CKPR, SRAM_REG_LOCK_KEY);
  /* PWC, CLK, RMU registers write protected */
  CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~( PWC_UNLOCK_CODE0 | PWC_UNLOCK_CODE1)) & (CM_PWC->FPRC)));
}

/**
 * @brief  Wait clock stable flag.
 * @param  [in] u8Flag      Specifies the stable flag to be wait. @ref CLK_STB_Flag
 * @param  [in] u32Time     Specifies the time to wait while the flag not be set.
 * @retval int32_t
 */
int32_t Wait_ClkStable(uint8_t u8Flag, uint32_t u32Time)
{
    volatile uint32_t u32Timeout = 0UL;
    int32_t i32Ret = LL_ERR_TIMEOUT;

    while (u32Timeout <= u32Time) {
        if( READ_REG8_BIT(CM_CMU->OSCSTBSR, u8Flag) != 0 ) {
            i32Ret = LL_OK;
            break;
        }
        u32Timeout++;
    }
    return i32Ret;
}

void Set_SysClockSrc(uint8_t u8Src)
{
    uint8_t u8TmpFlag = 0U;
    /* backup FCGx setting */
    volatile uint32_t fcg0 = CM_PWC->FCG0;
    volatile uint32_t fcg1 = CM_PWC->FCG1;
    volatile uint32_t fcg2 = CM_PWC->FCG2;
    volatile uint32_t fcg3 = CM_PWC->FCG3;

    /* Only current system clock source or target system clock source is PLLH
    need to close fcg0~fcg3 and open fcg0~fcg3 during switch system clock source.
    We need to backup fcg0~fcg3 before close them. */
    if (CLK_SYSCLK_SRC_PLL == READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW) || (CLK_SYSCLK_SRC_PLL == u8Src)) {
        u8TmpFlag = 1U;
        /* Close FCGx. */
        WRITE_REG32(CM_PWC->FCG0, CLK_FCG0_DEFAULT);
        WRITE_REG32(CM_PWC->FCG1, CLK_FCG1_DEFAULT);
        WRITE_REG32(CM_PWC->FCG2, CLK_FCG2_DEFAULT);
        WRITE_REG32(CM_PWC->FCG3, CLK_FCG3_DEFAULT);
        /* Wait stable after close FCGx. */
        DelayUS(CLK_SYSCLK_SW_STB);
    }
    /* Set system clock source */
    WRITE_REG8(CM_CMU->CKSWR, u8Src);
    /* Wait stable after setting system clock source */
    DelayUS(CLK_SYSCLK_SW_STB);
    if (1U == u8TmpFlag) {
        WRITE_REG32(CM_PWC->FCG0, fcg0);
        WRITE_REG32(CM_PWC->FCG1, fcg1);
        WRITE_REG32(CM_PWC->FCG2, fcg2);
        WRITE_REG32(CM_PWC->FCG3, fcg3);
        /* Wait stable after open fcg. */
        DelayUS(CLK_SYSCLK_SW_STB);
    }
}

void Set_SysClockDiv(uint32_t u32Clock, uint32_t u32Div)
{
    uint8_t u8TmpFlag = 0U;
    /* backup FCGx setting */
    volatile uint32_t fcg0 = CM_PWC->FCG0;
    volatile uint32_t fcg1 = CM_PWC->FCG1;
    volatile uint32_t fcg2 = CM_PWC->FCG2;
    volatile uint32_t fcg3 = CM_PWC->FCG3;

    /* Only current system clock source or target system clock source is PLLH
    need to close fcg0~fcg3 and open fcg0~fcg3 during switch system clock source.
    We need to backup fcg0~fcg3 before close them. */
    if (CLK_SYSCLK_SRC_PLL == READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW)) {
        u8TmpFlag = 1U;
        /* Close FCGx. */
        WRITE_REG32(CM_PWC->FCG0, CLK_FCG0_DEFAULT);
        WRITE_REG32(CM_PWC->FCG1, CLK_FCG1_DEFAULT);
        WRITE_REG32(CM_PWC->FCG2, CLK_FCG2_DEFAULT);
        WRITE_REG32(CM_PWC->FCG3, CLK_FCG3_DEFAULT);
        /* Wait stable after close FCGx. */
        DelayUS(CLK_SYSCLK_SW_STB);
    }
    MODIFY_REG32(CM_CMU->SCFGR, u32Clock, u32Div);
    DelayUS(CLK_SYSCLK_SW_STB);
    if (1U == u8TmpFlag) {
        WRITE_REG32(CM_PWC->FCG0, fcg0);
        WRITE_REG32(CM_PWC->FCG1, fcg1);
        WRITE_REG32(CM_PWC->FCG2, fcg2);
        WRITE_REG32(CM_PWC->FCG3, fcg3);
        /* Wait stable after open fcg. */
        DelayUS(CLK_SYSCLK_SW_STB);
    }
}

/**
 * @brief  Set the efm read wait cycles.
 * @param  [in] u32WaitCycle            Specifies the efm read wait cycles.
 *    @arg  This parameter can be of a value of @ref EFM_Wait_Cycle
 * @retval int32_t:
 *         - LL_OK: Program successfully.
 *         - LL_ERR_TIMEOUT: EFM is not ready.
 * @note   Call EFM_REG_Unlock() unlock EFM register first.
 */
int32_t Set_EFMWaitCycle(uint32_t u32WaitCycle)
{
    uint32_t u32Timeout = 0UL;

    MODIFY_REG32(CM_EFM->FRMC, EFM_FRMC_FLWT, u32WaitCycle);
    while (u32WaitCycle != READ_REG32_BIT(CM_EFM->FRMC, EFM_FRMC_FLWT)) {
        u32Timeout++;
        if (u32Timeout > EFM_TIMEOUT) {
            return LL_ERR_TIMEOUT;
        }
    }
    return LL_OK;
}

/**
 * @brief Delay function, delay ms approximately
 * @param [in] u32Count                   ms
 * @retval None
 */
#if defined (__CC_ARM)  /*!< ARM Compiler */
#pragma push
#pragma O0
#endif
__NO_OPTIMIZE void DelayMS(uint32_t u32Count)
{
    __IO uint32_t i;
    const uint32_t u32Cyc = (HCLK_VALUE + 6000UL - 1UL) / 6000UL;

    while (u32Count-- > 0UL) {
        i = u32Cyc;
        while (i-- > 0UL) {
        }
    }
}

/**
 * @brief Delay function, delay us approximately
 * @param [in] u32Count                   us
 * @retval None
 */
__NO_OPTIMIZE void DelayUS(uint32_t u32Count)
{
    __IO uint32_t i;
    const uint32_t u32Cyc = (HCLK_VALUE + 6000000UL - 1UL) / 6000000UL;

    while (u32Count-- > 0UL) {
        i = u32Cyc;
        while (i-- > 0UL) {
        }
    }
}
#if defined (__CC_ARM)  /*!< ARM Compiler */
#pragma pop
#endif

