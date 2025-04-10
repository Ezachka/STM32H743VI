/**
 *******************************************
 * @file    w25q_mem.c
 * @author  Serbinov Alexey
 * @version 0.2b
 * @date    17-01-2024
 * @brief   Source file for W25Qxxx lib
 * @note    
 *******************************************
 *
 * @note https://ru.mouser.com/datasheet/2/949/w25q256jv_spi_revg_08032017-1489574.pdf
 * @note https://www.st.com/resource/en/application_note/DM00227538-.pdf
 */

/**
 * @addtogroup W25Q_Driver
 * @{
 */

#include "w25q_mem.h"
#include "gpio_config.h"


mem_t mem;

/**
 * @addtogroup W25Q_Exp Exported types
 * @brief External fields and data
 * @{
 */

/// @}

/**
 * @addtogroup W25Q_PrivFi Private fields
 * @brief Private variables and defines
 * @{
 */
W25Q_SRs_t W25Q_SR;

/// @}

/**
 * @addtogroup W25Q_PrivFu Private Methods
 * @brief Internal lib's functions
 * @{
 */


static inline uint32_t page_to_addr(uint32_t pageNum, uint8_t pageShift); ///< Translate page addr to byte addr
/// @}

/**
 * @addtogroup W25Q_Pub Public methods
 * @brief Public Methods
 * @{
 */

const uint32_t Timeout_SPI_tx_rx=500;



/**
 * @addtogroup W25Q_Init Init methods
 * @brief Initalization
 * @{
 */

/**
 * @brief W25Q Init function
 *
 * @param none
 * @return 
 */
void w25q_init(SPI_TypeDef *SPI_x)
{
    w25q_reset(SPI_x);
    w25q_read_status_struct(SPI_x);   
    mem.mem_w25q_id = w25q_read_jedec_id(SPI_x);
    if(mem.mem_w25q_id !=0xef4016)
    {
        mem.mem_w25q_err=1;
    }
    else
    {
        mem.mem_w25q_err=0;
    }
}


/**
 * @}
 * @addtogroup W25Q_Reg Register Functions
 * @brief Operations with Status Registers
 * @{
 */



/**
 * @brief W25Q Read Status Register
 * Read one status register
 *
 * @param[out] 
 * @param[in] 
 * @return  
 */


void w25q_read_status_reg(SPI_TypeDef *SPI_x,uint8_t *reg_data, uint8_t reg_num) 
{ 
    mem_cs_disable; 

    uint8_t SR1[1];
    if (reg_num == 1)
        SR1[0]  = W25Q_READ_SR1;
    else if (reg_num == 2)
        SR1[0]  = W25Q_READ_SR2;
    else if (reg_num == 3)
        SR1[0]  = W25Q_READ_SR3;

    spi_transmit(SPI_x, SR1,1);

    spi_receive (SPI_x, SR1,1);    

    reg_data[0]=SR1[0];

    mem_cs_enable;
}




/**
 * @brief W25Q Write to Status Register disable QSPI
 * Write one status register
 *
 * @param[in] 
 * @param[in] 
 * @return 
 */
void w25q_disable_qspi(SPI_TypeDef *SPI_x)
{
    uint8_t tx[2];
    tx[0] = W25Q_ENABLE_VOLATILE_SR;
    
    w25q_wait_write_end(SPI_x);
    
    mem_cs_disable; 
    spi_transmit(SPI_x,tx, 1);
    mem_cs_enable;
    
    w25q_wait_write_end(SPI_x);
    
    tx[0] = W25Q_WRITE_SR2;
    tx[1] = ((1 & 0x0F) << 1);

    mem_cs_disable; 
    spi_transmit(SPI_x,tx, 2);
    mem_cs_enable;
    w25q_wait_write_end(SPI_x);
}

/**
 * @brief W25Q Read Status Registers
 * Read all status registers to struct
 *
 * @param[out] 
 * @return 
 */

