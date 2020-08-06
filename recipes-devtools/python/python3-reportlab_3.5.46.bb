DESCRIPTION = "The ReportLab Toolkit. An Open Source Python library for generating PDFs and graphics."
SECTION = "devel"
LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=b5239069dd30045258db1a185eb1d2dd"

SRC_URI = "https://pypi.python.org/packages/source/r/reportlab/reportlab-${PV}.tar.gz;downloadfilename=reportlab-{PV}.tar.gz \
           http://www.reportlab.com/ftp/fonts/pfbfer-20070710.zip;name=fonts;unpack=0 \
"

SRC_URI[md5sum] = "5c0e3a44e794547aed6f85149bd684f1"
SRC_URI[sha256sum] = "56d71b78e7e4bb31a93e1dff13c22d19b7fb3890b021a39b6c3661b095bd7de8"

SRC_URI[fonts.md5sum] = "35d20e26490cb2a8646fab6276ac6a4c"
SRC_URI[fonts.sha256sum] = "fb1462ff6ce8efae58a0c496f0537c5ac16d4895c68f1640c2973db574ddb376"

S = "${WORKDIR}/reportlab-${PV}"

inherit pypi setuptools3

DEPENDS = "python3-pillow freetype"

CFLAGS += "-fno-strict-aliasing -I${STAGING_INCDIR}/freetype2"
LDFLAGS += "-L${STAGING_LIBDIR}"

do_unpack_extra() {
	unzip -o -q ${DL_DIR}/pfbfer-20070710.zip -d ${S}/src/reportlab/fonts

	sed -i -e s:/usr/local/lib:${STAGING_LIBDIR}: \
               -e s:/usr/lib:${STAGING_LIBDIR}: \
               -e s:/usr/local/include:${STAGING_INCDIR}: \
               ${S}/setup.py

        sed -i -e s:/usr/local/Acrobat:/opt/Acrobat:g \
               ${S}/src/reportlab/rl_settings.py

        sed -i -e s:/usr/local:/usr: \
               ${S}/src/rl_addons/rl_accel/pyHnjmodule.c
}
addtask unpack_extra after do_unpack before do_patch

do_compile() {
    export STAGING_LIBDIR=${STAGING_LIBDIR}
    export STAGING_INCDIR=${STAGING_INCDIR}
    distutils3_do_compile
}
