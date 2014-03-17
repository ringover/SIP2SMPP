# Makefile.am #
bin_PROGRAMS = submit_multi_resp_test submit_multi_test alert_notification_test bind_receiver_resp_test bind_receiver_test bind_transceiver_resp_test bind_transceiver_test bind_transmitter_resp_test bind_transmitter_test cancel_sm_resp_test cancel_sm_test data_sm_resp_test data_sm_test deliver_sm_resp_test deliver_sm_test enquire_link_resp_test enquire_link_test generic_nack_test outbind_test query_sm_resp_test query_sm_test replace_sm_resp_test replace_sm_test submit_sm_resp_test submit_sm_test unbind_resp_test unbind_test esme analizer sendwp

include $(top_srcdir)/test_pdu/Makefile.am
include $(top_srcdir)/test_apps/Makefile.am
include $(top_srcdir)/src/Makefile.am

LDADD = $(top_srcdir)/binary/libsmpp34.la
AM_CPPFLAGS = -D_REENTRANT -DBSD_COMP -D_POSIX_PTHREAD_SEMANTICS
AM_CFLAGS = @CFLAGS@ -Wall -fPIC # -static
INCLUDES = -I../src -I..
AM_LDFLAGS = -lrt -lpthread
OBJ_EXTRA_DIST = $(esme_EXTRA_DIST)
