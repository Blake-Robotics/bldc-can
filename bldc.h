/* BLDC Library
 *
 * Functions to allow encoding/decoding of CAN commands
 * used on the VESC speed controller. The name of this
 * library reflects the naming convention of the
 * project 'bldc'.
 *
 */

#ifndef libbldc_H
#define libbldc_H

#include<linux/can.h>
#include <stdint.h>

// CAN Buffer encoding/decoding:
void bldc_buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index);
void bldc_buffer_append_uint16(uint8_t* buffer, uint16_t number, int32_t *index);
void bldc_buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);
void bldc_buffer_append_uint32(uint8_t* buffer, uint32_t number, int32_t *index);
void bldc_buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t *index);
void bldc_buffer_append_float32(uint8_t* buffer, float number, float scale, int32_t *index);

int16_t  bldc_buffer_get_int16(const uint8_t *buffer, int32_t *index);
uint16_t bldc_buffer_get_uint16(const uint8_t *buffer, int32_t *index);
int32_t  bldc_buffer_get_int32(const uint8_t *buffer, int32_t *index);
uint32_t bldc_buffer_get_uint32(const uint8_t *buffer, int32_t *index);
float    bldc_buffer_get_float16(const uint8_t *buffer, float scale, int32_t *index);
float    bldc_buffer_get_float32(const uint8_t *buffer, float scale, int32_t *index);

// CAN Packets:
typedef struct {
  int id;
  float erpm;
  float current_motor;
  float duty_now;
} bldc_status;

typedef struct {
  int id;
  float current_input;
  float voltage_input;
  uint32_t uptime;

} bldc_status2;

typedef struct {
  int id;
  float temperature_mos1;
  float temperature_motor;
  uint8_t fault_code;
} bldc_status3;

typedef enum {
  BLDC_PACKET_SET_DUTY = 0,
  BLDC_PACKET_SET_CURRENT,
  BLDC_PACKET_SET_CURRENT_BRAKE,
  BLDC_PACKET_SET_RPM,
  BLDC_PACKET_SET_POS,
  BLDC_PACKET_FILL_RX_BUFFER,
  BLDC_PACKET_FILL_RX_BUFFER_LONG,
  BLDC_PACKET_PROCESS_RX_BUFFER,
  BLDC_PACKET_PROCESS_SHORT_BUFFER,
  BLDC_PACKET_STATUS,
  BLDC_PACKET_STATUS2,
  BLDC_PACKET_STATUS3
} BLDC_PACKET_ID;

// Simple commands to set motor operation
void bldc_set_duty(struct can_frame *frame, int id, float duty);
void bldc_set_current(struct can_frame *frame, int id, float current);
void bldc_set_current_brake(struct can_frame *frame, int id, float current);
void bldc_set_erpm(struct can_frame *frame, int id, int32_t erpm);

// Returned status infomation
void bldc_get_status(struct can_frame *frame, bldc_status *status);
void bldc_get_status2(struct can_frame *frame, bldc_status2 *status);
void bldc_get_status3(struct can_frame *frame, bldc_status3 *status);

// Functions to write long commands to the VESC's buffer.
void bldc_process_cmd(struct can_frame **frames, int id, uint8_t* tx_buffer, uint8_t len);

// CRC function..
uint16_t crc16(uint8_t* buf, unsigned int len);
#endif //libbldc_H
