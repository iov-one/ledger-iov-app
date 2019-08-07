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

#include "parser_impl.h"

void parser_init(parser_context_t *ctx, const uint8_t *buffer, uint16_t bufferSize) {
    ctx->buffer = buffer;
    ctx->bufferSize = bufferSize;
    ctx->offset = 0;
    ctx->lastConsumed = 0;
    ctx->lastError = parser_ok;

    parser_txInit(&ctx->tx);
}

void parser_advance(parser_context_t *ctx) {
    if (ctx->lastError == parser_ok) {
        ctx->offset += ctx->lastConsumed;
        ctx->lastConsumed = 0;
    }
}

void parser_error(parser_context_t *ctx, parser_error_t err) {
    ctx->lastError = err;
    ctx->lastConsumed = 0;
}

const char *parser_getErrorDescription(parser_error_t err) {
    switch (err) {
        // FIXME: complete
        case parser_ok:
            return "No error";
        case parser_no_data:
            return "No more data";
        case parser_unexpected_buffer_end:
            return "Unexpected buffer end";
        case parser_unexpected_wire_type:
            return "Unexpected wire type";
        default:
            return "Unrecognized error code";
    }
}

uint64_t readRawVarint(parser_context_t *ctx) {
    uint16_t offset = ctx->offset + ctx->lastConsumed;
    uint64_t value = 0;
    uint16_t consumed = 0;

    const uint8_t *p = ctx->buffer + offset;
    const uint8_t *end = ctx->buffer + ctx->bufferSize + 1;

    ctx->lastError = parser_ok;

    // Extract value
    uint16_t shift = 0;
    while (p < end && shift < 64) {
        value += ((*p) & 0x7Fu) << shift;
        consumed++;
        if (!(*p & 0x80u)) {
            ctx->lastConsumed += consumed;
            return value;
        }
        shift += 7;
        p++;
    }

    ctx->lastError = parser_unexpected_buffer_end;
    return 0;
}

uint64_t readVarint(parser_context_t *ctx) {
    ctx->lastError = parser_ok;
    ctx->lastConsumed = 0;

    uint64_t v = readRawVarint(ctx);
    if (ctx->lastError != parser_ok) {
        ctx->lastConsumed = 0;
        return 0;
    }
    if (WIRE_TYPE(v) != WIRE_TYPE_VARINT) {
        parser_error(ctx, parser_unexpected_wire_type);
        return 0;
    }

    v = readRawVarint(ctx);
    parser_advance(ctx);
    return v;
}

const uint8_t *readString(parser_context_t *ctx, uint64_t *stringLen) {
    ctx->lastError = parser_ok;
    ctx->lastConsumed = 0;

    uint64_t v = readRawVarint(ctx);
    if (ctx->lastError != parser_ok) {
        ctx->lastConsumed = 0;
        return 0;
    }
    if (WIRE_TYPE(v) != WIRE_TYPE_LEN) {
        parser_error(ctx, parser_unexpected_wire_type);
        return 0;
    }

    *stringLen = readRawVarint(ctx);
    if (ctx->lastError != parser_ok) {
        ctx->lastConsumed = 0;
        return NULL;
    }

    // check that the returned buffer is not out of bounds
    if (ctx->offset + ctx->lastConsumed + *stringLen > ctx->bufferSize) {
        parser_error(ctx, parser_unexpected_buffer_end);
        return NULL;
    }

    const uint8_t *s = ctx->buffer + ctx->offset + ctx->lastConsumed;
    ctx->lastConsumed += *stringLen;
    parser_advance(ctx);
    return s;
}

void parser_readTx(parser_context_t *ctx) {
// TODO: Enumerate fields
// TODO: Handle default values
// TODO: Check for required fields
// TODO: Check for duplicates
// TODO: Check for unknown fields
}

// TODO: allow queries
