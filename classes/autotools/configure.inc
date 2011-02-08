# use autotools_stage_all for native packages
AUTOTOOLS_NATIVE_STAGE_INSTALL = "1"

EXTRA_OEMAKE = ""

inherit siteinfo

def _autotools_get_sitefiles(d):
    if oe.utils.inherits(d, 'native', 'nativesdk'):
        return

    sitedata = siteinfo_data(d)
    for path in d.getVar("BBPATH", True).split(":"):
        for element in sitedata:
            filename = os.path.join(path, "site", element)
            if os.path.exists(filename):
                yield filename

# Space separated list of shell scripts with variables defined to supply test
# results for autoconf tests we cannot run at build time.
export CONFIG_SITE = "${@' '.join(_autotools_get_sitefiles(d))}"

def autotools_set_crosscompiling(d):
    if not bb.data.inherits_class('native', d):
        return " cross_compiling=yes"
    return ""

def append_libtool_sysroot(d):
    if bb.data.getVar('LIBTOOL_HAS_SYSROOT', d, 1) == "yes":
        if bb.data.getVar('BUILD_SYS', d, 1) == bb.data.getVar('HOST_SYS', d, 1):
            return '--with-libtool-sysroot'
        else:
            return '--with-libtool-sysroot=${STAGING_DIR_HOST}'
    return ''

def distro_imposed_configure_flags(d):
    distro_features = bb.data.getVar('DISTRO_FEATURES', d, True) or ""
    distro_features = distro_features.split()
    flags = set()
    features = (('largefile', 'largefile'),
        ('ipv6'     , 'ipv6'),
        ('nls'      , 'nls'))

    for knob, cfgargs in features:
        if isinstance(cfgargs, basestring):
            cfgargs = [cfgargs]
        en_or_dis = knob in distro_features and "enable" or "disable"
        for flg in cfgargs:
            flags.add("--%s-%s" % (en_or_dis, flg))
    return " ".join(flags)

CONFIGUREOPTS = " --build=${BUILD_SYS} \
          --host=${HOST_SYS} \
          --target=${TARGET_SYS} \
          --prefix=${prefix} \
          --exec_prefix=${exec_prefix} \
          --bindir=${bindir} \
          --sbindir=${sbindir} \
          --libexecdir=${libexecdir} \
          --datadir=${datadir} \
          --sysconfdir=${sysconfdir} \
          --sharedstatedir=${sharedstatedir} \
          --localstatedir=${localstatedir} \
          --libdir=${libdir} \
          --includedir=${includedir} \
          --oldincludedir=${oldincludedir} \
          --infodir=${infodir} \
          --mandir=${mandir} \
          ${@append_libtool_sysroot(d)} \
          ${@distro_imposed_configure_flags(d)} \
        "

oe_runconf () {
    if [ -x ${S}/configure ] ; then
        ${S}/configure \
        ${CONFIGUREOPTS} ${EXTRA_OECONF} "$@"
    else
        oefatal "no configure script found"
    fi
}

PACKAGE_PREPROCESS_FUNCS += "${@['autotools_prepackage_lamangler',''][bb.data.getVar('LIBTOOL_HAS_SYSROOT', d, 1) == "yes"]}"
autotools_prepackage_lamangler () {
        for i in `find ${PKGD} -name "*.la"` ; do \
            sed -i -e 's:${STAGING_LIBDIR}:${libdir}:g;' \
                   -e 's:${D}::g;' \
                   -e 's:-I${WORKDIR}\S*: :g;' \
                   -e 's:-L${WORKDIR}\S*: :g;' \
                   $i
    done
}
