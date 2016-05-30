

DIR_ROOT=$(shell pwd)
#moded by caodi 2013-07-16 添加对powerpc系列板子的支持
# TOOLPREFIX=/opt/toolchains/mips-linux-uclibc/4.2.4/usr/bin/mips-linux-uclibc-
CC=$(TOOLPREFIX)g++

IMPLEMENTPATH=$(PWD)

LIBUSEROBJ=$(IMPLEMENTPATH)/ap_main.o \
		   $(IMPLEMENTPATH)/buffer.o \
		   $(IMPLEMENTPATH)/kvlist.o \
		   $(IMPLEMENTPATH)/lp_message.o \
		   $(IMPLEMENTPATH)/proto_defs.o \
		   $(IMPLEMENTPATH)/proto_elements.o \
		   $(IMPLEMENTPATH)/stringunit.o \
		   $(IMPLEMENTPATH)/timer.o \
		   $(IMPLEMENTPATH)/interfaces.o \
		   $(IMPLEMENTPATH)/private_message.o \
		   $(IMPLEMENTPATH)/driver_op.o


INCLUDE= -I$(IMPLEMENTPATH)/../ap_server/ -I$(IMPLEMENTPATH)/../web_tools/cgi-bin/library \
		 -I$(TOPDIR)/../../drivers/wlan/linux/ -I$(IMPLEMENTPATH)/../../public\
		 -I$(TOPDIR)/apps/ap_server/implement/cw/ -I.
LIBPATH= -L. -L$(INSTALL_ROOT)/lib  -lpthread -liconv -lrt
#LIBPATH= -lrt

CFLAGSS=$(INCLUDE)
CPPFLAGS= -g -c -fPIC -Wall $(INCLUDE)

TOOLS=apzit

all: $(TOOLS)
	echo $(DIR_ROOT)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(TOOLS):$(LIBUSEROBJ)
	$(CC) -g -o $(TOOLS) $(LIBUSEROBJ) $(INCLUDE) $(LIBPATH) -Wall


clean:
	rm -f $(TOOLS)
	rm -f $(LIBUSEROBJ)
