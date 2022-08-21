
#ifndef CMC_CMC_DEQUE_TEST_H
#define CMC_CMC_DEQUE_TEST_H

#include "macro_collections.h"

struct deque
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    size_t front;
    size_t back;
    int flag;
    struct deque_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct deque_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct deque *d_new(size_t capacity, struct deque_fval *f_val);
struct deque *d_new_custom(size_t capacity, struct deque_fval *f_val, struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks);
void d_clear(struct deque *_deque_);
void d_free(struct deque *_deque_);
void d_customize(struct deque *_deque_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool d_push_front(struct deque *_deque_, size_t value);
_Bool d_push_back(struct deque *_deque_, size_t value);
_Bool d_pop_front(struct deque *_deque_);
_Bool d_pop_back(struct deque *_deque_);
size_t d_front(struct deque *_deque_);
size_t d_back(struct deque *_deque_);
_Bool d_contains(struct deque *_deque_, size_t value);
_Bool d_empty(struct deque *_deque_);
_Bool d_full(struct deque *_deque_);
size_t d_count(struct deque *_deque_);
size_t d_capacity(struct deque *_deque_);
int d_flag(struct deque *_deque_);
_Bool d_resize(struct deque *_deque_, size_t capacity);
struct deque *d_copy_of(struct deque *_deque_);
_Bool d_equals(struct deque *_deque1_, struct deque *_deque2_);
struct deque d_init(size_t capacity, struct deque_fval *f_val);
struct deque d_init_custom(size_t capacity, struct deque_fval *f_val, struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks);
void d_release(struct deque _deque_);
struct deque_iter
{
    struct deque *target;
    size_t cursor;
    size_t index;
    _Bool start;
    _Bool end;
};
struct deque_iter d_iter_start(struct deque *target);
struct deque_iter d_iter_end(struct deque *target);
_Bool d_iter_at_start(struct deque_iter *iter);
_Bool d_iter_at_end(struct deque_iter *iter);
_Bool d_iter_to_start(struct deque_iter *iter);
_Bool d_iter_to_end(struct deque_iter *iter);
_Bool d_iter_next(struct deque_iter *iter);
_Bool d_iter_prev(struct deque_iter *iter);
_Bool d_iter_advance(struct deque_iter *iter, size_t steps);
_Bool d_iter_rewind(struct deque_iter *iter, size_t steps);
_Bool d_iter_go_to(struct deque_iter *iter, size_t index);
size_t d_iter_value(struct deque_iter *iter);
size_t *d_iter_rvalue(struct deque_iter *iter);
size_t d_iter_index(struct deque_iter *iter);
_Bool d_to_string(struct deque *_deque_, FILE *fptr);
_Bool d_print(struct deque *_deque_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_DEQUE_TEST_H */
