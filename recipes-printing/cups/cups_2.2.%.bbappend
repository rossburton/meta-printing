FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = "file://${PN}.conf \
                  file://${PN}.init \
"

inherit pkgconfig update-rc.d

CONFFILES_${PN} += "${sysconfdir}/default/cups"

INITSCRIPT_NAME = "cupsd"
INITSCRIPT_PARAMS = "defaults 65"

INIT_DIR = "${D}${sysconfdir}/init.d"
CONF_DIR = "${D}${sysconfdir}/default"

DEPENDS += "libgcrypt"

DIRFILES = "1"

EXTRA_OECONF_append = " --with-docdir=${datadir}/cups/html \
                        --disable-libpaper \
"

do_install_append() {
    install -d ${INIT_DIR} ${CONF_DIR}
    install -m 0644 ${WORKDIR}/${PN}.conf ${CONF_DIR}/${PN}
    install -m 0755 ${WORKDIR}/${PN}.init ${INIT_DIR}/${PN}d
}

python do_package_append() {
    import subprocess
    # Change permissions back the way they were, they probably had a reason...
    workdir = d.getVar('WORKDIR', True)
    subprocess.call('chmod 0511 %s/install/cups/var/run/cups/certs' % workdir, shell=True)
}

FILES_${PN} += "${datadir}/cups/ \
                ${datadir}/icons/ \
               "
