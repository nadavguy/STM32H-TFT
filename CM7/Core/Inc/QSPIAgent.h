/*
 * QSPIAgent.h
 *
 *  Created on: Jan 26, 2025
 *      Author: nadavguy
 */

#ifndef INC_QSPIAGENT_H_
#define INC_QSPIAGENT_H_

/**
  ******************************************************************************
  * @file    stm32746g_discovery_qspi.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    25-June-2015
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32746g_discovery_qspi.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32746G_DISCOVERY
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "w25n01.h"

/** @addtogroup STM32746G_DISCOVERY_QSPI
  * @{
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32746G_DISCOVERY_QSPI_Exported_Constants STM32746G_DISCOVERY_QSPI Exported Constants
  * @{
  */
/* QSPI Error codes */
#define QSPI_OK            ((uint8_t)0x00)
#define QSPI_ERROR         ((uint8_t)0x01)
#define QSPI_BUSY          ((uint8_t)0x02)
#define QSPI_NOT_SUPPORTED ((uint8_t)0x04)
#define QSPI_SUSPENDED     ((uint8_t)0x08)


/* W25Q128FV Micron memory */
/* Size of the flash */
#define QSPI_FLASH_SIZE            31     /* Address bus width to access whole memory space */
#define QSPI_PAGE_SIZE             2048

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32746G_DISCOVERY_QSPI_Exported_Types STM32746G_DISCOVERY_QSPI Exported Types
  * @{
  */
/* QSPI Info */
typedef struct {
  uint32_t FlashSize;          /*!< Size of the flash */
  uint32_t EraseSectorSize;    /*!< Size of sectors for the erase operation */
  uint32_t EraseSectorsNumber; /*!< Number of sectors for the erase operation */
  uint32_t ProgPageSize;       /*!< Size of pages for the program operation */
  uint32_t ProgPagesNumber;    /*!< Number of pages for the program operation */
} QSPI_Info;

/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @addtogroup STM32746G_DISCOVERY_QSPI_Exported_Functions
  * @{
  */
HAL_StatusTypeDef W25N01_Init(QSPI_HandleTypeDef *hqspi);
HAL_StatusTypeDef W25N01_ReadPage(QSPI_HandleTypeDef *hqspi, uint32_t page, uint8_t *buffer, uint32_t length);
HAL_StatusTypeDef W25N01_WritePage(QSPI_HandleTypeDef *hqspi, uint32_t page, const uint8_t *buffer, uint32_t length);
HAL_StatusTypeDef W25N01_EraseBlock(QSPI_HandleTypeDef *hqspi, uint32_t block);
uint32_t W25N01_ReadID(QSPI_HandleTypeDef *hqspi);

extern void fileSystemInit(void);
extern HAL_StatusTypeDef writeEnable(void);
extern HAL_StatusTypeDef checkWriteEnableLatch(void);
extern uint32_t calculateRowAddress(uint32_t page);
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#endif /* INC_QSPIAGENT_H_ */
