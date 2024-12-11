load(":sfe_modules.bzl", "sfe_modules")
load(":module_mgr.bzl", "define_target_modules")

def define_modules():
     define_target_modules(
        target = "sdxkova",
        variants = ["debug-defconfig", "perf-defconfig"],
        registry = sfe_modules,
        modules = [
            "shortcut-fe",
	    "shortcut-fe-ipv6",
	    "shortcut-fe-cm",
        ],
        config_options = [
            "CONFIG_ENABLE_SFE",
            "CONFIG_SFE_SUPPORT_IPV6",
	    "ISKERNELUPGRADED",
	    "ISKERNEL6_6",
	    "ISKERNEL5_15",
	    "ISKERNEL4_14",
	    "ISKERNEL5_4",
        ]
    )

