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
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "tsk.h"

#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
#   define strdup _strdup
#endif


#define RUN_TEST_ALL                0
#define RUN_TEST_LISTS              1
#define RUN_TEST_HEAP               1
#define RUN_TEST_STRINGS            1
#define RUN_TEST_URL                1
#define RUN_TEST_THREADS            0
#define RUN_TEST_MUTEX              0
#define RUN_TEST_CONDWAIT           0
#define RUN_TEST_SEMAPHORE          0
#define RUN_TEST_SAFEOBJECT         0
#define RUN_TEST_OBJECT             0
#define RUN_TEST_PARAMS             1
#define RUN_TEST_OPTIONS            1
#define RUN_TEST_TIMER              0
#define RUN_TEST_RUNNABLE           0
#define RUN_TEST_BUFFER             1
#define RUN_TEST_MD5                1
#define RUN_TEST_SHA1               1
#define RUN_TEST_BASE64             1
#define RUN_TEST_UUID               1
#define RUN_TEST_FSM                1

#if RUN_TEST_LISTS || RUN_TEST_ALL
#include "test_lists.h"
#endif

#if RUN_TEST_HEAP || RUN_TEST_ALL
#include "test_heap.h"
#endif

#if RUN_TEST_STRINGS || RUN_TEST_ALL
#include "test_strings.h"
#endif

#if RUN_TEST_URL || RUN_TEST_ALL
#include "test_url.h"
#endif

#if RUN_TEST_THREADS || RUN_TEST_ALL
#include "test_threads.h"
#endif

#if RUN_TEST_MUTEX || RUN_TEST_ALL
#include "test_mutex.h"
#endif

#if RUN_TEST_CONDWAIT || RUN_TEST_ALL
#include "test_condwait.h"
#endif

#if RUN_TEST_SEMAPHORE || RUN_TEST_ALL
#include "test_semaphore.h"
#endif

#if RUN_TEST_SAFEOBJECT || RUN_TEST_ALL
#include "test_safeobject.h"
#endif

#if RUN_TEST_OBJECT || RUN_TEST_ALL
#include "test_object.h"
#endif

#if RUN_TEST_PARAMS || RUN_TEST_ALL
#include "test_params.h"
#endif

#if RUN_TEST_OPTIONS || RUN_TEST_ALL
#include "test_options.h"
#endif

#if RUN_TEST_TIMER || RUN_TEST_ALL
#include "test_timer.h"
#endif

#if RUN_TEST_RUNNABLE || RUN_TEST_ALL
#include "test_runnable.h"
#endif

#if RUN_TEST_BUFFER || RUN_TEST_ALL
#include "test_buffer.h"
#endif

#if RUN_TEST_MD5 || RUN_TEST_ALL
#include "test_md5.h"
#endif

#if RUN_TEST_SHA1 || RUN_TEST_ALL
#include "test_sha1.h"
#endif

#if RUN_TEST_BASE64 || RUN_TEST_ALL
#include "test_base64.h"
#endif

#if RUN_TEST_UUID || RUN_TEST_ALL
#include "test_uuid.h"
#endif

#if RUN_TEST_FSM || RUN_TEST_ALL
#include "test_fsm.h"
#endif


#ifdef _WIN32_WCE
int _tmain(int argc, _TCHAR* argv[])
#else
int main()
#endif
{
#if RUN_TEST_LISTS || RUN_TEST_ALL
    test_basic_list();
    test_complex_list();
    test_filtered_list();
#endif

#if RUN_TEST_HEAP || RUN_TEST_ALL
    test_heap();
#endif

#if RUN_TEST_STRINGS || RUN_TEST_ALL
    test_strings();
#endif

#if RUN_TEST_URL || RUN_TEST_ALL
    test_url();
#endif

#if RUN_TEST_THREADS || RUN_TEST_ALL
    test_threads();
#endif

#if RUN_TEST_MUTEX || RUN_TEST_ALL
    test_mutex();
#endif

#if RUN_TEST_CONDWAIT || RUN_TEST_ALL
    test_condwait();
#endif

#if RUN_TEST_SEMAPHORE || RUN_TEST_ALL
    test_semaphore();
#endif

#if RUN_TEST_SAFEOBJECT || RUN_TEST_ALL
    test_safeobject();
#endif

#if RUN_TEST_OBJECT || RUN_TEST_ALL
    test_object();
#endif

#if RUN_TEST_PARAMS || RUN_TEST_ALL
    test_params();
#endif

#if RUN_TEST_OPTIONS || RUN_TEST_ALL
    test_options();
#endif

#if RUN_TEST_TIMER || RUN_TEST_ALL
    test_timer();
#endif

#if RUN_TEST_RUNNABLE || RUN_TEST_ALL
    test_runnable();
#endif


#if RUN_TEST_BUFFER || RUN_TEST_ALL
    test_buffer();
#endif


#if RUN_TEST_MD5 || RUN_TEST_ALL
    test_md5();
    test_hmac_md5();
#endif

#if RUN_TEST_SHA1 || RUN_TEST_ALL
    test_sha1();
    test_hmac_sha1();
#endif

#if RUN_TEST_BASE64 || RUN_TEST_ALL
    test_base64();
#endif

#if RUN_TEST_UUID || RUN_TEST_ALL
    test_uuid();
#endif

#if RUN_TEST_FSM || RUN_TEST_ALL
    test_fsm();
#endif

    return 0;
}
