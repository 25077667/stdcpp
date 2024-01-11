# stdcpp: Standard Library Implementations

## Overview:
The stdcpp project provides custom implementations of various standard C++ library functions. This is particularly useful in scenarios where the standard library is not fully supported by the compiler or the target environment. The project aims to offer a reliable alternative for developers facing such constraints.

## Minimum Requirements:
```
C++14
```

## Usage Instructions:
To utilize the stdcpp library, simply include the desired header file in your C++ project. The library is structured to be straightforward and easy to integrate.

## Features and reason:
| Header | class or function | Description | Reason |
| --- | --- | --- | --- |
| shared_mutex | shared_mutex | Provides a shared mutex implementation for C++14 and Windows XP. | AcquireSRWLockExclusive is supported since Windows 7. |
| utility | to_underlying | Provides a to_underlying implementation for C++14. | std::underlying_type is supported since C++23. |
| string | u8string | Provides a u8string implementation for C++14. | std::u8string is supported since C++20. |
| ranges | basic ranges | Provides basic ranges implementation for C++14. | std::ranges is supported since C++20 and C++23. |

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
