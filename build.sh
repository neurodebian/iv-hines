#!/bin/sh
# how to build from just the cvs sources

if test ! -f "ltmain.sh" ; then
	libtoolize -c -f #creates ltmain.sh config.guess config.sub libtool.m4
fi
        
aclocal
autoheader
autoconf

#./configure --prefix=`pwd`
# make
# make install

