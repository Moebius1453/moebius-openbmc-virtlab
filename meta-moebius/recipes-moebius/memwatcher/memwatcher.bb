SUMMARY = "Moebius memory watcher service"
DESCRIPTION = "Reads /proc/meminfo "
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "sdbusplus ${PYTHON_PN}-sdbus++-native"

inherit meson pkgconfig
inherit obmc-phosphor-systemd

SRC_URI = "file://meson.build \
           file://main.cpp \
           file://watcher.hpp \
           "

S = "${UNPACKDIR}"

SYSTEMD_SERVICE:${PN} = "memwatcher.service"