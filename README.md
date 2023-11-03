# cpplib

## What is it?

This is a boilerplate C++ library.

Initialy this library was a part of my another home project - a game-logic engine I'm
working on time to time. But the primary intention of that engine, and after all this
library as well, was to train my C++ skills: try new language features, cover "missing"
parts of STL library I'm interested in, add features needed for my other home projects.
That's it, **a playground in a form of boilerplate library**.

> **NOTE** The contents of this library is a permanent subject of change.

> **NOTE** This repository contains only blob-updates from the original repository
> maintained privately. So, unfortunately _git log_ records becoming almost useless here.

## Structure

> **NOTE** This section covers most (but not all) parts of the library.

<details><summary>Desired STL/language features</summary>

[[include/cpp/]](./include/cpp/)

* [lib_algorithm](./include/cpp/lib_algorithm) - Some advanced functions for **algorithm**
  (and probably **utility**) library
* [lib_bit](./include/cpp/lib_bit) - Endianess conversion and **bit_cast**
* [lib_chrono](./include/cpp/lib_chrono) - Simple stopwatch
* [lib_concepts](./include/cpp/lib_concepts) - Concepts for **type_traits** library
* [lib_debug](./include/cpp/lib_debug) - **assert** and **unused** support
* [lib_math](./include/cpp/lib_math) - **epsilon_eq** comparator
* [lib_scope](./include/cpp/lib_scope) - **scope_exit** feature
* [lib_type_traits](./include/cpp/lib_type_traits) - **conjunction**/**disjunction** for
  boolean arguments

</details>

<details><summary>Basic types</summary>

[[include/lib/]](./include/lib/)

* [types](./include/lib/types.hpp) - Fundamental types aliases
* [literals](./include/lib/literals.hpp) - User-defined literals for fundamental types.
* [pointers](./include/lib/ptr.hpp) - **std** smart-pointers aliases
* [references](./include/lib/ref.hpp) - **std::reference_wrapper** aliases
* [flags](./include/lib/flags.hpp) - Helper for enumerated flags

</details>

<details><summary>Template Library</summary>

[[include/lib/tl/]](./include/lib/tl/)

* [noncopyable](./include/lib/tl/noncopyable.hpp) - Standard **noncopyable**/**nonmovable**
  classes
* [listener](./include/lib/tl/listener.hpp) - Listener interface implementation
* [delegate](./include/lib/tl/delegate.hpp) - _C#_-like **delegate** implementation
* [result](./include/lib/tl/result.hpp) - _Rust_-like **result** implementation
* [linkable](./include/lib/tl/linkable.hpp) - one-to-one object linking interface

</details>

<details><summary>Binary data abstractions</summary>

[[include/lib/data/]](./include/lib/data/)

* [buffer](./include/lib/data/buffer.hpp) - Data buffer
* [stream](./include/lib/data/stream.hpp) - Read and write streams
* [serialize](./include/lib/data/serialize.hpp) - Serialization library
* [hash](./include/lib/data/hash.hpp) - Stream-based hash
* [codec](./include/lib/data/codec.hpp) - Stream-based codec (encoder/decoder)

</details>

<details><summary>Utility</summary>

[[include/lib/utils/]](./include/lib/utils/)

* [string](./include/lib/utils/string.hpp) - Different string-related functions
* [enum](./include/lib/utils/enum.hpp) - Enumeration support (helpers + _to_string_)
* [case_string](./include/lib/utils/case_string.hpp) - Case-conversion object (e.g. _Pascal_,
  _Camel_, _Snake_)
* [ids_pool](./include/lib/utils/ids_pool.hpp) - Simple pool of ID's implementation
* [value](./include/lib/utils/value.hpp) - Multitype value (integer, float, string)

</details>

<details><summary>Different</summary>

* [cli](./include/lib/cli/) - Command-line interface support
* [ecs](./include/lib/ecs/) - Simple _Entity Component System_
* [debug](./include/lib/debug/) - Useful classes for debug
* [test](./include/lib/test/) - Simple unit-test library
* [socket](./include/lib/socket/) - Client-server socket abstraction
* [system](./include/lib/system/) - System related stuff (e.g. error handling)

