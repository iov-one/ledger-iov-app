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
#include <stdint.h>
#include <stddef.h>
#include "parser_txdef.h"

void parser_coinInit(parser_coin_t *coin) {
    coin->whole = 0;
    coin->fractional = 0;
    coin->ticker = NULL;
    coin->tickerLen = 0;
}

void parser_feesInit(parser_fees_t *fees) {
    fees->payer = NULL;
    fees->payerLen = 0;
    parser_coinInit(&fees->coin);
}

void parser_sendmsgInit(parser_sendmsg_t *msg) {
    msg->metadata = 0;
    msg->source = NULL;
    msg->sourceLen = 0;
    msg->destination = NULL;
    msg->destinationLen = 0;
    parser_coinInit(&msg->amount);
    msg->memo = NULL;
    msg->memoLen = 0;
    msg->ref = NULL;
    msg->refLen = 0;
}

void parser_txInit(parser_tx_t *tx) {
    tx->version = NULL;
    tx->chainID = NULL;
    tx->nonce = NULL;
    parser_feesInit(&tx->fees);
    parser_sendmsgInit(&tx->sendmsg);
}
