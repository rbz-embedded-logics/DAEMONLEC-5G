FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://0001-add-support-for-targetx-hw.patch \
            file://target-x.cfg \
"

DELTA_KERNEL_DEFCONFIG = "target-x.cfg"
