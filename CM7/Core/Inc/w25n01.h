/*
 * w25n01.h
 *
 *  Created on: Jan 26, 2025
 *      Author: nadavguy
 */

#ifndef INC_W25N01_H_
#define INC_W25N01_H_


/**
  ******************************************************************************
  * @file    n25q128a.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2015
  * @brief   This file contains all the description of the N25Q128A QSPI memory.
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif

#define W25N01_CMD_READ_ID          0x9F
#define W25N01_CMD_READ_PAGE        0x13
#define W25N01_CMD_READ_DATA        0x03
#define W25N01_CMD_WRITE_ENABLE     0x06
#define W25N01_CMD_PROGRAM_LOAD     0x02
#define W25N01_CMD_PROGRAM_EXECUTE  0x10
#define W25N01_CMD_BLOCK_ERASE      0xD8

#define W25N01_PAGE_SIZE            2048
#define W25N01_BLOCK_SIZE           (64 * W25N01_PAGE_SIZE)  // 128KB

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
#endif /* INC_W25N01_H_ */
