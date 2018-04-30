/******************************************    *******************************
      > File Name: SystemDef.h
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月15日 星期三 10时43分33秒
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _SYSTEMDEF_H
#define _SYSTEMDEF_H


//时钟clock module
#define CM_PER_ADDR						(0x44E00000)				/*Clock Module Peripheral Registers
																	  时钟模块外设寄存器*/
#define CM_WKUP_ADDR					(0x44E00400)				/*Clock Module Wakeup Registers
																	  时钟模块唤醒寄存器*/
#define CM_DPLL_ADDR					(0x44E00500)				/*Clock Module PLL Registers
																	  时钟模块PLL寄存器*/
#define CM_MPU_ADDR						(0x44E00600)				/*Clock Module MPU Registers
																	  时钟模块MPU寄存器*/
#define CM_DEVICE_ADDR					(0x44E00700)				/*Clock Module Device Registers
																	  时钟模块设备寄存器*/
#define CM_RTC_ADDR						(0x44E00800)				/*Clock Module RTC Registers
																	  时钟模块RTC寄存器*/
#define CM_GFX_ADDR						(0x44E00900)				/*Clock Module Graphics Controller Registers
																	  时钟模块图形控制器寄存器*/
#define CM_CEFUSE_ADDR					(0x44E00A00)				/*Clock Module Efuse Registers
																	  时钟模块Efuse寄存器*/

//. CM_PER REGISTERS
#define CM_PER_L4LS_CLKSTCTRL_ADDR_OFFSET		(0x0)						/**/
#define CM_PER_L3S_CLKSTCTRL_ADDR_OFFSET		(0x4)						/**/
#define CM_PER_L3_CLKSTCTRL_ADDR_OFFSET			(0xC)						/**/
#define CM_PER_CPGMAC0_CLKCTRL_ADDR_OFFSET		(0x14)						/**/
#define CM_PER_LCDC_CLKCTRL_ADDR_OFFSET			(0x18)						/**/
#define CM_PER_USB0_CLKCTRL_ADDR_OFFSET			(0x1C)						/**/
#define CM_PER_TPTC0_CLKCTRL_ADDR_OFFSET		(0x24)						/**/
#define CM_PER_EMIF_CLKCTRL_ADDR_OFFSET			(0x28)						/**/
#define CM_PER_OCMCRAM_CLKCTRL_ADDR_OFFSET		(0x2C)						/**/
#define CM_PER_GPMC_CLKCTRL_ADDR_OFFSET			(0x30)						/**/
#define CM_PER_MCASP0_CLKCTRL_ADDR_OFFSET		(0x34)						/**/
#define CM_PER_UART5_CLKCTRL_ADDR_OFFSET		(0x38)						/**/
#define CM_PER_MMC0_CLKCTRL_ADDR_OFFSET			(0x3C)						/**/
#define CM_PER_ELM_CLKCTRL_ADDR_OFFSET			(0x40)						/**/
#define CM_PER_I2C2_CLKCTRL_ADDR_OFFSET			(0x44)						/**/
#define CM_PER_I2C1_CLKCTRL_ADDR_OFFSET			(0x48)						/**/
#define CM_PER_SPI0_CLKCTRL_ADDR_OFFSET			(0x4C)						/**/
#define CM_PER_SPI1_CLKCTRL_ADDR_OFFSET			(0x50)						/**/
#define CM_PER_L4LS_CLKCTRL_ADDR_OFFSET			(0x60)						/**/
#define CM_PER_MCASP1_CLKCTRL_ADDR_OFFSET		(0x68)						/**/
#define CM_PER_UART1_CLKCTRL_ADDR_OFFSET		(0x6C)						/**/
#define CM_PER_UART2_CLKCTRL_ADDR_OFFSET		(0x70)						/**/
#define CM_PER_UART3_CLKCTRL_ADDR_OFFSET		(0x74)						/**/
#define CM_PER_UART4_CLKCTRL_ADDR_OFFSET		(0x78)						/**/
#define CM_PER_TIMER7_CLKCTRL_ADDR_OFFSET		(0x7C)						/**/
/*
 * Bit		Field				Type	Reset	Description
 * 31-18	Reserved			R		0h
 * 17-16	IDLEST				R		3h		Module idle status.
 *												模块空闲状态
 *												0x0 = Func : Module is fully functional, including OCP
 *												0x0 = Func：模块功能齐全，包括OCP
 *												0x1 = Trans : Module is performing transition: wakeup, or sleep, or
 *												sleep abortion
 *												0x1 = Trans：模块正在执行转换：唤醒，或睡眠或者睡眠中止
 *												0x2 = Idle : Module is in Idle mode (only OCP part). It is functional if
 *												using separate functional clock
 *												0x2 =空闲：模块处于空闲模式（仅限OCP部分）。 如果使用单独的功能时钟，它是有用的
 *												0x3 = Disable : Module is disabled and cannot be accessed
 *												0x3 =禁用：模块被禁用，无法访问
 * 15-2		Reserved			R		0h
 * 1-0		MODULEMODE			R/W		0h		Control the way mandatory clocks are managed.
 *												控制强制时钟的管理方式。
 *												0x0 = DISABLED : Module is disable by SW. Any OCP access to
 *												module results in an error, except if resulting from a module wakeup
 *												(asynchronous wakeup).
 *												0x0 =禁用：模块由SW禁用。 除非由模块唤醒（异步唤醒），
 *												否则任何对模块的OCP访问都会导致错误。
 *												0x1 = RESERVED_1 : Reserved
 *												0x2 = ENABLE : Module is explicitly enabled. 
 *												Interface clock (if not used for functions) may be 
 *												gated according to the clock domain state. 
 *												Functional clocks are guarantied to stay present. 
 *												As long as in this configuration, power domain sleep transition cannot happen.
 *												0x2 =启用：显式启用模块。 
 *												接口时钟（如果不用于功能）可以根据时钟域状态进行选通。 
 *												功能时钟保证保持现状。 
 *												只要在这种配置下，电源域的睡眠转换就不会发生。
 *												0x3 = RESERVED : Reserved
 *
 * */
