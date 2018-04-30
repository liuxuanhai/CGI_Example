/******************************************    *******************************
      > File Name: GpioDef.h
      > Author: Ywl
      > Descripsion:
      > Created Time:     Mon 13 Nov 2017 07:42:39 PM CST
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _GPIODEF_H
#define _GPIODEF_H

/*
  GPIO基地址
 */
#define GPIO0_BASE_ADDR					(0x44E07000)							/*GPIO0首地址,~0x44E07FFF*/
#define GPIO1_BASE_ADDR					(0x4804C000)							/*GPIO0首地址,~0x4804CFFF*/
#define GPIO2_BASE_ADDR					(0x481AC000)							/*GPIO2首地址,~0x481ACFFF*/
#define GPIO3_BASE_ADDR					(0x481AE000)							/*GPIO2首地址,~0x481AEFFF*/
/*
  GPIO寄存器偏移地址
 */
#define GPIO_REVISION_OFFSET			(0x0)							/*GPIO版本寄存器，包含版本号，只读*/
/* 
 * 寄存器字段说明
 * Bit		Field			Type	Reset	Description
 * 31-30	SCHEME			R		1h		Used to distinguish between old Scheme and current.
 *											(区分旧的方案和当前的)
 * 29-28	RESERVED		R		1h		(保留)
 * 27-16	FUNC			R		60h		Indicates a software compatible module family.
 *											(表示软件兼容的模块系列)
 * 15-11	RTL				R		1h		RTL version
 * 10-8		MAJOR			R		0h		Major Revision
 * 7-6		CUSTOM			R		0h		Indicates a special version for a particular device.
 *											(表示特殊设备的特殊版本)
 * 5-0		MINOR			R		1h		Minor Revision
 * */
#define GPIO_SYSCONFIG_OFFSET			(0x10)							/*GPIO_SYSCONFIG寄存器控制着L4互连的各种参数。*/
/*
 * 当AUTOIDLE位置1时，由于采样门控机制中的数据，GPIO_DATAIN读取命令具有3个OCP周期延迟。
 * 当AUTOIDLE位未置1时，GPIO_DATAIN读命令的响应时间为2 OCP。
 *
 * Bit		Field			Type	Reset	Description
 * 31-5		RESERVED		R		0h
 * 4-3		IDLEMODE		R/W		0h		Power Management, Req/Ack control.
 *											电源管理，请求/确认控制。
 *											0h = Force-idle. An idle request is acknowledged unconditionally
 *											0h = 强制空闲。 空闲请求被无条件地确认
 *											1h = No-idle. An idle request is never acknowledged
 *											1h = 没有空闲。 空闲请求永远不会被确认
 *											2h = Smart-idle. Acknowledgment to an idle request is given based
 *											on the internal activity of the module
 *											2h = 智能闲置。 根据模块的内部活动给出对空闲请求的确认
 *											3h = Smart Idle Wakeup (GPIO0 only)
 *											3h = 智能空闲唤醒（仅限GPIO0）
 * 2		ENAWAKEUP		R/W		0h		0h = Wakeup generation is disabled.
 *											0h = 唤醒产生被禁用。
 *											1h = Wakeup capability is enabled upon expected transition on input
 *											GPIO pin.
 *											1h = 唤醒功能在输入GPIO引脚预期转换时使能。
 * 1		SOFTRESET		R/W		0h		Software reset.
 *											软件复位
 *											This bit is automatically reset by the hardware.
 *											该位由硬件自动复位
 *											During reads, it always returns 0.
 *											在读取期间，一直返回0
 *											0h = Normal mode
 *											0h = 正常模式
 *											1h = The module is reset
 *											1h = 模块复位
 * 0		AUTOIDLE		R/W		0h		Internal interface clock gating strategy
 *											内部接口时钟门控策略
 *											0h = Internal Interface OCP clock is free-running
 *											0h = 内部接口OCP时钟是自由运行的
 *											1h = Automatic internal OCP clock gating, based on the OCP
 *											interface activity
 *											1h = 基于OCP接口活动的自动内部OCP时钟门控
 * */
