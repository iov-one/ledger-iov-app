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

#include <stdio.h>
#include <zxmacros.h>
#include "parser.h"
#include "iov.h"

// 0  source
// 1  destination
// 2  amount  value / ticker
// 3  fees  value / ticker
// 4  memo                      (when exists)

parser_error_t parser_parse(parser_context_t *ctx,
                            uint8_t *data,
                            uint16_t dataLen) {
    parser_init(ctx, data, dataLen);
    parser_error_t err = parser_Tx(ctx);
    if (err != parser_ok)
        return err;

    return parser_ok;
}

uint8_t parser_getNumItems(parser_context_t *ctx) {
    if (parser_tx_obj.sendmsg.memoLen > 0)
        return 5;

    return 4;
}

parser_error_t parser_print(parser_context_t *ctx,
                            int8_t fieldIdx,
                            char *out, uint16_t outLen,
                            uint8_t pageIdx, uint8_t *pageCount) {
    return parser_no_data;
}

parser_error_t parser_getItem(parser_context_t *ctx,
                              int8_t displayIdx,
                              char *outKey, uint16_t outKeyLen,
                              char *outValue, uint16_t outValueLen,
                              uint8_t pageIdx, uint8_t *pageCount) {

    snprintf(outKey, outKeyLen, "?");
    snprintf(outValue, outValueLen, "?");

#define UI_BUFFER 250
    uint8_t buffer[UI_BUFFER];
    MEMSET(buffer, 0, UI_BUFFER);

    parser_error_t err = parser_ok;
    *pageCount = 1;
    switch (displayIdx) {
        case 0:     // Source
            snprintf(outKey, outKeyLen, "Source");
            err = parser_getAddress(parser_tx_obj.chainID, parser_tx_obj.chainIDLen,
                                    (char *) buffer, UI_BUFFER,
                                    parser_tx_obj.sendmsg.sourcePtr,
                                    parser_tx_obj.sendmsg.sourceLen);
            // page it
            parser_arrayToString(outValue, outValueLen, buffer,
                                 strlen((char *) buffer), pageIdx, pageCount);
            break;
        case 1:     // Destination
            snprintf(outKey, outKeyLen, "Dest");
            err = parser_getAddress(parser_tx_obj.chainID, parser_tx_obj.chainIDLen,
                                    (char *) buffer, UI_BUFFER,
                                    parser_tx_obj.sendmsg.destinationPtr,
                                    parser_tx_obj.sendmsg.destinationLen);
            // page it
            parser_arrayToString(outValue, outValueLen, buffer,
                                 strlen((char *) buffer), pageIdx, pageCount);
            break;
        case 2: {
            char ticker[IOV_TICKER_MAXLEN];
            err = parser_arrayToString(ticker, IOV_TICKER_MAXLEN,
                                       parser_tx_obj.sendmsg.amount.tickerPtr,
                                       parser_tx_obj.sendmsg.amount.tickerLen,
                                       0, NULL);
            if (err != parser_ok)
                return err;

            snprintf(outKey, outKeyLen, "Amount [%s]", ticker);
            err = parser_formatAmountFriendly(outValue,
                                              outValueLen,
                                              &parser_tx_obj.sendmsg.amount);
            break;
        }
        case 3: {
            char ticker[IOV_TICKER_MAXLEN];
            err = parser_arrayToString(ticker, IOV_TICKER_MAXLEN,
                                       parser_tx_obj.sendmsg.amount.tickerPtr,
                                       parser_tx_obj.sendmsg.amount.tickerLen,
                                       0, NULL);
            if (err != parser_ok)
                return err;

            snprintf(outKey, outKeyLen, "Fees [%s]", ticker);
            err = parser_formatAmountFriendly(outValue,
                                              outValueLen,
                                              &parser_tx_obj.fees.coin);
            break;
        }
        case 4:     // Memo
            snprintf(outKey, outKeyLen, "Memo");
            err = parser_arrayToString((char *) buffer, UI_BUFFER,
                                       parser_tx_obj.sendmsg.memoPtr,
                                       parser_tx_obj.sendmsg.memoLen,
                                       0, NULL);
            asciify((char *) buffer);
            // page it
            parser_arrayToString(outValue, outValueLen, buffer,
                                 strlen((char *) buffer),
                                 pageIdx, pageCount);
            break;
        default:
            *pageCount = 0;
            return parser_no_data;
    }

    return err;
}
