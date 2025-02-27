/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include "csuit/suit_manifest_print.h"
#include "suit_examples_common.h"
#include "trust_anchor_a128_cose_key_secret.h"

#define MAX_FILE_BUFFER_SIZE            2048

int main(int argc, char *argv[]) {
    // check arguments.
    if (argc < 3) {
        printf("%s <raw image> <encrypted image> <encryption info>]", argv[0]);
        return EXIT_FAILURE;
    }
    char *src_file = argv[1];
    char *enc_file = argv[2];
    char *encryption_info_file = argv[3];
    suit_err_t result = SUIT_SUCCESS;
    printf("%s %s %s %s\n", argv[0], src_file, enc_file, encryption_info_file);

    // Load secret key
    suit_mechanism_t mechanism = {0};
    result = suit_set_mechanism_from_cose_key(trust_anchor_a128_cose_key_secret, &mechanism);
    if (result != SUIT_SUCCESS) {
        printf("main : Failed to create A128KW secret key. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    mechanism.cose_tag = CBOR_TAG_COSE_ENCRYPT;
    mechanism.kid = Q_USEFUL_BUF_FROM_SZ_LITERAL("kid-1");
    mechanism.use = true;

    // Load raw image
    UsefulBuf_MAKE_STACK_UB(raw_buf, MAX_FILE_BUFFER_SIZE);
    raw_buf.len = read_from_file(src_file, raw_buf.ptr, raw_buf.len);

    // Prepare
    UsefulBuf_MAKE_STACK_UB(encrypted_payload_buf, MAX_FILE_BUFFER_SIZE);
    UsefulBuf_MAKE_STACK_UB(encryption_info_buf, MAX_FILE_BUFFER_SIZE);

    UsefulBufC encrypted_payload;
    UsefulBufC encryption_info;
    result = suit_encrypt_cose_encrypt(UsefulBuf_Const(raw_buf), &mechanism, encrypted_payload_buf, encryption_info_buf, &encrypted_payload, &encryption_info);

    if (result != SUIT_SUCCESS) {
        printf("\nmain : Failed to encrypt. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }

    printf("\nmain : Succeed to encrypt.\nmain : Write to %s and %s.\n", enc_file, encryption_info_file);
    write_to_file(enc_file, encrypted_payload.ptr, encrypted_payload.len);
    write_to_file(encryption_info_file, encryption_info.ptr, encryption_info.len);

    // Decrypt
    UsefulBufC plaintext_payload;
    UsefulBuf_MAKE_STACK_UB(plaintext_payload_buf, MAX_FILE_BUFFER_SIZE);
    result = suit_decrypt_cose_encrypt(encrypted_payload, encryption_info, plaintext_payload_buf, &mechanism, &plaintext_payload);
    if (result != SUIT_SUCCESS) {
        printf("\nmain : Failed to decrypt. %s(%d)\n", suit_err_to_str(result), result);
        return EXIT_FAILURE;
    }
    if (raw_buf.len != plaintext_payload.len) {
        printf("\nmain : Assertion failed. length %lu != %lu\n", raw_buf.len, plaintext_payload.len);
        return EXIT_FAILURE;
    }
    if (memcmp(raw_buf.ptr, plaintext_payload.ptr, raw_buf.len) != 0) {
        printf("\nmain : Assertion failed. raw_buf != plaintext_buf\n");
        suit_print_hex(raw_buf.ptr, raw_buf.len);
        printf("\n");
        suit_print_hex(plaintext_payload.ptr, plaintext_payload.len);
        printf("\n");
        return EXIT_FAILURE;
    }
    printf("\n main : Succeed to decrypt.\n");
    return EXIT_SUCCESS;
}
