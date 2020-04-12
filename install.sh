#!/usr/bin/env bash

mv libhamming.so /usr/lib/

USER=postgres
DATABASE=imhashdb
LIB_PATH="'/usr/lib/libhamming.so'"

psql -U $USER $DATABASE <<EOF
CREATE OR REPLACE FUNCTION hash_is_within_distance8(bytea, bytea, integer) RETURNS boolean
     AS $LIB_PATH, 'hash_is_within_distance8' LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hash_is_within_distance32(bytea, bytea, integer) RETURNS boolean
     AS $LIB_PATH, 'hash_is_within_distance32'
     LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hash_is_within_distance128(bytea, bytea, integer) RETURNS boolean
     AS $LIB_PATH, 'hash_is_within_distance128'
     LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION hash_distance8(bytea, bytea) RETURNS integer
     AS $LIB_PATH, 'hash_distance8'
     LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hash_distance32(bytea, bytea) RETURNS integer
     AS $LIB_PATH, 'hash_distance32'
     LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hash_distance128(bytea, bytea) RETURNS integer
     AS $LIB_PATH, 'hash_distance128'
     LANGUAGE C STRICT;
EOF
