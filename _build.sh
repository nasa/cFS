#! /bin/bash
make SIMULATION=native OMIT_DEPRECATED=true prep
make -j 6
make install