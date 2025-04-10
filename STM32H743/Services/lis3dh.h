#ifndef LIS3DH_H
#define LIS3DH_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32h743xx.h"

#include "stm32h7xx.h"
#include "gpio_config.h"
#include <stdint.h> 
#include <stdbool.h>
#include <string.h>	
    /*****************************************************************************/
    /******************************* DEFINES *************************************/  
#define MULTI_READ    (1<<6)
#define READ          (1<<7)
#define WRITE         (0<<7)   
    
#define lis3dh_cs_enable   //  LIS3DH_SPI_CS_PORT->BSRR = GPIO_BSRR_BS14
#define lis3dh_cs_disable  //  LIS3DH_SPI_CS_PORT->BSRR = GPIO_BSRR_BR14
    
#define LIS3DHTR_CONVERSIONDELAY (100)    
    
#define adc_range 1200
    /**************************************************************************
    ACCELEROMETER REGISTERS
    **************************************************************************/
#define LIS3DHTR_REG_ACCEL_STATUS       0x07  // Status Register
#define LIS3DHTR_REG_ACCEL_OUT_ADC1_L   0x08  // ADC1 Low Register
#define LIS3DHTR_REG_ACCEL_OUT_ADC1_H   0x09  // ADC1 High Register
#define LIS3DHTR_REG_ACCEL_OUT_ADC2_L   0x0A  // ADC2 Low Register
#define LIS3DHTR_REG_ACCEL_OUT_ADC2_H   0x0B  // ADC2 High Register
#define LIS3DHTR_REG_ACCEL_OUT_ADC3_L   0x0C  // ADC3 Low Register
#define LIS3DHTR_REG_ACCEL_OUT_ADC3_H   0x0D  // ADC3 High Register
#define LIS3DHTR_REG_ACCEL_WHO_AM_I     0x0F  // Device Identification Register
#define LIS3DHTR_REG_ACCEL_CTRL_REG0    0x1E  // Control Register 0
#define LIS3DHTR_REG_TEMP_CFG           0x1F  // Temperature Sensor Register
#define LIS3DHTR_REG_ACCEL_CTRL_REG1    0x20  // Control Register 1
#define LIS3DHTR_REG_ACCEL_CTRL_REG2    0x21  // Control Register 2
#define LIS3DHTR_REG_ACCEL_CTRL_REG3    0x22  // Control Register 3
#define LIS3DHTR_REG_ACCEL_CTRL_REG4    0x23  // Control Register 4
#define LIS3DHTR_REG_ACCEL_CTRL_REG5    0x24  // Control Register 5
#define LIS3DHTR_REG_ACCEL_CTRL_REG6    0x25  // Control Register 6
#define LIS3DHTR_REG_ACCEL_REFERENCE    0x26  // Reference/Datacapture Register
#define LIS3DHTR_REG_ACCEL_STATUS2      0x27  // Status Register 2
#define LIS3DHTR_REG_ACCEL_OUT_X_L      0x28  // X-Axis Acceleration Data Low Register
#define LIS3DHTR_REG_ACCEL_OUT_X_H      0x29  // X-Axis Acceleration Data High Register
#define LIS3DHTR_REG_ACCEL_OUT_Y_L      0x2A  // Y-Axis Acceleration Data Low Register
#define LIS3DHTR_REG_ACCEL_OUT_Y_H      0x2B  // Y-Axis Acceleration Data High Register
#define LIS3DHTR_REG_ACCEL_OUT_Z_L      0x2C  // Z-Axis Acceleration Data Low Register
#define LIS3DHTR_REG_ACCEL_OUT_Z_H      0x2D  // Z-Axis Acceleration Data High Register
#define LIS3DHTR_REG_ACCEL_FIFO_CTRL    0x2E  // FIFO Control Register
#define LIS3DHTR_REG_ACCEL_FIFO_SRC     0x2F  // FIFO Source Register
#define LIS3DHTR_REG_ACCEL_INT1_CFG     0x30  // Interrupt Configuration Register
#define LIS3DHTR_REG_ACCEL_INT1_SRC     0x31  // Interrupt Source Register
#define LIS3DHTR_REG_ACCEL_INT1_THS     0x32  // Interrupt Threshold Register
#define LIS3DHTR_REG_ACCEL_INT1_DURATION 0x33 // Interrupt Duration Register
#define LIS3DHTR_REG_ACCEL_INT2_CFG     0x34  // Interrupt 2 Configuration Register
#define LIS3DHTR_REG_ACCEL_INT2_SRC     0x35  // Interrupt 2 Source Register
#define LIS3DHTR_REG_ACCEL_INT2_THS     0x36  // Interrupt 2 Threshold Register
#define LIS3DHTR_REG_ACCEL_INT2_DURATION 0x37 // Interrupt 2 Duration Register
#define LIS3DHTR_REG_ACCEL_CLICK_CFG    0x38  // Click Recognition Register
#define LIS3DHTR_REG_ACCEL_CLICK_SRC    0x39  // Click Source Register
#define LIS3DHTR_REG_ACCEL_CLICK_THS    0x3A  // Click Threshold Register
#define LIS3DHTR_REG_ACCEL_TIME_LIMIT   0x3B  // Click Time Limit Register
#define LIS3DHTR_REG_ACCEL_TIME_LATENCY 0x3C  // Click Time Latency Register
#define LIS3DHTR_REG_ACCEL_TIME_WINDOW  0x3D  // Click Time Window Register
#define LIS3DHTR_REG_ACCEL_ACT_THS      0x3E  // Activity Threshold Register
#define LIS3DHTR_REG_ACCEL_ACT_DUR      0x3F  // Activity Duration Register
    
    /**************************************************************************
    TEMPERATURE REGISTER DESCRIPTION
    **************************************************************************/
