#include <postgres.h>
#include <fmgr.h>

__always_inline
static int _distance_8(const char *h1, const char *h2) {
    return __builtin_popcountll(*((uint64 *) h1) ^ *((uint64 *) h2));
}

__always_inline
static int _distance_32(const char *h1, const char *h2) {
    int distance = 0;
    distance += __builtin_popcountll(*((uint64 *) h1) ^ *((uint64 *) h2));
    distance += __builtin_popcountll(*((uint64 *) h1 + 1) ^ *((uint64 *) h2 + 1));
    distance += __builtin_popcountll(*((uint64 *) h1 + 2) ^ *((uint64 *) h2 + 2));
    distance += __builtin_popcountll(*((uint64 *) h1 + 3) ^ *((uint64 *) h2 + 3));
    return distance;
}

__always_inline
static int _distance_128(const char *h1, const char *h2) {
    int distance = 0;
    distance += __builtin_popcountll(*((uint64 *) h1) ^ *((uint64 *) h2));
    distance += __builtin_popcountll(*((uint64 *) h1 + 1) ^ *((uint64 *) h2 + 1));
    distance += __builtin_popcountll(*((uint64 *) h1 + 2) ^ *((uint64 *) h2 + 2));
    distance += __builtin_popcountll(*((uint64 *) h1 + 3) ^ *((uint64 *) h2 + 3));

    distance += __builtin_popcountll(*((uint64 *) h1 + 4) ^ *((uint64 *) h2 + 4));
    distance += __builtin_popcountll(*((uint64 *) h1 + 5) ^ *((uint64 *) h2 + 5));
    distance += __builtin_popcountll(*((uint64 *) h1 + 6) ^ *((uint64 *) h2 + 6));
    distance += __builtin_popcountll(*((uint64 *) h1 + 7) ^ *((uint64 *) h2 + 7));

    distance += __builtin_popcountll(*((uint64 *) h1 + 8) ^ *((uint64 *) h2 + 8));
    distance += __builtin_popcountll(*((uint64 *) h1 + 9) ^ *((uint64 *) h2 + 9));
    distance += __builtin_popcountll(*((uint64 *) h1 + 10) ^ *((uint64 *) h2 + 10));
    distance += __builtin_popcountll(*((uint64 *) h1 + 11) ^ *((uint64 *) h2 + 11));

    distance += __builtin_popcountll(*((uint64 *) h1 + 12) ^ *((uint64 *) h2 + 12));
    distance += __builtin_popcountll(*((uint64 *) h1 + 13) ^ *((uint64 *) h2 + 13));
    distance += __builtin_popcountll(*((uint64 *) h1 + 14) ^ *((uint64 *) h2 + 14));
    distance += __builtin_popcountll(*((uint64 *) h1 + 15) ^ *((uint64 *) h2 + 15));
    return distance;
}


PG_MODULE_MAGIC;

/**
 * Check if the hamming distance of the two raw byte arrays
 * is within the specified distance
 *
 * It is assumed that: the two arrays are exactly 8 bytes long
 *
 * Import with
    CREATE OR REPLACE FUNCTION hash_is_within_distance(bytea, bytea, integer) RETURNS boolean
     AS '/path/to/libhamming.so', 'hash_is_within_distance'
     LANGUAGE C STRICT;
 *
 * @return the hamming distance between the two arrays
 */
PG_FUNCTION_INFO_V1(hash_is_within_distance8);

Datum hash_is_within_distance8(PG_FUNCTION_ARGS) {
    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));
    int32 max_distance = PG_GETARG_INT32(2);

    PG_RETURN_BOOL(_distance_8(h1, h2) <= max_distance);
}

/** 32-byte **/
PG_FUNCTION_INFO_V1(hash_is_within_distance32);

Datum hash_is_within_distance32(PG_FUNCTION_ARGS) {
    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));
    int32 max_distance = PG_GETARG_INT32(2);

    PG_RETURN_BOOL(_distance_32(h1, h2) <= max_distance);
}

/** 128-byte **/
PG_FUNCTION_INFO_V1(hash_is_within_distance128);

Datum hash_is_within_distance128(PG_FUNCTION_ARGS) {
    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));
    int32 max_distance = PG_GETARG_INT32(2);

    PG_RETURN_BOOL(_distance_128(h1, h2) <= max_distance);
}

/**
 * Hamming distance of two raw byte arrays
 *
 * It is assumed that: the two arrays are exactly 8 bytes long
 *
 * Import with
    CREATE OR REPLACE FUNCTION hash_distance(bytea, bytea) RETURNS integer
     AS '/path/to/libhamming.so', 'hash_distance'
     LANGUAGE C STRICT;
 *
 * @return the hamming distance between the two arrays
 */

PG_FUNCTION_INFO_V1(hash_distance8);

Datum hash_distance8(PG_FUNCTION_ARGS) {
    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));

    PG_RETURN_INT32(_distance_8(h1, h2));
}

/** 32-byte **/
PG_FUNCTION_INFO_V1(hash_distance32);

Datum hash_distance32(PG_FUNCTION_ARGS) {
    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));

    PG_RETURN_INT32(_distance_32(h1, h2));
}

/** 128-byte **/
PG_FUNCTION_INFO_V1(hash_distance128);

Datum hash_distance128(PG_FUNCTION_ARGS) {
    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));

    PG_RETURN_INT32(_distance_128(h1, h2));
}
