set(COMPONENT_SRCS
    "main.c" "app_wifi.c"
    "app_camera.c"
    "app_httpd.c"
    "app_mdns.c"
    "app_http_post_jpeg.c")

set(COMPONENT_ADD_INCLUDEDIRS "include")

set(COMPONENT_REQUIRES
    esp32-camera
    nvs_flash
    esp_http_server
    mdns
    )

set(COMPONENT_EMBED_FILES
        "www/index_ov2640.html.gz"
        "www/index_ov3660.html.gz"
        "www/index_ov5640.html.gz"
        "www/monitor.html.gz")

register_component()
