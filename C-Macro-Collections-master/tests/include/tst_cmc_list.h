
#ifndef CMC_CMC_LIST_TEST_H
#define CMC_CMC_LIST_TEST_H

#include "macro_collections.h"

struct list
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    int flag;
    struct list_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct list_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct list *l_new(size_t capacity, struct list_fval *f_val);
struct list *l_new_custom(size_t capacity, struct list_fval *f_val, struct cmc_alloc_node *alloc,
                          struct cmc_callbacks *callbacks);
void l_clear(struct list *_list_);
void l_free(struct list *_list_);
void l_customize(struct list *_list_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool l_push_front(struct list *_list_, size_t value);
_Bool l_push_at(struct list *_list_, size_t value, size_t index);
_Bool l_push_back(struct list *_list_, size_t value);
_Bool l_pop_front(struct list *_list_);
_Bool l_pop_at(struct list *_list_, size_t index);
_Bool l_pop_back(struct list *_list_);
size_t l_front(struct list *_list_);
size_t l_get(struct list *_list_, size_t index);
size_t *l_get_ref(struct list *_list_, size_t index);
size_t l_back(struct list *_list_);
size_t l_index_of(struct list *_list_, size_t value, _Bool from_start);
_Bool l_contains(struct list *_list_, size_t value);
_Bool l_empty(struct list *_list_);
_Bool l_full(struct list *_list_);
size_t l_count(struct list *_list_);
_Bool l_fits(struct list *_list_, size_t size);
size_t l_capacity(struct list *_list_);
int l_flag(struct list *_list_);
_Bool l_resize(struct list *_list_, size_t capacity);
struct list *l_copy_of(struct list *_list_);
_Bool l_equals(struct list *_list1_, struct list *_list2_);
struct list_iter
{
    struct list *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct list_iter l_iter_start(struct list *target);
struct list_iter l_iter_end(struct list *target);
_Bool l_iter_at_start(struct list_iter *iter);
_Bool l_iter_at_end(struct list_iter *iter);
_Bool l_iter_to_start(struct list_iter *iter);
_Bool l_iter_to_end(struct list_iter *iter);
_Bool l_iter_next(struct list_iter *iter);
_Bool l_iter_prev(struct list_iter *iter);
_Bool l_iter_advance(struct list_iter *iter, size_t steps);
_Bool l_iter_rewind(struct list_iter *iter, size_t steps);
_Bool l_iter_go_to(struct list_iter *iter, size_t index);
size_t l_iter_value(struct list_iter *iter);
size_t *l_iter_rvalue(struct list_iter *iter);
size_t l_iter_index(struct list_iter *iter);
_Bool l_seq_push_front(struct list *_list_, size_t *values, size_t size);
_Bool l_seq_push_at(struct list *_list_, size_t *values, size_t size, size_t index);
_Bool l_seq_push_back(struct list *_list_, size_t *values, size_t size);
_Bool l_seq_pop_at(struct list *_list_, size_t from, size_t to);
struct list *l_seq_sublist(struct list *_list_, size_t from, size_t to);
_Bool l_to_string(struct list *_list_, FILE *fptr);
_Bool l_print(struct list *_list_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_LIST_TEST_H */
