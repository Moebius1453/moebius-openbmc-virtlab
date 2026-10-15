SUMMARY = "Moebius writable filesystem statistics"
DESCRIPTION = "Report total and available space of the BMC writable filesystem"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "sdbusplus phosphor-dbus-interfaces"

inherit meson pkgconfig
inherit obmc-phosphor-systemd

SRC_URI = "file://main.cpp \
           file://storageinfo.hpp \
           file://meson.build \
           "

S = "${UNPACKDIR}"

SYSTEMD_SERVICE:${PN} = "storageinfo.service"
