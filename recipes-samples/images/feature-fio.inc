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
