
AM_CXXFLAGS = -I../include 

AM_LDFLAGS = -L../src/ -L $(BOOST_LIB)
LDADD = -linet -lboost_system-gcc41-mt-1_35

noinst_PROGRAMS = benchmark server

benchmark_SOURCES = benchmark.cxx getopt.cxx

server_SOURCES = server.cxx getopt.cxx

