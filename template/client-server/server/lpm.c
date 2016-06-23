#include "lpm.h"

extern int errno;
extern char *message;
extern struct ic_request    req;

static struct rule_files rfs;

static struct ic_component icc_lpm[] = {
    {IC_LPM_FNAME_CMD, get_lpm_rule_fname, get_lpm_rule_fname_init, default_icc_cleanup_func},
    {IC_LPM_DATA_CMD, get_lpm_rule_data, get_lpm_rule_data_init, default_icc_cleanup_func},
};

int ic_lpm_module_init() {
    return IC_COMPONENT_ARRAY_REGISTER(icc_lpm, IC_COMPONENT_NUM(lpm));
}

int ic_lpm_module_cleanup() {
    return IC_COMPONENT_ARRAY_UNREGISTER(icc_lpm, IC_COMPONENT_NUM(lpm));
}

static int rule_files_init(char *directory, struct rule_files *rfiles) {

    int err;
    struct stat st;

    err = stat(directory, &st);

    if (err == 0) {
        if (st.st_mode & S_IFDIR)
            printf("%s is a directory!\n", directory);

        if (st.st_mode & S_IFREG) {
            printf("%s is a regular file!\n", directory);
            printf("Please confirm the configuration for storing LPM rules!\n");

            return -1;
        }
    } else {
        printf("Get stat on directory [%s] failed!\n", directory);

        if (errno == ENOENT) {
            printf("The directory [%s] does not exist! \
                    Creating new directory...\n", directory);

            err = mkdir(directory, S_IRWXU);

            if (err != 0) {
                printf("mkdir failed! errno = %s\n",  strerror(errno));

                return -1;
            }
            else {
                printf("Created the directory %s\n", directory);
            }
        }
    }

    DIR *dir;
    struct dirent *dp;
    dir = opendir(directory);

    while ((dp = readdir(dir)) != NULL) {

        /* the directories . and .. are ignored */
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {

            strncpy(rfiles->fname[rfiles->num_active_files++], dp->d_name, strlen(dp->d_name));

            //printf("DEBUG: \"%s\"\n", dp->d_name);
        }
    }

    closedir(dir);

    return 0;
}

int get_lpm_rule_fname_init() {

    int ret = -1;

    memset(&rfs, 0, sizeof(struct rule_files));

    /* no new rule file is updating */
    rfs.cur_updating_file_idx = -1;

    ret = rule_files_init(RULE_FILE_DIR, &rfs);

    int i = 0;

    for (i = 0; i < rfs.num_active_files; i++)
        printf("Find active rule file: %s\n", rfs.fname[i]);

    return ret;
}

int get_lpm_rule_data_init() {
    /* TODO: update the LPM rules in Grantor */
    return 0;
}

int get_lpm_rule_fname(int sockfd) {

    int i = 0;
    int ret = -1;

    ret = receive_data_msg(sockfd, req.length);

    if (ret < 0)
        return ret;

    /* receive the filename correctly, then store filename */
    char filename[MAX_FNAME_LEN];
    memset(filename, 0, sizeof(filename));
    strncpy(filename, RULE_FILE_DIR, strlen(RULE_FILE_DIR));
    strncpy(filename + strlen(RULE_FILE_DIR), message, req.length);

    for (i = 0; i < rfs.num_active_files; i++) {
        if (strcmp(rfs.fname[i], filename) == 0) {
            rfs.cur_updating_file_idx = i;
            break;
        }
    }

    if (rfs.cur_updating_file_idx == -1) {

        rfs.cur_updating_file_idx = rfs.num_active_files;
        strncpy(rfs.fname[rfs.num_active_files++], filename, strlen(filename));

        printf("Get new LPM rule fname: %s\n", rfs.fname[rfs.cur_updating_file_idx]);
    } else {
        printf("Updating old LPM rule fname: %s\n", rfs.fname[rfs.cur_updating_file_idx]);
    }

    return 0;
}

int get_lpm_rule_data(int sockfd) {
    int ret = -1;

    /* receive the content of the file */
    ret = receive_data_msg(sockfd, req.length);

    if (ret < 0)
        return ret;

    /* TODO: store the data to file*/

    if (rfs.cur_updating_file_idx == -1) {
        printf("ERROR: no LPM rule file is waiting for updating!\n");
        return -1;
    }

    FILE *fp = fopen(rfs.fname[rfs.cur_updating_file_idx], "w");

    if (!fp) {
        printf("ERROR: openning file %s! %s\n", rfs.fname[rfs.num_active_files - 1], strerror(errno));
        return -1;
    }

    size_t write_len = fwrite(message, sizeof(char), req.length, fp);

    if (write_len != req.length) {
        printf("ERROR: writing to file with lenghth %lu, but expected length %d\n", write_len, req.length);

        fclose(fp);

        return -1;
    }

    fclose(fp);

    return 0;
}
