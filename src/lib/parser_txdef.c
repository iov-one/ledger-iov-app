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
#include <stddef.h>
#include "parser_txdef.h"

void parser_metadataInit(parser_metadata_t *metadata) {
    metadata->seen.schema = 0;

    metadata->schema = 0;
}

void parser_coinInit(parser_coin_t *coin) {
    coin->seen.whole = 0;
    coin->seen.fractional = 0;
    coin->seen.ticker = 0;

    coin->whole = 0;
    coin->fractional = 0;
    coin->tickerPtr = NULL;
    coin->tickerLen = 0;
}

void parser_feesInit(parser_fees_t *fees) {
    fees->seen.payer = 0;
    fees->seen.coin = 0;

    fees->payerPtr = NULL;
    fees->payerLen = 0;

    fees->coinPtr = NULL;
    fees->coinLen = 0;
    parser_coinInit(&fees->coin);
}

void parser_sendmsgInit(parser_sendmsg_t *msg) {
    msg->seen.metadata = 0;
    msg->seen.source = 0;
    msg->seen.destination = 0;
    msg->seen.amount = 0;
    msg->seen.memo = 0;
    msg->seen.ref = 0;

    msg->metadataPtr = NULL;
    msg->metadataLen = 0;
    parser_metadataInit(&msg->metadata);

    msg->sourcePtr = NULL;
    msg->sourceLen = 0;

    msg->destinationPtr = NULL;
    msg->destinationLen = 0;

    msg->amountPtr = NULL;
    msg->amountLen = 0;
    parser_coinInit(&msg->amount);

    msg->memoPtr = NULL;
    msg->memoLen = 0;

    msg->refPtr = NULL;
    msg->refLen = 0;
}

void parser_txInit(parser_tx_t *tx) {
    tx->version = NULL;
    tx->chainID = NULL;
    tx->nonce = 0;

    tx->feesPtr = NULL;
    tx->feesLen = 0;
    parser_feesInit(&tx->fees);

    tx->sendmsgPtr = NULL;
    tx->sendmsgLen = 0;
    parser_sendmsgInit(&tx->sendmsg);
}
