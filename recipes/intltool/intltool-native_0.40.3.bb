require intltool_${PV}.bb

inherit native

EXTRA_OEMAKE = "'PERL_TARGET=/usr/bin/env perl'"

do_configure_prepend() {
	cd ${S}
	for i in intltool*.in ; do
		sed -i -e s:-w::g $i
	done
	cd -
}

SRC_URI[md5sum] = "7adee54938b22d32c63ecb5223fd4249"
SRC_URI[sha256sum] = "f30cbb62345310a27f74a43fef17f146d99203c097c6460002da0a22d969a594"
