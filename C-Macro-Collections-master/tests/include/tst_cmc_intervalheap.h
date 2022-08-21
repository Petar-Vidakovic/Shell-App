
#ifndef CMC_CMC_INTERVALHEAP_TEST_H
#define CMC_CMC_INTERVALHEAP_TEST_H

#include "macro_collections.h"

struct intervalheap
{
    size_t (*buffer)[2];
    size_t capacity;
    size_t size;
    size_t count;
    int flag;
    struct intervalheap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct intervalheap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct intervalheap *ih_new(size_t capacity, struct intervalheap_fval *f_val);
struct intervalheap *ih_new_custom(size_t capacity, struct intervalheap_fval *f_val, struct cmc_alloc_node *alloc,
                                   struct cmc_callbacks *callbacks);
void ih_clear(struct intervalheap *_heap_);
void ih_free(struct intervalheap *_heap_);
void ih_customize(struct intervalheap *_heap_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool ih_insert(struct intervalheap *_heap_, size_t value);
_Bool ih_remove_max(struct intervalheap *_heap_);
_Bool ih_remove_min(struct intervalheap *_heap_);
_Bool ih_update_max(struct intervalheap *_heap_, size_t value);
_Bool ih_update_min(struct intervalheap *_heap_, size_t value);
size_t ih_max(struct intervalheap *_heap_);
size_t ih_min(struct intervalheap *_heap_);
_Bool ih_contains(struct intervalheap *_heap_, size_t value);
_Bool ih_empty(struct intervalheap *_heap_);
_Bool ih_full(struct intervalheap *_heap_);
size_t ih_count(struct intervalheap *_heap_);
size_t ih_capacity(struct intervalheap *_heap_);
int ih_flag(struct intervalheap *_heap_);
_Bool ih_resize(struct intervalheap *_heap_, size_t capacity);
struct intervalheap *ih_copy_of(struct intervalheap *_heap_);
_Bool ih_equals(struct intervalheap *_heap1_, struct intervalheap *_heap2_);
struct intervalheap_iter
{
    struct intervalheap *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct intervalheap_iter ih_iter_start(struct intervalheap *target);
struct intervalheap_iter ih_iter_end(struct intervalheap *target);
_Bool ih_iter_at_start(struct intervalheap_iter *iter);
_Bool ih_iter_at_end(struct intervalheap_iter *iter);
_Bool ih_iter_to_start(struct intervalheap_iter *iter);
_Bool ih_iter_to_end(struct intervalheap_iter *iter);
_Bool ih_iter_next(struct intervalheap_iter *iter);
_Bool ih_iter_prev(struct intervalheap_iter *iter);
_Bool ih_iter_advance(struct intervalheap_iter *iter, size_t steps);
_Bool ih_iter_rewind(struct intervalheap_iter *iter, size_t steps);
_Bool ih_iter_go_to(struct intervalheap_iter *iter, size_t index);
size_t ih_iter_value(struct intervalheap_iter *iter);
size_t ih_iter_index(struct intervalheap_iter *iter);
_Bool ih_to_string(struct intervalheap *_heap_, FILE *fptr);
_Bool ih_print(struct intervalheap *_heap_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_INTERVALHEAP_TEST_H */