#define LIS3DHTR_REG_TEMP_ADC_PD_MASK (0x80)     // ADC Power Enable Status
#define LIS3DHTR_REG_TEMP_ADC_PD_DISABLED (0x00) // ADC Disabled
#define LIS3DHTR_REG_TEMP_ADC_PD_ENABLED (0x80)  // ADC Enabled
    
#define LIS3DHTR_REG_TEMP_TEMP_EN_MASK (0x40)     // Temperature Sensor (T) Enable Status
#define LIS3DHTR_REG_TEMP_TEMP_EN_DISABLED (0x00) // Temperature Sensor (T) Disabled
#define LIS3DHTR_REG_TEMP_TEMP_EN_ENABLED (0x40)  // Temperature Sensor (T) Enabled
    
    /**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
    **************************************************************************/
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_MASK (0xF0) // Acceleration Data Rate Selection
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_PD (0x00)   // Power-Down Mode
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1 (0x10)    // Normal / Low Power Mode (1 Hz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_10 (0x20)   // Normal / Low Power Mode (10 Hz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_25 (0x30)   // Normal / Low Power Mode (25 Hz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_50 (0x40)   // Normal / Low Power Mode (50 Hz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_100 (0x50)  // Normal / Low Power Mode (100 Hz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_200 (0x60)  // Normal / Low Power Mode (200 Hz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_400 (0x70)  // Normal / Low Power Mode (400 Hz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1_6K (0x80) // Low Power Mode (1.6 KHz)
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_5K (0x90)   // Normal (1.25 KHz) / Low Power Mode (5 KHz)
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_MASK (0x08)   // Low Power Mode Enable
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_NORMAL (0x00) // Normal Mode
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_LOW (0x08)    // Low Power Mode
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_MASK (0x04)    // Acceleration Z-Axis Enable
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_DISABLE (0x00) // Acceleration Z-Axis Disabled
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE (0x04)  // Acceleration Z-Axis Enabled
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_MASK (0x02)    // Acceleration Y-Axis Enable
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_DISABLE (0x00) // Acceleration Y-Axis Disabled
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE (0x02)  // Acceleration Y-Axis Enabled
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_MASK (0x01)    // Acceleration X-Axis Enable
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_DISABLE (0x00) // Acceleration X-Axis Disabled
#define LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE (0x01)  // Acceleration X-Axis Enabled
    
    
    /**************************************************************************
    ACCELEROMETER CONTROL REGISTER 2 DESCRIPTION
    **************************************************************************/