#define GPIO_EOI_OFFSET					(0x20)
/*
 * 该模块通过中断信号支持DMA事件。 该寄存器必须在DMA完成后写入，以便从该模块触发后续DMA事件
 * Bit		Field			Type	Reset	Description
 * 31-1		RESERVED		R		0h
 * 0		DMAEvent_Ack	R/W		0h		Write 0 to acknowledge DMA event has been completed.
 *											写0确认DMA事件已经完成。
 *											Module will be able to generate another DMA event only when the
 *											previous one has been acknowledged using this register.
 *											只有在使用该寄存器确认了前一个DMA事件时，模块才能生成另一个DMA事件。
 *											Reads always returns 0
 *											读取总是返回0
 * */
#define GPIO_IRQSTATUS_RAW_0_OFFSET		(0x24)
#define GPIO_IRQSTATUS_RAW_1_OFFSET		(0x28)
/*
 * GPIO_IRQSTATUS_RAW_0寄存器提供中断处理的核心状态信息，显示所有活动事件（启用和未启用）。 
 * 这些字段是可读写的。 
 * 写1到一个位将其设置为1，即触发IRQ（主要用于调试）。 
 * 写入0不起作用，即寄存器值不被修改。 
 * 只有启用时，活动事件触发IRQ输出线上的实际中断请求。
 * Bit		Field			Type	Reset	Description
 * 31-0		INTLINE[n]		R/W		0h		Interrupt n status.
 *											中断n状态
 *											0h = No effect.
 *											0h = 没有效果
 *											1h = IRQ is triggered.
 *											1h = 触发IRQ
 * */
#define GPIO_IRQSTATUS_0_OFFSET			(0x2C)
#define GPIO_IRQSTATUS_1_OFFSET			(0x30)
/*
 * GPIO_IRQSTATUS_0寄存器提供中断处理的核心状态信息，显示所有已启用的活动事件。 
 * 这些字段是可读写的。 
 * 将1写入一个位会将该位清零，即清除IRQ。 
 * 写入0不起作用，即寄存器值不被修改。 
 * 只有启用，活动事件触发IRQ输出线上的实际中断请求。
 * Bit		Field			Type	Reset	Description
 * 31-0		INTLINE[n]		R/W1C	0h		Interrupt n status.
 *											0h (W) = No effect.
 *											0h(写入) = 没有效果
 *											0h (R) = IRQ is not triggered.
 *											0h(读取) = IRQ没有触发
 *											1h (W) = Clears the IRQ.
 *											1h(写入) = 清除IRQ
 *											1h (R) = IRQ is triggered.
 *											1h(读取) = 触发IRQ
 * */
#define GPIO_IRQSTATUS_SET_0_OFFSET		(0x34)
#define GPIO_IRQSTATUS_SET_1_OFFSET		(0x38)
/*
 * GPIO_IRQSTATUS_SET_0寄存器中每1位都会启用特定的中断事件来触发中断请求。 
 * 写1到一个位使能中断字段。 
 * 写入0不起作用，即寄存器值不被修改。
 * Bit		Field			Type	Reset	Description
 * 31-0		INTLINE[n]		R/W		0h		Interrupt n enable
 *											中断n使能
 *											0h = No effect.
 *											0h = 没有效果
 *											1h = Enable IRQ generation.
 *											1h = 使能IRQ产生
 * */
#define GPIO_IRQSTATUS_CLR_0_OFFSET		(0x3C)
#define GPIO_IRQSTATUS_CLR_1_OFFSET		(0x40)
/*
 * GPIO_IRQSTATUS_CLR_0寄存器中的每1位都会清除特定的中断事件。 
 * 写1到一位禁止中断字段。 
 * 写入0不起作用，即寄存器值不被修改。
 * Bit		Field			Type	Reset	Description
 * 31-0		INTLINE[n]		R/W		0h		Interrupt n enable
 *											中断n使能
 *											0h = No effect.
 *											0h = 没有效果
 *											1h = Disable IRQ generation.
 *											1h = 禁止IRQ产生
 * */
