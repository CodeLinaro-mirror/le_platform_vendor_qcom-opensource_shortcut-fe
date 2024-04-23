load(":module_mgr.bzl", "create_module_registry")
SFE_PATH = ""

sfe_modules = create_module_registry([":sfe_src_headers"])

sfe_modules.register(
    name = "shortcut-fe",
    path = SFE_PATH,
    conditional_srcs = {
        "CONFIG_ENABLE_SFE": [
	    "sfe_ipv4.c",
        ],
    }
)

sfe_modules.register(
    name = "shortcut-fe-cm",
    path = SFE_PATH,
    conditional_srcs = {
        "CONFIG_ENABLE_SFE": [
            "sfe_cm.c",
       ],
    }
)

sfe_modules.register(
    name = "shortcut-fe-ipv6",
    path = SFE_PATH,
    conditional_srcs = {
        "CONFIG_SFE_SUPPORT_IPV6": [
            "sfe_ipv6.c",
       ],
    }
)


