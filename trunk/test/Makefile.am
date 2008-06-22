
AM_CXXFLAGS = -I../include 

AM_LDFLAGS = -L../src/ -L $(BOOST_LIB)
LDADD = -linet -lboost_system-gcc41-mt-1_35

noinst_PROGRAMS = tcpserver

tcpserver_SOURCES = tcp_server.cxx 

