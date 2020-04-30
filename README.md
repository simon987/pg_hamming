# PostgreSQL hamming distance

Hamming distance for fixed-length `bytea` datatype (8-byte, 32-byte and 128-byte).


### Compiling from source (CMake)

```bash
apt install postgresql-server-11-dev
cmake .
make
```

Will break catastrophically if function arguments are not exactly the correct length.

See [hamming.c](hamming.c) for more information


