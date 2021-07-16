#include "rodos.h"

#include "hal/hal_ethernet.h"
#include "rodos-lwip.h"

HAL_ETH halEth(ETH_IDX0);

IPAddr ip = {192,168,42,2};
IPAddr mask = {255,255,255,0};
IPAddr gw = {192,168,42,254};

IPStack ipstack(&halEth, &ip, &mask, &gw);
