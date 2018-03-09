
static int toyota_ipas_fwd_hook(int bus_num, CAN_FIFOMailBox_TypeDef *to_fwd) {
  if (bus_num == 0) {
    if ((to_fwd->RIR>>21) == 0xb4) {
      // cap the speed at 6kph (600)
      int speed = ((to_fwd->RDHR) & 0xFF00) | ((to_fwd->RDHR >> 16) & 0xFF);
      if (speed > 600) {
        speed = 600;
      }
      int checksum = (0xb4 + 8 + speed) & 0xff;
      to_fwd->RDLR = 0;
      to_fwd->RDHR = (checksum << 24) + ((speed & 0xFF) << 16) + (speed & 0xFF00);
    }

    return 2;
  }
  if (bus_num == 2) {
    return 0;
  }
  return -1;
}

const safety_hooks toyota_ipas_hooks = {
  .init = alloutput_init,
  .rx = default_rx_hook,
  .tx = alloutput_tx_hook,
  .tx_lin = nooutput_tx_lin_hook,
  .fwd = toyota_ipas_fwd_hook,
};