void w25q_read_status_struct(SPI_TypeDef *SPI_x) 
{
    // buffer register variables
    uint8_t SRs[3] = { 0, };
    // first portion
    w25q_read_status_reg(SPI_x,&SRs[0], 1);
    // second portion
    w25q_read_status_reg(SPI_x,&SRs[1], 2);
    // third portion
    w25q_read_status_reg(SPI_x,&SRs[2], 3);

    W25Q_SR.SR1.Register=SRs[0];
    W25Q_SR.SR2.Register=SRs[1];
    W25Q_SR.SR3.Register=SRs[2];

    if((W25Q_SR.SR1.Register ==0) && (W25Q_SR.SR2.Register==0) && (W25Q_SR.SR3.Register==0)){
        mem.mem_w25q_err=1;
    }
}



/**
 * @}
 * @addtogroup W25Q_Read Read Functions
 * @brief Read operations - single data type variables or raw 8-bit blocks
 * @{
 */

/**
 * @brief W25Q Read data
 * Readdata
 *
 * @param[out] 
 * @param[in] 
 * @param[in] 
 * @return 
 */


void w25q_read(SPI_TypeDef *SPI_x, uint32_t addr,uint8_t *data, uint16_t size)
{
    uint8_t tx[4];
    tx[0]= W25Q_READ_DATA;//read
    tx[1]= (addr<<16) & 0xFF;
    tx[2]= (addr<<8)  & 0xFF;
    tx[3]=  addr      & 0xFF;

    mem_cs_disable; 

    spi_transmit (SPI_x,  tx, 4)  ;
    spi_receive  (SPI_x, data, size)  ;

    mem_cs_enable;
}


/**
 * @}
 * @addtogroup W25Q_Write Write functions
 * @brief Program/write operations - single data type variables or raw 8-bit blocks
 * @{
 */






/**
 * @brief W25Q Write data
 * 
 *
 * @param[in] 
 * @param[in] 
 * @param[in] 
 * @return 
 */
void w25q_write(SPI_TypeDef *SPI_x, uint32_t addr,uint8_t *data, uint16_t size)
{
    uint8_t tx[4];   

    w25q_wait_write_end(SPI_x);
    w25q_set_block_protect(SPI_x,0x00);
    w25q_write_enable(SPI_x);
    mem_cs_disable;  

    tx[0] = W25Q_PAGE_PROGRAM;
    tx[1] = (addr >> 16) & 0xFF;
    tx[2] = (addr >> 8) & 0xFF;
    tx[3] = addr & 0xFF;
    spi_transmit(SPI_x,tx, 4);

    spi_transmit(SPI_x,data, size);

    mem_cs_enable; 

    w25q_write_disable(SPI_x);
    w25q_set_block_protect(SPI_x,0x0F);
  
}

/**
 * @brief W25Q Write Page
 * 
 *
 * @param[in] 
 * @param[in] 
 * @param[in] 
 * @return 
 */
void w25q_write_page(SPI_TypeDef *SPI_x, uint32_t PageNum,uint32_t pageShift,uint8_t *data, uint32_t size)
{
    uint8_t tx[4];   

    if (PageNum >= PAGE_COUNT )
    {
        size=MEM_FLASH_SIZE;
    }
    else if (pageShift > 256 - 4)
    {
        size=MEM_FLASH_SIZE-pageShift;
    }

    uint32_t page_addr = page_to_addr(PageNum, pageShift);

    w25q_wait_write_end(SPI_x);
    w25q_set_block_protect(SPI_x,0x00);
    w25q_write_enable(SPI_x);
    mem_cs_disable;  

    tx[0] = W25Q_PAGE_PROGRAM;
    tx[1] = (page_addr  >> 16) & 0xFF;
    tx[2] = (page_addr  >> 8) & 0xFF;
    tx[3] =  page_addr  & 0xFF;
    spi_transmit(SPI_x,tx, 4);

    spi_transmit(SPI_x,data, size);

    mem_cs_enable; 

    w25q_write_disable(SPI_x);
    w25q_set_block_protect(SPI_x,0x0F);
  
}


