
#ifndef CMC_CMC_HASHBIDIMAP_TEST_H
#define CMC_CMC_HASHBIDIMAP_TEST_H

#include "macro_collections.h"

struct hashbidimap
{
    struct hashbidimap_entry *(*buffer)[2];
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct hashbidimap_fkey *f_key;
    struct hashbidimap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct hashbidimap_iter (*it_start)(struct hashbidimap *);
    struct hashbidimap_iter (*it_end)(struct hashbidimap *);
};
struct hashbidimap_entry
{
    size_t key;
    size_t value;
    size_t dist[2];
    struct hashbidimap_entry **ref[2];
};
struct hashbidimap_fkey
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashbidimap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashbidimap_iter
{
    struct hashbidimap *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashbidimap *hbm_new(size_t capacity, double load, struct hashbidimap_fkey *f_key,
                            struct hashbidimap_fval *f_val);
struct hashbidimap *hbm_new_custom(size_t capacity, double load, struct hashbidimap_fkey *f_key,
                                   struct hashbidimap_fval *f_val, struct cmc_alloc_node *alloc,
                                   struct cmc_callbacks *callbacks);
void hbm_clear(struct hashbidimap *_map_);
void hbm_free(struct hashbidimap *_map_);
void hbm_customize(struct hashbidimap *_map_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool hbm_insert(struct hashbidimap *_map_, size_t key, size_t value);
_Bool hbm_update_key(struct hashbidimap *_map_, size_t val, size_t new_key);
_Bool hbm_update_val(struct hashbidimap *_map_, size_t key, size_t new_val);
_Bool hbm_remove_by_key(struct hashbidimap *_map_, size_t key, size_t *out_key, size_t *out_val);
_Bool hbm_remove_by_val(struct hashbidimap *_map_, size_t val, size_t *out_key, size_t *out_val);
size_t hbm_get_key(struct hashbidimap *_map_, size_t val);
size_t hbm_get_val(struct hashbidimap *_map_, size_t key);
_Bool hbm_contains_key(struct hashbidimap *_map_, size_t key);
_Bool hbm_contains_val(struct hashbidimap *_map_, size_t val);
_Bool hbm_empty(struct hashbidimap *_map_);
_Bool hbm_full(struct hashbidimap *_map_);
size_t hbm_count(struct hashbidimap *_map_);
size_t hbm_capacity(struct hashbidimap *_map_);
double hbm_load(struct hashbidimap *_map_);
int hbm_flag(struct hashbidimap *_map_);
_Bool hbm_resize(struct hashbidimap *_map_, size_t capacity);
struct hashbidimap *hbm_copy_of(struct hashbidimap *_map_);
_Bool hbm_equals(struct hashbidimap *_map1_, struct hashbidimap *_map2_);
struct hashbidimap_iter *hbm_iter_new(struct hashbidimap *target);
void hbm_iter_free(struct hashbidimap_iter *iter);
void hbm_iter_init(struct hashbidimap_iter *iter, struct hashbidimap *target);
_Bool hbm_iter_start(struct hashbidimap_iter *iter);
_Bool hbm_iter_end(struct hashbidimap_iter *iter);
void hbm_iter_to_start(struct hashbidimap_iter *iter);
void hbm_iter_to_end(struct hashbidimap_iter *iter);
_Bool hbm_iter_next(struct hashbidimap_iter *iter);
_Bool hbm_iter_prev(struct hashbidimap_iter *iter);
_Bool hbm_iter_advance(struct hashbidimap_iter *iter, size_t steps);
_Bool hbm_iter_rewind(struct hashbidimap_iter *iter, size_t steps);
_Bool hbm_iter_go_to(struct hashbidimap_iter *iter, size_t index);
size_t hbm_iter_key(struct hashbidimap_iter *iter);
size_t hbm_iter_value(struct hashbidimap_iter *iter);
size_t hbm_iter_index(struct hashbidimap_iter *iter);
_Bool hbm_to_string(struct hashbidimap *_map_, FILE *fptr);
_Bool hbm_print(struct hashbidimap *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
                const char *key_val_sep);

#endif /* CMC_CMC_HASHBIDIMAP_TEST_H */
