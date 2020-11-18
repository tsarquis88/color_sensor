#define CMD_RDY     0x90
#define CMD_RDX     0xD0

#define error       50.0

#define RL_min      200.0
#define RL_max      3800.0
#define RW_min      200.0
#define RW_max      3800.0

#define res_l       319.0
#define res_w       239.0

#define tp_cs_port 0
#define tp_cs_pin 27

unsigned long tpx = 0x0000;
unsigned long tpy = 0x0000;


void tp_init();
unsigned long TP_read();
void TP_read_coordinates(uint32_t *x_pos, uint32_t *y_pos);
uint32_t filter_data(uint32_t value, uint32_t axis);
void config_ts_exti();
void config_ts_pinsel();
