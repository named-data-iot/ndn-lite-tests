/*
 * Copyright (C) 2018 Tianyuan Yu, Zhiyi Zhang, Edward Lu, Hanwen Zhang
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "aes-tests.h"
#include "../CUnit/CUnit.h"
#include "../CUnit/Basic.h"
#include <stdio.h>
#include <string.h>
#include "../print-helpers.h"
#include "../test-helpers.h"
#include "ndn-lite/security/ndn-lite-aes.h"

#define PLAIN_TEXT_BUFFER_MAX_SIZE 255

static uint8_t plain_text[PLAIN_TEXT_BUFFER_MAX_SIZE];

#define CIPHER_TEXT_BUFFER_MAX_SIZE 255

static uint8_t cipher_text[CIPHER_TEXT_BUFFER_MAX_SIZE];

static const char *_current_test_name;
static bool _test_success = true;

void aes_test_case_1(void)
{
  // https://tools.ietf.org/html/rfc3602 Case #2
  uint8_t key[] = {0xc2, 0x86, 0x69, 0x6d, 0x88, 0x7c, 0x9a, 0xa0, 0x61, 0x1b, 0xbb, 0x3e, 0x20, 0x25, 0xa4, 0x5a};
  uint8_t iv[] = {0x56, 0x2e, 0x17, 0x99, 0x6d, 0x09, 0x3d, 0x28, 0xdd, 0xb3, 0xba, 0x69, 0x5a, 0x2e, 0x6f, 0x58};
  uint8_t plain_text[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                          0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
  uint8_t cipher_text[] = {0xd2, 0x96, 0xcd, 0x94, 0xc2, 0xcc, 0xcf, 0x8a, 0x3a, 0x86, 0x30, 0x28, 0xb5, 0xe1, 0xdc, 0x0a,
                           0x75, 0x86, 0x60, 0x2d, 0x25, 0x3c, 0xff, 0xf9, 0x1b, 0x82, 0x66, 0xbe, 0xa6, 0xd6, 0x1a, 0xb1};
  uint8_t output[256] = {0};

  ndn_aes_key_t aes_key;
  ndn_aes_key_init(&aes_key, key, sizeof(key), 123);
  uint32_t used_size = 0;
  int ret_val = ndn_aes_cbc_encrypt(plain_text, sizeof(plain_text), output, &used_size, iv, &aes_key);
  CU_ASSERT_EQUAL(memcmp(cipher_text, output, used_size), 0);
  CU_ASSERT_EQUAL(used_size, sizeof(cipher_text));
}

void aes_test_case_2(void)
{
  // https://tools.ietf.org/html/rfc3602 Case #2
  uint8_t key[] = {0xc2, 0x86, 0x69, 0x6d, 0x88, 0x7c, 0x9a, 0xa0, 0x61, 0x1b, 0xbb, 0x3e, 0x20, 0x25, 0xa4, 0x5a};
  uint8_t iv[] = {0x56, 0x2e, 0x17, 0x99, 0x6d, 0x09, 0x3d, 0x28, 0xdd, 0xb3, 0xba, 0x69, 0x5a, 0x2e, 0x6f, 0x58};
  uint8_t plain_text[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                          0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
  uint8_t cipher_text[] = {0xd2, 0x96, 0xcd, 0x94, 0xc2, 0xcc, 0xcf, 0x8a, 0x3a, 0x86, 0x30, 0x28, 0xb5, 0xe1, 0xdc, 0x0a,
                           0x75, 0x86, 0x60, 0x2d, 0x25, 0x3c, 0xff, 0xf9, 0x1b, 0x82, 0x66, 0xbe, 0xa6, 0xd6, 0x1a, 0xb1};
  uint8_t output[256] = {0};

  ndn_aes_key_t aes_key;
  ndn_aes_key_init(&aes_key, key, sizeof(key), 123);
  uint32_t used_size = 0;
  int ret_val = ndn_aes_cbc_decrypt(cipher_text, sizeof(cipher_text), output, &used_size, iv, &aes_key);
  CU_ASSERT_EQUAL(memcmp(plain_text, output, used_size), 0);
  CU_ASSERT_EQUAL(used_size, sizeof(plain_text));
}

void aes_test_case_3(void)
{
  // encryption with padding
  uint8_t key[] = {0xc2, 0x86, 0x69, 0x6d, 0x88, 0x7c, 0x9a, 0xa0, 0x61, 0x1b, 0xbb, 0x3e, 0x20, 0x25, 0xa4, 0x5a};
  uint8_t iv[] = {0x56, 0x2e, 0x17, 0x99, 0x6d, 0x09, 0x3d, 0x28, 0xdd, 0xb3, 0xba, 0x69, 0x5a, 0x2e, 0x6f, 0x58};
  uint8_t plain_text[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                          0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                          0x00, 0x01, 0x02};
  uint8_t cipher_text[] = {0xd2, 0x96, 0xcd, 0x94, 0xc2, 0xcc, 0xcf, 0x8a, 0x3a, 0x86, 0x30, 0x28, 0xb5, 0xe1, 0xdc, 0x0a,
                           0x75, 0x86, 0x60, 0x2d, 0x25, 0x3c, 0xff, 0xf9, 0x1b, 0x82, 0x66, 0xbe, 0xa6, 0xd6, 0x1a, 0xb1,
                           0xb0, 0xae, 0x85, 0x1d, 0x6f, 0x38, 0xc8, 0x01, 0x70, 0x88, 0xed, 0x52, 0x18, 0xe6, 0x97, 0x4e};
  uint8_t output[256] = {0};

  ndn_aes_key_t aes_key;
  ndn_aes_key_init(&aes_key, key, sizeof(key), 123);
  uint32_t used_size = 0;
  int ret_val = ndn_aes_cbc_encrypt(plain_text, sizeof(plain_text), output, &used_size, iv, &aes_key);
  CU_ASSERT_EQUAL(memcmp(cipher_text, output, used_size), 0);
  CU_ASSERT_EQUAL(used_size, sizeof(cipher_text));
}

void aes_test_case_4(void)
{
  // decryption with padding
  uint8_t key[] = {0xc2, 0x86, 0x69, 0x6d, 0x88, 0x7c, 0x9a, 0xa0, 0x61, 0x1b, 0xbb, 0x3e, 0x20, 0x25, 0xa4, 0x5a};
  uint8_t iv[] = {0x56, 0x2e, 0x17, 0x99, 0x6d, 0x09, 0x3d, 0x28, 0xdd, 0xb3, 0xba, 0x69, 0x5a, 0x2e, 0x6f, 0x58};
  uint8_t plain_text[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                          0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                          0x00, 0x01, 0x02};
  uint8_t cipher_text[] = {0xd2, 0x96, 0xcd, 0x94, 0xc2, 0xcc, 0xcf, 0x8a, 0x3a, 0x86, 0x30, 0x28, 0xb5, 0xe1, 0xdc, 0x0a,
                           0x75, 0x86, 0x60, 0x2d, 0x25, 0x3c, 0xff, 0xf9, 0x1b, 0x82, 0x66, 0xbe, 0xa6, 0xd6, 0x1a, 0xb1,
                           0xb0, 0xae, 0x85, 0x1d, 0x6f, 0x38, 0xc8, 0x01, 0x70, 0x88, 0xed, 0x52, 0x18, 0xe6, 0x97, 0x4e};
  uint8_t output[256] = {0};

  ndn_aes_key_t aes_key;
  ndn_aes_key_init(&aes_key, key, sizeof(key), 123);
  uint32_t used_size = 0;
  int ret_val = ndn_aes_cbc_decrypt(cipher_text, sizeof(cipher_text), output, &used_size, iv, &aes_key);
  CU_ASSERT_EQUAL(memcmp(plain_text, output, used_size), 0);
  CU_ASSERT_EQUAL(used_size, sizeof(plain_text));
}

void aes_test_0(void)
{
  CU_ASSERT(true);
}

void add_aes_test_suite(void)
{
  CU_pSuite pSuite = NULL;

  /* add a suite to the registry */
  pSuite = CU_add_suite("AES CBC Test", ndn_security_init, NULL);
  if (NULL == pSuite)
  {
    CU_cleanup_registry();
    // return CU_get_error();
    return;
  }
  if (NULL == CU_add_test(pSuite, "aes_test_case_1", aes_test_case_1) ||
      NULL == CU_add_test(pSuite, "aes_test_case_2", aes_test_case_2) ||
      NULL == CU_add_test(pSuite, "aes_test_case_3", aes_test_case_3) ||
      NULL == CU_add_test(pSuite, "aes_test_case_4", aes_test_case_4))
  {
    CU_cleanup_registry();
    // return CU_get_error();
    return;
  }
}