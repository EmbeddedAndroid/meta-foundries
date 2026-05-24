# meta-foundries

Foundries.io product layer for embedded Linux builds.

This layer holds the Foundries product code (aktualizr-lite, composectl,
fioconfig, lmp-device-register, fio-docker-fsck, docker-credential-helper-fio,
and friends), together with the kernel-config fragments those tools require.
It is BSP-agnostic; pair it with `meta-qcom` + `meta-qcom-distro` for
Qualcomm boards, `meta-intel` for x86, and so on.

For per-factory wiring (factory image, kas entrypoint, factory-config.yml),
see the companion repository `meta-foundries-overlay`.

## Layer composition

```
       oe-core (+ meta-oe, meta-virt, meta-tpm, meta-security)
                  |
            meta-updater
                  |
           meta-foundries
                  |
      +-----------+--------------+
      |           |              |
  meta-qcom   meta-intel      ... other BSPs
      |
 meta-qcom-distro
      |
      +---- meta-foundries-overlay
```

## Layer compatibility

```
LAYERSERIES_COMPAT = "wrynose"
LAYERDEPENDS       = "core meta-updater"
```

## Branches

- `main`: development branch tracking wrynose.

## License

Apache-2.0; see `LICENSE`.
