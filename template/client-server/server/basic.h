#ifndef __IC_BASIC_H
#define __IC_BASIC_H

#include "component.h"

int ic_basic_module_init();
int ic_basic_module_cleanup();

int connection_setup(int sockfd);
int connection_terminate(int sockfd);
int connection_terminate_directly(int sockfd);

#endif