#define CM_PER_TIMER2_CLKCTRL_ADDR_OFFSET		(0x80)						/**/
#define CM_PER_TIMER3_CLKCTRL_ADDR_OFFSET		(0x84)						/**/
#define CM_PER_TIMER4_CLKCTRL_ADDR_OFFSET		(0x88)						/**/
#define CM_PER_GPIO1_CLKCTRL_ADDR_OFFSET		(0xAC)						/**/
#define CM_PER_GPIO2_CLKCTRL_ADDR_OFFSET		(0xB0)						/**/
#define CM_PER_GPIO3_CLKCTRL_ADDR_OFFSET		(0xB4)						/**/
#define CM_PER_TPCC_CLKCTRL_ADDR_OFFSET			(0xBC)						/**/
#define CM_PER_DCAN0_CLKCTRL_ADDR_OFFSET		(0xC0)						/**/
#define CM_PER_DCAN1_CLKCTRL_ADDR_OFFSET		(0xC4)						/**/
#define CM_PER_EPWMSS1_CLKCTRL_ADDR_OFFSET		(0xCC)						/**/
#define CM_PER_EPWMSS0_CLKCTRL_ADDR_OFFSET		(0xD4)						/**/
#define CM_PER_EPWMSS2_CLKCTRL_ADDR_OFFSET		(0xD8)						/**/
#define CM_PER_L3_INSTR_CLKCTRL_ADDR_OFFSET		(0xDC)						/**/
#define CM_PER_L3_CLKCTRL_ADDR_OFFSET			(0xE0)						/**/
#define CM_PER_IEEE5000_CLKCTRL_ADDR_OFFSET		(0xE4)						/**/
#define CM_PER_PRU_ICSS_CLKCTRL_ADDR_OFFSET		(0xE8)						/**/
#define CM_PER_TIMER5_CLKCTRL_ADDR_OFFSET		(0xEC)						/**/
#define CM_PER_TIMER6_CLKCTRL_ADDR_OFFSET		(0xF0)						/**/
#define CM_PER_MMC1_CLKCTRL_ADDR_OFFSET			(0xF4)						/**/
#define CM_PER_MMC2_CLKCTRL_ADDR_OFFSET			(0xF8)						/**/
#define CM_PER_TPTC1_CLKCTRL_ADDR_OFFSET		(0xFC)						/**/
#define CM_PER_TPTC2_CLKCTRL_ADDR_OFFSET		(0x100)						/**/
#define CM_PER_SPINLOCK_CLKCTRL_ADDR_OFFSET		(0x10C)						/**/
#define CM_PER_MAILBOX0_CLKCTRL_ADDR_OFFSET		(0x110)						/**/
#define CM_PER_L4HS_CLKSTCTRL_ADDR_OFFSET		(0x11C)						/**/
#define CM_PER_L4HS_CLKCTRL_ADDR_OFFSET			(0x120)						/**/
#define CM_PER_OCPWP_L3_CLKSTCTRL_ADDR_OFFSET	(0x12C)						/**/
#define CM_PER_OCPWP_CLKCTRL_ADDR_OFFSET		(0x130)						/**/
#define CM_PER_PRU_ICSS_CLKSTCTRL_ADDR_OFFSET	(0x140)						/**/
#define CM_PER_CPSW_CLKSTCTRL_ADDR_OFFSET		(0x144)						/**/
#define CM_PER_LCDC_CLKSTCTRL_ADDR_OFFSET		(0x148)						/**/
#define CM_PER_CLKDIV32K_CLKCTRL_ADDR_OFFSET	(0x14C)						/**/
#define CM_PER_CLK_24MHZ_CLKSTCTRL_ADDR_OFFSET	(0x150)						/**/

//. CM_DPLL REGISTERS . [warm reset insensitive]
#define CLKSEL_TIMER7_CLK_ADDR_OFFSET		(0x4)						/*Selects the Mux select line for TIMER7 clock
																		  为TIMER7时钟选择Mux选择线*/
