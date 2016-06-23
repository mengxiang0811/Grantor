#include "modules.h"

static char *MODULE_NAME[IC_MODULE_MAX] = {
    "basic",
    "lpm",
};

static char *MODULE_DESCRIPTION[IC_MODULE_MAX] = {
    "Basic Module",
    "LPM Module",
};

ic_module_init  module_init_funcs[IC_MODULE_MAX] = {
    IC_MODULE_INIT_WITH_NAME(basic),
    IC_MODULE_INIT_WITH_NAME(lpm),
};

ic_module_cleanup  module_cleanup_funcs[IC_MODULE_MAX] = {
    IC_MODULE_CLEANUP_WITH_NAME(basic),
    IC_MODULE_CLEANUP_WITH_NAME(lpm),
};

void modules_init() {

    int i = 0;
    int ret = -1;

    ret = common_init();

    if (ret < 0) {
        printf("ERROR: init the common parts!");
        exit(-1);
    }


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

void modules_cleanup() {

    int i = 0;
    int ret = -1;

    common_cleanup();

    for (i = 0; i < IC_MODULE_MAX; i++) {

        printf("Cleanup the [%s]\n", MODULE_DESCRIPTION[i]);

        ret = module_cleanup_funcs[i]();

        if (ret < 0) {
            printf("ERROR: cleanup the [%s]\n", MODULE_DESCRIPTION[i]);
            exit(-1);
        }

        printf("\n");
    }
}
