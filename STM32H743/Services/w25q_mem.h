/**
 *******************************************
 * @file    w25q_mem.h
 * @author  Dmitriy Semenov / Crazy_Geeks
 * @version 0.1b
 * @date	12-August-2021
 * @brief   Header for W25Qxxx lib
 * @note 	https://github.com/Crazy-Geeks/STM32-W25Q-QSPI
 *******************************************
 *
 * @note https://ru.mouser.com/datasheet/2/949/w25q256jv_spi_revg_08032017-1489574.pdf
 * @note https://www.st.com/resource/en/application_note/DM00227538-.pdf
*/

#ifndef W25Q_QSPI_W25Q_MEM_H_
#define W25Q_QSPI_W25Q_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32g4xx_spi.h"
   
#include "stm32g4xx_it.h"
#include <stdint.h> 
#include <stdbool.h>
#include <string.h>	
    
    
     /*****************************************************************************/

    
#define mem_cs_enable     MEM_CS_PORT->BSRR = GPIO_BSRR_BS14
#define mem_cs_disable    MEM_CS_PORT->BSRR = GPIO_BSRR_BR14
    
   
         typedef struct
        {
            uint32_t mem_w25q_id;
            uint8_t mem_w25q_err;
        }mem_t;   
    
/**
 * @addtogroup W25Q_Driver
 * @brief W25Q QSPI Driver
 * @{
 */

/**
 * @defgroup W25Q_Param W25Q Chip's Parameters
 * @brief User's chip parameters
 * @{
 */
// YOUR CHIP'S SETTINGS
/// Mem size in MB-bit
#define MEM_FLASH_SIZE 256U 	// 256 MB-bit
/// Mem big block size in KB
#define MEM_BLOCK_SIZE 64U		// 64 KB: 256 pages
/// Mem small block size in KB
#define MEM_SBLOCK_SIZE 32U		// 32 KB: 128 pages
/// Mem sector size in KB
#define MEM_SECTOR_SIZE 4U		// 4 KB : 16 pages
/// Mem page size in bytes
#define MEM_PAGE_SIZE  256U		// 256 byte : 1 page
/// Blocks count
#define BLOCK_COUNT (MEM_FLASH_SIZE * 2) // 512 blocks
/// Sector count
#define SECTOR_COUNT (BLOCK_COUNT * 16)  // 8192 sectors
/// Pages count
#define PAGE_COUNT (SECTOR_COUNT * 16)	 // 131'072 pages

/**@}*/



/**
 * @struct W25Q_STATUS_REG
 * @brief  W25Q Status Registers
 * @TODO: Mem protected recognition
 *
 * Structure to check chip's status registers
 * @{
 */
  //-----------------SRs-----------------//
  ///----------------SR1----------------//
  typedef struct
  {
    union 
    {
      uint8_t Register;
      struct {
        
        bool BUSY       :1 ;  ///< Erase/Write in progress
        bool WEL        :1 ;  ///< Write enable latch (1 - write allowed)
        bool BP0        :1 ;  ///< 
        bool BP1        :1 ;  ///< 
        bool BP2        :1 ;  ///<  
        bool TB         :1 ;  ///< 
        bool SEC        :1 ;  ///< 
        bool SRP        :1 ;  ///< 
      };               
    }SR1;
    
    ///----------------SR2----------------//
    union 
    {
      uint8_t Register;
      struct {
        
        bool SRL        :1 ;  ///< 
        bool QE         :1 ;  ///< 
        bool RESERVED   :1 ;  ///< 
        bool LB1        :1 ;  ///< 
        bool LB2        :1 ;  ///<  
        bool LB3        :1 ;  ///< 
        bool CMP        :1 ;  ///< 
        bool SUS        :1 ;  ///< 
      };
    }SR2;

    ///----------------SR3----------------//
    union 
    {
      uint8_t Register;
      struct {

        bool  RESERV1  :1 ;  ///< 
        bool  RESERV2  :1 ;	 ///< 
        bool  WPS      :1 ;  ///< 
        bool  RESERV3  :1 ;  ///< 
        bool  RESERV4  :1 ;  ///<  
        bool  DRV2     :1 ;  ///< 
        bool  DRV1     :1 ;  ///< 
        bool  RESERV5  :1 ;  ///< 
      };
    }SR3;
  }W25Q_SRs_t;
  extern W25Q_SRs_t W25Q_SR;

   
   
