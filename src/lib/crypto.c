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
#include "iov.h"
#include <bech32.h>

uint32_t bip32Path[BIP32_LEN_DEFAULT];

#if defined(TARGET_NANOS) || defined(TARGET_NANOX)
#include "cx.h"

void crypto_extractPublicKey(uint32_t bip32Path[BIP32_LEN_DEFAULT], uint8_t *pubKey) {
    cx_ecfp_public_key_t cx_publicKey;
    cx_ecfp_private_key_t cx_privateKey;
    uint8_t privateKeyData[32];

    // Generate keys
    os_perso_derive_node_bip32_seed_key(
            HDW_ED25519_SLIP10,
            CX_CURVE_Ed25519,
            bip32Path,
            BIP32_LEN_DEFAULT,
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
            bip32Path,
            BIP32_LEN_DEFAULT,
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

void crypto_extractPublicKey(uint32_t path[BIP32_LEN_DEFAULT], uint8_t *pubKey) {
    // Empty version for non-Ledger devices
    MEMSET(pubKey, 0, 32);
}

uint16_t crypto_sign(uint8_t *signature,
                     uint16_t signatureMaxlen,
                     const uint8_t *message,
                     uint16_t messageLen) {
    // Empty version for non-Ledger devices
    return 0;
}

#define CX_SHA256_SIZE 32

int cx_hash_sha256(const unsigned char *in, unsigned int len, unsigned char *out, unsigned int out_len) {
    // Empty version for non-Ledger devices
    // Mock with picosha2
    return 0;
}

#endif

char *hrp;

void crypto_set_hrp(char *p) {
    hrp = p;
}

uint16_t crypto_fillAddress(uint8_t *buffer, uint16_t buffer_len) {
    if (buffer_len < ED25519_PK_LEN + 30) {
        return 0;
    }

    // extract pubkey (first 32 bytes)
    crypto_extractPublicKey(bip32Path, buffer);

    char tmp[IOV_PK_PREFIX_LEN + ED25519_PK_LEN];
    strcpy(tmp, IOV_PK_PREFIX);
    MEMCPY(tmp + IOV_PK_PREFIX_LEN, buffer, ED25519_PK_LEN);

    //
    uint8_t hash[CX_SHA256_SIZE];
    cx_hash_sha256((uint8_t *) tmp, IOV_PK_PREFIX_LEN + ED25519_PK_LEN,
                   hash, CX_SHA256_SIZE);

    char *addr = (char *) (buffer + ED25519_PK_LEN);
    bech32EncodeFromBytes(addr, hrp, hash, 20);
    return ED25519_PK_LEN + strlen(addr);
}
