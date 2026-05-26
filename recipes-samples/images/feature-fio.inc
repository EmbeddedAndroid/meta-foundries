# meta-foundries product feature include.
#
# require'd by any factory image to install the Foundries tooling.
# Mirrors the role lmp-feature-factory.inc played under meta-lmp.

CORE_IMAGE_BASE_INSTALL += "packagegroup-foundries"

# Dev convenience: empty-password root login over ssh and console.
# meta-qcom's ci/base.yml already sets the same on qcom BSPs, so this
# is a no-op there; it's the non-qcom factory builds (intel, uno-q
# variants targeting other BSPs, future qemuarm64 factory) that need
# it explicitly. Production builds for shipping hardware must override
# in meta-foundries-overlay or device-config to set a real password.
EXTRA_IMAGE_FEATURES += "allow-empty-password empty-root-password allow-root-login"
