cd yocto/
DISTRO=mod-rbz-xwayland MACHINE=mod_imx8m_plus source targetx-imx8-setup-release.sh -b build_targetx_imx8
bitbake imx-image-full
