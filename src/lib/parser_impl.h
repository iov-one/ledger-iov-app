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
} parser_error_t;

typedef struct {
    const uint8_t *buffer;
    uint16_t bufferSize;
    uint16_t offset;
    uint16_t lastConsumed;
    parser_error_t lastError;
    parser_tx_t tx;
} parser_context_t;

#define WIRE_TYPE_VARINT   0            // Zigzag is not supported
#define WIRE_TYPE_64BIT    1            // TODO: Not supported, throw error
#define WIRE_TYPE_LEN      2
#define WIRE_TYPE_32BIT    5            // TODO: Not supported, throw error

#define FIELD_NUM(x) ((x) >> 3u)
#define WIRE_TYPE(x) ((uint8_t)((x) & 0x7u))

void parser_init(parser_context_t *ctx, const uint8_t *buffer, uint16_t bufferSize);

uint64_t readRawVarint(parser_context_t *ctx);
uint64_t readVarint(parser_context_t *ctx);
const uint8_t *readString(parser_context_t *ctx, uint64_t *stringLen);

#ifdef __cplusplus
}
#endif
