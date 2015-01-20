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
	{ 0x6d662533, "_find_first_bit_le" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0x54df2eeb, "skb_queue_head" },
	{ 0x5171e48, "device_remove_file" },
	{ 0xcdb32922, "device_remove_bin_file" },
	{ 0xd66b7075, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xf9a482f9, "msleep" },
	{ 0x9b388444, "get_zeroed_page" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x528c709d, "simple_read_from_buffer" },
	{ 0xfed4d591, "cfg80211_find_vendor_ie" },
	{ 0xa20c7b74, "generic_file_llseek" },
	{ 0xf7ba0723, "debugfs_create_dir" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x3ec8886f, "param_ops_int" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xfdc16927, "ieee80211_sta_ps_transition" },
	{ 0xd8f795ca, "del_timer" },
	{ 0x753556f8, "ieee80211_queue_work" },
	{ 0x7420e123, "dev_set_drvdata" },
	{ 0x7627f4b, "__alloc_workqueue_key" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0x9c64fbd, "ieee80211_frequency_to_channel" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x1ba20013, "ieee80211_req_channel_switch" },
	{ 0xd3dbfbc4, "_find_first_zero_bit_le" },
	{ 0xac9531ad, "cfg80211_testmode_alloc_reply_skb" },
	{ 0x4bdf09d6, "ieee80211_cqm_rssi_notify" },
	{ 0x52760ca9, "getnstimeofday" },
	{ 0xb54533f7, "usecs_to_jiffies" },
	{ 0x7ef39823, "ieee80211_hdrlen" },
	{ 0xbef74651, "ieee80211_beacon_get_tim" },
	{ 0xd3f57a2, "_find_next_bit_le" },
	{ 0x2a3aa678, "_test_and_clear_bit" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0x4205ad24, "cancel_work_sync" },
	{ 0x8e5007c1, "queue_work" },
	{ 0xe2fae716, "kmemdup" },
	{ 0xc4abd5ee, "ieee80211_unregister_hw" },
	{ 0x7513e94e, "ieee80211_channel_to_frequency" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xba42f7d7, "ieee80211_started_vifs_count" },
	{ 0xb78c61e8, "param_ops_bool" },
	{ 0x74c86cc0, "init_timer_key" },
	{ 0xb77a7c47, "prepare_to_wait_exclusive" },
	{ 0xe1d61c3a, "cancel_delayed_work_sync" },
	{ 0x62b72b0d, "mutex_unlock" },
	{ 0xaed6cf6, "ieee80211_scan_completed" },
	{ 0x4624ce42, "ieee80211_sched_scan_results" },
	{ 0x999e8297, "vfree" },
	{ 0xddc015c1, "debugfs_create_file" },
	{ 0x7fe1a403, "cfg80211_find_ie" },
	{ 0x16be5182, "debugfs_remove_recursive" },
	{ 0x7d11c268, "jiffies" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0x8e4084d2, "skb_trim" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xce2840e7, "irq_set_irq_wake" },
	{ 0x826fc8ee, "ieee80211_stop_queue" },
	{ 0x97d620d9, "ieee80211_tx_status" },
	{ 0xcf8609cd, "ieee80211_get_operstate" },
	{ 0xf6288e02, "__init_waitqueue_head" },
	{ 0x72aa82c6, "param_ops_charp" },
	{ 0x69b18f43, "rfc1042_header" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0xc593e00b, "__ieee80211_get_channel" },
	{ 0xe5ab387a, "default_llseek" },
	{ 0xe28c2f5a, "dev_alloc_skb" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0x48a0f939, "mutex_lock_interruptible" },
	{ 0xdc798d37, "__mutex_init" },
	{ 0x27e1a049, "printk" },
	{ 0x42224298, "sscanf" },
	{ 0x18916955, "ieee80211_free_txskb" },
	{ 0x71c90087, "memcmp" },
	{ 0xcd8daac9, "ieee80211_report_low_ack" },
	{ 0x328a05f1, "strncpy" },
	{ 0x3a1bf9ba, "nla_put" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xf1425c8b, "ieee80211_rx" },
	{ 0x246852a2, "skb_push" },
	{ 0xe16b893b, "mutex_lock" },
	{ 0x3cc85622, "destroy_workqueue" },
	{ 0x9a6221c5, "mod_timer" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0xc42c569, "platform_driver_register" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0x1a4915e, "skb_pull" },
	{ 0x81ab9afc, "ieee80211_proberesp_get" },
	{ 0xb221fe23, "ieee80211_queue_delayed_work" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0x8331ef0f, "device_create_file" },
	{ 0x4059792f, "print_hex_dump" },
	{ 0x6f59c525, "skb_queue_tail" },
	{ 0x3ff62317, "local_bh_disable" },
	{ 0x973d0f9e, "kstrtoul_from_user" },
	{ 0xa41509a5, "ieee80211_ap_ch_switch_done" },
	{ 0x1d26df1c, "kmem_cache_alloc" },
	{ 0xb2d7fd35, "__alloc_skb" },
	{ 0xaa739a1f, "ieee80211_roaming_status" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x4f391d0e, "nla_parse" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x1000e51, "schedule" },
	{ 0x20b6ada0, "kfree_skb" },
	{ 0x799aca4, "local_bh_enable" },
	{ 0x996bdb64, "_kstrtoul" },
	{ 0xe579cee1, "ieee80211_find_sta" },
	{ 0xc27487dd, "__bug" },
	{ 0x258ddbe0, "device_create_bin_file" },
	{ 0xe68a8fc1, "ieee80211_wake_queue" },
	{ 0xb98b0324, "ieee80211_stop_rx_ba_session" },
	{ 0x4302d0eb, "free_pages" },
	{ 0xb9e52429, "__wake_up" },
	{ 0x611b263f, "ieee80211_get_hdrlen_from_skb" },
	{ 0x16fe4a4b, "ieee80211_restart_hw" },
	{ 0xbd814f38, "ieee80211_register_hw" },
	{ 0x8d66a3a, "warn_slowpath_fmt" },
	{ 0x9aae29fa, "ieee80211_pspoll_get" },
	{ 0xfcec0987, "enable_irq" },
	{ 0xc9b72460, "ieee80211_nullfunc_get" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0xf2a2d855, "ieee80211_alloc_hw" },
	{ 0x6d3bdb4c, "ieee80211_ap_probereq_get" },
	{ 0x374ed073, "scnprintf" },
	{ 0x5e2e152, "request_firmware" },
	{ 0x7f9c45fa, "ieee80211_probereq_get" },
	{ 0x99d7cc87, "ieee80211_free_hw" },
	{ 0x8893fa5d, "finish_wait" },
	{ 0xb75b90f6, "skb_dequeue" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x6e95268e, "cfg80211_testmode_reply" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0x60f71cfa, "complete" },
	{ 0x701d0ebd, "snprintf" },
	{ 0x85067a89, "platform_get_irq" },
	{ 0xca54fee, "_test_and_set_bit" },
	{ 0x99bb8806, "memmove" },
	{ 0xde88b610, "consume_skb" },
	{ 0x87181b02, "platform_driver_unregister" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0x49ebacbd, "_clear_bit" },
	{ 0xdca3ba5a, "skb_put" },
	{ 0xd6be9ec6, "ieee80211_chswitch_done" },
	{ 0x80d817ab, "wait_for_completion_timeout" },
	{ 0xc3fe87c8, "param_ops_uint" },
	{ 0x62c6962f, "ieee80211_sched_scan_stopped" },
	{ 0xa170bbdb, "outer_cache" },
	{ 0x5026c704, "dev_get_drvdata" },
	{ 0x6319c0bb, "release_firmware" },
	{ 0x54e91e06, "ieee80211_connection_loss" },
	{ 0x81799cee, "vscnprintf" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=cfg80211,mac80211";

MODULE_ALIAS("platform:wl12xx");
