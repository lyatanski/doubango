/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
#include "DDebug.h"

#include "SipStack.h"

#include "Common.h"

enum cb_type {
    cb_info,
    cb_warn,
    cb_error,
    cb_fatal
};

int debug_xxx_cb(const void* arg, const char* fmt, enum cb_type type, va_list *app)
{
    int ret = -1;
    if(!arg) {
        return -1;
    }

    const DDebugCallback* stack = static_cast<const DDebugCallback*>(arg);

    if(stack) {
        char* message = tsk_null;
        tsk_sprintf_2(&message, fmt, app);

        switch(type) {
        case cb_info:
            ret = stack->OnDebugInfo(message);
            break;
        case cb_warn:
            ret = stack->OnDebugWarn(message);
            break;
        case cb_error:
            ret = stack->OnDebugError(message);
            break;
        case cb_fatal:
            ret = stack->OnDebugFatal(message);
            break;
        }

        TSK_FREE(message);
    }

    return ret;
}

DDebugCallback::DDebugCallback()
{
    tsk_debug_set_arg_data(this);
    tsk_debug_set_info_cb(DDebugCallback::debug_info_cb);
    tsk_debug_set_warn_cb(DDebugCallback::debug_warn_cb);
    tsk_debug_set_error_cb(DDebugCallback::debug_error_cb);
    tsk_debug_set_fatal_cb(DDebugCallback::debug_fatal_cb);
}

DDebugCallback::~DDebugCallback()
{
    tsk_debug_set_arg_data(tsk_null);
    tsk_debug_set_info_cb(tsk_null);
    tsk_debug_set_warn_cb(tsk_null);
    tsk_debug_set_error_cb(tsk_null);
    tsk_debug_set_fatal_cb(tsk_null);
}

int DDebugCallback::debug_info_cb(const void* arg, const char* fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = debug_xxx_cb(arg, fmt, cb_info, &ap);
    va_end(ap);

    return ret;
}

int DDebugCallback::debug_warn_cb(const void* arg, const char* fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = debug_xxx_cb(arg, fmt, cb_warn, &ap);
    va_end(ap);

    return ret;
}

int DDebugCallback::debug_error_cb(const void* arg, const char* fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = debug_xxx_cb(arg, fmt, cb_error, &ap);
    va_end(ap);

    return ret;
}

int DDebugCallback::debug_fatal_cb(const void* arg, const char* fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = debug_xxx_cb(arg, fmt, cb_fatal, &ap);
    va_end(ap);

    return ret;
}

