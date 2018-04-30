/******************************************    *******************************
      > File Name: TimerDef.h
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月15日 星期三 08时45分11秒
      > Modify Time: 
 *********************************    ***************************************/

#ifndef _TIMERDEF_H
#define _TIMERDEF_H

#define DMTIMER0_ADDR					(0x44E05000)					/*DMTimer0 Registers*/
#define DMTIMER1_1MS_ADDR				(0x44E31000)					/*DMTimer1 1ms Registers*/
#define DMTIMER2_ADDR					(0x48040000)					/*DMTimer2 Registers*/
#define DMTIMER3_ADDR					(0x48042000)					/*DMTimer3 Registers*/
#define DMTIMER4_ADDR					(0x48044000)					/*DMTimer4 Registers*/
#define DMTIMER5_ADDR					(0x48046000)					/*DMTimer5 Registers*/
#define DMTIMER6_ADDR					(0x48048000)					/*DMTimer6 Registers*/
#define DMTIMER7_ADDR					(0x4804A000)					/*DMTimer7 Registers*/

#define TIDR_ADDR_OFFSET				(0x0)							/*Identification Register
																		  鉴定寄存器*/
#define TIOCP_CFG_ADDR_OFFSET			(0x10)							/*Timer OCP Configuration Register
																		  定时器OCP配置寄存器*/
/*
 * Bit		Field				Type	Reset	Description
 * 31-4		RESERVED			R		0h
 * 3-2		IDLEMODE			R/W		0h		Power management, req/ack control
 *												电源管理，请求/确认控制
 *												0h = Force-idle mode: local target's idle state follows (acknowledges)
 *												the system's idle requests unconditionally, i.e. regardless of the IP
 *												module's internal requirements. Backup mode, for debug only.
 *												0h =强制空闲模式：本地目标的空闲状态无条件地跟随（确认）系统的空闲请求，
 *												即与IP模块的内部要求无关。 备份模式，仅用于调试。
 *												1h = No-idle mode: local target never enters idle state. Backup
 *												mode, for debug only.
 *												1h =无空闲模式：本地目标永不进入空闲状态。 备份模式，仅用于调试。
 *												2h = Smart-idle mode: local target's idle state eventually follows
 *												(acknowledges) the system's idle requests, depending on the IP
 *												module's internal requirements. IP module shall not generate (IRQor
 *												DMA-request-related) wakeup events.
 *												2h =智能空闲模式：根据IP模块的内部要求，
 *												本地目标的空闲状态最终跟随（确认）系统的空闲请求。 
 *												IP模块不会产生（IRQ或DMA请求相关的）唤醒事件。
 *												3h = Smart-idle wakeup-capable mode: local target's idle state
 *												eventually follows (acknowledges) the system's idle requests,
 *												depending on the IP module's internal requirements. IP module may
 *												generate (IRQ- or DMA-request-related) wakeup events when in idle
 *												state. Only available for Timer0. Not available for Timer2-7
 *												3h =智能空闲唤醒功能模式：根据IP模块的内部要求，
 *												本地目标的空闲状态最终会跟随（确认）系统的空闲请求。 
 *												处于空闲状态时，IP模块可能会产生（IRQ或DMA请求相关的）唤醒事件。 
 *												只适用于Timer0。 不适用于Timer2-7
 * 1		EMUFREE				R/W		0h		Sensitivity to emulation (debug) suspend event from Debug Subsystem.
 *												仿真灵敏度（调试）从调试子系统暂停事件。
 *												0h = The timer is frozen during a debug suspend event.
 *												0h =定时器在调试暂停事件期间被冻结。
 *												1h = The timer runs free, and Debug suspend event is ignored.
 *												1h =定时器自由运行，Debug suspend事件被忽略。
 * 0		SOFTRESET			R/W		0h		Software reset.
 *												软件复位
 *												0h = No action.
 *												0h =没有行动。
 *												1h = Reset ongoing.
 *												1h =重置正在进行。
 * */
#define IRQ_EOI_ADDR_OFFSET				(0x20)							/*Timer IRQ End-of-Interrupt Register
																		  定时器IRQ中断结束寄存器*/
#define IRQSTATUS_RAW_ADDR_OFFSET		(0x24)							/*Timer Status Raw Register
																		  定时器状态原始寄存器*/
#define IRQSTATUS_ADDR_OFFSET			(0x28)							/*Timer Status Register
																		  定时器状态寄存器*/
#define IRQENABLE_SET_ADDR_OFFSET		(0x2C)							/*Timer Interrupt Enable Set Register
																		  定时器中断使能设置寄存器*/
#define IRQENABLE_CLR_ADDR_OFFSET		(0x30)							/*Timer Interrupt Enable Clear Register
																		  定时器中断使能清除寄存器*/
#define IRQWAKEEN_ADDR_OFFSET			(0x34)							/*Timer IRQ Wakeup Enable Register
																		  定时器IRQ唤醒使能寄存器*/
#define TCLR_ADDR_OFFSET				(0x38)							/*Timer Control Register
																		  定时器控制寄存器*/
