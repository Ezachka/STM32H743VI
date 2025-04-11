#include "display_xxxx.h"

/**
******************************************************************************
* File Name          : display_xxx.c
* Description        : This lib made for any HD44780 LCDs with i2c adapter
*                      i2c comunnication only  
******************************************************************************
* @attention
* -1602 tested, correct working
* 
* 
*
******************************************************************************
*/

/****************************************************************************/
/****************************** DEFINITIONS *********************************/

/****************************************************************************/
/******************************* VARIABLES **********************************/
display_xxxx_t      display;
/****************************************************************************/
/******************************* FUNCTIONS **********************************/
ErrorStatus lcd_i2c_data_send(I2C_TypeDef* I2C_x,uint8_t data, uint8_t flags);
ErrorStatus lcd_xxxx_go_to_line(I2C_TypeDef* I2C_x,uint8_t line_number);
ErrorStatus lcd_xxxx_identification(display_xxxx_t* display, uint8_t adr,uint8_t columns, uint8_t lines);
ErrorStatus lcd_xxxx_clear(I2C_TypeDef* I2C_x);
ErrorStatus lcd_xxxx_next_stroke(I2C_TypeDef* I2C_x);
/****************************************************************************/
/************************  HW DEPENDENT FUNCTIONS  **************************/

