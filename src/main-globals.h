#ifndef MAIN_GLOBALS_H
#define MAIN_GLOBALS_H
#include <time.h>
#include "dict.h"

extern unsigned volatile is_tx_done;
extern unsigned volatile is_rx_done;
extern time_t global_now;
extern Dict tcp_port_service_db;
extern Dict udp_port_service_db;
extern Dict sctp_port_service_db;

#endif
