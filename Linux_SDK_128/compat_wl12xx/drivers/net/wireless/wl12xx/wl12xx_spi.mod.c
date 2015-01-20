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
	{ 0x73ac286e, "spi_setup" },
	{ 0xa7587646, "crc7" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0x197030d7, "dev_err" },
	{ 0x3f95a353, "platform_device_del" },
	{ 0xef0087fe, "platform_device_alloc" },
	{ 0x4546076e, "driver_unregister" },
	{ 0x36efdf8a, "platform_device_add" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x39f73469, "spi_sync" },
	{ 0x791b8e83, "platform_device_add_resources" },
	{ 0x1d26df1c, "kmem_cache_alloc" },
	{ 0xea4dc9cc, "platform_device_add_data" },
	{ 0x37a0cba, "kfree" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xe5dffa09, "spi_register_driver" },
	{ 0x5026c704, "dev_get_drvdata" },
	{ 0x5bf69cc2, "platform_device_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

