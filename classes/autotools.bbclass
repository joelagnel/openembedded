require classes/autotools/staging.inc
require classes/autotools/bootstrap.inc
require classes/autotools/configure.inc

def autotools_deps(d):
    if bb.data.getVar('INHIBIT_AUTOTOOLS_DEPS', d, 1):
        return ''

    deps = 'gnu-config-native'
    if oe.data.typed_value('INHIBIT_AUTOTOOLS_BOOTSTRAP', d):
        return deps

    pn = bb.data.getVar('PN', d, 1)
    if pn in ['autoconf-native', 'automake-native', 'help2man-native']:
        return deps

    deps += ' autoconf-native automake-native help2man-native'

    if pn not in ['libtool', 'libtool-native', 'libtool-cross']:
        deps += ' libtool-native'
        if (not oe.utils.inherits(d, 'native', 'nativesdk', 'cross',
                                  'sdk') and
            not d.getVar('INHIBIT_DEFAULT_DEPS', True)):
            deps += ' libtool-cross'

    return deps

AUTOTOOLS_DEPENDS = "${@autotools_deps(d)}"

DEPENDS_prepend = "${AUTOTOOLS_DEPENDS} "
DEPENDS_virtclass-native_prepend = "${AUTOTOOLS_DEPENDS} "
DEPENDS_virtclass-nativesdk_prepend = "${AUTOTOOLS_DEPENDS} "

_INHIBITED = "${@base_ifelse(oe.data.typed_value('INHIBIT_AUTOTOOLS_BOOTSTRAP', \
                                                 d), \
                             'y', 'n')}"
autotools_do_configure () {
    if [ "${_INHIBITED}" = "n" ]; then
        autotools_do_bootstrap
    else
        find ${S} -name config.guess -exec \
            ln -sf ${STAGING_DATADIR_NATIVE}/gnu-config/config.guess "{}" \;
        find ${S} -name config.sub -exec \
            ln -sf ${STAGING_DATADIR_NATIVE}/gnu-config/config.sub "{}" \;
    fi

    if [ -e ${S}/configure ]; then
        oe_runconf $@
    else
        oenote "nothing to configure"
    fi
}

autotools_do_install() {
    oe_runmake 'DESTDIR=${D}' install
}

EXPORT_FUNCTIONS do_configure do_install