/**
* @brief lcd_i2c_data_send
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_i2c_data_send(I2C_TypeDef* I2C_x,uint8_t data, uint8_t flags){
    i2c_status_t status;
	uint8_t msb = data & 0xF0;   
    uint8_t lsb =(data << 4) & 0xF0;          
	uint8_t arr[4];
    uint8_t back_light;
    if(display.settings.black_light_on){//&& flags==DISPLAY_XXXX_SEND_DATA){
        back_light=DISPLAY_XXXX_BACKLIGHT;
    }
    else{
        back_light=DISPLAY_XXXX_NO_BACKLIGHT;
    }
	arr[0] = msb|flags|back_light|DISPLAY_XXXX_EN; 
	arr[1] = msb|flags|back_light;         
	arr[2] = lsb|flags|back_light|DISPLAY_XXXX_EN;
	arr[3] = lsb|flags|back_light;
    
    status = i2c_transmit(I2C_x,DISPLAY_XXXX_I2C_ADDR, arr, sizeof(arr), 50); 
    delay_ms(1);
    if(status==transmitted){
        display.connect = true;
        return SUCCESS;
    }
    display.connect = false;
    return ERROR;
}



/****************************************************************************/
/**********************  HW UNDEPENDENT FUNCTIONS  **************************/
/**
* @brief lcd_xxxx_config_update
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_xxxx_config_update(I2C_TypeDef* I2C_x){
    ErrorStatus status = SUCCESS;   
    static uint8_t old_functions;
    static uint8_t old_display_control;
    static uint8_t old_entry_mode;
    static uint8_t old_cursor_shift;
    
    
    ///opration
    if(display.operation.clear_display){
        status|=lcd_xxxx_clear(I2C_x);
        display.operation.clear_display=false;
    }
    else if(display.operation.return_home){
        status|=lcd_xxxx_home(I2C_x);
        display.operation.return_home=false;
    }
    else if(display.operation.set_cursor){
        lcd_xxxx_cursor(I2C_x,display.data.cursor_column,display.data.cursor_line);
        display.operation.set_cursor=false;
    }
    else if(display.operation.update_data){
        status|=lcd_xxxx_clear(I2C_x);
        status|=lcd_xxxx_write_on_line(I2C_x,display.data.line_one,1);
        status|=lcd_xxxx_write_on_line(I2C_x,display.data.line_two,2);
        status|=lcd_xxxx_write_on_line(I2C_x,display.data.line_three,3);
        status|=lcd_xxxx_write_on_line(I2C_x,display.data.line_four,4);
        display.operation.update_data=false;
    }

    ///settings change
    //entry mode
    if(display.settings.entry_mode.increment_for_cursor){
        display.regs.entry_mode|=DISPLAY_XXXX_ENTRY_SHIFT_INCREMENT;
    }
    else{
        display.regs.entry_mode&=~DISPLAY_XXXX_ENTRY_SHIFT_INCREMENT;    //dicrement---
    }
        if(display.settings.entry_mode.shift_left){
        display.regs.entry_mode|=DISPLAY_XXXX_ENTRY_LEFT;
    }
    else{
        display.regs.entry_mode&=~DISPLAY_XXXX_ENTRY_LEFT;           //right
    }
    //functions
    if(display.settings.function.interface_8_or_4){
        display.regs.functions|=DISPLAY_XXXX_8_BIT_MODE;
    }
    else{
        display.regs.functions&=~DISPLAY_XXXX_8_BIT_MODE;    //4bit
    }
    if(display.settings.function.line_two_or_one){
        display.regs.functions|=DISPLAY_XXXX_2_LINE;
    }
    else{
        display.regs.functions&=~DISPLAY_XXXX_2_LINE;    //1 line
    }    
    if(display.settings.function.dot_size_h){
        display.regs.functions|=DISPLAY_XXXX_5x10_DOTS;
    }
    else{
        display.regs.functions&=~DISPLAY_XXXX_5x10_DOTS;    //5x8
    }    
    //display control
    if(display.settings.display_control.display_on){
        display.regs.display_control|=DISPLAY_XXXX_DISPLAY_ON;
    }
    else{
        display.regs.display_control&=~DISPLAY_XXXX_DISPLAY_ON;   //off 
    }
    if(display.settings.display_control.cursor_on){
        display.regs.display_control|=DISPLAY_XXXX_CURSOR_ON;
    }
    else{
        display.regs.display_control&=~DISPLAY_XXXX_CURSOR_ON;    //off cursor
    }
    if(display.settings.display_control.cursor_blink_on){
        display.regs.display_control|=DISPLAY_XXXX_BLINK_ON;
    }
    else{
        display.regs.display_control&=~DISPLAY_XXXX_BLINK_ON;    //off blink
    }
        //shift
    if(display.settings.shift.s_c){
        display.regs.cursor_shift|=DISPLAY_XXXX_DISPLAY_MOVE;
    }
    else{
        display.regs.cursor_shift&=~DISPLAY_XXXX_DISPLAY_MOVE;       //
    }
    if(display.settings.shift.r_l){
        display.regs.cursor_shift|=DISPLAY_XXXX_MOVE_RIGHT;
    }
    else{
        display.regs.cursor_shift&=~DISPLAY_XXXX_MOVE_RIGHT;           //
    }
    ///regs update
    if(old_functions!=display.regs.functions){
        status|=lcd_i2c_data_send(I2C_x,display.regs.functions,DISPLAY_XXXX_SEND_SETTINGS);   
        delay_ms(5);
        old_functions=display.regs.functions;
        status|=lcd_xxxx_clear(I2C_x);  
        display.operation.update_data=true;
    }
    else if(old_display_control!=display.regs.display_control){
        status|=lcd_i2c_data_send(I2C_x,display.regs.display_control,DISPLAY_XXXX_SEND_SETTINGS);
        delay_ms(5);
        old_display_control=display.regs.display_control;
        status|=lcd_xxxx_clear(I2C_x);  
        display.operation.update_data=true;
    } 
    else if(old_entry_mode!=display.regs.entry_mode){
        status|=lcd_i2c_data_send(I2C_x,display.regs.entry_mode,DISPLAY_XXXX_SEND_SETTINGS);
        delay_ms(5);
        old_entry_mode=display.regs.entry_mode;
        status|=lcd_xxxx_clear(I2C_x);  
        display.operation.update_data=true;
    } 
    else if(old_cursor_shift!=display.regs.cursor_shift){
        status|=lcd_i2c_data_send(I2C_x,display.regs.cursor_shift,DISPLAY_XXXX_SEND_SETTINGS); 
        delay_ms(5);
        old_cursor_shift=display.regs.cursor_shift;
        status|=lcd_xxxx_clear(I2C_x);  
        display.operation.update_data=true;
    } 
    
    return status;
}

/**
* @brief lcd_xxxx_init
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_xxxx_init(I2C_TypeDef* I2C_x,uint8_t adr,uint8_t columns, uint8_t lines,bool back_light){
    ErrorStatus status = SUCCESS;   
    
    lcd_xxxx_identification(&display,adr,columns,lines);
    
    strcpy(display.data.line_one,"    Hello");
    strcpy(display.data.line_two,"    World!");
    display.settings.black_light_on =   back_light;
    
    display.regs.functions              |=   DISPLAY_XXXX_CMD_FUNCTION_SET;
    display.regs.entry_mode             |=   DISPLAY_XXXX_CMD_ENTRY_MODE_SET;
    display.regs.display_control        |=   DISPLAY_XXXX_CMD_DISPLAY_CONTROL;
    display.regs.cursor_shift           |=   DISPLAY_XXXX_CMD_CURSOR_SHIFT;
    
    display.settings.function.line_two_or_one=true;
    display.settings.function.interface_8_or_4=true;
    display.settings.function.dot_size_h=false;
    
    display.settings.display_control.display_on=true;
    display.settings.display_control.cursor_on=false;
    display.settings.display_control.cursor_blink_on=false;
    
    display.settings.entry_mode.increment_for_cursor=false;
    display.settings.entry_mode.shift_left=true;
    
    display.settings.shift.s_c=false;
    display.settings.shift.r_l=false;
    
    status |= lcd_xxxx_config_update(I2C_x);  
    
    delay_ms(5);
    status |= lcd_xxxx_home(I2C_x);  //set cursor at begin of stro
    delay_ms(5);
    status |= lcd_xxxx_clear(I2C_x); //clear display
    display.operation.update_data=true;
    return status;
}


/**
* @brief lcd_xxxx_write_on_line
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_xxxx_write_on_line(I2C_TypeDef* I2C_x,char *str, uint8_t line){
    ErrorStatus status = SUCCESS;  
    uint8_t len = strlen(str);
    if(len>display.columns){
        len = display.columns;
        status = ERROR;
    }
    if(line>display.lines){
        return ERROR;
    }
    status|=lcd_xxxx_go_to_line(I2C_x,line);
    for (uint8_t i = 0; i < len; i++){
        status |= lcd_i2c_data_send(I2C_x, (uint8_t)str[i], DISPLAY_XXXX_SEND_DATA);
    }
    return status;
}



/**
* @brief lcd_xxxx_change_line
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_xxxx_change_line(char *str, uint8_t line){
    ErrorStatus status = SUCCESS;
    switch (line) {
        case 1:
        if (strncmp(display.data.line_one, str, display.columns) == 0) {
            return ERROR; 
        }
        memset(display.data.line_one, 0x00, sizeof(display.data.line_one));
        strncpy(display.data.line_one, str, display.columns);
        break;
        
        case 2:
        if (strncmp(display.data.line_two, str, display.columns) == 0) {
            return ERROR;
        }
        memset(display.data.line_two, 0x00, sizeof(display.data.line_two));
        strncpy(display.data.line_two, str, display.columns);
        break;
        
        case 3:
        if (strncmp(display.data.line_three, str, display.columns) == 0) {
            return ERROR;
        }
        memset(display.data.line_three, 0x00, sizeof(display.data.line_three));
        strncpy(display.data.line_three, str, display.columns);
        break;
        
        case 4:
        if (strncmp(display.data.line_four, str, display.columns) == 0) {
            return ERROR;
        }
        memset(display.data.line_four, 0x00, sizeof(display.data.line_four));
        strncpy(display.data.line_four, str, display.columns);
        break;
        
        default:
        return ERROR;
    }
    display.operation.update_data=true;
    return status;
}

/**
* @brief lcd_xxxx_identification
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_xxxx_identification(display_xxxx_t* display, uint8_t adr,uint8_t columns, uint8_t lines){
    display->columns=columns;
    display->lines=lines;
    display->adr=adr;
    switch (columns) {
        case 8:
        if (lines == 1) {
            strncpy(display->name, "0801", 4);
            return SUCCESS;
        } else if (lines == 2) {
            strncpy(display->name, "0802", 4);
            return SUCCESS;
        } else {
            strncpy(display->name, "ERR ", 4);
            return ERROR;
        }
        break;
        case 16:
        if (lines == 1) {
            strncpy(display->name, "1601", 4);
            return SUCCESS;
        } else if (lines == 2) {
            strncpy(display->name, "1602", 4);
            return SUCCESS;
        } else if (lines == 4) {
            strncpy(display->name, "1604", 4);
            return SUCCESS;
        } else {
            strncpy(display->name, "ERR ", 4);
            return ERROR;
        }
        break;
        case 20:
        if (lines == 1) {
            strncpy(display->name, "2001", 4);
            return SUCCESS;
        } else if (lines == 2) {
            strncpy(display->name, "2002", 4);
            return SUCCESS;
        } else if (lines == 4) {
            strncpy(display->name, "2004", 4);
            return SUCCESS;
        } else {
            strncpy(display->name, "ERR ", 4);
            return ERROR;
        }
        break;
        case 24:
        if (lines == 1) {
            strncpy(display->name, "2401", 4);
            return SUCCESS;
        } else if (lines == 2) {
            strncpy(display->name, "2402", 4);
            return SUCCESS;
        } else {
            strncpy(display->name, "ERR ", 4);
            return ERROR;
        }
        break;
        case 40:
        if (lines == 1) {
            strncpy(display->name, "4001", 4);
            return SUCCESS;
        } else if (lines == 2) {
            strncpy(display->name, "4002", 4);
            return SUCCESS;
        } else {
            strncpy(display->name, "ERR ", 4);
            return ERROR;
        }
        break;
        
        default:
        strncpy(display->name, "ERR ", 4);
        return ERROR;
        break;
    }
}
/**
* @brief lcd_xxxx_go_to_line
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_xxxx_go_to_line(I2C_TypeDef* I2C_x,uint8_t line_number){
    if(line_number>display.lines){
        return ERROR;
    }
    switch(line_number){
        case 1:
        return lcd_i2c_data_send(I2C_x,0b10000000,DISPLAY_XXXX_SEND_SETTINGS);  //line 1
        break;
        case 2:
        return lcd_i2c_data_send(I2C_x,0b11000000,DISPLAY_XXXX_SEND_SETTINGS);  //line 2
        break;       
        case 3:
        return lcd_i2c_data_send(I2C_x,0b10010100,DISPLAY_XXXX_SEND_SETTINGS);  //line 3
        break;       
        case 4:
        return lcd_i2c_data_send(I2C_x,0b11010100,DISPLAY_XXXX_SEND_SETTINGS);  //line 4
        break;          
        default:
        return ERROR;
        break;
    }
}

/**
* @brief lcd_xxxx_clear
*  
*  
* 
* @param[in] I2C_x  I2C peripheral
* @return    ErrorStatus
*/

