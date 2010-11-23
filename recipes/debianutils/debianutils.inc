DESCRIPTION = "Miscellaneous utilities specific to Debian"
SECTION = "base"
LICENSE = "GPL BSD BSD SMAIL"
INC_PR = "r2"

SRC_URI = "${DEBIAN_MIRROR}/main/d/${PN}/${PN}_${PV}.tar.gz"

inherit autotools

sbinfiles = "add-shell installkernel mkboot remove-shell"
binfiles = "savelog sensible-browser sensible-editor sensible-pager which"
basebinfiles = "run-parts tempfile"

do_install_append () {
    # Debian places some utils into ${base_bindir} as does busybox
    if [ "${bindir}" != "${base_bindir}" ]; then
        install -d ${D}${base_bindir}
        for app in $basebinfiles; do
            mv ${D}${bindir}/$app ${D}${base_bindir}/$app
        done
    fi
}

do_install_append_pn-debianutils () {
    for app in ${sbinfiles}; do
        mv ${D}${sbindir}/$app ${D}${sbindir}/$app.${PN}
    done
    for app in ${binfiles}; do
        mv ${D}${bindir}/$app ${D}${bindir}/$app.${PN}
    done
    for app in ${basebinfiles}; do
        mv ${D}${base_bindir}/$app ${D}${base_bindir}/$app.${PN}
    done
}

pkg_prerm_${PN} () {
for app in ${sbinfiles} ${binfiles} ${basebinfiles}; do
   update-alternatives --remove $app $app.${PN}
done
}
pkg_postinst_${PN} () {
for app in ${sbinfiles}; do
    update-alternatives --install ${sbindir}/$app $app $app.${PN} 100
done
for app in ${binfiles}; do
    update-alternatives --install ${bindir}/$app $app $app.${PN} 100
done
for app in ${basebinfiles}; do
    update-alternatives --install ${base_bindir}/$app $app $app.${PN} 100
done
}
