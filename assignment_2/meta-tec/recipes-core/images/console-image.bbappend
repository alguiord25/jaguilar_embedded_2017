IMAGE_INSTALL += " rgb2yuv-c"
IMAGE_INSTALL += " rgb2yuv-intrinsics"
IMAGE_INSTALL += " rgb2yuv-neon"
IMAGE_INSTALL += " rgb2yuv-neon"
IMAGE_INSTALL += " conv-app"
IMAGE_INSTALL += " conv-mod"
MACHINE_EXTRA_RRECOMMENDS += "kernel-module-conv"
KERNEL_MODULE_AUTOLOAD += "conv"
