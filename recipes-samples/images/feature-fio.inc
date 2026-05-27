# meta-foundries product feature include.
#
# require'd by any factory image to install the Foundries tooling.
# Mirrors the role lmp-feature-factory.inc played under meta-lmp.

CORE_IMAGE_BASE_INSTALL += "packagegroup-foundries"

# Dev login: fio:fio with sudo + docker, root locked. Mirrors the
# stock meta-lmp behaviour. Production images that ship to customer
# hardware must override (BBMASK this include, or override
# EXTRA_USERS_PARAMS) to use customer-managed credentials and/or a
# pubkey-only sshd.
IMAGE_INSTALL:append = " sudo"

inherit extrausers

EXTRA_USERS_PARAMS = " \
    useradd -m -s /bin/bash -p \$(openssl passwd -6 fio) fio; \
    usermod -aG sudo,docker,video,audio,plugdev,dialout fio; \
    usermod -L root; \
"

# oe-core's sudo recipe ships an upstream-default /etc/sudoers without
# the Debian/Ubuntu "%sudo ALL=" line. Adding fio to the sudo group on
# its own grants nothing. Drop a sudoers.d fragment so members of the
# sudo group can actually sudo (with password). Production images that
# need pubkey-only or NOPASSWD policy should override this snippet.
feature_fio_setup_sudoers () {
    install -d ${IMAGE_ROOTFS}${sysconfdir}/sudoers.d
    printf '%%sudo ALL=(ALL:ALL) ALL\n' > ${IMAGE_ROOTFS}${sysconfdir}/sudoers.d/sudo-group
    chmod 0440 ${IMAGE_ROOTFS}${sysconfdir}/sudoers.d/sudo-group
}

ROOTFS_POSTPROCESS_COMMAND += "feature_fio_setup_sudoers;"
