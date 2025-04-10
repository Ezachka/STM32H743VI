
#include "stm32h7xx_usart.h"
#include "stm32h7xx_rcc.h"
#include "stm32h7xx_gpio.h"
#include "gpio_config.h"

void usart_gpio_init ( USART_TypeDef* USART_x );
uint32_t usart_clock_config ( USART_TypeDef* USART_x );


//@TODO не доделаны фунции USART6 UART7 UART8



ErrorStatus usart_init ( USART_TypeDef* USART_x, uint32_t baund_rate,usart_parity_mode_t parity,usart_swap_pin_t swap)
{
	float temp = 0.0f;
	uint32_t brr = 0x00;
	uint32_t bus_freq = 0x00;
    
	bus_freq=usart_clock_config (USART_x);
    if(bus_freq==ERROR)
    {
        return ERROR;
    }
    
    
	usart_gpio_init ( USART_x );
	
	
	/**** USART bound rate setting ****/
    if(USART_x == LPUART1){
        /*!< Fcl must range from 3 x baud rate to 4096 x baud rate*/
        /*!< if Fcl=16MHz -> 4800baud >= baund_rate <= 4000Kbaud  */
        /*!< if Fcl=170MHz -> 57600baud >= baund_rate   */
        temp = (float)bus_freq * 256.0f/ (float)baund_rate;
        brr  = (uint32_t) temp;
    }
    else{
        temp = (float)bus_freq / (float)baund_rate / 16.0f;
        brr  = ((uint32_t)temp<<4) | ((uint32_t)( (temp-(uint16_t)temp)*16) );
    }
    
	USART_x->BRR = brr;
    
	/**** USART register setting ****/
    USART_x->CR1 &= ~USART_CR1_FIFOEN;  //Fifo mode disable
    
	USART_x->CR1 &= ~USART_CR1_OVER8;	//Oversampling by 16
	USART_x->CR1 &= ~USART_CR1_M;		//Word length 8 bits
	USART_x->CR1 &= ~USART_CR1_WAKE;	//Wakeup method: Idle Line 
	USART_x->CR1 &= ~USART_CR1_TXEIE;	//TXE interrupt inhibited
	USART_x->CR1 &= ~USART_CR1_TCIE;	//Transmission complete interrupt inhibited
	USART_x->CR1 |=  USART_CR1_RXNEIE;	//RXNE interrupt enable
	USART_x->CR1 &= ~USART_CR1_IDLEIE;	//IDLE interrupt inhibited
	USART_x->CR1 |=  USART_CR1_TE;		//Transmitter enable
	USART_x->CR1 |=  USART_CR1_RE;		//Receiver enable
	USART_x->CR1 &= ~USART_CR1_UESM;	//USART disable in low-power mode
	
	USART_x->CR2 &= ~(USART_CR2_STOP);	// 1 Stop bit
    
    if(parity==none_parity)
    {
        USART_x->CR1 &= ~USART_CR1_PCE;		//Parity control disable
    }
    else // ODD or even
    {
        USART_x->CR1 |= (0x01<<USART_CR1_M_Pos);		//Word length 9 bits to one more even
        USART_x->CR1 |= USART_CR1_PCE;		            //Parity control enable
        USART_x->CR1 |= (parity<<USART_CR1_PS_Pos);		//Parity control enable
    }
    if(swap)
    {
        USART_x->CR2 |= USART_CR2_SWAP;      //swap TX-RX pins 
    }
    
	USART_x->CR1 |= USART_CR1_UE;		//USART enable
    
	if( USART_x == USART1 ){
		NVIC_EnableIRQ(USART1_IRQn);
        NVIC_SetPriority( USART1_IRQn, 2);
	}
	else if( USART_x == USART2 ){
		NVIC_EnableIRQ(USART2_IRQn);
	}
	else if( USART_x == USART3 ){
		NVIC_EnableIRQ(USART3_IRQn);
	}
	else if( USART_x == UART4 ){
		NVIC_EnableIRQ(UART4_IRQn);
	}
	else if( USART_x == UART5 ){
		NVIC_EnableIRQ(UART5_IRQn);
	}
    else if( USART_x == USART6 ){
		NVIC_EnableIRQ(USART6_IRQn);
	}
    else if( USART_x == UART7 ){
		NVIC_EnableIRQ(UART7_IRQn);
	}
    else if( USART_x == UART8 ){
		NVIC_EnableIRQ(UART8_IRQn);
	}
	else if( USART_x == LPUART1 ){
		NVIC_EnableIRQ(LPUART1_IRQn);
	}
    return SUCCESS;
}

void usart_transmit_byte ( USART_TypeDef* USART_x, void *data )
{
	uint8_t* data_tx;
	data_tx = data;
	
	while (!(USART_x->ISR & USART_ISR_TXE_TXFNF));
	USART_x->TDR = *data_tx;
}

void usart_transmit ( USART_TypeDef* USART_x, void *data_buff, uint16_t data_len )
{
	uint8_t* data_tx;
	data_tx = data_buff;
	
	for( uint16_t i=0; i<data_len; i++ ){
		while (!(USART_x->ISR & USART_ISR_TXE_TXFNF));
		USART_x->TDR = (uint8_t) data_tx[i];
	}
}

uint8_t usart_receive ( USART_TypeDef* USART_x )
{
	return (uint8_t)( USART_x->RDR );
}


volatile uint8_t 	usart1_tx_buf[ SCB_TX_BUFF_LEN ];
const uint8_t 	    usart1_tx_buf_size = SCB_TX_BUFF_LEN - 1;
volatile uint8_t 	usart1_tx_buf_len = 0;
uint8_t				usart1_tx_buf_p;

