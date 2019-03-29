DESCRIPTION = "PDF transformation/inspection software"
HOMEPAGE = "http://qpdf.sourceforge.net"
LICENSE = "Artistic-2.0"
SECTION = "libs"
DEPENDS = "libpcre zlib jpeg"

SRC_URI = "https://github.com/qpdf/qpdf/archive/release-qpdf-${PV}.tar.gz"

LIC_FILES_CHKSUM = "file://Artistic-2.0;md5=7806296b9fae874361e6fb10072b7ee3"
SRC_URI[md5sum] = "328db8214bc3a187ba912e310b14d498"
SRC_URI[sha256sum] = "71d9b6c77ac56521d86a481cb9e52c0774de106c1dcf7a0644a05bd1230be87e"

inherit autotools-brokensep gettext

# disable random file detection for cross-compile
EXTRA_OECONF = "--without-random \
                --disable-static \
                --disable-check-autofiles \
                "

EXTRA_OEMAKE_class-target = "LIBTOOL=${STAGING_BINDIR_CROSS}/${HOST_SYS}-libtool"

S="${WORKDIR}/${PN}-release-${PN}-${PV}"

# avoid Makefile returning error on 'make clean' before configure was run
CLEANBROKEN = "1"

DEBIAN_NOAUTONAME_libqpdf = "1"

PACKAGES =+ "libqpdf"
FILES_libqpdf = "${libdir}/libqpdf.so.*"

RDEPEND_${PN} = "libqpdf"
