#define FRAME_HEAD  0x9b

void power_line_init(void);
void delay(volatile unsigned long  delay_time);   //delay(1000) is about 2.28ms at 16MHz
unsigned long read_ctrl_reg(void);
void write_ctrl_reg(unsigned long write_data);
void data_trans(unsigned long tx_data_h,unsigned long tx_data_l);
unsigned char data_recv(unsigned long *return_value_h,unsigned long *return_value_l,int try_times);
unsigned char frame_head_srch(int try_times);
unsigned char get_pin_rxd(void);
void set_pin_txd(unsigned char set_to_val);
void set_pin_reg_data(unsigned char set_to_val);
void set_pin_rxtx(unsigned char set_to_val);
void wait_posedge(void);
void wait_negedge(void);
unsigned char get_pin_cdpd(void);
unsigned char conti_0_check(int conti_0_num);

extern unsigned char curr_state;