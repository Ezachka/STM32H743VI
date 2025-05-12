
#include "virtual_com_port.h"


// спасибо https://deepbluembedded.com/stm32-usb-cdc-virtual-com-port-vcp-examples/



void usb_init(void)
{  
    
 //   USB->BCDR |= USB_BCDR_DPPU; // enable pull up to DP   
    HAL_Init();
   MX_USB_DEVICE_Init();
}

