#
#  Author: Hailong.Xia (hlxxxx@gmail.com)
#  Copyright @ 2008-2009 IGameStudio All Rights Reserved
#

AM_CPPFLAGS = -Wno-deprecated -I../../../ -I $(BOOST_INCLUDE)
AM_LDFLAGS = -L../../lib/ -L../../../os/lib/
LDADD = -los -lnet -lpthread ../../lib/libboost_system.so.1.40.0

noinst_PROGRAMS = tcpcomplexdemoserver tcpechoclient tcpechoserver \
	tcpsimpledemoclient tcpsimpledemoserver timer

tcpcomplexdemoserver_SOURCES = TcpComplexDemoServer.cpp
tcpechoclient_SOURCES = TcpEchoClient.cpp
tcpechoserver_SOURCES = TcpEchoServer.cpp
tcpsimpledemoclient_SOURCES = TcpSimpleDemoClient.cpp
tcpsimpledemoserver_SOURCES = TcpSimpleDemoServer.cpp
timer_SOURCES = Timer.cpp