volatile uint8_t 	usart1_rx_buf[ SCB_RX_BUFF_LEN ];
const uint8_t 		usart1_rx_buf_size = SCB_RX_BUFF_LEN - 1;
volatile uint8_t 	usart1_rx_buf_len = 0;
uint8_t				usart1_rx_buf_p;

uint8_t	usart1_receive_byte_last;
uint8_t	usart1_read_byte_last;
uint8_t	usart1_error_count;    

void usart_1_irq_handler ( void )
{
    // byte received
    if( USART1->ISR & USART_ISR_RXNE_RXFNE )
    {
        usart1_receive_byte_last = (USART1->RDR) & 0xFF;
        if( usart1_rx_buf_len < usart1_rx_buf_size ) 
        {
            usart1_rx_buf[ usart1_rx_buf_len ] = usart1_receive_byte_last;
            usart1_rx_buf_len += 1;
        }
        //        return;
    }
    
    //byte transmited
    if( USART1->ISR & USART_ISR_TXE_TXFNF )
    {
        if( (usart1_tx_buf_len - usart1_tx_buf_p) > 0 )
        {
            USART1->TDR = usart1_tx_buf[ usart1_tx_buf_p ] & 0xFF;
            
            usart1_tx_buf_p += 1;
        }
        
        if( (usart1_tx_buf_len - usart1_tx_buf_p) == 0 )
        {
            usart1_tx_buf_len = 0;
            usart1_tx_buf_p = 0;
            
            USART1->CR1 &= ~USART_CR1_TXEIE; //stop writing operations
        }
        //        return;
    }
    
    //Errors check
    if ( !(USART1->ISR & (USART_ISR_ORE|USART_ICR_NECF|USART_ISR_FE|USART_ISR_PE|USART_ISR_UDR)) )
        return;
    
    //Overrun error detected
    if( USART1->ISR & USART_ISR_ORE ){
        USART1->ICR |= ( USART_ICR_ORECF );	
        usart1_error_count++;   
    }
    // Noise error in multibuffer communication detected
    if( USART1->ISR & USART_ISR_NE ){
        USART1->ICR |= ( USART_ICR_NECF );
        usart1_error_count++;
    }
    // Framing Error in multibuffer communication detected
    if( USART1->ISR & USART_ISR_FE ){
        USART1->ICR |= ( USART_ICR_FECF );
        usart1_error_count++;
    }
    // Parity error detected
    if( USART1->ISR & USART_ISR_PE ){  
        USART1->ICR |= ( USART_ICR_PECF );
        usart1_error_count++;
    }
    // SPI slave underrun error detected
    if(USART1->ISR & USART_ISR_UDR){
        USART1->ICR |= ( USART_ICR_UDRCF );	
        usart1_error_count++;
    }
}

volatile uint8_t 	usart2_tx_buf[ SCB_TX_BUFF_LEN ];
const uint8_t 	    usart2_tx_buf_size = SCB_TX_BUFF_LEN - 1;
volatile uint8_t 	usart2_tx_buf_len = 0;
uint8_t				usart2_tx_buf_p;

volatile uint8_t 	usart2_rx_buf[ SCB_RX_BUFF_LEN ];
const uint8_t 		usart2_rx_buf_size = SCB_RX_BUFF_LEN - 1;
volatile uint8_t 	usart2_rx_buf_len = 0;
uint8_t				usart2_rx_buf_p;

uint8_t	usart2_receive_byte_last;
uint8_t	usart2_read_byte_last;
uint8_t	usart2_error_count;    

void usart_2_irq_handler ( void )
{
    // byte received
    if( USART2->ISR & USART_ISR_RXNE_RXFNE )
    {
        usart2_receive_byte_last = (USART2->RDR) & 0xFF;
        if( usart2_rx_buf_len < usart2_rx_buf_size ) 
        {
            usart2_rx_buf[ usart2_rx_buf_len ] = usart2_receive_byte_last;
            usart2_rx_buf_len += 1;
        }
        //        return;
    }
    
    //byte transmited
    if( USART2->ISR & USART_ISR_TXE_TXFNF )
    {
        if( (usart2_tx_buf_len - usart2_tx_buf_p) > 0 )
        {
            USART2->TDR = usart2_tx_buf[ usart2_tx_buf_p ] & 0xFF;
            
            usart2_tx_buf_p += 1;
        }
        
        if( (usart2_tx_buf_len - usart2_tx_buf_p) == 0 )
        {
            usart2_tx_buf_len = 0;
            usart2_tx_buf_p = 0;
            
            USART2->CR1 &= ~USART_CR1_TXEIE; //stop writing operations
        }
        //        return;
    }
    
    //Errors check
    if ( !(USART2->ISR & (USART_ISR_ORE|USART_ICR_NECF|USART_ISR_FE|USART_ISR_PE|USART_ISR_UDR)) )
        return;
    
    //Overrun error detected
    if( USART2->ISR & USART_ISR_ORE ){
        USART2->ICR |= ( USART_ICR_ORECF );	
        usart2_error_count++;   
    }
    // Noise error in multibuffer communication detected
    if( USART2->ISR & USART_ISR_NE ){
        USART2->ICR |= ( USART_ICR_NECF );
        usart2_error_count++;
    }
    // Framing Error in multibuffer communication detected
    if( USART2->ISR & USART_ISR_FE ){
        USART2->ICR |= ( USART_ICR_FECF );
        usart2_error_count++;
    }
    // Parity error detected
    if( USART2->ISR & USART_ISR_PE ){
        USART2->ICR |= ( USART_ICR_PECF );
        usart2_error_count++;
    }
    // SPI slave underrun error detected
    if(USART2->ISR & USART_ISR_UDR){
        USART2->ICR |= ( USART_ICR_UDRCF );	
        usart2_error_count++;
    }
}

