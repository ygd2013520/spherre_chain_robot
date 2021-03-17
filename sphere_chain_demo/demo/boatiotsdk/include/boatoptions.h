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

/*!@brief Boatwallet options

@file
boatoptions.h defines options for compiling.
*/

#ifndef __BOATOPTIONS_H__
#define __BOATOPTIONS_H__

// BOAT LOG LEVEL DEFINITION
// Log level is used to control the detail of log output.
// 3 types of detail level can be specified in BoatLog():
// "CRITICAL" is for critical exceptions that may halt the wallet from going on.
// "NORMAL" is for warnings that may imply some error but could be held.
// "VERBOSE" is for detail information that is only useful for debug.
#define BOAT_LOG_NONE                            0
#define BOAT_LOG_CRITICAL                        1
#define BOAT_LOG_NORMAL                          2
#define BOAT_LOG_VERBOSE                         3

// BOAT_LOG_LEVEL is a macro that limits the log detail up to that level.
// Seting it to BOAT_LOG_NONE means outputing nothing.
#define BOAT_LOG_LEVEL   BOAT_LOG_VERBOSE

// RPC USE OPTION: One and only one RPC_USE option shall be set to 1
#define RPC_USE_LIBCURL                          1
#define RPC_USE_NOTHING                          0

#define RPC_USE_COUNT                            (RPC_USE_LIBCURL + RPC_USE_NOTHING)

#if (RPC_USE_COUNT != 1)
#error "One and only one RPC_USE option shall be set to 1"
#endif
#undef RPC_USE_COUNT


// Blockchain Protocol OPTION
#define PROTOCOL_USE_ETHEREUM   	             1
#define PROTOCOL_USE_HLFABRIC                    0
#define PROTOCOL_USE_PLATON     	             1
#define PROTOCOL_USE_PLATONE    	             1
#define PROTOCOL_USE_FISCOBCOS    	             1

// Protocol dependency
#if (PROTOCOL_USE_PLATON != 0)
#undef PROTOCOL_USE_ETHEREUM
#define PROTOCOL_USE_ETHEREUM                    1
#endif

#if PROTOCOL_USE_PLATONE != 0
#undef PROTOCOL_USE_ETHEREUM
#define PROTOCOL_USE_ETHEREUM                    1
#undef PROTOCOL_USE_PLATON
#define PROTOCOL_USE_PLATON                      1
#endif

#if (PROTOCOL_USE_FISCOBCOS != 0)
#undef PROTOCOL_USE_ETHEREUM
#define PROTOCOL_USE_ETHEREUM                    1
#endif

#define PROTOCOL_USE_COUNT                       (PROTOCOL_USE_ETHEREUM + \
												  PROTOCOL_USE_HLFABRIC + \
												  PROTOCOL_USE_PLATON   + \
												  PROTOCOL_USE_PLATONE  + \
												  PROTOCOL_USE_FISCOBCOS)

#if PROTOCOL_USE_COUNT == 0
#error "At least ONE PROTOCOL_USE option shall be set to 1"
#endif
#undef PROTOCOL_USE_COUNT


//! Mining interval and Pending transaction timeout
#define BOAT_MINE_INTERVAL                       3  //!< Mining Interval of the blockchain, in seconds
#define BOAT_WAIT_PENDING_TX_TIMEOUT             30 //!< Timeout waiting for a transaction being mined, in seconds


//! Maxmum length for node's URL
#define BOAT_NODE_URL_MAX_LEN                    127
#define BOAT_KEYID_MAX_LEN                       300
#define BOAT_FILENAME_MAX_LEN                    127

//! hyperledger fabric configuration items
#define HLFABRIC_ARGS_MAX_NUM                    4   //!< arguments max number in fabric command
#define HLFABRIC_ENDORSER_MAX_NUM                10  //!< support endorser max number
#define HLFABRIC_ORDERER_MAX_NUM                 4   //!< support orderer max number

#define HLFABRIC_TLS_SUPPORT                     1 //!< If need client support TLS, set it to 1.
#define HLFABRIC_TLS_IDENTIFY_CLIENT             1 //!< If server need identify client, set it to 1.
                                                   //!< this macro valid only HLFABRIC_TLS_SUPPORT has
                                                   //!< set to 1.
#define HLFABRIC_ROOTCA_MAX_NUM                  5 //!< support ROOT CA certificate max number

#define HTTP2_SEND_MAX_BUF                       4096

#endif /* end of __BOATOPTIONS_H__ */
