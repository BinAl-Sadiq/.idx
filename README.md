# .idx
.idx is a C library for reading/writing .idx files. It also enables you to convert png and jpg files into .idx files, and vice versa. And it provides useful functions for changing the bytes sequence order(E.g. from small-endian to big-endian, and vice versa).

## Breif & simple introduction to .idx files
This binary file format is used for storing multidimensional matrices. The matrix's data stored as a c-style array, which means __less memory usage__ and __better performance__. This format uses the "Big-Endian" bytes sequence order.

### .idx format
The first four bytes are called the "magic number". The first two bytes of this number are always equal to 0. The third byte determines the data type. There are several data types as illustrated bellow:
```c
//idx_data_type.h

enum idx_data_type
{
	IDX_UINT8 = 0x08,
	IDX_SINT8 = 0x09,
	IDX_SINT16 = 0x0B,
	IDX_SINT32 = 0x0C,
	IDX_FLOAT = 0x0D,
	IDX_DOUBLE = 0x0E
};
```
The fourth byte determines the count of the dimensions in the matrix. The next bytes will represnet the length of each dimension in four bytes. Finally, the rest of the bytes represent the matrix's data.
The format can be breifally illustrated as follows:

>magic number
>
>size in dimension 0
>
>size in dimension 1
>
>size in dimension 2
>
>…..
>
>size in dimension N
>
>data

[- Mario Emmanuel, "Do you really know how MNIST is stored?", medium.com](https://medium.com/theconsole/do-you-really-know-how-mnist-is-stored-600d69455937)


## Usages
### Extract data out of idx/png/jpg files
```c
struct idx_data* idx_ptr = idx_load_from_idx_file("train-images.idx3-ubyte");
//struct idx_data* idx_ptr = idx_load_from_png_file("cat.png");
//struct idx_data* idx_ptr = idx_load_from_jpg_file("dog.jpg");

for (uint32_t i = 0; i < idx_ptr->dimensions_lengths[0]; i++)
  for (uint32_t ii = 0; ii < idx_ptr->dimensions_lengths[1]; ii++)
    for (uint32_t iii = 0; iii < idx_ptr->dimensions_lengths[2]; iii++)
    {
      uint8_t pixel = *(uint8_t*)idx_get_element_ptr(idx_ptr, i, ii, iii);
      //...
    }

idx_data_free(idx_ptr);
```
### Store data to idx/png/jpg files
```c
idx_store_to_idx_file(idx_ptr, "example.idx3-ubyte");
idx_store_to_png_file(idx_ptr, "example.png");
idx_store_to_jpg_file(idx_ptr, "example.jpg");
```
### Change bytes sequences order from small-endian to big-endian, and vice versa
```c
//idx_swap.h

inline void* idx_swap16(const void* ptr);

inline void* idx_swap32(const void* ptr);

inline void* idx_swap64(const void* ptr);

void* idx_swap_array(const void*ptr, size_t elements_count, enum idx_data_type data_type);
```

## Debugging
You might accidentally cause an error while using the library's functions. You need to call ```enum idx_error_type idx_get_error(void);``` to determine the error's type.
### Error types
```c
enum idx_error_type
{
	IDX_NO_ERROR,
	IDX_INVALID_FIRST_TWO_BYTES,
	IDX_INVALID_DATA_TYPE,
	IDX_NO_DIMENSIONS,
	IDX_COULD_NOT_OPEN_FILE,
	IDX_MEMORY_SHORTAGE,
	IDX_COULD_NOT_LOAD_IMAGE
};
```
### Example
```c
struct idx_data* idx_ptr = idx_load_from_idx_file("no such a file!");
if (!idx_ptr)
{
  enum idx_error_type err = idx_get_error();

  if (err == IDX_COULD_NOT_OPEN_FILE)
  {
    //Do something...
  }

  exit(1);
}
```

## License
[MIT License](https://github.com/BinAl-Sadiq/La-Rayb/blob/main/LICENSE)