volatile uint8_t 	usart3_tx_buf[ SCB_TX_BUFF_LEN ];
const uint8_t 	    usart3_tx_buf_size = SCB_TX_BUFF_LEN - 1;
volatile uint8_t 	usart3_tx_buf_len = 0;
uint8_t				usart3_tx_buf_p;

volatile uint8_t 	usart3_rx_buf[ SCB_RX_BUFF_LEN ];
const uint8_t 		usart3_rx_buf_size = SCB_RX_BUFF_LEN - 1;
volatile uint8_t 	usart3_rx_buf_len = 0;
uint8_t				usart3_rx_buf_p;

uint8_t	usart3_receive_byte_last;
uint8_t	usart3_read_byte_last;
uint8_t	usart3_error_count;    

void usart_3_irq_handler ( void )
{
    // byte received
    if( USART3->ISR & USART_ISR_RXNE_RXFNE )
    {
        usart3_receive_byte_last = (USART3->RDR) & 0xFF;
        if( usart3_rx_buf_len < usart3_rx_buf_size ) 
        {
            usart3_rx_buf[ usart3_rx_buf_len ] = usart3_receive_byte_last;
            usart3_rx_buf_len += 1;
        }
        //        return;
    }
    
    //byte transmited
    if( USART3->ISR & USART_ISR_TXE_TXFNF )
    {
        if( (usart3_tx_buf_len - usart3_tx_buf_p) > 0 )
        {
            USART3->TDR = usart3_tx_buf[ usart3_tx_buf_p ] & 0xFF;
            
            usart3_tx_buf_p += 1;
        }
        
        if( (usart3_tx_buf_len - usart3_tx_buf_p) == 0 )
        {
            usart3_tx_buf_len = 0;
            usart3_tx_buf_p = 0;
            
            USART3->CR1 &= ~USART_CR1_TXEIE; //stop writing operations
        }
        //        return;
    }
    
    //Errors check
    if ( !(USART3->ISR & (USART_ISR_ORE|USART_ICR_NECF|USART_ISR_FE|USART_ISR_PE|USART_ISR_UDR)) )
        return;
    
    //Overrun error detected
    if( USART3->ISR & USART_ISR_ORE ){
        USART3->ICR |= ( USART_ICR_ORECF );	
        usart3_error_count++;   
    }
    // Noise error in multibuffer communication detected
    if( USART3->ISR & USART_ISR_NE ){
        USART3->ICR |= ( USART_ICR_NECF );
        usart3_error_count++;
    }
    // Framing Error in multibuffer communication detected
    if( USART3->ISR & USART_ISR_FE ){
        USART3->ICR |= ( USART_ICR_FECF );
        usart3_error_count++;
    }
    // Parity error detected
    if( USART3->ISR & USART_ISR_PE ){
        USART3->ICR |= ( USART_ICR_PECF );
        usart3_error_count++;
    }
    // SPI slave underrun error detected
    if(USART3->ISR & USART_ISR_UDR){
        USART3->ICR |= ( USART_ICR_UDRCF );	
        usart3_error_count++;
    }
}

volatile uint8_t 	uart4_tx_buf[ SCB_TX_BUFF_LEN ];
const uint8_t 	    uart4_tx_buf_size = SCB_TX_BUFF_LEN - 1;
volatile uint8_t 	uart4_tx_buf_len = 0;
uint8_t				uart4_tx_buf_p;

volatile uint8_t 	uart4_rx_buf[ SCB_RX_BUFF_LEN ];
const uint8_t 		uart4_rx_buf_size = SCB_RX_BUFF_LEN - 1;
volatile uint8_t 	uart4_rx_buf_len = 0;
uint8_t				uart4_rx_buf_p;

uint8_t	uart4_receive_byte_last;
uint8_t	uart4_read_byte_last;
uint8_t	uart4_error_count;    

void uart_4_irq_handler ( void )
{
    // byte received
    if( UART4->ISR & USART_ISR_RXNE_RXFNE )
    {
        uart4_receive_byte_last = (UART4->RDR) & 0xFF;
        if( uart4_rx_buf_len < uart4_rx_buf_size ) 
        {
            uart4_rx_buf[ uart4_rx_buf_len ] = uart4_receive_byte_last;
            uart4_rx_buf_len += 1;
        }
        //        return;
    }
    
    //byte transmited
    if( UART4->ISR & USART_ISR_TXE_TXFNF )
    {
        if( (uart4_tx_buf_len - uart4_tx_buf_p) > 0 )
        {
            UART4->TDR = uart4_tx_buf[ uart4_tx_buf_p ] & 0xFF;
            
            uart4_tx_buf_p += 1;
        }
        
        if( (uart4_tx_buf_len - uart4_tx_buf_p) == 0 )
        {
            uart4_tx_buf_len = 0;
            uart4_tx_buf_p = 0;
            
            UART4->CR1 &= ~USART_CR1_TXEIE; //stop writing operations
        }
        //        return;
    }
    
    //Errors check
    if ( !(UART4->ISR & (USART_ISR_ORE|USART_ICR_NECF|USART_ISR_FE|USART_ISR_PE|USART_ISR_UDR)) )
        return;
    
    //Overrun error detected
    if( UART4->ISR & USART_ISR_ORE ){
        UART4->ICR |= ( USART_ICR_ORECF );	
        uart4_error_count++;   
    }
    // Noise error in multibuffer communication detected
    if( UART4->ISR & USART_ISR_NE ){
        UART4->ICR |= ( USART_ICR_NECF );
        uart4_error_count++;
    }
    // Framing Error in multibuffer communication detected
    if( UART4->ISR & USART_ISR_FE ){
        UART4->ICR |= ( USART_ICR_FECF );
        uart4_error_count++;
    }
    // Parity error detected
    if( UART4->ISR & USART_ISR_PE ){
        UART4->ICR |= ( USART_ICR_PECF );
        uart4_error_count++;
    }
    // SPI slave underrun error detected
    if(UART4->ISR & USART_ISR_UDR){
        UART4->ICR |= ( USART_ICR_UDRCF );	
        uart4_error_count++;
    }
}

