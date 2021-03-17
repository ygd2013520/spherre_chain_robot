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

/*!@brief The wrapper header file for 3rd application to include.

@file
boatiotsdk.h is the wrapper header file for 3rd application to include.
*/

#ifndef __BOATIOTSDK_H__
#define __BOATIOTSDK_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "boatoptions.h"
#include "boaterrcode.h"
#include "boattypes.h"
#include "boatexception.h"
#include "boatutility.h"
#include "boatrlp.h"
#if PROTOCOL_USE_ETHEREUM == 1
#include "protocolapi/api_ethereum.h"
#endif

#if PROTOCOL_USE_HLFABRIC == 1
#include "protocolapi/api_hlfabric.h"
#endif

#if PROTOCOL_USE_PLATON == 1
#include "protocolapi/api_platon.h"
#endif

#if PROTOCOL_USE_PLATONE == 1
#include "protocolapi/api_platone.h"
#endif
#if PROTOCOL_USE_FISCOBCOS == 1
#include "protocolapi/api_fiscobcos.h"
#endif

#include "boatwallet.h"


#endif
