FROM ubuntu:18.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive

ARG LCOV_VERSION=1.13-3

ARG ENABLE_UNIT_TESTS 
ENV ENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS} 

ARG SIMULATION
ENV SIMULATION=${SIMULATION}

ARG BUILDTYPE
ENV BUILDTYPE=${BUILDTYPE}

ARG OMIT_DEPRECATED
ENV OMIT_DEPRECATED=${OMIT_DEPRECATED}

RUN apt-get -qy update                              \
    && apt-get -y install --no-install-recommends   \
        ca-certificates=20190110~18.04.1            \
        git=1:2.17.1-1ubuntu0.7                     \
        cmake=3.10.2-1ubuntu2.18.04.1               \
        make=4.1-9.1ubuntu1                         \
        gcc=4:7.4.0-1ubuntu2.3                      \
        g++=4:7.4.0-1ubuntu2.3                      \
    && rm -rf /var/lib/apt/lists/*                

# Optional: Install lcov
RUN if [ "${ENABLE_UNIT_TESTS}" = true ]; then { apt-get -qy update && apt-get -y install lcov=${LCOV_VERSION}; } fi

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


FROM ubuntu:18.04

COPY --from=builder /cFS/build /cFS/build

WORKDIR /cFS/build/exe/cpu1

ENTRYPOINT [ "./core-cpu1" ]