#define LIS3DHTR_CTRL_REG2_HPM_MASK (0xC0)       // HPM
#define LIS3DHTR_CTRL_REG2_HPM_NORMAL (0x00) // Normal mode
#define LIS3DHTR_CTRL_REG2_HPM_REF (0x40) // Reference signal for filtering
#define LIS3DHTR_CTRL_REG2_HPM_NORMAL1 (0x80) // Normal mode
#define LIS3DHTR_CTRL_REG2_HPM_AUTORESET (0xC0) // Autoreset on interrupt event
    
#define LIS3DHTR_CTRL_REG2_HPCF_MASK (0x30) // HPCF
#define LIS3DHTR_CTRL_REG2_HPCF_NONE (0x00) // NONE
    
#define LIS3DHTR_CTRL_REG2_FDS_MASK (0x08) // Filtered data selection
#define LIS3DHTR_CTRL_REG2_FDS_DISABLE (0x00) // filter disable
#define LIS3DHTR_CTRL_REG2_FDS_ENABLE (0x08) // filter enable
    
#define LIS3DHTR_CTRL_REG2_HPCLICK_MASK (0x04) // High-pass filter enabled for CLICK function.
#define LIS3DHTR_CTRL_REG2_HPCLICK_DISABLE (0x00) // filter disable
#define LIS3DHTR_CTRL_REG2_HPCLICK_ENABLE (0x04) // filter enable
    
#define LIS3DHTR_CTRL_REG2_HP_IA2_MASK (0x02) // High-pass filter enabled for AOI function on interrupt 2
#define LIS3DHTR_CTRL_REG2_HP_IA2_DISABLE (0x00) // filter disable
#define LIS3DHTR_CTRL_REG2_HP_IA2_ENABLE (0x02) 
    
#define LIS3DHTR_CTRL_REG2_HP_IA1_MASK (0x01) // High-pass filter enabled for AOI function on interrupt 1
#define LIS3DHTR_CTRL_REG2_HP_IA1_DISABLE (0x00) // filter disable
#define LIS3DHTR_CTRL_REG2_HP_IA1_ENABLE (0x01)  // filter enable
    
    
    /**************************************************************************
    ACCELEROMETER CONTROL REGISTER 3 DESCRIPTION
    **************************************************************************/
#define LIS3DHTR_CTRL_REG3_CLICK_MASK (0x80)       // Click interrupt on INT1
#define LIS3DHTR_CTRL_REG3_CLICK_DISABLE (0x00) // disable
#define LIS3DHTR_CTRL_REG3_CLICK_ENABLE (0x80)  // enable
    
#define LIS3DHTR_CTRL_REG3_IA1_MASK (0x40)       //  IA1 interrupt on INT1
#define LIS3DHTR_CTRL_REG3_IA1_DISABLE (0x00) // disable
#define LIS3DHTR_CTRL_REG3_IA1_ENABLE (0x40)  // enable
    
#define LIS3DHTR_CTRL_REG3_IA2_MASK (0x20)       //  IA2 interrupt on INT1
#define LIS3DHTR_CTRL_REG3_IA2_DISABLE (0x00) // disable
#define LIS3DHTR_CTRL_REG3_IA2_ENABLE (0x20)  // enable
    
#define LIS3DHTR_CTRL_REG3_ZXYDA_MASK (0x10)       // ZYXDA interrupt on INT1
#define LIS3DHTR_CTRL_REG3_ZXYDA_DISABLE (0x00) // disable
#define LIS3DHTR_CTRL_REG3_ZXYDA_ENABLE (0x10)  // enable
    
#define LIS3DHTR_CTRL_REG3_321DA_MASK (0x08)       //  321DA interrupt on INT1
#define LIS3DHTR_CTRL_REG3_321DA_DISABLE (0x00) // disable
#define LIS3DHTR_CTRL_REG3_321DA_ENABLE (0x08)  // enable
    
#define LIS3DHTR_CTRL_REG3_WTM_MASK (0x04)       //  FIFO watermark interrupt on INT1
#define LIS3DHTR_CTRL_REG3_WTM_DISABLE (0x00) // disable
#define LIS3DHTR_CTRL_REG3_WTM_ENABLE (0x04)  // enable
    
