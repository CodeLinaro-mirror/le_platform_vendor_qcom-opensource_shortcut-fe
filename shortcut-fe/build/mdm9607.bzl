load(":sfe_modules.bzl", "sfe_modules")
load(":module_mgr.bzl", "define_target_modules")

def define_mdm9607():
     define_target_modules(
        target = "mdm9607",
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
	    "CONFIG_ISKERNELUPGRADED",
	    "ISKERNEL6_1",
	    "ISKERNEL4_14",
        ]
    )
