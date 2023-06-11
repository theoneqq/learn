/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define WORD_SIZE 4
#define ALIGNMENT 4
#define DWORD_SIZE_SIZE (WORD_SIZE * 2)
#define CHUNK_SIZE (1 << 12)

#define PACK(size, alloc_state) ((size) | (alloc_state))
#define GET(p) (*(unsigned char *)(p))
#define SET(p, val) (*(unsigned char *)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~(ALIGNMENT - 1))
#define GET_ALLOC_STATE(p) (GET(p) & 1)

#define HEAD(bp) ((char *)(bp) - WORD_SIZE)
#define FOOT(bp) ((char *)(bp) + GET_SIZE(HEAD(bp)) - DWORD_SIZE_SIZE)

#define NEXT_BLOCK(bp) ((char *)(bp) + GET_SIZE(HEAD(bp)))
#define PREV_BLOCK(bp) ((char *)(bp) - GET_SIZE((char *) (bp) - DWORD_SIZE_SIZE))

/* rounds up to the nearest multiple of WORD_SIZE */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))  // math.ceil(size / WORD_SIZE) * WORD_SIZE


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

static char *heap_base; 

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_base = mem_sbrk(4 * WORD_SIZE)) == (void *)(-1)) {
        return -1;
    }
    SET(heap_base, 0);
    SET(heap_base + WORD_SIZE, PACK(DWORD_SIZE_SIZE, 1));
    SET(heap_base + 2 * WORD_SIZE, PACK(DWORD_SIZE_SIZE, 1));
    SET(heap_base + 3 * WORD_SIZE, PACK(0, 1));

    heap_base += 2 * WORD_SIZE;

    if (extend_heap(CHUNK_SIZE / WORD_SIZE) == NULL) {
        return -1;
    }    
    return 0;
}

static void* coalesce(void *bp) {
    int prev_alloc_state = GET_ALLOC_STATE(FOOT(PREV_BLOCK(bp)));
    int next_alloc_state = GET_ALLOC_STATE(HEAD(NEXT_BLOCK(bp)));
    int size = GET_SIZE(HEAD(bp));

    if (prev_alloc_state && next_alloc_state) {
        return bp;
    } else if (prev_alloc_state && !next_alloc_state) {
        size += GET_SIZE(HEAD(NEXT_BLOCK(bp)));
        SET(HEAD(bp), PACK(size, 0));
        SET(FOOT(bp), PACK(size, 0));
    } else if (!prev_alloc_state && next_alloc_state) {
        size += GET_SIZE(HEAD(PREV_BLOCK(bp)));
        SET(FOOT(bp), PACK(size, 0));
        SET(HEAD(PREV_BLOCK(bp)), PACK(size, 0));
        bp = PREV_BLOCK(bp);
    } else {
        size += GET_SIZE(HEAD(NEXT_BLOCK(bp))) + GET_SIZE(FOOT(PREV_BLOCK(bp)));
        SET(HEAD(PREV_BLOCK(bp)), PACK(size, 0));
        SET(FOOT(NEXT_BLOCK(bp)), PACK(size, 0));
        bp = PREV_BLOCK(bp);
    }

    return bp;
}

static void* extend_heap(int words) {
    int extend_size = ALIGN(words * WORD_SIZE);
    char *bp;
    if ((void*) (bp = mem_sbrk(extend_size)) == (void *) (-1)) {
        return NULL;
    }
    SET(HEAD(bp), PACK(extend_size, 0));
    SET(FOOT(bp), PACK(extend_size, 0));

    set(HEAD(NEXT_BLOCK(bp)), PACK(0, 1));

    return coalesce(bp);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */

void *find_fit(int size) {
    for (char *bp = heap_base; GET_SIZE(HEAD(bp)) > 0; bp = NEXT_BLOCK(bp)) {
        if (GET_SIZE(HEAD(bp)) >= size && GET_ALLOC_STATE(HEAD(bp)) == 0)
            return bp;
    }
    return NULL;
}

void *cut(char* bp, int size) {
    int c_size = GET_SIZE(HEAD(bp));
    if (c_size - size >= 2 * WORD_SIZE) {
        SET(HEAD(bp), PACK(size, 1));
        SET(FOOT(bp), PACK(size, 1));
        bp = NEXT_BLOCK(bp);
        SET(HEAD(bp), PACK(c_size - size, 0));
        SET(FOOT(bp), PACK(c_size - size, 0));
    } else {
        SET(HEAD(bp), PACK(c_size, 1));
        SET(FOOT(bp), PACK(c_size, 1));
    }
}

void *mm_malloc(size_t size)
{
    int new_size = ALIGN(size + DWORD_SIZE_SIZE);
    if (new_size <= 0) {
        return NULL;
    }

    char *bp = (char *) find_fit(new_size);
    if (NULL != bp) {
        cut(bp, new_size);
        return bp;
    }

    int extend_size = CHUNK_SIZE <= new_size ? new_size : CHUNK_SIZE;
    if ((bp = extend_heap(extend_size / WORD_SIZE)) == NULL)
        return NULL;

    cut(bp, new_size);

    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
   /*write something*/ 
   int size = GET_SIZE(HEAD(ptr));
   SET(HEAD(ptr), PACK(size, 0));
   SET(FOOT(ptr), PACK(size, 0));

   return coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}


