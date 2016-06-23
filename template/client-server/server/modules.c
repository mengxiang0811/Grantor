#include "modules.h"

ic_module_init  module_init_funcs[IC_MODULE_MAX] = {
    IC_MODULE_INIT_WITH_NAME(basic),
    IC_MODULE_INIT_WITH_NAME(lpm),
};

static char *MODULE_NAME[IC_MODULE_MAX] = {
    "basic",
    "lpm",
};

static char *MODULE_DESCRIPTION[IC_MODULE_MAX] = {
    "Basic Module",
    "LPM Module",
};

void modules_init() {

    int i = 0;
    int ret = -1;

    common_init();

    for (i = 0; i < IC_MODULE_MAX; i++) {

        printf("Initialize the [%s]\n", MODULE_DESCRIPTION[i]);

        ret = module_init_funcs[i]();

        if (ret < 0) {
            printf("ERROR: init the [%s]\n", MODULE_DESCRIPTION[i]);
            exit(-1);
        }

        printf("\n");
    }
}
