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

/*!@brief Boatwallet SDK header file

@file
boatwallet.h is the SDK header file.
*/

#ifndef __BOATWALLET_H__
#define __BOATWALLET_H__

#include "boatiotsdk.h"

/*! @defgroup wallet boat wallet
 * @{
 */

typedef struct TBoatWalletList
{
    BBOOL is_used;
    BoatProtocolType protocol_type; //!< Blockchain protocol type
    BCHAR *wallet_name_str;         //!< Wallet name for persist wallet, NULL for one-time wallet
    void  *wallet_ptr;              //!< Wallet context of one of BoatWalletInfo type
}BoatWalletList;


#define BOAT_MAX_WALLET_NUM 5
//!@brief BoAT IoT SDK Context
typedef struct TBoatIotSdkContext
{
    // Protocol specifiec properties are defined in protocol specific WalletInfo structure
    BoatWalletList wallet_list[BOAT_MAX_WALLET_NUM];  //!< Wallet Info List
}BoatIotSdkContext;


#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Initialize Boat IoT SDK
 *
 * @details
 *   This function initialize global context of Boat IoT SDK.
 *   \n BoatIotSdkInit() MUST be called before any use of BoAT IoT SDK per process.
 *   BoatIotSdkDeInit() MUST be called after use of BoAT IoT SDK.
 *
 * @param This function doesn't take any argument.
 *
 * @return
 *   This function returns BOAT_SUCCESS if initialization is successful.\n
 *   Otherwise it returns BOAT_ERROR.
 * @see BoatIotSdkDeInit()
 ******************************************************************************/
BOAT_RESULT BoatIotSdktInit(void);


/*!****************************************************************************
 * @brief De-initialize BoAT IoT SDK
 *
 * @details
 *   This function de-initialize context of BoAT IoT SDK.
 *   \n BoatIotSdkInit() MUST be called before any use of BoAT IoT SDK per process.
 *   BoatIotSdkDeInit() MUST be called after use of BoAT IoT SDK.
 *   
 * @return This function doesn't return any thing.
 *
 * @see BoatIotSdkInit()
 ******************************************************************************/
void BoatIotSdkDeInit(void);


/*!****************************************************************************
 * @brief Create BoAT wallet
 *
 * @details
 *   This function creates a wallet or loads an existed persistent wallet.
 *   \n A wallet is a configuration for blockchain parameters as well as context for
 *   blockchain operations. There are 2 types of wallet. One is a one-time wallet,
 *   which only exists in RAM and disappears once the device is powered down. The
 *   other is a persistent wallet, which is saved in non-volatile memory for later
 *   loading.
 *   \n If the given <wallet_name_str> is NULL, a one-time wallet is created. Otherwise
 *   a persistent wallet with the given name will be created or loaded. If no wallet
 *   with the given name exists, a new persistent wallet will be created with the
 *   name and immediately loaded into RAM. If a persistent wallet with the given
 *   name exists, it will be loaded into RAM.
 *   \n To create a one-time wallet, <wallet_name_str> MUST be NULL and a non-NULL
 *   <wallet_config_ptr> MUST be specified.
 *   \n To create a persistent wallet, <wallet_name_str>, <wallet_config_ptr> and
 *   <wallet_config_size> MUST be specified. If a persitent wallet with the given
 *   <wallet_name_str> exists, the specified configuration will overwrite the
 *   existed one, both in RAM and in non-volatile memroy.
 *   \n To load an existed persistent wallet into RAM, specify its name in <wallet_name_str>,
 *   specify <wallet_config_ptr> = NULL and <wallet_config_size> = the configuration
 *   size of the specified <protocol_type>.  
 *   \n To unload a wallet from RAM, which was loaded by BoatWalletCreate(), call
 *   BoatWalletUnload(). For persisent wallet, this does NOT delete it from
 *   non-volatile memory.   
 *   \n To delete a persistent wallet from non-volatile memroy, call BoatWalletDelete().
 *   This does NOT unload it from RAM. If a loaded persistent wallet is deleted
 *   without unloading it, it becomes a one-time wallet.
 *
 * @param[in] protocol_type
 *   The blockchain protocol. See boattypes.h for supported protocol.
 *
 * @param[in] wallet_name_str
 *   A string of wallet name.\n
 *   If the given <wallet_name_str> is NULL, a one-time wallet is created.\n
 *   Otherwise a persistent wallet with the given name will be created or loaded.
 *
 * @param[in] wallet_config_ptr
 *   Configuration (e.g. crypto key) for the wallet.\n
 *   The exact configuration definition is determinted by the specified <protocol_type>.
 *	
 * @param[in] wallet_config_size
 *   Size (in byte) of configuration specified by <wallet_config_ptr>.
 *
 * @return
 *   This function returns the non-negative index of the loaded wallet.\n
 *   It returns -1 if wallet creation fails.
 *	
 * @see BoatWalletUnload() BoatWalletDelete()
 ******************************************************************************/
BSINT32 BoatWalletCreate( BoatProtocolType protocol_type, const BCHAR *wallet_name_str, 
						  const void * wallet_config_ptr, BUINT32 wallet_config_size );


/*!****************************************************************************
 * @brief Unload a wallet
 *
 * @details
 *   This function unloads a wallet from RAM if no more operations on this wallet
 *   is required. If the wallet is of persistent type, this function will NOT
 *   delete it from non-volatile memory. To delete a persistent wallet from
 *   non-volatile memory, call BoatWalletDelete().
 *
 * @param[in] wallet_index
 *   The wallet index to unload.
 *
 * @return 
 *   This function doesn't return any thing.
 *
 * @see BoatWalletCreate() BoatWalletDelete()
 ******************************************************************************/
void BoatWalletUnload(BSINT32 wallet_index);


/*!****************************************************************************
 * @brief Delete a persistent wallet
 *
 * @details
 *   This function deletes a persistent wallet from non-volatile memory. This
 *   does NOT unload it from RAM. If a loaded persistent wallet is deleted
 *   without unloading it, it becomes a one-time wallet.
 *
 * @param[in] wallet_name_str
 *   The wallet name to delete.
 *
 * @return
 *   This function doesn't return any thing.
 *
 * @see BoatWalletCreate() BoatWalletUnload()
 ******************************************************************************/
void BoatWalletDelete(BCHAR * wallet_name_str);


/*!****************************************************************************
 * @brief Get the BoAT wallet context by index.
 *
 * @details
 *   This function gets the BoAT wallet context by index.
 * 
 * @param[in] wallet_index
 *   The wallet index.
 *
 * @return This function returns a pointer to the wallet context.
 ******************************************************************************/
void * BoatGetWalletByIndex(BSINT32 wallet_index);

/*!****************************************************************************
 * @brief Get the BoAT prikey context.
 *
 * @details
 *   This function gets the BoAT prikey context.
 * 
 * @param[out] pri_key_ptr
 *   The prikey context.
 * @param[in] pri_key_len
 *   The prikey length.[223-256]
 *
 * @return
 *   This function returns BOAT_SUCCESS if get the prikey context successful.\n
 *   Otherwise it returns BOAT_ERROR.
 ******************************************************************************/
BOAT_RESULT BoatGetPrikey(BUINT8 *pri_key_ptr, BUINT32 pri_key_len);

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
