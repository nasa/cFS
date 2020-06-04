FROM centos:7 AS builder

ARG LCOV_VERSION=1.13-1.el7

ARG ENABLE_UNIT_TESTS
ENV ENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS} 

ARG SIMULATION
ENV SIMULATION=${SIMULATION}

ARG BUILDTYPE
ENV BUILDTYPE=${BUILDTYPE}

ARG OMIT_DEPRECATED
ENV OMIT_DEPRECATED=${OMIT_DEPRECATED}

RUN yum -y -q update                    \
    && yum -y install                   \
        git-1.8.3.1-21.el7_7.x86_64     \
        cmake-2.8.12.2-2.el7.x86_64     \
        make-3.82-24.el7.x86_64         \
        gcc-4.8.5-39.el7.x86_64

RUN if [ "${ENABLE_UNIT_TESTS}" = true ]; \
    then { curl https://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/l/lcov-${LCOV_VERSION}.noarch.rpm \
           -o lcov-${LCOV_VERSION}.noarch.rpm \
           && yum -y install lcov-${LCOV_VERSION}.noarch.rpm; } fi

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


FROM centos:7

COPY --from=builder /cFS/build /cFS/build

WORKDIR /cFS/build/exe/cpu1

CMD [ "./core-cpu1" ]