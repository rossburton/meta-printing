SUMMARY = "An Internet printing system for Unix"
SECTION = "console/utils"
LICENSE = "GPLv2 & LGPLv2"
DEPENDS = "gnutls libgcrypt libpng jpeg dbus dbus-glib zlib libusb"

SRC_URI = "https://github.com/apple/cups/releases/download/release-${PV}/${BP}-source.tar.gz \
           file://use_echo_only_in_init.patch \
           file://0001-don-t-try-to-run-generated-binaries.patch \
           file://cups_serverbin.patch \
           file://${PN}.conf \
           file://${PN}.init \
	  "

CONFFILES_${PN} += "${sysconfdir}/cupsd.conf ${sysconfdir}/default/cups"

INITSCRIPT_NAME = "cupsd"
INITSCRIPT_PARAMS = "defaults 65"

UPSTREAM_CHECK_URI = "https://github.com/apple/cups/releases"
UPSTREAM_CHECK_REGEX = "cups-(?P<pver>\d+\.\d+(\.\d+)?)-source.tar"

LEAD_SONAME = "libcupsdriver.so"

CLEANBROKEN = "1"

inherit autotools-brokensep binconfig update-rc.d useradd systemd

USERADD_PACKAGES = "${PN}"
GROUPADD_PARAM_${PN} = "--system lpadmin"

SYSTEMD_SERVICE_${PN} = "org.cups.cupsd.socket org.cups.cupsd.path org.cups.cupsd.service org.cups.cups-lpd.socket org.cups.cups-lpd@.service"

PACKAGECONFIG ??= "${@bb.utils.contains('DISTRO_FEATURES', 'zeroconf', 'avahi', '', d)} \
                   ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)} \
                   ${@bb.utils.contains('DISTRO_FEATURES', 'pam', 'pam', '', d)}"
PACKAGECONFIG[avahi] = "--enable-avahi,--disable-avahi,avahi"
PACKAGECONFIG[acl] = "--enable-acl,--disable-acl,acl"
PACKAGECONFIG[pam] = "--enable-pam, --disable-pam, libpam"
PACKAGECONFIG[systemd] = "--with-systemd=${systemd_system_unitdir},--without-systemd,systemd"
PACKAGECONFIG[xinetd] = "--with-xinetd=${sysconfdir}/xinetd.d,--without-xinetd,xinetd"

EXTRA_OECONF = " \
               --with-docdir=${datadir}/cups/html \
               --disable-libpaper \
               --enable-gnutls \
               --enable-dbus \
               --enable-browsing \
               --disable-gssapi \
               --enable-debug \
               --disable-relro \
               --enable-libusb \
               --without-php \
               --without-perl \
               --without-python \
               --without-java \
               DSOFLAGS='${LDFLAGS}' \
               "

EXTRA_AUTORECONF += "--exclude=autoheader"

do_compile () {
	echo "all:"    >  man/Makefile
	echo "libs:" >> man/Makefile
	echo "install:" >> man/Makefile
	echo "install-data:" >> man/Makefile
	echo "install-exec:" >> man/Makefile
	echo "install-headers:" >> man/Makefile
	echo "install-libs:" >> man/Makefile

	oe_runmake
}

do_install () {
	oe_runmake "DSTROOT=${D}" install

	install -d ${D}${sysconfdir}/init.d ${D}${sysconfdir}/default
	install -m 0644 ${WORKDIR}/${PN}.conf ${D}${sysconfdir}/default/${PN}
	install -m 0755 ${WORKDIR}/${PN}.init ${D}${sysconfdir}/init.d/${PN}d

	# Remove /var/run from package as cupsd will populate it on startup
	rm -fr ${D}/${localstatedir}/run
	rmdir ${D}/${libdir}/${BPN}/driver

	# Fix the pam configuration file permissions
	if ${@bb.utils.contains('DISTRO_FEATURES', 'pam', 'true', 'false', d)}; then
	    chmod 0644 ${D}${sysconfdir}/pam.d/cups
	fi

	# Remove sysinit script and symlinks if sysvinit is not in DISTRO_FEATURES
	if ${@bb.utils.contains('DISTRO_FEATURES','sysvinit','false','true',d)}; then
	    rm -rf ${D}${sysconfdir}/init.d/
	    rm -rf ${D}${sysconfdir}/rc*
	fi
}

python do_package_append() {
    import subprocess
    # Change permissions back the way they were, they probably had a reason...
    workdir = d.getVar('WORKDIR', True)
    subprocess.call('chmod 0511 %s/install/cups/var/run/cups/certs' % workdir, shell=True)
}

PACKAGES =+ "${PN}-lib ${PN}-libimage"

RDEPENDS_${PN} += "${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'procps', '', d)}"
FILES_${PN} += "${libdir}/cups/backend \
		${libdir}/cups/cgi-bin \
		${libdir}/cups/filter \
		${libdir}/cups/monitor \
		${libdir}/cups/notifier \
		${libdir}/cups/daemon \
	       "

FILES_${PN}-lib = "${libdir}/libcups.so.*"

FILES_${PN}-libimage = "${libdir}/libcupsimage.so.*"

#package the html for the webgui inside the main packages (~1MB uncompressed)

FILES_${PN} += "${datadir}/cups/ \
                ${datadir}/icons/ \
               "

SYSROOT_PREPROCESS_FUNCS += "cups_sysroot_preprocess"
cups_sysroot_preprocess () {
	sed -i ${SYSROOT_DESTDIR}${bindir_crossscripts}/cups-config -e 's:cups_datadir=.*:cups_datadir=${datadir}/cups:' -e 's:cups_serverbin=.*:cups_serverbin=${libdir}/cups:'
}