volatile uint8_t 	uart5_tx_buf[ SCB_TX_BUFF_LEN ];
const uint8_t 	    uart5_tx_buf_size = SCB_TX_BUFF_LEN - 1;
volatile uint8_t 	uart5_tx_buf_len = 0;
uint8_t				uart5_tx_buf_p;

volatile uint8_t 	uart5_rx_buf[ SCB_RX_BUFF_LEN ];
const uint8_t 		uart5_rx_buf_size = SCB_RX_BUFF_LEN - 1;
volatile uint8_t 	uart5_rx_buf_len = 0;
uint8_t				uart5_rx_buf_p;

uint8_t	uart5_receive_byte_last;
uint8_t	uart5_read_byte_last;
uint8_t	uart5_error_count;    

void uart_5_irq_handler ( void )
{
    // byte received
    if( UART5->ISR & USART_ISR_RXNE_RXFNE )
    {
        uart5_receive_byte_last = (UART5->RDR) & 0xFF;
        if( uart5_rx_buf_len < uart5_rx_buf_size ) 
        {
            uart5_rx_buf[ uart5_rx_buf_len ] = uart5_receive_byte_last;
            uart5_rx_buf_len += 1;
        }
        //        return;
    }
    
    //byte transmited
    if( UART5->ISR & USART_ISR_TXE_TXFNF )
    {
        if( (uart5_tx_buf_len - uart5_tx_buf_p) > 0 )
        {
            UART5->TDR = uart5_tx_buf[ uart5_tx_buf_p ] & 0xFF;
            
            uart5_tx_buf_p += 1;
        }
        
        if( (uart5_tx_buf_len - uart5_tx_buf_p) == 0 )
        {
            uart5_tx_buf_len = 0;
            uart5_tx_buf_p = 0;
            
            UART5->CR1 &= ~USART_CR1_TXEIE; //stop writing operations
        }
        //        return;
    }
    
    //Errors check
    if ( !(UART5->ISR & (USART_ISR_ORE|USART_ICR_NECF|USART_ISR_FE|USART_ISR_PE|USART_ISR_UDR)) )
        return;
    
    //Overrun error detected
    if( UART5->ISR & USART_ISR_ORE ){
        UART5->ICR |= ( USART_ICR_ORECF );	
        uart5_error_count++;   
    }
    // Noise error in multibuffer communication detected
    if( UART5->ISR & USART_ISR_NE ){
        UART5->ICR |= ( USART_ICR_NECF );
        uart5_error_count++;
    }
    // Framing Error in multibuffer communication detected
    if( UART5->ISR & USART_ISR_FE ){
        UART5->ICR |= ( USART_ICR_FECF );
        uart5_error_count++;
    }
    // Parity error detected
    if( UART5->ISR & USART_ISR_PE ){
        UART5->ICR |= ( USART_ICR_PECF );
        uart5_error_count++;
    }
    // SPI slave underrun error detected
    if(UART5->ISR & USART_ISR_UDR){
        UART5->ICR |= ( USART_ICR_UDRCF );	
        uart5_error_count++;
    }
}

volatile uint8_t 	lpuart1_tx_buf[ SCB_TX_BUFF_LEN ];
const uint8_t 	    lpuart1_tx_buf_size = SCB_TX_BUFF_LEN - 1;
volatile uint8_t 	lpuart1_tx_buf_len = 0;
uint8_t				lpuart1_tx_buf_p;

volatile uint8_t 	lpuart1_rx_buf[ SCB_RX_BUFF_LEN ];
const uint8_t 		lpuart1_rx_buf_size = SCB_RX_BUFF_LEN - 1;
volatile uint8_t 	lpuart1_rx_buf_len = 0;
uint8_t				lpuart1_rx_buf_p;

uint8_t	lpuart1_receive_byte_last;
uint8_t	lpuart1_read_byte_last;
uint8_t	lpuart1_error_count;    

void lpuart_1_irq_handler ( void )
{
    // byte received
    if( LPUART1->ISR & USART_ISR_RXNE_RXFNE )
    {
        lpuart1_receive_byte_last = (LPUART1->RDR) & 0xFF;
        if( lpuart1_rx_buf_len < lpuart1_rx_buf_size ) 
        {
            lpuart1_rx_buf[ lpuart1_rx_buf_len ] = lpuart1_receive_byte_last;
            lpuart1_rx_buf_len += 1;
        }
        //        return;
    }
    
    //byte transmited
    if( LPUART1->ISR & USART_ISR_TXE_TXFNF )
    {
        if( (lpuart1_tx_buf_len - lpuart1_tx_buf_p) > 0 )
        {
            LPUART1->TDR = lpuart1_tx_buf[ lpuart1_tx_buf_p ] & 0xFF;
            
            lpuart1_tx_buf_p += 1;
        }
        
        if( (lpuart1_tx_buf_len - lpuart1_tx_buf_p) == 0 )
        {
            lpuart1_tx_buf_len = 0;
            lpuart1_tx_buf_p = 0;
            
            LPUART1->CR1 &= ~USART_CR1_TXEIE; //stop writing operations
        }
        //        return;
    }
    
    //Errors check
    if ( !(LPUART1->ISR & (USART_ISR_ORE|USART_ICR_NECF|USART_ISR_FE|USART_ISR_PE|USART_ISR_UDR)) )
        return;
    
    //Overrun error detected
    if( LPUART1->ISR & USART_ISR_ORE ){
        LPUART1->ICR |= ( USART_ICR_ORECF );	
        lpuart1_error_count++;   
    }
    // Noise error in multibuffer communication detected
    if( LPUART1->ISR & USART_ISR_NE ){
        LPUART1->ICR |= ( USART_ICR_NECF );
        lpuart1_error_count++;
    }
    // Framing Error in multibuffer communication detected
    if( LPUART1->ISR & USART_ISR_FE ){
        LPUART1->ICR |= ( USART_ICR_FECF );
        lpuart1_error_count++;
    }
    // Parity error detected
    if( LPUART1->ISR & USART_ISR_PE ){
        LPUART1->ICR |= ( USART_ICR_PECF );
        lpuart1_error_count++;
    }
    // SPI slave underrun error detected
    if(LPUART1->ISR & USART_ISR_UDR){
        LPUART1->ICR |= ( USART_ICR_UDRCF );	
        lpuart1_error_count++;
    }
}

