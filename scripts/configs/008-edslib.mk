CONFIG_NAMES += edslib
NONCFS_CONFIG_NAMES += edslib
O_edslib = build-edslib
PREP_OPTS_edslib += -DENABLE_UNIT_TESTS=TRUE
PREP_OPTS_edslib += -DCMAKE_PREFIX_PATH=$(HOME)/code/local/lib/cmake
PREP_OPTS_edslib +=  -S "$(CURDIR)/tools/eds"
