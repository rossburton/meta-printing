SUMMARY = "A set of Python bindings for the libcups library from the CUPS project"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "git://github.com/OpenPrinting/pycups.git"
SRCREV = "7fc1e1f3caa4c66039539fb52b26b7b98de37fbb"

S = "${WORKDIR}/git"

inherit setuptools3

RDEPENDS_${PN} = "cups"