uint8_t uart_read_available( USART_TypeDef* USART_x )
{
    if( USART_x == USART1 ){
		return usart1_rx_buf_len - usart1_rx_buf_p;
	}
	else if( USART_x == USART2 ){
		return usart2_rx_buf_len - usart2_rx_buf_p;
	}
	else if( USART_x == USART3 ){
		return usart3_rx_buf_len - usart3_rx_buf_p;
	}
	else if( USART_x == UART4 ){
		return uart4_rx_buf_len - uart4_rx_buf_p;
	}
	else if( USART_x == UART5 ){
		return uart5_rx_buf_len - uart5_rx_buf_p;
	}
	else if( USART_x == LPUART1 ){
		return lpuart1_rx_buf_len - lpuart1_rx_buf_p;
	}  
    return 0;   
}

uint8_t usart_read_byte( USART_TypeDef* USART_x )
{
    if( USART_x == USART1 ){
        if( usart1_rx_buf_len > 0 ){
            usart1_read_byte_last = usart1_rx_buf[ usart1_rx_buf_p ];
            usart1_rx_buf_p += 1;
            
            if( usart1_rx_buf_p == usart1_rx_buf_len ){
                usart1_rx_buf_len = 0;
                usart1_rx_buf_p = 0;
            }
            
            return usart1_read_byte_last;
        }
	}
	else if( USART_x == USART2 ){
        if( usart2_rx_buf_len > 0 ){
            usart2_read_byte_last = usart2_rx_buf[ usart2_rx_buf_p ];
            usart2_rx_buf_p += 1;
            
            if( usart2_rx_buf_p == usart2_rx_buf_len ){
                usart2_rx_buf_len = 0;
                usart2_rx_buf_p = 0;
            }
            
            return usart2_read_byte_last;
        }
	}
	else if( USART_x == USART3 ){
        if( usart3_rx_buf_len > 0 ){
            usart3_read_byte_last = usart3_rx_buf[ usart3_rx_buf_p ];
            usart3_rx_buf_p += 1;
            
            if( usart3_rx_buf_p == usart3_rx_buf_len ){
                usart3_rx_buf_len = 0;
                usart3_rx_buf_p = 0;
            }
            
            return usart3_read_byte_last;
        }
	}
	else if( USART_x == UART4 ){
        if( uart4_rx_buf_len > 0 ){
            uart4_read_byte_last = uart4_rx_buf[ uart4_rx_buf_p ];
            uart4_rx_buf_p += 1;
            
            if( uart4_rx_buf_p == uart4_rx_buf_len ){
                uart4_rx_buf_len = 0;
                uart4_rx_buf_p = 0;
            }
            
            return uart4_read_byte_last;
        }
	}
	else if( USART_x == UART5 ){
        if( uart5_rx_buf_len > 0 ){
            uart5_read_byte_last = uart5_rx_buf[ uart5_rx_buf_p ];
            uart5_rx_buf_p += 1;
            
            if( uart5_rx_buf_p == uart5_rx_buf_len ){
                uart5_rx_buf_len = 0;
                uart5_rx_buf_p = 0;
            }
            
            return uart5_read_byte_last;
        }
	}
	else if( USART_x == LPUART1 ){
        if( lpuart1_rx_buf_len > 0 ){
            lpuart1_read_byte_last = lpuart1_rx_buf[ lpuart1_rx_buf_p ];
            lpuart1_rx_buf_p += 1;
            
            if( lpuart1_rx_buf_p == lpuart1_rx_buf_len ){
                lpuart1_rx_buf_len = 0;
                lpuart1_rx_buf_p = 0;
            }
            
            return lpuart1_read_byte_last;
        }
        
	}  
    return 0;
}

void usart_send_byte( USART_TypeDef* USART_x, uint8_t byte )
{
    if( USART_x == USART1 ){
        if( usart1_tx_buf_len < usart1_tx_buf_size ){
            usart1_tx_buf[ usart1_tx_buf_len ] = byte;
            usart1_tx_buf_len += 1;
            
            USART1->CR1 |= USART_CR1_TXEIE;
        }	
	}
	else if( USART_x == USART2 ){
        if( usart2_tx_buf_len < usart2_tx_buf_size ){
            usart2_tx_buf[ usart2_tx_buf_len ] = byte;
            usart2_tx_buf_len += 1;
            
            USART2->CR1 |= USART_CR1_TXEIE;
        }			
	}
	else if( USART_x == USART3 ){
        if( usart3_tx_buf_len < usart3_tx_buf_size ){
            usart3_tx_buf[ usart3_tx_buf_len ] = byte;
            usart3_tx_buf_len += 1;
            
            USART3->CR1 |= USART_CR1_TXEIE;
        }			
	}
	else if( USART_x == UART4 ){
        if( uart4_tx_buf_len < uart4_tx_buf_size ){
            uart4_tx_buf[ uart4_tx_buf_len ] = byte;
            uart4_tx_buf_len += 1;
            
            UART4->CR1 |= USART_CR1_TXEIE;
        }	
	}
	else if( USART_x == UART5 ){
        if( uart5_tx_buf_len < uart5_tx_buf_size ){
            uart5_tx_buf[ uart5_tx_buf_len ] = byte;
            uart5_tx_buf_len += 1;
            
            UART5->CR1 |= USART_CR1_TXEIE;
        }	
	}
	else if( USART_x == LPUART1 ){
        if( lpuart1_tx_buf_len < lpuart1_tx_buf_size ){
            lpuart1_tx_buf[ lpuart1_tx_buf_len ] = byte;
            lpuart1_tx_buf_len += 1;
            
            LPUART1->CR1 |= USART_CR1_TXEIE;
        }	
	}       
}

