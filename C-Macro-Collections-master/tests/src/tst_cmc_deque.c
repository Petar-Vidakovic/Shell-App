
#include "tst_cmc_deque.h"

struct deque *d_new(size_t capacity, struct deque_fval *f_val)
{
    return d_new_custom(capacity, f_val, ((void *)0), ((void *)0));
}
struct deque *d_new_custom(size_t capacity, struct deque_fval *f_val, struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks)
{
    ;
    if (capacity < 1)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct deque *_deque_ = alloc->malloc(sizeof(struct deque));
    if (!_deque_)
        return ((void *)0);
    _deque_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_deque_->buffer)
    {
        alloc->free(_deque_);
        return ((void *)0);
    }
    _deque_->capacity = capacity;
    _deque_->count = 0;
    _deque_->front = 0;
    _deque_->back = 0;
    _deque_->flag = CMC_FLAG_OK;
    _deque_->f_val = f_val;
    _deque_->alloc = alloc;
    (_deque_)->callbacks = callbacks;
    return _deque_;
}
void d_clear(struct deque *_deque_)
{
    if (_deque_->f_val->free)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            _deque_->f_val->free(_deque_->buffer[i]);
            i = (i + 1) % _deque_->capacity;
        }
    }
    memset(_deque_->buffer, 0, sizeof(size_t) * _deque_->capacity);
    _deque_->count = 0;
    _deque_->front = 0;
    _deque_->back = 0;
    _deque_->flag = CMC_FLAG_OK;
}
void d_free(struct deque *_deque_)
{
    if (_deque_->f_val->free)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            _deque_->f_val->free(_deque_->buffer[i]);
            i = (i + 1) % _deque_->capacity;
        }
    }
    _deque_->alloc->free(_deque_->buffer);
    _deque_->alloc->free(_deque_);
}
void d_customize(struct deque *_deque_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!alloc)
        _deque_->alloc = &cmc_alloc_node_default;
    else
        _deque_->alloc = alloc;
    (_deque_)->callbacks = callbacks;
    _deque_->flag = CMC_FLAG_OK;
}
_Bool d_push_front(struct deque *_deque_, size_t value)
{
    if (d_full(_deque_))
    {
        if (!d_resize(_deque_, _deque_->capacity * 2))
            return 0;
    }
    _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1 : _deque_->front - 1;
    _deque_->buffer[_deque_->front] = value;
    _deque_->count++;
    _deque_->flag = CMC_FLAG_OK;
    if ((_deque_)->callbacks && (_deque_)->callbacks->create)
        (_deque_)->callbacks->create();
    ;
    return 1;
}
_Bool d_push_back(struct deque *_deque_, size_t value)
{
    if (d_full(_deque_))
    {
        if (!d_resize(_deque_, _deque_->capacity * 2))
            return 0;
    }
    _deque_->buffer[_deque_->back] = value;
    _deque_->back = (_deque_->back == _deque_->capacity - 1) ? 0 : _deque_->back + 1;
    _deque_->count++;
    _deque_->flag = CMC_FLAG_OK;
    if ((_deque_)->callbacks && (_deque_)->callbacks->create)
        (_deque_)->callbacks->create();
    ;
    return 1;
}
_Bool d_pop_front(struct deque *_deque_)
{
    if (d_empty(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    _deque_->buffer[_deque_->front] = (size_t){ 0 };
    _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1;
    _deque_->count--;
    _deque_->flag = CMC_FLAG_OK;
    if ((_deque_)->callbacks && (_deque_)->callbacks->delete)
        (_deque_)->callbacks->delete ();
    ;
    return 1;
}
_Bool d_pop_back(struct deque *_deque_)
{
    if (d_empty(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    _deque_->back = (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1;
    _deque_->buffer[_deque_->back] = (size_t){ 0 };
    _deque_->count--;
    _deque_->flag = CMC_FLAG_OK;
    if ((_deque_)->callbacks && (_deque_)->callbacks->delete)
        (_deque_)->callbacks->delete ();
    ;
    return 1;
}
size_t d_front(struct deque *_deque_)
{
    if (d_empty(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    _deque_->flag = CMC_FLAG_OK;
    if ((_deque_)->callbacks && (_deque_)->callbacks->read)
        (_deque_)->callbacks->read();
    ;
    return _deque_->buffer[_deque_->front];
}
size_t d_back(struct deque *_deque_)
{
    if (d_empty(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    _deque_->flag = CMC_FLAG_OK;
    if ((_deque_)->callbacks && (_deque_)->callbacks->read)
        (_deque_)->callbacks->read();
    ;
    return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1];
}
_Bool d_contains(struct deque *_deque_, size_t value)
{
    _deque_->flag = CMC_FLAG_OK;
    _Bool result = 0;
    for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
    {
        if (_deque_->f_val->cmp(_deque_->buffer[i], value) == 0)
        {
            result = 1;
            break;
        }
        i = (i + 1) % _deque_->capacity;
    }
    if ((_deque_)->callbacks && (_deque_)->callbacks->read)
        (_deque_)->callbacks->read();
    ;
    return result;
}
_Bool d_empty(struct deque *_deque_)
{
    return _deque_->count == 0;
}
_Bool d_full(struct deque *_deque_)
{
    return _deque_->count >= _deque_->capacity;
}
size_t d_count(struct deque *_deque_)
{
    return _deque_->count;
}
size_t d_capacity(struct deque *_deque_)
{
    return _deque_->capacity;
}
int d_flag(struct deque *_deque_)
{
    return _deque_->flag;
}
_Bool d_resize(struct deque *_deque_, size_t capacity)
{
    _deque_->flag = CMC_FLAG_OK;
    if (_deque_->capacity == capacity)
        goto success;
    if (capacity < _deque_->count)
    {
        _deque_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    size_t *new_buffer = _deque_->alloc->malloc(sizeof(size_t) * capacity);
    if (!new_buffer)
    {
        _deque_->flag = CMC_FLAG_ALLOC;
        return 0;
    }
    for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
    {
        new_buffer[j] = _deque_->buffer[i];
        i = (i + 1) % _deque_->capacity;
    }
    _deque_->alloc->free(_deque_->buffer);
    _deque_->buffer = new_buffer;
    _deque_->capacity = capacity;
    _deque_->front = 0;
    _deque_->back = _deque_->count;
success:
    if ((_deque_)->callbacks && (_deque_)->callbacks->resize)
        (_deque_)->callbacks->resize();
    ;
    return 1;
}
struct deque *d_copy_of(struct deque *_deque_)
{
    struct deque *result = d_new_custom(_deque_->capacity, _deque_->f_val, _deque_->alloc, ((void *)0));
    if (!result)
    {
        _deque_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    (result)->callbacks = _deque_->callbacks;
    if (_deque_->f_val->cpy)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            result->buffer[j] = _deque_->f_val->cpy(_deque_->buffer[i]);
            i = (i + 1) % _deque_->capacity;
        }
    }
    else
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            result->buffer[j] = _deque_->buffer[i];
            i = (i + 1) % _deque_->capacity;
        }
    }
    result->count = _deque_->count;
    result->front = 0;
    result->back = _deque_->count;
    _deque_->flag = CMC_FLAG_OK;
    return result;
}
_Bool d_equals(struct deque *_deque1_, struct deque *_deque2_)
{
    _deque1_->flag = CMC_FLAG_OK;
    _deque2_->flag = CMC_FLAG_OK;
    if (_deque1_->count != _deque2_->count)
        return 0;
    size_t i, j, k;
    for (i = _deque1_->front, j = _deque2_->front, k = 0; k < _deque1_->count; k++)
    {
        if (_deque1_->f_val->cmp(_deque1_->buffer[i], _deque2_->buffer[j]) != 0)
            return 0;
        i = (i + 1) % _deque1_->capacity;
        j = (j + 1) % _deque2_->capacity;
    }
    return 1;
}
struct deque d_init(size_t capacity, struct deque_fval *f_val)
{
    return d_init_custom(capacity, f_val, ((void *)0), ((void *)0));
}
struct deque d_init_custom(size_t capacity, struct deque_fval *f_val, struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks)
{
    ;
    struct deque _deque_ = { 0 };
    if (capacity < 1)
        return _deque_;
    if (!f_val)
        return _deque_;
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    _deque_.buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_deque_.buffer)
        return _deque_;
    _deque_.capacity = capacity;
    _deque_.count = 0;
    _deque_.front = 0;
    _deque_.back = 0;
    _deque_.flag = CMC_FLAG_OK;
    _deque_.f_val = f_val;
    _deque_.alloc = alloc;
    (&_deque_)->callbacks = callbacks;
    return _deque_;
}
void d_release(struct deque _deque_)
{
    if (_deque_.f_val->free)
    {
        for (size_t i = _deque_.front, j = 0; j < _deque_.count; j++)
        {
            _deque_.f_val->free(_deque_.buffer[i]);
            i = (i + 1) % _deque_.capacity;
        }
    }
    _deque_.alloc->free(_deque_.buffer);
}
struct deque_iter d_iter_start(struct deque *target)
{
    struct deque_iter iter;
    iter.target = target;
    iter.cursor = target->front;
    iter.index = 0;
    iter.start = 1;
    iter.end = d_empty(target);
    return iter;
}
struct deque_iter d_iter_end(struct deque *target)
{
    struct deque_iter iter;
    iter.target = target;
    if (!d_empty(target))
    {
        if (iter.target->back == 0)
            iter.cursor = iter.target->capacity - 1;
        else
            iter.cursor = iter.target->back - 1;
        iter.index = iter.target->count - 1;
    }
    else
    {
        iter.cursor = 0;
        iter.index = 0;
    }
    iter.start = d_empty(target);
    iter.end = 1;
    return iter;
}
_Bool d_iter_at_start(struct deque_iter *iter)
{
    return d_empty(iter->target) || iter->start;
}
_Bool d_iter_at_end(struct deque_iter *iter)
{
    return d_empty(iter->target) || iter->end;
}
_Bool d_iter_to_start(struct deque_iter *iter)
{
    if (!d_empty(iter->target))
    {
        iter->cursor = iter->target->front;
        iter->index = 0;
        iter->start = 1;
        iter->end = 0;
        return 1;
    }
    return 0;
}
_Bool d_iter_to_end(struct deque_iter *iter)
{
    if (!d_empty(iter->target))
    {
        if (iter->target->back == 0)
            iter->cursor = iter->target->capacity - 1;
        else
            iter->cursor = iter->target->back - 1;
        iter->index = iter->target->count - 1;
        iter->start = 0;
        iter->end = 1;
        return 1;
    }
    return 0;
}
_Bool d_iter_next(struct deque_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = d_empty(iter->target);
    iter->cursor = (iter->cursor + 1) % (iter->target->capacity);
    iter->index++;
    return 1;
}
_Bool d_iter_prev(struct deque_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = d_empty(iter->target);
    iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;
    iter->index--;
    return 1;
}
_Bool d_iter_advance(struct deque_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= iter->target->count)
        return 0;
    iter->start = d_empty(iter->target);
    iter->index += steps;
    iter->cursor = (iter->cursor + steps) % iter->target->capacity;
    return 1;
}
_Bool d_iter_rewind(struct deque_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->index < steps)
        return 0;
    iter->end = d_empty(iter->target);
    iter->index -= steps;
    if (iter->cursor < steps)
        iter->cursor += iter->target->capacity;
    iter->cursor -= steps;
    return 1;
}
_Bool d_iter_go_to(struct deque_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return d_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return d_iter_advance(iter, index - iter->index);
    return 1;
}
size_t d_iter_value(struct deque_iter *iter)
{
    if (d_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor];
}
size_t *d_iter_rvalue(struct deque_iter *iter)
{
    if (d_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t d_iter_index(struct deque_iter *iter)
{
    return iter->index;
}
_Bool d_to_string(struct deque *_deque_, FILE *fptr)
{
    struct deque *d_ = _deque_;
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
                        "front:%"
                        "I64u"
                        ", "
                        "back:%"
                        "I64u"
                        ", "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        "deque", "size_t", d_, d_->buffer, d_->capacity, d_->count, d_->front, d_->back, d_->flag,
                        d_->f_val, d_->alloc, (d_)->callbacks);
}
_Bool d_print(struct deque *_deque_, FILE *fptr, const char *start, const char *separator, const char *end)
{
    fprintf(fptr, "%s", start);
    for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
    {
        if (!_deque_->f_val->str(fptr, _deque_->buffer[i]))
            return 0;
        i = (i + 1) % _deque_->capacity;
        if (j + 1 < _deque_->count)
            fprintf(fptr, "%s", separator);
    }
    fprintf(fptr, "%s", end);
    return 1;
}