</details>

<details><summary>Implementations</summary>

[[include/lib/impl/]](./include/lib/impl/)

* [cli](./include/lib/impl/cli/) - CLI drivers
* [stream](./include/lib/impl/stream/) - Stream classes (e.g. buffer, fifo, file, etc.)
* [socket](./include/lib/impl/socket/) [[POSIX](./include/lib/impl_posix/socket/),
  [Windows](./include/lib/impl_windows/socket/)] - _TCP_ and _UNIX_ sockets
* [base64](./include/lib/impl/codec/base64.hpp) - _Base64_ codec
* [sha1](./include/lib/impl/hash/sha1.hpp) - _SHA1_ hash

</details>

## Compilation

> **NOTE** C++23 version support required.

<details><summary>GCC, Clang</summary>

Tested on _Linux_:
* _GCC 13.2.0_ (_x86_64_, _i686_, _aarch64_, _arm-gnueabi_, _arm-gnueabihf_)
* _Clang 16.0.6_ (_x86_64_, _aarch64_)
* _Clang 17.0.4_ (_aarch64-android24_)

```sh
# Building:
mkdir ./build
cd ./build
cmake -DCMAKE_BUILD_TYPE=Debug ../  # 'CMAKE_BUILD_TYPE' can be 'Debug' or 'Release'.
make -j 4                           # '-j N' where 'N' is the number of threads.

# Testing:
make run-test
# or
./test_cpplib                       # Type 'exit' or 'help' for 'Cli' test.

# Hexdump with CP437 charset output:
./hexdump_cpplib ./hexdump_cpplib | head -n 10

# Some graphics fun in the command line. :)
./graphics_plasma_cpplib            # Hit <Enter> to quit.
```

</details>

<details><summary>MinGW-w64</summary>

