SUMMARY = "Moebius writable storage watcher"
DESCRIPTION = "Read StorageInfo metrics and log available space status changes"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "sdbusplus"

inherit meson pkgconfig
inherit obmc-phosphor-systemd

SRC_URI = "file://main.cpp \
           file://space_status.hpp \
           file://meson.build \
           "

S = "${UNPACKDIR}"

SYSTEMD_SERVICE:${PN} = "storagewatcher.service"
