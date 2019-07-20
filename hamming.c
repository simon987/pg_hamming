#include "postgresql/server/postgres.h"
#include "postgresql/server/fmgr.h"

PG_MODULE_MAGIC;

int const HASH_SIZE = 12;
int const HASH_BITS = HASH_SIZE * HASH_SIZE;
int const BYTEA_LEN = HASH_BITS / 8;

PG_FUNCTION_INFO_V1(hash_is_within_distance);

/**
 * Check if the hamming distance of the two raw byte arrays
 * is within the specified distance
 *
 * It is assumed that: the two arrays are exactly
 *  BYTEA_LEN bytes long
 *
 * Import with
    CREATE OR REPLACE FUNCTION hash_is_within_distance(bytea, bytea, integer) RETURNS boolean
     AS '/path/to/libhamming.so', 'hash_is_within_distance'
     LANGUAGE C STRICT;'
 *
 * @return the hamming distance between the two arrays
 */
Datum hash_is_within_distance(PG_FUNCTION_ARGS) {

    bytea *hash1 = PG_GETARG_BYTEA_P(0);
    bytea *hash2 = PG_GETARG_BYTEA_P(1);
    int32 max_distance = PG_GETARG_INT32(2);

    int distance = 0;

    char *h1 = hash1->vl_len_;
    char *h2 = hash2->vl_len_;

    for (int i = BYTEA_LEN; i >= 0; i--) {
        distance += __builtin_popcount(h1[i] ^ h2[i]);

        if (distance > max_distance) {
            PG_RETURN_BOOL(false);
        }
    }

    PG_RETURN_BOOL(true);
}

PG_FUNCTION_INFO_V1(hash_distance);

/**
 * Hamming distance of two raw byte arrays
 *
 * It is assumed that: the two arrays are exactly
 *  BYTEA_LEN bytes long
 *
 * Import with
    CREATE OR REPLACE FUNCTION hash_distance(bytea, bytea) RETURNS integer
     AS '/path/to/libhamming.so', 'hash_distance'
     LANGUAGE C STRICT;'
 *
 * @return the hamming distance between the two arrays
 */
Datum hash_distance(PG_FUNCTION_ARGS) {

    bytea *hash1 = PG_GETARG_BYTEA_P(0);
    bytea *hash2 = PG_GETARG_BYTEA_P(1);

    int distance = 0;

    char *h1 = hash1->vl_len_;
    char *h2 = hash2->vl_len_;

    for (int i = BYTEA_LEN; i >= 0; i--) {
        distance += __builtin_popcount(h1[i] ^ h2[i]);
    }

    PG_RETURN_INT32(distance);
}
