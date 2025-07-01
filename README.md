# Usage
Just include the file wherever you want to, then you have a few (literally 2) options:  
1. Declaring the struct and the functions in a header file by using ```__VECTOR_DECLARE_TYPE(__ELEMENT_TYPE, __TYPE_NAME)``` and then implement them in a separate file by using ```__VECTOR_METHODS_IMPL(__ELEMENT_TYPE, __TYPE_NAME)```
2. Doing everything all at once by using ```__VECTOR_FULLY_IMPLEMENT_TYPE(__ELEMENT_TYPE, __TYPE_NAME)```

Of note is that ```__ELEMENT_TYPE``` should be the type you want the vector to be implemented for while ```__TYPE_NAME``` is what will be used in function names and typedef for the struct.
# Available functions
- ```__TYPE_NAME_vector_t* __TYPE_NAME_vector_new(size_t capacity)```:  
Heap allocates a vector that contains elements of type ```__ELEMENT_TYPE``` with the given capacity and size = 0
- ```__TYPE_NAME_vector_t* __TYPE_NAME_vector_from_array_copy(__ELEMENT_TYPE* arr, size_t size)```:  
Heap allocates a vector that contains elements of type ```__ELEMENT_TYPE``` of size and capacity equal to size with its elements initialized to the ones inside the array passed as a parameter (it does NOT set that array as the array that backs the vector)
- ```void __TYPE_NAME_vector_fill(__TYPE_NAME_vector_t* vec, __ELEMENT_TYPE element)```:  
Fills the vector to capacity with the passed element.
- ```void __TYPE_NAME_vector_push(__TYPE_NAME_vector_t* vec, __ELEMENT_TYPE element)```:  
Appends an element at the end of the vector. If there is not enough space, the backing array is resized.
- ```__ELEMENT_TYPE __TYPE_NAME_vector_pop(__TYPE_NAME_vector_t* vec)```:  
"Removes" the element at the front of the vector. If there is way too much empty space, the backing array is resized.
- ```__ELEMENT_TYPE __TYPE_NAME_vector_at(__TYPE_NAME_vector_t* vec, size_t index)```:  
Gets the element at the passed index. Exits the program if the index is out of range.
- ```void __TYPE_NAME_vector_for_each(__TYPE_NAME_vector_t* vec, void(*f)(__ELEMENT_TYPE))```:  
Sets the element at the passed index. Exits the program if the index is out of range.
- ```void __TYPE_NAME_vector_for_each(__TYPE_NAME_vector_t* vec, void(*f)(__ELEMENT_TYPE))```:
Calls a function that returns nothing on each element of the vector
- ```__ELEMENT_TYPE __TYPE_NAME_vector_reduce(__TYPE_NAME_vector_t* vec, __ELEMENT_TYPE(*f)(__ELEMENT_TYPE, __ELEMENT_TYPE))```:  
Calls a function that takes 2 parameters of type ```__ELEMENT_TYPE``` and returns a value of type ```__ELEMENT_TYPE``` on each element. The result of a call with the previous element as a parameter is used as parameter for the following calls.