/**
 * @}
 * @addtogroup W25Q_Erase Erase functions
 * @brief Erase sector, blocks or whole chip
 * @{
 */

/**
 * @brief W25Q Sector erase (4KB)
 * Minimal size operation to erase data
 *
 * @note Should be executed before writing
 * @param[in] 
 * @return 
 */

void w25q_erase_sector(SPI_TypeDef *SPI_x, uint32_t addr) 
{
    uint8_t tx[4];
    w25q_wait_write_end(SPI_x);
    w25q_set_block_protect(SPI_x,0x00);
    w25q_write_enable(SPI_x);

    mem_cs_disable;

    tx[0] = W25Q_SECTOR_ERASE;
    tx[1] = (addr >> 16) & 0xFF;
    tx[2] = (addr >> 8) & 0xFF;
    tx[3] = addr & 0xFF;

    spi_transmit(SPI_x, tx,4);  

    mem_cs_enable; /// cs off like write disable
    
    w25q_wait_erase_end(SPI_x);
    w25q_wait_write_end(SPI_x);
    w25q_write_disable(SPI_x);
    w25q_set_block_protect(SPI_x,0x0F);
}


/**
 * @brief W25Q Block erase (64 KB)
 * Func to erase big block
 *
 * @note Should be executed before writing
 * @param[in] BlockAddr Block start address
 * @param[in] size Size of block:  64KB
 * @return 
 */

void w25q_erase_block(SPI_TypeDef *SPI_x,uint32_t addr)
{
    uint8_t tx[4];

    w25q_wait_write_end(SPI_x);
    w25q_set_block_protect(SPI_x,0x00);
    w25q_write_enable(SPI_x);

    mem_cs_disable; 


    tx[0] = W25Q_64KB_BLOCK_ERASE;
    tx[1] = (addr >> 16) & 0xFF;
    tx[2] = (addr >> 8) & 0xFF;
    tx[3] = addr & 0xFF;

    spi_transmit(SPI_x,tx, 4);

    mem_cs_enable; /// cs off like write disable

    w25q_wait_erase_end(SPI_x);
    w25q_wait_write_end(SPI_x);
    w25q_write_disable(SPI_x);
    w25q_set_block_protect(SPI_x,0x0F);
}


/**
 * @brief W25Q Chip erase
 * Func to erase all the data on chip
 *
 * @note Should be executed before writing
 * @param 
 * @return 
 */
void w25q_erase_chip(SPI_TypeDef *SPI_x)
{
    w25q_wait_write_end(SPI_x);
    w25q_set_block_protect(SPI_x,0x00);
    w25q_write_enable(SPI_x);
    mem_cs_disable; 
    uint8_t tx[1] = {W25Q_CHIP_ERASE};
    spi_transmit(SPI_x,tx, 1);
    mem_cs_enable; /// cs off like write disable
    w25q_wait_erase_end(SPI_x);
    w25q_wait_write_end(SPI_x);
    w25q_write_disable(SPI_x);
    w25q_set_block_protect(SPI_x,0x0F);
    delay_ms(500);// подождем пока не очистится память
}
   

/**
 * @}
 * @addtogroup W25Q_SUS Suspend functions
 * @brief Pause/resume operations
 * @{
 */

/**
 * @brief W25Q Programm/Erase Suspend
 * Pause programm or suspend operatiom
 *
 * @note SUS == 0 && BUSY == 1, otherwise ignored
 * @note Power loose during suspend state may corrupt data
 * @param none
 * @return W25Q_STATE enum
 */


/**
 * @brief W25Q Programm/Erase Resume
 * Resume suspended state
 *
 * @note SUS == 1, otherwise ignored
 * @param none
 * @return W25Q_STATE enum
 */


/**
 * @}
 * @addtogroup W25Q_Sleep Sleep functions
 * @brief Баю-бай, ток засыпай
 * @{
 */