/** @} */

    void w25q_init(SPI_TypeDef *SPI_x);
    void w25q_reset(SPI_TypeDef *SPI_x);
       
    void w25q_read_status_reg(SPI_TypeDef *SPI_x,uint8_t *reg_data, uint8_t reg_num); 
    void w25q_read_status_struct(SPI_TypeDef *SPI_x);	
        
    void w25q_read(SPI_TypeDef *SPI_x, uint32_t addr,uint8_t *data, uint16_t size);
       
    void w25q_write(SPI_TypeDef *SPI_x, uint32_t addr,uint8_t *data, uint16_t size);
    void w25q_write_page(SPI_TypeDef *SPI_x, uint32_t PageNum,uint32_t pageShift,uint8_t *data, uint32_t size);

    uint32_t w25q_read_jedec_id(SPI_TypeDef *SPI_x);
    uint8_t  w25q_read_id(SPI_TypeDef *SPI_x);

    void w25q_erase_chip(SPI_TypeDef *SPI_x);
    void w25q_erase_clock(SPI_TypeDef *SPI_x,uint32_t addr);
    void w25q_erase_sector(SPI_TypeDef *SPI_x, uint32_t addr) ;
        
    void w25q_disable_qspi(SPI_TypeDef *SPI_x);
    void w25q_set_block_protect(SPI_TypeDef *SPI_x,uint8_t val) ;
      
    void w25q_wait_write_end(SPI_TypeDef *SPI_x);
    void w25q_wait_erase_end(SPI_TypeDef *SPI_x);
    void w25q_write_enable(SPI_TypeDef *SPI_x) ;
    void w25q_write_disable(SPI_TypeDef *SPI_x) ;



  
/**
 * @defgroup W25Q_Commands W25Q Chip's Commands
 * @brief W25Q Chip commands from datasheet
 * @{
 */
