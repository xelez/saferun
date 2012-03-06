Author: Alexander Ankudinov

# Some good coding styles:
 * [Apache Developers' C Language Style Guide](http://httpd.apache.org/dev/styleguide.html)
 * [Mozilla Coding style](https://developer.mozilla.org/En/Developer_Guide/Coding_Style)
 * [Article "Coding Standards" by Ivan Gevirtz](http://www.ivanism.com/Articles/CodingStandards.html)
 * Linux Kernel Coding Style (you can found it in kernel git in Documentation/)

In fact all of those have very good ideas. I like Apache's Guide most of all.

# Most important things (mostly from Apache Style Guide):
 * Functions should be short and easily understood.
 * Comments should be provided to explain the rationale for code which is not obvious, and to document behavior of functions.
 * The guidelines can be broken if necessary to achieve a clearer layout.
 * Code must be easy readable, easy understandable and beautiful.
 * Document your code, keep documentation and code in sync.
 * It compiles with C++ compiler. But the only thing used from C++ must be exceptions.
 * If I haven't documented something here - see Apache Coding Standards, other source files and use your brain. 

# Indentation
 * Use 4 spaces for indentation. No tabs. (just because I like this style)

# Documentation
 * Use oxygen
 * Document where it is coded. For example, structs are documented in header files, functions in source files.
 * Don't document obvious things.

# Variable's naming
 * i, j, k - are ok for cycles.
 * t, tmp - are ok for temporary values.
 * Use meaningful names for anything else.

# Notes on header and source file structure
 * First system includes, and then local includes.

# Logging and profiling
 * Use modules for that.
 * If you need something more complex, then feel free to update those modules.

# Error handling
 * Use exceptions for this inside the library.
 * Don't use exceptions for anything else.
 * No exceptions should be thrown outside the library to be compatible with C code.
 * If and error occures: 1) log the error with one of the macroses, 2) throw an error.
 * If function is called outside the library then return -1 or NULL if error occures.

# Library interface
 * Declare all structs with typedefs to be compatible with C.
 * All exported symbols are in one main header file: saferun.h
 * All exported symbols should have "saferun\_" prefix.

# Suggestions are welcome
