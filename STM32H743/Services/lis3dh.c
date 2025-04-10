#include "lis3dh.h"
//#include "accel.h"
#include "stm32h7xx_spi.h"
#include "stm32h7xx_interrupts.h"
//#include "base.h"

/******************************************************************************/
/******************************* DEFINITIONS **********************************/

/******************************* variables **********************************/
lis3dh_values_t lis3dh_values;
lis3dh_registers_t lis3dh_registers;
int16_t acceleration_range;

bool lis3dh_data_ready=false;
/******************************* FUNCTIONS ************************************/

/******************************************************************************/
/****************************  SPI dependent func  **********************************/
/**
* @brief lis3dh_write_to_reg
*  Функция записи данных в определенный регистр 
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI ,uint8_t reg регистр ,uint8_t data данные 
* @return    lis3dh_status_t статус передачи
*/

lis3dh_status_t lis3dh_write_to_reg(SPI_TypeDef *SPI_x,uint8_t reg,uint8_t data)
{
    lis3dh_cs_disable;
    if((reg>0x00 && reg<=0x06) || reg ==0x0E || (reg>=0x10 && reg<=0x1d)) // защита от случайной записи по резервированным регистрам, такая запись может сломать датчик
    {
        return adr_err;
    }
    uint8_t data_tx[2];
    reg = reg | WRITE;
    data_tx[0]=reg;
    data_tx[1]=data;
    spi_transmit(SPI_x,data_tx,2);
    lis3dh_cs_enable;
    return compliete;
    
}


/**
* @brief lis3dh_read_from_reg
*  Функция чтения  данных по определенному регистру
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI ,uint8_t reg стартовый регистр,uint8_t *buff буфер для данных ,uint8_t size кол во байт
* @return lis3dh_status_t статус передачи 
*/

lis3dh_status_t lis3dh_read_from_reg(SPI_TypeDef *SPI_x,uint8_t reg,uint8_t *buff, uint8_t size)
{
    lis3dh_cs_disable;
    uint8_t data[1];
    if(size == 0x01)
    {
        reg= reg | READ;
    }
    else
    {
        reg= reg | READ | MULTI_READ ;
    }
    data[0]=reg;
    spi_transmit(SPI_x,data,1); 
    spi_receive(SPI_x,buff,size);
    
    lis3dh_cs_enable;
    
    return compliete;
}


/******************************************************************************/
/******************************************************************************/
/**
* @brief lis3dh_init
*  Инициплизпция акселерометра
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return 
*/

void lis3dh_init(SPI_TypeDef *SPI_x)
{
    uint8_t data=0x00 ;
    
    data = LIS3DHTR_REG_TEMP_ADC_PD_ENABLED | LIS3DHTR_REG_TEMP_TEMP_EN_ENABLED; 
    lis3dh_write_to_reg(SPI_x,LIS3DHTR_REG_TEMP_CFG,data);
    delay_ms(LIS3DHTR_CONVERSIONDELAY);    
   
    data = LIS3DHTR_REG_TEMP_ADC_PD_ENABLED | LIS3DHTR_REG_TEMP_TEMP_EN_ENABLED; 
    lis3dh_write_to_reg(SPI_x,LIS3DHTR_REG_TEMP_CFG,data);
    delay_ms(LIS3DHTR_CONVERSIONDELAY);
    
    data =  LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_NORMAL |  LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE |  LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE | LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE ;
    lis3dh_write_to_reg(SPI_x,LIS3DHTR_REG_ACCEL_CTRL_REG1,data); 
    delay_ms(LIS3DHTR_CONVERSIONDELAY);
    
    data = LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED | LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_LSB | LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_DISABLE | LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_NORMAL |  LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE;
    lis3dh_write_to_reg(SPI_x,LIS3DHTR_REG_ACCEL_CTRL_REG4,data); 
    
    delay_ms(LIS3DHTR_CONVERSIONDELAY);
    lis3dh_full_range_set(SPI_x,LIS3DHTR_RANGE_16G); 
    
    lis3dh_data_rate_set(SPI_x,LIS3DHTR_DATARATE_400HZ);
    
    lis3dh_who_i_am_get(SPI_x);
    lis3dh_regs_read(SPI_x);
}

/**
* @brief lis3dh_full_range_set
*  Установка разрения измерений 2,4,8,16g 
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI , scale_type_t range разрешение 
* @return 
*/
void lis3dh_full_range_set(SPI_TypeDef *SPI_x, scale_type_t range)
{
    uint8_t data[1];
    
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_CTRL_REG4,data,1);
    
    data[0] &= ~LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_MASK;
    data[0] |= range;
    delay_ms(LIS3DHTR_CONVERSIONDELAY);
    
    lis3dh_write_to_reg(SPI_x,LIS3DHTR_REG_ACCEL_CTRL_REG4,data[0] );
    delay_ms(LIS3DHTR_CONVERSIONDELAY);
    
    switch (range)
    {
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_16G:
        acceleration_range = 1280;
        break;
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_8G:
        acceleration_range = 3968;
        break;
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_4G:
        acceleration_range = 7282;
        break;
        case LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_2G:
        acceleration_range = 16000;
        break;
        default:
        break;
    }
}