#define GPIO_IRQWAKEN_0_OFFSET			(0x44)
#define GPIO_IRQWAKEN_1_OFFSET			(0x48)
/*
 * 每个事件唤醒使能矢量（对应于第一行中断）。
 * GPIO_IRQWAKEN_0寄存器中的每1位使特定（同步）IRQ请求源能够生成异步唤醒（在相应的唤醒线上）。 
 * 该寄存器允许用户在产生唤醒请求时屏蔽输入GPIO上的预期转换。 
 * 在来自主处理器的任何空闲模式请求之前，GPIO_IRQWAKEN_0与接口时钟同步编程。 
 * 注：在强制空闲模式下，模块唤醒功能被完全禁止。 
 * 唤醒产生也可以使用来自GPIO_SYSCONFIG寄存器的EnaWakeUp位在模块级进行门控。
 * Bit		Field			Type	Reset	Description
 * 31-0		INTLINE			R/W		0h		Wakeup Enable for Interrupt Line
 *											中断线路唤醒使能
 *											0h = Disable wakeup generation.
 *											0h = 禁止唤醒产生
 *											1h = Enable wakeup generation
 *											1h = 允许唤醒产生
 * */
#define GPIO_SYSSTATUS_OFFSET			(0x114)
/*
 * GPIO_SYSSTATUS寄存器提供有关GPIO模块的复位状态信息。 
 * 这是一个只读寄存器; 写入这个寄存器不起作用。
 * Bit		Field			Type	Reset	Description
 * 31-1		RESERVED		R		0h
 * 0		RESETDONE		R		0h		Reset status information.
 *											复位状态信息
 *											0h = Internal Reset is on-going.
 *											0h = 内部复位正在进行
 *											1h = Reset completed.
 *											1h = 复位完成
 * */
#define GPIO_CTRL_OFFSET				(0x130)
/*
 * GPIO_CTRL寄存器控制时钟门控功能。 
 * DISABLEMODULE位控制模块级的时钟门控功能。 
 * 置位时，该位强制所有内部时钟路径的时钟门控。 
 * 模块内部活动已暂停。 
 * 系统接口不受此位的影响。 
 * 系统接口时钟门控由系统配置寄存器（GPIO_SYSCONFIG）中的AUTOIDLE位控制。 
 * 由于在芯片级选择了多路复用配置，当模块不使用时，该位将用于省电。 
 * 该位优先于所有其他内部配置位。
 * Bit		Field			Type	Reset	Description
 * 31-3		RESERVED		R		0h
 * 2-1		GATINGRATIO		R/W		0h		Gating Ratio.
 *											门控比例
 *											Controls the clock gating for the event detection logic.
 *											控制事件检测逻辑的时钟门控。
 *											0h = Functional clock is interface clock.
 *											0h = 功能时钟是接口时钟。
 *											1h = Functional clock is interface clock divided by 2.
 *											1h = 功能时钟是接口时钟除以2。
 *											2h = Functional clock is interface clock divided by 4.
 *											2h = 功能时钟是接口时钟除以4。
 *											3h = Functional clock is interface clock divided by 8.
 *											3h = 功能时钟是接口时钟除以8。
 * 0		DISABLEMODULE	R/W		0h		Module Disable
 *											模块禁用
 *											0h = Module is enabled, clocks are not gated.
 *											0h = 模块使能，时钟不门控。
 *											1h = Module is disabled, clocks are gated.
 *											1h = 模块禁止，时钟门控
 *
 * */
