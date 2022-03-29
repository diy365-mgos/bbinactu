#include "mgos.h"
#include "mg_bbinactu_sdk.h"

static mgos_bvar_t s_bool_state = NULL;

/*****************************************
 * Cast Functions
 */

// Convert (mgos_bbinactu_t) into (struct mg_bthing_sens *)
struct mg_bthing_sens *MG_BBINACTU_CAST1(mgos_bbinactu_t thing) {
  return (struct mg_bthing_sens *)thing;
}

// Convert (mgos_bbinactu_t) into (struct mg_bthing_actu *)
struct mg_bthing_actu *MG_BBINACTU_CAST2(mgos_bbinactu_t thing) {
  return (struct mg_bthing_actu *)thing;
}
/*****************************************/

enum mg_bthing_state_result mg_bbinactu_setting_state_cb(struct mg_bthing_actu *actu,
                                                           mgos_bvarc_t state,
                                                           void *userdata) {
  if (actu && state) {
    struct mg_bbinactu_cfg *cfg = MG_BBINACTU_CFG(actu);
    bool bool_state;
    if (mgos_bbinsens_state_parse(MGOS_BBINACTU_SENSCAST(actu), state, &bool_state)) {
      mgos_bvar_set_bool(s_bool_state, bool_state);
      return cfg->overrides.setting_state_cb(actu, MGOS_BVAR_CONST(s_bool_state), userdata);
    } else {
      LOG(LL_ERROR, ("Unable to set the state of bBinaryActuator '%s'. See above error/s for details.",
        mgos_bthing_get_uid(MG_BTHING_ACTU_CAST5(actu))));
    }
  }
  return MG_BTHING_STATE_RESULT_ERROR;
}

bool mg_bbinactu_init(mgos_bbinactu_t actu,
                        struct mg_bbinactu_cfg *actu_cfg,
                        struct mg_bbinsens_cfg *sens_cfg) {
  if (actu_cfg) {
    // init sensor-base obj (if cfg is provided) and
    // init actuator-base obj
    if ((!sens_cfg || mg_bbinsens_init(MGOS_BBINACTU_SENSCAST(actu), sens_cfg)) &&
        mg_bactuator_init(actu, &actu_cfg->base, NULL)) {
      /* initalize overrides cfg */
      actu_cfg->overrides.setting_state_cb = mg_bthing_on_setting_state(MG_BBINACTU_CAST2(actu), mg_bbinactu_setting_state_cb);

      // initialize the static temporary state variable
      if (!s_bool_state) s_bool_state = mgos_bvar_new_bool(false);
      
      return true; // initialization successfully completed
    }
    mg_bbinactu_reset(actu);
  } else {
    LOG(LL_ERROR, ("Invalid NULL 'actu_cfg' parameter."));
  }

  LOG(LL_ERROR, ("Error initializing bBinaryActuator '%s'. See above error message for more details.",
    mgos_bthing_get_uid(MGOS_BBINACTU_THINGCAST(actu))));
  return false; 
}

void mg_bbinactu_reset(mgos_bbinactu_t actu) {
  struct mg_bbinactu_cfg *cfg = MG_BBINACTU_CFG(actu);

  /* clear overrides cfg */
  if (cfg->overrides.setting_state_cb) {
    mg_bthing_on_setting_state(actu, cfg->overrides.setting_state_cb);
    cfg->overrides.setting_state_cb = NULL;
  }

  // reset actuator-base obj
  mg_bactuator_reset(actu);
  // reset sensor-base obj
  mg_bbinsens_reset(MG_BTHING_ACTU_CAST3(MG_BACTUATOR_CAST1(actu)));
}
