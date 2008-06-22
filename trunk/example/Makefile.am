
AM_CXXFLAGS = -I../include 

AM_LDFLAGS = -L../src/ -L $(BOOST_LIB)
LDADD = -linet -lboost_system-gcc41-mt-1_35 -lz

noinst_PROGRAMS = delegate buffer serialize timer timeout \
	tcpechoclient tcpechoserver tcptimeclient tcptimeserver

delegate_SOURCES = delegate.cxx 

buffer_SOURCES = buffer.cxx 

serialize_SOURCES = serialize.cxx 

timer_SOURCES = timers.cxx 

timeout_SOURCES = timeout_service.cxx 

tcpechoclient_SOURCES = tcp_echo_client.cxx 

tcpechoserver_SOURCES = tcp_echo_server.cxx 

tcptimeclient_SOURCES = tcp_time_client.cxx 

tcptimeserver_SOURCES = tcp_time_server.cxx 

