/*******************************************************************************
*   (c) 2016 Ledger
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
#pragma once

#include <stdbool.h>
#include "apdu_codes.h"

#define CLA                             0x22

#define OFFSET_CLA                      0
#define OFFSET_INS                      1  //< Instruction offset
#define OFFSET_P1                       2  //< P1
#define OFFSET_P2                       3  //< P2
#define OFFSET_DATA_LEN                 4  //< Data Length
#define OFFSET_DATA                     5  //< Data offset

#define APDU_MIN_LENGTH                 5

#define OFFSET_PCK_INDEX                OFFSET_P1  //< Package index offset
#define OFFSET_PCK_COUNT                OFFSET_P2  //< Package count offset

#define INS_GET_VERSION                 0
#define INS_GET_ADDR_ED25519            1
#define INS_SIGN_ED25519                2

#define BIP44_PATH_0                    (0x80000000 | 0x2c)
#define BIP44_PATH_1                    (0x80000000 | 0xea)

void app_init();

void app_main();
