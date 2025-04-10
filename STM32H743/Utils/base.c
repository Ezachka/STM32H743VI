#include "base.h"

typedef union 
{ 
	float number;  
	unsigned char bytes[4]; 
} floatunion_t; 
floatunion_t floatunion;

float array_to_float( unsigned char* data, unsigned char start )
{
	floatunion.bytes[0] = data[ start + 0 ];
	floatunion.bytes[1] = data[ start + 1 ];
	floatunion.bytes[2] = data[ start + 2 ];
	floatunion.bytes[3] = data[ start + 3 ];
	
	return floatunion.number;
}

float array_to_float_no_nan( unsigned char* data, unsigned char start )
{
	floatunion.bytes[0] = data[ start + 0 ];
	floatunion.bytes[1] = data[ start + 1 ];
	floatunion.bytes[2] = data[ start + 2 ];
	floatunion.bytes[3] = data[ start + 3 ];
	
	if( is_nan( &floatunion.number ) ) return 0.0f;
	return floatunion.number;
}

void float_to_array( float number, unsigned char* data, unsigned char start )
{
	floatunion.number = number;
	
	data[ start + 0 ] = floatunion.bytes[0];
	data[ start + 1 ] = floatunion.bytes[1];
	data[ start + 2 ] = floatunion.bytes[2];
	data[ start + 3 ] = floatunion.bytes[3];
}

typedef union 
{ 
	unsigned long number;  
	unsigned char bytes[4]; 
} ulongunion_t; 
ulongunion_t ulongunion;

unsigned long array_to_ulong( unsigned char* data, unsigned long start )
{
	ulongunion.bytes[0] = data[ start + 0 ];
	ulongunion.bytes[1] = data[ start + 1 ];
	ulongunion.bytes[2] = data[ start + 2 ];
	ulongunion.bytes[3] = data[ start + 3 ];
		
	return ulongunion.number;
}

void ulong_to_array( unsigned long number, unsigned char* data, unsigned char start )
{
	ulongunion.number = number;
	
	data[ start + 0 ] = ulongunion.bytes[0];
	data[ start + 1 ] = ulongunion.bytes[1];
	data[ start + 2 ] = ulongunion.bytes[2];
	data[ start + 3 ] = ulongunion.bytes[3];
}

typedef union 
{ 
	unsigned short number;  
	unsigned char bytes[2]; 
} ushortunion_t; 
ushortunion_t ushortunion;

unsigned short array_to_ushort( unsigned char* data, unsigned char start )
{
	ushortunion.bytes[0] = data[ start + 0 ];
	ushortunion.bytes[1] = data[ start + 1 ];
		
	return ushortunion.number;
}

void ushort_to_array( unsigned short number, unsigned char* data, unsigned char start )
{
	ushortunion.number = number;
	
	data[ start + 0 ] = ushortunion.bytes[0];
	data[ start + 1 ] = ushortunion.bytes[1];
}



