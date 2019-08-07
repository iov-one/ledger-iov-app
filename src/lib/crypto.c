/*******************************************************************************
*   (c) 2019 ZondaX GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include "crypto.h"
#include <bech32.h>

uint32_t bip44Path[5];

#if defined(TARGET_NANOS) || defined(TARGET_NANOX)
#include "cx.h"

void crypto_extractPublicKey(uint32_t bip44Path[BIP44_LEN_DEFAULT], uint8_t *pubKey) {
    cx_ecfp_public_key_t cx_publicKey;
    cx_ecfp_private_key_t cx_privateKey;
    uint8_t privateKeyData[32];

    // Generate keys
    os_perso_derive_node_bip32_seed_key(
            HDW_ED25519_SLIP10,
            CX_CURVE_Ed25519,
            bip44Path,
            BIP44_LEN_DEFAULT,
            privateKeyData,
            NULL,
            NULL,
            0);

    cx_ecfp_init_private_key(CX_CURVE_Ed25519, privateKeyData, 32, &cx_privateKey);
    cx_ecfp_init_public_key(CX_CURVE_Ed25519, NULL, 0, &cx_publicKey);
    cx_ecfp_generate_pair(CX_CURVE_Ed25519, &cx_publicKey, &cx_privateKey, 1);
    MEMSET(privateKeyData, 0, 32);

    // Format pubkey
    for (int i = 0; i < 32; i++) {
        pubKey[i] = cx_publicKey.W[64 - i];
    }

    if ((cx_publicKey.W[32] & 1) != 0) {
        pubKey[31] |= 0x80;
    }
}

uint16_t crypto_sign(uint8_t *signature, uint16_t signatureMaxlen, const uint8_t *message, uint16_t messageLen) {
    // Hash
    uint8_t messageDigest[CX_SHA512_SIZE];
    cx_hash_sha512(message, messageLen, messageDigest, CX_SHA512_SIZE);

    // Generate keys
    cx_ecfp_private_key_t cx_privateKey;
    uint8_t privateKeyData[32];
    os_perso_derive_node_bip32_seed_key(
            HDW_ED25519_SLIP10,
            CX_CURVE_Ed25519,
            bip44Path,
            BIP44_LEN_DEFAULT,
            privateKeyData,
            NULL,
            NULL,
            0);
    cx_ecfp_init_private_key(CX_CURVE_Ed25519, privateKeyData, 32, &cx_privateKey);

    // Sign
    unsigned int info = 0;
    int signatureLength = cx_eddsa_sign(&cx_privateKey,
                                        CX_LAST,
                                        CX_SHA512,
                                        messageDigest,
                                        CX_SHA512_SIZE,
                                        NULL,
                                        0,
                                        signature,
                                        signatureMaxlen,
                                        &info);

    MEMSET(&cx_privateKey, 0, sizeof(cx_privateKey));
    MEMSET(privateKeyData, 0, 32);

    return signatureLength;
}
#else

void crypto_extractPublicKey(uint32_t bip44Path[BIP44_LEN_DEFAULT], uint8_t *pubKey) {
    MEMSET(pubKey, 0, 32);
};

uint16_t crypto_sign(uint8_t *signature, uint16_t signatureMaxlen, const uint8_t *message, uint16_t messageLen) {
    return 0;
};
#endif

uint16_t crypto_fillAddress(uint8_t *buffer, uint16_t buffer_len) {
    // extract pubkey (first 32 bytes)
    crypto_extractPublicKey(bip44Path, buffer);

    // FIXME: Add support for other HRPs
    const char *hrp = "iov";
    char *addr = (char *) (buffer + 32);

    bech32EncodeFromBytes(addr, hrp, buffer, 32);
    return 32 + strlen(addr);
};