void usart_send_data( USART_TypeDef* USART_x, uint8_t* data, uint8_t size )
{
    if( USART_x == USART1 ){
        for( uint8_t i = 0; i < size; i++ ){
            if( usart1_tx_buf_len < usart1_tx_buf_size ){
                usart1_tx_buf[ usart1_tx_buf_len ] = data[i];
                usart1_tx_buf_len += 1;
            }
        }
        if( usart1_tx_buf_len>0 ){
            USART1->CR1 |= USART_CR1_TXEIE;
        }
	}
	else if( USART_x == USART2 ){
        for( uint8_t i = 0; i < size; i++ ){
            if( usart2_tx_buf_len < usart2_tx_buf_size ){
                usart2_tx_buf[ usart2_tx_buf_len ] = data[i];
                usart2_tx_buf_len += 1;
            }
        }
        if( usart2_tx_buf_len>0 ){
            USART2->CR1 |= USART_CR1_TXEIE;
        }
	}
	else if( USART_x == USART3 ){
        for( uint8_t i = 0; i < size; i++ ){
            if( usart3_tx_buf_len < usart3_tx_buf_size ){
                usart3_tx_buf[ usart3_tx_buf_len ] = data[i];
                usart3_tx_buf_len += 1;
            }
        }
        if( usart3_tx_buf_len>0 ){
            USART3->CR1 |= USART_CR1_TXEIE;
        }
	}
	else if( USART_x == UART4 ){
        for( uint8_t i = 0; i < size; i++ ){
            if( uart4_tx_buf_len < uart4_tx_buf_size ){
                uart4_tx_buf[ uart4_tx_buf_len ] = data[i];
                uart4_tx_buf_len += 1;
            }
        }
        if( uart4_tx_buf_len>0 ){
            UART4->CR1 |= USART_CR1_TXEIE;
        }
	}
	else if( USART_x == UART5 ){
        for( uint8_t i = 0; i < size; i++ ){
            if( uart5_tx_buf_len < uart5_tx_buf_size ){
                uart5_tx_buf[ uart5_tx_buf_len ] = data[i];
                uart5_tx_buf_len += 1;
            }
        }
        if( uart5_tx_buf_len>0 ){
            UART5->CR1 |= USART_CR1_TXEIE;
        }
	}
	else if( USART_x == LPUART1 ){
        for( uint8_t i = 0; i < size; i++ ){
            if( lpuart1_tx_buf_len < lpuart1_tx_buf_size ){
                lpuart1_tx_buf[ lpuart1_tx_buf_len ] = data[i];
                lpuart1_tx_buf_len += 1;
            }
        }
        if( lpuart1_tx_buf_len>0 ){
            LPUART1->CR1 |= USART_CR1_TXEIE;
        }
	}    
}

void usart_clear_tx_buf(  USART_TypeDef* USART_x )
{
    if( USART_x == USART1 ){
        usart1_tx_buf_len = 0;
        usart1_tx_buf_p = 0;
	}
	else if( USART_x == USART2 ){
        usart2_tx_buf_len = 0;
        usart2_tx_buf_p = 0;
	}
	else if( USART_x == USART3 ){
        usart3_tx_buf_len = 0;
        usart3_tx_buf_p = 0;
	}
	else if( USART_x == UART4 ){
        uart4_tx_buf_len = 0;
        uart4_tx_buf_p = 0;
	}
	else if( USART_x == UART5 ){
        uart5_tx_buf_len = 0;
        uart5_tx_buf_p = 0;
	}
	else if( USART_x == LPUART1 ){
        lpuart1_tx_buf_len = 0;
        lpuart1_tx_buf_p = 0;
	}      
}

void usart_clear_rx_buf( USART_TypeDef* USART_x )
{
    if( USART_x == USART1 ){
        usart1_rx_buf_len = 0;
        usart1_rx_buf_p = 0;
	}
	else if( USART_x == USART2 ){
        usart2_rx_buf_len = 0;
        usart2_rx_buf_p = 0;
	}
	else if( USART_x == USART3 ){
        usart3_rx_buf_len = 0;
        usart3_rx_buf_p = 0;
	}
	else if( USART_x == UART4 ){
        uart4_rx_buf_len = 0;
        uart4_rx_buf_p = 0;
	}
	else if( USART_x == UART5 ){
        uart5_rx_buf_len = 0;
        uart5_rx_buf_p = 0;
	}
	else if( USART_x == LPUART1 ){
        lpuart1_rx_buf_len = 0;
        lpuart1_rx_buf_p = 0;
	}     
}

