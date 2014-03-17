# test_pdu/Makefile.am #

TESTAPPS_SOURCE_DIR = $(top_srcdir)/test_apps
sendwp_SOURCES = \
    $(TESTAPPS_SOURCE_DIR)/tcp.c\
    $(TESTAPPS_SOURCE_DIR)/smpp.c\
    $(TESTAPPS_SOURCE_DIR)/sendwp.h\
    $(TESTAPPS_SOURCE_DIR)/sendwp.c

sendwp_CPPFLAGS = `xml2-config --cflags`
sendwp_LDFLAGS = `xml2-config --libs`
sendwp_pkgdatadir = $(prefix)/bin
sendwp_pkgdata_DATA = \
     $(TESTAPPS_SOURCE_DIR)/sendwp.xml

esme_SOURCES = \
    $(TESTAPPS_SOURCE_DIR)/tcp.c\
    $(TESTAPPS_SOURCE_DIR)/smpp.c\
    $(TESTAPPS_SOURCE_DIR)/esme.h\
    $(TESTAPPS_SOURCE_DIR)/esme.c

esme_CPPFLAGS = `xml2-config --cflags`
esme_LDFLAGS = `xml2-config --libs`
esme_pkgdatadir = $(prefix)/bin
esme_pkgdata_DATA = \
     $(TESTAPPS_SOURCE_DIR)/esme.xml

analizer_SOURCES = \
    $(TESTAPPS_SOURCE_DIR)/analizer.c

analizer_CPPFLAGS = 
analizer_LDFLAGS = 
analizer_pkgdatadir = 
analizer_pkgdata_DATA = 

