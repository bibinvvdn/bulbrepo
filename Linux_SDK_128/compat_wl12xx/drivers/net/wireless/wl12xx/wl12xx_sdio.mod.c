#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x48788ca, "module_layout" },
	{ 0xd66b7075, "kmalloc_caches" },
	{ 0x7420e123, "dev_set_drvdata" },
	{ 0x7d96c0e0, "sdio_writesb" },
	{ 0xee31e9be, "sdio_enable_func" },
	{ 0xba96320c, "wl12xx_get_platform_data" },
	{ 0xdeb9ae88, "sdio_get_host_pm_caps" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x197030d7, "dev_err" },
	{ 0x27e1a049, "printk" },
	{ 0x3f95a353, "platform_device_del" },
	{ 0xef0087fe, "platform_device_alloc" },
	{ 0x36efdf8a, "platform_device_add" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xce290743, "sdio_readsb" },
	{ 0x526b66b0, "sdio_unregister_driver" },
	{ 0x791b8e83, "platform_device_add_resources" },
	{ 0x3944f02, "sdio_f0_writeb" },
	{ 0x1d26df1c, "kmem_cache_alloc" },
	{ 0xcf769253, "sdio_f0_readb" },
	{ 0xea4dc9cc, "platform_device_add_data" },
	{ 0x8e0249a1, "mmc_power_save_host" },
	{ 0x7c25c626, "mmc_power_restore_host" },
	{ 0x856a7794, "sdio_memcpy_toio" },
	{ 0x37a0cba, "kfree" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x8ce1e9c1, "sdio_register_driver" },
	{ 0x290fd391, "sdio_memcpy_fromio" },
	{ 0xae09945a, "sdio_claim_host" },
	{ 0x5026c704, "dev_get_drvdata" },
	{ 0x924d3b66, "sdio_set_block_size" },
	{ 0xeeef37a4, "sdio_disable_func" },
	{ 0x9941730a, "sdio_release_host" },
	{ 0x5bf69cc2, "platform_device_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("sdio:c*v0097d4076*");
