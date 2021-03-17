/******************************************************************************
Copyright (C) 2018-2020 aitos.io

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

/*!@brief Header file for utility functions

@file
boatutility.h is header file for boatwallet utility functions.
*/

#ifndef __BOATUTILITY_H__
#define __BOATUTILITY_H__

#include "boatiotsdk.h"


/*! @defgroup utility boat utility
 * @{
 */

//!@brief Argument type for UtilityTrimBin(), UtilityHex2Bin() and UtilityUint32ToBigend()
typedef enum
{
    TRIMBIN_TRIM_NO,    //!< Don't trim zeros
    TRIMBIN_LEFTTRIM    //!< Trim leading or MSB zeros
}TRIMBIN_TRIM_MODE;

//!@brief Argument type for UtilityHex2Bin()
typedef enum
{
    BIN2HEX_TRIM_NO = 0,        //!< Don't trim zeros
    BIN2HEX_LEFTTRIM_QUANTITY,  //!< Trim {0x00, 0x01, 0x00 0xAB} => "0x100AB" or "100AB"
    BIN2HEX_LEFTTRIM_UFMTDATA   //!< Trim {0x00, 0x01, 0x00 0xAB} => "0x0100AB" or "0100AB"
}BIN2HEX_TRIM_MODE;

//!@brief Argument type for UtilityBin2Hex()
typedef enum
{
    BIN2HEX_PREFIX_0x_NO = 0,   //<! Prepend "0x" to converted HEX string
    BIN2HEX_PREFIX_0x_YES       //<! Don't prepend "0x" to converted HEX string
}BIN2HEX_PREFIX_0x_MODE;


//!@brief A struct to maintain a dynamic length string.
typedef struct TStringWithLen
{
    BCHAR *string_ptr;   //!< address of the string storage
    BUINT32 string_len;  //!< string length in byte excluding NULL terminator, equal to strlen(string_ptr)
    BUINT32 string_space;//!< size of the space <string_ptr> pointing to, including null terminator
}StringWithLen;


//!@brief A struct to big number.
//!bignum256 are 256 bits stored as 8*30 bit + 1*16 bit
//!val[0] are lowest 30 bits, val[8] highest 16 bits
typedef struct {
	BUINT32 val[9];
} bignum256;

extern const BCHAR * const g_log_level_name_str[];

/*!****************************************************************************
 * @brief Log Output
 *
 * @param level
 *   Log priority level. 
 *   One of BOAT_LOG_CRITICAL, BOAT_LOG_NORMAL or BOAT_LOG_VERBOSE.
 *
 *@param format
 *   Similar to that in printf().
 ******************************************************************************/