Tested on _x86_64_ and _i686_ architectures:
* _GCC 11.4.0_ on _Cygwin_
* _GCC 12.2.0_ on _Linux_
* _Not tested with MSYS2 :(_

```sh
# Building:
mkdir ./build
cd ./build
cmake --toolchain=../cmake/toolchain.x86_64-mingw-w64.cmake -DCMAKE_BUILD_TYPE=Debug ../
                                    # 'CMAKE_BUILD_TYPE' can be 'Debug' or 'Release'.
                                    # Toolchain files used in cases when CMake installation
                                    # doesn't have native MinGW-w64 support:
                                    #     * 'toolchain.x86_64-mingw-w64.cmake' used for x64 build;
                                    #     * 'toolchain.i686-mingw-w64.cmake' used for x86 build.
make -j 4                           # '-j N' where 'N' is the number of threads.

# At this step some MinGW-w64 libraries probably need to be copied into './build' directory:
#     * 'libgcc_s_seh-1.dll', 'libstdc++-6.dll', and 'libwinpthread-1.dll' for x64 on Linux;
#     * 'libgcc_s_dw2-1.dll', 'libstdc++-6.dll', and 'libwinpthread-1.dll' for x86 on Linux;
#     * 'libgcc_s_seh-1.dll', 'libstdc++-6.dll', and 'libwinpthread-1.dll' for x64 on Cygwin;
#     * 'libgcc_s_sjlj-1.dll', 'libstdc++-6.dll', and 'libwinpthread-1.dll' for x86 on Cygwin.

# Testing:
make run-test
# or
./test_cpplib.exe                   # Type 'exit' or 'help' for 'Cli' test.
                                    # On Linux it's possible to check with Wine:
                                    # $ wine ./test_cpplib.exe

# Hexdump with CP437 charset output:
./hexdump_cpplib.exe ./hexdump_cpplib.exe | head -n 10
                                    # On Linux it's possible to check with Wine:
                                    # $ wine ./hexdump_cpplib.exe ./hexdump_cpplib.exe | head -n 10
```

</details>

<details><summary>MSVC</summary>

Tested on _x64_ and _Win32_ architectures:
* _MSVC 19.37.32822_ on _Windows_

```bat
REM Open 'x64 Native Tools Command Prompt for VS 2022' for x64 build,
REM or open 'x86 Native Tools Command Prompt for VS 2022' for x86 build.

REM Building:
MKDIR build
CD build
cmake -A x64 ..\                  & REM '-A x64' for x64 build;
                                    REM '-A Win32' for x86 build.

msbuild cpplib.sln -p:Configuration=Debug -p:Platform=x64
                                    REM 'Configuration' may be 'Debug' or 'Release';
                                    REM 'Platform' may be 'x64' or 'Win32' (depends on toolset).

REM Testing:
Debug\test_cpplib.exe             & REM Type 'exit' or 'help' for 'Cli' test.
REM or
REM Release\test_cpplib.exe

REM Hexdump with CP437 charset output:
Debug\hexdump_cpplib.exe Debug\hexdump_cpplib.exe
                                    REM ...but be ready to hit <Ctrl>+<C>. :)
```

</details>

## Specifics

### Both headers and sources are placed in the _include_ directory

There are two reasons for that.
First of all I'm still waiting for complete _modules_ support.
Secondary, it's easier for me to maintain _hpp_ and _cpp_ files if they are placed
side-to-side.

### Headers within a library are included by relative paths

Historically the library was planned as a header-only one. Later I get off this idea, but wanted
to allow parts of the library used in-place without any changes within external project.
Just an easy drop-in. If user want to use only few classes (header-only ones) of the library,
it's possible to include them even by absolute path. This may be useful e.g. for
[debug](./include/lib/debug/) classes.

### Some places has strange/mixed code style

Remember, it's a playground?

Experimenting forsure will include readability and style. And yes, what considered readable
for me not always will be readable for anyone other and vice versa. Perhaps, some day I'll
put a codestyle guidelines to follow, but for now there is no such. So, all the "strange"
places always have some idea (good or not) on the background.

* _Alternative operator representations_ are used for all logical operators (_and_, _or_, _not_
  instead of _&&_, _||_, _!_). As for me, this should be a default for _C++_. Taking into
  account a not so rare typo like _if (!Illusion) ..._ , it's a way to _pythonify_ the code -
  make it more readable and error-proof.

* _C-Style_ cast is used for all non-object casting (like integers, floats, enumerations).
  It's a long story of getting frustrated to write 13 more symbols for "the same thing".
  But yeah, probably, some day all the _C-Style_ casts will be removed.

* _Lambda_'s [used](./include/test/lib/packets/handler.cpp#L179) (_lines 179 and 236_) for
  scoped blocks with _return_ statement instead of _do ... break ... while ( false );_ pattern
  (_note, CPPLIB&#95;&#95;TEST&#95;&#95;... has return statement inside_). I've found it as
  a more natural way to organize such blocks.

* _ADL_ not used for _str()_ function. For now it's considered [all](./include/lib/data/codec.hpp#L121)
  [the](./include/lib/socket/client.hpp#L66) _str_ function overloads should be defined in
  root (_::lib_) namespace. At least, current implementation of
  [LIB_UTILS_ENUM_NAMES](./include/lib/utils/enum.hpp#L86) depends on it.

* [Here](./include/lib/impl_posix/socket/tcp/server_client.hpp#L31) is an example of mixed cases
  (_lines 31 and 37_). The idea is to split lower and upper interfaces by using different
  cases: for low-level interfaces use _lower_snake_case_, for user-level use _camelCase_.

* Strange formatting can be found [here](./include/test/lib/impl/socket/tcp.cpp#L61)
  (_lines 61-64_) - experimenting on ways to format code to split main part _while_-conditions
  and simple _sleep_ body. You may consider it also as another kind of _pythonification_.

### Not all the classes covered by tests

Furthermore. Many of the tests cover tested classes only partially. Sometimes I need to
cover the whole interface by tests, sometimes, only desired functionality, sometimes I'm
too lazy or have no time to write a test.

### Bugs/ABI breaking

Definitely. Or at least perhaps. I'll trying to keep code clean and standard-compliant.
But still... It's a C++, someday you'll break at least a strict aliasing rule.

## License

All the sources are released under [MIT License](LICENSE).

[Some](./include/cpp/lib_bit#L33) [parts](./include/cpp/lib_math#L19) of the source code
are released under [CC BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/) license.