ErrorStatus lcd_xxxx_clear(I2C_TypeDef* I2C_x){
    ErrorStatus status = SUCCESS;   
    status|=lcd_i2c_data_send(I2C_x,DISPLAY_XXXX_CMD_CLEAR,DISPLAY_XXXX_SEND_SETTINGS);  //clear display
    return status;
}
/**
* @brief lcd_xxxx_home
*  
*  
* 
* @param[in] I2C_x  I2C peripheral
* @return    ErrorStatus
*/

ErrorStatus lcd_xxxx_home(I2C_TypeDef* I2C_x){
    ErrorStatus status = SUCCESS;   
    status|=lcd_i2c_data_send(I2C_x,DISPLAY_XXXX_CMD_RETURN_HOME,DISPLAY_XXXX_SEND_SETTINGS);  //go home
    return status;
}
/**
* @brief lcd_xxxx_cursor
*  
*  
* 
* @param[in] 
* @return    
*/

ErrorStatus lcd_xxxx_cursor(I2C_TypeDef* I2C_x,uint8_t column, uint8_t line){
    ErrorStatus status = SUCCESS;
    uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( line > (display.lines-1) ) {
		line = display.lines-1;    
	}
    status|=lcd_i2c_data_send(I2C_x,DISPLAY_XXXX_CMD_SET_DDRAMA_DDR | (column + row_offsets[line]),DISPLAY_XXXX_SEND_SETTINGS);  //set cursor
    return status;
}

