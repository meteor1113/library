EXTRA_DIST = tcpclient.dsp tcpserver.dsp ucpserver.dsp udpclient.dsp

INCLUDES = -I$(top_srcdir)/include

noinst_PROGRAMS = tcpserver tcpclient udpserver udpclient

tcpserver_SOURCES = tcpserver.cpp
tcpserver_LDADD = ../src/libsock.a

tcpclient_SOURCES = tcpclient.cpp
tcpclient_LDADD = ../src/libsock.a

udpserver_SOURCES = udpserver.cpp
udpserver_LDADD = ../src/libsock.a

udpclient_SOURCES = udpclient.cpp
udpclient_LDADD = ../src/libsock.a