#define W25Q_WRITE_ENABLE 0x06U			///< sets WEL bit, must be set before any write/program/erase
#define W25Q_WRITE_DISABLE 0x04U		///< resets WEL bit (state after power-up)
#define W25Q_ENABLE_VOLATILE_SR 0x50U	///< check 7.1 in datasheet
#define W25Q_READ_SR1 0x05U				///< read status-register 1
#define W25Q_READ_SR2 0x35U				///< read status-register 2
#define W25Q_READ_SR3 0x15U				///< read ststus-register 3
#define W25Q_WRITE_SR1 0x01U			///< write status-register 1 (8.2.5)
#define W25Q_WRITE_SR2 0x31U			///< write status-register 2 (8.2.5)
#define W25Q_WRITE_SR3 0x11U			///< write status-register 3 (8.2.5)
#define W25Q_READ_EXT_ADDR_REG 0xC8U	///< read extended addr reg (only in 3-byte mode)
#define W25Q_WRITE_EXT_ADDR_REG 0xC8U	///< write extended addr reg (only in 3-byte mode)
#define W25Q_ENABLE_4B_MODE 0xB7U			///< enable 4-byte mode (128+ MB address)
#define W25Q_DISABLE_4B_MODE 0xE9U			///< disable 4-byte mode (<=128MB)
#define W25Q_READ_DATA 0x03U				///< read data by standard SPI_x
#define W25Q_READ_DATA_4B 0x13U				///< read data by standard SPI_x in 4-byte mode
#define W25Q_FAST_READ 0x0BU				///< highest FR speed (8.2.12)
#define W25Q_FAST_READ_4B 0x0CU				///< fast read in 4-byte mode
#define W25Q_FAST_READ_DUAL_OUT 0x3BU		///< fast read in dual-SPI_x OUTPUT (8.2.14)
#define W25Q_FAST_READ_DUAL_OUT_4B 0x3CU	///< fast read in dual-SPI_x OUTPUT in 4-byte mode
#define W25Q_FAST_READ_QUAD_OUT 0x6BU		///< fast read in quad-SPI_x OUTPUT (8.2.16)
#define W25Q_FAST_READ_QUAD_OUT_4B 0x6CU	///< fast read in quad-SPI_x OUTPUT in 4-byte mode
#define W25Q_FAST_READ_DUAL_IO 0xBBU		///< fast read in dual-SPI_x I/O (address transmits by both lines)
#define W25Q_FAST_READ_DUAL_IO_4B 0xBCU		///< fast read in dual-SPI_x I/O in 4-byte mode
#define W25Q_FAST_READ_QUAD_IO 0xEBU		///< fast read in quad-SPI_x I/O (address transmits by quad lines)
#define W25Q_FAST_READ_QUAD_IO_4B 0xECU		///< fast read in quad-SPI_x I/O in 4-byte mode
#define W25Q_SET_BURST_WRAP 0x77U			///< use with quad-I/O (8.2.22)
#define W25Q_PAGE_PROGRAM 0x02U				///< program page (256bytes) by single SPI_x line
#define W25Q_PAGE_PROGRAM_4B 0x12U			///< program page by single SPI_x in 4-byte mode
#define W25Q_PAGE_PROGRAM_QUAD_INP 0x32U	///< program page (256bytes) by quad SPI_x lines
#define W25Q_PAGE_PROGRAM_QUAD_INP_4B 0x34U ///< program page by quad SPI_x in 4-byte mode
#define W25Q_SECTOR_ERASE 0x20U				///< sets all 4Kbyte sector with 0xFF (erases it)
#define W25Q_SECTOR_ERASE_4B 0x21U			///< sets all 4Kbyte sector with 0xFF in 4-byte mode
#define W25Q_32KB_BLOCK_ERASE 0x52U			///< sets all 32Kbyte block with 0xFF
#define W25Q_64KB_BLOCK_ERASE 0xD8U			///< sets all 64Kbyte block with 0xFF
#define W25Q_64KB_BLOCK_ERASE_4B 0xDCU		///< sets all 64Kbyte sector with 0xFF in 4-byte mode
#define W25Q_CHIP_ERASE 0xC7U				///< fill all the chip with 0xFF
//#define W25Q_CHIP_ERASE 0x60U				///< another way to erase chip
#define W25Q_ERASEPROG_SUSPEND 0x75U		///< suspend erase/program operation (can be applied only when SUS=0, BYSY=1)
#define W25Q_ERASEPROG_RESUME 0x7AU			///< resume erase/program operation (if SUS=1, BUSY=0)
#define W25Q_POWERDOWN 0xB9U				///< powers down the chip (power-up by reading ID)
#define W25Q_POWERUP 0xABU					///< release power-down
#define W25Q_DEVID 0xABU					///< read Device ID (same as powerup)
#define W25Q_FULLID 0x90U					///< read Manufacturer ID & Device ID
#define W25Q_FULLID_DUAL_IO 0x92U			///< read Manufacturer ID & Device ID by dual I/O
#define W25Q_FULLID_QUAD_IO 0x94U			///< read Manufacturer ID & Device ID by quad I/O
#define W25Q_READ_UID 0x4BU					///< read unique chip 64-bit ID
#define W25Q_READ_JEDEC_ID 0x9FU			///< read JEDEC-standard ID
#define W25Q_READ_SFDP 0x5AU				///< read SFDP register parameters
#define W25Q_ERASE_SECURITY_REG 0x44U		///< erase security registers
#define W25Q_PROG_SECURITY_REG 0x42U		///< program security registers
#define W25Q_READ_SECURITY_REG 0x48U		///< read security registers
#define W25Q_IND_BLOCK_LOCK 0x36U			///< make block/sector read-only
#define W25Q_IND_BLOCK_UNLOCK 0x39U			///< disable block/sector protection
#define W25Q_READ_BLOCK_LOCK 0x3DU			///< check block/sector protection
#define W25Q_GLOBAL_LOCK 0x7EU				///< global read-only protection enable
#define W25Q_GLOBAL_UNLOCK 0x98U			///< global read-only protection disable
#define W25Q_ENABLE_RST 0x66U				///< enable software-reset ability
#define W25Q_RESET 0x99U					///< make software reset
/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif /* W25Q_QSPI_W25Q_MEM_H_ */
