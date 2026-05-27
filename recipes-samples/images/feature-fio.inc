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

# Persistent home directory wiring for the fio user.
#
# meta-updater's image_types_ostree.bbclass converts /home into a symlink
# to var/rootdirs/home so user data persists across OSTree deployments,
# but it only tmpfiles-d-creates /var/rootdirs/ itself -- not
# /var/rootdirs/home/. Combined with extrausers' useradd -m running at
# image build time (when /var/ has nothing under rootdirs yet), the
# fio user ends up with no homedir on the deployed device:
#
#   $ ssh fio@<device>
#   Could not chdir to home directory /home/fio: No such file or directory
#   $ ls /var/rootdirs/home/
#   (empty)
#
# meta-lmp's lmp.bbclass papers over the same problem for the lmp
# distro by appending a tmpfiles.d entry for /var/rootdirs/home and
# by rewriting /etc/passwd to use the absolute /var/rootdirs/home
# path so the lookup doesn't depend on the symlink. Mirror that here
# so any DISTRO inheriting meta-updater (notably qcom-distro-sota)
# behaves the same as lmp. Additionally pre-seed fio's homedir +
# skel files so the first login finds a populated $HOME without
# relying on pam_mkhomedir.
feature_fio_setup_homedir () {
    install -d ${IMAGE_ROOTFS}${nonarch_libdir}/tmpfiles.d
    cat >> ${IMAGE_ROOTFS}${nonarch_libdir}/tmpfiles.d/00fio-home.conf <<EOF
d /var/rootdirs/home 0755 root root -
d /var/rootdirs/home/fio 0755 fio fio -
EOF

    if [ -f ${IMAGE_ROOTFS}/etc/passwd ]; then
        sed -i -e 's,:/home/fio:,:/var/rootdirs/home/fio:,g' ${IMAGE_ROOTFS}/etc/passwd
    fi
    if [ -f ${IMAGE_ROOTFS}/usr/lib/passwd ]; then
        sed -i -e 's,:/home/fio:,:/var/rootdirs/home/fio:,g' ${IMAGE_ROOTFS}/usr/lib/passwd
    fi

    FIOUID=$(awk -F: '/^fio:/{print $3}' ${IMAGE_ROOTFS}/etc/passwd 2>/dev/null)
    FIOGID=$(awk -F: '/^fio:/{print $4}' ${IMAGE_ROOTFS}/etc/passwd 2>/dev/null)
    if [ -n "$FIOUID" ] && [ -n "$FIOGID" ]; then
        install -d ${IMAGE_ROOTFS}/var/rootdirs/home/fio
        if [ -d ${IMAGE_ROOTFS}/etc/skel ]; then
            cp -a ${IMAGE_ROOTFS}/etc/skel/. ${IMAGE_ROOTFS}/var/rootdirs/home/fio/ || true
        fi
        chown -R $FIOUID:$FIOGID ${IMAGE_ROOTFS}/var/rootdirs/home/fio
        chmod 0755 ${IMAGE_ROOTFS}/var/rootdirs/home/fio
    fi
}

ROOTFS_POSTPROCESS_COMMAND += "feature_fio_setup_homedir;"
