
#ifndef CMC_CMC_HASHMULTISET_TEST_H
#define CMC_CMC_HASHMULTISET_TEST_H

#include "macro_collections.h"

struct hashmultiset
{
    struct hashmultiset_entry *buffer;
    size_t capacity;
    size_t count;
    size_t cardinality;
    double load;
    int flag;
    struct hashmultiset_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct hashmultiset_entry
{
    size_t value;
    size_t multiplicity;
    size_t dist;
    enum cmc_entry_state state;
};
struct hashmultiset_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashmultiset *hms_new(size_t capacity, double load, struct hashmultiset_fval *f_val);
struct hashmultiset *hms_new_custom(size_t capacity, double load, struct hashmultiset_fval *f_val,
                                    struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
void hms_clear(struct hashmultiset *_set_);
void hms_free(struct hashmultiset *_set_);
void hms_customize(struct hashmultiset *_set_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool hms_insert(struct hashmultiset *_set_, size_t value);
_Bool hms_insert_many(struct hashmultiset *_set_, size_t value, size_t count);
_Bool hms_update(struct hashmultiset *_set_, size_t value, size_t multiplicity);
_Bool hms_remove(struct hashmultiset *_set_, size_t value);
size_t hms_remove_all(struct hashmultiset *_set_, size_t value);
_Bool hms_max(struct hashmultiset *_set_, size_t *value);
_Bool hms_min(struct hashmultiset *_set_, size_t *value);
size_t hms_multiplicity_of(struct hashmultiset *_set_, size_t value);
_Bool hms_contains(struct hashmultiset *_set_, size_t value);
_Bool hms_empty(struct hashmultiset *_set_);
_Bool hms_full(struct hashmultiset *_set_);
size_t hms_count(struct hashmultiset *_set_);
size_t hms_cardinality(struct hashmultiset *_set_);
size_t hms_capacity(struct hashmultiset *_set_);
double hms_load(struct hashmultiset *_set_);
int hms_flag(struct hashmultiset *_set_);
_Bool hms_resize(struct hashmultiset *_set_, size_t capacity);
struct hashmultiset *hms_copy_of(struct hashmultiset *_set_);
_Bool hms_equals(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
struct hashmultiset_iter
{
    struct hashmultiset *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashmultiset_iter hms_iter_start(struct hashmultiset *target);
struct hashmultiset_iter hms_iter_end(struct hashmultiset *target);
_Bool hms_iter_at_start(struct hashmultiset_iter *iter);
_Bool hms_iter_at_end(struct hashmultiset_iter *iter);
_Bool hms_iter_to_start(struct hashmultiset_iter *iter);
_Bool hms_iter_to_end(struct hashmultiset_iter *iter);
_Bool hms_iter_next(struct hashmultiset_iter *iter);
_Bool hms_iter_prev(struct hashmultiset_iter *iter);
_Bool hms_iter_advance(struct hashmultiset_iter *iter, size_t steps);
_Bool hms_iter_rewind(struct hashmultiset_iter *iter, size_t steps);
_Bool hms_iter_go_to(struct hashmultiset_iter *iter, size_t index);
size_t hms_iter_value(struct hashmultiset_iter *iter);
size_t hms_iter_multiplicity(struct hashmultiset_iter *iter);
size_t hms_iter_index(struct hashmultiset_iter *iter);
struct hashmultiset *hms_union(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
struct hashmultiset *hms_intersection(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
struct hashmultiset *hms_difference(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
struct hashmultiset *hms_summation(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
struct hashmultiset *hms_symmetric_difference(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
_Bool hms_is_subset(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
_Bool hms_is_superset(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
_Bool hms_is_proper_subset(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
_Bool hms_is_proper_superset(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
_Bool hms_is_disjointset(struct hashmultiset *_set1_, struct hashmultiset *_set2_);
_Bool hms_to_string(struct hashmultiset *_set_, FILE *fptr);
_Bool hms_print(struct hashmultiset *_set_, FILE *fptr, const char *start, const char *separator, const char *end,
                const char *key_val_sep);

#endif /* CMC_CMC_HASHMULTISET_TEST_H */
