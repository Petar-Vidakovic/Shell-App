
#ifndef CMC_CMC_SORTEDLIST_TEST_H
#define CMC_CMC_SORTEDLIST_TEST_H

#include "macro_collections.h"

struct sortedlist
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    _Bool is_sorted;
    int flag;
    struct sortedlist_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct sortedlist_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct sortedlist *sl_new(size_t capacity, struct sortedlist_fval *f_val);
struct sortedlist *sl_new_custom(size_t capacity, struct sortedlist_fval *f_val, struct cmc_alloc_node *alloc,
                                 struct cmc_callbacks *callbacks);
void sl_clear(struct sortedlist *_list_);
void sl_free(struct sortedlist *_list_);
void sl_customize(struct sortedlist *_list_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool sl_insert(struct sortedlist *_list_, size_t value);
_Bool sl_remove(struct sortedlist *_list_, size_t index);
size_t sl_max(struct sortedlist *_list_);
size_t sl_min(struct sortedlist *_list_);
size_t sl_get(struct sortedlist *_list_, size_t index);
size_t sl_index_of(struct sortedlist *_list_, size_t value, _Bool from_start);
_Bool sl_contains(struct sortedlist *_list_, size_t value);
_Bool sl_empty(struct sortedlist *_list_);
_Bool sl_full(struct sortedlist *_list_);
size_t sl_count(struct sortedlist *_list_);
size_t sl_capacity(struct sortedlist *_list_);
int sl_flag(struct sortedlist *_list_);
_Bool sl_resize(struct sortedlist *_list_, size_t capacity);
void sl_sort(struct sortedlist *_list_);
struct sortedlist *sl_copy_of(struct sortedlist *_list_);
_Bool sl_equals(struct sortedlist *_list1_, struct sortedlist *_list2_);
struct sortedlist_iter
{
    struct sortedlist *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct sortedlist_iter sl_iter_start(struct sortedlist *target);
struct sortedlist_iter sl_iter_end(struct sortedlist *target);
_Bool sl_iter_at_start(struct sortedlist_iter *iter);
_Bool sl_iter_at_end(struct sortedlist_iter *iter);
_Bool sl_iter_to_start(struct sortedlist_iter *iter);
_Bool sl_iter_to_end(struct sortedlist_iter *iter);
_Bool sl_iter_next(struct sortedlist_iter *iter);
_Bool sl_iter_prev(struct sortedlist_iter *iter);
_Bool sl_iter_advance(struct sortedlist_iter *iter, size_t steps);
_Bool sl_iter_rewind(struct sortedlist_iter *iter, size_t steps);
_Bool sl_iter_go_to(struct sortedlist_iter *iter, size_t index);
size_t sl_iter_value(struct sortedlist_iter *iter);
size_t sl_iter_index(struct sortedlist_iter *iter);
_Bool sl_to_string(struct sortedlist *_list_, FILE *fptr);
_Bool sl_print(struct sortedlist *_list_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_SORTEDLIST_TEST_H */
