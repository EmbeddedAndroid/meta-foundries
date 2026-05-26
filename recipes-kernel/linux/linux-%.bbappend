# Wire in the docker / composefs / wireguard kernel fragments that
# meta-foundries ships. linux-fio.inc declares the FIO_KERNEL_FEATURES
# knob and pulls the corresponding .cfg files into SRC_URI; absent
# this require line the fragments are dead weight.
require ${THISDIR}/linux-fio.inc

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://wireguard.cfg"
