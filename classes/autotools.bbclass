require classes/autotools/staging.inc
require classes/autotools/bootstrap.inc
require classes/autotools/configure.inc

def autotools_deps(d):
    if bb.data.getVar('INHIBIT_AUTOTOOLS_DEPS', d, 1):
        return ''

    pn = bb.data.getVar('PN', d, 1)
    deps = ''

    if pn in ['autoconf-native', 'automake-native', 'help2man-native']:
        return deps

    deps += 'autoconf-native automake-native help2man-native '

    if pn not in ['libtool', 'libtool-native', 'libtool-cross']:
        deps += 'libtool-native '
        if (not oe.utils.inherits(d, 'native', 'nativesdk', 'cross',
                                  'sdk') and
            not d.getVar('INHIBIT_DEFAULT_DEPS', True)):
            deps += 'libtool-cross '

    return deps + 'gnu-config-native '

DEPENDS_prepend = "${@autotools_deps(d)}"
DEPENDS_virtclass-native_prepend = "${@autotools_deps(d)}"
DEPENDS_virtclass-nativesdk_prepend = "${@autotools_deps(d)}"

autotools_do_configure () {
    autotools_do_bootstrap

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
