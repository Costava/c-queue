/*
File: q.h
Author: Costava
License: BSD 2-Clause License

Copyright 2021 Costava

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef Q_H
#define Q_H

#include <stdbool.h>
#include <stddef.h> // size_t
#include <stdint.h> // SIZE_MAX
#include <stdio.h>
#include <stdlib.h>

#define Q_GENERATE_HEADER_CODE(type, suffix)                                   \
    typedef struct q##suffix {                                                 \
        type  *buf;      /* Where to store queue elements */                   \
        size_t capacity; /* Allocated size of buf */                           \
        size_t length;   /* Length of queue */                                 \
                                                                               \
        type  *front;    /* Dequeue next element from here */                  \
        type  *back;     /* Enqueue next element here */                       \
        type  *last;     /* Last spot (highest address) in buf */              \
    } q##suffix;                                                               \
                                                                               \
    /* Call _init on a q before using it */                                    \
    extern void q##suffix##_init(q##suffix *const q, const size_t capacity);   \
                                                                               \
    /* Clean up internals of q */                                              \
    extern void q##suffix##_deinit(q##suffix *const q);                        \
                                                                               \
    /* Add val to the queue */                                                 \
    extern void q##suffix##_enqueue(q##suffix *const q, type val);             \
                                                                               \
    /* Return true if val was set to the dequeued value */                     \
    /* else return false (queue is empty) */                                   \
    extern bool q##suffix##_try_dequeue(q##suffix *const q, type *val);        \
                                                                               \
    /* Clear the queue */                                                      \
    extern void q##suffix##_clear(q##suffix *const q);

#define Q_GENERATE_IMPLEMENTATION_CODE(type, suffix)                           \
    void q##suffix##_init(q##suffix *const q, const size_t capacity) {         \
        if (capacity > 0) {                                                    \
            q->buf = calloc(capacity, sizeof(type));                           \
                                                                               \
            if (q->buf == NULL) {                                              \
                fprintf(stderr, "%s: Failed to calloc %zu of size %zu\n",      \
                    __func__, capacity, sizeof(type));                         \
                                                                               \
                exit(1);                                                       \
            }                                                                  \
                                                                               \
            q->last = &(q->buf[capacity - 1]);                                 \
        }                                                                      \
        else {                                                                 \
            q->buf = NULL;                                                     \
            q->last = NULL;                                                    \
        }                                                                      \
                                                                               \
        q->front = q->buf;                                                     \
        q->back = q->buf;                                                      \
                                                                               \
        q->capacity = capacity;                                                \
        q->length = 0;                                                         \
    }                                                                          \
                                                                               \
    void q##suffix##_deinit(q##suffix *const q) {                              \
        free(q->buf);                                                          \
        q->buf      = NULL;                                                    \
        q->capacity = 0;                                                       \
        q->length   = 0;                                                       \
        q->front    = NULL;                                                    \
        q->back     = NULL;                                                    \
        q->last     = NULL;                                                    \
    }                                                                          \
                                                                               \
    void q##suffix##_enqueue(q##suffix *const q, type val) {                   \
        if (q->length == q->capacity) {                                        \
            if (q->capacity == SIZE_MAX) {                                     \
                fprintf(stderr, "%s: Queue is already full with SIZE_MAX "     \
                    "elements\n", __func__);                                   \
                                                                               \
                exit(1);                                                       \
            }                                                                  \
                                                                               \
            const size_t new_capacity = ((SIZE_MAX / 2) >= q->capacity)        \
                ? (q->capacity * 2)                                            \
                : SIZE_MAX;                                                    \
                                                                               \
            /* Do not use realloc. We have to reorder buf in any case, */      \
            /*   so potential copying done by realloc would be a waste. */     \
            type *new_buf = calloc(new_capacity, sizeof(type));                \
                                                                               \
            if (new_buf == NULL) {                                             \
                fprintf(stderr, "%s: Failed to calloc %zu of size %zu\n",      \
                        __func__, new_capacity, sizeof(type));                 \
                                                                               \
                    exit(1);                                                   \
            }                                                                  \
                                                                               \
            size_t i = 0;                                                      \
            for (type *current = q->front; current <= q->last; current += 1) { \
                new_buf[i] = *current;                                         \
                i += 1;                                                        \
            }                                                                  \
                                                                               \
            for (type *current = q->buf; current < q->front; current += 1) {   \
                new_buf[i] = *current;                                         \
                i += 1;                                                        \
            }                                                                  \
                                                                               \
            free(q->buf);                                                      \
            q->buf = new_buf;                                                  \
            q->capacity = new_capacity;                                        \
            /* length remains the same */                                      \
            q->front = q->buf;                                                 \
            q->back = &(q->buf[q->length]);                                    \
            q->last = &(q->buf[q->capacity - 1]);                              \
        }                                                                      \
        else if (q->length > q->capacity) {                                    \
            fprintf(stderr, "%s: Queue is in invalid state. "                  \
                "length: %zu capacity: %zu\n", __func__,                       \
                q->length, q->capacity);                                       \
                                                                               \
            exit(1);                                                           \
        }                                                                      \
                                                                               \
        *q->back = val;                                                        \
        q->length += 1;                                                        \
                                                                               \
        if (q->back == q->last) {                                              \
            /* Wrap around */                                                  \
            q->back = q->buf;                                                  \
        }                                                                      \
        else {                                                                 \
            q->back += 1;                                                      \
        }                                                                      \
    }                                                                          \
                                                                               \
    bool q##suffix##_try_dequeue(q##suffix *const q, type *val) {              \
        if (q->length == 0) {                                                  \
            return false;                                                      \
        }                                                                      \
                                                                               \
        *val = *q->front;                                                      \
        q->length -= 1;                                                        \
                                                                               \
        if (q->front == q->last) {                                             \
            /* Wrap around */                                                  \
            q->front = q->buf;                                                 \
        }                                                                      \
        else {                                                                 \
            q->front += 1;                                                     \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    void q##suffix##_clear(q##suffix *const q) {                               \
        q->length = 0;                                                         \
        q->front = q->buf;                                                     \
        q->back = q->buf;                                                      \
    }

#define Q_GENERATE_FOR_TYPE(type, suffix)                                      \
    Q_GENERATE_HEADER_CODE(type, suffix)                                       \
    Q_GENERATE_IMPLEMENTATION_CODE(type, suffix)

#endif // Q_H
