acpaths = "default"
EXTRA_AUTORECONF = "--exclude=autopoint"

oe_autoreconf () {
    if [ x"${acpaths}" = xdefault ]; then
        acpaths=
        for i in `find ${S} -maxdepth 2 -name \*.m4|grep -v 'aclocal.m4'| \
            grep -v 'acinclude.m4' | sed -e 's,\(.*/\).*$,\1,'|sort -u`; do
            acpaths="$acpaths -I $i"
        done
    else
        acpaths="${acpaths}"
    fi
    AUTOV=`automake --version | head -n 1 | sed "s/.* //;s/\.[0-9]\+$//"`
    install -d ${STAGING_DATADIR}/aclocal
    install -d ${STAGING_DATADIR}/aclocal-$AUTOV
    acpaths="$acpaths -I${STAGING_DATADIR}/aclocal-$AUTOV -I ${STAGING_DATADIR}/aclocal"
    # autoreconf is too shy to overwrite aclocal.m4 if it doesn't look
    # like it was auto-generated.  Work around this by blowing it away
    # by hand, unless the package specifically asked not to run aclocal.
    if ! echo ${EXTRA_AUTORECONF} | grep -q "aclocal"; then
        rm -f aclocal.m4
    fi
    if [ -e configure.in ]; then
        CONFIGURE_AC=configure.in
    else
        CONFIGURE_AC=configure.ac
    fi
    if grep "^[[:space:]]*AM_GLIB_GNU_GETTEXT" $CONFIGURE_AC >/dev/null; then
        if grep "sed.*POTFILES" $CONFIGURE_AC >/dev/null; then
            : do nothing -- we still have an old unmodified configure.ac
        else
            echo "no" | glib-gettextize --force
        fi
    else if grep "^[[:space:]]*AM_GNU_GETTEXT" $CONFIGURE_AC >/dev/null; then
        if [ -e ${STAGING_DATADIR}/gettext/config.rpath ]; then
            ln -sf ${STAGING_DATADIR}/gettext/config.rpath ${S}/
        else
            oenote ${STAGING_DATADIR}/gettext/config.rpath not found. gettext is not installed.
        fi
    fi

    fi
    for aux in m4 `sed -n -e '/^[[:space:]]*AC_CONFIG_MACRO_DIR/s|[^(]*([[]*\([^])]*\)[]]*)|\1|p' $CONFIGURE_AC`; do
        mkdir -p ${aux}
    done
    autoreconf -Wcross --verbose --install --symlink --force ${EXTRA_AUTORECONF} $acpaths || oefatal "autoreconf execution failed."
    if grep "^[[:space:]]*[AI][CT]_PROG_INTLTOOL" $CONFIGURE_AC >/dev/null; then
        intltoolize --force --automake
    fi
}

autotools_do_bootstrap() {
    case ${PN} in
    autoconf*|automake*)
        ;;
    *)
        find ${S} -name configure.in -o -name configure.ac | \
            while read fn; do
                rm -f `dirname $fn`/configure
            done
        if [ -e ${S}/configure.in -o -e ${S}/configure.ac ]; then
            olddir=`pwd`
            cd ${S}
            oe_autoreconf
            cd $olddir
        fi
        ;;
    esac
}
