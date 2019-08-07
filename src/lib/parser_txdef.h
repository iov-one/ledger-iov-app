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

// TODO: create a single toString
typedef struct {
    int64_t whole;
    int64_t fractional;
    uint8_t *ticker;
    uint8_t tickerLen;
} parser_coin_t;

typedef struct {
    uint8_t *payer;
    uint16_t payerLen;
    parser_coin_t coin;
} parser_fees_t;

typedef struct {
    uint32_t metadata;
    uint8_t *source;
    uint16_t sourceLen;
    uint8_t *destination;
    uint16_t destinationLen;
    parser_coin_t amount;
    uint8_t *memo;
    uint16_t memoLen;
    uint8_t *ref;
    uint16_t refLen;
} parser_sendmsg_t;

typedef struct {
    uint8_t *version;
    uint8_t chainIDLen;
    uint8_t *chainID;
    int64_t *nonce;
    parser_fees_t fees;
    parser_sendmsg_t sendmsg;
} parser_tx_t;

void parser_coinInit(parser_coin_t *coin);
void parser_feesInit(parser_fees_t *fees);
void parser_sendmsgInit(parser_sendmsg_t *msg);
void parser_txInit(parser_tx_t *tx);

//version | len(chainID) | chainID      | nonce             | signBytes
//4bytes  | uint8        | ascii string | int64 (bigendian) | serialized transaction

//1, 2    Fees: Payer           [Addr - Bytes]
//1, 3    Fees: Coin            [Coin] -> Stringify

//51, 1   SendMsg: Metadata     [?????]
//51, 2   SendMsg: Source       [Addr - Bytes]
//51, 3   SendMsg: Destination  [Addr - Bytes]
//51, 4   SendMsg: Amount       [Coin] -> Stringify
//51, 5   SendMsg: Memo         [String]
//51, 6   SendMsg: Ref          [?????]

//[Coin] -> Stringify
//?, 1    Whole
//?, 2    Fractional
//?, 3    Ticker

#ifdef __cplusplus
}
#endif
