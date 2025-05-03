#include "tm1637.h"
#include "string.h"

static const uint8_t digit_to_segment[] = {
    0x3f, 0x06, 0x5b, 0x4f,
    0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f
};

static void tm1637_delay(void) {
    // You can replace this with a more accurate delay if needed
    for (volatile int i = 0; i < 50; i++);
}

void tm1637_delay_long(uint32_t how_long) {
    // Should be replaced with a more accurate delay
    for (volatile uint32_t i = 0; i < how_long; i++);
}

static void tm1637_start(void) {

	GPIOB -> ODR |= 0x0080;						// HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
	GPIOB -> ODR |= 0x0040;						// HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    tm1637_delay();
    GPIOB -> ODR &= 0xFF7F;						// HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
    tm1637_delay();
}

static void tm1637_stop(void) {
	GPIOB->ODR &= ~(1 << 6);   					// PB6 LOW (CLK)
	tm1637_delay();
	GPIOB->ODR &= ~(1 << 7);   					// PB7 LOW (DIO)
	tm1637_delay();
	GPIOB->ODR |=  (1 << 6);   					// PB6 HIGH (CLK)
	tm1637_delay();
	GPIOB->ODR |=  (1 << 7);   					// PB7 HIGH (DIO)
	tm1637_delay();
}

static void tm1637_write_byte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
    	GPIOB->ODR &= ~(1 << 6);   					// PB6 LOW (CLK) - HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    	if(b & 0x01){
    		GPIOB->ODR |=  (1 << 7);   				// PB7 HIGH (DIO)
    	}
    	else{
    		GPIOB->ODR &= ~(1 << 7);   				// PB7 LOW (DIO)
    	}
        tm1637_delay();
        GPIOB->ODR |=  (1 << 6);   					// PB6 HIGH (CLK) - HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
        tm1637_delay();
        b >>= 1;
    }
}

static uint8_t tm1637_get_ack(void) {
    uint8_t ack=0;
    GPIOB->ODR &= ~(1 << 6);					// HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    											// set_data_pin_input();
    tm1637_delay();
    											// ack = HAL_GPIO_ReadPin(DIO_PORT, DIO_PIN);
    GPIOB->ODR |=  (1 << 6);					// HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    tm1637_delay();
    GPIOB->ODR &= ~(1 << 6);					// HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    											// set_data_pin_output();
    return ack;
}

static void tm1637_write(uint8_t *data, uint8_t len) {
    tm1637_start();
    for (uint8_t i = 0; i < len; i++) {
        tm1637_write_byte(data[i]);
        tm1637_get_ack();						// data not read
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



