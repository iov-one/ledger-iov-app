/*******************************************************************************
*  (c) 2019 ZondaX GmbH
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
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "parser_txdef.h"

typedef enum {
    parser_ok = 0,
    parser_no_data = 1,
    parser_unexpected_buffer_end = 2,
    parser_unexpected_wire_type = 3,
    parser_unexpected_version = 4,
    parser_unexpected_characters = 5,
    parser_unexpected_field = 6,
    parser_duplicated_field = 7,
    parser_value_out_of_range = 8,
    parser_unexpected_chain = 9,
    parser_unexpected_field_length = 10,
} parser_error_t;

typedef struct {
    const uint8_t *buffer;
    uint16_t bufferSize;
    uint16_t offset;
    uint16_t lastConsumed;
} parser_context_t;

extern parser_tx_t parser_tx_obj;

#define WIRE_TYPE_VARINT   0            // Zigzag is not supported
#define WIRE_TYPE_64BIT    1            // Not supported
#define WIRE_TYPE_LEN      2
#define WIRE_TYPE_32BIT    5            // Not supported

#define FIELD_NUM(x) ((x) >> 3u)
#define WIRE_TYPE(x) ((uint8_t)((x) & 0x7u))

parser_error_t parser_init(parser_context_t *ctx,
                           const uint8_t *buffer,
                           uint16_t bufferSize);

parser_error_t _readRawVarint(parser_context_t *ctx, uint64_t *value);

parser_error_t _readVarint(parser_context_t *ctx, uint64_t *value);

parser_error_t _readUInt32(parser_context_t *ctx, uint32_t *value);

parser_error_t _readArray(parser_context_t *ctx, const uint8_t **s, uint16_t *stringLen);

parser_error_t parser_readPB_Metadata(const uint8_t *bufferPtr,
                                      uint16_t bufferLen,
                                      parser_metadata_t *metadata);

parser_error_t parser_readPB_Coin(const uint8_t *bufferPtr,
                                  uint16_t bufferLen,
                                  parser_coin_t *coin);

parser_error_t parser_readPB_Fees(const uint8_t *bufferPtr,
                                  uint16_t bufferLen,
                                  parser_fees_t *fees);

parser_error_t parser_readPB_SendMsg(const uint8_t *bufferPtr,
                                     uint16_t bufferLen,
                                     parser_sendmsg_t *sendmsg);

parser_error_t parser_readPB_Root(parser_context_t *ctx);

parser_error_t parser_readRoot(parser_context_t *ctx);

parser_error_t parser_Tx(parser_context_t *ctx);

bool_t parser_IsMainnet(const uint8_t *chainID, uint16_t chainIDLen);

const char *parser_getHRP(const uint8_t *chainID, uint16_t chainIDLen);

parser_error_t parser_getAddress(const uint8_t *chainID, uint16_t chainIDLen,
                                 char *addr, uint16_t addrLen,
                                 const uint8_t *ptr, uint16_t len);

parser_error_t parser_arrayToString(char *out, uint16_t outLen,
                                    const uint8_t *in, uint8_t inLen,
                                    uint8_t pageIdx, uint8_t *pageCount);

parser_error_t parser_formatAmount(char *out, uint16_t outLen, parser_coin_t *coin);
parser_error_t parser_formatAmountFriendly(char *out, uint16_t outLen, parser_coin_t *coin);

#ifdef __cplusplus
}
#endif
