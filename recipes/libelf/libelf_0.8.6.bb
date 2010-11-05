DESCRIPTION = "libelf is an ELF object file access library. \
The elf library provides routines to access, and manipulate, Elf object files."
LICENSE = "LGPL"
SECTION = "libs"
PR = "r2"

SRC_URI = "http://www.mr511.de/software/libelf-${PV}.tar.gz;name=archive \
           http://www.stud.uni-hannover.de/~michael/software/libelf-${PV}.tar.gz;name=archive"

inherit autotools

PARALLEL_MAKE = ""

TARGET_CC_ARCH += "${LDFLAGS}"

do_configure_prepend () {
	if test ! -e ${S}/acinclude.m4; then
		cp ${S}/aclocal.m4 ${S}/acinclude.m4
	fi
}

do_install () {
	oe_runmake 'prefix=${D}${prefix}' 'exec_prefix=${D}${exec_prefix}' \
		   'libdir=${D}${libdir}' 'includedir=${D}${includedir}' \
		   install
}
BBCLASSEXTEND = "native"
# both SRC_URI items are the same file
SRC_URI[archive.md5sum] = "d444fb0068cdfed01bb1fd1e91d29270"
SRC_URI[archive.sha256sum] = "6ff7a5dbb5ccf14995f6bde7f1fca6be5f7f91f62b2680a00d32e82b172c9499"
