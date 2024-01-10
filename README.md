# stdcpp: Standard Library Implementations

## Overview:
The stdcpp project provides custom implementations of various standard C++ library functions. This is particularly useful in scenarios where the standard library is not fully supported by the compiler or the target environment. The project aims to offer a reliable alternative for developers facing such constraints.

## Usage Instructions:
To utilize the stdcpp library, simply include the desired header file in your C++ project. The library is structured to be straightforward and easy to integrate.

## Building the Project:
stdcpp is primarily a header-only library, streamlining its integration into other projects. However, for those interested in compiling the library, follow these steps using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

## Testing:
The project includes a comprehensive suite of tests to ensure reliability and correctness. These tests are automatically configured through CMake. Upon building the project, test binaries are created in the build directory. These are named following the pattern `t_<header_name>` and can be executed to validate the functionality of each header file.

## Contributing:
Contributions to stdcpp are welcomed and appreciated. This can involve adding new header files or enhancing existing ones. When contributing new headers, ensure that they do not duplicate functionality already present in the standard C++ library. Your efforts help in making stdcpp a more robust and extensive library.
