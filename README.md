## ⚠️ This is not a duplicate of upstream ZMK. Do not delete it.

This repo (`thoughtfix/zmk`, branch `bbkeyboard_tp-fix9900`) is a fork of [ZitaoTech/zmk](https://github.com/ZitaoTech/zmk) (branch `bbkeyboard_tp`), which is itself the fork that adds BB9900/uConsole hardware support (trackpad driver, board definition) on top of core ZMK. It will look nearly identical to ZitaoTech's fork in a file browser, because it is, except for a small number of commits on top.

Those commits exist because two bugs in the [thoughtfix/fix9900](https://github.com/thoughtfix/fix9900) keyboard firmware needed real C code changes, not just a keymap or devicetree edit:

- Trackpad scroll mode: axis-locking, batched HID reports, and replacing a CapsLock/ScrollLock-indicator-triggered trigger with a deliberate click-to-toggle.
- A custom `vol_shift` behavior for the volume/mute keys, which needs to explicitly release and restore the Shift modifier bit around a press. No built-in ZMK behavior does this.

Full details are in [`config/NOTES.md`](https://github.com/thoughtfix/fix9900/blob/V1.2.1/config/NOTES.md) in the companion config repo.

**`fix9900`'s `config/west.yml` points directly at this repo and branch.** Deleting or renaming it breaks that fork's build, both local builds and its GitHub Actions CI fail immediately. If this repo is ever gone, `fix9900`'s build has to be repointed at a fresh fork with these commits reapplied. (This has happened once already.)

## From this part down is the original ZMK README

# Zephyr™ Mechanical Keyboard (ZMK) Firmware

[![Discord](https://img.shields.io/discord/719497620560543766)](https://zmk.dev/community/discord/invite)
[![Build](https://github.com/zmkfirmware/zmk/workflows/Build/badge.svg)](https://github.com/zmkfirmware/zmk/actions)
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-v2.0%20adopted-ff69b4.svg)](CODE_OF_CONDUCT.md)

[ZMK Firmware](https://zmk.dev/) is an open source ([MIT](LICENSE)) keyboard firmware built on the [Zephyr™ Project](https://www.zephyrproject.org/) Real Time Operating System (RTOS). ZMK's goal is to provide a modern, wireless, and powerful firmware free of licensing issues.

Check out the website to learn more: https://zmk.dev/.

You can also come join our [ZMK Discord Server](https://zmk.dev/community/discord/invite).

To review features, check out the [feature overview](https://zmk.dev/docs/). ZMK is under active development, and new features are listed with the [enhancement label](https://github.com/zmkfirmware/zmk/issues?q=is%3Aissue+is%3Aopen+label%3Aenhancement) in GitHub. Please feel free to add 👍 to the issue description of any requests to upvote the feature.
