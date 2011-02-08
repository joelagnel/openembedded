# STAGE_TEMP_PREFIX is used for a speedup by packaged-staging
STAGE_TEMP="${WORKDIR}/temp-staging"
STAGE_TEMP_PREFIX = ""

autotools_stage_includes() {
    if [ "${INHIBIT_AUTO_STAGE_INCLUDES}" != "1" ]
    then
        rm -rf ${STAGE_TEMP}
        mkdir -p ${STAGE_TEMP}
        make DESTDIR="${STAGE_TEMP}" install
        cp -pPR ${STAGE_TEMP}/${includedir}/* ${STAGING_INCDIR}
        rm -rf ${STAGE_TEMP}
    fi
}

autotools_stage_dir() {
    sysroot_stage_dir $1 ${STAGE_TEMP_PREFIX}$2
}

autotools_stage_libdir() {
    sysroot_stage_libdir $1 ${STAGE_TEMP_PREFIX}$2
}

autotools_stage_all() {
    if [ "${INHIBIT_AUTO_STAGE}" = "1" ]
    then
        return
    fi
    rm -rf ${STAGE_TEMP}
    mkdir -p ${STAGE_TEMP}
    oe_runmake DESTDIR="${STAGE_TEMP}" install
    rm -rf ${STAGE_TEMP}/${mandir} || true
    rm -rf ${STAGE_TEMP}/${infodir} || true
    sysroot_stage_dirs ${STAGE_TEMP} ${STAGE_TEMP_PREFIX}
    rm -rf ${STAGE_TEMP}
}
