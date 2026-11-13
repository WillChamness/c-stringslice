# C-StringSlice
This repository contains a single-header file for slicing a C string and interacting with that slice.

# Usage
Download the `stringslice.h` file and add it into your project. To import the function declarations, simply add the following line to your C file:
```
#include "stringslice.h"
```
If you want the implementation included, you must declare the `STRINGSLICE_LIB_IMPLEMENTATION` macro prior to including the header file.
For example:
```
#define STRINGSLICE_LIB_IMPLEMENTATION
// ...
#include "stringslice.h"
```

Note that the macro must be declared in exactly one file to prevent linking errors. If multiple files need to include the header, you can
create a `stringslice.c` file and define the implementations there. For example:
```
/* stringslice.c */
#define STRINGSLICE_LIB_IMPLEMENTATION
#include "stringslice.h"

/* file1.c */
#include "stringslice.h"

/* file2.c */
#include "stringslice.h"
```

# Documentation
All documentation is included in the header file as javadoc docstrings.
