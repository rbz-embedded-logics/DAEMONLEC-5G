mkdir yocto
cd yocto
repo init -u https://github.com/rbz-embedded-logics/rbz-manifest.git -b rbz-linux-nanbield -m imx-6.6.3-1.0.0.xml
repo sync

cp -R ../meta-rbz-targetx sources
cp sources/meta-rbz-targetx/sdk/targetx-imx8-setup-release.sh .
