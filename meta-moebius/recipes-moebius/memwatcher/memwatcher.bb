SUMMARY = "Moebius memory watcher service"
DESCRIPTION = "Watches memory metrics over D-Bus and logs significant changes "
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "sdbusplus ${PYTHON_PN}"

inherit meson pkgconfig
inherit obmc-phosphor-systemd

SRC_URI = "file://meson.build \
           file://main.cpp \
           file://watcher.hpp \
           "

S = "${UNPACKDIR}"

SYSTEMD_SERVICE:${PN} = "memwatcher.service"