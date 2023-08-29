#ifndef _MYSQL_HANDLE_TO_NAME
#define _MYSQL_HANDLE_TO_NAME

#include "gsh_types.h"
#include "hashtable.h"
#include <stdint.h>
#define HANDLE_TO_NAME_HT_PRIME 7

enum MDC_MYSQL_FILETYPE{
    NON_MYSQL_FILE = 0,
    MYSQL_DATA_FILE = 1,
    MYSQL_LOG_FILE = 2,
};

/*
 * @brief reserve
 */
void mdc_hashkey_encode(uint64_t hk, struct gsh_buffdesc* buf);

int mdc_mysql_filetype(uint64_t hk);

#endif