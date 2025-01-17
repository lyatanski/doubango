/*
* Copyright (C) 2009 Mamadou Diop.
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#include "tsk.h"

#include "tsdp.h"
#include "tinysdp/parsers/tsdp_parser_message.h"

#define RUN_TEST_LOOP       0

#define RUN_TEST_ALL        1
#define RUN_TEST_PARSER     1
#define RUN_TEST_SOA        1

#include "test_parser.h"
#include "test_soa.h"

#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
    do {
        test_parser();
#if RUN_TEST_ALL  || RUN_TEST_PARSER
        test_parser();
#endif

#if RUN_TEST_ALL  || RUN_TEST_SOA
        test_soa();
#endif

    }
    while(RUN_TEST_LOOP);
}
