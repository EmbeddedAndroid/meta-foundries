SUMMARY = "Smallest image that installs the meta-foundries product packages"
DESCRIPTION = "Smoke-test image used by meta-foundries CI to verify the \
layer builds and assembles cleanly against the chosen BSP. Not an \
OTA-capable factory image; see meta-foundries-overlay for that."
LICENSE = "MIT"

IMAGE_INSTALL = "packagegroup-core-boot packagegroup-foundries"
IMAGE_LINGUAS = ""

inherit core-image