#define LIS3DHTR_CTRL_REG3_OVERRUN_MASK (0x02)       //  FIFO overrun interrupt on INT1
#define LIS3DHTR_CTRL_REG3_OVERRUN_DISABLE (0x00) // disable
#define LIS3DHTR_CTRL_REG3_OVERRUN_ENABLE (0x02)  // enable
    
    
    /**************************************************************************
    ACCELEROMETER CONTROL REGISTER 4 DESCRIPTION
    **************************************************************************/
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_MASK (0x80)       // Block Data Update
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS (0x00) // Continuous Update
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED (0x80) // Output Registers Not Updated until MSB and LSB Read
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_MASK (0x40) // Big/Little Endian Data Selection
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_LSB (0x00)  // Data LSB @ lower address
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_MSB (0x40)  // Data MSB @ lower address
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_MASK (0x30) // Full-Scale Selection
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_2G (0x00)   // +/- 2G
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_4G (0x10)   // +/- 4G
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_8G (0x20)   // +/- 8G
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_16G (0x30)  // +/- 16G
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_MASK (0x08)    // High Resolution Output Mode
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_DISABLE (0x00) // High Resolution Disable
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_ENABLE (0x08)  // High Resolution Enable
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_MASK (0x06)   // Self-Test Enable
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_NORMAL (0x00) // Normal Mode
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_0 (0x02)      // Self-Test 0
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_1 (0x04)      // Self-Test 1
    
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_MASK (0x01)  // SPI Serial Interface Mode Selection
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE (0x00) // 4-Wire Interface
#define LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_3WIRE (0x01) // 3-Wire Interface
    
