require pulseaudio.inc

PR = ${INC_PR}.2

DEPENDS += "gdbm speex"

DEFAULT_PREFERENCE_om-gta01 = "-1"
DEFAULT_PREFERENCE_om-gta02 = "-1"
DEFAULT_PREFERENCE_motorola-ezx = "-1"

FILESPATHPKG =. "pulseaudio-0.9.21:"

inherit gettext

inherit update-rc.d
INITSCRIPT_NAME = "pulseaudio.sh"
INITSCRIPT_PARAMS = "start 99 2 3 4 5 ."

#SRC_URI += "\
#  file://buildfix.patch \
#  file://autoconf_version.patch \
#  file://tls_m4.patch \
#  file://configure_silent_rules.patch \
#  file://armv4+v5asm.patch \
#"

SRC_URI += "\
  file://autoconf_version.patch \
  file://tls_m4.patch \
  file://configure_silent_rules.patch \
  file://armv4+v5asm.patch \
  file://fixbluezbuild.patch \
  file://ubacktrace.patch \
  file://pulseaudio-system-mode-startup.sh \
"

#do_compile_prepend() {
#    cd ${S}
#    mkdir -p ${S}/libltdl
#    cp ${STAGING_LIBDIR}/libltdl* ${S}/libltdl
#}

SRC_URI[md5sum] = "ca85ab470669b05e100861654cf5eb3c"
SRC_URI[sha256sum] = "c6019324395117a258c048a6db5e9734551cc2c61dc35b46403ff00d64be55f0"

do_install_append() {
  # init script to start pulseaudio system-wide
  install -d ${D}/${sysconfdir}/init.d/
  install -m 0755 ${WORKDIR}/pulseaudio-system-mode-startup.sh ${D}/${sysconfdir}/init.d/pulseaudio.sh

  # Disable startup of pulseaudio as a per-user instance
  chmod -x ${D}/${bindir}/start-pulseaudio-x11
}

