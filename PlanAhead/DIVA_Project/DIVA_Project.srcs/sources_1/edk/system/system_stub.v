//-----------------------------------------------------------------------------
// system_stub.v
//-----------------------------------------------------------------------------

module system_stub
  (
    fpga_0_RS232_Uart_1_RX_pin,
    fpga_0_RS232_Uart_1_TX_pin,
    fpga_0_RS232_Uart_2_RX_pin,
    fpga_0_RS232_Uart_2_TX_pin,
    fpga_0_LEDs_8Bit_GPIO_IO_pin,
    fpga_0_LEDs_Positions_GPIO_IO_pin,
    fpga_0_Push_Buttons_5Bit_GPIO_IO_pin,
    fpga_0_DIP_Switches_8Bit_GPIO_IO_pin,
    fpga_0_IIC_EEPROM_Sda_pin,
    fpga_0_IIC_EEPROM_Scl_pin,
    fpga_0_SRAM_Mem_A_pin,
    fpga_0_SRAM_Mem_CEN_pin,
    fpga_0_SRAM_Mem_OEN_pin,
    fpga_0_SRAM_Mem_WEN_pin,
    fpga_0_SRAM_Mem_BEN_pin,
    fpga_0_SRAM_Mem_ADV_LDN_pin,
    fpga_0_SRAM_Mem_DQ_pin,
    fpga_0_SRAM_ZBT_CLK_OUT_pin,
    fpga_0_SRAM_ZBT_CLK_FB_pin,
    fpga_0_PCIe_Bridge_RXN_pin,
    fpga_0_PCIe_Bridge_RXP_pin,
    fpga_0_PCIe_Bridge_TXN_pin,
    fpga_0_PCIe_Bridge_TXP_pin,
    fpga_0_DDR2_SDRAM_DDR2_Clk_pin,
    fpga_0_DDR2_SDRAM_DDR2_Clk_n_pin,
    fpga_0_DDR2_SDRAM_DDR2_CE_pin,
    fpga_0_DDR2_SDRAM_DDR2_CS_n_pin,
    fpga_0_DDR2_SDRAM_DDR2_ODT_pin,
    fpga_0_DDR2_SDRAM_DDR2_RAS_n_pin,
    fpga_0_DDR2_SDRAM_DDR2_CAS_n_pin,
    fpga_0_DDR2_SDRAM_DDR2_WE_n_pin,
    fpga_0_DDR2_SDRAM_DDR2_BankAddr_pin,
    fpga_0_DDR2_SDRAM_DDR2_Addr_pin,
    fpga_0_DDR2_SDRAM_DDR2_DQ_pin,
    fpga_0_DDR2_SDRAM_DDR2_DM_pin,
    fpga_0_DDR2_SDRAM_DDR2_DQS_pin,
    fpga_0_DDR2_SDRAM_DDR2_DQS_n_pin,
    fpga_0_clk_1_sys_clk_pin,
    fpga_0_rst_1_sys_rst_pin,
    fpga_0_PCIe_Diff_Clk_IBUF_DS_P_pin,
    fpga_0_PCIe_Diff_Clk_IBUF_DS_N_pin,
    LCD_1602_4BIT_INTERFACE_GPIO_IO,
    CTRL_BUTTONs_16Bit_GPIO_IO_pin,
    CTRL_STICK_L_X_Dac_Out_pin,
    CTRL_STICK_L_Y_Dac_Out_pin,
    CTRL_STICK_R_X_Dac_Out_pin,
    CTRL_STICK_R_Y_Dac_Out_pin,
    CTRL_TRIGGER_LT_Dac_Out_pin,
    CTRL_TRIGGER_RT_Dac_Out_pin
  );
  input fpga_0_RS232_Uart_1_RX_pin;
  output fpga_0_RS232_Uart_1_TX_pin;
  input fpga_0_RS232_Uart_2_RX_pin;
  output fpga_0_RS232_Uart_2_TX_pin;
  inout [0:7] fpga_0_LEDs_8Bit_GPIO_IO_pin;
  inout [0:4] fpga_0_LEDs_Positions_GPIO_IO_pin;
  inout [0:4] fpga_0_Push_Buttons_5Bit_GPIO_IO_pin;
  inout [0:7] fpga_0_DIP_Switches_8Bit_GPIO_IO_pin;
  inout fpga_0_IIC_EEPROM_Sda_pin;
  inout fpga_0_IIC_EEPROM_Scl_pin;
  output [7:30] fpga_0_SRAM_Mem_A_pin;
  output fpga_0_SRAM_Mem_CEN_pin;
  output fpga_0_SRAM_Mem_OEN_pin;
  output fpga_0_SRAM_Mem_WEN_pin;
  output [0:3] fpga_0_SRAM_Mem_BEN_pin;
  output fpga_0_SRAM_Mem_ADV_LDN_pin;
  inout [0:31] fpga_0_SRAM_Mem_DQ_pin;
  output fpga_0_SRAM_ZBT_CLK_OUT_pin;
  input fpga_0_SRAM_ZBT_CLK_FB_pin;
  input fpga_0_PCIe_Bridge_RXN_pin;
  input fpga_0_PCIe_Bridge_RXP_pin;
  output fpga_0_PCIe_Bridge_TXN_pin;
  output fpga_0_PCIe_Bridge_TXP_pin;
  output [1:0] fpga_0_DDR2_SDRAM_DDR2_Clk_pin;
  output [1:0] fpga_0_DDR2_SDRAM_DDR2_Clk_n_pin;
  output [1:0] fpga_0_DDR2_SDRAM_DDR2_CE_pin;
  output [1:0] fpga_0_DDR2_SDRAM_DDR2_CS_n_pin;
  output [1:0] fpga_0_DDR2_SDRAM_DDR2_ODT_pin;
  output fpga_0_DDR2_SDRAM_DDR2_RAS_n_pin;
  output fpga_0_DDR2_SDRAM_DDR2_CAS_n_pin;
  output fpga_0_DDR2_SDRAM_DDR2_WE_n_pin;
  output [1:0] fpga_0_DDR2_SDRAM_DDR2_BankAddr_pin;
  output [12:0] fpga_0_DDR2_SDRAM_DDR2_Addr_pin;
  inout [63:0] fpga_0_DDR2_SDRAM_DDR2_DQ_pin;
  output [7:0] fpga_0_DDR2_SDRAM_DDR2_DM_pin;
  inout [7:0] fpga_0_DDR2_SDRAM_DDR2_DQS_pin;
  inout [7:0] fpga_0_DDR2_SDRAM_DDR2_DQS_n_pin;
  input fpga_0_clk_1_sys_clk_pin;
  input fpga_0_rst_1_sys_rst_pin;
  input fpga_0_PCIe_Diff_Clk_IBUF_DS_P_pin;
  input fpga_0_PCIe_Diff_Clk_IBUF_DS_N_pin;
  inout [0:6] LCD_1602_4BIT_INTERFACE_GPIO_IO;
  inout [0:15] CTRL_BUTTONs_16Bit_GPIO_IO_pin;
  output CTRL_STICK_L_X_Dac_Out_pin;
  output CTRL_STICK_L_Y_Dac_Out_pin;
  output CTRL_STICK_R_X_Dac_Out_pin;
  output CTRL_STICK_R_Y_Dac_Out_pin;
  output CTRL_TRIGGER_LT_Dac_Out_pin;
  output CTRL_TRIGGER_RT_Dac_Out_pin;

  wire vcc, gnd;
  assign vcc = 1'b1;
  assign gnd = 1'b0;

  wire fpga_0_mb_plb_clk;
  wire [0:5] CTRL_DAC_READ_EN_GPIO_IO_O_pin;
  
  wire CTRL_STICK_L_X_Dac_clk_en_pin;
  wire CTRL_STICK_L_Y_Dac_clk_en_pin;
  wire CTRL_STICK_R_X_Dac_clk_en_pin;
  wire CTRL_STICK_R_Y_Dac_clk_en_pin;
  wire CTRL_TRIGGER_LT_Dac_clk_en_pin;
  wire CTRL_TRIGGER_RT_Dac_clk_en_pin;

  wire CTRL_STICK_L_X_Read_en_pin;
  wire CTRL_STICK_L_Y_Read_en_pin;
  wire CTRL_STICK_R_X_Read_en_pin;
  wire CTRL_STICK_R_Y_Read_en_pin;
  wire CTRL_TRIGGER_LT_Read_en_pin;
  wire CTRL_TRIGGER_RT_Read_en_pin;
  
  assign CTRL_STICK_L_X_Dac_clk_en_pin  = vcc;
  assign CTRL_STICK_L_Y_Dac_clk_en_pin  = vcc;
  assign CTRL_STICK_R_X_Dac_clk_en_pin  = vcc;
  assign CTRL_STICK_R_Y_Dac_clk_en_pin  = vcc;
  assign CTRL_TRIGGER_LT_Dac_clk_en_pin = vcc;
  assign CTRL_TRIGGER_RT_Dac_clk_en_pin = vcc;

  dac_read_en_sig_generator dresg_L_X(.clk(fpga_0_mb_plb_clk), .gpio_in(CTRL_DAC_READ_EN_GPIO_IO_O_pin[0]), .read_en_out(CTRL_STICK_L_X_Read_en_pin));
  dac_read_en_sig_generator dresg_L_Y(.clk(fpga_0_mb_plb_clk), .gpio_in(CTRL_DAC_READ_EN_GPIO_IO_O_pin[1]), .read_en_out(CTRL_STICK_L_Y_Read_en_pin));
  dac_read_en_sig_generator dresg_R_X(.clk(fpga_0_mb_plb_clk), .gpio_in(CTRL_DAC_READ_EN_GPIO_IO_O_pin[2]), .read_en_out(CTRL_STICK_R_X_Read_en_pin));
  dac_read_en_sig_generator dresg_R_Y(.clk(fpga_0_mb_plb_clk), .gpio_in(CTRL_DAC_READ_EN_GPIO_IO_O_pin[3]), .read_en_out(CTRL_STICK_R_Y_Read_en_pin));
  dac_read_en_sig_generator dresg_LT(.clk(fpga_0_mb_plb_clk), .gpio_in(CTRL_DAC_READ_EN_GPIO_IO_O_pin[4]), .read_en_out(CTRL_TRIGGER_LT_Read_en_pin));
  dac_read_en_sig_generator dresg_RT(.clk(fpga_0_mb_plb_clk), .gpio_in(CTRL_DAC_READ_EN_GPIO_IO_O_pin[5]), .read_en_out(CTRL_TRIGGER_RT_Read_en_pin));

  (* BOX_TYPE = "user_black_box" *)
  system
    system_i (
      .fpga_0_RS232_Uart_1_RX_pin ( fpga_0_RS232_Uart_1_RX_pin ),
      .fpga_0_RS232_Uart_1_TX_pin ( fpga_0_RS232_Uart_1_TX_pin ),
      .fpga_0_RS232_Uart_2_RX_pin ( fpga_0_RS232_Uart_2_RX_pin ),
      .fpga_0_RS232_Uart_2_TX_pin ( fpga_0_RS232_Uart_2_TX_pin ),
      .fpga_0_LEDs_8Bit_GPIO_IO_pin ( fpga_0_LEDs_8Bit_GPIO_IO_pin ),
      .fpga_0_LEDs_Positions_GPIO_IO_pin ( fpga_0_LEDs_Positions_GPIO_IO_pin ),
      .fpga_0_Push_Buttons_5Bit_GPIO_IO_pin ( fpga_0_Push_Buttons_5Bit_GPIO_IO_pin ),
      .fpga_0_DIP_Switches_8Bit_GPIO_IO_pin ( fpga_0_DIP_Switches_8Bit_GPIO_IO_pin ),
      .fpga_0_IIC_EEPROM_Sda_pin ( fpga_0_IIC_EEPROM_Sda_pin ),
      .fpga_0_IIC_EEPROM_Scl_pin ( fpga_0_IIC_EEPROM_Scl_pin ),
      .fpga_0_SRAM_Mem_A_pin ( fpga_0_SRAM_Mem_A_pin ),
      .fpga_0_SRAM_Mem_CEN_pin ( fpga_0_SRAM_Mem_CEN_pin ),
      .fpga_0_SRAM_Mem_OEN_pin ( fpga_0_SRAM_Mem_OEN_pin ),
      .fpga_0_SRAM_Mem_WEN_pin ( fpga_0_SRAM_Mem_WEN_pin ),
      .fpga_0_SRAM_Mem_BEN_pin ( fpga_0_SRAM_Mem_BEN_pin ),
      .fpga_0_SRAM_Mem_ADV_LDN_pin ( fpga_0_SRAM_Mem_ADV_LDN_pin ),
      .fpga_0_SRAM_Mem_DQ_pin ( fpga_0_SRAM_Mem_DQ_pin ),
      .fpga_0_SRAM_ZBT_CLK_OUT_pin ( fpga_0_SRAM_ZBT_CLK_OUT_pin ),
      .fpga_0_SRAM_ZBT_CLK_FB_pin ( fpga_0_SRAM_ZBT_CLK_FB_pin ),
      .fpga_0_PCIe_Bridge_RXN_pin ( fpga_0_PCIe_Bridge_RXN_pin ),
      .fpga_0_PCIe_Bridge_RXP_pin ( fpga_0_PCIe_Bridge_RXP_pin ),
      .fpga_0_PCIe_Bridge_TXN_pin ( fpga_0_PCIe_Bridge_TXN_pin ),
      .fpga_0_PCIe_Bridge_TXP_pin ( fpga_0_PCIe_Bridge_TXP_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_Clk_pin ( fpga_0_DDR2_SDRAM_DDR2_Clk_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_Clk_n_pin ( fpga_0_DDR2_SDRAM_DDR2_Clk_n_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_CE_pin ( fpga_0_DDR2_SDRAM_DDR2_CE_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_CS_n_pin ( fpga_0_DDR2_SDRAM_DDR2_CS_n_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_ODT_pin ( fpga_0_DDR2_SDRAM_DDR2_ODT_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_RAS_n_pin ( fpga_0_DDR2_SDRAM_DDR2_RAS_n_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_CAS_n_pin ( fpga_0_DDR2_SDRAM_DDR2_CAS_n_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_WE_n_pin ( fpga_0_DDR2_SDRAM_DDR2_WE_n_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_BankAddr_pin ( fpga_0_DDR2_SDRAM_DDR2_BankAddr_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_Addr_pin ( fpga_0_DDR2_SDRAM_DDR2_Addr_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_DQ_pin ( fpga_0_DDR2_SDRAM_DDR2_DQ_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_DM_pin ( fpga_0_DDR2_SDRAM_DDR2_DM_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_DQS_pin ( fpga_0_DDR2_SDRAM_DDR2_DQS_pin ),
      .fpga_0_DDR2_SDRAM_DDR2_DQS_n_pin ( fpga_0_DDR2_SDRAM_DDR2_DQS_n_pin ),
      .fpga_0_clk_1_sys_clk_pin ( fpga_0_clk_1_sys_clk_pin ),
      .fpga_0_rst_1_sys_rst_pin ( fpga_0_rst_1_sys_rst_pin ),
      .fpga_0_mb_plb_clk ( fpga_0_mb_plb_clk ),
      .fpga_0_PCIe_Diff_Clk_IBUF_DS_P_pin ( fpga_0_PCIe_Diff_Clk_IBUF_DS_P_pin ),
      .fpga_0_PCIe_Diff_Clk_IBUF_DS_N_pin ( fpga_0_PCIe_Diff_Clk_IBUF_DS_N_pin ),
      .LCD_1602_4BIT_INTERFACE_GPIO_IO ( LCD_1602_4BIT_INTERFACE_GPIO_IO ),
      .CTRL_BUTTONs_16Bit_GPIO_IO_pin ( CTRL_BUTTONs_16Bit_GPIO_IO_pin ),
      .CTRL_DAC_READ_EN_GPIO_IO_O_pin ( CTRL_DAC_READ_EN_GPIO_IO_O_pin ),
      .CTRL_STICK_L_X_Read_en_pin ( CTRL_STICK_L_X_Read_en_pin ),
      .CTRL_STICK_L_X_Dac_clk_en_pin ( CTRL_STICK_L_X_Dac_clk_en_pin ),
      .CTRL_STICK_L_X_Dac_Out_pin ( CTRL_STICK_L_X_Dac_Out_pin ),
      .CTRL_STICK_L_Y_Read_en_pin ( CTRL_STICK_L_Y_Read_en_pin ),
      .CTRL_STICK_L_Y_Dac_clk_en_pin ( CTRL_STICK_L_Y_Dac_clk_en_pin ),
      .CTRL_STICK_L_Y_Dac_Out_pin ( CTRL_STICK_L_Y_Dac_Out_pin ),
      .CTRL_STICK_R_X_Read_en_pin ( CTRL_STICK_R_X_Read_en_pin ),
      .CTRL_STICK_R_X_Dac_clk_en_pin ( CTRL_STICK_R_X_Dac_clk_en_pin ),
      .CTRL_STICK_R_X_Dac_Out_pin ( CTRL_STICK_R_X_Dac_Out_pin ),
      .CTRL_STICK_R_Y_Read_en_pin ( CTRL_STICK_R_Y_Read_en_pin ),
      .CTRL_STICK_R_Y_Dac_clk_en_pin ( CTRL_STICK_R_Y_Dac_clk_en_pin ),
      .CTRL_STICK_R_Y_Dac_Out_pin ( CTRL_STICK_R_Y_Dac_Out_pin ),
      .CTRL_TRIGGER_LT_Read_en_pin ( CTRL_TRIGGER_LT_Read_en_pin ),
      .CTRL_TRIGGER_LT_Dac_clk_en_pin ( CTRL_TRIGGER_LT_Dac_clk_en_pin ),
      .CTRL_TRIGGER_LT_Dac_Out_pin ( CTRL_TRIGGER_LT_Dac_Out_pin ),
      .CTRL_TRIGGER_RT_Read_en_pin ( CTRL_TRIGGER_RT_Read_en_pin ),
      .CTRL_TRIGGER_RT_Dac_clk_en_pin ( CTRL_TRIGGER_RT_Dac_clk_en_pin ),
      .CTRL_TRIGGER_RT_Dac_Out_pin ( CTRL_TRIGGER_RT_Dac_Out_pin )
    );

endmodule
