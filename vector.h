#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define __VECTOR_STRUCT_DECL(__ELEMENT_TYPE, __TYPE_NAME) typedef struct __TYPE_NAME##_vector {\
__ELEMENT_TYPE* arr;\
size_t starting_index;\
size_t size;\
size_t capacity;\
} __TYPE_NAME##_vector_t;

#define __VECTOR_METHODS_DECL(__ELEMENT_TYPE, __TYPE_NAME) \
__TYPE_NAME##_vector_t* __TYPE_NAME##_vector_new(size_t capacity);\
__TYPE_NAME##_vector_t* __TYPE_NAME##_vector_from_array_copy(__ELEMENT_TYPE* arr, size_t size);\
void __TYPE_NAME##_vector_resize(__ELEMENT_TYPE* arr, size_t capacity);\
void __TYPE_NAME##_vector_fill(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE element);\
void __TYPE_NAME##_vector_push(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE element);\
__ELEMENT_TYPE __TYPE_NAME##_vector_pop(__TYPE_NAME##_vector_t* vec);\
__ELEMENT_TYPE __TYPE_NAME##_vector_at(__TYPE_NAME##_vector_t* vec, size_t index);\
void __TYPE_NAME##_vector_set_at(__TYPE_NAME##_vector_t* vec, size_t index, __ELEMENT_TYPE element);\
void __TYPE_NAME##_vector_for_each(__TYPE_NAME##_vector_t* vec, void(*f)(__ELEMENT_TYPE));\
__ELEMENT_TYPE __TYPE_NAME##_vector_reduce(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE##(*f)(__ELEMENT_TYPE, __ELEMENT_TYPE));\
void __TYPE_NAME##_vector_elements_in_array(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE *arr);\
bool __TYPE_NAME##_vector_backing_array_wraps_around(__TYPE_NAME##_vector_t* vec);\
void __TYPE_NAME##_vector_rearrange(__TYPE_NAME##_vector_t* vec, size_t starting_index);

#define __VECTOR_DECLARE_TYPE(__ELEMENT_TYPE, __TYPE_NAME) \
__VECTOR_STRUCT_DECL(__ELEMENT_TYPE, __TYPE_NAME) \
__VECTOR_METHODS_DECL(__ELEMENT_TYPE, __TYPE_NAME)

#define __VECTOR_NEW_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
__TYPE_NAME##_vector_t* __TYPE_NAME##_vector_new(size_t capacity) {\
    __TYPE_NAME##_vector_t* vec = (__TYPE_NAME##_vector_t*)malloc(sizeof(__TYPE_NAME##_vector_t));\
    vec->capacity = capacity;\
    vec->arr = (__ELEMENT_TYPE*)malloc(sizeof(__ELEMENT_TYPE)*vec->capacity);\
    vec->size = 0;\
    vec->starting_index = 0;\
    return vec;\
}

#define __VECTOR_RESIZE_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
void __TYPE_NAME##_vector_resize(__TYPE_NAME##_vector_t* vec, size_t capacity) {\
    if(capacity < vec->size) {\
        perror("Attempted to reduce capacity of a vector to a value lower than its size\n");\
        exit(1);\
    }\
    __ELEMENT_TYPE* arr = (__ELEMENT_TYPE*)malloc(sizeof(__ELEMENT_TYPE)*capacity);\
    for(size_t i = vec->starting_index; i < vec->starting_index + vec->size; i++) {\
        arr[i-vec->starting_index] = vec->arr[i % vec->capacity];\
    }\
    free(vec->arr);\
    vec->arr = arr;\
    vec->capacity = capacity;\
    vec->starting_index = 0;\
}

#define __VECTOR_FROM_ARRAY_COPY_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
__TYPE_NAME##_vector_t* __TYPE_NAME##_vector_from_array_copy(__ELEMENT_TYPE* arr, size_t size) {\
    __TYPE_NAME##_vector_t* vec = __TYPE_NAME##_vector_new(size);\
    vec->size = size;\
    for(size_t i = 0; i < size; i++) {\
        vec->arr[i] = arr[i];\
    }\
    return vec;\
}

#define __VECTOR_FILL_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
void __TYPE_NAME##_vector_fill(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE element) {\
    for(size_t i = 0; i < vec->capacity; i++) {\
        vec->arr[i] = element;\
    }\
    vec->size = vec->capacity;\
}

#define __VECTOR_PUSH_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
void __TYPE_NAME##_vector_push(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE element) {\
    if(vec->size == vec->capacity) {\
        __TYPE_NAME##_vector_resize(vec, vec->capacity * 2);\
    }\
    size_t write_index = (vec->starting_index + vec->size) % vec->capacity;\
    vec->arr[write_index] = element;\
    vec->size+=1;\
}

