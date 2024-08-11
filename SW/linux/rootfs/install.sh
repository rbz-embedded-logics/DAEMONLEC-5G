if [ ! -d target/ ]
then
  mkdir target
fi

cp yocto/build_targetx_imx8/tmp/deploy/images/mod_imx8m_plus/boot.scr target/
cp yocto/build_targetx_imx8/tmp/deploy/images/mod_imx8m_plus/Image target/
cp yocto/build_targetx_imx8/tmp/deploy/images/mod_imx8m_plus/tee.bin target/
cp yocto/build_targetx_imx8/tmp/deploy/images/mod_imx8m_plus/rbz-carrier-basic-imx8m-plus.dtb target/
cp yocto/build_targetx_imx8/tmp/deploy/images/mod_imx8m_plus/imx-boot-mod_imx8m_plus-sd.bin-flash_evk target/imx-boot.bin
cp yocto/build_targetx_imx8/tmp/deploy/images/mod_imx8m_plus/imx-boot-mod_imx8m_plus-sd.bin-flash_evk_flexspi target/imx-boot-spi.bin
cp yocto/build_targetx_imx8/tmp/deploy/images/mod_imx8m_plus/imx-image-full-mod_imx8m_plus.rootfs.tar.zst target/
cp yocto/build_targetx_imx8/tmp/depoly/images/mod_imx8m_plus/imx-image-full-mod_imx8m_plus.rootfs.wic.zst target/
