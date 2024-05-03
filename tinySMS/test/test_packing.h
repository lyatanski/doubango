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
#ifndef _TEST_SMSPACKING_H
#define _TEST_SMSPACKING_H

#define CHECK_STREQ(exp, got) if(!tsk_striequals((exp), (got))) \
    printf("FAIL: %s expected %s, got %s\n", __func__, (char*)(exp), (char*)(got)); \
    else printf("PASS: %s\n", (char*)(exp));
#define CHECK(cond, ...) if(cond) {\
    printf("FAIL " #cond " " __VA_ARGS__ ); printf("\n"); \
    } else { printf("PASS " #cond " " __VA_ARGS__ ); printf("\n"); }

typedef struct test_message_7bit_s {
    const char* ascii;
    const char* encod;
    const size_t  size;
}
test_message_t;
#define MSG(data) data, sizeof(data)

test_message_t test_messages_7bit[] = {
    "a",                          MSG("\x61"),
    "ab",                         MSG("\x61\x31"),
    "abc",                        MSG("\x61\xF1\x18"),
    "abcdefg",                    MSG("\x61\xF1\x98\x5C\x36\x9F\x01"),
    "abcdefgh",                   MSG("\x61\xF1\x98\x5C\x36\x9F\xD1"),
    "abcdefghi",                  MSG("\x61\xF1\x98\x5C\x36\x9F\xD1\x69"),
    "abcdefgh12345678",           MSG("\x61\xF1\x98\x5C\x36\x9F\xD1\x31\xD9\x8C\x56\xB3\xDD\x70"),
    "abcdefgh123456789",          MSG("\x61\xF1\x98\x5C\x36\x9F\xD1\x31\xD9\x8C\x56\xB3\xDD\x70\x39"),
    "This is a test sms message", MSG("\x54\x74\x7A\x0E\x4A\xCF\x41\x61\x10\xBD\x3C\xA7\x83\xE6\xED\x39\xA8\x5D\x9E\xCF\xC3\xE7\x32"),
    "salut",                      MSG("\xF3\x30\xBB\x4E\x07"),
    "hellohello",                 MSG("\xE8\x32\x9B\xFD\x46\x97\xD9\xEC\x37"),
};

void test_7bit()
{
    tsk_size_t i;

    /* To 7bit */
    for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_t); i++) {
        tsk_buffer_t* buffer;
        CHECK(!(buffer = tsms_pack_to_7bit(test_messages_7bit[i].ascii)), "for %ld", i)
        CHECK_STREQ(buffer->data, test_messages_7bit[i].encod)
        TSK_OBJECT_SAFE_FREE(buffer);
    }

    /* From 7bit */
    for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_t); i++) {
        char *temp;
        CHECK(!(temp = tsms_pack_from_7bit(test_messages_7bit[i].encod, (tsk_size_t)tsk_strlen(test_messages_7bit[i].encod))), "for %ld", i)
        CHECK_STREQ(temp, test_messages_7bit[i].ascii)
        TSK_FREE(temp);
    }
}

void test_8bit()
{
    tsk_size_t i;

    /* To 8bit */
    for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_t); i++) {
        tsk_buffer_t* buffer;
        if((buffer = tsms_pack_to_8bit(test_messages_7bit[i].ascii))) {
            if(!tsk_strequals(buffer->data, test_messages_7bit[i].ascii)) {
                printf("tsms_pack_to_8bit(%s) Failed", test_messages_7bit[i].ascii);
            }
            TSK_OBJECT_SAFE_FREE(buffer);
        }
    }

    /* From 8bit */
    for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_t); i++) {
        char* temp;
        if((temp = tsms_pack_from_8bit(test_messages_7bit[i].ascii, (tsk_size_t)tsk_strlen(test_messages_7bit[i].ascii)))) {
            if(!tsk_strequals(temp, test_messages_7bit[i].ascii)) {
                printf("tsms_pack_from_8bit(%s) Failed", test_messages_7bit[i].ascii);
            }
            TSK_FREE(temp);
        }
    }
}

void test_ucs2()
{
    tsk_size_t i;

    /* To ucs2 */
    for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_t); i++) {
        tsk_buffer_t* buffer;
        if((buffer = tsms_pack_to_ucs2(test_messages_7bit[i].ascii))) {
            if(!memcmp(buffer->data, test_messages_7bit[i].encod, test_messages_7bit[i].size)) {
                printf("tsms_pack_to_ucs2(%s) Failed\n", test_messages_7bit[i].ascii);
            }
            TSK_OBJECT_SAFE_FREE(buffer);
        }
    }

    /* From ucs2 */
    for(i=0; i<sizeof(test_messages_7bit)/sizeof(test_message_t); i++) {
        char* temp;
        if((temp = tsms_pack_from_ucs2(test_messages_7bit[i].encod, test_messages_7bit[i].size))) {
            if(!tsk_strequals(temp, test_messages_7bit[i].ascii)) {
                printf("tsms_pack_from_ucs2(%s) Failed\n", test_messages_7bit[i].ascii);
            }
            TSK_FREE(temp);
        }
    }
}

void test_packing()
{
    test_7bit();
    test_8bit();
    //test_ucs2();
}

#endif /* _TEST_SMSPACKING_H */
