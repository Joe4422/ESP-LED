# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "anchor_manager.c" "animator.c" "colour.c" "config.c" "main.c" "region_manager.c" "shaders.c" "startup.c" "storage_manager.c" "strip_manager.c" "web_data.c" "webserver.c" "wifi_manager.c" "ws2812_control.c" )
set(COMPONENT_ADD_INCLUDEDIRS ". ../build/config")

register_component()
