
#ifndef CMC_CMC_TREEMAP_TEST_H
#define CMC_CMC_TREEMAP_TEST_H

#include "macro_collections.h"

struct treemap
{
    struct treemap_node *root;
    size_t count;
    int flag;
    struct treemap_fkey *f_key;
    struct treemap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct treemap_node
{
    size_t key;
    size_t value;
    unsigned char height;
    struct treemap_node *right;
    struct treemap_node *left;
    struct treemap_node *parent;
};
struct treemap_fkey
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct treemap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct treemap *tm_new(struct treemap_fkey *f_key, struct treemap_fval *f_val);
struct treemap *tm_new_custom(struct treemap_fkey *f_key, struct treemap_fval *f_val, struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks);
void tm_clear(struct treemap *_map_);
void tm_free(struct treemap *_map_);
void tm_customize(struct treemap *_map_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool tm_insert(struct treemap *_map_, size_t key, size_t value);
_Bool tm_update(struct treemap *_map_, size_t key, size_t new_value, size_t *old_value);
_Bool tm_remove(struct treemap *_map_, size_t key, size_t *out_value);
_Bool tm_max(struct treemap *_map_, size_t *key, size_t *value);
_Bool tm_min(struct treemap *_map_, size_t *key, size_t *value);
size_t tm_get(struct treemap *_map_, size_t key);
size_t *tm_get_ref(struct treemap *_map_, size_t key);
_Bool tm_contains(struct treemap *_map_, size_t key);
_Bool tm_empty(struct treemap *_map_);
size_t tm_count(struct treemap *_map_);
int tm_flag(struct treemap *_map_);
struct treemap *tm_copy_of(struct treemap *_map_);
_Bool tm_equals(struct treemap *_map1_, struct treemap *_map2_);
struct treemap_iter
{
    struct treemap *target;
    struct treemap_node *cursor;
    struct treemap_node *first;
    struct treemap_node *last;
    size_t index;
    _Bool start;
    _Bool end;
};
struct treemap_iter tm_iter_start(struct treemap *target);
struct treemap_iter tm_iter_end(struct treemap *target);
_Bool tm_iter_at_start(struct treemap_iter *iter);
_Bool tm_iter_at_end(struct treemap_iter *iter);
_Bool tm_iter_to_start(struct treemap_iter *iter);
_Bool tm_iter_to_end(struct treemap_iter *iter);
_Bool tm_iter_next(struct treemap_iter *iter);
_Bool tm_iter_prev(struct treemap_iter *iter);
_Bool tm_iter_advance(struct treemap_iter *iter, size_t steps);
_Bool tm_iter_rewind(struct treemap_iter *iter, size_t steps);
_Bool tm_iter_go_to(struct treemap_iter *iter, size_t index);
size_t tm_iter_key(struct treemap_iter *iter);
size_t tm_iter_value(struct treemap_iter *iter);
size_t *tm_iter_rvalue(struct treemap_iter *iter);
size_t tm_iter_index(struct treemap_iter *iter);
_Bool tm_to_string(struct treemap *_map_, FILE *fptr);
_Bool tm_print(struct treemap *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
               const char *key_val_sep);

#endif /* CMC_CMC_TREEMAP_TEST_H */
