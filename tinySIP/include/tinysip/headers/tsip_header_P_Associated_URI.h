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

/**@file tsip_header_P_Associated_URI.h
 * @brief SIP header 'P-Associated-URI'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_P_ASSOCIATED_URI_H_
#define _TSIP_HEADER_P_ASSOCIATED_URI_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

#include "tinysip/tsip_uri.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_P_ASSOCIATED_URI_VA_ARGS(uri)       tsip_header_P_Associated_URI_def_t, (const tsip_uri_t*)uri

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  SIP header 'P-Associated-URI' as per RFC 3455.
///
/// @par ABNF: P-Associated-URI =   "P-Associated-URI" HCOLON p-aso-uri-spec *(COMMA p-aso-uri-spec)
/// p-aso-uri-spec  =   name-addr *( SEMI ai-param )
/// ai-param    =   generic-param
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_P_Associated_URI_s {
    TSIP_DECLARE_HEADER;

    char *display_name;
    tsip_uri_t *uri;
}
tsip_header_P_Associated_URI_t;

typedef tsk_list_t tsip_header_P_Associated_URIs_L_t;

TINYSIP_API tsip_header_P_Associated_URI_t* tsip_header_P_Associated_URI_create(const tsip_uri_t* uri);
TINYSIP_API tsip_header_P_Associated_URI_t* tsip_header_P_Associated_URI_create_null();

TINYSIP_API tsip_header_P_Associated_URIs_L_t *tsip_header_P_Associated_URI_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_P_Associated_URI_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_P_ASSOCIATED_URI_H_ */