void usart_gpio_init ( USART_TypeDef* USART_x )
{
    /* GPIO configuration struct */
    gpio_configuration_t gpio_config;
  	/**** USART gpio setting ****/
	if( USART_x == USART1 ){
        //USART_1_TX PB6 
        gpio_config.port     = USART_1_TX_PORT;
        gpio_config.pin      = USART_1_TX_PIN;
        gpio_config.mode     = gpio_af;
        gpio_config.speed    = very_high_speed;
        gpio_config.otype    = gpio_otype_pp;
        gpio_config.pupd     = gpio_pupd_up;
        gpio_configuration ( &gpio_config );
        USART_1_TX_PORT->AFR[0] |= (0x07 << GPIO_AFRL_AFSEL6_Pos); //AF = 7 (USART_1_TX)
        
        //USART_1_RX PB7 
        gpio_config.port     = USART_1_RX_PORT;
        gpio_config.pin      = USART_1_RX_PIN;
        gpio_config.mode     = gpio_af;
        gpio_config.speed    = very_high_speed;
        gpio_config.otype    = gpio_otype_pp;
        gpio_config.pupd     = gpio_pupd_nopull;
        gpio_configuration ( &gpio_config );
        USART_1_RX_PORT->AFR[0] |= (0x07 << GPIO_AFRL_AFSEL7_Pos); //AF = 7 (USART_1_RX)    
	}
	else if( USART_x == USART2 ){
        //        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
        //	  
        //	  	//PA2 - TX
        //		GPIOA->MODER 	&= ~(GPIO_MODER_MODER2);		//MODER clear
        //		GPIOA->MODER 	|=  (GPIO_MODER_MODER2_1);		//AF
        //		GPIOA->OTYPER 	&= ~(GPIO_OTYPER_OT_2);			//PushPull
        //		GPIOA->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR2);	//MAX_speed
        //		GPIOA->PUPDR 	&= ~(GPIO_PUPDR_PUPDR2);		//PUPDR clear
        //		GPIOA->PUPDR 	|=  (GPIO_PUPDR_PUPDR2_0);		//Pull Up 
        //        GPIOA->AFR[0] 	&= ~(GPIO_AFRL_AFSEL2_Msk);     //AF clear
        //		GPIOA->AFR[0] 	|=  (GPIO_AFRL_AFSEL2_2|GPIO_AFRL_AFSEL2_1|GPIO_AFRL_AFSEL2_0); //AF = 7 (USART2)
        //        
        //		//PA3 - RX
        //		GPIOA->MODER 	&= ~(GPIO_MODER_MODER3);		//MODER clear
        //		GPIOA->MODER 	|=  (GPIO_MODER_MODER3_1);		//AF
        //		GPIOA->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR3);	//MAX_speed
        //		GPIOA->PUPDR 	&= ~(GPIO_PUPDR_PUPDR3);		//PUPDR clear
        //        GPIOA->AFR[0] 	&= ~(GPIO_AFRL_AFSEL3_Msk);     //AF clear
        //        GPIOA->AFR[0] 	|=  (GPIO_AFRL_AFSEL3_2|GPIO_AFRL_AFSEL3_1|GPIO_AFRL_AFSEL3_0); //AF = 7 (USART2)
	}
	else if( USART_x == USART3 ){
        //        RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
        //	  
        //	  	//PD8 - TX
        //		GPIOD->MODER 	&= ~(GPIO_MODER_MODER8);		//MODER clear
        //		GPIOD->MODER 	|=  (GPIO_MODER_MODER8_1);		//AF
        //		GPIOD->OTYPER 	&= ~(GPIO_OTYPER_OT_8);			//PushPull
        //		GPIOD->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR8);	//MAX_speed
        //		GPIOD->PUPDR 	&= ~(GPIO_PUPDR_PUPDR8);		//PUPDR clear
        //		GPIOD->PUPDR 	|=  (GPIO_PUPDR_PUPDR8_0);		//Pull Up 
        //        GPIOD->AFR[1] 	&= ~(GPIO_AFRH_AFSEL8_Msk);     //AF clear
        //		GPIOD->AFR[1] 	|=  (GPIO_AFRH_AFSEL8_2|GPIO_AFRH_AFSEL8_1|GPIO_AFRH_AFSEL8_0); //AF = 7 (USART3)
        //        
        //		//PD9 - RX
        //		GPIOD->MODER 	&= ~(GPIO_MODER_MODER9);		//MODER clear
        //		GPIOD->MODER 	|=  (GPIO_MODER_MODER9_1);		//AF
        //		GPIOD->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR9);	//MAX_speed
        //		GPIOD->PUPDR 	&= ~(GPIO_PUPDR_PUPDR9);		//PUPDR clear
        //        GPIOD->AFR[1] 	&= ~(GPIO_AFRH_AFSEL9_Msk);     //AF clear
        //        GPIOD->AFR[1] 	|=  (GPIO_AFRH_AFSEL9_2|GPIO_AFRH_AFSEL9_1|GPIO_AFRH_AFSEL9_0); //AF = 7 (USART3)
	}
	else if( USART_x == UART4 ){
        //        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
        //        
        //        //PC10 - TX
        //		GPIOC->MODER 	&= ~(GPIO_MODER_MODER10);		//MODER clear
        //		GPIOC->MODER 	|=  (GPIO_MODER_MODER10_1);		//AF
        //		GPIOC->OTYPER 	&= ~(GPIO_OTYPER_OT_10);	    //PushPull
        //		GPIOC->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR10);	//MAX_speed
        //		GPIOC->PUPDR 	&= ~(GPIO_PUPDR_PUPDR10);		//PUPDR clear
        //		GPIOC->PUPDR 	|=  (GPIO_PUPDR_PUPDR10_1);		//Pull Up 
        //		GPIOC->AFR[1] 	&= ~(GPIO_AFRH_AFSEL10_Msk);    //AF clear
        //        GPIOC->AFR[1] 	|=  (GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_0);    //AF = 5 (UART4)
        //        
        //		//PC11 - RX
        //		GPIOC->MODER 	&= ~(GPIO_MODER_MODER11);		//MODER clear
        //		GPIOC->MODER 	|=  (GPIO_MODER_MODER11_1);		//AF
        //		GPIOC->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR11);	//MAX_speed
        //		GPIOC->PUPDR 	&= ~(GPIO_PUPDR_PUPDR11);		//PUPDR clear
        //        GPIOC->AFR[1] 	&= ~(GPIO_AFRH_AFSEL11_Msk);    //AF clear
        //        GPIOC->AFR[1] 	|=  (GPIO_AFRH_AFSEL11_2 | GPIO_AFRH_AFSEL11_0);    //AF = 5 (UART4)	
	}
	else if( USART_x == UART5 ){
        //        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
        //        RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
        //        
        //        //PC12 - TX
        //		GPIOC->MODER 	&= ~(GPIO_MODER_MODER12);		//MODER clear
        //		GPIOC->MODER 	|=  (GPIO_MODER_MODER12_1);		//AF
        //		GPIOC->OTYPER 	&= ~(GPIO_OTYPER_OT_12);	    //PushPull
        //		GPIOC->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR12);	//MAX_speed
        //		GPIOC->PUPDR 	&= ~(GPIO_PUPDR_PUPDR12);		//PUPDR clear
        //		GPIOC->PUPDR 	|=  (GPIO_PUPDR_PUPDR12_1);		//Pull Up 
        //		GPIOC->AFR[1] 	&= ~(GPIO_AFRH_AFSEL12_Msk);    //AF clear
        //        GPIOC->AFR[1] 	|=  (GPIO_AFRH_AFSEL12_2 | GPIO_AFRH_AFSEL12_0);    //AF = 5 (UART5)
        //        
        //		//PD2 - RX
        //		GPIOD->MODER 	&= ~(GPIO_MODER_MODER2);		//MODER clear
        //		GPIOD->MODER 	|=  (GPIO_MODER_MODER2_1);		//AF
        //		GPIOD->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR2);	//MAX_speed
        //		GPIOD->PUPDR 	&= ~(GPIO_PUPDR_PUPDR2);		//PUPDR clear
        //        GPIOD->AFR[0] 	&= ~(GPIO_AFRL_AFSEL2_Msk);     //AF clear
        //        GPIOD->AFR[0] 	|=  (GPIO_AFRL_AFSEL2_2 | GPIO_AFRL_AFSEL2_0);    //AF = 5 (UART5)
	}
	else if( USART_x == LPUART1 ){
        //        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
        //        
        //        //PB11 - TX
        //		GPIOB->MODER 	&= ~(GPIO_MODER_MODER11);		//MODER clear
        //		GPIOB->MODER 	|=  (GPIO_MODER_MODER11_1);		//AF
        //		GPIOB->OTYPER 	&= ~(GPIO_OTYPER_OT_11);	    //PushPull
        //		GPIOB->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR11);	//MAX_speed
        //		GPIOB->PUPDR 	&= ~(GPIO_PUPDR_PUPDR11);		//PUPDR clear
        //		GPIOB->PUPDR 	|=  (GPIO_PUPDR_PUPDR11_1);		//Pull Up 
        //		GPIOB->AFR[1] 	&= ~(GPIO_AFRH_AFSEL11_Msk);    //AF clear
        //        GPIOB->AFR[1] 	|=  (GPIO_AFRH_AFSEL11_3);      //AF = 8 (LPUART1)
        //        
        //		//PB10 - RX
        //		GPIOB->MODER 	&= ~(GPIO_MODER_MODER10);		//MODER clear
        //		GPIOB->MODER 	|=  (GPIO_MODER_MODER10_1);		//AF
        //		GPIOB->OSPEEDR 	|=  (GPIO_OSPEEDER_OSPEEDR10);	//MAX_speed
        //		GPIOB->PUPDR 	&= ~(GPIO_PUPDR_PUPDR10);		//PUPDR clear
        //        GPIOB->AFR[1] 	&= ~(GPIO_AFRH_AFSEL10_Msk);    //AF clear
        //        GPIOB->AFR[1] 	|=  (GPIO_AFRH_AFSEL10_3);      //AF = 8 (LPUART1)
	}
}

