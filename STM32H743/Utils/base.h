#ifndef BASE_H
#define BASE_H

	

	/// sptit by math, types, string, delay, convert data

	#define TRUE 1
	#define FALSE 0


	
	
	float array_to_float( unsigned char* data, unsigned char start );
	float array_to_float_no_nan( unsigned char* data, unsigned char start );
	void float_to_array( float number, unsigned char* data, unsigned char start );
	unsigned short array_to_ushort( unsigned char* data, unsigned char start );
	void ushort_to_array( unsigned short number, unsigned char* data, unsigned char start );
	void ulong_to_array( unsigned long number, unsigned char* data, unsigned char start );
	
	void long_to_str( long number, unsigned char* string_, unsigned char *string_len );
	void ulong_to_str( unsigned long number, unsigned char* string_, unsigned char *string_len );
	void short_to_str( short number, unsigned char* string_, unsigned char *string_len );
	void ushort_to_str( unsigned short number, unsigned char* string_, unsigned char *string_len );
	void uchar_to_str( unsigned char number, unsigned char* string_, unsigned char *string_len );
	
	unsigned char str_to_uchar( unsigned char* str, unsigned char str_len);
	unsigned short str_to_ushort( unsigned char* str, unsigned char str_len);	
	float str_to_float( unsigned char* str, unsigned char str_len);
	
	unsigned char strings_compare( unsigned char* first, unsigned char* second, unsigned char length );

	void array_clear( unsigned char* arr, unsigned char arr_len);
	
			
	static const float pi = 3.14159265359;
	
	
	double abs_double( double value );
	float abs_float( float value );
	short abs_short( short value );
	long abs_long( long value );
	
	float round_float( float value, float round );
	unsigned char is_nan( float *value );
	
	void limit_uchar( unsigned char *number, unsigned char max, unsigned char min );
	void limit_short( short *number, short max, short min );
	void limit_ushort( unsigned short *number, unsigned short max, unsigned short min );
	void limit_ulong( unsigned long *number, unsigned long max, unsigned long min );
	void limit_long( long *number, long max, long min );
	void limit_float( float *number, float max, float min );
	void limit_abs_uchar( unsigned char *number, unsigned char max_abs );
	void limit_abs_short( short *number, short max_abs );
	void limit_abs_ushort( unsigned short *number, unsigned short max_abs );
	void limit_abs_ulong( unsigned long *number, unsigned long max_abs );
	void limit_abs_long( long *number, long max_abs );
	void limit_abs_float( float *number, float max_abs );

	/// round_float
	long float_to_long( float value );
	
	float deg_to_rad( float angle_degree );
	float rad_to_deg( float angle_rad );
	
	float angle_180( float angle_degree );
	float angle_360( float angle_degree );
	float angle_360_positive( float angle_degree );
	
	unsigned char random_get( void );
	unsigned char random_seed_get( unsigned char seed );
	
	
	float max(float x,float y);
	float min(float x,float y);
	

#endif //BASE_H
