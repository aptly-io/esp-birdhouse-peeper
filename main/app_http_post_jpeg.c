/* HTTP GET Example using plain POSIX sockets

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_camera.h"
// #include "nvs_flash.h"
// #include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

// #include "capture.h"

#include "app_camera.h"

/* Constants that aren't configurable through menuconfig */

#define WEB_SERVER "192.168.40.41"
#define WEB_PORT "3334"

static const char *TAG = "http1-post";

static const char *REQUEST = "POST http://" WEB_SERVER ":" WEB_PORT "/ HTTP/1.0\r\n"
    "Host: "WEB_SERVER"\r\n"
    "User-Agent: esp-idf/4.0 esp32\r\n"
    "Content-Type: multipart/form-data; boundary=Boundary666\r\n"
    "Content-Length: %d\r\n"
    "\r\n";

static const char *PART_HEADER = "------Boundary666\r\n"
"Content-Disposition: form-data; name=\"file\"; filename=\"captureX.jpeg\"\r\n"
"Content-Type: image/jpeg\r\n"
"\r\n";

static const char *PART_FOOTER = "------Boundary666--";



static void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[64];


    while(1) {
        int err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &res);

        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        /* Code to print the resolved IP.

           Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        break;
    }

    while(1) {

        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");

        camera_fb_t *fb = NULL;
        esp_err_t esp_err = create_camera_image(&fb);
        if (ESP_OK != esp_err) {
            ESP_LOGE(TAG, "... create_camera_image failed: %d", esp_err);
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        size_t content_length = 0;
        content_length += strlen(PART_HEADER);
        content_length += fb->len;
        content_length += strlen(PART_FOOTER);

        ESP_LOGD(TAG, "PART_HEADER: %d, capture: %d, PART_FOOTER: %d, total: %d",
            strlen(PART_HEADER), fb->len, strlen(PART_FOOTER), content_length);

        static char buffer[4000];
        int buffer_len = snprintf(buffer, sizeof(buffer), REQUEST, content_length);
        if (buffer_len >= sizeof(buffer)) {
            ESP_LOGE(TAG, "... buffer too small!");
            close(s);
            free_camera_image(fb);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ssize_t write_len;
        if ((write_len = write(s, buffer, buffer_len)) < 0) {
            ESP_LOGE(TAG, "... socket send failed for request");
            close(s);
            free_camera_image(fb);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "%s", buffer);

        if ((write_len = write(s, PART_HEADER, strlen(PART_HEADER))) < 0) {
            ESP_LOGE(TAG, "... socket send failed for part-header: %d", write_len);
            close(s);
            free_camera_image(fb);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "%s", PART_HEADER);

        if ((write_len = write(s, fb->buf, fb->len)) < 0) {
            ESP_LOGE(TAG, "... socket send failed for part-body: %d", write_len);
            close(s);
            free_camera_image(fb);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "PART_BODY: %u bytes", fb->len);
        free_camera_image(fb);
        fb = NULL;

        if ((write_len = write(s, PART_FOOTER, strlen(PART_FOOTER))) < 0) {
            ESP_LOGE(TAG, "... socket send failed for part-footer: %d", write_len);
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "%s", PART_FOOTER);

        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        /* Read HTTP response */
        do {
            bzero(recv_buf, sizeof(recv_buf));
            r = read(s, recv_buf, sizeof(recv_buf)-1);
            for(int i = 0; i < r; i++) {
                putchar(recv_buf[i]);
            }
        } while(r > 0);

        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
        close(s);
        for(int countdown = 5; countdown >= 0; countdown--) {
            ESP_LOGI(TAG, "%d... ", countdown);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Starting again!");
    }
}

void app_http1_post_jpeg()
{
    // /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
    //  * Read "Establishing Wi-Fi or Ethernet Connection" section in
    //  * examples/protocols/README.md for more information about this function.
    //  */
    // ESP_ERROR_CHECK(example_connect());

    xTaskCreate(&http_get_task, "http_get_task", 4096, NULL, 5, NULL);
}
