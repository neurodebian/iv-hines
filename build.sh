#!/bin/sh
# how to build from just the cvs sources

if test -f /usr/bin/glibtoolize ; then
	glibtoolize -c -f
else
	libtoolize -c -f ; then#creates ltmain.sh config.guess config.sub
fi
    
aclocal
autoheader
autoconf

#./configure --prefix=`pwd`
# make
# make install

