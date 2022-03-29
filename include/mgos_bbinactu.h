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

#ifndef MGOS_BBINACTU_H_
#define MGOS_BBINACTU_H_

#include <stdbool.h>
#include "mgos_bthing.h"
#include "mgos_bactuator.h"
#include "mgos_bbinsens.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mg_bthing_actu;
typedef struct mg_bthing_actu *mgos_bbinactu_t;

mgos_bthing_t MGOS_BBINACTU_THINGCAST(mgos_bbinactu_t actuator);

mgos_bbinsens_t MGOS_BBINACTU_SENSCAST(mgos_bbinactu_t actuator);

mgos_bactuator_t MGOS_BBINACTU_DOWNCAST(mgos_bbinactu_t actuator);

mgos_bbinactu_t mgos_bbinactu_create(const char *id, const char *domain);

bool mgos_bbinactu_set_state(mgos_bbinactu_t actuator, bool state);

bool mgos_bbinactu_toggle_state(mgos_bbinactu_t actuator);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_BBINACTU_H_ */