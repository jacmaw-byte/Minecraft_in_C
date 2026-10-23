//
// Created by jacobmawby on 09/09/2026.
//

#ifndef MINECRAFT_IN_C_VECTOR_H
#define MINECRAFT_IN_C_VECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define append_vector_lit(v, value) do { \
    typeof(value) _value = (value); \
    append_vector_impl((v), &_value); \
} while (0)

struct Vector {
    size_t size;
    size_t element_size;
    size_t max_elements;
    void* data;
};

static inline void resize_vector(struct Vector* this) {
    void* buffer = realloc(this->data, this->max_elements * this->element_size);
    if (buffer == NULL) {
        printf("failed to reallocate Vector");
        exit(-1);
    }

    this->data = buffer;
}

static inline void init_vector(struct Vector* this, size_t element_size) {
    this->data = NULL;
    this->size = 0;
    this->element_size = element_size;
    this->max_elements = 1;

    resize_vector(this);
}

static inline void pop_vector(struct Vector* this, size_t index) {
    if (index >= this->size) {
        return;
    }

    size_t elements_after = this->size - index - 1;

    if (elements_after > 0) {
        memmove(
            (char*)this->data + index * this->element_size,
            (char*)this->data + (index + 1) * this->element_size,
            elements_after * this->element_size
        );
    }

    this->size--;
}

static inline size_t get_vector_bytes(struct Vector* this) {
    return this->element_size * this->size;
}

static inline void reserve_vector(struct Vector* this, size_t amount) {
    this->max_elements = amount;

    resize_vector(this);
}

static inline void append_vector_impl(struct Vector* this, const void* data) {
    // Only reallocate every once in a while for efficiency
    if (this->size >= this->max_elements) {
        this->max_elements *= 2;

        resize_vector(this);
    }

    memcpy((char*)this->data + this->size++ * this->element_size, data, this->element_size);
}

static inline void free_vector(const struct Vector* this) {
    free(this->data);
}

#endif //MINECRAFT_IN_C_VECTOR_H
