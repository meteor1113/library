noinst_LIBRARIES = libsock.a
noinst_PROGRAMS = tcpserver tcpclient udpserver udpclient

libsock_a_SOURCES = \
	socket.cpp \
	socket.hpp

tcpserver_SOURCES = tcpserver.cpp
tcpserver_LDADD = libsock.a

tcpclient_SOURCES = tcpclient.cpp
tcpclient_LDADD = libsock.a

udpserver_SOURCES = udpserver.cpp
udpserver_LDADD = libsock.a

udpclient_SOURCES = udpclient.cpp
udpclient_LDADD = libsock.a
