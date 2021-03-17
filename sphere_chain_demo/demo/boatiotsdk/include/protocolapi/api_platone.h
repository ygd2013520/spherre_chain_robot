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

/*!@brief BoAT IoT SDK interface header file for PlatONE

@file
api_platone.h is header file for BoAT IoT SDK PlatONE's interface.
*/

#ifndef __API_PLATONE_H__
#define __API_PLATONE_H__

#include "boatiotsdk.h"
#include "api_ethereum.h"
#include "api_platon.h"

/*! @defgroup platone-api boat platone-API
 * @{
 */

#define BOAT_PLATONE_NONCE_AUTO                  BOAT_ETH_NONCE_AUTO
#define BOAT_PLATONE_ADDRESS_SIZE                BOAT_ETH_ADDRESS_SIZE

typedef BoatPlatonTxtype    BoatPlatoneTxtype;
typedef BoatEthAccountInfo  BoatPlatoneAccountInfo;
typedef BoatEthNetworkInfo  BoatPlatoneNetworkInfo;
typedef BoatEthWallet       BoatPlatoneWallet;
typedef BoatEthWalletConfig BoatPlatoneWalletConfig;
typedef BoatEthTxFieldSig   BoatPlatoneTxFieldSig;

//!@brief RAW PlatONE transaction fields

//! The only difference from Ethereum is appending a txtype field at the end of the
//! struct. Thus a BoatPlatoneRawtxFields pointer can be transfered to any function
//! that accepts a BoatEthRawtxFields argument. It behaves as a C implementation of
//! class inheritance, i.e. a pointer of an inherited class could be assigned to a
//! pointer of a base class.
typedef struct TBoatPlatoneRawtxFields
{
    // Following fields are inherited from Ethereum.
    // DO NOT modify these fields unless all of Ethereum and all other protocols
    // inherited from Ethereum are modified synchronously.
    BoatFieldMax32B nonce;        //!< nonce, uint256 in bigendian, equal to the transaction count of the sender's account address
    BoatFieldMax32B gasprice;     //!< gasprice in wei, uint256 in bigendian
    BoatFieldMax32B gaslimit;     //!< gaslimit, uint256 in bigendian
    BUINT8 recipient[BOAT_ETH_ADDRESS_SIZE]; //!< recipient's address, 160 bits
    BoatFieldMax32B value;        //!< value to transfer, uint256 in bigendian
    BoatFieldVariable data;       //!< data to transfer, unformatted stream
    BoatFieldMax4B v;             //!< chain id or recovery identifier, @see RawtxPerform()
    BoatEthTxFieldSig sig;        //!< ECDSA signature, including r and s parts
    
    // PlatONE specific fields are appended here.
    BoatPlatoneTxtype txtype;     //!< Transaction Type

}BoatPlatoneRawtxFields;

//!@brief PlatONE Transaction

//! The only difference between PlatONE transaction and Ethereum transaction is
//! BoatPlatoneRawtxFields has more fields than BoatEthRawtxFields. To allow
//! PlatONE to re-use Ethereum APIs that take BoatEthTx as function arguments,
//! <rawtx_fields> MUST be the last field.
typedef struct TBoatPlatoneTx
{
    BoatPlatoneWallet *wallet_ptr; //!< Wallet pointer the transaction is combined with
    BoatFieldMax32B tx_hash;       //!< Transaction hash returned from network
    BBOOL is_sync_tx;              //!< True to perform a synchronous transaction (wait for getting mined), False for asynchronous transaction

    // rawtx_fields MUST be the last field
    BoatPlatoneRawtxFields rawtx_fields;      //!< RAW transaction fields
}BoatPlatoneTx;