#define CLKSEL_TIMER2_CLK_ADDR_OFFSET		(0x8)						/**/
#define CLKSEL_TIMER3_CLK_ADDR_OFFSET		(0xC)						/**/
#define CLKSEL_TIMER4_CLK_ADDR_OFFSET		(0x10)						/**/
/*
 * Bit		Field					Type	Reset	Description
 * 31-2		Reserved				R		0h
 * 1-0		CLKSEL					R/W		1h		Selects the Mux select line for TIMER7 clock [warm reset insensitive]
 *													0x0 = SEL1 : Select TCLKIN clock
 *													选择TCLKIN时钟
 *													0x1 = SEL2 : Select CLK_M_OSC clock
 *													选择CLK_M_OSC时钟
 *													0x2 = SEL3 : Select CLK_32KHZ clock
 *													选择CLK_32KHZ时钟
 *													0x3 = SEL4 : Reserved
 * */
#define CM_MAC_CLKSEL_ADDR_OFFSET			(0x14)						/*Selects the clock divide ration for MII clock
																		  选择MII时钟的时钟分频比*/
/*
 * 31-3		Reserved				R		0h
 * 2		MII_CLK_SEL				R/W		1h		MII Clock Divider Selection.
 *													MII时钟分频器选择。
 *													This bit is warm reset insensitive when CPSW RESET_ISO is enabled
 *													当CPSW RESET_ISO被使能时，该位对温度复位不敏感
 *													0x0 = SEL0 : Selects 1/2 divider of SYSCLK2
 *													0x0 = SEL0：选择SYSCLK2的1/2分频器
 *													0x1 = SEL1 : Selects 1/5 divide ratio of SYSCLK2
 *													0x1 = SEL1：选择SYSCLK2的1/5分频比
 * 1-0		Reserved				R		0h
 * */
#define CLKSEL_TIMER5_CLK_ADDR_OFFSET		(0x18)						/**/
#define CLKSEL_TIMER6_CLK_ADDR_OFFSET		(0x1C)						/**/
#define CM_CPTS_RFT_CLKSEL_ADDR_OFFSET		(0x20)						/*Selects the Mux select line for CPTS RFT clock
																		  为CPTS RFT时钟选择Mux选择线*/
#define CLKSEL_TIMER1MS_CLK_ADDR_OFFSET		(0x28)						/**/
#define CLKSEL_GFX_FCLK_ADDR_OFFSET			(0x2C)						/*Selects the divider value for GFX clock
																		  选择GFX时钟的分频器值*/
#define CLKSEL_PRU_ICSS_OCP_CLK_ADDR_OFFSET	(0x30)						/*Controls the Mux select line for PRU-ICSS OCP clock
																		  控制PRU-ICSS OCP时钟的MUX选择线*/
#define CLKSEL_LCDC_PIXEL_CLK_ADDR_OFFSET	(0x34)						/*Controls the Mux select line for LCDC PIXEL clock
																		  控制LCDC PIXEL时钟的Mux选择线*/
#define CLKSEL_WDT1_CLK_ADDR_OFFSET			(0x38)						/*Selects the Mux select line for Watchdog1 clock
																		  为看门狗1时钟选择Mux选择线*/
#define CLKSEL_GPIO0_DBCLK_ADDR_OFFSET		(0x3C)						/*Selects the Mux select line for GPIO0 debounce clock
																		  为GPIO0去抖时钟选择Mux选择线*/
/*
 * 
 * */


/*
 * . CM_DIV_M6_DPLL_CORE Register
 * 该寄存器提供对HSDIVIDER的CLKOUT3 o / p的控制。 [热重置不敏感]
 * Bit		Field					Type		Reset	Description
 * 31-13	RESERVED				Rreturns0s	0h
 * 12		HSDIVIDER_CLKOUT3_P		R/W			0h		Automatic power down for HSDIVIDER M6 divider and hence
 *			WDN											CLKOUT3 output when the o/p clock is gated.
 *														0h (R/W) = Keep M6 divider powered on even when CLKOUT3 is gated.
 *														1h (R/W) = Automatically power down M6 divider when CLKOUT3 is gated.
 * 11-10	RESERVED				Rreturns0s	0h
 * 9		ST_HSDIVIDER_CLKOUT3	R			0h		HSDIVIDER CLKOUT3 status
 *														0h (R) = The clock output is enabled
 *														1h (R) = The clock output is gated
 * 8		HSDIVIDER_CLKOUT3_		R/W			0h		Control gating of HSDIVIDER CLKOUT3
 *			GATE_CTRL									0h (R/W) = Automatically gate this clock when there is no
 *														dependency for it
 *														1h (R/W) = Force this clock to stay enabled even if there is no
 *														request
 * 7-6		RESERVED				Rreturns0s	0h
 * 5		HSDIVIDER_CLKOUT3_		R			0h		Toggle on this status bit after changing HSDIVIDER_CLKOUT3_DIV
 *			DIVCHACK									indicates that the change in divider value has taken effect
 * 4-0		HSDIVIDER_CLKOUT3_		R/W			4h		DPLL post-divider factor, M6, for internal clock generation.
 *			DIV											Divide values from 1 to 31.
 *														0h (R/W) = Reserved
 * */

#endif

