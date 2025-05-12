#ifndef VIRTUAL_COM_PORT_H
#define VIRTUAL_COM_PORT_H

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "stm32h7xx.h"
#include "gpio_config.h"
#include "stm32h7xx_interrupts.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
    
  //  void MX_USB_Device_Init(void);
    void usb_init(void);
    void USB_CDC_RxHandler(uint8_t*, uint32_t);
    void GOVNO_HALOVSKOE(void);
#ifdef __cplusplus
}
#endif

#endif /*VIRTUAL_COM_PORT_H */