deps_config := \
	/home/ami/esp/esp-idf/components/app_trace/Kconfig \
	/home/ami/esp/esp-idf/components/aws_iot/Kconfig \
	/home/ami/esp/esp-idf/components/bt/Kconfig \
	/home/ami/esp/esp-idf/components/driver/Kconfig \
	/home/ami/esp/esp-idf/components/esp32/Kconfig \
	/home/ami/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/ami/esp/esp-idf/components/esp_event/Kconfig \
	/home/ami/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/ami/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/ami/esp/esp-idf/components/ethernet/Kconfig \
	/home/ami/esp/esp-idf/components/fatfs/Kconfig \
	/home/ami/esp/esp-idf/components/freemodbus/Kconfig \
	/home/ami/esp/esp-idf/components/freertos/Kconfig \
	/home/ami/esp/esp-idf/components/heap/Kconfig \
	/home/ami/esp/esp-idf/components/libsodium/Kconfig \
	/home/ami/esp/esp-idf/components/log/Kconfig \
	/home/ami/esp/esp-idf/components/lwip/Kconfig \
	/home/ami/esp/esp-idf/components/mbedtls/Kconfig \
	/home/ami/esp/esp-idf/components/mdns/Kconfig \
	/home/ami/esp/esp-idf/components/mqtt/Kconfig \
	/home/ami/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/ami/esp/esp-idf/components/openssl/Kconfig \
	/home/ami/esp/esp-idf/components/pthread/Kconfig \
	/home/ami/esp/esp-idf/components/spi_flash/Kconfig \
	/home/ami/esp/esp-idf/components/spiffs/Kconfig \
	/home/ami/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/ami/esp/esp-idf/components/vfs/Kconfig \
	/home/ami/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/ami/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/ami/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/ami/repos/amiduino/esp/esp32/02_blink/main/Kconfig.projbuild \
	/home/ami/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/ami/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
