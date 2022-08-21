
#include "tst_cmc_heap.h"

static void h_impl_float_up(struct heap *_heap_, size_t index);
static void h_impl_float_down(struct heap *_heap_, size_t index);
struct heap *h_new(size_t capacity, enum cmc_heap_order HO, struct heap_fval *f_val)
{
    return h_new_custom(capacity, HO, f_val, ((void *)0), ((void *)0));
}
struct heap *h_new_custom(size_t capacity, enum cmc_heap_order HO, struct heap_fval *f_val,
                          struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (capacity < 1)
        return ((void *)0);
    if (HO != CMC_MIN_HEAP && HO != CMC_MAX_HEAP)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct heap *_heap_ = alloc->malloc(sizeof(struct heap));
    if (!_heap_)
        return ((void *)0);
    _heap_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_heap_->buffer)
    {
        alloc->free(_heap_);
        return ((void *)0);
    }
    _heap_->capacity = capacity;
    _heap_->count = 0;
    _heap_->HO = HO;
    _heap_->flag = CMC_FLAG_OK;
    _heap_->f_val = f_val;
    _heap_->alloc = alloc;
    (_heap_)->callbacks = callbacks;
    return _heap_;
}
void h_clear(struct heap *_heap_)
{
    if (_heap_->f_val->free)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            _heap_->f_val->free(_heap_->buffer[i]);
        }
    }
    memset(_heap_->buffer, 0, sizeof(size_t) * _heap_->capacity);
    _heap_->count = 0;
    _heap_->flag = CMC_FLAG_OK;
}
void h_free(struct heap *_heap_)
{
    if (_heap_->f_val->free)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            _heap_->f_val->free(_heap_->buffer[i]);
        }
    }
    _heap_->alloc->free(_heap_->buffer);
    _heap_->alloc->free(_heap_);
}
void h_customize(struct heap *_heap_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!alloc)
        _heap_->alloc = &cmc_alloc_node_default;
    else
        _heap_->alloc = alloc;
    (_heap_)->callbacks = callbacks;
    _heap_->flag = CMC_FLAG_OK;
}
_Bool h_insert(struct heap *_heap_, size_t value)
{
    if (h_full(_heap_))
    {
        if (!h_resize(_heap_, _heap_->count * 2))
            return 0;
    }
    _heap_->buffer[_heap_->count++] = value;
    if (_heap_->count > 1)
    {
        h_impl_float_up(_heap_, _heap_->count - 1);
    }
    _heap_->flag = CMC_FLAG_OK;
    if ((_heap_)->callbacks && (_heap_)->callbacks->create)
        (_heap_)->callbacks->create();
    ;
    return 1;
}
_Bool h_remove(struct heap *_heap_)
{
    if (h_empty(_heap_))
    {
        _heap_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    _heap_->buffer[0] = _heap_->buffer[_heap_->count - 1];
    _heap_->buffer[_heap_->count - 1] = (size_t){ 0 };
    _heap_->count--;
    h_impl_float_down(_heap_, 0);
    _heap_->flag = CMC_FLAG_OK;
    if ((_heap_)->callbacks && (_heap_)->callbacks->delete)
        (_heap_)->callbacks->delete ();
    ;
    return 1;
}
size_t h_peek(struct heap *_heap_)
{
    if (h_empty(_heap_))
    {
        _heap_->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    _heap_->flag = CMC_FLAG_OK;
    if ((_heap_)->callbacks && (_heap_)->callbacks->read)
        (_heap_)->callbacks->read();
    ;
    return _heap_->buffer[0];
}
_Bool h_contains(struct heap *_heap_, size_t value)
{
    _heap_->flag = CMC_FLAG_OK;
    _Bool result = 0;
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (_heap_->f_val->cmp(_heap_->buffer[i], value) == 0)
        {
            result = 1;
            break;
        }
    }
    if ((_heap_)->callbacks && (_heap_)->callbacks->read)
        (_heap_)->callbacks->read();
    ;
    return result;
}
_Bool h_empty(struct heap *_heap_)
{
    return _heap_->count == 0;
}
_Bool h_full(struct heap *_heap_)
{
    return _heap_->count >= _heap_->capacity;
}
size_t h_count(struct heap *_heap_)
{
    return _heap_->count;
}
size_t h_capacity(struct heap *_heap_)
{
    return _heap_->capacity;
}
int h_flag(struct heap *_heap_)
{
    return _heap_->flag;
}
_Bool h_resize(struct heap *_heap_, size_t capacity)
{
    _heap_->flag = CMC_FLAG_OK;
    if (_heap_->capacity == capacity)
        goto success;
    if (capacity < _heap_->count)
    {
        _heap_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    size_t *new_buffer = _heap_->alloc->realloc(_heap_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
    {
        _heap_->flag = CMC_FLAG_ALLOC;
        return 0;
    }
    _heap_->buffer = new_buffer;
    _heap_->capacity = capacity;
success:
    if ((_heap_)->callbacks && (_heap_)->callbacks->resize)
        (_heap_)->callbacks->resize();
    ;
    return 1;
}
struct heap *h_copy_of(struct heap *_heap_)
{
    struct heap *result = h_new_custom(_heap_->capacity, _heap_->HO, _heap_->f_val, _heap_->alloc, ((void *)0));
    if (!result)
    {
        _heap_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    (result)->callbacks = _heap_->callbacks;
    if (_heap_->f_val->cpy)
    {
        for (size_t i = 0; i < _heap_->count; i++)
            result->buffer[i] = _heap_->f_val->cpy(_heap_->buffer[i]);
    }
    else
        memcpy(result->buffer, _heap_->buffer, sizeof(size_t) * _heap_->count);
    result->count = _heap_->count;
    _heap_->flag = CMC_FLAG_OK;
    return result;
}
_Bool h_equals(struct heap *_heap1_, struct heap *_heap2_)
{
    _heap1_->flag = CMC_FLAG_OK;
    _heap2_->flag = CMC_FLAG_OK;
    if (_heap1_->count != _heap2_->count)
        return 0;
    for (size_t i = 0; i < _heap1_->count; i++)
    {
        if (_heap1_->f_val->cmp(_heap1_->buffer[i], _heap2_->buffer[i]) != 0)
            return 0;
    }
    return 1;
}
static void h_impl_float_up(struct heap *_heap_, size_t index)
{
    size_t C = index;
    size_t child = _heap_->buffer[C];
    size_t parent = _heap_->buffer[(index - 1) / 2];
    int mod = _heap_->HO;
    while (C > 0 && _heap_->f_val->cmp(child, parent) * mod > 0)
    {
        size_t tmp = _heap_->buffer[C];
        _heap_->buffer[C] = _heap_->buffer[(C - 1) / 2];
        _heap_->buffer[(C - 1) / 2] = tmp;
        C = (C - 1) / 2;
        if (C == 0)
            break;
        child = _heap_->buffer[C];
        parent = _heap_->buffer[(C - 1) / 2];
    }
}
static void h_impl_float_down(struct heap *_heap_, size_t index)
{
    int mod = _heap_->HO;
    while (index < _heap_->count)
    {
        size_t L = 2 * index + 1;
        size_t R = 2 * index + 2;
        size_t C = index;
        if (L < _heap_->count && _heap_->f_val->cmp(_heap_->buffer[L], _heap_->buffer[C]) * mod > 0)
        {
            C = L;
        }
        if (R < _heap_->count && _heap_->f_val->cmp(_heap_->buffer[R], _heap_->buffer[C]) * mod > 0)
        {
            C = R;
        }
        if (C != index)
        {
            size_t tmp = _heap_->buffer[index];
            _heap_->buffer[index] = _heap_->buffer[C];
            _heap_->buffer[C] = tmp;
            index = C;
        }
        else
            break;
    }
}
struct heap_iter h_iter_start(struct heap *target)
{
    struct heap_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.start = 1;
    iter.end = h_empty(target);
    return iter;
}
struct heap_iter h_iter_end(struct heap *target)
{
    struct heap_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.start = h_empty(target);
    iter.end = 1;
    if (!h_empty(target))
        iter.cursor = target->count - 1;
    return iter;
}
_Bool h_iter_at_start(struct heap_iter *iter)
{
    return h_empty(iter->target) || iter->start;
}
_Bool h_iter_at_end(struct heap_iter *iter)
{
    return h_empty(iter->target) || iter->end;
}
_Bool h_iter_to_start(struct heap_iter *iter)
{
    if (!h_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = h_empty(iter->target);
        return 1;
    }
    return 0;
}
_Bool h_iter_to_end(struct heap_iter *iter)
{
    if (!h_empty(iter->target))
    {
        iter->cursor = iter->target->count - 1;
        iter->start = h_empty(iter->target);
        iter->end = 1;
        return 1;
    }
    return 0;
}
_Bool h_iter_next(struct heap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = h_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool h_iter_prev(struct heap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = h_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool h_iter_advance(struct heap_iter *iter, size_t steps)
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
    if (iter->end)
        return 0;
    iter->start = h_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool h_iter_rewind(struct heap_iter *iter, size_t steps)
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
    iter->end = h_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool h_iter_go_to(struct heap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->cursor > index)
        return h_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return h_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t h_iter_value(struct heap_iter *iter)
{
    if (h_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor];
}
size_t h_iter_index(struct heap_iter *iter)
{
    return iter->cursor;
}
_Bool h_to_string(struct heap *_heap_, FILE *fptr)
{
    struct heap *h_ = _heap_;
    const char *t = h_->HO == 1 ? "MaxHeap" : "MinHeap";
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
                        "type:%s, "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks: %p}",
                        "heap", "size_t", h_, h_->buffer, h_->capacity, h_->count, t, h_->flag, h_->f_val, h_->alloc,
                        (h_)->callbacks);
}
_Bool h_print(struct heap *_heap_, FILE *fptr, const char *start, const char *separator, const char *end)
{
    fprintf(fptr, "%s", start);
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (!_heap_->f_val->str(fptr, _heap_->buffer[i]))
            return 0;
        if (i + 1 < _heap_->count)
            fprintf(fptr, "%s", separator);
    }
    fprintf(fptr, "%s", end);
    return 1;
}
