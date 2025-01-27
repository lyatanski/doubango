
/* #line 1 "./ragel/tsip_parser_header_Refer_To.rl" */

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

/**@file tsip_header_Refer_To.c
 * @brief SIP Refer-To header as per RFC 3515.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Refer_To.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*   Ragel state machine.
*/

/* #line 89 "./ragel/tsip_parser_header_Refer_To.rl" */



tsip_header_Refer_To_t* tsip_header_Refer_To_create(const tsip_uri_t* uri)
{
    return tsk_object_new(TSIP_HEADER_REFER_TO_VA_ARGS(uri));
}

tsip_header_Refer_To_t* tsip_header_Refer_To_create_null()
{
    return tsip_header_Refer_To_create(tsk_null);
}

int tsip_header_Refer_To_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
    if(header) {
        int ret;
        const tsip_header_Refer_To_t *Refer_To = (const tsip_header_Refer_To_t *)header;

        /* Uri with hacked display-name*/
        if((ret = tsip_uri_serialize(Refer_To->uri, tsk_true, tsk_true, output))) {
            return ret;
        }
        return ret;
    }
    return -1;
}

tsip_header_Refer_To_t *tsip_header_Refer_To_parse(const char *data, tsk_size_t size)
{
    int cs = 0;
    const char *p = data;
    const char *pe = p + size;
    const char *eof = pe;
    tsip_header_Refer_To_t *r_to = tsip_header_Refer_To_create_null();

    const char *tag_start = tsk_null;


    /* #line 87 "./src/headers/tsip_header_Refer_To.c" */
    static const char _tsip_machine_parser_header_Refer_To_actions[] = {
        0, 1, 0, 1, 1, 1, 2, 1,
        3, 1, 4, 2, 1, 3
    };

    static const short _tsip_machine_parser_header_Refer_To_key_offsets[] = {
        0, 0, 2, 7, 10, 31, 32, 34,
        55, 56, 58, 61, 65, 77, 80, 80,
        81, 85, 86, 103, 104, 106, 122, 141,
        146, 147, 149, 153, 172, 173, 175, 194,
        195, 197, 200, 208, 209, 211, 215, 219,
        220, 222, 225, 231, 249, 256, 264, 272,
        280, 282, 289, 298, 300, 303, 305, 308,
        310, 313, 316, 317, 320, 321, 324, 325,
        334, 343, 351, 359, 367, 375, 377, 383,
        392, 401, 410, 412, 415, 418, 419, 420,
        437, 455, 459, 460, 462, 470, 471, 473,
        477, 483, 495, 498, 498, 502, 520, 539,
        544, 564, 582, 591, 595, 618, 642, 652,
        676, 699, 707, 716, 732, 749, 766, 783,
        794, 810, 828, 839, 851, 862, 874, 885,
        897, 909, 919, 931, 941, 953, 963, 981,
        999, 1016, 1033, 1050, 1067, 1078, 1093, 1111,
        1129, 1147, 1158, 1170, 1182, 1192, 1202, 1213,
        1225, 1237, 1249, 1255, 1266, 1279, 1285, 1292,
        1298, 1305, 1311, 1318, 1325, 1330, 1337, 1342,
        1349, 1354, 1367, 1380, 1392, 1404, 1416, 1428,
        1434, 1444, 1457, 1470, 1483, 1489, 1496, 1503,
        1508, 1513, 1515, 1517, 1519, 1520, 1522, 1524
    };

    static const char _tsip_machine_parser_header_Refer_To_trans_keys[] = {
        82, 114, 9, 32, 58, 69, 101, 9,
        32, 58, 9, 13, 32, 33, 34, 37,
        39, 60, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 96, 97, 122, 10,
        9, 32, 9, 13, 32, 33, 34, 37,
        39, 60, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 96, 97, 122, 10,
        9, 32, 9, 32, 60, 65, 90, 97,
        122, 9, 32, 43, 58, 45, 46, 48,
        57, 65, 90, 97, 122, 9, 32, 58,
        62, 9, 13, 32, 59, 10, 9, 13,
        32, 33, 37, 39, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 10,
        9, 32, 9, 32, 33, 37, 39, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 33, 37, 39,
        59, 61, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        59, 61, 10, 9, 32, 9, 32, 59,
        61, 9, 13, 32, 33, 34, 37, 39,
        91, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 10, 9, 32, 9,
        13, 32, 33, 34, 37, 39, 91, 126,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 10, 9, 32, 9, 32, 34,
        13, 34, 92, 127, 0, 8, 10, 31,
        10, 9, 32, 9, 13, 32, 59, 9,
        13, 32, 59, 10, 9, 32, 9, 32,
        59, 0, 9, 11, 12, 14, 127, 9,
        13, 32, 33, 37, 39, 59, 126, 42,
        43, 45, 46, 48, 57, 65, 90, 95,
        122, 58, 48, 57, 65, 70, 97, 102,
        58, 93, 48, 57, 65, 70, 97, 102,
        58, 93, 48, 57, 65, 70, 97, 102,
        58, 93, 48, 57, 65, 70, 97, 102,
        58, 93, 58, 48, 57, 65, 70, 97,
        102, 46, 58, 93, 48, 57, 65, 70,
        97, 102, 48, 57, 46, 48, 57, 48,
        57, 46, 48, 57, 48, 57, 93, 48,
        57, 93, 48, 57, 93, 46, 48, 57,
        46, 46, 48, 57, 46, 46, 58, 93,
        48, 57, 65, 70, 97, 102, 46, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 58,
        93, 48, 57, 65, 70, 97, 102, 46,
        58, 93, 48, 57, 65, 70, 97, 102,
        46, 58, 93, 48, 57, 65, 70, 97,
        102, 46, 58, 93, 48, 57, 65, 70,
        97, 102, 48, 57, 46, 48, 57, 46,
        48, 57, 46, 58, 9, 13, 32, 33,
        37, 39, 126, 42, 43, 45, 46, 48,
        57, 65, 90, 95, 122, 9, 13, 32,
        33, 37, 39, 60, 126, 42, 43, 45,
        46, 48, 57, 65, 90, 95, 122, 9,
        13, 32, 60, 10, 9, 32, 13, 34,
        92, 127, 0, 8, 10, 31, 10, 9,
        32, 9, 13, 32, 60, 0, 9, 11,
        12, 14, 127, 9, 32, 43, 58, 45,
        46, 48, 57, 65, 90, 97, 122, 9,
        32, 58, 9, 13, 32, 59, 9, 13,
        32, 33, 37, 39, 59, 126, 42, 43,
        45, 46, 48, 57, 65, 90, 95, 122,
        9, 13, 32, 33, 37, 39, 59, 61,
        126, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 59, 61,
        9, 13, 32, 33, 34, 37, 39, 59,
        91, 126, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        37, 39, 59, 126, 42, 43, 45, 46,
        48, 57, 65, 90, 95, 122, 9, 13,
        32, 34, 59, 92, 127, 0, 31, 9,
        13, 32, 59, 9, 13, 32, 33, 34,
        37, 39, 59, 92, 126, 127, 0, 31,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 33, 34, 37,
        39, 59, 61, 92, 126, 127, 0, 31,
        42, 43, 45, 46, 48, 57, 65, 90,
        95, 122, 9, 13, 32, 34, 59, 61,
        92, 127, 0, 31, 9, 13, 32, 33,
        34, 37, 39, 59, 91, 92, 126, 127,
        0, 31, 42, 43, 45, 46, 48, 57,
        65, 90, 95, 122, 9, 13, 32, 33,
        34, 37, 39, 59, 92, 126, 127, 0,
        31, 42, 43, 45, 46, 48, 57, 65,
        90, 95, 122, 9, 13, 32, 59, 0,
        8, 11, 127, 9, 13, 32, 34, 59,
        92, 127, 0, 31, 9, 13, 32, 34,
        58, 59, 92, 127, 0, 31, 48, 57,
        65, 70, 97, 102, 9, 13, 32, 34,
        58, 59, 92, 93, 127, 0, 31, 48,
        57, 65, 70, 97, 102, 9, 13, 32,
        34, 58, 59, 92, 93, 127, 0, 31,
        48, 57, 65, 70, 97, 102, 9, 13,
        32, 34, 58, 59, 92, 93, 127, 0,
        31, 48, 57, 65, 70, 97, 102, 9,
        13, 32, 34, 58, 59, 92, 93, 127,
        0, 31, 9, 13, 32, 34, 58, 59,
        92, 127, 0, 31, 48, 57, 65, 70,
        97, 102, 9, 13, 32, 34, 46, 58,
        59, 92, 93, 127, 0, 31, 48, 57,
        65, 70, 97, 102, 9, 13, 32, 34,
        59, 92, 127, 0, 31, 48, 57, 9,
        13, 32, 34, 46, 59, 92, 127, 0,
        31, 48, 57, 9, 13, 32, 34, 59,
        92, 127, 0, 31, 48, 57, 9, 13,
        32, 34, 46, 59, 92, 127, 0, 31,
        48, 57, 9, 13, 32, 34, 59, 92,
        127, 0, 31, 48, 57, 9, 13, 32,
        34, 59, 92, 93, 127, 0, 31, 48,
        57, 9, 13, 32, 34, 59, 92, 93,
        127, 0, 31, 48, 57, 9, 13, 32,
        34, 59, 92, 93, 127, 0, 31, 9,
        13, 32, 34, 46, 59, 92, 127, 0,
        31, 48, 57, 9, 13, 32, 34, 46,
        59, 92, 127, 0, 31, 9, 13, 32,
        34, 46, 59, 92, 127, 0, 31, 48,
        57, 9, 13, 32, 34, 46, 59, 92,
        127, 0, 31, 9, 13, 32, 34, 46,
        58, 59, 92, 93, 127, 0, 31, 48,
        57, 65, 70, 97, 102, 9, 13, 32,
        34, 46, 58, 59, 92, 93, 127, 0,
        31, 48, 57, 65, 70, 97, 102, 9,
        13, 32, 34, 58, 59, 92, 93, 127,
        0, 31, 48, 57, 65, 70, 97, 102,
        9, 13, 32, 34, 58, 59, 92, 93,
        127, 0, 31, 48, 57, 65, 70, 97,
        102, 9, 13, 32, 34, 58, 59, 92,
        93, 127, 0, 31, 48, 57, 65, 70,
        97, 102, 9, 13, 32, 34, 58, 59,
        92, 93, 127, 0, 31, 48, 57, 65,
        70, 97, 102, 9, 13, 32, 34, 58,
        59, 92, 93, 127, 0, 31, 9, 13,
        32, 34, 59, 92, 127, 0, 31, 48,
        57, 65, 70, 97, 102, 9, 13, 32,
        34, 46, 58, 59, 92, 93, 127, 0,
        31, 48, 57, 65, 70, 97, 102, 9,
        13, 32, 34, 46, 58, 59, 92, 93,
        127, 0, 31, 48, 57, 65, 70, 97,
        102, 9, 13, 32, 34, 46, 58, 59,
        92, 93, 127, 0, 31, 48, 57, 65,
        70, 97, 102, 9, 13, 32, 34, 59,
        92, 127, 0, 31, 48, 57, 9, 13,
        32, 34, 46, 59, 92, 127, 0, 31,
        48, 57, 9, 13, 32, 34, 46, 59,
        92, 127, 0, 31, 48, 57, 9, 13,
        32, 34, 46, 59, 92, 127, 0, 31,
        9, 13, 32, 34, 58, 59, 92, 127,
        0, 31, 9, 13, 32, 58, 59, 48,
        57, 65, 70, 97, 102, 9, 13, 32,
        58, 59, 93, 48, 57, 65, 70, 97,
        102, 9, 13, 32, 58, 59, 93, 48,
        57, 65, 70, 97, 102, 9, 13, 32,
        58, 59, 93, 48, 57, 65, 70, 97,
        102, 9, 13, 32, 58, 59, 93, 9,
        13, 32, 58, 59, 48, 57, 65, 70,
        97, 102, 9, 13, 32, 46, 58, 59,
        93, 48, 57, 65, 70, 97, 102, 9,
        13, 32, 59, 48, 57, 9, 13, 32,
        46, 59, 48, 57, 9, 13, 32, 59,
        48, 57, 9, 13, 32, 46, 59, 48,
        57, 9, 13, 32, 59, 48, 57, 9,
        13, 32, 59, 93, 48, 57, 9, 13,
        32, 59, 93, 48, 57, 9, 13, 32,
        59, 93, 9, 13, 32, 46, 59, 48,
        57, 9, 13, 32, 46, 59, 9, 13,
        32, 46, 59, 48, 57, 9, 13, 32,
        46, 59, 9, 13, 32, 46, 58, 59,
        93, 48, 57, 65, 70, 97, 102, 9,
        13, 32, 46, 58, 59, 93, 48, 57,
        65, 70, 97, 102, 9, 13, 32, 58,
        59, 93, 48, 57, 65, 70, 97, 102,
        9, 13, 32, 58, 59, 93, 48, 57,
        65, 70, 97, 102, 9, 13, 32, 58,
        59, 93, 48, 57, 65, 70, 97, 102,
        9, 13, 32, 58, 59, 93, 48, 57,
        65, 70, 97, 102, 9, 13, 32, 58,
        59, 93, 9, 13, 32, 59, 48, 57,
        65, 70, 97, 102, 9, 13, 32, 46,
        58, 59, 93, 48, 57, 65, 70, 97,
        102, 9, 13, 32, 46, 58, 59, 93,
        48, 57, 65, 70, 97, 102, 9, 13,
        32, 46, 58, 59, 93, 48, 57, 65,
        70, 97, 102, 9, 13, 32, 59, 48,
        57, 9, 13, 32, 46, 59, 48, 57,
        9, 13, 32, 46, 59, 48, 57, 9,
        13, 32, 46, 59, 9, 13, 32, 58,
        59, 70, 102, 69, 101, 82, 114, 45,
        84, 116, 79, 111, 0
    };

    static const char _tsip_machine_parser_header_Refer_To_single_lengths[] = {
        0, 2, 5, 3, 9, 1, 2, 9,
        1, 2, 3, 0, 4, 3, 0, 1,
        4, 1, 7, 1, 2, 6, 9, 5,
        1, 2, 4, 9, 1, 2, 9, 1,
        2, 3, 4, 1, 2, 4, 4, 1,
        2, 3, 0, 8, 1, 2, 2, 2,
        2, 1, 3, 0, 1, 0, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 3,
        3, 2, 2, 2, 2, 2, 0, 3,
        3, 3, 0, 1, 1, 1, 1, 7,
        8, 4, 1, 2, 4, 1, 2, 4,
        0, 4, 3, 0, 4, 8, 9, 5,
        10, 8, 7, 4, 11, 12, 8, 12,
        11, 4, 7, 8, 9, 9, 9, 9,
        8, 10, 7, 8, 7, 8, 7, 8,
        8, 8, 8, 8, 8, 8, 10, 10,
        9, 9, 9, 9, 9, 7, 10, 10,
        10, 7, 8, 8, 8, 8, 5, 6,
        6, 6, 6, 5, 7, 4, 5, 4,
        5, 4, 5, 5, 5, 5, 5, 5,
        5, 7, 7, 6, 6, 6, 6, 6,
        4, 7, 7, 7, 4, 5, 5, 5,
        5, 2, 2, 2, 1, 2, 2, 0
    };

    static const char _tsip_machine_parser_header_Refer_To_range_lengths[] = {
        0, 0, 0, 0, 6, 0, 0, 6,
        0, 0, 0, 2, 4, 0, 0, 0,
        0, 0, 5, 0, 0, 5, 5, 0,
        0, 0, 0, 5, 0, 0, 5, 0,
        0, 0, 2, 0, 0, 0, 0, 0,
        0, 0, 3, 5, 3, 3, 3, 3,
        0, 3, 3, 1, 1, 1, 1, 1,
        1, 1, 0, 1, 0, 1, 0, 3,
        3, 3, 3, 3, 3, 0, 3, 3,
        3, 3, 1, 1, 1, 0, 0, 5,
        5, 0, 0, 0, 2, 0, 0, 0,
        3, 4, 0, 0, 0, 5, 5, 0,
        5, 5, 1, 0, 6, 6, 1, 6,
        6, 2, 1, 4, 4, 4, 4, 1,
        4, 4, 2, 2, 2, 2, 2, 2,
        2, 1, 2, 1, 2, 1, 4, 4,
        4, 4, 4, 4, 1, 4, 4, 4,
        4, 2, 2, 2, 1, 1, 3, 3,
        3, 3, 0, 3, 3, 1, 1, 1,
        1, 1, 1, 1, 0, 1, 0, 1,
        0, 3, 3, 3, 3, 3, 3, 0,
        3, 3, 3, 3, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    static const short _tsip_machine_parser_header_Refer_To_index_offsets[] = {
        0, 0, 3, 9, 13, 29, 31, 34,
        50, 52, 55, 59, 62, 71, 75, 76,
        78, 83, 85, 98, 100, 103, 115, 130,
        136, 138, 141, 146, 161, 163, 166, 181,
        183, 186, 190, 197, 199, 202, 207, 212,
        214, 217, 221, 225, 239, 244, 250, 256,
        262, 265, 270, 277, 279, 282, 284, 287,
        289, 292, 295, 297, 300, 302, 305, 307,
        314, 321, 327, 333, 339, 345, 348, 352,
        359, 366, 373, 375, 378, 381, 383, 385,
        398, 412, 417, 419, 422, 429, 431, 434,
        439, 443, 452, 456, 457, 462, 476, 491,
        497, 513, 527, 536, 541, 559, 578, 588,
        607, 625, 632, 641, 654, 668, 682, 696,
        707, 720, 735, 745, 756, 766, 777, 787,
        798, 809, 819, 830, 840, 851, 861, 876,
        891, 905, 919, 933, 947, 958, 970, 985,
        1000, 1015, 1025, 1036, 1047, 1057, 1067, 1076,
        1086, 1096, 1106, 1113, 1122, 1133, 1139, 1146,
        1152, 1159, 1165, 1172, 1179, 1185, 1192, 1198,
        1205, 1211, 1222, 1233, 1243, 1253, 1263, 1273,
        1280, 1288, 1299, 1310, 1321, 1327, 1334, 1341,
        1347, 1353, 1356, 1359, 1362, 1364, 1367, 1370
    };

    static const unsigned char _tsip_machine_parser_header_Refer_To_indicies[] = {
        0, 0, 1, 2, 2, 3, 4, 4,
        1, 2, 2, 3, 1, 3, 5, 3,
        6, 7, 6, 6, 8, 6, 6, 6,
        6, 9, 6, 9, 1, 10, 1, 11,
        11, 1, 11, 12, 11, 6, 7, 6,
        6, 8, 6, 6, 6, 6, 9, 6,
        9, 1, 13, 1, 14, 14, 1, 14,
        14, 8, 1, 15, 15, 1, 16, 16,
        17, 18, 17, 17, 17, 17, 1, 16,
        16, 18, 1, 19, 20, 19, 21, 22,
        21, 23, 1, 24, 1, 23, 25, 23,
        26, 26, 26, 26, 26, 26, 26, 26,
        26, 1, 27, 1, 28, 28, 1, 28,
        28, 26, 26, 26, 26, 26, 26, 26,
        26, 26, 1, 29, 30, 29, 31, 31,
        31, 32, 33, 31, 31, 31, 31, 31,
        31, 1, 34, 35, 34, 23, 33, 1,
        36, 1, 37, 37, 1, 37, 37, 23,
        33, 1, 33, 38, 33, 39, 40, 39,
        39, 41, 39, 39, 39, 39, 39, 39,
        1, 42, 1, 43, 43, 1, 43, 44,
        43, 39, 40, 39, 39, 41, 39, 39,
        39, 39, 39, 39, 1, 45, 1, 46,
        46, 1, 46, 46, 40, 1, 47, 48,
        49, 1, 1, 1, 40, 50, 1, 40,
        40, 1, 51, 30, 51, 32, 1, 52,
        53, 52, 23, 1, 54, 1, 55, 55,
        1, 55, 55, 23, 1, 40, 40, 40,
        1, 51, 30, 51, 39, 39, 39, 32,
        39, 39, 39, 39, 39, 39, 1, 57,
        56, 56, 56, 1, 59, 48, 58, 58,
        58, 1, 59, 48, 60, 60, 60, 1,
        59, 48, 61, 61, 61, 1, 59, 48,
        1, 63, 62, 56, 56, 1, 64, 59,
        48, 65, 58, 58, 1, 66, 1, 67,
        68, 1, 69, 1, 70, 71, 1, 72,
        1, 48, 73, 1, 48, 74, 1, 48,
        1, 70, 75, 1, 70, 1, 67, 76,
        1, 67, 1, 64, 59, 48, 77, 60,
        60, 1, 64, 59, 48, 61, 61, 61,
        1, 79, 48, 78, 78, 78, 1, 81,
        48, 80, 80, 80, 1, 81, 48, 82,
        82, 82, 1, 81, 48, 83, 83, 83,
        1, 81, 48, 1, 84, 78, 78, 1,
        64, 81, 48, 85, 80, 80, 1, 64,
        81, 48, 86, 82, 82, 1, 64, 81,
        48, 83, 83, 83, 1, 87, 1, 64,
        88, 1, 64, 89, 1, 64, 1, 63,
        1, 90, 91, 90, 92, 92, 92, 92,
        92, 92, 92, 92, 92, 1, 93, 94,
        93, 92, 92, 92, 95, 92, 92, 92,
        92, 92, 92, 1, 96, 12, 96, 8,
        1, 97, 1, 90, 90, 1, 99, 100,
        101, 1, 1, 1, 98, 102, 1, 98,
        98, 1, 93, 94, 93, 95, 1, 98,
        98, 98, 1, 103, 103, 104, 105, 104,
        104, 104, 104, 1, 103, 103, 105, 1,
        106, 107, 108, 107, 109, 106, 109, 108,
        109, 110, 110, 110, 109, 110, 110, 110,
        110, 110, 110, 106, 111, 112, 111, 113,
        113, 113, 114, 115, 113, 113, 113, 113,
        113, 113, 106, 116, 108, 116, 109, 115,
        106, 117, 108, 117, 118, 119, 118, 118,
        109, 120, 118, 118, 118, 118, 118, 118,
        106, 121, 112, 121, 118, 118, 118, 114,
        118, 118, 118, 118, 118, 118, 106, 122,
        108, 122, 123, 124, 125, 106, 106, 119,
        121, 112, 121, 114, 106, 124, 108, 124,
        126, 123, 126, 126, 124, 125, 126, 106,
        106, 126, 126, 126, 126, 126, 119, 127,
        112, 127, 128, 123, 128, 128, 129, 130,
        125, 128, 106, 106, 128, 128, 128, 128,
        128, 119, 131, 108, 131, 123, 124, 130,
        125, 106, 106, 119, 132, 108, 132, 133,
        134, 133, 133, 124, 135, 125, 133, 106,
        106, 133, 133, 133, 133, 133, 119, 136,
        112, 136, 133, 123, 133, 133, 129, 125,
        133, 106, 106, 133, 133, 133, 133, 133,
        119, 122, 108, 122, 124, 119, 119, 106,
        136, 112, 136, 123, 129, 125, 106, 106,
        119, 122, 108, 122, 123, 138, 124, 125,
        106, 106, 137, 137, 137, 119, 122, 108,
        122, 123, 140, 124, 125, 134, 106, 106,
        139, 139, 139, 119, 122, 108, 122, 123,
        140, 124, 125, 134, 106, 106, 141, 141,
        141, 119, 122, 108, 122, 123, 140, 124,
        125, 134, 106, 106, 142, 142, 142, 119,
        122, 108, 122, 123, 140, 124, 125, 134,
        106, 106, 119, 122, 108, 122, 123, 144,
        124, 125, 106, 106, 143, 137, 137, 119,
        122, 108, 122, 123, 145, 140, 124, 125,
        134, 106, 106, 146, 139, 139, 119, 122,
        108, 122, 123, 124, 125, 106, 106, 147,
        119, 122, 108, 122, 123, 148, 124, 125,
        106, 106, 149, 119, 122, 108, 122, 123,
        124, 125, 106, 106, 150, 119, 122, 108,
        122, 123, 151, 124, 125, 106, 106, 152,
        119, 122, 108, 122, 123, 124, 125, 106,
        106, 153, 119, 122, 108, 122, 123, 124,
        125, 134, 106, 106, 154, 119, 122, 108,
        122, 123, 124, 125, 134, 106, 106, 155,
        119, 122, 108, 122, 123, 124, 125, 134,
        106, 106, 119, 122, 108, 122, 123, 151,
        124, 125, 106, 106, 156, 119, 122, 108,
        122, 123, 151, 124, 125, 106, 106, 119,
        122, 108, 122, 123, 148, 124, 125, 106,
        106, 157, 119, 122, 108, 122, 123, 148,
        124, 125, 106, 106, 119, 122, 108, 122,
        123, 145, 140, 124, 125, 134, 106, 106,
        158, 141, 141, 119, 122, 108, 122, 123,
        145, 140, 124, 125, 134, 106, 106, 142,
        142, 142, 119, 122, 108, 122, 123, 160,
        124, 125, 134, 106, 106, 159, 159, 159,
        119, 122, 108, 122, 123, 162, 124, 125,
        134, 106, 106, 161, 161, 161, 119, 122,
        108, 122, 123, 162, 124, 125, 134, 106,
        106, 163, 163, 163, 119, 122, 108, 122,
        123, 162, 124, 125, 134, 106, 106, 164,
        164, 164, 119, 122, 108, 122, 123, 162,
        124, 125, 134, 106, 106, 119, 122, 108,
        122, 123, 124, 125, 106, 106, 165, 159,
        159, 119, 122, 108, 122, 123, 145, 162,
        124, 125, 134, 106, 106, 166, 161, 161,
        119, 122, 108, 122, 123, 145, 162, 124,
        125, 134, 106, 106, 167, 163, 163, 119,
        122, 108, 122, 123, 145, 162, 124, 125,
        134, 106, 106, 164, 164, 164, 119, 122,
        108, 122, 123, 124, 125, 106, 106, 168,
        119, 122, 108, 122, 123, 145, 124, 125,
        106, 106, 169, 119, 122, 108, 122, 123,
        145, 124, 125, 106, 106, 170, 119, 122,
        108, 122, 123, 145, 124, 125, 106, 106,
        119, 122, 108, 122, 123, 144, 124, 125,
        106, 106, 119, 107, 108, 107, 172, 109,
        171, 171, 171, 106, 107, 108, 107, 174,
        109, 123, 173, 173, 173, 106, 107, 108,
        107, 174, 109, 123, 175, 175, 175, 106,
        107, 108, 107, 174, 109, 123, 176, 176,
        176, 106, 107, 108, 107, 174, 109, 123,
        106, 107, 108, 107, 178, 109, 177, 171,
        171, 106, 107, 108, 107, 179, 174, 109,
        123, 180, 173, 173, 106, 107, 108, 107,
        109, 181, 106, 107, 108, 107, 182, 109,
        183, 106, 107, 108, 107, 109, 184, 106,
        107, 108, 107, 185, 109, 186, 106, 107,
        108, 107, 109, 187, 106, 107, 108, 107,
        109, 123, 188, 106, 107, 108, 107, 109,
        123, 189, 106, 107, 108, 107, 109, 123,
        106, 107, 108, 107, 185, 109, 190, 106,
        107, 108, 107, 185, 109, 106, 107, 108,
        107, 182, 109, 191, 106, 107, 108, 107,
        182, 109, 106, 107, 108, 107, 179, 174,
        109, 123, 192, 175, 175, 106, 107, 108,
        107, 179, 174, 109, 123, 176, 176, 176,
        106, 107, 108, 107, 194, 109, 123, 193,
        193, 193, 106, 107, 108, 107, 196, 109,
        123, 195, 195, 195, 106, 107, 108, 107,
        196, 109, 123, 197, 197, 197, 106, 107,
        108, 107, 196, 109, 123, 198, 198, 198,
        106, 107, 108, 107, 196, 109, 123, 106,
        107, 108, 107, 109, 199, 193, 193, 106,
        107, 108, 107, 179, 196, 109, 123, 200,
        195, 195, 106, 107, 108, 107, 179, 196,
        109, 123, 201, 197, 197, 106, 107, 108,
        107, 179, 196, 109, 123, 198, 198, 198,
        106, 107, 108, 107, 109, 202, 106, 107,
        108, 107, 179, 109, 203, 106, 107, 108,
        107, 179, 109, 204, 106, 107, 108, 107,
        179, 109, 106, 107, 108, 107, 178, 109,
        106, 205, 205, 1, 206, 206, 1, 207,
        207, 1, 208, 1, 209, 209, 1, 2,
        2, 1, 1, 0
    };

    static const unsigned char _tsip_machine_parser_header_Refer_To_trans_targs[] = {
        2, 0, 3, 4, 177, 5, 79, 84,
        11, 89, 6, 7, 8, 9, 10, 12,
        13, 12, 14, 15, 16, 16, 17, 18,
        183, 19, 22, 20, 21, 23, 17, 22,
        18, 27, 23, 24, 25, 26, 28, 43,
        34, 44, 29, 30, 31, 32, 33, 35,
        37, 42, 36, 38, 38, 39, 40, 41,
        45, 78, 46, 49, 47, 48, 50, 65,
        51, 63, 52, 53, 61, 54, 55, 59,
        56, 57, 58, 60, 62, 64, 66, 74,
        67, 70, 68, 69, 71, 72, 73, 75,
        76, 77, 80, 82, 79, 81, 8, 11,
        81, 83, 84, 85, 87, 88, 86, 90,
        89, 91, 92, 92, 17, 93, 94, 95,
        17, 94, 93, 96, 95, 96, 97, 98,
        142, 92, 98, 99, 100, 105, 101, 102,
        101, 100, 103, 102, 103, 104, 106, 107,
        98, 108, 141, 109, 112, 110, 111, 113,
        128, 114, 126, 115, 116, 124, 117, 118,
        122, 119, 120, 121, 123, 125, 127, 129,
        137, 130, 133, 131, 132, 134, 135, 136,
        138, 139, 140, 143, 176, 144, 147, 145,
        146, 148, 163, 149, 161, 150, 151, 159,
        152, 153, 157, 154, 155, 156, 158, 160,
        162, 164, 172, 165, 168, 166, 167, 169,
        170, 171, 173, 174, 175, 178, 179, 180,
        181, 182
    };

    static const char _tsip_machine_parser_header_Refer_To_trans_actions[] = {
        0, 0, 0, 0, 0, 0, 1, 1,
        0, 1, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 3, 0, 0, 0,
        9, 0, 1, 0, 0, 7, 7, 0,
        7, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 7, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 5, 5, 5,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 3, 3, 3, 1, 11,
        11, 0, 11, 0, 3, 3, 0, 0,
        0, 11, 3, 0, 3, 0, 1, 11,
        0, 11, 0, 3, 3, 0, 0, 0,
        11, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0
    };

    static const int tsip_machine_parser_header_Refer_To_start = 1;
    static const int tsip_machine_parser_header_Refer_To_first_final = 183;
    static const int tsip_machine_parser_header_Refer_To_error = 0;

    static const int tsip_machine_parser_header_Refer_To_en_main = 1;


    /* #line 128 "./ragel/tsip_parser_header_Refer_To.rl" */
    (void)(eof);
    (void)(tsip_machine_parser_header_Refer_To_first_final);
    (void)(tsip_machine_parser_header_Refer_To_error);
    (void)(tsip_machine_parser_header_Refer_To_en_main);

    /* #line 639 "./src/headers/tsip_header_Refer_To.c" */
    {
        cs = tsip_machine_parser_header_Refer_To_start;
    }

    /* #line 133 "./ragel/tsip_parser_header_Refer_To.rl" */

    /* #line 646 "./src/headers/tsip_header_Refer_To.c" */
    {
        int _klen;
        unsigned int _trans;
        const char *_acts;
        unsigned int _nacts;
        const char *_keys;

        if ( p == pe ) {
            goto _test_eof;
        }
        if ( cs == 0 ) {
            goto _out;
        }
_resume:
        _keys = _tsip_machine_parser_header_Refer_To_trans_keys + _tsip_machine_parser_header_Refer_To_key_offsets[cs];
        _trans = _tsip_machine_parser_header_Refer_To_index_offsets[cs];

        _klen = _tsip_machine_parser_header_Refer_To_single_lengths[cs];
        if ( _klen > 0 ) {
            const char *_lower = _keys;
            const char *_mid;
            const char *_upper = _keys + _klen - 1;
            while (1) {
                if ( _upper < _lower ) {
                    break;
                }

                _mid = _lower + ((_upper-_lower) >> 1);
                if ( (*p) < *_mid ) {
                    _upper = _mid - 1;
                }
                else if ( (*p) > *_mid ) {
                    _lower = _mid + 1;
                }
                else {
                    _trans += (_mid - _keys);
                    goto _match;
                }
            }
            _keys += _klen;
            _trans += _klen;
        }

        _klen = _tsip_machine_parser_header_Refer_To_range_lengths[cs];
        if ( _klen > 0 ) {
            const char *_lower = _keys;
            const char *_mid;
            const char *_upper = _keys + (_klen<<1) - 2;
            while (1) {
                if ( _upper < _lower ) {
                    break;
                }

                _mid = _lower + (((_upper-_lower) >> 1) & ~1);
                if ( (*p) < _mid[0] ) {
                    _upper = _mid - 2;
                }
                else if ( (*p) > _mid[1] ) {
                    _lower = _mid + 2;
                }
                else {
                    _trans += ((_mid - _keys)>>1);
                    goto _match;
                }
            }
            _trans += _klen;
        }

_match:
        _trans = _tsip_machine_parser_header_Refer_To_indicies[_trans];
        cs = _tsip_machine_parser_header_Refer_To_trans_targs[_trans];

        if ( _tsip_machine_parser_header_Refer_To_trans_actions[_trans] == 0 ) {
            goto _again;
        }

        _acts = _tsip_machine_parser_header_Refer_To_actions + _tsip_machine_parser_header_Refer_To_trans_actions[_trans];
        _nacts = (unsigned int) *_acts++;
        while ( _nacts-- > 0 ) {
            switch ( *_acts++ ) {
            case 0:
                /* #line 49 "./ragel/tsip_parser_header_Refer_To.rl" */
            {
                tag_start = p;
            }
            break;
            case 1:
                /* #line 53 "./ragel/tsip_parser_header_Refer_To.rl" */
            {
                if(!r_to->uri) { /* Only one URI */
                    int len = (int)(p  - tag_start);
                    if(r_to && !r_to->uri) {
                        if((r_to->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && r_to->display_name) {
                            r_to->uri->display_name = tsk_strdup(r_to->display_name);
                        }
                    }
                }
            }
            break;
            case 2:
                /* #line 64 "./ragel/tsip_parser_header_Refer_To.rl" */
            {
                if(!r_to->display_name) {
                    TSK_PARSER_SET_STRING(r_to->display_name);
                    tsk_strunquote(&r_to->display_name);
                }
            }
            break;
            case 3:
                /* #line 71 "./ragel/tsip_parser_header_Refer_To.rl" */
            {
                TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(r_to));
            }
            break;
            case 4:
                /* #line 75 "./ragel/tsip_parser_header_Refer_To.rl" */
            {
            }
            break;
                /* #line 759 "./src/headers/tsip_header_Refer_To.c" */
            }
        }

_again:
        if ( cs == 0 ) {
            goto _out;
        }
        if ( ++p != pe ) {
            goto _resume;
        }
_test_eof: {
        }
_out: {
        }
    }

    /* #line 134 "./ragel/tsip_parser_header_Refer_To.rl" */

    if( cs <
            /* #line 775 "./src/headers/tsip_header_Refer_To.c" */
            183
            /* #line 135 "./ragel/tsip_parser_header_Refer_To.rl" */
      ) {
        TSK_DEBUG_ERROR("Failed to parse 'Refer-To' header.");
        TSK_OBJECT_SAFE_FREE(r_to);
    }

    return r_to;
}







//========================================================
//  Refer_To header object definition
//

static tsk_object_t* tsip_header_Refer_To_ctor(tsk_object_t *self, va_list * app)
{
    tsip_header_Refer_To_t *Refer_To = self;
    if(Refer_To) {
        const tsip_uri_t* uri = va_arg(*app, const tsip_uri_t*);

        TSIP_HEADER(Refer_To)->type = tsip_htype_Refer_To;
        TSIP_HEADER(Refer_To)->serialize = tsip_header_Refer_To_serialize;

        if(uri) {
            Refer_To->uri = tsk_object_ref((void*)uri);
        }
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new Refer_To header.");
    }
    return self;
}

static tsk_object_t* tsip_header_Refer_To_dtor(tsk_object_t *self)
{
    tsip_header_Refer_To_t *Refer_To = self;
    if(Refer_To) {
        TSK_FREE(Refer_To->display_name);
        TSK_OBJECT_SAFE_FREE(Refer_To->uri);

        TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Refer_To));
    }
    else {
        TSK_DEBUG_ERROR("Null Refer_To header.");
    }

    return self;
}

static const tsk_object_def_t tsip_header_Refer_To_def_s = {
    sizeof(tsip_header_Refer_To_t),
    tsip_header_Refer_To_ctor,
    tsip_header_Refer_To_dtor,
    tsk_null
};
const tsk_object_def_t *tsip_header_Refer_To_def_t = &tsip_header_Refer_To_def_s;
