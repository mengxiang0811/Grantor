#include "lpm.h"

static struct rule_files rfs;

extern int errno;
extern char *message;
extern struct ic_request    req;

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

    ret = rule_files_init(RULE_FILE_DIR, &rfs);

    int i = 0;

    for (i = 0; i < rfs.num_active_files; i++)
        printf("Find active rule file: %s\n", rfs.fname[i]);

    return ret;
}

int get_lpm_rule_data_init() {
    return 0;
}

int get_lpm_rule_fname(int sockfd) {
    int ret = -1;

    ret = receive_data_msg(sockfd, req.length);

    if (ret < 0)
        return ret;

    /* receive the filename correctly, then store filename */

    strncpy(rfs.fname[rfs.num_active_files], RULE_FILE_DIR, strlen(RULE_FILE_DIR));
    strncpy(rfs.fname[rfs.num_active_files++] + strlen(RULE_FILE_DIR), message, req.length);

    printf("Get new fname: %s\n", rfs.fname[rfs.num_active_files - 1]);

    return 0;
}

int get_lpm_rule_data(int sockfd) {
    int ret = -1;

    /* receive the content of the file */
    ret = receive_data_msg(sockfd, req.length);

    if (ret < 0)
        return ret;

    /* TODO: store the data to file*/

    FILE *fp = fopen(rfs.fname[rfs.num_active_files - 1], "w");

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