uint32_t usart_clock_config ( USART_TypeDef* USART_x )
{
    uint32_t bus_freq = 0x00;
    /**** USART clocking setting ****/
	if( USART_x == USART1 ){
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART16SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_2;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:     break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
	}
	else if( USART_x == USART2 ){
		RCC->APB1LENR |= RCC_APB1LENR_USART2EN;
        
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART28SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_1;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:        break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
	}
	else if( USART_x == USART3 ){
		RCC->APB1LENR |= RCC_APB1LENR_USART3EN;
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART28SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_1;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:        break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
	}
	else if( USART_x == UART4 ){
		RCC->APB1LENR |= RCC_APB1LENR_UART4EN;
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART28SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_1;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:        break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
	}
	else if( USART_x == UART5 ){
		RCC->APB1LENR |= RCC_APB1LENR_UART5EN;
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART28SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_1;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:        break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
	}
    else if( USART_x == USART6 ){
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART16SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_2;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:     break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
    }
    else if( USART_x == UART7 ){
		RCC->APB1LENR |= RCC_APB1LENR_UART7EN;
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART28SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_1;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:        break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
	}
    else if( USART_x == UART8 ){
		RCC->APB1LENR |= RCC_APB1LENR_UART8EN;
        switch ( RCC->D2CCIP2R &  RCC_D2CCIP2R_USART28SEL_Msk ){
            case 0x00: bus_freq = rcc_clocks.bus_freq.apb_1;      break;
            case 0x01:     break;
            case 0x02:     break;
            case 0x03: bus_freq = rcc_clocks.source_val.hsi;       break;
            case 0x04:        break;
            case 0x05: bus_freq = rcc_clocks.source_val.lse;       break;
            default:  return ERROR; break;
        }
        return bus_freq;
	}

    return ERROR;
}
