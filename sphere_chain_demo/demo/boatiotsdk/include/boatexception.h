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

/*!@brief Error catching macros

@file
boatexception.h defines error catching mechanism.
*/

#ifndef __BOATEXCEPTION_H__
#define __BOATEXCEPTION_H__

//!@{
//!@brief Declare variables used for try...throw...catch
#define boat_try_declare BOAT_RESULT _boat_result, boat_exception = BOAT_SUCCESS

/* boat_try is a limited implementation of try directive that only allows for
trying a single function call with return value of type BOAT_RESULT. If the
function returns BOAT_SUCCESS, it does nothing. Otherwise an exception is thrown.
The boat_catch directive must specify "cleanup" as its func_catch_point for use
with boat_try.

The boat_try directive can be used to simplify the codes that normally check
the function return value (type BOAT_RESULT) and omit following codes if any
of the tried function returns other than BOAT_SUCCESS.
*/
//!@brief A limited implementation of try directive
//!@param foo Any function calling, with return value of type BOAT_RESULT
#define boat_try(foo)\
{\
    _boat_result=(foo);\
    if( _boat_result != BOAT_SUCCESS ) boat_throw(_boat_result, cleanup)\
}


//!@brief Throw an exception
//!@param e Error code.
//!@param func_catch_point The catch block name to catch the exception.
#define boat_throw(e, func_catch_point) {boat_exception = e; goto func_catch_point;}

//!@brief Catch an exception
//!@param func_catch_point The catch block name matching the one passed to boat_throw().
#define boat_catch(func_catch_point) func_catch_point: _boat_result = 0; if( (boat_exception+_boat_result) != BOAT_SUCCESS )
//!@}





#endif
