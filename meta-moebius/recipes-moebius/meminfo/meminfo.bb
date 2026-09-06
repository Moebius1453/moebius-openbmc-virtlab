SUMMARY = "Moebius memory statistics service"
DESCRIPTION = "Reads /proc/meminfo and exposes the values on D-Bus"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "sdbusplus ${PYTHON_PN}-sdbus++-native"

inherit meson pkgconfig
inherit obmc-phosphor-systemd

SRC_URI = "file://meson.build \
           file://main.cpp \
           file://meminfo.hpp \
           file://interfaces \
           file://gen \
           "

S = "${UNPACKDIR}"

SYSTEMD_SERVICE:${PN} = "meminfo.service"

