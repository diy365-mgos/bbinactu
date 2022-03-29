/*
 * Copyright (c) 2021 DIY356
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MG_BBINACTU_SDK_H_
#define MG_BBINACTU_SDK_H_

#include <stdbool.h>
#include "mg_bthing_sdk.h"
#include "mg_bbinsens_sdk.h"
#include "mg_bactuator_sdk.h"
#include "mgos_bbinactu.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MG_BBINACTU_NEW(a) MG_BTHING_ACTU_NEW(a);

/*****************************************
 * Cast Functions
 */

// Convert (mgos_bbinactu_t) into (struct mg_bthing_sens *)
struct mg_bthing_sens *MG_BBINACTU_CAST1(mgos_bbinactu_t thing);

// Convert (mgos_bbinactu_t) into (struct mg_bthing_actu *)
struct mg_bthing_actu *MG_BBINACTU_CAST2(mgos_bbinactu_t thing);
/*****************************************/

#define MG_BBINACTU_CFG(a) ((struct mg_bbinactu_cfg *)MG_BBINACTU_CAST2(a)->cfg)

struct mg_bbinactu_overrides {
  mg_bthing_setting_state_handler_t setting_state_cb;
};

struct mg_bbinactu_cfg {
  struct mg_bactuator_cfg base;
  struct mg_bbinactu_overrides overrides; 
};

bool mg_bbinactu_init(mgos_bbinactu_t actu,
                        struct mg_bbinactu_cfg *actu_cfg,
                        struct mg_bbinsens_cfg *sens_cfg);

void mg_bbinactu_reset(mgos_bbinactu_t actu);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_BBINACTU_SDK_H_ */