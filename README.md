# PostgreSQL hamming distance

Hamming distance for fixed-length `bytea` datatype.


### Compiling from source (CMake)

```bash
cmake .
make
```

The functions in this library are very domain-specific and can realistically
only be used within the scope of [irarchives](https://github.com/simon987/irarchives).

See [hamming.c](hamming.c) for more information
