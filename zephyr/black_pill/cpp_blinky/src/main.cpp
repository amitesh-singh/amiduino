/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <vector>

//since C++20

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   2000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */

class led {

constexpr static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
public:
  led()
  {
      if (!gpio_is_ready_dt(&red_led)) {
          printk("Failed to initialize");
      }
      auto ret = gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_ACTIVE);
      if (ret < 0) {
          printk("something wrong happened");
      }
  }
  void toggle()
  {
      gpio_pin_toggle_dt(&red_led);
  }
};

int main(void)
{
  led led;
	while (1) {
		led.toggle();
		printk("Hello world\n");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
