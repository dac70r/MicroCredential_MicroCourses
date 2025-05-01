#include "tm1637.h"
#include "string.h"

#define CLOCK_DELAY_US 5

static GPIO_TypeDef* CLK_PORT;
static uint16_t CLK_PIN;
static GPIO_TypeDef* DIO_PORT;
static uint16_t DIO_PIN;

static const uint8_t digit_to_segment[] = {
    0x3f, 0x06, 0x5b, 0x4f,
    0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f
};

static void tm1637_delay(void) {
    // You can replace this with a more accurate delay if needed
    for (volatile int i = 0; i < 50; i++);
}

static void set_data_pin_input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DIO_PORT, &GPIO_InitStruct);
}

static void set_data_pin_output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DIO_PORT, &GPIO_InitStruct);
}

void tm1637_init(GPIO_TypeDef* clk_port, uint16_t clk_pin, GPIO_TypeDef* data_port, uint16_t data_pin) {

    CLK_PORT = clk_port;
    CLK_PIN = clk_pin;
    DIO_PORT = data_port;
    DIO_PIN = data_pin;
/*
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = CLK_PIN | DIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CLK_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET); */

	// Register Equivalent Setup of PB6 and PB7
	GPIOB -> MODER &= 0xFFFF0FFF;				// Configure PB6 and PB7 as output pins
	GPIOB -> MODER |= 0x00005000;				// Configure PB6 and PB7 as output pins
	GPIOB -> PUPDR |= 0x0000A000; 				// Pull down PB6 and PB7 (Optional)

    GPIOB->OTYPER &= ~(1 << 6);              // 0 = Push-pull, 1 = Open-drain
    GPIOB->OTYPER &= ~(1 << 7);              // 0 = Push-pull, 1 = Open-drain

	GPIOB->OSPEEDR &= ~0x0000F000;         // Low speed (optional)
	GPIOB -> ODR |= 0x00C0;						// Set PB6 and PB7 as HIGH */
}

static void tm1637_start(void) {
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    tm1637_delay();
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
    tm1637_delay();
}

static void tm1637_stop(void) {
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    tm1637_delay();
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
    tm1637_delay();
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    tm1637_delay();
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
    tm1637_delay();
}

static uint8_t tm1637_get_ack(void) {
    uint8_t ack;

    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    set_data_pin_input();
    tm1637_delay();
    ack = HAL_GPIO_ReadPin(DIO_PORT, DIO_PIN);
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    tm1637_delay();
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    set_data_pin_output();
    return ack;
}

static void tm1637_write_byte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, (b & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        tm1637_delay();
        HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
        tm1637_delay();
        b >>= 1;
    }
}

static void tm1637_write(uint8_t *data, uint8_t len) {
    tm1637_start();
    for (uint8_t i = 0; i < len; i++) {
        tm1637_write_byte(data[i]);
        tm1637_get_ack();
    }
    tm1637_stop();
}

void tm1637_set_brightness(uint8_t brightness) {
    uint8_t control;
    if (brightness == 0) {
        control = 0x80;
    } else {
        if (brightness > 8) brightness = 8;
        control = 0x88 | (brightness - 1);
    }
    tm1637_write(&control, 1);
}

void tm1637_show_digits(const char *str) {
    uint8_t data[6] = {0x00}; // 0xC0 command + 4 digits
    uint8_t command1 = 0x40; // Auto-increment mode
    uint8_t command2 = 0xC0; // Start at address 0

    tm1637_write(&command1, 1); // Set data write mode
    data[0] = command2;

    for (uint8_t i = 0, j = 1; i < 5; i++) {
        if (i == 2) {
            if (str[i] == ':') {
                data[2] |= 0x80;
            }
            else{
            	data[2] &= ~(0x80);
            }
        } else {
            uint8_t seg = 0;
            if (str[i] >= '0' && str[i] <= '9') {
                seg = digit_to_segment[str[i] - '0'];
            }
            data[j++] = seg;
        }
    }

    tm1637_write(data, 5);
}

void tm1637_show_segments(const uint8_t segments[4]) {
    uint8_t command1 = 0x40;  // Auto-increment mode
    uint8_t command2 = 0xC0;  // Start at address 0
    uint8_t data[5];

    // Write display mode
    tm1637_write(&command1, 1);

    // Prepare address + 4 bytes of data
    data[0] = command2;
    data[1] = segments[0];
    data[2] = segments[1];
    data[3] = segments[2];
    data[4] = segments[3];

    tm1637_write(data, 5);
}

void uint8_to_spaced_string(uint8_t value, char *output) {
	// Insert space between hundreds and tens
	uint8_t hundreds = value / 100;
	uint8_t tens = (value / 10) % 10;
	uint8_t ones = value % 10;

	output[0] = ' ';
	output[1] = '0' + hundreds;
	output[2] = ' ';
	output[3] = '0' + tens;
	output[4] = '0' + ones;
	output[5] = '\0';
}