#ifdef __cplusplus
extern "C" {
#endif


/*!****************************************************************************
 * @brief Sign and send a transaction. Also call a stateful contract function.
 *
 * @details
 *   This function sign and set a transaction.
 *   \n BoatWalletSetXXX() and BoatTxSetXXX() functions must be properly called
 *   before call this function.
 *   \n A transaction whose recipient may be an EOA address or a contract address.
 *   In latter case it's usually a contract function call.
 *   \n This function invokes the eth_sendRawTransaction RPC method.
 *   eth_sendRawTransaction method only applies the transaction and returns a
 *   transaction hash. The transaction is not verified (got mined) until the
 *   nodes in the network get into consensus about the transaction. This
 *   function will invoke eth_getTransactionReceipt method to wait for the
 *   transaction being mined or timeout.
 *   \n If the transaction is a contract function call, the caller cannot get its
 *   return value because the transaction is asynchronously executed. It's a
 *   good practice to save the return value in a state variable and use
 *   BoatPlatoneCallContractFunc() to call a "read" contract function that could read
 *   and return the state variable.
 *
 * @note
 *   Any contract function that may change the state of the contract shall
 *   be called in a transaction way. "state" is the "global variable" used in a
 *   contract.
 *   \n Any contract function that doesn't change the state of the contract can
 *   be called either in a transaction way or by BoatPlatoneCallContractFunc(), which
 *   invokes the eth_call RPC method. However the former will consume gas and
 *   latter doesn't consume gas.
 *
 * @param[in] tx_ptr
 *   Pointer to the transaction structure.
 *   
 * @return
 *   This function returns BOAT_SUCCESS if setting is successful.\n
 *   Otherwise it returns one of the error codes.
 *
 * @see BoatPlatonTxSend()
 ******************************************************************************/
BOAT_RESULT BoatPlatoneTxSend(BoatPlatoneTx *tx_ptr);


/*!****************************************************************************
 * @brief Transfer PlatONE value to spceified recipient
 *
 * @details
 *   This function transfer PlatONE value from the wallet's owner account to the
 *   specified recipient account.
 *   \n Before calling this function, all necessary wallet fields such as private key,
 *   node URL and etc. must be set correctly.
 *
 * @param[in] tx_ptr
 *   Transaction pointer.
 *
 * @param[in] value_hex_str
 *   A string representing the value (Unit: wei) to transfer, in HEX format like\n
 *   "0x89AB3C".\n
 *   Note that decimal value is not accepted. If a decimal value such as "1234"\n
 *   is specified, it's treated as "0x1234".
 *
 * @return
 *   This function returns BOAT_SUCCESS if transfer is successful.\n
 *   Otherwise it returns one of the error codes.
 ******************************************************************************/
BOAT_RESULT BoatPlatoneTransfer(BoatPlatoneTx *tx_ptr, BCHAR * value_hex_str);


// Ethereum APIs compatible for PlatONE

/*!****************************************************************************
 * @brief Initialize Boat PlatONE Wallet
 * @see BoatEthWalletInit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BoatPlatonWallet * BoatPlatoneWalletInit(const BoatPlatoneWalletConfig *config_ptr, BUINT32 config_size)
{
    return BoatEthWalletInit((const BoatEthWalletConfig *) config_ptr, config_size);
}


/*!****************************************************************************
 * @brief De-initialize Boat PlatONE Wallet
 * @see BoatEthWalletDeInit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE void BoatPlatoneWalletDeInit(BoatPlatonWallet *wallet_ptr)
{
    return BoatEthWalletDeInit((BoatEthWallet *) wallet_ptr);
}


/*!****************************************************************************
 * @brief Set Node Url
 * @see BoatEthWalletSetNodeUrl()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneWalletSetNodeUrl(BoatPlatoneWallet *wallet_ptr, const BCHAR *node_url_ptr)
{
    return BoatEthWalletSetNodeUrl((BoatEthWallet *)wallet_ptr, node_url_ptr);
}


/*!****************************************************************************
 * @brief Set EIP155
 * @see BoatEthWalletSetEIP155Comp()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneWalletSetEIP155Comp(BoatPlatoneWallet *wallet_ptr, BBOOL eip155_compatibility)
{
    return BoatEthWalletSetEIP155Comp((BoatEthWallet *)wallet_ptr,eip155_compatibility);
}


/*!****************************************************************************
 * @brief Set ChainId
 * @see BoatEthWalletSetChainId()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneWalletSetChainId(BoatPlatoneWallet *wallet_ptr, BUINT32 chain_id)
{
    return BoatEthWalletSetChainId((BoatEthWallet *)wallet_ptr,chain_id);
}


/*!****************************************************************************
 * @brief Get Balance
 * @see BoatEthWalletGetBalance()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BCHAR * BoatPlatoneWalletGetBalance(BoatPlatoneWallet *wallet_ptr, BCHAR *alt_address_str)
{
    return BoatEthWalletGetBalance((BoatEthWallet *)wallet_ptr, alt_address_str);
}


/*!****************************************************************************
 * @brief Initialize a transaction
 * @see BoatPlatonTxInit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxInit(BoatPlatoneWallet *wallet_ptr,
													    BoatPlatoneTx *tx_ptr,
													    BBOOL is_sync_tx,
													    BCHAR * gasprice_str,
													    BCHAR * gaslimit_str,
													    BCHAR *recipient_str,
													    BoatPlatoneTxtype txtype)
{
    return BoatPlatonTxInit( (BoatPlatonWallet *)wallet_ptr,
                             (BoatPlatonTx *)tx_ptr,
                             is_sync_tx,
                             gasprice_str,
                             gaslimit_str,
                             recipient_str,
                             (BoatPlatonTxtype)txtype );
}
 

/*!****************************************************************************
 * @brief Set Nonce
 * @see BoatEthTxSetNonce()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetNonce(BoatPlatoneTx *tx_ptr, BUINT64 nonce)
{
    return BoatEthTxSetNonce((BoatEthTx *)tx_ptr, nonce);
}


/*!****************************************************************************
 * @brief Set GasPrice
 * @see BoatEthTxSetGasPrice()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetGasPrice(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *gas_price_ptr)
{
    return BoatEthTxSetGasPrice((BoatEthTx *)tx_ptr, gas_price_ptr);
}


/*!****************************************************************************
 * @brief Set GasLimit
 * @see BoatEthTxSetGasLimit()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetGasLimit(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *gas_limit_ptr)
{
    return BoatEthTxSetGasLimit((BoatEthTx *)tx_ptr, gas_limit_ptr);
}


/*!****************************************************************************
 * @brief Set Recipient
 * @see BoatEthTxSetRecipient()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetRecipient(BoatPlatoneTx *tx_ptr, BUINT8 address[BOAT_PLATONE_ADDRESS_SIZE])
{
    return BoatEthTxSetRecipient((BoatEthTx *)tx_ptr,address);
}


/*!****************************************************************************
 * @brief Set Value
 * @see BoatEthTxSetValue()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetValue(BoatPlatoneTx *tx_ptr, BoatFieldMax32B *value_ptr)
{
    return BoatEthTxSetValue((BoatEthTx *)tx_ptr,value_ptr);
}


/*!****************************************************************************
 * @brief Set Data
 * @see BoatEthTxSetData()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetData(BoatPlatoneTx *tx_ptr, BoatFieldVariable *data_ptr)
{
    return BoatEthTxSetData((BoatEthTx *)tx_ptr,data_ptr);
}


/*!****************************************************************************
 * @brief Set transaction type
 * @see BoatPlatonTxSetTxtype()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneTxSetTxtype(BoatPlatoneTx *tx_ptr, BoatPlatoneTxtype txtype)
{
    return BoatPlatonTxSetTxtype((BoatPlatonTx *)tx_ptr, (BoatPlatonTxtype) txtype);
}


/*!****************************************************************************
 * @brief Call a state-less contract function
 * @see BoatPlatonCallContractFunc()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BCHAR * BoatPlatoneCallContractFunc(BoatPlatoneTx *tx_ptr, BUINT8 *rlp_param_ptr, BUINT32 rlp_param_len)
{
    return BoatPlatonCallContractFunc((BoatPlatonTx *)tx_ptr, rlp_param_ptr, rlp_param_len);
}


/*!****************************************************************************
 * @brief Wait for a transaction being mined.
 * @see BoatEthGetTransactionReceipt()
 ******************************************************************************/
__BOATSTATIC __BOATINLINE BOAT_RESULT BoatPlatoneGetTransactionReceipt(BoatPlatoneTx *tx_ptr)
{
    return BoatEthGetTransactionReceipt((BoatEthTx *) tx_ptr);
}

/*! @}*/

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif
