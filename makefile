################################################################################
#   Description	: Makefile for util library file                               #
#   Copyright   : Futures Barain Com.,LTD                                      #
#   Date        : 2009.05.25                                                   #
#   Remark      :                                                              #
################################################################################

HOMEDIR	= $(FEP_HOME)

SRCS	= util.c
OBJS	= util.o

INCDIR  = $(HOMEDIR)/include
LIBDIR  = $(HOMEDIR)/lib
LIBNAME = libutil.a

CFLAGS  = -c -I$(INCDIR)
ARFLAGS	= rcs

defult	: $(SRCS)
	cc $(CFLAGS) $(SRCS)
	ar $(ARFLAGS) $(LIBNAME) $(OBJS)
	rm -f $(OBJS) 

install	: $(SRCS)
	cc $(CFLAGS) $(SRCS)
	ar $(ARFLAGS) $(LIBNAME) $(OBJS)
	mv $(LIBNAME) $(LIBDIR)
	rm -f $(OBJS) 

################################################################################
#                             End of File                                      #
################################################################################
