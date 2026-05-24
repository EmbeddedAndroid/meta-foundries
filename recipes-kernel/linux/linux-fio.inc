# meta-foundries kernel feature glue.
#
# Adds the kernel config fragments required by Foundries product
# code (aktualizr-lite + composectl need docker / composefs;
# modsign and ima are available but opt-in).
#
# To apply, add a one-line bbappend to your kernel recipe:
#
#     # linux-<bsp>_%.bbappend
#     require recipes-kernel/linux/linux-fio.inc
#
# Composable via FIO_KERNEL_FEATURES. Default = "docker composefs";
# overlays can opt in to "modsign" and "ima" once the device has
# the keying infrastructure for them.

FIO_KERNEL_FEATURES ??= "docker composefs"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
    ${@bb.utils.contains('FIO_KERNEL_FEATURES', 'docker',    'file://docker.cfg',    '', d)} \
    ${@bb.utils.contains('FIO_KERNEL_FEATURES', 'composefs', 'file://composefs.cfg', '', d)} \
    ${@bb.utils.contains('FIO_KERNEL_FEATURES', 'modsign',   'file://modsign.cfg',   '', d)} \
    ${@bb.utils.contains('FIO_KERNEL_FEATURES', 'ima',       'file://ima.cfg',       '', d)} \
"
