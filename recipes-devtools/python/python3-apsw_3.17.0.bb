SUMMARY = "APSW stands for Another Python SQLite Wrapper."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=830db1ec625f8ee8996c4a07c6b8150c"

SRC_URI = "git://github.com/rogerbinns/apsw.git;protocol=https"
SRCREV = "26b61c39a98db3ccad7f852adc944b0b8e94c242"

inherit  setuptools3

S = "${WORKDIR}/git"
