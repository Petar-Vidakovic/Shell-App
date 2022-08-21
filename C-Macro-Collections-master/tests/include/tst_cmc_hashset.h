
#ifndef CMC_CMC_HASHSET_TEST_H
#define CMC_CMC_HASHSET_TEST_H

#include "macro_collections.h"

struct hashset
{
    struct hashset_entry *buffer;
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct hashset_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct hashset_entry
{
    size_t value;
    size_t dist;
    enum cmc_entry_state state;
};
struct hashset_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashset *hs_new(size_t capacity, double load, struct hashset_fval *f_val);
struct hashset *hs_new_custom(size_t capacity, double load, struct hashset_fval *f_val, struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks);
void hs_clear(struct hashset *_set_);
void hs_free(struct hashset *_set_);
void hs_customize(struct hashset *_set_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool hs_insert(struct hashset *_set_, size_t value);
_Bool hs_remove(struct hashset *_set_, size_t value);
_Bool hs_max(struct hashset *_set_, size_t *value);
_Bool hs_min(struct hashset *_set_, size_t *value);
_Bool hs_contains(struct hashset *_set_, size_t value);
_Bool hs_empty(struct hashset *_set_);
_Bool hs_full(struct hashset *_set_);
size_t hs_count(struct hashset *_set_);
size_t hs_capacity(struct hashset *_set_);
double hs_load(struct hashset *_set_);
int hs_flag(struct hashset *_set_);
_Bool hs_resize(struct hashset *_set_, size_t capacity);
struct hashset *hs_copy_of(struct hashset *_set_);
_Bool hs_equals(struct hashset *_set1_, struct hashset *_set2_);
struct hashset_iter
{
    struct hashset *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashset_iter hs_iter_start(struct hashset *target);
struct hashset_iter hs_iter_end(struct hashset *target);
_Bool hs_iter_at_start(struct hashset_iter *iter);
_Bool hs_iter_at_end(struct hashset_iter *iter);
_Bool hs_iter_to_start(struct hashset_iter *iter);
_Bool hs_iter_to_end(struct hashset_iter *iter);
_Bool hs_iter_next(struct hashset_iter *iter);
_Bool hs_iter_prev(struct hashset_iter *iter);
_Bool hs_iter_advance(struct hashset_iter *iter, size_t steps);
_Bool hs_iter_rewind(struct hashset_iter *iter, size_t steps);
_Bool hs_iter_go_to(struct hashset_iter *iter, size_t index);
size_t hs_iter_value(struct hashset_iter *iter);
size_t hs_iter_index(struct hashset_iter *iter);
struct hashset *hs_union(struct hashset *_set1_, struct hashset *_set2_);
struct hashset *hs_intersection(struct hashset *_set1_, struct hashset *_set2_);
struct hashset *hs_difference(struct hashset *_set1_, struct hashset *_set2_);
struct hashset *hs_symmetric_difference(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_subset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_superset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_proper_subset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_proper_superset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_disjointset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_to_string(struct hashset *_set_, FILE *fptr);
_Bool hs_print(struct hashset *_set_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_HASHSET_TEST_H */