#define GPIO_OE_OFFSET					(0x134)							/*数据方向寄存器,某位设置为0则相应管脚为输出*/
/*
 * GPIO_OE寄存器用于使能引脚输出功能。 
 * 复位时，所有与GPIO相关的引脚都被配置为输入输出功能。 
 * 该寄存器不在模块内部使用，其唯一的功能是携带焊盘配置。 
 * 当应用程序使用引脚作为输出并且不希望从该引脚产生中断时，应用程序可以/必须正确配置中断使能寄存器。
 * Bit		Field			Type	Reset		Description
 * 31-0		OUTPUTEN[n]		R/W		FFFFFFFFh	Output Data Enable
 *												输出数据启用
 *												0h = The corresponding GPIO port is configured as an output.
 *												0h = 相应的GPIO端口被配置为输出。
 *												1h = The corresponding GPIO port is configured as an input.
 *												0h = 相应的GPIO端口被配置为输入。
 * */
#define GPIO_DATAIN_OFFSET				(0x138)							/*数据寄存器,用于输入读取*/
/*
 * GPIO_DATAIN寄存器用于注册从GPIO引脚读取的数据。 
 * GPIO_DATAIN寄存器是只读寄存器。 
 * 输入数据与接口时钟同步采样，然后与接口时钟同步捕获到GPIO_DATAIN寄存器中。 
 * 所以，改变之后，在两个接口时钟周期（所需要的同步和写入数据周期）之后，GPIO引脚电平被捕获到该寄存器中。 
 * 当系统配置寄存器（GPIO_SYSCONFIG）中的AUTOIDLE位置1时，由于采样门控机制中的数据，GPIO_DATAIN读取命令有3个OCP周期延迟。 
 * 当AUTOIDLE位未置1时，GPIO_DATAIN读命令的响应时间为2 OCP。
 * Bit		Field			Type	Reset	Description
 * 31-0		DATAIN			R		0h		Sampled Input Data
 *											采样输入数据
 * */
#define GPIO_DATAOUT_OFFSET				(0x13C)							/*数据寄存器,用于输出写入*/
/*
 * GPIO_DATAOUT寄存器用于设置GPIO输出引脚的值。 
 * 数据与接口时钟同步写入GPIO_DATAOUT寄存器。 
 * 该寄存器可以通过直接读/写操作或使用备用设置/清除功能进行访问。 
 * 该功能可以通过对设置数据输出寄存器（GPIO_SETDATAOUT）的单个写访问或清除数据输出寄存器（GPIO_CLEARDATAOUT）地址来设置或清除该寄存器的特定位
 * Bit		Field			Type	Reset	Description
 * 31-0		DATAOUT			R/W		0h		Data to set on output pins
 *											要在输出引脚上设置的数据
 * */
#define GPIO_LEVELDETECT0_OFFSET		(0x140)
#define GPIO_LEVELDETECT1_OFFSET		(0x144)
/*
 * GPIO_LEVELDETECT0寄存器用于为每个输入线启用/禁用要用于产生中断请求的低电平（0）检测。 
 * 同时使能一个给定引脚的高电平检测和低电平检测，可以产生一个恒定的中断发生器
 * Bit		Field			Type	Reset	Description
 * 31-0		LEVELDETECT0[n]	R/W		0h		Low Level Interrupt Enable
 *											低电平中断启用
 *											0h = Disable the IRQ assertion on low-level detect.
 *											0h = 在低电平检测时禁用IRQ断言。
 *											1h = Enable the IRQ assertion on low-level detect.
 *											1h = 在低电平检测时允许IRQ断言。
 * */
#define GPIO_RISINGDETECT_OFFSET		(0x148)
/*
 * GPIO_RISINGDETECT寄存器用于为每条输入线启用/禁用用于产生中断请求的上升沿（转换0到1）检测。
 * Bit		Field			Type	Reset	Description
 * 31-0		RISINGDETECT[n]	R/W		0h		Rising Edge Interrupt Enable
 *											上升沿中断使能
 *											0h = Disable IRQ on rising-edge detect.
 *											0h = 在上升沿检测时禁止IRQ
 *											1h = Enable IRQ on rising-edge detect.
 *											1h = 在上升沿检测时允许IRQ
 * */
#define GPIO_FALLINGDETECT_OFFSET		(0x14C)
/*
 * GPIO_FALLINGDETECT寄存器用于为每个输入线启用/禁用用于产生中断请求的下降沿（转换1到0）检测。
 * */
