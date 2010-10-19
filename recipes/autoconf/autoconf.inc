DESCRIPTION = "A package of M4 macros to produce scripts to \
automatically configure sourcecode."
LICENSE = "GPLv3+-with-Autoconf-Configure-Script-Exception-v3.0"
HOMEPAGE = "http://www.gnu.org/software/autoconf/"
SECTION = "devel"
DEPENDS += "m4-native"
RDEPENDS_${PN} = "m4 perl gnu-config"
DEPENDS_virtclass-native = "m4-native gnu-config-native"
RDEPENDS_${PN}_virtclass-native = "m4-native gnu-config-native"

INC_PR = "r10"

SRC_URI = "${GNU_MIRROR}/autoconf/autoconf-${PV}.tar.bz2;name=autoconf \
	   file://program_prefix.patch"

inherit autotools

# Excluded: tests man doc
SUBDIRS = "bin . lib"

do_configure_prepend () {
	if ${@['true', 'false'][bb.data.inherits_class('native', d)]}
	then
		export ac_cv_path_PERL="${ac_cv_path_PERL=${bindir}/perl}"
		export ac_cv_path_M4="${ac_cv_path_M4=${bindir}/m4}"
		export ac_cv_prog_gnu_m4="${ac_cv_prog_gnu_m4=yes}"
	fi
}

do_compile () {
	sed -i -e's,^SUBDIRS = .*,SUBDIRS = ${SUBDIRS},' Makefile
	oe_runmake
}

SYSROOT_PREPROCESS_FUNCS_append_virtclass-native = " autoconf_fixup"

_SYSROOT_DEST = "${SYSROOT_DESTDIR}${STAGING_DIR_HOST}"
autoconf_fixup () {
	create_wrapper ${_SYSROOT_DEST}${bindir}/autoconf \
		AUTOM4TE=${bindir}/autom4te
	create_wrapper ${_SYSROOT_DEST}${bindir}/autom4te \
		autom4te_perllibdir=${datadir}/autoconf \
		AC_MACRODIR=${datadir}/autoconf \
		M4=${bindir}/m4
	create_wrapper ${_SYSROOT_DEST}${bindir}/autoheader \
		autom4te_perllibdir=${datadir}/autoconf \
		AUTOM4TE=${bindir}/autom4te
	create_wrapper ${_SYSROOT_DEST}${bindir}/autoreconf \
		autom4te_perllibdir=${datadir}/autoconf \
		AUTOM4TE=${bindir}/autom4te \
		AUTOCONF=${bindir}/autoconf \
		AUTOHEADER=${bindir}/autoheader
	create_wrapper ${_SYSROOT_DEST}${bindir}/ifnames \
		autom4te_perllibdir=${datadir}/autoconf
	create_wrapper ${_SYSROOT_DEST}${bindir}/autoscan \
		autom4te_perllibdir=${datadir}/autoconf \
		AUTOM4TE=${bindir}/autom4te
	create_wrapper ${_SYSROOT_DEST}${bindir}/autoupdate \
		autom4te_perllibdir=${datadir}/autoconf \
		AUTOM4TE=${bindir}/autom4te \
		M4=${bindir}/m4
}

PARALLEL_MAKE = ""
BBCLASSEXTEND = "native"
