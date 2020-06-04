FROM ubuntu:20.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive

ARG LCOV_VERSION=1.14-2

ARG ENABLE_UNIT_TESTS
ENV ENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS} 

ARG SIMULATION
ENV SIMULATION=${SIMULATION}

ARG BUILDTYPE
ENV BUILDTYPE=${BUILDTYPE}

ARG OMIT_DEPRECATED
ENV OMIT_DEPRECATED=${OMIT_DEPRECATED}

RUN apt-get -qy update                        \
    && apt-get -y install                     \
        ca-certificates=20190110ubuntu1.1     \
        git=1:2.25.1-1ubuntu3                 \
        cmake=3.16.3-1ubuntu1                 \
        make=4.2.1-1.2                        \
        gcc=4:9.3.0-1ubuntu2                  \
        g++=4:9.3.0-1ubuntu2                  \
    && rm -rf /var/lib/apt/lists/*          

# Optional: Install lcov
RUN if [ "${ENABLE_UNIT_TESTS}" = true ]; \
    then { apt-get update && \
    apt-get -y install software-properties-common=0.98.9 && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get -qy update && \
    apt-get -y install lcov=${LCOV_VERSION}; } fi                    

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


FROM ubuntu:20.04

COPY --from=builder /cFS/build /cFS/build

WORKDIR /cFS/build/exe/cpu1

ENTRYPOINT [ "./core-cpu1" ]