#define GPIO_DEBOUNCENABLE_OFFSET		(0x150)
/*
 * GPIO_DEBOUNCENABLE寄存器用于启用/禁用每个输入行的去抖动功能。
 * Bit		Field				Type	Reset	Description
 * 31-0		DEBOUNCEENABLE[n]	R/W		0h		Input Debounce Enable
 *												输入去抖启用
 *												0h = Disable debouncing feature on the corresponding input port.
 *												0h =禁用相应输入端口上的去抖动功能。
 *												1h = Enable debouncing feature on the corresponding input port.
 *												1h =在相应的输入端口上启用去抖动功能
 * */
#define GPIO_DEBOUNCINGTIME_OFFSET		(0x150)
/*
 * GPIO_DEBOUNCINGTIME寄存器控制去抖动时间（对于所有端口，该值是全局的）。 
 * 去抖单元以去抖时钟（32 kHz）运行，该寄存器表示要使用的时钟周期数（31 s）。
 * Bit		Field			Type	Reset	Description
 * 31-8		RESERVED		R/W		0h
 * 7-0		DEBOUNCETIME	R/W		0h		Input Debouncing Value in 31 microsecond steps.
 *											以31微秒的步长输入去抖值。
 *											Debouncing Value = (DEBOUNCETIME + 1) * 31 microseconds
 *											去抖值=（DEBOUNCETIME + 1）* 31微秒
 * */
#define GPIO_CLEARDATAOUT_OFFSET		(0x190)							/*清除数据输出寄存器*/
/*
 * 将GPIO_CLEARDATAOUT寄存器中的某位写入1将清零GPIO_DATAOUT寄存器中的相应位; 
 * 写0不起作用。 
 * 读取GPIO_CLEARDATAOUT寄存器将返回数据输出寄存器（GPIO_DATAOUT）的值。
 * Bit		Field			Type	Reset	Description
 * 31-0		INTLINE[n]		R/W		0h		Clear Data Output Register
 *											清除数据输出寄存器
 *											0h = No effect
 *											0h = 没有效果
 *											1h = Clear the corresponding bit in the GPIO_DATAOUT register.
 *											1h = 清除GPIO_DATAOUT寄存器中的相应位
 * */
#define GPIO_SETDATAOUT_OFFSET			(0x194)							/*数据输出寄存器置1寄存器*/
/*
 * 将GPIO_SETDATAOUT寄存器中的某位写入1可将GPIO_DATAOUT寄存器中的相应位置1; 
 * 写0不起作用。 
 * 读取GPIO_SETDATAOUT寄存器将返回数据输出寄存器（GPIO_DATAOUT）的值。
 * Bit		Field			Type	Reset	Description
 * 31-0		INTLINE[n]		R/W		0h		Set Data Output Register
 *											设置数据输出寄存器
 *											0h = No effect
 *											0h = 没有效果
 *											1h = Set the corresponding bit in the GPIO_DATAOUT register.
 *											1h = 置位GPIO_DATAOUT寄存器的相应位
 * */



#define GPIO_PIN(bank, gpio)			((bank)*32 + (gpio))	/*GPIO转换为编号*/
#define GPIO_PIN_GET_GROUP(pin)			((pin)/32)				/*通过管脚编号取得所在组*/
#define GPIO_PIN_GET_NO(pin)			((pin)%32)				/*通过管脚编号取得组内编号*/

#define GPIO_PIN_GROUP_0				0						/*第一组GPIO*/
#define GPIO_PIN_GROUP_1				1
#define GPIO_PIN_GROUP_2				2
#define GPIO_PIN_GROUP_3				3

#define GPIO_MIN_PIN					GPIO_PIN(0, 0)				/*第一根管脚编号*/
#define GPIO_MAX_PIN					GPIO_PIN(3, 31)				/*最大管脚编号*/

