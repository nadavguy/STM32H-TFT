#include "QSPIAgent.h"
#include "w25n01.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "quadspi.h"

static QSPI_HandleTypeDef *g_qspi;
#define W25N01_WRITE_ENABLE_CMD  0x06

uint8_t buffer[2048] __attribute__((aligned(4)));

// Helper function for QSPI command transmission
static HAL_StatusTypeDef W25N01_SendCommand(QSPI_HandleTypeDef *hqspi, uint8_t cmd, uint32_t addr, uint8_t dummy_cycles) {
    QSPI_CommandTypeDef qspi_cmd = {0};
//    uint32_t rowAddress = calculateRowAddress(addr);
    qspi_cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    qspi_cmd.AddressMode       = QSPI_ADDRESS_1_LINE;
    qspi_cmd.AddressSize       = QSPI_ADDRESS_24_BITS;
    qspi_cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    qspi_cmd.DataMode          = QSPI_DATA_NONE;
    qspi_cmd.DummyCycles       = dummy_cycles;
    qspi_cmd.Instruction       = cmd;
    qspi_cmd.Address           = addr;
    qspi_cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    return HAL_QSPI_Command(hqspi, &qspi_cmd, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef W25N01_SendCommandErase(QSPI_HandleTypeDef *hqspi, uint8_t cmd, uint32_t addr, uint8_t dummy_cycles)
{
    QSPI_CommandTypeDef qspi_cmd = {0};
    uint32_t rowAddress = calculateRowAddress(addr);
    qspi_cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    qspi_cmd.AddressMode       = QSPI_ADDRESS_1_LINE;
    qspi_cmd.AddressSize       = QSPI_ADDRESS_24_BITS;
    qspi_cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    qspi_cmd.DataMode          = QSPI_DATA_NONE;
    qspi_cmd.DummyCycles       = dummy_cycles;
    qspi_cmd.Instruction       = cmd;
    qspi_cmd.Address           = addr * 131072;
    qspi_cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    return HAL_QSPI_Command(hqspi, &qspi_cmd, HAL_MAX_DELAY);
}

HAL_StatusTypeDef W25N01_Init(QSPI_HandleTypeDef *hqspi)
{
    g_qspi = hqspi;

    // Verify the device by reading its ID
    uint32_t id = W25N01_ReadID(hqspi);
    if (id != 0xEFAA21)
    {  // Replace with the actual ID from the datasheet
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint32_t W25N01_ReadID(QSPI_HandleTypeDef *hqspi) {
    uint8_t id[3] = {0};

    QSPI_CommandTypeDef qspi_cmd = {
        .InstructionMode   = QSPI_INSTRUCTION_1_LINE,
        .Instruction       = W25N01_CMD_READ_ID,
        .AddressMode       = QSPI_ADDRESS_NONE,
        .AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
        .DataMode          = QSPI_DATA_1_LINE,
        .DummyCycles       = 8,
        .NbData            = 3,
        .SIOOMode          = QSPI_SIOO_INST_EVERY_CMD,
    };

    HAL_QSPI_Command(hqspi, &qspi_cmd, HAL_MAX_DELAY);
    HAL_QSPI_Receive(hqspi, id, HAL_MAX_DELAY);


    return (id[0] << 16) | (id[1] << 8) | id[2];
}

HAL_StatusTypeDef W25N01_ReadPage(QSPI_HandleTypeDef *hqspi, uint32_t page, uint8_t *buffer, uint32_t length)
{
    if (length > W25N01_PAGE_SIZE)
    	return HAL_ERROR;

    // Issue the page read command
//    if (W25N01_SendCommand(hqspi, W25N01_CMD_READ_PAGE, page, 0) != HAL_OK)
//    {
//        return HAL_ERROR;
//    }
    QSPI_CommandTypeDef qspi_cmd = {0};
//    uint32_t rowAddress = calculateRowAddress(addr);
    qspi_cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    qspi_cmd.AddressMode       = QSPI_ADDRESS_1_LINE;
    qspi_cmd.AddressSize       = QSPI_ADDRESS_24_BITS;
    qspi_cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    qspi_cmd.DataMode          = QSPI_DATA_NONE;
    qspi_cmd.DummyCycles       = 0;
    qspi_cmd.Instruction       = W25N01_CMD_READ_PAGE;
    qspi_cmd.Address           = page;
    qspi_cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    if( HAL_QSPI_Command(hqspi, &qspi_cmd, HAL_MAX_DELAY) != HAL_OK)
    {
    	return HAL_ERROR;
    }

    // Read the page data
    QSPI_CommandTypeDef qspi_cmd2 =
    {
        .InstructionMode   = QSPI_INSTRUCTION_1_LINE,
        .Instruction       = W25N01_CMD_READ_DATA,
        .AddressMode       = QSPI_ADDRESS_1_LINE,
        .AddressSize       = QSPI_ADDRESS_16_BITS,
        .Address           = 0,
        .AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
        .DataMode          = QSPI_DATA_1_LINE,
        .DummyCycles       = 8,
        .NbData            = length,
        .SIOOMode          = QSPI_SIOO_INST_EVERY_CMD,
    };

    if (HAL_QSPI_Command(hqspi, &qspi_cmd2, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_QSPI_Receive(hqspi, buffer, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef W25N01_WritePage(QSPI_HandleTypeDef *hqspi, uint32_t page, const uint8_t *buffer, uint32_t length)
{
    if (length > W25N01_PAGE_SIZE)
    	return HAL_ERROR;

    // Enable write operations
//    if (W25N01_SendCommand(hqspi, W25N01_CMD_WRITE_ENABLE, 0, 0) != HAL_OK)
//    {
//        return HAL_ERROR;
//    }
    QSPI_CommandTypeDef qspi_cmd = {0};
//    uint32_t rowAddress = calculateRowAddress(addr);
    qspi_cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    qspi_cmd.AddressMode       = QSPI_ADDRESS_NONE;
    qspi_cmd.AddressSize       = QSPI_ADDRESS_24_BITS;
    qspi_cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    qspi_cmd.DataMode          = QSPI_DATA_NONE;
    qspi_cmd.DummyCycles       = 0;
    qspi_cmd.Instruction       = W25N01_CMD_WRITE_ENABLE;
    qspi_cmd.Address           = 0;
    qspi_cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(hqspi, &qspi_cmd, HAL_MAX_DELAY) != HAL_OK)
    {
    	return HAL_ERROR;
    }

    // Load data into the NAND page buffer
    QSPI_CommandTypeDef qspi_cmd2 =
    {
        .InstructionMode   = QSPI_INSTRUCTION_1_LINE,
        .Instruction       = W25N01_CMD_PROGRAM_LOAD,
        .AddressMode       = QSPI_ADDRESS_1_LINE,
        .AddressSize       = QSPI_ADDRESS_16_BITS,
        .Address           = 0,
        .AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
        .DataMode          = QSPI_DATA_1_LINE,
        .DummyCycles       = 0,
        .NbData            = length,
        .SIOOMode          = QSPI_SIOO_INST_EVERY_CMD,
    };

    if (HAL_QSPI_Command(hqspi, &qspi_cmd2, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_QSPI_Transmit(hqspi, (uint8_t *)buffer, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // Execute the program command
//    if (W25N01_SendCommand(hqspi, W25N01_CMD_PROGRAM_EXECUTE, page, 8) != HAL_OK)
//    {
//        return HAL_ERROR;
//    }
    QSPI_CommandTypeDef qspi_cmd3 =
    {
    		.InstructionMode   = QSPI_INSTRUCTION_1_LINE,
			.Instruction       = W25N01_CMD_PROGRAM_EXECUTE,
			.AddressMode       = QSPI_ADDRESS_1_LINE,
			.AddressSize       = QSPI_ADDRESS_24_BITS,
			.Address           = 0,
			.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
			.DataMode          = QSPI_DATA_NONE,
			.DummyCycles       = 0,
			.NbData            = 0,
			.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD,
    };

    if (HAL_QSPI_Command(hqspi, &qspi_cmd3, HAL_MAX_DELAY) != HAL_OK)
    {
    	return HAL_ERROR;
    }

    HAL_Delay(5);  // Wait for programming to complete

    return HAL_OK;
}

HAL_StatusTypeDef W25N01_EraseBlock(QSPI_HandleTypeDef *hqspiToUse, uint32_t block)
{
//    uint32_t page = block * 64;  // Each block contains 64 pages

//    // Enable write operations
//    if (W25N01_SendCommand(hqspi, W25N01_CMD_WRITE_ENABLE, 0, 0) != HAL_OK)
//    {
//        return HAL_ERROR;
//    }
//
//    // Issue the block erase command
//    if (W25N01_SendCommandErase(hqspi, W25N01_CMD_BLOCK_ERASE, block, 8) != HAL_OK)
//    {
//        return HAL_ERROR;
//    }

	for (int i = block * 64 ; i < (block + 1) * 64 ; i = i + 1)
	{
		memset(buffer, 0xFF, sizeof(buffer));
		if (W25N01_WritePage(hqspiToUse, i, buffer, sizeof(buffer)) != HAL_OK)
		{
			printf("Page write failed!\r\n");
		}
	}

    HAL_Delay(10);  // Wait for block erase to complete

    return HAL_OK;
}

/**
 * @brief Enable write operations for W25N01GZ NAND Flash
 * @retval HAL_StatusTypeDef - HAL_OK if successful, otherwise HAL_ERROR
 */
HAL_StatusTypeDef writeEnable(void)
{
    QSPI_CommandTypeDef sCommand = {0};

    // Configure the QSPI command structure
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;  // Use single line for instruction
    sCommand.Instruction       = W25N01_WRITE_ENABLE_CMD;  // Write Enable command
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;        // No address for this command
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; // No alternate bytes
    sCommand.DataMode          = QSPI_DATA_NONE;           // No data for this command
    sCommand.DummyCycles       = 0;                        // No dummy cycles
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;    // Disable DDR mode
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY; // Default hold delay
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD; // Send instruction for every command

    // Send the Write Enable command
    if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    // Wait until the write enable latch is set
//    if (checkWriteEnableLatch() != HAL_OK) {
//        return HAL_ERROR;
//    }

    return HAL_OK;
}

/**
 * @brief Check if the Write Enable Latch (WEL) is set in the Status Register
 * @retval HAL_StatusTypeDef - HAL_OK if WEL is set, otherwise HAL_ERROR
 */
HAL_StatusTypeDef checkWriteEnableLatch(void)
{
    QSPI_CommandTypeDef sCommand = {0};
    uint8_t regData;

    // Configure the command to read the status register
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;  // Use single line for instruction
    sCommand.Instruction       = 0x05;                    // Read Status Register command
    sCommand.AddressMode       = 0xC0;        // No address for this command
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; // No alternate bytes
    sCommand.DataMode          = QSPI_DATA_1_LINE;         // Data on a single line
    sCommand.DummyCycles       = 0;                        // No dummy cycles
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;    // Disable DDR mode
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY; // Default hold delay
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD; // Send instruction for every command

    // Send the command to read the status register
    if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    // Receive the status register value
    if (HAL_QSPI_Receive(&hqspi, &regData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    // Check if the Write Enable Latch (WEL) bit is set (bit 1 in status register)
    if ((regData & 0x02) == 0) {
        return HAL_ERROR;  // WEL not set
    }

    return HAL_OK;  // WEL is set
}

uint32_t calculateRowAddress(uint32_t page)
{
    uint32_t blockNumber = page / 64;  // Calculate the block number
    uint32_t pageNumber  = page % 64; // Calculate the page within the block
    return (blockNumber << 6) | pageNumber; // Combine block and page addresses
}

void fileSystemInit(void)
{

	FRESULT FS_ret;

	DWORD free_clusters, free_sectors, total_sectors;
	FATFS *getFreeFs;


//	memset(buffer, 0xFF, sizeof(buffer));
//	buffer[0] = 48;
//	buffer[2047] = 49;
//
//	writeEnable();
//

//	for (int i = 0 ; i < 1024 ; i++)
//	{
//		W25N01_EraseBlock(&hqspi, i);
//	}
	if (W25N01_ReadPage(&hqspi, 63, buffer, sizeof(buffer)) != HAL_OK)
	{
		printf("Page read failed!\r\n");
	}
	else
	{
		printf("Page read success, data: 0x%02X\r\n", buffer[0]);
	}
	sprintf(buffer,"\xEB\xFE\x90" "MSDOS5.0");
	if (W25N01_WritePage(&hqspi, 63, buffer, sizeof(buffer)) != HAL_OK)
	{
	    printf("Page write failed!\r\n");
	}

	if (W25N01_ReadPage(&hqspi, 63, buffer, sizeof(buffer)) != HAL_OK)
	{
		printf("Page read failed!\r\n");
	}
	else
	{
		printf("Page read success, data: 0x%02X\r\n", buffer[0]);
	}
//	memset(buffer, 0xFF, sizeof(buffer));
//	// Test read
//	if (W25N01_ReadPage(&hqspi, 63, buffer, sizeof(buffer)) != HAL_OK) {
//	    printf("Page read failed!\r\n");
//	} else {
//	    printf("Page read success, data: 0x%02X\r\n", buffer[0]);
//	}

	for (int i = 1 ; i < 65536 ; i++)
	{
//		sprintf(buffer,"\xEB\xFE\x90" "MSDOS5.0");
//		buffer[2047] = i%255;
//		if (W25N01_WritePage(&hqspi, i, buffer, sizeof(buffer)) != HAL_OK) {
//			printf("Page write failed!\r\n");
//		}
		memset(buffer, 0xFF, sizeof(buffer));
		// Test read
		if (W25N01_ReadPage(&hqspi, i, buffer, W25N01_PAGE_SIZE) != HAL_OK)
		{
			printf("Page read failed!\r\n");
		}
		else
		{
			printf("Page read success, data: 0x%02X\r\n", buffer[0]);
		}
	}

	while(1)
	{
		HAL_Delay(100);
	}
//	// Test erase and write
//	for (int i = 0 ; i < 1024 ; i++)
//	{
//		if (W25N01_EraseBlock(&hqspi, i) != HAL_OK)
//		{
//			printf("Block erase failed!\r\n");
//		}
//
//		buffer[0] = 48;
//		buffer[1] = 49;
//
//		if (W25N01_ReadPage(&hqspi, i, buffer, sizeof(buffer)) != HAL_OK)
//		{
//			printf("Page read failed!\r\n");
//		}
//		else
//		{
//			printf("Page read success, data: 0x%02X\r\n", buffer[0]);
//		}
//	}

	do
	{
		HAL_Delay(1);
		FS_ret = f_mount(&USERFatFS, "0:/", 0);
	} while (FS_ret != FR_OK);

	FS_ret = f_getfree("0:/", &free_clusters, &getFreeFs);
	//	FS_ret = f_mkfs("\\", FM_EXFAT, 0, buffer, sizeof(buffer));
	if (FS_ret != FR_OK)
	{
		FS_ret = f_mkfs("0:/", FM_ANY, 0, buffer, sizeof(buffer));
		if (W25N01_ReadPage(&hqspi, 63, (uint8_t *)buffer, W25N01_PAGE_SIZE) != HAL_OK)
		{
			printf("Page read failed!\r\n");
		}
		else
		{
			printf("Page read success, data: 0x%02X\r\n", buffer[0]);
		}
		if (W25N01_ReadPage(&hqspi, 64, buffer, W25N01_PAGE_SIZE) != HAL_OK)
		{
			printf("Page read failed!\r\n");
		}
		else
		{
			printf("Page read success, data: 0x%02X\r\n", buffer[0]);
		}
		FS_ret = f_getfree("0:/", &free_clusters, &getFreeFs);
	}

	total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
	free_sectors = free_clusters * getFreeFs->csize;
	float free_kb = (float)free_sectors*(float)(2048.0)/1048576;
	float total_kb = (float)total_sectors*(float)(2048.0)/1048576;
//	f_mount(&USERFatFS, (TCHAR const *)USERPath, 1);
}
