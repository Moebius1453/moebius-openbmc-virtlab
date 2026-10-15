SUMMARY = "Moebius writable filesystem statistics"
DESCRIPTION = "Report total and available space of the BMC writable filesystem"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit meson

SRC_URI = "file://main.cpp \
           file://meson.build \
           "

S = "${UNPACKDIR}"
