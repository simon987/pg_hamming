FROM ubuntu:20.04 as build

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update
RUN apt install -y cmake postgresql-server-dev-12 -y

WORKDIR /build/
COPY . /build/

RUN cmake .

FROM postgres:12

COPY --from=build /build/libhamming.so /usr/lib/libhamming.so
COPY install.sh /docker-entrypoint-initdb.d/init-user-db.sh
