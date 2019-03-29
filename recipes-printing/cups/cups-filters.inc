DESCRIPTION = "CUPS backends, filters, and other software"
HOMEPAGE = "http://www.linuxfoundation.org/collaborate/workgroups/openprinting/cups-filters"

LICENSE = "GPLv2 & LGPLv2 & MIT & GPLv2+ & GPLv3 & BSD-4-clause"
LIC_FILES_CHKSUM = "file://COPYING;md5=079833adec8b92690ca302dd95507100"

SECTION = "console/utils"

DEPENDS = "cups glib-2.0 glib-2.0-native dbus dbus-glib lcms ghostscript poppler qpdf libpng"
DEPENDS_class-native = "poppler-native glib-2.0-native dbus-native pkgconfig-native gettext-native libpng-native"

SRC_URI = "http://openprinting.org/download/cups-filters/cups-filters-${PV}.tar.gz"

inherit autotools-brokensep gettext pkgconfig

EXTRA_OECONF_append = " --enable-ghostscript --disable-ijs --disable-ldap \
                       --with-pdftops=hybrid --enable-imagefilters \
                       --enable-ghostscript --with-gs-path=${bindir}/gs \
                       --with-pdftops-path=${bindir}/gs \
                       --with-fontdir=${datadir}/fonts --with-rcdir=no \
                       --with-cups-rundir=${localstatedir}/run/cups \
                       --localstatedir=${localstatedir}/var \
                       --with-rcdir=no \
                       --without-php"

EXTRA_OECONF_append_class-native = " --with-pdftops=pdftops \
                                    --disable-avahi --disable-ghostscript \
                                    --disable-ijs --disable-ldap \
                                    --with-png --without-jpeg --without-tiff"

EXTRA_OEMAKE = "-j1"

BBCLASSEXTEND = "native"

PACKAGECONFIG[jpeg] = "--with-jpeg,--without-jpeg,jpeg"
PACKAGECONFIG[png] = "--with-png,--without-png,libpng"
PACKAGECONFIG[tiff] = "--with-tiff,--without-tiff,tiff"

PACKAGECONFIG ??= "${@bb.utils.contains('DISTRO_FEATURES', 'zeroconf', 'avahi', '', d)}"

PACKAGECONFIG[avahi] = "--enable-avahi,--disable-avahi,avahi"
PACKAGECONFIG[dbus] = "--enable-dbus,--disable-dbus,dbus"

PACKAGES =+ "\
	${PN}-gst \
        ${PN}-data \
        "

FILES_${PN}-gst = "\
	${libdir}/cups/filter/gstopxl \
	${libdir}/cups/filter/gstoraster \
	"

FILES_${PN}-data = "\
	/usr/share/cups/data \
	"

FILES_${PN}-dbg += "\
	${libdir}/cups/filter/.debug \
	${libdir}/cups/backend/.debug \
	"

FILES_${PN} += "\
        ${libdir}/cups/filter \
        ${libdir}/cups/backend \
        ${libdir}/cups/driver \
        /usr/share/cups/charsets \
        /usr/share/cups/drv \
        /usr/share/cups/mime \
        /usr/share/cups/ppdc \
        /usr/share/ppd/cupsfilters \
        /usr/share/cups/braille \
        /usr/share/cups/banners \
        /usr/share/cups/braille/index.sh \
        /usr/share/cups/braille/cups-braille.sh \
        /usr/share/cups/braille/indexv3.sh \
        /usr/share/cups/braille/indexv4.sh \
        /usr/share/cups/banners/topsecret \
        /usr/share/cups/banners/secret \
        /usr/share/cups/banners/confidential \
        /usr/share/cups/banners/unclassified \
        /usr/share/cups/banners/form \
        /usr/share/cups/banners/classified \
        /usr/share/cups/banners/standard \
"

do_install_append() {
	# remove banners, braille dirs
	rm -rf ${D}/usr/share/cups/{banners,braille}

	# remove testprint (already provided by cups)
	rm -f ${D}/usr/share/cups/data/testprint

	# remove sysroot path contamination from pkgconfig file
	sed -i -e 's:${STAGING_DIR_TARGET}::' ${D}/${libdir}/pkgconfig/libcupsfilters.pc
}

RDEPENDS_${PN} += "bash"
