#
# Master $(MAKE)file for unit tests
#

all: 
	$(MAKE) -C ut_format_tctf
	$(MAKE) -C ut_format_tmtf
	$(MAKE) -C ut_service_cop1
	$(MAKE) -C ut_service_tc_sync
	$(MAKE) -C ut_service_tm_sdlp
	$(MAKE) -C ut_service_tm_sync
	$(MAKE) -C ut_trans_rs422
	$(MAKE) -C ut_trans_udp

clean:
	rm -f ./*.o
	$(MAKE) -C ut_format_tctf clean
	$(MAKE) -C ut_format_tmtf clean
	$(MAKE) -C ut_service_cop1 clean
	$(MAKE) -C ut_service_tc_sync clean
	$(MAKE) -C ut_service_tm_sdlp clean
	$(MAKE) -C ut_service_tm_sync clean
	$(MAKE) -C ut_trans_rs422 clean
	$(MAKE) -C ut_trans_udp clean

run:
# note the leading "-" makes it always run and ignore errors
	-$(MAKE) -C ut_format_tctf run
	-$(MAKE) -C ut_format_tmtf run
	-$(MAKE) -C ut_service_cop1 run
	-$(MAKE) -C ut_service_tc_sync run
	-$(MAKE) -C ut_service_tm_sdlp run
	-$(MAKE) -C ut_service_tm_sync run
	-$(MAKE) -C ut_trans_rs422 run
	-$(MAKE) -C ut_trans_udp run

gcov:
	$(MAKE) -C ut_format_tctf gcov
	$(MAKE) -C ut_format_tmtf gcov
	$(MAKE) -C ut_service_cop1 gcov
	$(MAKE) -C ut_service_tc_sync gcov
	$(MAKE) -C ut_service_tm_sdlp gcov
	$(MAKE) -C ut_service_tm_sync gcov
	$(MAKE) -C ut_trans_rs422 gcov
	$(MAKE) -C ut_trans_udp gcov



