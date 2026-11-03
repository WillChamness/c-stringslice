# C-StringSlice
This repository contains a single-header file for slicing a C string and interacting with that slice.

# Usage
To import the function declarations, simply add the following line to your C file:
```
#include "stringslice.h"
```
If you want the implementation included, you must declare `STRINGSLICE_LIB_IMPLEMENTATION` macro prior to including the header file.
For example:
```
#define STRINGSLICE_LIB_IMPLEMENTATION
// ...
#include "stringslice.h"
```

# Documentation
All documentation is included in the header file as javadoc docstrings.
