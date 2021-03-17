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

/*!@brief Define basic types for BoAT IoT SDK.

@file
boattypes.h defines types used in BoAT IoT SDK.
*/

#ifndef __BOATTYPES_H__
#define __BOATTYPES_H__


#define BOAT_OUT
#define BOAT_IN
#define BOAT_INOUT

#define BOAT_TRUE 1
#define BOAT_FALSE 0

#define __BOATSTATIC static
#define __BOATINLINE inline

//!@brief Basic Types, prefixed with "B" to avoid possible conflict with customer's typedef
typedef bool BBOOL;
typedef char BCHAR;
typedef unsigned char BUINT8;
typedef unsigned short BUINT16;
typedef unsigned int BUINT32;
typedef unsigned long long int BUINT64;
typedef signed char BSINT8;
typedef signed short BSINT16;
typedef signed int BSINT32;
typedef signed long long int BSINT64;

typedef BSINT32 BOAT_RESULT;
typedef BUINT8 BoatAddress[20];


//!@brief Blockchain Protocol types
typedef enum
{
    BOAT_PROTOCOL_UNKNOWN = 0,     //!< Placeholder for unknown protocol
    BOAT_PROTOCOL_ETHEREUM,        //!< Ethereum
    BOAT_PROTOCOL_HLFABRIC,        //!< HyperLedger Fabric
    BOAT_PROTOCOL_PLATON,          //!< PlatON public chain
    BOAT_PROTOCOL_PLATONE,         //!< PlatONE Enterprise consortium chain
	BOAT_PROTOCOL_FISCOBCOS        //!< FISCOBCOS Enterprise consortium chain
}BoatProtocolType;

//!@brief Blockchain signature algorithm
typedef enum
{
    BOAT_SIGNATURE_UNKNOWN = 0,    //!< Placeholder for unknown signature algorithm
	BOAT_SIGNATURE_SECP256K1,      //!< secp256k1 signature algorithm
	BOAT_SIGNATURE_SECP256R1       //!< secp256r1 signature algorithm
}BoatSignatureAlgType;

//!@brief Blockchain hash algorithm
typedef enum
{
    BOAT_HASH_UNKNOWN = 0,    //!< Placeholder for unknown hash algorithm
	BOAT_HASH_KECCAK256,      //!< keccak256 hash algorithm
	BOAT_HASH_SHA256          //!< sha256 hash algorithm
}BoatHashAlgType;

//!@brief common struct for variable length fields
typedef struct TBoatFieldVariable
{
    BUINT8 *field_ptr;  //!< The address of the field storage. The storage MUST be allocated before use.
    BUINT32 field_len;  //!< The length of the field in byte
}BoatFieldVariable;

//!@brief common struct for 4-byte (32-bit) length fields
typedef struct TBoatFieldMax4B
{
    BUINT8 field[4];     //!< Field storage
    BUINT32 field_len;   //!< The effective length of the field in byte
}BoatFieldMax4B;

//!@brief common struct for 32-byte (256-bit) length transaction fields
typedef struct TBoatFieldMax32B
{
    BUINT8 field[32];    //!< Field storage
    BUINT32 field_len;   //!< The effective length of the field in byte
}BoatFieldMax32B;

//!@brief common struct for 8-byte (64-bit) length transaction fields
typedef struct TBoatFieldMax8B
{
    BUINT8 field[8];     //!< Field storage
    BUINT32 field_len;   //!< The effective length of the field in byte
}BoatFieldMax8B;      

//!@brief common struct for 24-byte (192-bit) length transaction fields
typedef struct TBoatFieldMax24B
{
    BUINT8 field[24];     //!< Field storage
    BUINT32 field_len;    //!< The effective length of the field in byte
}BoatFieldMax24B;  

#endif