void uchar_to_str( unsigned char number, unsigned char*string_, unsigned char *string_len )
{
	if( number == 0 ) 
	{
		string_[0] = 48; //ASCII '0'
		*string_len = 1;
		
		return;
	} 
	

	unsigned char digit = 100;
	
	while( digit >= 1 )
	{
		if( number > digit ) 
		{
			string_[ *string_len ] = 48 + (number / digit) % 10;
			*string_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}	
} //.uchar_to_str

void ushort_to_str( unsigned short number, unsigned char* string_, unsigned char *string_len )
{
	if( number == 0 ) 
	{
		string_[0] = 48; //ASCII '0'
		*string_len = 1;
		
		return;
	} 
	

	unsigned short digit = 10000;
	
	while( digit >= 1 )
	{
		if( number > digit ) 
		{
			string_[ *string_len ] = 48 + (number / digit) % 10;
			*string_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}	
} //.ushort_to_str

void short_to_str( short number, unsigned char* string_, unsigned char *string_len )
{
	if( number == 0 ) 
	{
		string_[0] = 48; //ASCII '0'
		*string_len = 1;
		
		return;
	} 
	
	
	char less_zero_k = 1;
	if( number < 0 ) less_zero_k = -1;
	

	unsigned short digit = 10000;
	
	while( digit >= 1 )
	{
		if( number > digit ) 
		{
			string_[ *string_len ] = 48 + (( number * less_zero_k ) / digit) % 10;
			*string_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}
} //.short_to_str

void ulong_to_str( unsigned long number, unsigned char* string_, unsigned char *string_len )
{
	if( number == 0 ) 
	{
		string_[0] = 48; //ASCII '0'
		*string_len = 1;
		
		return;
	} 
	

	unsigned long digit = 1000000000;
	
	while( digit >= 1 )
	{
		if( number > digit ) 
		{
			string_[ *string_len ] = 48 + (number / digit) % 10;
			*string_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}	
} //.ulong_to_str

void long_to_str( long number, unsigned char* string_, unsigned char *string_len )
{
	if( number == 0 ) 
	{
		string_[0] = 48; //ASCII '0'
		*string_len = 1;
		
		return;
	} 
	
	
	char less_zero_k = 1;
	if( number < 0 ) less_zero_k = -1;
	

	unsigned long digit = 1000000000;
	
	while( digit >= 1 )
	{
		if( number > digit ) 
		{
			string_[ *string_len ] = 48 + (( number * less_zero_k ) / digit) % 10;
			*string_len += 1;
		}
		
		
		if( digit == 1) return;
		digit /= 10;
	}
} //.long_to_str



unsigned char str_to_uchar( unsigned char* str, unsigned char str_len)
{
	//uchar max value 255
	unsigned char result = 0;
	
	unsigned char numeral = 0;
	unsigned char digit = 1;
	const unsigned char digit_max = 100;

	
	unsigned char temp_uchar;
	unsigned char i = str_len;
	
	while( i-- ) 
	{
		temp_uchar = str[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;			
			if( (digit == digit_max) && (numeral > 2) && (result > 55) ) return result; // error data
			
			result += numeral * digit;
			
			if( digit == digit_max ) return result; 
			digit *= 10;
		}
		else
		{
			if( digit != 1 ) return result; // error data
		}
	}
	
	return result;
}

unsigned short str_to_ushort( unsigned char* str, unsigned char str_len)
{
	//ushort max value 65535
	unsigned short result = 0;
	
	unsigned char numeral = 0;
	unsigned short digit = 1;
	const unsigned short digit_max = 10000;
	
	
	unsigned char temp_uchar;
	unsigned char i = str_len;
	
	while( i-- ) 
	{
		temp_uchar = str[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;
			if( (digit == digit_max) && (numeral > 6) && (result > 5535) ) return result; // error data
			
			result += numeral * digit;
			
			if( digit == digit_max ) return result;
			digit *= 10;
		}
		else
		{
			if( digit != 1 ) return result; // error data
		}
	}
	
	return result;
}

short str_to_short( unsigned char* str, unsigned char str_len)
{
	//short max value from -32767 to +32767
	short result = 0;
	

	unsigned char numeral = 0;
	unsigned short digit = 1;
	const unsigned short digit_max = 10000;

	unsigned char math_sign_neg_f = FALSE;
	
		
	unsigned char temp_uchar;
	unsigned char i = str_len;
	
	while( i-- ) 
	{
		temp_uchar = str[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			numeral = temp_uchar - 48;			
			if( (digit == digit_max) && (numeral > 3) && (result > 2767) ) break; // error data
			
			result += numeral * digit;
			
			if( digit == digit_max ) break; // error data
			else digit *= 10;
		}
		else
		{
			if( (digit != 1) && (temp_uchar == 45) ) 
			{
				math_sign_neg_f = TRUE;
				break;
			}
		}
	}
	

	if( math_sign_neg_f ) result *= -1;
	
	return result;
}



float str_to_float( unsigned char* str, unsigned char str_len)
{
	//short max value from -32767 to +32767
	float result = 0;
	
	unsigned char numeral = 0;	
	
	unsigned char math_sign_neg_f = FALSE;
	
	unsigned char integral_part_f = FALSE;	
	unsigned long integral_value = 0;
	unsigned long integral_digit = 1;
	const unsigned long integral_digit_max = 100000000;
	float fractional_value = 0;
	unsigned long fractional_digit = 1;
	const unsigned long fractional_digit_max = 100000000;	
	
		
	unsigned char temp_uchar;
	unsigned char i = str_len;	
	
	while( i-- ) 
	{
		temp_uchar = str[i];
		if( (temp_uchar > 47) && (temp_uchar < 58) ) //ASCII codes for 0..9, 48 == '0' && 57 == '9'
		{
			
			numeral = temp_uchar - 48;
			
			if( integral_part_f == FALSE )
			{		
				if( (fractional_digit == fractional_digit_max) && (numeral > 2) && (fractional_value > 147483647) ) break; // error data
				
				fractional_value += numeral * fractional_digit;
				
				if( fractional_digit == fractional_digit_max ) break; // error data
				else fractional_digit *= 10;
			}
			else
			{			
				if( (integral_digit == integral_digit_max) && (numeral > 2) && (integral_value > 147483647) ) break; // error data
				
				integral_value += numeral * integral_digit;
				
				if( integral_digit == integral_digit_max ) break; // error data;
				else integral_digit *= 10;
			}
			
		}
		else
		{
			
			if( (temp_uchar == 46) && (integral_part_f == FALSE) ) integral_part_f = TRUE;
			
			if( (fractional_digit != 1) && (temp_uchar == 45) ) 
			{
				math_sign_neg_f = TRUE;
				break;
			}
			
		}
	}//while( i-- ) 
	

	if( integral_part_f == FALSE )
	{
		result = fractional_value;
	}
	else
	{
		result = integral_value;		
		result += (fractional_value / fractional_digit);
	}
	if( math_sign_neg_f ) result *= -1;
	
	return result;
}



unsigned char strings_compare( unsigned char* first, unsigned char* second, unsigned char length )
{
	for( unsigned char i = 0; i < length; i++ )
	{
		if( first[i] != second[i] )
		{
			 return FALSE;
		}
	}

  	return TRUE;
}



void array_clear( unsigned char* arr, unsigned char arr_len)
{
	for( unsigned char i = 0; i < arr_len; i++ ) arr[i] = 0;
}




double abs_double( double value )
{
	if( value < 0 )
	{
		return value * (-1.0f);
	}
	
	return value;
}

float abs_float( float value )
{
	if( value < 0 )
	{
		return value * (-1.0f);
	}
	
	return value;
}

short abs_short( short value )
{
	if( value < 0 )
	{
		return value * (-1);
	}
	
	return value;
}

long abs_long( long value )
{
	if( value < 0 )
	{
		return value * (-1);
	}
	
	return value;
}



float round_float( float value, float round )
{
	if( abs_float(value) < round ) return 0.0f;

	int div_value = (int)(value / round);
	
	return div_value * round;
}

unsigned char is_nan( float *value )
{
	return ((((unsigned char*)value)[2] & 0x80)==0x80) && ((((unsigned char*)value)[3] & 0x7F)==0x7F);
}



void limit_uchar( unsigned char *number, unsigned char max, unsigned char min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}
void limit_short( short *number, short max, short min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}
void limit_ushort( unsigned short *number, unsigned short max, unsigned short min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}
void limit_ulong( unsigned long *number, unsigned long max, unsigned long min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}
void limit_long( long *number, long max, long min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}
void limit_float( float *number, float max, float min )
{
	if( *number > max ) *number = max;
	if( *number < min ) *number = min;
}
void limit_abs_uchar( unsigned char *number, unsigned char max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}
void limit_abs_short( short *number, short max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}
void limit_abs_ushort( unsigned short *number, unsigned short max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}
void limit_abs_ulong( unsigned long *number, unsigned long max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}
void limit_abs_long( long *number, long max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}
void limit_abs_float( float *number, float max_abs )
{
	if( *number > max_abs ) *number = max_abs;
	if( *number < -1*max_abs ) *number = -1*max_abs;
}



long float_to_long( float value )
{
	long value_round = 0;
	value_round = (long) value;

	static const double fraction_up = 0.5555555;
	if(  ( (value + fraction_up) - value_round ) > 1  ) //round up to 1
	{
		value_round++;
	}
	
	return value_round;
}



float deg_to_rad( float angle_degree )
{
	return ( angle_degree * pi ) / 180.0f;
}

float rad_to_deg( float angle_rad )
{
	return ( angle_rad * 180.0f ) / pi;
}


	
float angle_180( float angle_degree )
{
	short  revolutions = (short)( angle_degree / 360.0f );
	float return_angle = angle_degree - (revolutions * 360.0f);	
	
	if( return_angle > 180 ) return_angle = return_angle - 360;
	else if( return_angle < -180 ) return_angle = return_angle + 360;
	
	return return_angle;
}

float angle_360( float angle_degree )
{
	short revolutions = (short)( abs_float(angle_degree) / 360.0f );
	float return_angle = angle_degree - (revolutions * 360.0f);
	
	return return_angle;
}

float angle_360_positive( float angle_degree )
{
	short revolutions = (short)(angle_degree / 360.0f);
	float return_angle = angle_degree - (revolutions * 360.0f);
	
	if( return_angle < 0 ) return_angle += 360.0f;
	
	return return_angle;
}



unsigned char random_get( void )
{
	static unsigned char rand_a;
	static unsigned char rand_b;
	static unsigned char rand_c;
	static unsigned char rand_x;

	rand_x = 0x77; //random value
	rand_a = (rand_a^rand_c^rand_x);
	rand_b = (rand_b + rand_a);
	rand_c = (rand_c + (rand_b>>1)^rand_a);
	
	return rand_c;
}

unsigned char random_seed_get( unsigned char seed )
{
	static unsigned char rand_a;
	static unsigned char rand_b;
	static unsigned char rand_c;

	rand_a = (rand_a^rand_c^seed);
	rand_b = (rand_b + rand_a);
	rand_c = (rand_c + (rand_b>>1)^rand_a);
	
	return rand_c;
}


float max(float x,float y)
{
	return x>y?x:y;
}

float min(float x,float y)
{
	return x>y?y:x;
}


