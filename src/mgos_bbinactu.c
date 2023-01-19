#include "mgos.h"
#include "mgos_bthing.h"
#include "mg_bbinactu_sdk.h"
#include "mgos_bbinactu.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif

mgos_bthing_t MGOS_BBINACTU_THINGCAST(mgos_bbinactu_t actuator) {
  return MG_BTHING_SENS_CAST4(MG_BBINACTU_CAST1(actuator));
}

mgos_bsensor_t MGOS_BBINACTU_SENSCAST(mgos_bbinactu_t actuator) {
  return (mgos_bsensor_t)MG_BBINACTU_CAST1(actuator);
}

mgos_bactuator_t MGOS_BBINACTU_DOWNCAST(mgos_bbinactu_t actuator) {
  return (mgos_bactuator_t)actuator;
}

mgos_bbinactu_t mgos_bbinactu_create(const char *id, const char *domain) {
  mgos_bbinactu_t MG_BBINACTU_NEW(actu);
  if (mg_bthing_init(MG_BTHING_ACTU_CAST4(actu), id, MGOS_BBINACTU_TYPE, domain)) {
    struct mg_bbinactu_cfg *actu_cfg = calloc(1, sizeof(struct mg_bbinactu_cfg));
    struct mg_bbinsens_cfg *sens_cfg = calloc(1, sizeof(struct mg_bbinsens_cfg));
    if (sens_cfg && actu_cfg) {
      mgos_bthing_t thing = MGOS_BBINACTU_THINGCAST(actu);
      if (mg_bbinactu_init(actu, actu_cfg, sens_cfg) && mg_bthing_register(thing)) {
        LOG(LL_INFO, ("bBinaryActuator '%s' successfully created.", mgos_bthing_get_uid(thing)));
        return actu;
      }
    } else {
      LOG(LL_ERROR, ("Unable to allocate memory for 'mg_bbinsens_cfg' and/or 'mg_bbinactu_cfg'"));
    }
    free(sens_cfg);
    free(actu_cfg);
    mg_bthing_reset(MG_BTHING_ACTU_CAST4(actu));
  }
  free(actu);
  LOG(LL_ERROR, ("Error creating bBinaryActuator '%s'. See above error message for more details.'", (id ? id : "")));
  return NULL; 
}

bool mgos_bbinactu_set_state(mgos_bbinactu_t actuator, bool state) {
  bool ret = false;
  if (actuator) {
    mgos_bbinsens_t sens = MGOS_BBINACTU_SENSCAST(actuator);
    mgos_bvar_t var_state = mgos_bvar_new();
    if (mg_bbinsens_bool_to_state(sens, state, var_state)) {
      LOG(LL_INFO, ("mgos_bbinactu_set_state()"));
      ret = mgos_bthing_set_state(MGOS_BBINACTU_THINGCAST(actuator), MGOS_BVAR_CONST(var_state));
    }
    mgos_bvar_free(var_state);
  }
  return ret;
}

bool mgos_bbinactu_toggle_state(mgos_bbinactu_t actuator) {
  bool state;
  if (mgos_bbinsens_get_state(MGOS_BBINACTU_SENSCAST(actuator), &state)) {
    LOG(LL_INFO, ("mgos_bbinactu_toggle_state()"));
    return mgos_bbinactu_set_state(actuator, !state);
  }
  return false;
}

bool mgos_bbinactu_init() {
  return true;
}