#include <stddef.h>
#include <string.h>

#include "nvs.h"

// Note that the area used for storage must be aligned to the flash's pages. If
// you want to use a larger area, be sure to also change the size and location
// in the linker script.
#define NVS_AREA_SIZE           1024
#define NVS_BLOCK_COUNT         (NVS_AREA_SIZE / sizeof(NVS_Block_t))
#define NVS_VALID_BLOCK_MARKER  0xab34

typedef struct
__attribute__((packed))
{
    // Actual block data
    NVS_Data_t data;

    // Ensure struct size is divisible by two
    uint8_t padding[sizeof(NVS_Data_t) % 2];

    // Block marker for detecting if the block is the current or an old block
    uint16_t marker;

    uint32_t crc;
} NVS_Block_t;

__attribute__((used, section(".nvstore")))
volatile NVS_Block_t NVS_Area[NVS_BLOCK_COUNT];

volatile NVS_Block_t *NVS_FlashData;
__attribute__((used))
NVS_Block_t NVS_RAMData;

NVS_Data_t *const NVS_Data = &NVS_RAMData.data;

static uint32_t NVS_CalculateCRC(NVS_Data_t *data)
{
    CRC->CR = CRC_CR_RESET;
    for(int i = 0; i < sizeof(NVS_Data_t); i++)
    {
        CRC->DR = ((uint8_t*)(data))[i];
    }
    return CRC->DR;
}

static void NVS_ProgramHalfWord(uint16_t *dest, uint16_t value)
{
    FLASH->CR |= FLASH_CR_PG;
    *(volatile uint16_t*)dest = value;
    while(FLASH->SR & FLASH_SR_BSY);
    if(*dest != value)
    {
        // Write failed
        __asm__("bkpt");
    }
}

static void NVS_UnlockFlash(void)
{
    while(FLASH->SR & FLASH_SR_BSY);
    if(FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xcdef89ab;
    }
}

static void NVS_EraseArea(void)
{
    for(int i = 0; i < NVS_AREA_SIZE; i += 1024)
    {
        while(FLASH->SR & FLASH_SR_BSY);
        FLASH->CR |= FLASH_CR_PER;
        FLASH->AR = (uint32_t)NVS_Area + i;
        FLASH->CR |= FLASH_CR_STRT;
        while(FLASH->SR & FLASH_SR_BSY);
        if(FLASH->SR & FLASH_SR_EOP)
        {
            // Clear EOP flag
            FLASH->SR = FLASH_SR_EOP;
        }
        else
        {
            // Erase failed
            __asm__("bkpt");
        }
        FLASH->CR &= ~FLASH_CR_PER;
    }
}

static bool NVS_BlockEmpty(NVS_Block_t *block)
{
    for(int i = 0; i < sizeof(NVS_Block_t) / 2; i++)
    {
        if(*((uint16_t*)block + i) != 0xffff)
        {
            return false;
        }
    }
    return true;
}

static void NVS_LoadDefaults(void)
{
    NVS_Data->set_temperature = 350;
}

bool NVS_Load(void)
{
    RCC->AHBENR |= RCC_AHBENR_CRCEN;

    volatile NVS_Block_t *block = NULL;

    // Find valid block
    for(int i = 0; i < NVS_BLOCK_COUNT; i++)
    {
        block = &NVS_Area[i];
        if(block->marker == NVS_VALID_BLOCK_MARKER)
        {
            // Valid block found, next up is the CRC check
            break;
        }
        else if(block->marker == 0xffff)
        {
            // This block was erased and no block before was valid, so we can
            // assume there is no valid block
            block = NULL;
            break;
        }
    }

    if(block == NULL || block->marker != NVS_VALID_BLOCK_MARKER
        || block->crc != NVS_CalculateCRC(&block->data))
    {
        // No valid block found
        NVS_LoadDefaults();
        return false;
    }
    else
    {
        // Valid block and CRC check successful
        NVS_FlashData = block;
        memcpy(&NVS_RAMData, (void*)block, sizeof(NVS_Block_t));
        return true;
    }
}

void NVS_Save(void)
{
    NVS_UnlockFlash();

    // Currently loaded block. Is NULL if the defaults were loaded in NVS_Load()
    // instead of some flash contents.
    NVS_Block_t *current_block = NVS_FlashData;
    NVS_Block_t *next_block = NVS_FlashData + 1;

    if(current_block == NULL || next_block > NVS_Area + NVS_BLOCK_COUNT
        || !NVS_BlockEmpty(next_block))
    {
        NVS_EraseArea();
        next_block = &NVS_Area[0];
        current_block = NULL;
    }

    NVS_RAMData.crc = NVS_CalculateCRC(NVS_Data);
    NVS_RAMData.marker = NVS_VALID_BLOCK_MARKER;

    // The block length is guaranteed to be divisible by 2
    for(int i = 0; i < sizeof(NVS_Block_t) / 2; i++)
    {
        NVS_ProgramHalfWord((uint16_t*)next_block + i,
            *((uint16_t*)&NVS_RAMData + i));
    }

    if(current_block != NULL)
    {
        NVS_ProgramHalfWord((uint16_t*)&current_block->marker, 0x0000);
    }

    NVS_FlashData = next_block;
}