#define __VECTOR_POP_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
__ELEMENT_TYPE __TYPE_NAME##_vector_pop(__TYPE_NAME##_vector_t* vec) {\
    __ELEMENT_TYPE el = vec->arr[vec->starting_index];\
    vec->size-=1;\
    vec->starting_index = (vec->starting_index + 1) % vec->capacity;\
    if(vec->size != 0 && vec->capacity/vec->size >= 4) {\
        __TYPE_NAME##_vector_resize(vec, vec->capacity / 2);\
    }\
    return el;\
}

#define __VECTOR_AT_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
__ELEMENT_TYPE __TYPE_NAME##_vector_at(__TYPE_NAME##_vector_t* vec, size_t index) {\
    if(index >= vec->size) {\
        fprintf(stderr, "Error: attempted to access index %zu of vector of size %zu backed by array at address %zx\n", index, vec->size, vec->arr);\
        exit(1);\
    }\
    return vec->arr[(vec->starting_index + index) % vec->capacity];\
}

#define __VECTOR_SET_AT_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
void __TYPE_NAME##_vector_set_at(__TYPE_NAME##_vector_t* vec, size_t index, __ELEMENT_TYPE element) {\
    if(index >= vec->size) {\
        fprintf(stderr, "Error: attempted to access index %zu of vector of size %zu backed by array at address %zx\n", index, vec->size, vec->arr);\
        exit(1);\
    }\
    vec->arr[(vec->starting_index + index) % vec->capacity] = element;\
}

#define __VECTOR_FOR_EACH_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
void __TYPE_NAME##_vector_for_each(__TYPE_NAME##_vector_t* vec, void(*f)(__ELEMENT_TYPE)) {\
    for(size_t i = vec->starting_index; i < vec->starting_index + vec->size; i++) {\
        (*f)(vec->arr[i % vec->capacity]);\
    }\
}

#define __VECTOR_REDUCE_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
__ELEMENT_TYPE __TYPE_NAME##_vector_reduce(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE##(*f)(__ELEMENT_TYPE, __ELEMENT_TYPE)) {\
    if(vec->size == 0) {\
        fprintf(stderr, "Error: attempted to reduce an empty vector backed by array at address %zx\n",vec->arr);\
        exit(1);\
    }\
    __ELEMENT_TYPE acc = vec->arr[vec->starting_index];\
    for(size_t i = vec->starting_index + 1; i < vec->starting_index + vec->size; i++) {\
        acc = (*f)(acc, vec->arr[i % vec->capacity]);\
    }\
    return acc;\
}

#define __VECTOR_ELEMENTS_IN_ARRAY_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
void __TYPE_NAME##_vector_elements_in_array(__TYPE_NAME##_vector_t* vec, __ELEMENT_TYPE* arr) {\
    for(size_t i = vec->starting_index + 1; i < vec->starting_index + vec->size; i++) {\
        arr[i - vec->starting_index] = vec->arr[i % vec->capacity];\
    }\
}

#define __VECTOR_BACKING_ARRAY_WRAPS_AROUND_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
bool __TYPE_NAME##_vector_backing_array_wraps_around(__TYPE_NAME##_vector_t* vec) {\
    return (vec->starting_index + vec->size) > vec->capacity;\
}

#define __VECTOR_REARRANGE_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
void __TYPE_NAME##_vector_rearrange(__TYPE_NAME##_vector_t* vec, size_t starting_index) {\
    if(starting_index > vec->capacity) {\
        fprintf(stderr, "Error: attempted to set starting index of vector to a value outside of range\n");\
        exit(1);\
    }\
    if(starting_index == vec->starting_index) {\
        return;\
    }\
    __ELEMENT_TYPE* arr = (__ELEMENT_TYPE*)malloc(sizeof(__ELEMENT_TYPE) * vec->capacity);\
    for(size_t i = 0; i < vec->size; i++) {\
        arr[(starting_index + i) % vec->capacity] = vec->arr[(vec->starting_index + i) % vec->capacity];\
    }\
    free(vec->arr);\
    vec->arr = arr;\
    vec->starting_index = starting_index;\
}

#define __VECTOR_METHODS_IMPL(__ELEMENT_TYPE, __TYPE_NAME) \
__VECTOR_NEW_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_FROM_ARRAY_COPY_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_RESIZE_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_FILL_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_PUSH_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_POP_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_AT_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_SET_AT_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_FOR_EACH_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_REDUCE_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_ELEMENTS_IN_ARRAY_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_BACKING_ARRAY_WRAPS_AROUND_IMPL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_REARRANGE_IMPL(__ELEMENT_TYPE, __TYPE_NAME)

#define __VECTOR_FULLY_IMPLEMENT_TYPE(__ELEMENT_TYPE, __TYPE_NAME) \
__VECTOR_STRUCT_DECL(__ELEMENT_TYPE, __TYPE_NAME)\
__VECTOR_METHODS_IMPL(__ELEMENT_TYPE, __TYPE_NAME)

#endif