/**
 * @brief W25Q Sleep / Power Down
 * Set chip to low-power state
 *
 * @note Use WakeUP or ReadID
 * @param none
 * @return 
 */

void w25q_sleep(SPI_TypeDef *SPI_x)
{
    uint8_t tx[1];
    tx[0]= W25Q_POWERDOWN;//enable reset
    mem_cs_disable; 
    spi_transmit (SPI_x, tx, 1);
    mem_cs_enable;

}

/**
 * @brief W25Q WakeUP
 * Wake UP function
 *
 * @param none
 * @return 
 */

void w25q_wakeup(SPI_TypeDef *SPI_x)
{
    uint8_t tx[1];
    tx[0]= W25Q_POWERUP;//enable reset
    mem_cs_disable; 
    spi_transmit (SPI_x, tx, 1);
    mem_cs_enable;

}

/**
 * @}
 * @addtogroup W25Q_ID ID functions
 * @brief Who am I? Хто я?
 * @{
 */

/**
 * @brief W25Q Read ID
 * Function for reading chip ID
 *
 * @param[out] 
 * @return Id 
 */



uint8_t w25q_read_id(SPI_TypeDef *SPI_x)
{
    uint8_t data[4];
    uint8_t tx[1];
    tx[0]=W25Q_DEVID; //get id
    mem_cs_disable; 
    spi_transmit (SPI_x, tx, 1);
    spi_receive  (SPI_x, data, 4)  ;

    mem_cs_enable;
    return data[3];
}


/**
 * @brief W25Q Read JEDEC ID
 * Read ID by JEDEC standards
 *
 * @attention Func in development
 * @param[out] 
 * @return u32 JEDEC ID
 */


uint32_t w25q_read_jedec_id(SPI_TypeDef *SPI_x)
{
    uint8_t data[4];
    uint8_t tx[1];
    tx[0]=W25Q_READ_JEDEC_ID; //get 
    mem_cs_disable; 
    spi_transmit (SPI_x, tx, 1);
    spi_receive  (SPI_x, data, 3)  ;

    mem_cs_enable;
    return (data[0] << 16) | (data[1] << 8) | data[2];
}


/**
 * @}
 * @addtogroup W25Q_Secure Security register functions
 * @brief Shhh, security
 * @{
 */

/**
 * @brief W25Q Erase Security Registers
 * Clean security registers (one or all)
 *
 * @attention Func in development
 * @param[in] numReg Number of security register (1..3 / 0-all)
 * @return W25Q_STATE enum
 */



/**
 * @brief W25Q Program Security Register
 * Write data to security reg
 *
 * @attention Func in development
 * @param[in] buf Pointer to 8-bit data bufer
 * @param[in] numReg Number of security register (1..3)
 * @param[in] byteAddr Byte addr in register (0..255)
 * @return W25Q_STATE enum
 */

/**
 * @brief W25Q Read Security Register
 * Read data from security reg
 *
 * @attention Func in development
 * @param[out] buf Pointer to 8-bit data bufer
 * @param[in] numReg Number of security register (1..3)
 * @param[in] byteAddr Byte addr in register (0..255)
 * @return W25Q_STATE enum
 */


/**
 * @}
 * @addtogroup W25Q_Protect Read-only protection functions
 * @brief No writing! Protect block or whole chip
 * @{
 */



/**
 * @brief W25Q Set Block Protect
 * 
 *
 * @attention 
 * @param[in] 
 * @param[in] 
 * @return
 */

void w25q_set_block_protect(SPI_TypeDef *SPI_x,uint8_t val) 
{
    uint8_t tx[2];
    tx[0] = W25Q_ENABLE_VOLATILE_SR;

    mem_cs_disable; 
    spi_transmit(SPI_x,tx, 1);
    mem_cs_enable;

    tx[0] = W25Q_WRITE_SR1;
    tx[1] = ((val & 0x0F) << 2);

    mem_cs_disable; 
    spi_transmit(SPI_x,tx, 2);
    mem_cs_enable;
}




