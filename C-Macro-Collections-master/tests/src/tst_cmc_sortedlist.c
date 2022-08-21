
#include "tst_cmc_sortedlist.h"

static size_t sl_impl_binary_search_first(struct sortedlist *_list_, size_t value);
static size_t sl_impl_binary_search_last(struct sortedlist *_list_, size_t value);
void sl_impl_sort_quicksort(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high);
void sl_impl_sort_insertion(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high);
struct sortedlist *sl_new(size_t capacity, struct sortedlist_fval *f_val)
{
    return sl_new_custom(capacity, f_val, ((void *)0), ((void *)0));
}
struct sortedlist *sl_new_custom(size_t capacity, struct sortedlist_fval *f_val, struct cmc_alloc_node *alloc,
                                 struct cmc_callbacks *callbacks)
{
    ;
    if (capacity < 1)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct sortedlist *_list_ = alloc->malloc(sizeof(struct sortedlist));
    if (!_list_)
        return ((void *)0);
    _list_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_list_->buffer)
    {
        alloc->free(_list_);
        return ((void *)0);
    }
    _list_->capacity = capacity;
    _list_->count = 0;
    _list_->is_sorted = 0;
    _list_->flag = CMC_FLAG_OK;
    _list_->f_val = f_val;
    _list_->alloc = alloc;
    (_list_)->callbacks = callbacks;
    return _list_;
}
void sl_clear(struct sortedlist *_list_)
{
    if (_list_->f_val->free)
    {
        for (size_t i = 0; i < _list_->count; i++)
            _list_->f_val->free(_list_->buffer[i]);
    }
    memset(_list_->buffer, 0, sizeof(size_t) * _list_->capacity);
    _list_->count = 0;
    _list_->flag = CMC_FLAG_OK;
}
void sl_free(struct sortedlist *_list_)
{
    if (_list_->f_val->free)
    {
        for (size_t i = 0; i < _list_->count; i++)
            _list_->f_val->free(_list_->buffer[i]);
    }
    _list_->alloc->free(_list_->buffer);
    _list_->alloc->free(_list_);
}
void sl_customize(struct sortedlist *_list_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!alloc)
        _list_->alloc = &cmc_alloc_node_default;
    else
        _list_->alloc = alloc;
    (_list_)->callbacks = callbacks;
    _list_->flag = CMC_FLAG_OK;
}
_Bool sl_insert(struct sortedlist *_list_, size_t value)
{
    if (sl_full(_list_))
    {
        if (!sl_resize(_list_, _list_->capacity * 2))
            return 0;
    }
    _list_->buffer[_list_->count++] = value;
    _list_->is_sorted = 0;
    _list_->flag = CMC_FLAG_OK;
    if ((_list_)->callbacks && (_list_)->callbacks->create)
        (_list_)->callbacks->create();
    ;
    return 1;
}
_Bool sl_remove(struct sortedlist *_list_, size_t index)
{
    if (sl_empty(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    if (index >= _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return 0;
    }
    memmove(_list_->buffer + index, _list_->buffer + index + 1, (_list_->count - index) * sizeof(size_t));
    _list_->buffer[--_list_->count] = (size_t){ 0 };
    _list_->flag = CMC_FLAG_OK;
    if ((_list_)->callbacks && (_list_)->callbacks->delete)
        (_list_)->callbacks->delete ();
    ;
    return 1;
}
size_t sl_max(struct sortedlist *_list_)
{
    if (sl_empty(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    sl_sort(_list_);
    _list_->flag = CMC_FLAG_OK;
    if ((_list_)->callbacks && (_list_)->callbacks->read)
        (_list_)->callbacks->read();
    ;
    return _list_->buffer[_list_->count - 1];
}
size_t sl_min(struct sortedlist *_list_)
{
    if (sl_empty(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    sl_sort(_list_);
    _list_->flag = CMC_FLAG_OK;
    if ((_list_)->callbacks && (_list_)->callbacks->read)
        (_list_)->callbacks->read();
    ;
    return _list_->buffer[0];
}
size_t sl_get(struct sortedlist *_list_, size_t index)
{
    if (sl_empty(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    if (index >= _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return (size_t){ 0 };
    }
    sl_sort(_list_);
    _list_->flag = CMC_FLAG_OK;
    if ((_list_)->callbacks && (_list_)->callbacks->read)
        (_list_)->callbacks->read();
    ;
    return _list_->buffer[index];
}
size_t sl_index_of(struct sortedlist *_list_, size_t value, _Bool from_start)
{
    _list_->flag = CMC_FLAG_OK;
    sl_sort(_list_);
    if ((_list_)->callbacks && (_list_)->callbacks->read)
        (_list_)->callbacks->read();
    ;
    if (from_start)
    {
        return sl_impl_binary_search_first(_list_, value);
    }
    return sl_impl_binary_search_last(_list_, value);
}
_Bool sl_contains(struct sortedlist *_list_, size_t value)
{
    _list_->flag = CMC_FLAG_OK;
    if (sl_empty(_list_))
        return 0;
    sl_sort(_list_);
    if ((_list_)->callbacks && (_list_)->callbacks->read)
        (_list_)->callbacks->read();
    ;
    return sl_impl_binary_search_first(_list_, value) < _list_->count;
}
_Bool sl_empty(struct sortedlist *_list_)
{
    return _list_->count == 0;
}
_Bool sl_full(struct sortedlist *_list_)
{
    return _list_->count >= _list_->capacity;
}
size_t sl_count(struct sortedlist *_list_)
{
    return _list_->count;
}
size_t sl_capacity(struct sortedlist *_list_)
{
    return _list_->capacity;
}
int sl_flag(struct sortedlist *_list_)
{
    return _list_->flag;
}
_Bool sl_resize(struct sortedlist *_list_, size_t capacity)
{
    if (_list_->capacity == capacity)
        goto success;
    if (capacity < _list_->count)
    {
        _list_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    size_t *new_buffer = _list_->alloc->realloc(_list_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
    {
        _list_->flag = CMC_FLAG_ALLOC;
        return 0;
    }
    _list_->buffer = new_buffer;
    _list_->capacity = capacity;
success:
    _list_->flag = CMC_FLAG_OK;
    if ((_list_)->callbacks && (_list_)->callbacks->resize)
        (_list_)->callbacks->resize();
    ;
    return 1;
}
void sl_sort(struct sortedlist *_list_)
{
    _list_->flag = CMC_FLAG_OK;
    if (!_list_->is_sorted && _list_->count > 1)
    {
        sl_impl_sort_quicksort(_list_->buffer, _list_->f_val->cmp, 0, _list_->count - 1);
        _list_->is_sorted = 1;
    }
}
struct sortedlist *sl_copy_of(struct sortedlist *_list_)
{
    struct sortedlist *result = sl_new_custom(_list_->capacity, _list_->f_val, _list_->alloc, ((void *)0));
    if (!result)
    {
        _list_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    (result)->callbacks = _list_->callbacks;
    if (_list_->f_val->cpy)
    {
        for (size_t i = 0; i < _list_->count; i++)
            result->buffer[i] = _list_->f_val->cpy(_list_->buffer[i]);
    }
    else
        memcpy(result->buffer, _list_->buffer, sizeof(size_t) * _list_->count);
    result->count = _list_->count;
    _list_->flag = CMC_FLAG_OK;
    return result;
}
_Bool sl_equals(struct sortedlist *_list1_, struct sortedlist *_list2_)
{
    if (_list1_->count != _list2_->count)
        return 0;
    sl_sort(_list1_);
    sl_sort(_list2_);
    for (size_t i = 0; i < _list1_->count; i++)
    {
        if (_list1_->f_val->cmp(_list1_->buffer[i], _list2_->buffer[i]) != 0)
            return 0;
    }
    return 1;
}
static size_t sl_impl_binary_search_first(struct sortedlist *_list_, size_t value)
{
    if (sl_empty(_list_))
        return 1;
    size_t L = 0;
    size_t R = _list_->count;
    while (L < R)
    {
        size_t M = L + (R - L) / 2;
        if (_list_->f_val->cmp(_list_->buffer[M], value) < 0)
            L = M + 1;
        else
            R = M;
    }
    if (_list_->f_val->cmp(_list_->buffer[L], value) == 0)
        return L;
    return _list_->count;
}
static size_t sl_impl_binary_search_last(struct sortedlist *_list_, size_t value)
{
    if (sl_empty(_list_))
        return 1;
    size_t L = 0;
    size_t R = _list_->count;
    while (L < R)
    {
        size_t M = L + (R - L) / 2;
        if (_list_->f_val->cmp(_list_->buffer[M], value) > 0)
            R = M;
        else
            L = M + 1;
    }
    if (L > 0 && _list_->f_val->cmp(_list_->buffer[L - 1], value) == 0)
        return L - 1;
    return _list_->count;
}
void sl_impl_sort_quicksort(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high)
{
    while (low < high)
    {
        if (high - low < 10)
        {
            sl_impl_sort_insertion(array, cmp, low, high);
            break;
        }
        else
        {
            size_t pivot = array[high];
            size_t pindex = low;
            for (size_t i = low; i < high; i++)
            {
                if (cmp(array[i], pivot) <= 0)
                {
                    size_t _tmp_ = array[i];
                    array[i] = array[pindex];
                    array[pindex] = _tmp_;
                    pindex++;
                }
            }
            size_t _tmp_ = array[pindex];
            array[pindex] = array[high];
            array[high] = _tmp_;
            if (pindex - low < high - pindex)
            {
                sl_impl_sort_quicksort(array, cmp, low, pindex - 1);
                low = pindex + 1;
            }
            else
            {
                sl_impl_sort_quicksort(array, cmp, pindex + 1, high);
                high = pindex - 1;
            }
        }
    }
}
void sl_impl_sort_insertion(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high)
{
    for (size_t i = low + 1; i <= high; i++)
    {
        size_t _tmp_ = array[i];
        size_t j = i;
        while (j > low && cmp(array[j - 1], _tmp_) > 0)
        {
            array[j] = array[j - 1];
            j--;
        }
        array[j] = _tmp_;
    }
}
struct sortedlist_iter sl_iter_start(struct sortedlist *target)
{
    sl_sort(target);
    struct sortedlist_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.start = 1;
    iter.end = sl_empty(target);
    return iter;
}
struct sortedlist_iter sl_iter_end(struct sortedlist *target)
{
    sl_sort(target);
    struct sortedlist_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.start = sl_empty(target);
    iter.end = 1;
    if (!sl_empty(target))
        iter.cursor = target->count - 1;
    return iter;
}
_Bool sl_iter_at_start(struct sortedlist_iter *iter)
{
    return sl_empty(iter->target) || iter->start;
}
_Bool sl_iter_at_end(struct sortedlist_iter *iter)
{
    return sl_empty(iter->target) || iter->end;
}
_Bool sl_iter_to_start(struct sortedlist_iter *iter)
{
    if (!sl_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = sl_empty(iter->target);
        return 1;
    }
    return 0;
}
_Bool sl_iter_to_end(struct sortedlist_iter *iter)
{
    if (!sl_empty(iter->target))
    {
        iter->start = sl_empty(iter->target);
        iter->cursor = iter->target->count - 1;
        iter->end = 1;
        return 1;
    }
    return 0;
}
_Bool sl_iter_next(struct sortedlist_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = sl_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool sl_iter_prev(struct sortedlist_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = sl_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool sl_iter_advance(struct sortedlist_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= iter->target->count)
        return 0;
    iter->start = sl_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool sl_iter_rewind(struct sortedlist_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor < steps)
        return 0;
    iter->end = sl_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool sl_iter_go_to(struct sortedlist_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->cursor > index)
        return sl_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return sl_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t sl_iter_value(struct sortedlist_iter *iter)
{
    if (sl_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor];
}
size_t sl_iter_index(struct sortedlist_iter *iter)
{
    return iter->cursor;
}
_Bool sl_to_string(struct sortedlist *_list_, FILE *fptr)
{
    struct sortedlist *l_ = _list_;
    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%"
                        "I64u"
                        ", "
                        "count:%"
                        "I64u"
                        ", "
                        "is_sorted:%s, "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        "sortedlist", "size_t", l_, l_->buffer, l_->capacity, l_->count,
                        l_->is_sorted ? "true" : "false", l_->flag, l_->f_val, l_->alloc, (l_)->callbacks);
}
_Bool sl_print(struct sortedlist *_list_, FILE *fptr, const char *start, const char *separator, const char *end)
{
    sl_sort(_list_);
    fprintf(fptr, "%s", start);
    for (size_t i = 0; i < _list_->count; i++)
    {
        if (!_list_->f_val->str(fptr, _list_->buffer[i]))
            return 0;
        if (i + 1 < _list_->count)
            fprintf(fptr, "%s", separator);
    }
    fprintf(fptr, "%s", end);
    return 1;
}
