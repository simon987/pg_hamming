#!/usr/bin/env bash

USER=dbuser
DATABASE=dbname
LIB_PATH='/path/to/libhamming.so'

psql -U $USER $DATABASE <<EOF
CREATE OR REPLACE FUNCTION hash_is_within_distance18(bytea, bytea, integer) RETURNS boolean
     AS $LIB_PATH, 'hash_is_within_distance18'
     LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hash_is_within_distance8(bytea, bytea, integer) RETURNS boolean
     AS $LIB_PATH, 'hash_is_within_distance8'
     LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION hash_distance18(bytea, bytea) RETURNS integer
     AS $LIB_PATH, 'hash_distance18'
     LANGUAGE C STRICT;
CREATE OR REPLACE FUNCTION hash_distance8(bytea, bytea) RETURNS integer
     AS $LIB_PATH, 'hash_distance8'
     LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION hash_is_within_distance18_any(bytea, bytea, integer) RETURNS bool
     AS $LIB_PATH, 'hash_is_within_distance18_any'
     LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION hash_equ18_any(bytea, bytea) RETURNS bool
     AS $LIB_PATH, 'hash_equ18_any'
     LANGUAGE C STRICT;
EOF