/*
 * 当TCM字段从（00）传递到任何其他组合时，则清除TCAR_IT_FLAG和边沿检测逻辑。 
 * TCLR寄存器的ST位可能由OCP接口更新或由于单次溢出而复位。 OCP接口更新具有优先级
 * Bit		Field				Type	Reset	Description
 * 31-15	RESERVED			R		0h
 * 14		GPO_CFG				R/W		0h		General purpose output this register drives directly the
 *												PORGPOCFG output pin
 *												通用输出寄存器直接驱动PORGPOCFG输出引脚
 *												0h = PORGPOCFG drives 0 and configures the timer pin as an output.
 *												0h = PORGPOCFG驱动0，并将定时器引脚配置为输出。
 *												1h = PORGPOCFG drives 1 and configures the timer pin as an input.
 *												1h = PORGPOCFG驱动1并将定时器引脚配置为输入。
 * 13		CAPT_MODE			R/W		0h		Capture mode.
 *												捕获模式
 *												0h = Single capture
 *												0h =单次捕获
 *												1h = Capture on second event
 *												1h = 第二个事件捕获
 * 12		PT					R/W		0h		Pulse or toggle mode on PORTIMERPWM output pin
 *												PORTIMERPWM输出引脚上的脉冲或触发模式
 *												0h = Pulse
 *												0h =脉冲
 *												1h = Toggle
 *												1h =切换
 * 11-10	TRG					R/W		0h		Trigger output mode on PORTIMERPWM output pin
 *												在PORTIMERPWM输出引脚上触发输出模式
 *												0h = No trigger
 *												0h =没有触发
 *												1h = Trigger on overflow
 *												1h =在溢出时触发
 *												2h = Trigger on overflow and match
 *												2h =触发溢出和匹配
 *												3h = Reserved
 *												3h =保留
 * 9-8		TCM					R/W		0h		Transition Capture Mode on PIEVENTCAPT input pin
 *												PIEVENTCAPT输入引脚上的转换捕捉模式
 *												0h = No capture
 *												0h =没有捕获
 *												1h = Capture on low to high transition
 *												1h =捕获从低到高转换
 *												2h = Capture on high to low transition
 *												2h =捕捉从高到低的过渡
 *												3h = Capture on both edge transition
 *												3h =在两个边沿转换上捕获
 * 7		SCPWM				R/W		0h		This bit should be set or clear while the timer is stopped or the
 *												trigger is off
 *												当定时器停止或触发器关闭时，该位应置1或清零
 *												0h = Clear the PORTIMERPWM output pin and select positive pulse for pulse mode
 *												0h =清除PORTIMERPWM输出引脚，选择脉冲模式的正脉冲
 *												1h = Set the PORTIMERPWM output pin and select negative pulse for pulse mode
 *												1h =设置PORTIMERPWM输出引脚，并为脉冲模式选择负脉冲
 * 6		CE					R/W		0h		0h = Compare mode is disabled
 *												0h =比较模式被禁用
 *												1h = Compare mode is enabled
 *												1h =比较模式启用
 * 5		PRE					R/W		0h		Prescaler enable
 *												预分频器启用
 *												0h = The TIMER clock input pin clocks the counter
 *												0h = TIMER时钟输入引脚为计数器提供时钟
 *												1h = The divided input pin clocks the counter
 *												1h =分频输入引脚为计数器提供时钟
 * 4-2		PTV					R/W		0h		Pre-scale clock Timer value
 *												预缩放时钟定时器值
 * 1		AR					R/W		0h		0h = One shot timer
 *												0h =一次性计时器
 *												1h = Auto-reload timer
 *												1h =自动重新加载定时器
 * 0		ST					R/W		0h		In the case of one-shot mode selected (AR = 0), this bit is
 *												automatically reset by internal logic when the counter is overflowed.
 *												在选择单次模式（AR = 0）的情况下，当计数器溢出时，该位由内部逻辑自动复位。
 *												0h (R) = Stop timeOnly the counter is frozen
*												0h（R）=停止时间只有计数器被冻结
 *												1h = Start timer
 *												1h =启动计时器
 * */
#define TCRR_ADDR_OFFSET				(0x3C)							/*Timer Counter Register
																		  定时器计数器寄存器*/
#define TLDR_ADDR_OFFSET				(0x40)							/*Timer Load Register
																		  定时器装载寄存器*/
/*
 * 31-0		LOAD_VALUE				R/W		0h	Timer counter value loaded on overflow in auto-reload mode or on
 *												TTGR write access
 *												定时器计数器值在自动重载模式或TTGR写入访问时被装入溢出
 *
 * */
#define TTGR_ADDR_OFFSET				(0x44)							/*Timer Trigger Register
																		  定时器触发寄存器*/
#define TWPS_ADDR_OFFSET				(0x48)							/*Timer Write Posting Bits Register
																		  定时器写入位寄存器*/
#define TMAR_ADDR_OFFSET				(0x4C)							/*Timer Match Register
																		  定时器匹配寄存器*/
#define TCAR1_ADDR_OFFSET				(0x50)							/*Timer Capture Register
																		  定时器捕获寄存器1*/
#define TSICR_ADDR_OFFSET				(0x54)							/*Timer Synchronous Interface Control Register
																		  定时器同步接口控制寄存器*/
#define TCAR2_ADDR_OFFSET				(0x58)							/*Timer Capture Register定时器捕获寄存器2*/

#endif

