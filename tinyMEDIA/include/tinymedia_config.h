/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

#ifndef TINYMEDIA_CONFIG_H
#define TINYMEDIA_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#   define TMEDIA_UNDER_WINDOWS 1
#   if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#       define TMEDIA_UNDER_WINDOWS_RT      1
#   endif
#endif

// OS X or iOS
#if defined(__APPLE__)
#   define TMEDIA_UNDER_APPLE               1
#   include <TargetConditionals.h>
#   include <Availability.h>
#endif
#if TARGET_OS_MAC
#   define TMEDIA_UNDER_MAC             1
#endif
#if TARGET_OS_IPHONE
#   define TMEDIA_UNDER_IPHONE          1
#endif
#if TARGET_IPHONE_SIMULATOR
#   define TMEDIA_UNDER_IPHONE_SIMULATOR    1
#endif

#if (TMEDIA_UNDER_WINDOWS || defined(__SYMBIAN32__))
#   define TINYMEDIA_API        __declspec(dllexport)
#   define TINYMEDIA_GEXTERN extern __declspec(dllexport)
#else
#   define TINYMEDIA_API
#   define TINYMEDIA_GEXTERN    extern
#endif

/* Guards against C++ name mangling
*/
#ifdef __cplusplus
#   define TMEDIA_BEGIN_DECLS extern "C" {
#   define TMEDIA_END_DECLS }
#else
#   define TMEDIA_BEGIN_DECLS
#   define TMEDIA_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#   if !defined(_CRT_SECURE_NO_WARNINGS)
#       define _CRT_SECURE_NO_WARNINGS
#   endif /* _CRT_SECURE_NO_WARNINGS */
#endif

/* Detecting C99 compilers
 */
#if (__STDC_VERSION__ == 199901L) && !defined(__C99__)
#   define __C99__
#endif

#include <stdint.h>
#ifdef __SYMBIAN32__
#include <stdlib.h>
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif

#endif // TINYMEDIA_CONFIG_H
