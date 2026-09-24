#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef int i2c_port_t;
typedef void* i2c_cmd_handle_t;
typedef enum { I2C_MODE_MASTER = 1 } i2c_mode_t;
typedef enum { I2C_MASTER_WRITE = 0 } i2c_rw_t;

#define I2C_NUM_0 ((i2c_port_t)0)
#define I2C_MASTER_NUM I2C_NUM_0

typedef struct {
    i2c_mode_t mode;
    int sda_io_num;
    int scl_io_num;
    int sda_pullup_en;
    int scl_pullup_en;
    struct { uint32_t clk_speed; } master;
    uint32_t clk_flags;
} i2c_config_t;

inline int  i2c_param_config(i2c_port_t p, const i2c_config_t *c) { (void)p; (void)c; return 0; }
inline int  i2c_driver_install(i2c_port_t p, i2c_mode_t m, size_t rb, size_t tb, int f) { (void)p;(void)m;(void)rb;(void)tb;(void)f; return 0; }
inline i2c_cmd_handle_t i2c_cmd_link_create(void) { return NULL; }
inline int  i2c_master_start(i2c_cmd_handle_t h) { (void)h; return 0; }
inline int  i2c_master_write_byte(i2c_cmd_handle_t h, uint8_t b, bool ack) { (void)h;(void)b;(void)ack; return 0; }
inline int  i2c_master_write(i2c_cmd_handle_t h, const uint8_t *d, size_t s, bool ack) { (void)h;(void)d;(void)s;(void)ack; return 0; }
inline int  i2c_master_stop(i2c_cmd_handle_t h) { (void)h; return 0; }
inline int  i2c_master_cmd_begin(i2c_port_t p, i2c_cmd_handle_t h, uint32_t t) { (void)p;(void)h;(void)t; return 0; }
inline void i2c_cmd_link_delete(i2c_cmd_handle_t h) { (void)h; }
