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

/**@file tsip_header_Accept_Contact.h
 * @brief SIP header 'Accept-Contact'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_ACCEPT_CONTACT_H_
#define _TSIP_HEADER_ACCEPT_CONTACT_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  SIP header 'Accept-Contact' as per RFC 3261 subclause .
/// @author Mamadou
/// @date   12/3/2009
///
/// @par ABNF
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Accept_Contact_s {
    TSIP_DECLARE_HEADER;
}
tsip_header_Accept_Contact_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_ACCEPT_CONTACT_H_ */

