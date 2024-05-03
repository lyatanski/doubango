/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#include "tinysms.h"

void printhex(const char* what, uint8_t* ptr, size_t size)
{
    size_t i;
    printf("%s", what);
    for(i=0; i<size; i++) {
        printf("%.2X", *ptr++);
    }
    printf("\n");
}

#include "test_packing.h"
#include "test_tpdu.h"
#include "test_rpdu.h"


#define RUN_TEST_LOOP       0

#define RUN_TEST_ALL        0
#define RUN_TEST_PACKING    1
#define RUN_TEST_TPDU       1
#define RUN_TEST_RPDU       1


int main()
{
    do {
#if RUN_TEST_ALL  || RUN_TEST_PACKING
        test_packing();
#endif

#if RUN_TEST_ALL  || RUN_TEST_TPDU
        test_tpdu();
#endif

#if RUN_TEST_ALL  || RUN_TEST_RPDU
        test_rpdu();
#endif
    }
    while(RUN_TEST_LOOP);
}

