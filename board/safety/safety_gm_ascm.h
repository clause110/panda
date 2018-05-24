// BUS 0 is on the LKAS module (ASCM) side
// BUS 2 is on the actuator (EPS) side

static int gm_ascm_fwd_hook(int bus_num, CAN_FIFOMailBox_TypeDef *to_fwd) {

  uint32_t addr = to_fwd->RIR>>21;

  if (bus_num == 0) {
    // do not propagate lkas messages from ascm to actuators
    // block 0x152 and 0x154, which are the lkas command from ASCM1 and ASCM2
    if ((addr == 0x152) || (addr == 0x154)) {
      return -1;
    }
    // on the chassis bus, the OBDII port is on the module side, so we need to read
    // the lkas messages sent by openpilot (put on unused 0x153 addr) and send it to 
    // the actuator as 0x154
    if (addr == 0x153) {
      to_fwd->RIR = (0x154 << 21) | (to_fwd->RIR & 0x1fffff);
    }
    return 2;
  }

  if (bus_num == 2) {
    return 0;
  }

  return -1;
}

static int gm_ascm_ign_hook() {
  return -1;
}

const safety_hooks gm_ascm_hooks = {
  .init = nooutput_init,
  .rx = default_rx_hook,
  .tx = alloutput_tx_hook,
  .tx_lin = nooutput_tx_lin_hook,
  .ignition = gm_ascm_ign_hook,
  .fwd = gm_ascm_fwd_hook,
};
