/*GPIO*/
#include <drivers/gpio.h>
#include "gpio.h"

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

#define RED_LED_PIN 30
#define GREEN_LED_PIN 31

#define GPIO0 DT_GPIO_LABEL(GPIO0)

//GPIO setup
const struct device *dev; 
const struct device *gpio; 

void gpio_init() {
  dev = device_get_binding(LED0);
  if (dev == NULL) {
    printk("error in dev=device_get_binding\n");
    return;
  }
  int ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT | FLAGS);
  if (ret) {
    printk("Error configuring LED0 pin: %d\n", ret);
  }

  gpio = device_get_binding("GPIO_0");
  if (gpio == NULL) {
    printk("error getting GPIO_0 device\n");
    return;
  }
  ret = gpio_pin_configure(gpio, RED_LED_PIN, GPIO_OUTPUT);
  if(ret) {
    printk("Error configuring pin %d: %d\n", RED_LED_PIN, ret);
  }
  ret = gpio_pin_configure(gpio, GREEN_LED_PIN, GPIO_OUTPUT);
  if(ret) {
    printk("Error configuring pin %d: %d\n", GREEN_LED_PIN, ret);
  }
}

void gpio_set_red(bool on)
{
  if(on) {
      gpio_pin_set(dev, PIN, true);
      gpio_pin_set(gpio, RED_LED_PIN, true);
  } else {
      gpio_pin_set(dev, PIN, false);
      gpio_pin_set(gpio, RED_LED_PIN, false);
  }
}

void gpio_set_green(bool on)
{
  if(on) {
      gpio_pin_set(gpio, GREEN_LED_PIN, true);
  } else {
      gpio_pin_set(gpio, GREEN_LED_PIN, false);
  }
}
