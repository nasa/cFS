# Warning: This image is currently not operational 

FROM alpine:3.11 AS builder

ARG ENABLE_UNIT_TESTS
ENV ENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS} 

ARG SIMULATION
ENV SIMULATION=${SIMULATION}

ARG BUILDTYPE
ENV BUILDTYPE=${BUILDTYPE}

ARG OMIT_DEPRECATED
ENV OMIT_DEPRECATED=${OMIT_DEPRECATED}

RUN echo http://nl.alpinelinux.org/alpine/edge/testing >> /etc/apk/repositories

RUN set -ex &&                          \
    apk add --update --no-cache         \
        make=4.2.1-r2                   \
        cmake=3.15.5-r0                 \
        git=2.24.3-r0                   \
        gcc=9.2.0-r4                    \
        g++=9.2.0-r4

RUN if [ "${ENABLE_UNIT_TESTS}" = true ]; \
    then { echo http://nl.alpinelinux.org/alpine/edge/testing >> /etc/apk/repositories && \
    apk add --update --no-cache lcov=1.14-r0; } fi

WORKDIR /cFS

COPY . . 

RUN git submodule init \
    && git submodule update \
    && cp cfe/cmake/Makefile.sample Makefile \
    && cp -r cfe/cmake/sample_defs .

RUN make prep
RUN make
RUN make install
RUN if [ "${ENABLE_UNIT_TESTS}" = true ]; then { ( make test | grep 'Failed' ) && ( make lcov | grep '%' ); } fi
RUN if [ "${ENABLE_UNIT_TESTS}" = true ]; then { cat ./build/native/Testing/Temporary/LastTest.log | grep 'FAIL' | grep -v 'FAIL::0'; } fi


FROM alpine:3.11

COPY --from=builder /cFS/build /cFS/build

WORKDIR /cFS/build/exe/cpu1

CMD [ "./core-cpu1" ]