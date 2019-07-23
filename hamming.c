#include "postgresql/server/postgres.h"
#include "postgresql/server/fmgr.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hash_is_within_distance);

/**
 * Check if the hamming distance of the two raw byte arrays
 * is within the specified distance
 *
 * It is assumed that: the two arrays are exactly 18 bytes long
 *
 * Import with
    CREATE OR REPLACE FUNCTION hash_is_within_distance(bytea, bytea, integer) RETURNS boolean
     AS '/path/to/libhamming.so', 'hash_is_within_distance'
     LANGUAGE C STRICT;
 *
 * @return the hamming distance between the two arrays
 */
Datum hash_is_within_distance(PG_FUNCTION_ARGS) {

    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));
    int32 max_distance = PG_GETARG_INT32(2);

    int distance = 0;

    distance += __builtin_popcountll(
            *((uint64 *) h1) ^ *((uint64 *) h2)
    );
    if (distance > max_distance) {
        PG_RETURN_BOOL(false);
    }
    distance += __builtin_popcountll(
            *((uint64 *) h1 + 1) ^ *((uint64 *) h2 + 1)
    );
    if (distance > max_distance) {
        PG_RETURN_BOOL(false);
    }
    distance += __builtin_popcount(
            *((uint16 *) h1 + 8) ^ *((uint16 *) h2 + 8)
    );

    PG_RETURN_BOOL(distance <= max_distance);
}

PG_FUNCTION_INFO_V1(hash_distance);

/**
 * Hamming distance of two raw byte arrays
 *
 * It is assumed that: the two arrays are exactly 18 bytes long
 *
 * Import with
    CREATE OR REPLACE FUNCTION hash_distance(bytea, bytea) RETURNS integer
     AS '/path/to/libhamming.so', 'hash_distance'
     LANGUAGE C STRICT;
 *
 * @return the hamming distance between the two arrays
 */
Datum hash_distance(PG_FUNCTION_ARGS) {

    char *h1 = VARDATA(PG_GETARG_BYTEA_P(0));
    char *h2 = VARDATA(PG_GETARG_BYTEA_P(1));

    int distance = 0;

    distance += __builtin_popcountll(
            *((uint64 *) h1) ^ *((uint64 *) h2)
    );
    distance += __builtin_popcountll(
            *((uint64 *) h1 + 1) ^ *((uint64 *) h2 + 1)
    );
    distance += __builtin_popcount(
            *((uint16 *) h1 + 8) ^ *((uint16 *) h2 + 8)
    );

    PG_RETURN_INT32(distance);
}


PG_FUNCTION_INFO_V1(hash_is_within_distance_any);

/**
 * Check if the first argument matches any (within distance 'max_distance')
    hashes among an array of hashes
 *
 * It is assumed that: the first array is exactly 18 bytes long, the
    second array length is a multiple of 18 bytes
 *
 * Import with
    CREATE OR REPLACE FUNCTION hash_is_within_distance_any(bytea, bytea, integer) RETURNS bool
     AS '/path/to/libhamming.so', 'hash_is_within_distance_any'
     LANGUAGE C STRICT;
 *
 * @return true if at least 1 hash matches
 */
Datum hash_is_within_distance_any(PG_FUNCTION_ARGS) {

    char *h = VARDATA(PG_GETARG_BYTEA_P(0));
    bytea *h_bytea = PG_GETARG_BYTEA_P(1);
    char *h_arr = VARDATA(h_bytea);
    int32 max_distance = PG_GETARG_INT32(2);

    int distance;

    for (int i = (VARSIZE(h_bytea) - VARHDRSZ) / 18 - 1; i >= 0; i--) {

        distance = __builtin_popcountll(
                *((uint64 *) h) ^ *((uint64 *) h_arr)
        );
        if (distance > max_distance) {
            h_arr += 18;
            continue;
        }
        distance += __builtin_popcountll(
                *((uint64 *) h + 1) ^ *((uint64 *) h_arr + 1)
        );
        if (distance > max_distance) {
            h_arr += 18;
            continue;
        }
        distance += __builtin_popcount(
                *((uint16 *) h + 8) ^ *((uint16 *) h_arr + 8)
        );

        if (distance <= max_distance) {
            PG_RETURN_BOOL(true);
        }
        h_arr += 18;
    }

    PG_RETURN_BOOL(false);
}

PG_FUNCTION_INFO_V1(hash_equ_any);

/**
 * Check if the first argument exactly matches any hashes among an array of hashes
 *
 * It is assumed that: the first array is exactly 18 bytes long, the
    second array length is a multiple of 18 bytes
 *
 * Import with
 * CREATE OR REPLACE FUNCTION hash_equ_any(bytea, bytea) RETURNS bool
     AS '/path/to/libhamming.so', 'hash_equ_any'
     LANGUAGE C STRICT;
 * @return true if at least 1 hash is equal
 */
Datum hash_equ_any(PG_FUNCTION_ARGS) {

    char *h = VARDATA(PG_GETARG_BYTEA_P(0));
    bytea *h_bytea = PG_GETARG_BYTEA_P(1);
    char *h_arr = VARDATA(h_bytea);

    for (int i = (VARSIZE(h_bytea) - VARHDRSZ) / 18 - 1; i >= 0; i--) {

        // This is a bit faster than __builtin_memcmp
        if (*((uint64 *) h) == *((uint64 *) h_arr) &&
            *((uint64 *) h + 1) == *((uint64 *) h_arr + 1) &&
            *((uint16 *) h + 8) == *((uint16 *) h_arr + 8)) {
            PG_RETURN_BOOL(true);
        }

        h_arr += 18;
    }

    PG_RETURN_BOOL(false);
}