/**
* @brief lis3dh_data_rate_set
*  Установка частоты дискретизации
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI ,odr_type_t odr дискретизация 
* @return 
*/
void lis3dh_data_rate_set(SPI_TypeDef *SPI_x,odr_type_t odr)
{
    uint8_t data[1];
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_CTRL_REG1,data ,1);
    data[0] &= ~LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_MASK;
    data[0] |= odr;
    delay_ms(LIS3DHTR_CONVERSIONDELAY);
    
    lis3dh_write_to_reg(SPI_x,LIS3DHTR_REG_ACCEL_CTRL_REG1,data[0] );
    delay_ms(LIS3DHTR_CONVERSIONDELAY);
}

/**
* @brief lis3dh_temperature_get
*  Получение температуры 
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return  
*/   

void lis3dh_temperature_get(SPI_TypeDef *SPI_x)
{  
    uint8_t buf[2];
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_OUT_ADC3_L,buf,2);
    int16_t temperature = (buf[0]|(buf[1]<<8)) / 256;
    temperature += 25;
    lis3dh_values.temperature=temperature;
    if(lis3dh_values.temperature ==25)
    {
        lis3dh_values.error_data=true;
    }
}
/**
* @brief lis3dh_axeleration_get
*  Получение ускорений,  
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return  
*/   

void lis3dh_axeleration_get(SPI_TypeDef *SPI_x)
{
    uint8_t buf[6];
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_OUT_X_L,buf,6);
    lis3dh_values.accel.x = (float)((int16_t)(buf[1]<<8 |buf[0])) / acceleration_range;
    lis3dh_values.accel.y = (float)((int16_t)(buf[3]<<8 |buf[2])) / acceleration_range; 
    lis3dh_values.accel.z = (float)((int16_t)(buf[5]<<8 |buf[4])) / acceleration_range;
    lis3dh_values.accel.total = lis3dh_values.accel.x+lis3dh_values.accel.y+lis3dh_values.accel.z;
    if(lis3dh_values.connection)
    {
        lis3dh_data_ready = true;
    }
    else
    {
        lis3dh_data_ready = false;
    }
}
/**_
* @brief lis3dh_adc_get
*  Получение данных с АЦП 
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return  
*/   
void lis3dh_adc_get(SPI_TypeDef *SPI_x)
{
    uint8_t buf[6];
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_OUT_ADC1_L,buf,6);
    lis3dh_values.adc.adc_1 = (float)((int16_t)(buf[1]<<8 |buf[0])) / adc_range;
    lis3dh_values.adc.adc_2 = (float)((int16_t)(buf[3]<<8 |buf[2])) / adc_range; 
    lis3dh_values.adc.adc_3 = (float)((int16_t)(buf[5]<<8 |buf[4])) / adc_range;
}
/**
* @brief lis3dh_who_i_am_get
*  Получение данных о идентефикаторе
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return  
*/   
void lis3dh_who_i_am_get(SPI_TypeDef *SPI_x)
{ 
    uint8_t buf[6];
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_WHO_AM_I,buf,1);
    lis3dh_values.who_i_am = buf[0];
    if(lis3dh_values.who_i_am ==0x33)
    {
        lis3dh_values.connection=true;
    }
}
/**
* @brief lis3dh_regs_read
*  Получение данных конфигурационных регистров и регистров статуса
*  
* 
* @param[in] SPI_TypeDef* SPI_x    - используемый SPI
* @return  
*/   
void lis3dh_regs_read(SPI_TypeDef *SPI_x)
{ 
    uint8_t buf[8];
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_STATUS,buf,1);
    lis3dh_registers.reg_status_aux.reg= buf[0];
    lis3dh_read_from_reg(SPI_x,LIS3DHTR_REG_ACCEL_CTRL_REG0,buf,8);
    lis3dh_registers.reg_0.reg= buf[0];
    lis3dh_registers.reg_temp.reg= buf[1];
    lis3dh_registers.reg_1.reg= buf[2];
    lis3dh_registers.reg_2.reg= buf[3];
    lis3dh_registers.reg_3.reg= buf[4];
    lis3dh_registers.reg_4.reg= buf[5];
    lis3dh_registers.reg_5.reg= buf[6];
    lis3dh_registers.reg_6.reg= buf[7];
}

/**
* @brief lis3d_is_data_ready
*  Проверка готовности данных
*  
* 
* @param[in] 
* @return  
*/  
bool lis3d_is_data_ready(void)
{
    if(lis3dh_data_ready)
    {
        lis3dh_data_ready = !lis3dh_data_ready;
        return  !lis3dh_data_ready;
    }
    else
    {
        return   lis3dh_data_ready;
    }
}
/**
* @brief lis3d_x_axis_get
*  функция возврата данных об ускорении по оси Х
*  
* 
* @param[in] 
* @return  float accel.x
*/  
float lis3d_x_axis_get( void )
{
    return lis3dh_values.accel.x;
}  
/**
* @brief lis3d_y_axis_get
*  функция возврата данных об ускорении по оси Y
*  
* 
* @param[in] 
* @return  float accel.y
*/  
float lis3d_y_axis_get( void )
{
    return lis3dh_values.accel.y;
}  
/**
* @brief lis3d_z_axis_get
*  функция возврата данных об ускорении по оси Z
*  
* 
* @param[in] 
* @return  float accel.z
*/  
float lis3d_z_axis_get( void )
{
    return lis3dh_values.accel.z;
}  
/**
* @}
*/






