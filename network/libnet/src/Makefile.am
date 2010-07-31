#
#  Author: Hailong.Xia (hlxxxx@gmail.com)
#  Copyright @ 2008-2009 IGameStudio All Rights Reserved
#

noinst_LIBRARIES = libnet.a 

libnet_a_CPPFLAGS = -Wno-deprecated -I../../../ -I $(BOOST_INCLUDE)
libnet_a_SOURCES = \
	Network.cpp \
	Service.cpp \
	Socket.cpp \
	TcpConnection.cpp \
	TcpListener.cpp \
	Timer.cpp \
	UdpBinder.cpp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)
	$(INSTALL_DATA) -m 700 libnet.a $(DESTDIR)$(prefix)



