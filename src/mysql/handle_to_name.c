#include "handle_to_name.h"
#include "abstract_mem.h"
#include "gsh_types.h"
#include "hashtable.h"
#include "log.h"
#include <stdint.h>
#include <string.h>

void mdc_hashkey_encode(uint64_t hk, struct gsh_buffdesc* buf){
	buf->len = sizeof(hk);
	buf->addr = gsh_malloc(buf->len);
	*(uint64_t*)(buf->addr) = hk;
}

uint32_t mdc_hashkey_partition(struct hash_param *hparam, struct gsh_buffdesc *key) {
    uint64_t index;
    memcpy(&index, key->addr, sizeof(index));
    return (uint32_t) (index % hparam->index_size);
}

uint64_t mdc_hashkey_hash(struct hash_param *hparam, struct gsh_buffdesc* key) {
    return *(uint64_t *)(key->addr);
}

int mdc_hashkey_equal(struct gsh_buffdesc *k1, struct gsh_buffdesc *k2) {
	uint64_t hk1, hk2;
	hk1 = *(uint64_t*)(k1->addr);
	hk2 = *(uint64_t*)(k2->addr);
	return (hk1 == hk2) ? 0 : 1;
}

static hash_parameter_t cid_unconfirmed_hash_param = {
	.index_size = HANDLE_TO_NAME_HT_PRIME,
	.hash_func_key = mdc_hashkey_partition,
	.hash_func_rbt = mdc_hashkey_hash,
	.hash_func_both = NULL,
	.compare_key = mdc_hashkey_equal,
	.display_key = NULL,
	.display_val = NULL,
	.ht_name = "MDC Handle to filename HT",
	.flags = HT_FLAG_CACHE,
	.ht_log_component = COMPONENT_CACHE_INODE,
};

hash_table_t* mdc_mysql_ht = NULL;

int mdc_mysql_ht_init() {}

int mdc_mysql_filetype(uint64_t hk)  {
    struct hash_latch latch;
    struct gsh_buffdesc key, val;
	const char *fname;
    int status;

	mdc_hashkey_encode(hk, &key);
	status = hashtable_getlatch(mdc_mysql_ht, &key, &val, true, &latch);

	switch (status) {
	case HASHTABLE_SUCCESS:
		fname = val.addr;
		LogDebug(COMPONENT_CACHE_INODE, "Get from MDC_MYSQL_HT: %s", fname);
		if (memcmp(fname + val.len - 4, ".ibd", 4)) {
			hashtable_releaselatched(mdc_mysql_ht, &latch);
			status = MYSQL_DATA_FILE;
		} else {
			status = MYSQL_LOG_FILE;
		}
		break;
	case HASHTABLE_ERROR_NO_SUCH_KEY:
		status = NON_MYSQL_FILE;
		break;
	default:
	};
	return status;
}