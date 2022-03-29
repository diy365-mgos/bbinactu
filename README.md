# bBinaryActuators Library
## Overview
A bBinaryActuator is a specilized version of [bActuator](https://github.com/diy365-mgos/bactuator). It allows you to easily manage bActuators having just binary (true/false) state.
## Features
- **Verbose state** - Optionally, you can configure verbose state instead of default booleans (e.g: 'ON'/'OFF'). This feature is inherited form [bBinarySensors](https://github.com/diy365-mgos/bbinsens#features).
- **GPIO as actuators** - You can easily manage GPIO as actuators. Just include the [bThings GPIO library](https://github.com/diy365-mgos/bthing-gpio) in your project.
## GET STARTED
Copy, build and flash one of the following ready-to-use firmwares.
#### Example 1 - Toggle GPIO state
Create a bActuator for toggling the GPIO #2 state.

Include these libraries in your mos.yml file.
```yaml
libs:
  - origin: https://github.com/diy365-mgos/bbinactu
  - origin: https://github.com/diy365-mgos/bthing-gpio
```
```c
#include "mgos.h"
#include "mgos_bbinactu.h"
#include "mgos_bthing_gpio.h"

static int gpio_pin = 2; // LED GPIO

static void actuator_state_changed_cb(int ev, void *ev_data, void *userdata) {
  struct mgos_bthing_state *arg = (struct mgos_bthing_state *)ev_data;

  LOG(LL_INFO, ("Binary actuator '%s' (on GPIO %d) state: %s",
    mgos_bthing_get_uid(arg->thing), gpio_pin, mgos_bvar_get_str(arg->state)));
}

void simulate_external_trigger(void *param) {
  mgos_bbinactu_toggle_state((mgos_bbinactu_t)param);
}

enum mgos_app_init_result mgos_app_init(void) {
  mgos_event_add_handler(MGOS_EV_BTHING_STATE_CHANGED, actuator_state_changed_cb, NULL);

  /* create the actuator */
  mgos_bbinactu_t actu = mgos_bbinactu_create("actu1", NULL);
  mgos_bbinsens_set_verbose_state(MGOS_BBINACTU_SENSCAST(actu), "ON", "OFF");
  /* attach GPIO  */
  mgos_bthing_gpio_attach(MGOS_BBINACTU_THINGCAST(actu), gpio_pin, false, MGOS_BTHING_GPIO_PULL_AUTO);

  // Simulate an external trigger for changing actuator state
  mgos_set_timer(5000, MGOS_TIMER_REPEAT, simulate_external_trigger, actu);
  
  return MGOS_APP_INIT_SUCCESS;
}
```
## Inherited APIs
A bBinaryActuator inherits inherits APIs from:
- [bThing](https://github.com/diy365-mgos/bthing)
- [bBinarySensor](https://github.com/diy365-mgos/bbinsens)
- [bActuator](https://github.com/diy365-mgos/bactuator)
### Remarks on: mgos_bthing_on_get_state()
The [get-state handler](https://github.com/diy365-mgos/bthing#mgos_bthing_get_state_handler_t) must set `true` or `false` the `state` parameter.
```c
static bool my_get_state_handler(mgos_bthing_t thing, mgos_bvar_t state, void *userdata) {
  bool sensor_state;
  // sensor_state = ... get the physical actuator state (true or false)
  mgos_bvar_set_bool(state, sensor_state);
  return true;
}
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
mgos_bthing_on_get_state(MGOS_BBINACTU_THINGCAST(actu), my_get_state_handler, NULL);
```
### Remarks on: mgos_bthing_get_state()
The [mgos_bthing_get_state()](https://github.com/diy365-mgos/bthing#mgos_bthing_get_state) returns a boolean value or a string value in case [verbose state](https://github.com/diy365-mgos/bbinsens#mgos_bbinsens_set_verbose_state) is configured. Alternatively you can use the [mgos_bbinsens_get_state()](https://github.com/diy365-mgos/bbinsens#mgos_bbinsens_get_state) helper function.
```c
// standard (bool) state
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
mgos_bvarc_t state = mgos_bthing_get_state(MGOS_BBINACTU_THINGCAST(actu));
bool actu_state = mgos_bvar_get_bool(state);

// verbose state
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
mgos_bbinsens_set_verbose_state(MGOS_BBINACTU_SENSCAST(actu), "ON", "OFF");
mgos_bvarc_t state = mgos_bthing_get_state(MGOS_BBINACTU_THINGCAST(actu));
const char *actu_state = mgos_bvar_get_str(state);
```
### Remarks on: mgos_bthing_on_set_state()
The `state` parameter value in the [set-state handler](https://github.com/diy365-mgos/bthing#mgos_bthing_set_state_handler_t) is a boolean value.
```c
static bool my_set_state_handler(mgos_bthing_t thing, mgos_bvarc_t state, void *userdata) {
  bool state_to_set = mgos_bvar_get_bool(state);
  // set actuator state...
  return true;
}
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
mgos_bthing_on_set_state(MGOS_BBINACTU_THINGCAST(actu), my_set_state_handler, NULL);
```
### Remarks on: mgos_bthing_set_state()
The [mgos_bthing_set_state()](https://github.com/diy365-mgos/bthing#mgos_bthing_set_state) allows boolean values as input parameter. In case [verbose state](https://github.com/diy365-mgos/bbinsens#mgos_bbinsens_set_verbose_state) is configured, string values are allowed instead. Alternatively you can use [mgos_bbinactu_set_state()](#mgos_bbinactu_set_state) or [mgos_bbinactu_toggle_state](#mgos_bbinactu_toggle_state) helper functions.
```c
// standard (bool) state
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
mgos_bvar_t state = mgos_bvar_new_bool(true);
mgos_bthing_set_state(MGOS_BBINACTU_THINGCAST(actu), MGOS_BVAR_CONST(state));
mgos_bvar_free(state);

// verbose state
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
mgos_bbinsens_set_verbose_state(MGOS_BBINACTU_SENSCAST(actu), "ON", "OFF");
mgos_bvar_t state = mgos_bvar_new_str("ON");
mgos_bthing_set_state(MGOS_BBINACTU_THINGCAST(actu), MGOS_BVAR_CONST(state));
mgos_bvar_free(state);
```
## C/C++ APIs Reference
### MGOS_BBINACTU_TYPE
```c
#define MGOS_BBINACTU_TYPE 
```
The bBinaryActuator type ID. It can be used with [mgos_bthing_is_typeof()](https://github.com/diy365-mgos/bthing#mgos_bthing_is_typeof).

Example:
```c
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
if (mgos_bthing_is_typeof(MGOS_BBINACTU_THINGCAST(actu), MGOS_BBINACTU_TYPE))
  LOG(LL_INFO, ("I'm a bBinaryActuator."));
if (mgos_bthing_is_typeof(MGOS_BBINACTU_THINGCAST(actu), MGOS_BACTUATOR_TYPE))
  LOG(LL_INFO, ("I'm a bActuator."));
if (mgos_bthing_is_typeof(MGOS_BBINACTU_THINGCAST(actu), MGOS_BTHING_TYPE_ACTUATOR))
  LOG(LL_INFO, ("I'm a bThing actuator."));
if (mgos_bthing_is_typeof(MGOS_BBINACTU_THINGCAST(actu), MGOS_BBINSENS_TYPE))
  LOG(LL_INFO, ("I'm a bBinarySensor."));
if (mgos_bthing_is_typeof(MGOS_BBINACTU_THINGCAST(actu), MGOS_BSENSOR_TYPE))
  LOG(LL_INFO, ("I'm a bSensor."));
if (mgos_bthing_is_typeof(MGOS_BBINACTU_THINGCAST(actu), MGOS_BTHING_TYPE_SENSOR))
  LOG(LL_INFO, ("I'm a bThing sensor."));
```
Output console:
```bash
I'm a bBinaryActuator.
I'm a bActuator.
I'm a bThing actuator.
I'm a bBinarySensor.
I'm a bSensor.
I'm a bThing sensor.
```
### MGOS_BBINACTU_THINGCAST
```c
mgos_bthing_t MGOS_BBINACTU_THINGCAST(mgos_bbinactu_t actuator);
```
Casts a bBinaryActuator to a generic bThing to be used with [inherited bThing APIs](https://github.com/diy365-mgos/bthing).

|Parameter||
|--|--|
|actuator|A bBinaryActuator.|

Example:
```c
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
LOG(LL_INFO, ("%s successfully created.", mgos_bthing_get_uid(MGOS_BBINACTU_THINGCAST(actu))));
```
### MGOS_BBINACTU_SENSCAST
```c
mgos_bbinsens_t MGOS_BBINACTU_SENSCAST(mgos_bbinactu_t actuator);
```
Casts a bBinaryActuator to a bBinarySensor to be used with [inherited bBinarySensor APIs](https://github.com/diy365-mgos/bbinsens).

|Parameter||
|--|--|
|actuator|A bBinaryActuator.|

Example:
```c
mgos_bbinactu_t actu = mgos_bbinactu_create(...);
bool state;
if (mgos_bbinsens_get_state(MGOS_BBINACTU_SENSCAST(actu), &state))
  LOG(LL_INFO, ("The state is %s.", (state ? "TRUE" : "FALSE")));
```
### MGOS_BBINACTU_DOWNCAST
```c
mgos_bactuator_t MGOS_BBINACTU_DOWNCAST(mgos_bbinactu_t actuator);
```
Casts a bBinaryActuator to a bActuator to be used with [inherited bActuator APIs](https://github.com/diy365-mgos/bactuator).

|Parameter||
|--|--|
|actuator|A bBinaryActuator.|
### mgos_bbinactu_create
```c
mgos_bbinactu_t mgos_bbinactu_create(const char *id, const char *domain);
```
Creates a bBinaryActuator. Returns `NULL` on error.

|Parameter||
|--|--|
|id|The bBinaryActuator ID.|
|domain|The domain name or `NULL`.|
### mgos_bbinactu_set_state
```c
bool mgos_bbinactu_set_state(mgos_bbinactu_t actuator, bool state);
```
Sets the binary state of a bBinaryActuator. Returns `true` on success, or `false` otherwise. This is an helper function for [mgos_bthing_set_state()](https://github.com/diy365-mgos/bthing#mgos_bthing_set_state).

|Parameter||
|--|--|
|actuator|A bBinaryActuator.|
|state|The binary state to set.|
### mgos_bbinactu_toggle_state
```c
bool mgos_bbinactu_toggle_state(mgos_bbinactu_t actuator);
```
Toggles the binary state of a bBinaryActuator. Returns `true` on success, or `false` otherwise.

|Parameter||
|--|--|
|actuator|A bBinaryActuator.|
## To Do
- Implement javascript APIs for [Mongoose OS MJS](https://github.com/mongoose-os-libs/mjs).
