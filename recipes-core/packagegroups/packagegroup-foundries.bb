SUMMARY = "Foundries factory product packages"
DESCRIPTION = "Pulls in the Foundries tooling that defines a factory build: \
OTA agent, factory config, device registration, compose-app runtime, and \
supporting helpers. Any image inheriting feature-fio.inc gets this group."
LICENSE = "MIT"

inherit packagegroup

PACKAGES = "${PN}"

RDEPENDS:${PN} = " \
    aktualizr-lite \
    compose-apps-early-start \
    composectl \
    docker-cli-config \
    docker-compose \
    docker-credential-helper-fio \
    docker-moby \
    fio-diag \
    fio-docker-fsck \
    fioconfig \
    lmp-auto-hostname \
    lmp-device-auto-register \
    lmp-device-register \
    ostree-pending-reboot \
    wireguard-tools \
"