/*GPIO_0*/
#define GPIO0_0						GPIO_PIN(0, 0)
#define GPIO0_1						GPIO_PIN(0, 1)
#define GPIO0_2						GPIO_PIN(0, 2)
#define GPIO0_3						GPIO_PIN(0, 3)
#define GPIO0_4						GPIO_PIN(0, 4)
#define GPIO0_5						GPIO_PIN(0, 5)
#define GPIO0_6						GPIO_PIN(0, 6)
#define GPIO0_7						GPIO_PIN(0, 7)
#define GPIO0_8						GPIO_PIN(0, 8)
#define GPIO0_9						GPIO_PIN(0, 9)
#define GPIO0_10					GPIO_PIN(0, 10)
#define GPIO0_11					GPIO_PIN(0, 11)
#define GPIO0_12					GPIO_PIN(0, 12)
#define GPIO0_13					GPIO_PIN(0, 13)
#define GPIO0_14					GPIO_PIN(0, 14)
#define GPIO0_15					GPIO_PIN(0, 15)
#define GPIO0_16					GPIO_PIN(0, 16)
#define GPIO0_17					GPIO_PIN(0, 17)
#define GPIO0_18					GPIO_PIN(0, 18)
#define GPIO0_19					GPIO_PIN(0, 19)
#define GPIO0_20					GPIO_PIN(0, 20)
#define GPIO0_21					GPIO_PIN(0, 21)
#define GPIO0_22					GPIO_PIN(0, 22)
#define GPIO0_23					GPIO_PIN(0, 23)
#define GPIO0_24					GPIO_PIN(0, 24)
#define GPIO0_25					GPIO_PIN(0, 25)
#define GPIO0_26					GPIO_PIN(0, 26)
#define GPIO0_27					GPIO_PIN(0, 27)
#define GPIO0_28					GPIO_PIN(0, 28)
#define GPIO0_29					GPIO_PIN(0, 29)
#define GPIO0_30					GPIO_PIN(0, 30)
#define GPIO0_31					GPIO_PIN(0, 31)
/*GPIO_1*/
#define GPIO1_0						GPIO_PIN(1, 0)
#define GPIO1_1						GPIO_PIN(1, 1)
#define GPIO1_2						GPIO_PIN(1, 2)
#define GPIO1_3						GPIO_PIN(1, 3)
#define GPIO1_4						GPIO_PIN(1, 4)
#define GPIO1_5						GPIO_PIN(1, 5)
#define GPIO1_6						GPIO_PIN(1, 6)
#define GPIO1_7						GPIO_PIN(1, 7)
#define GPIO1_8						GPIO_PIN(1, 8)
#define GPIO1_9						GPIO_PIN(1, 9)
#define GPIO1_10					GPIO_PIN(1, 10)
#define GPIO1_11					GPIO_PIN(1, 11)
#define GPIO1_12					GPIO_PIN(1, 12)
#define GPIO1_13					GPIO_PIN(1, 13)
#define GPIO1_14					GPIO_PIN(1, 14)
#define GPIO1_15					GPIO_PIN(1, 15)
#define GPIO1_16					GPIO_PIN(1, 16)
#define GPIO1_17					GPIO_PIN(1, 17)
#define GPIO1_18					GPIO_PIN(1, 18)
#define GPIO1_19					GPIO_PIN(1, 19)
#define GPIO1_20					GPIO_PIN(1, 20)
#define GPIO1_21					GPIO_PIN(1, 21)
#define GPIO1_22					GPIO_PIN(1, 22)
#define GPIO1_23					GPIO_PIN(1, 23)
#define GPIO1_24					GPIO_PIN(1, 24)
#define GPIO1_25					GPIO_PIN(1, 25)
#define GPIO1_26					GPIO_PIN(1, 26)
#define GPIO1_27					GPIO_PIN(1, 27)
#define GPIO1_28					GPIO_PIN(1, 28)
#define GPIO1_29					GPIO_PIN(1, 29)
#define GPIO1_30					GPIO_PIN(1, 30)
#define GPIO1_31					GPIO_PIN(1, 31)
/*GPIO_2*/
#define GPIO2_0						GPIO_PIN(2, 0)
#define GPIO2_1						GPIO_PIN(2, 1)
#define GPIO2_2						GPIO_PIN(2, 2)
#define GPIO2_3						GPIO_PIN(2, 3)
#define GPIO2_4						GPIO_PIN(2, 4)
#define GPIO2_5						GPIO_PIN(2, 5)
#define GPIO2_6						GPIO_PIN(2, 6)
#define GPIO2_7						GPIO_PIN(2, 7)
#define GPIO2_8						GPIO_PIN(2, 8)
#define GPIO2_9						GPIO_PIN(2, 9)
#define GPIO2_10					GPIO_PIN(2, 10)
#define GPIO2_11					GPIO_PIN(2, 11)
#define GPIO2_12					GPIO_PIN(2, 12)
#define GPIO2_13					GPIO_PIN(2, 13)
#define GPIO2_14					GPIO_PIN(2, 14)
#define GPIO2_15					GPIO_PIN(2, 15)
#define GPIO2_16					GPIO_PIN(2, 16)
#define GPIO2_17					GPIO_PIN(2, 17)
#define GPIO2_18					GPIO_PIN(2, 18)
#define GPIO2_19					GPIO_PIN(2, 19)
#define GPIO2_20					GPIO_PIN(2, 20)
#define GPIO2_21					GPIO_PIN(2, 21)
#define GPIO2_22					GPIO_PIN(2, 22)
#define GPIO2_23					GPIO_PIN(2, 23)
#define GPIO2_24					GPIO_PIN(2, 24)
#define GPIO2_25					GPIO_PIN(2, 25)
#define GPIO2_26					GPIO_PIN(2, 26)
#define GPIO2_27					GPIO_PIN(2, 27)
#define GPIO2_28					GPIO_PIN(2, 28)
#define GPIO2_29					GPIO_PIN(2, 29)
#define GPIO2_30					GPIO_PIN(2, 30)
#define GPIO2_31					GPIO_PIN(2, 31)
/*GPIO_3*/
#define GPIO3_0						GPIO_PIN(3, 0)
#define GPIO3_1						GPIO_PIN(3, 1)
#define GPIO3_2						GPIO_PIN(3, 2)
#define GPIO3_3						GPIO_PIN(3, 3)
#define GPIO3_4						GPIO_PIN(3, 4)
#define GPIO3_5						GPIO_PIN(3, 5)
#define GPIO3_6						GPIO_PIN(3, 6)
#define GPIO3_7						GPIO_PIN(3, 7)
#define GPIO3_8						GPIO_PIN(3, 8)
#define GPIO3_9						GPIO_PIN(3, 9)
#define GPIO3_10					GPIO_PIN(3, 10)
#define GPIO3_11					GPIO_PIN(3, 11)
#define GPIO3_12					GPIO_PIN(3, 12)
#define GPIO3_13					GPIO_PIN(3, 13)
#define GPIO3_14					GPIO_PIN(3, 14)
#define GPIO3_15					GPIO_PIN(3, 15)
#define GPIO3_16					GPIO_PIN(3, 16)
#define GPIO3_17					GPIO_PIN(3, 17)
#define GPIO3_18					GPIO_PIN(3, 18)
#define GPIO3_19					GPIO_PIN(3, 19)
#define GPIO3_20					GPIO_PIN(3, 20)
#define GPIO3_21					GPIO_PIN(3, 21)
#define GPIO3_22					GPIO_PIN(3, 22)
#define GPIO3_23					GPIO_PIN(3, 23)
#define GPIO3_24					GPIO_PIN(3, 24)
#define GPIO3_25					GPIO_PIN(3, 25)
#define GPIO3_26					GPIO_PIN(3, 26)
#define GPIO3_27					GPIO_PIN(3, 27)
#define GPIO3_28					GPIO_PIN(3, 28)
#define GPIO3_29					GPIO_PIN(3, 29)
#define GPIO3_30					GPIO_PIN(3, 30)
#define GPIO3_31					GPIO_PIN(3, 31)

#endif

