#ifndef __IC_LPM_H
#define __IC_LPM_H

#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "component.h"

#define MAX_FNAME_LEN       128
#define MAX_RULE_FILE_NUM   128

#define RULE_FILE_DIR       "./rule/"

struct rule_files {
    int cur_updating_file_idx;
    int num_active_files;

    /* file name */
    char fname[MAX_RULE_FILE_NUM][MAX_FNAME_LEN];

    /* 0 means never updated yet
     * maybe useful for future debugging
     * Not used yet
     * */
    time_t file_last_update_time[MAX_RULE_FILE_NUM];
};

int ic_lpm_module_init();
int ic_lpm_module_cleanup();

int get_lpm_rule_fname_init();
int get_lpm_rule_fname(int sockfd);

int get_lpm_rule_data_init();
int get_lpm_rule_data(int sockfd);

#endif