/**
 * @}
 * @addtogroup W25Q_RST Reset functions
 * @brief Reboot the chip
 * @{
 */

/**
 * @brief W25Q Software Reset
 * Reset by register (not by external GPIO pin)
 *
 * @param[in] force Enable/disable (0/1) force reset - wait for BUSY and SUSpend
 * @return W25Q_STATE enum
 */

void w25q_reset(SPI_TypeDef *SPI_x)
{
    uint8_t tx[1];
    
    tx[0]= W25Q_ENABLE_RST;//enable reset
    mem_cs_disable; 
    spi_transmit (SPI_x, tx, 1);
    mem_cs_enable;
    //delay_ms(1);
    
    tx[0]= W25Q_RESET;//reset
    mem_cs_disable; 
    spi_transmit (SPI_x, tx, 1);
    mem_cs_enable;
}  


/// @}
// addgroup{
/// @}
// Public {

/**
 * @brief W25Q Wait write is end
 * 
 *
 * @param[in] 
 * @return 
 */
void w25q_wait_write_end(SPI_TypeDef *SPI_x)
{
    uint32_t timeout_counter=0;
   // delay_ms(1);
    mem_cs_disable; 
    uint8_t SR1[1];
    SR1[0] = W25Q_READ_SR1; //read SR1
    spi_transmit (SPI_x,  SR1, 1)  ;//wd
    
    timeout_counter = systick_get() + Timeout_SPI_tx_rx;
    do{
        spi_receive(SPI_x, SR1,1);
        W25Q_SR.SR1.Register = SR1[0];
      //  delay_ms(1);
        if (timeout_counter<systick_get()) {
        return;
        }
    }
    while(W25Q_SR.SR1.WEL  == 0x01); //wait untill WEL is done


    mem_cs_enable;
}

/**
 * @brief W25Q Wait erase is end
 *  // многие библиотеки игнорируют это важную вещь, надо дождаться завершения стирания, а не ставить задержки
 *
 * @param[in] 
 * @return 
 */
void w25q_wait_erase_end(SPI_TypeDef *SPI_x)
{
    uint32_t timeout_counter=0;
   // delay_ms(1);
    mem_cs_disable; 
    uint8_t SR1[1];
    SR1[0] = W25Q_READ_SR1; //read SR1
    spi_transmit (SPI_x,  SR1, 1)  ;//wd
    timeout_counter = systick_get() + Timeout_SPI_tx_rx;
    do{
        spi_receive(SPI_x, SR1,1);
        W25Q_SR.SR1.Register = SR1[0];
       // delay_ms(1);
        if (timeout_counter<systick_get()) {
        return;
        }
    }
    while(W25Q_SR.SR1.BUSY  == 0x01); //wait untill BUSY is done

    mem_cs_enable;
}


/**
 * @brief W25Q Write Enable
 *
 * @note 
 * @param[in] 
 * @return 
 */
void w25q_write_enable(SPI_TypeDef *SPI_x) 
{
    mem_cs_disable; 
    uint8_t tx[1] ={W25Q_WRITE_ENABLE};

    spi_transmit(SPI_x, tx,1);
    mem_cs_enable;
}

/**
 * @brief W25Q Write Disable
 *
 * 
 *
 * @attention 
 * @param[in] 
 * @return 
 */

void w25q_write_disable(SPI_TypeDef *SPI_x) 
{
    mem_cs_disable; 
    uint8_t tx[1] ={W25Q_WRITE_DISABLE};

    spi_transmit(SPI_x, tx,1);
    mem_cs_enable;
}


/**
 * @brief Page to address
 * Translate page to byte-address
 *
 * @param[in] pageNum Number of page
 * @param[in] pageShift Byte to shift inside page
 * @return byte-address
 */
uint32_t page_to_addr(uint32_t pageNum, uint8_t pageShift) 
{
    return pageNum * MEM_PAGE_SIZE + pageShift;
}

///@}
