#ifndef __LPM_H
#define __LPM_H

#include "comm.h"

int get_lpm_rule_fname_init();
int get_lpm_rule_fname(int sockfd);

int get_lpm_rule_data_init();
int get_lpm_rule_data(int sockfd);

#endif