#define LIS3DHTR_REG_ACCEL_STATUS2_UPDATE_MASK (0x08)   // Has New Data Flag Mask
    /******************************* Structs *************************************/   
    
    
    typedef enum  // power mode
    {
        POWER_MODE_NORMAL = LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_NORMAL,
        POWER_MODE_LOW = LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_LOW
    }power_type_t;
    
    typedef enum  // measurement rage
    {
        LIS3DHTR_RANGE_2G = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_2G,   //
        LIS3DHTR_RANGE_4G = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_4G,   //
        LIS3DHTR_RANGE_8G = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_8G,   //
        LIS3DHTR_RANGE_16G = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_16G, //
    }scale_type_t;
    
    typedef enum  // output data rate
    {
        LIS3DHTR_DATARATE_POWERDOWN = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_PD,
        LIS3DHTR_DATARATE_1HZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1,
        LIS3DHTR_DATARATE_10HZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_10,
        LIS3DHTR_DATARATE_25HZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_25,
        LIS3DHTR_DATARATE_50HZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_50,
        LIS3DHTR_DATARATE_100HZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_100,
        LIS3DHTR_DATARATE_200HZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_200,
        LIS3DHTR_DATARATE_400HZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_400,
        LIS3DHTR_DATARATE_1_6KH = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1_6K,
        LIS3DHTR_DATARATE_5KHZ = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_5K
    }odr_type_t;
    
    
    /**
    * @brief  lis3dh values .
    */    
    typedef struct
    {
        struct{
            float      x;      
            float      y;  
            float      z; 
            float      total;
        }accel;
        struct{
            float      adc_1;      
            float      adc_2;  
            float      adc_3;  
        }adc;
        bool  connection;
        bool  error_data;
        uint8_t who_i_am;
        int16_t    temperature;
    } lis3dh_values_t;

    /**
    * @brief  lis3dh R/W statuses .
    */     
    typedef enum
    {
        adr_err = 0,
        compliete = 1,
        err_2 = 2,
    }lis3dh_status_t;
    
    /**
    * @brief  lis3dh all reg .
    */    
    typedef struct
    { 
        union
        {
            uint8_t reg;
            struct
            {
                bool  da_1 :1;
                bool  da_2 :1;
                bool  da_3 :1;
                bool  da_321 :1;
                bool  or_1 :1;
                bool  or_2 :1;
                bool  or_3 :1;
                bool  or_321 :1;   
            };
        }reg_status_aux;
        union
        {
            uint8_t reg;
            struct
            {
                bool  res_1 :1;
                bool  res_2 :1;
                bool  res_3 :1;
                bool  res_4 :1;
                bool  res_5 :1;
                bool  res_6 :1;
                bool  res_7 :1;
                bool  sdo_pu_disc :1;   
            };
        }reg_0;
        union
        {
            uint8_t reg;
            struct
            {
                bool  res_1 :1;
                bool  res_2 :1;
                bool  res_3 :1;
                bool  res_4 :1;
                bool  res_5 :1;
                bool  res_6 :1;
                bool  temp_en :1;
                bool  adc_en :1;   
            };
        }reg_temp;
        union
        {
            uint8_t reg;
            struct
            {
                bool  x_en :1;
                bool  y_en :1;
                bool  z_en :1;
                bool  lp_en :1;
                bool  odr_0 :1;
                bool  odr_1 :1;
                bool  odr_2 :1;
                bool  odr_3 :1;            
            };
        }reg_1;
        union
        {
            uint8_t reg;
            struct
            {
                bool  hp_ia1 :1;
                bool  hp_ia2 :1;
                bool  hpclick :1;
                bool  fds :1;
                bool  hpcf1 :1;
                bool  hpcf2 :1;
                bool  hpm0 :1;
                bool  hpm1 :1;        
            };
        }reg_2;
        union
        {
            uint8_t reg;
            struct
            {
                bool  res_1 :1;
                bool  i1_ovr :1;
                bool  i1_wtm :1;
                bool  i1_321da :1;
                bool  i1_zyxda :1;
                bool  i1_ia2 :1;
                bool  i1_ia1 :1;
                bool  i1_click :1;              
            };
        }reg_3;
        union
        {
            uint8_t reg;
            struct
            {
                bool sim :1;
                bool st0 :1;
                bool st1 :1;
                bool hr :1;
                bool fs0:1;
                bool fs1 :1;
                bool ble :1;
                bool bdu :1;     
            };
        }reg_4;
        union
        {
            uint8_t reg;
            struct
            {
                bool d4d_in2:1;
                bool lir_int2:1;
                bool d4d_int1:1;
                bool lir_int1:1;
                bool res_5:1;
                bool res_6:1;
                bool fifo_en:1;
                bool boot:1; 
            };
        }reg_5;
        union
        {
            uint8_t reg;
            struct
            {
                bool res_1:1;
                bool int_pol:1;
                bool res_3:1;
                bool i2_act:1;
                bool i2_boot:1;
                bool i2_ia2:1;
                bool i2_ia1:1;
                bool i2_click:1; 
            };
        }reg_6;
    }lis3dh_registers_t;
    
    /******************************* Structs *************************************/   
    
    /******************************* FUNCTIONS *************************************/   

    
    lis3dh_status_t lis3dh_write_to_reg(SPI_TypeDef *SPI_x,uint8_t reg,uint8_t data);
    lis3dh_status_t lis3dh_read_from_reg(SPI_TypeDef *SPI_x,uint8_t reg,uint8_t *buff, uint8_t size);
    void lis3dh_regs_read(SPI_TypeDef *SPI_x);
    
    void lis3dh_init(SPI_TypeDef *SPI_x);
    void lis3dh_full_range_set(SPI_TypeDef *SPI_x, scale_type_t range);
    void lis3dh_data_rate_set(SPI_TypeDef *SPI_x,odr_type_t odr);
    
    void lis3dh_axeleration_get(SPI_TypeDef *SPI_x);
    void lis3dh_temperature_get(SPI_TypeDef *SPI_x);
    void lis3dh_who_i_am_get(SPI_TypeDef *SPI_x);    

    
    bool lis3d_is_data_ready(void);
    float lis3d_x_axis_get(void);
    float lis3d_y_axis_get(void);
    float lis3d_z_axis_get(void);
    
#ifdef __cplusplus
}
#endif

#endif /* LIS3DH */

/**********END OF FILE****/ 