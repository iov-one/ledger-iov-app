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
#include "parser.h"

parser_error_t parser_parse(parser_context_t *ctx,
                            uint8_t *data,
                            uint16_t dataLen) {
    // TODO: initialize context
    return parser_ok;
}

uint8_t parser_getNumItems(parser_context_t *ctx) {
    return 0;
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
    return parser_no_data;
}
