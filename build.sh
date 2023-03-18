export ARCH=arm64
mkdir out

echo"automatically installing the necessary dependencies for successful compilation"

sudo apt-get update && apt-get install git ccache automake lzop bison gperf build-essential zip curl zlib1g-dev zlib1g-dev:i386 g++-multilib libxml2-utils bzip2 libbz2-dev libbz2-1.0 libghc-bzlib-dev squashfs-tools pngcrush schedtool dpkg-dev liblz4-tool make optipng clang gcc-aarch64-linux-gnu zip -y

echo"successful installation!.."

echo"now wait and see the magic"

BUILD_CROSS_COMPILE=aarch64-linux-gnu-
CLANG_TRIPLE=aarch64-linux-gnu-
KERNEL_MAKE_ENV="DTC_EXT=$(pwd)/tools/dtc CONFIG_BUILD_ARM64_DT_OVERLAY=y"

CLANG_VERSION=17
LLVM_BIN=/workspace/clang/bin/

make -j8 -C $(pwd) O=$(pwd)/out $KERNEL_MAKE_ENV ARCH=arm64 CROSS_COMPILE=$BUILD_CROSS_COMPILE \
	CLANG_DIR=$LLVM_BIN LLVM=1 LLVM_IAS=1 CLANG_TRIPLE=$CLANG_TRIPLE exynos9820-d2s_defconfig

make -j8 -C $(pwd) O=$(pwd)/out $KERNEL_MAKE_ENV ARCH=arm64 CROSS_COMPILE=$BUILD_CROSS_COMPILE LLVM=1 LLVM_IAS=1 \
	CLANG_DIR=$LLVM_BIN CLANG_TRIPLE=$CLANG_TRIPLE oldconfig

make -j8 -C $(pwd) O=$(pwd)/out $KERNEL_MAKE_ENV ARCH=arm64 CROSS_COMPILE=$BUILD_CROSS_COMPILE LLVM=1 LLVM_IAS=1 \
	CLANG_DIR=$LLVM_BIN CLANG_TRIPLE=$CLANG_TRIPLE

$(pwd)/tools/mkdtimg cfg_create $(pwd)/out/dtb.img dt.configs/exynos9820.cfg -d ${DTB_DIR}/exynos

IMAGE="out/arch/arm64/boot/Image"
if [[ -f "$IMAGE" ]]; then
	rm AnyKernel3/zImage > /dev/null 2>&1
	rm AnyKernel3/dtb > /dev/null 2>&1
	rm AnyKernel3/*.zip > /dev/null 2>&1
	mv out/dtb.img AnyKernel3/dtb
	mv $IMAGE AnyKernel3/zImage
	cd AnyKernel3
	zip -r9 Kernel-N975F-$(date +"%Y%m%d%H%M").zip .
fi
