#
#  Author: Hailong.Xia (hlxxxx@gmail.com)
#  Copyright @ 2008-2009 IGameStudio All Rights Reserved
#

AM_CPPFLAGS = -Wno-deprecated -I../../../ -I $(BOOST_INCLUDE)
AM_LDFLAGS = -L../../lib/
LDADD = -los -lpthread

noinst_PROGRAMS = container csvreader delegate fifo lockfreefifo logger \
        serialize streambuffer thread timeoutservice wordfilter objectsync \
	tinyjson encodingconvert

container_SOURCES = Container.cpp
csvreader_SOURCES = CsvReader.cpp
delegate_SOURCES = Delegate.cpp
fifo_SOURCES = Fifo.cpp
lockfreefifo_SOURCES = LockFreeFifo.cpp
logger_SOURCES = Logger.cpp
serialize_SOURCES = Serialize.cpp
streambuffer_SOURCES = StreamBuffer.cpp
thread_SOURCES = Thread.cpp
timeoutservice_SOURCES = TimeoutService.cpp
wordfilter_SOURCES = WordFilter.cpp
objectsync_SOURCES = ObjectSync.cpp
tinyjson_SOURCES = TinyJson.cpp
encodingconvert_SOURCES = EncodingConvert.cpp

