#
#  Author: Hailong.Xia (hlxxxx@gmail.com)
#  Copyright @ 2008-2009 IGameStudio All Rights Reserved
#

noinst_LIBRARIES = libos.a 

libos_a_CPPFLAGS = -Wno-deprecated -I../../../ -I $(BOOST_INCLUDE)
libos_a_SOURCES = Condition.cpp Logger.cpp Mutex.cpp Thread.cpp Time.cpp TinyStr.cpp \
	TinyXml.cpp TinyXmlError.cpp TinyXmlParser.cpp TinyJsonParser.cpp EncodingConvert.cpp

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)
	$(INSTALL_DATA) -m 700 libos.a $(DESTDIR)$(prefix)