#if BOAT_LOG_LEVEL == BOAT_LOG_NONE
#define BoatLog(level, format,...)
#else
#define BoatLog(level, format,...)\
    do{\
        if( level <= BOAT_LOG_LEVEL ) {printf("%s: "__FILE__":%d, %s(): "format"\n", g_log_level_name_str[level-1], __LINE__, __func__, ##__VA_ARGS__);}\
    }while(0)
#endif


/*!****************************************************************************
 * @brief Log Output in hex mode
 *
 * @param level
 *   Log priority level. 
 *   One of BOAT_LOG_CRITICAL, BOAT_LOG_NORMAL or BOAT_LOG_VERBOSE.
 *
 * @param title
 *   tips of this dump
 *
 * @param buf
 *   buffer to be dump
 *
 * @param len
 *   dump len
 *
 * @see BoatLog BoatLog_hexacsiidump
 ******************************************************************************/
#if BOAT_LOG_LEVEL == BOAT_LOG_NONE
#define BoatLog_hexdump(level, title, buf, len)
#else
#define BoatLog_hexdump(level, title, buf, len)\
	do{\
		if( level <= BOAT_LOG_LEVEL ){\
			printf( "%s: %s[%03d]: ", g_log_level_name_str[level-1], title, len );\
			if( len > 0 ){\
				for( int i = 0; i < len; i++ ){printf( "%c%c", "0123456789ABCDEF" [buf[i] / 16],"0123456789ABCDEF" [buf[i] % 16] );}\
			}else{\
				printf("(nil)");\
			}\
			printf( "\n" );\
		}\
	}while(0)
#endif
   
   
/*!****************************************************************************
 * @brief Log Output in hex and acsii mode
 *
 * @param level
 *   Log priority level. 
 *   One of BOAT_LOG_CRITICAL, BOAT_LOG_NORMAL or BOAT_LOG_VERBOSE.
 *
 * @param title
 *   tips of this dump
 *
 * @param buf
 *   buffer to be dump
 *
 * @param len
 *   dump len
 *
 * @see BoatLog BoatLog_hexdump
 ******************************************************************************/      
#if BOAT_LOG_LEVEL == BOAT_LOG_NONE
#define BoatLog_hexasciidump(level, title, buf, len)
#else
#define BoatLog_hexasciidump(level, title, buf, len)\
	   do{\
		   if( level <= BOAT_LOG_LEVEL ){\
		   printf( "%s: %s[%03d]: \n", g_log_level_name_str[level-1], title, len );\
			   if( len > 0 ){\
				   for( int j = 0; j < len / 16 + 1; j++ ){\
					   printf("%08x  ", j * 16);\
					   for( int i = 0; i < 16; i++ ){\
						   printf( "%c%c ",\
								   "0123456789ABCDEF " [(i + j * 16 < len) ? buf[i + j * 16] / 16 : 16],\
								   "0123456789ABCDEF " [(i + j * 16 < len) ? buf[i + j * 16] % 16 : 16] );}\
					   printf( " |" );\
					   for( int i = 0; i < 16; i++ ){\
						   if( i + j * 16 < len ){\
							   if( (buf[i + j * 16] > 32) && (buf[i + j * 16] < 127) ){\
								   printf( "%c", buf[ i + j * 16] );\
							   }else{\
								   printf( "." );\
							   }\
						   }else{\
						   	   printf( " " );\
						   }\
					   }\
					   printf( "|\n" );\
				   }\
		   }else{\
			   printf("(nil)");\
		   }\
		   printf( "\n" );\
	   }\
   }while(0)
   #endif

/*!****************************************************************************
 * @brief Round value up to the nearest multiple of step
 * 
 * @param value
 *   The value to round up
 *
 * @param step
 *   The step of round
 ******************************************************************************/
#define BOAT_ROUNDUP(value, step) ((((value) - 1)/(step) + 1) * (step))


/*!****************************************************************************
 * @brief Return the minimal value of two
 *
 * @param value1
 *   The first value to compare
 *
 * @param value2
 *   The second value to compare
 ******************************************************************************/
#define BOAT_MIN(value1, value2) ((value1) < (value2) ? (value1) : (value2))


/*!****************************************************************************
 * @brief Return the maximal value of two
 *
 * @param value1
 *   The first value to compare
 *
 * @param value2
 *   The second value to compare
 ******************************************************************************/
#define BOAT_MAX(value1, value2) ((value1) > (value2) ? (value1) : (value2))

#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Trim zeros of a binary stream
 * 
 * @details
 *   This function trims zeros of a binary stream and returns the length after
 *   trimming. Currently only left trimming (i.e. trimming leading zeros) is
 *   supported.
 *   \n This function simply ignores leading zeros and copy the first non-zero byte
 *   til the end of the stream to the target buffer. It doesn't treat the data
 *   in buffer as integer, i.e. it doesn't care about the endian.
 *   \n The source and target buffer may be overlapped.
 *   \n For example, {0x00, 0x01, 0x02, 0x00} is trimed to {0x01, 0x02, 0x00} and
 *   returns 3.
 *   
 * @param to_ptr
 *   A buffer to contain the trimmed binary stream. Its size must be at
 *   least <from_len> bytes. <to_ptr> and <from_ptr> may be overlapped.
 *       
 * @param from_ptr
 *   The binary stream to trim. <to_ptr> and <from_ptr> may be overlapped.
 *
 * @param from_len
 *   Length of <from_ptr>.
 *
 * @param trim_mode
 *   Only TRIMBIN_LEFTTRIM is supported, i.e. trim leading zeros.\n
 *   If TRIMBIN_TRIM_NO is specified, this function acts like memmove().
 *       
 * @param zero_as_null
 *   In case the binary stream contains only one or multiple 0x00, if\n
 *   <zero_as_null> is BOAT_TRUE, it will be treated as a null stream and\n
 *   returns 0.\n
 *   If <zero_as_null> is BOAT_FALSE, it will be trimed to a single 0x00 and\n
 *   returns 1.\n
 *   <zero_as_null> should be BOAT_TRUE for RLP-Encoding purpose.
 *
 * @return
 *   This function returns how many bytes the trimmed binary stream is.
 ******************************************************************************/
BUINT32 UtilityTrimBin( BOAT_OUT BUINT8 *to_ptr,
						const BUINT8 *from_ptr,
						BUINT32 from_len,
						TRIMBIN_TRIM_MODE trim_mode,
						BBOOL zero_as_null );


/*!****************************************************************************
 * @brief 
 *   Convert a binary stream to HEX string with optional leading zeros trimming 
 *   and "0x" prefix
 * 
 * @details
 *   This function converts a binary stream to a null-terminated HEX string.
 *   Optionally leading zeros can be trimmed during conversion.
 *   Optionally "0x" prefix can be prepended to the HEX string.
 *   \n There is no space to delimit every HEX code.
 *   The case of "a" through "f" is always lower case.  
 *   \n There are 3 ways to trim leading zeros. For example, a binary stream
 *   {0x00, 0x01, 0x00, 0xAB} will be converted to string:   
 *   "000100ab": if <trim_mode> = BIN2HEX_TRIM_NO
 *   "100ab"   : if <trim_mode> = BIN2HEX_LEFTTRIM_QUANTITY
 *   "0100ab"  : if <trim_mode> = BIN2HEX_LEFTTRIM_UFMTDATA
 *
 * @note
 *   This function doesn't treat the binary stream as an integer and thus
 *    it doens't make any endian conversion.
 *
 * @param[out] to_str
 *   The buffer to hold the converted HEX string.\n
 *   Its size should be least <from_len>*2+1 bytes if <prefix_0x_mode>=BIN2HEX_PREFIX_0x_NO,\n
 *   or <from_len>*2+3 bytes if <prefix_0x_mode>=BIN2HEX_PREFIX_0x_YES.\n
 *   If <to_str> is NULL, this function does nothing and returns 0.
 *       
 * @param[in] from_ptr
 *   The binary stream to convert from.\n
 *   If <from_ptr> is NULL and <to_str> is NOT NULL, this function writes a\n
 *   '\0' to <to_str>, i.e. a NULL string and returns 1.
 *
 * @param[in] from_len
 *   The length of <from_ptr> in byte.\n
 *   If <from_len> is 0 and <to_str> is NOT NULL, this function writes a\n
 *   '\0' to <to_str>, i.e. a NULL string and returns 1.
 *
 * @param[in] trim_mode
 *   BIN2HEX_TRIM_NO if no trimming is required;\n
 *   BIN2HEX_LEFTTRIM_QUANTITY to trim as a quantity;\n
 *   BIN2HEX_LEFTTRIM_UFMTDATA to trim as unformatted data
 *
 * @param[in] prefix_0x_mode
 *   BIN2HEX_PREFIX_0x_YES: Prepend a "0x" prefix at the beginning of the HEX string;\n
 *   BIN2HEX_PREFIX_0x_NO:  Don't prepend "0x" prefix.
 *       
 * @param[in] zero_as_null
 *   In case the binary stream contains only one or multiple 0x00, if\n
 *   <zero_as_null> is BOAT_TRUE, it will be converted to a null sting and\n
 *   returns 0.\n
 *   If <zero_as_null> is BOAT_FALSE, it will be converted to "0" or "00"\n
 *   according to <trim_mode>.\n
 *   <zero_as_null> should be BOAT_TRUE for RLP-Encoding purpose.
 *
 * @return
 *   This function returns the length of the converted HEX string excluding NULL\n
 *   terminator. It equals to strlen of the string.\n
 *   If <to_str> is NULL, this function does nothing and returns 0.\n
 *   If <from_len> is 0 or <from_ptr> is NULL, and <to_str> is NOT NULL, this
 *   function writes a '\0' to <to_str>, i.e. a NULL string and returns 0.
 ******************************************************************************/
BUINT32 UtilityBin2Hex( BOAT_OUT BCHAR *to_str,
						const BUINT8 *from_ptr,
						BUINT32 from_len,
						BIN2HEX_TRIM_MODE trim_mode,
						BIN2HEX_PREFIX_0x_MODE prefix_0x_mode,
						BBOOL zero_as_null );


/*!****************************************************************************
 * @brief Convert a HEX string to binary stream with optional leading zeros trimming
 * 
 * @details
 *   This function converts a HEX string to a binary stream.
 *   \n Optionally leading zeros can be trimmed during conversion.
 *   \n If there is "0x" prefix at the beginning of the HEX string, it's ignored.
 *   \n There should be no space between HEX codes.
 *   \n If <to_size> is too small to hold the converted binary stream, only the
 *   beginning <to_size> bytes are converted and written to <to_ptr>.
 *   \n The trim_mode is the same as UtilityTrimBin(). Odd length of HEX string is
 *   allowed as if it were left filled with a "0". For example, a HEX string
 *   "0x00123ab" is treated as "0x000123ab" and converted to:
 *   {0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_LEFTTRIM;
 *   {0x00, 0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_TRIM_NO
 *
 * @note
 *   This function doesn't treat the HEX string as an integer and thus
 *   it doens't make any endian conversion.
 *
 * @param[out] to_ptr
 *   The buffer to hold the converted binary stream.
 *
 * @param[in] to_size
 *   The size of <to_ptr> in bytes.\n
 *   It's safe to ensure <to_size> >= (strlen(from_str)+1)/2 bytes.\n
 *       
 * @param[in] from_str
 *   The null-terminated HEX string to convert from. "0x" prefix is optional\n
 *   and ignored. There should be no space between HEX codes.
 *       
 * @param[in] trim_mode
 *   TRIMBIN_LEFTTRIM: Trim leading zeros;\n
 *   TRIMBIN_TRIM_NO:  Don't trim.
 *
 * @param[in] zero_as_null
 *   In case the HEX string contains only one or multiple "00" and <trim_mode>\n
 *   is TRIMBIN_LEFTTRIM, if <zero_as_null> is BOAT_TRUE, nothing will be\n
 *   written to <to_ptr> and it returns 0.\n
 *   If <zero_as_null> is BOAT_FALSE, it will be converted to a single 0x00\n
 *   and returns 1.\n
 *   <zero_as_null> should be BOAT_TRUE for RLP-Encoding purpose.
 *
 * @return
 *   This function returns how many bytes the converted binary stream is if the\n
 *   conversion completes successfully. If the output buffer is too small to\n
 *   hold the converted binary stream, it returns <to_size>.\n
 *   If any non-HEX character is encountered, or <from_str> is an all-zero HEX\n
 *   string and <trim_mode> is TRIMBIN_LEFTTRIM, it returns 0.
 *	
 * @see UtilityTrimBin()
*******************************************************************************/
BUINT32 UtilityHex2Bin( BOAT_OUT BUINT8 *to_ptr,
                        BUINT32 to_size,
                        const BCHAR *from_str,
                        TRIMBIN_TRIM_MODE trim_mode,
                        BBOOL zero_as_null );


/*!****************************************************************************
 * @brief Convert a host endian BUINT32 to bigendian with optional MSB zeros trimming
 * 
 * @details
 *   This function converts a host endian (typically littleendian) BUINT32 integer
 *   to bigendian.
 *   \n Optionally most siganificant zeros can be trimmed during conversion.
 *   \n The trim_mode is the same as UtilityTrimBin(). For example, a BUINT32 integer
 *   0x000123ab is converted to:
 *   {0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_LEFTTRIM;
 *   {0x00, 0x01, 0x23, 0xab}: if <trim_mode> = TRIMBIN_TRIM_NO
 *
 * @param[out] to_big_ptr
 *   The buffer to hold the converted binary stream.
 *       
 * @param[in] from_host_integer
 *   The host endian BUINT32 integer to convert.
 *
 * @param[in] trim_mode
 *   TRIMBIN_LEFTTRIM: Trim MSB zeros;\n
 *   TRIMBIN_TRIM_NO:  Don't trim.
 *
 * @return
 *   This function returns how many bytes the converted bigendian integer is.\n
 *   If <trim_mode> = TRIMBIN_TRIM_NO, it always returns 4.
 * @see UtilityTrimBin()
 ******************************************************************************/
BUINT8 UtilityUint32ToBigend( BOAT_OUT BUINT8 *to_big_ptr,
							  BUINT32 from_host_integer,
							  TRIMBIN_TRIM_MODE trim_mode );


/*!****************************************************************************
 * @brief Convert a host endian BUINT64 to bigendian with optional MSB zeros trimming
 * 
 * @details
 *   This function converts a host endian (typical littleendian) BUINT64 integer
 *   to bigendian.
 *   \n Optionally most siganificant zeros can be trimmed during conversion.
 *   \n It's a 64-bit version of UtilityUint32ToBigend(). 
 *   
 * @param[out] to_big_ptr
 *   The buffer to hold the converted binary stream.
 *       
 * @param[in] from_host_integer
 *   The host endian BUINT64 integer to convert.
 *
 * @param[in] trim_mode
 *   TRIMBIN_LEFTTRIM: Trim MSB zeros;\n
 *   TRIMBIN_TRIM_NO:  Don't trim.
 *   
 * @return
 *   This function returns how many bytes the converted bigendian integer is.\n
 *   If <trim_mode> = TRIMBIN_TRIM_NO, it always returns 8.
 *
 * @see UtilityUint32ToBigend()  UtilityTrimBin()
*******************************************************************************/
BUINT8 UtilityUint64ToBigend( BOAT_OUT BUINT8 *to_big_ptr,
							  BUINT64 from_host_integer,
							  TRIMBIN_TRIM_MODE trim_mode );


/*!****************************************************************************
 * @brief Change endianness
 * 
 * @details
 *   This function changes a value's endianness. It simply swaps Byte[0] and
 *   Byte[n-1], Byte[1] and Byte[n-2]...
 *
 * @param[inout] value_ptr
 *   The value to change endianness.
 *       
 * @param[in] value_len
 *   Size (in byte) of <value_ptr>.
 *  
 * @return
 *   This function returns the pointer of the converted value.
 *   If <value_ptr> is NULL, it returns NULL.  
 ******************************************************************************/
void * UtilityChangeEndian(BOAT_INOUT void *value_ptr, BUINT32 value_len);


/*!*****************************************************************************
 * @brief Convert a host endian BUINT32 to bigendian
 * 
 * @details
 *   This function converts a host endian (typically littleendian) BUINT32 integer
 *   to bigendian.
 *
 * @param[in] from_host_integer
 *   The host endian BUINT32 integer to convert.
 *
 * @return
 *   This function returns the converted bigendian BUINT32 integer.
 * 	
 * @see UtilityUint32ToBigend()
 ******************************************************************************/
BUINT32 Utilityhtonl(BUINT32 from_host_integer);


/*!****************************************************************************
 * @brief Convert a bigendian BUINT32 to host endian
 * 
 * @details
 *   This function converts a bigendian BUINT32 integer to host endian 
 *   (typically littleendian).
 * 
 * @param[in] from_big_integer
 *   The big endian BUINT32 integer to convert.
 *
 * @return
 *   This function returns the converted host endian BUINT32 integer.
 *
 * @see Utilityhtonl()
 ******************************************************************************/
BUINT32 Utilityntohl(BUINT32 from_host_integer);


/*!****************************************************************************
 * @brief Convert a string representing wei in HEX to ether in double float
 * 
 * @details
 *   This function converts a string representing wei in HEX to ether in double
 *   float.
 *   \n 1 ether is 1e18 wei. Note that in Ethereum an integer type is up to 256 bits
 *   while most C compilers natively support up to 64-bit integer. A 64-bit
 *   unsigned interger can represent up to 2^64 - 1, which is roughly 1.845e19.
 *   Thus it's not possible to convert more than 1.845e19 wei (i.e. 18.45 ether)
 *   to native integer type.
 *   \n Converting integer type wei to double float type ether loses pricision.
 *   Fortunately the unit of ether is usually for friendly human-reading only
 *   and slight pricision loss is not a problem.
 *
 * @param[in] wei_str
 *   The HEX-represented string of wei, either in "0xabcd..." or "abcd..." format.
 *
 * @return
 *   This function returns the converted ether in double float.
*******************************************************************************/
double UtilityWeiStrToEthDouble(const BCHAR *wei_str);


/*!****************************************************************************
* @brief Convert a 32 bytes binary data to big number format
* @details
*    Input 32 bytes binary data should be big endian. for example:
*    - 0x12       should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x00, 0x12.
*    - 0x1234     should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x12, 0x34.
*    - 0x12345678 should stored as 0x00, 0x00, ..., 0x12, 0x34, 0x56, 0x78.
* param[in] in_number
*    Input data pointer, It's should be a 32 bytes length array.
* param[out] out_number
*    The pointer to stored converted data in big number format.
* @see Utility_writeBignumToBigend
*******************************************************************************/
void Utility_readBigendToBignum(const BUINT8 *in_number, bignum256 *out_number);


/*!****************************************************************************
* @brief Convert a 32 bytes binary data to big number format
* @details
*    Output 32 bytes binary data is  big endian format. for example:
*    - 0x12       should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x00, 0x12.
*    - 0x1234     should stored as 0x00, 0x00, ..., 0x00, 0x00, 0x12, 0x34.
*    - 0x12345678 should stored as 0x00, 0x00, ..., 0x12, 0x34, 0x56, 0x78.
* param[in] in_number
*    Input data pointer, It's in big number format.
* param[out] out_number
*    output data pointer, It's should be a 32 bytes length array.
* @see Utility_readBigendToBignum
*******************************************************************************/
void Utility_writeBignumToBigend(const bignum256 *in_number, BUINT8 *out_number);

/*!****************************************************************************
 * @brief 
 *   hash generate function.
 * 
 * @details
 *   hash generate function.
 *
 * @param type 
 *	 the compute hash type.
 *   \n - #BOAT_HASH_SHA256 compute SHA256.
 *   \n - #BOAT_HASH_KECCAK256 compute KECCAK256.
 *
 * @param[in] input
 *   the origin data of hash computation.
 *
 * @param[in] inputLen 
 *   the origin data length of hash computation.
 *
 * @param[out] hashed 
 *   compute hash result. Caller should provide enough space to store it.
 *
 * @param[out] hashedLen 
 *   computed hash length. If Don't need this param, set it NULL.
 *
 * @param rsvd
 *   reserved for futrue. 
 *
 * @return BOAT_RESULT 
 *   return BOAT_SUCCESS if generate success; otherwise return a negative error code
 ******************************************************************************/
BOAT_RESULT  BoatHash( const BoatHashAlgType type, const BUINT8* input, BUINT32 inputLen, 
				       BUINT8* hashed, BUINT8* hashedLen, void* rsvd );

/*!****************************************************************************
 * @brief  Wrapper function for memory allocation
 * 
 * @details
 *   This function is a wrapper for dynamic memory allocation.
 *   \n It typically wraps malloc() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 * @param[in] size 
 *   How many bytes to allocate.
 * @return 
 *   This function returns the address of the allocated memory. If allocation\n
 *  fails, it returns NULL.
 ******************************************************************************/
void   *BoatMalloc(BUINT32 size);


/*!****************************************************************************
 * @brief Wrapper function for memory allocation
 *
 * @details
 *   This function is a wrapper for dynamic memory de-allocation.
 *   \n It typically wraps free() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 * @param[in] mem_ptr
 *   The address to free. The address must be the one returned by BoatMalloc().
 ******************************************************************************/
void    BoatFree(void *mem_ptr);


/*!****************************************************************************
 * @brief Wrapper function for sleep (thread suspension)
 *
 * @details
 *   This function is a wrapper for sleep (thread suspension).
 *   \n It typically wraps sleep() or usleep() in a linux or Windows system.
 *   \n For RTOS it depends on the specification of the RTOS.
 * 
 * @param[in] second
 *   How many seconds to sleep.
 ******************************************************************************/
void    BoatSleep(BUINT32 second);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif