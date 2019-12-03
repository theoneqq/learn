#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

struct chunk {
    size_t size;
    struct chunk *next;
    struct chunk *prev;
    int free;
    void *ptr; //pointer to the allocated chunk namly data
    char data[1]; //byte precise, data boundary
};

typedef struct chunk *t_chunk;
#define CHUNK_SIZE 20
#define align4(x) (((((x) - 1) >> 2) << 2) + 4)
t_chunk base = NULL;

t_chunk find_chund(t_chunk *last, size_t size) {
    t_chunk b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return (b);
}

t_chunk get_chunk(void *p) {
    char *tmp = p;
    return (p = tmp -= CHUNK_SIZE);
}

//p point to data field in chunk
bool valid_ptr(void *p) {
    if (base) {
        if (p > base && p < sbrk(0)) {
            return p == get_chunk(p)->ptr;
        }
    }
    return false;
}

void split_chunk(t_chunk t, size_t s) {
    t_chunk new_chunk;
    new_chunk = (t_chunk) (t->data + s);
    new_chunk->size = t->size - s - CHUNK_SIZE;
    new_chunk->next = t->next;
    new_chunk->prev = t;
    new_chunk->free = 1;
    new_chunk->ptr = new_chunk->data;
    t->size = s;
    t->next = new_chunk;
    if (new_chunk->next)
        new_chunk->next->prev = new_chunk;
}

t_chunk extend_heap(t_chunk last, size_t s) {
    t_chunk t = (t_chunk) sbrk(0);
    if (sbrk(CHUNK_SIZE + s) == (void *) - 1)
        return (NULL);
    t->size = s;
    t->next = NULL;
    t->prev = last;
    t->ptr = t->data;
    if (last)
        last->next = t;
    t->free = 0;
    return (b);
}

t_chunk fusion(t_chunk t) {
    if (t->next && t->next->free) {
        t->next = t->next->next;
        t->size += t->next->size + CHUNK_SIZE;
        if (t->next)
            t->next->prev = t;
    }
    return (t);
}

void *malloc(size_t size) {
    t_chunk t, last;
    size_t s = align4(size);
    if (base) {
        last = base;
        t = find_chund(&last, s);
        if (t) {
            if ((t->size - s) >= (CHUNK_SIZE + 4))
                split_chunk(t, s);
            t->free = 0;
        } else {
           t = extend_heap(last, s);
           if (!t)
               return (NULL);
        }
    } else {
        t = extend_heap(NULL, s);
        if (!t)
            return (NULL);
        base = t;
    }
    return (t->data);
}

void free(void *p) {
    if (valid_ptr(p)) {
        t_chunk t = get_chunk(p);
        t->free = 1;
        if (t->prev && t->prev->free) {
            t = fusion(t->prev);
        }
        if (t->next && t->next->free) {
            fusion(t);
        } else {
            if (t->prev)
                t->prev->next = NULL;
            else
                base = NULL;
            brk(t);
        }
    }
}

int main() {
    return 0;
}
