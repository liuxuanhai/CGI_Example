/******************************************    *******************************
  > File Name: PwmDef.h
  > Author: Ywl
  > Descripsion:
  > Created Time:     2017年10月30日 星期一 11时02分54秒
  > Modify Time: 
 *********************************    ***************************************/

#ifndef _PWM_PARA_CONF_H
#define _PWM_PARA_CONF_H


/*addr*/
#define PWM_BASE_ADDR					(0x4830000)		

#define PWMSS0_ADDR						(PWM_BASE_ADDR + 0x0000)			/*PWMSS0 Configuration Registers,~0x483000FF*/
#define PWMSS1_ADDR						(PWM_BASE_ADDR + 0x0200)			/*PWMSS1 Configuration Registers,~0x483020FF*/
#define PWMSS2_ADDR						(PWM_BASE_ADDR + 0x0400)			/*PWMSS2 Configuration Registers,~0x483040FF*/

/* 脉冲宽度调制子系统（PWMSS）包括
 * 1.增强型高级单个实例分辨率脉宽调制器（eHRPWM）
 * 2.增强型捕捉（eCAP）和增强型正交
 * 3.编码脉冲（eQEP）模块
 * */
#define ECAP0_ADDR						(PWMSS0_ADDR + 0x0100)		/*~0x017F*/
#define EQEP0_ADDR						(PWMSS0_ADDR + 0x0180)		/*~0x01FF*/
#define EPWM0_ADDR						(PWMSS0_ADDR + 0x0200)		/*~0x025F*/

#define ECAP1_ADDR						(PWMSS1_ADDR + 0x0100)		/*~0x217F*/
#define EQEP1_ADDR						(PWMSS1_ADDR + 0x0180)		/*~0x21FF*/
#define EPWM1_ADDR						(PWMSS1_ADDR + 0x0200)		/*~0x225F*/

#define ECAP2_ADDR						(PWMSS2_ADDR + 0x0100)		/*~0x47F*/
#define EQEP2_ADDR						(PWMSS2_ADDR + 0x0180)		/*~0x41FF*/
#define EPWM2_ADDR						(PWMSS2_ADDR + 0x0200)		/*~0x425F*/

/*PWMSS配置寄存器*/
#define PWMSS_IDVER_ADDR_OFFSET			(0x0)			/*由软件用于跟踪功能，错误和兼容性*/
#define PWMSS_SYSCONFIG_ADDR_OFFSET		(0x4)			/*时钟管理配置*/
#define PWMSS_CLKCONFIG_ADDR_OFFSET		(0x8)			/*PWMSS子模块clkstop req和clk_en控件*/
#define PWMSS_CLKSTATUS_ADDR_OFFSET		(0xC)			/*PWMSS子模块clkstop ack和clk_en ack状态*/

/*ECAP Registers*/
#define TSCTR_ADDR_OFFSET				(0x0)			/*Time-Stamp Counter Register*/
#define CTRPHS_ADDR_OFFSET				(0x4)			/*Counter Phase Offset Value Register*/
#define CAP1_ADDR_OFFSET				(0x8)			/*Capture 1 Register*/
#define CAP2_ADDR_OFFSET				(0xC)			/*Capture 2 Register*/
#define CAP3_ADDR_OFFSET				(0x10)			/*Capture 3 Register*/
#define CAP4_ADDR_OFFSET				(0x14)			/*Capture 4 Register*/
#define ECCTL1_ADDR_OFFSET				(0x28)			/*Capture Control Register 1*/
#define ECCTL2_ADDR_OFFSET				(0x2A)			/*Capture Control Register 2*/
#define ECEINT_ADDR_OFFSET				(0x2C)			/*Capture Interrupt Enable Register*/
#define ECFLG_ADDR_OFFSET				(0x2E)			/*Capture Interrupt Flag Register*/
#define ECCLR_ADDR_OFFSET				(0x30)			/*Capture Interrupt Clear Register*/
#define ECFRC_ADDR_OFFSET				(0x32)			/*Capture Interrupt Force Register*/
#define REVID_ADDR_OFFSET				(0x5C)			/*Revision ID Register*/

//EQEP Registers
#define QPOSCNT_ADDR_OFFSET				(0x0)			/*eQEP Position Counter Register*/
#define QPOSINIT_ADDR_OFFSET			(0x4)			/*eQEP Position Counter Initialization Register*/
#define QPOSMAX_ADDR_OFFSET				(0x8)			/*eQEP Maximum Position Count Register*/
#define QPOSCMP_ADDR_OFFSET				(0xC)			/*eQEP Position-Compare Register*/
#define QPOSILAT_ADDR_OFFSET			(0x10)			/*eQEP Index Position Latch Register*/
#define QPOSSLAT_ADDR_OFFSET			(0x14)			/*eQEP Strobe Position Latch Register*/
#define QPOSLAT_ADDR_OFFSET				(0x18)			/*eQEP Position Counter Latch Register*/
#define QUTMR_ADDR_OFFSET				(0x1C)			/*eQEP Unit Timer Register*/
#define QUPRD_ADDR_OFFSET				(0x20)			/*eQEP Unit Period Register*/
#define QWDTMR_ADDR_OFFSET				(0x24)			/*eQEP Watchdog Timer Register*/
#define QWDPRD_ADDR_OFFSET				(0x26)			/*eQEP Watchdog Period Register*/
#define QDECCTL_ADDR_OFFSET				(0x28)			/*eQEP Decoder Control Register*/
#define QEPCTL_ADDR_OFFSET				(0x2A)			/*eQEP Control Register*/
#define QCAPCTL_ADDR_OFFSET				(0x2C)			/*eQEP Capture Control Register*/
#define QPOSCTL_ADDR_OFFSET				(0x2E)			/*eQEP Position-Compare Control Register*/
#define QEINT_ADDR_OFFSET				(0x30)			/*eQEP Interrupt Enable Register*/
#define QFLG_ADDR_OFFSET				(0x32)			/*eQEP Interrupt Flag Register*/
#define QCLR_ADDR_OFFSET				(0x34)			/*eQEP Interrupt Clear Register*/
#define QFRC_ADDR_OFFSET				(0x36)			/*eQEP Interrupt Force Register*/
#define QEPSTS_ADDR_OFFSET				(0x38)			/*eQEP Status Register*/
#define QCTMR_ADDR_OFFSET				(0x3A)			/*eQEP Capture Timer Register*/
#define QCPRD_ADDR_OFFSET				(0x3C)			/*eQEP Capture Period Register*/
#define QCTMRLAT_ADDR_OFFSET			(0x3E)			/*eQEP Capture Timer Latch Register*/
#define QCPRDLAT_ADDR_OFFSET			(0x40)			/*eQEP Capture Period Latch Register*/
#define REVID_ADDR_OFFSET				(0x5C)			/*eQEP Revision ID Register*/


//EPWM Registers
#define TBCTL_ADDR_OFFSET				(0x0)			/*Time-Base Control Register,时基控制寄存器*/
/*
 * Bit		Field				Type	Reset	Description
 * 15-14	FREE_SOFT			R/W		0h		Emulation Mode Bits.
 *												仿真模式位。
 *												These bits select the behavior of the ePWM time-base counter
 *												during emulation suspend events.
 *												这些位在仿真暂停事件期间选择ePWM时基计数器的行为。
 *												Emulation debug events can be set up in the Debug Subsystem.
 *												仿真调试事件可以在调试子系统中设置。
 *												0h (R/W) = Stop after the next time-base counter increment or
 *												decrement
 *												0h（R / W）=下一个时基计数器递增或递减后停止
 *												1h (R/W) = Stop when counter completes a whole cycle. 
 *												(a) Upcount mode - stop when the time-base counter = period (TBCNT = TBPRD). 
 *												(b) Down-count mode - stop when the time-base counter 0000 (TBCNT = 0000h). 
 *												(c) Up-down-count mode - stop when the time-base counter = 0000 (TBCNT = 0000h).
 *												1h（R / W）=计数器完成整个周期后停止。 
 *												（a）加计数模式 - 当时基计数器=周期（TBCNT = TBPRD）时停止。 
 *												（b）减计数模式 - 当时基计数器= 0000（TBCNT = 0000h）时停止。 
 *												（c）加减计数模式 - 当时基计数器= 0000（TBCNT = 0000h）时停止。
 *												2h (R/W) = Free run
 *												2h（R / W）=自由运行
 *												3h (R/W) = Free run
 *												3h（R / W）=自由运行
 * 13		PHSDIR				R/W		0h		Phase Direction Bit.
 *												相位方向位。
 *												This bit is only used when the time-base counter is configured in the
 *												up-down-count mode.
 *												该位仅在时基计数器配置为加减计数模式时使用。
 *												The PHSDIR bit indicates the direction the time-base counter
 *												(TBCNT) will count after a synchronization event occurs and a new
 *												phase value is loaded from the phase (TBPHS) register.
 *												PHSDIR位指示时基计数器（TBCNT）在发生同步事件并从相位（TBPHS）
 *												寄存器加载新的相位值时计数的方向。
 *												This is irrespective of the direction of the counter before the
 *												synchronization event..
 *												这与同步事件之前计数器的方向无关。
 *												In the up-count and down-count modes this bit is ignored.
 *												在加计数和减计数模式下，该位被忽略。
 *												0h (R/W) = Count down after the synchronization event.
 *												0h（R / W）=在同步事件之后倒计时。
 *												1h (R/W) = Count up after the synchronization event.
 *												1h（R / W）=在同步事件之后计数。
 * 12-10	CLKDIV				R/W		0h		Time-base Clock Prescale Bits.
 *												时基时钟预分频比特。
 *												These bits determine part of the time-base clock prescale value.
 *												这些位决定了时基预分频值的一部分。
 *												TBCLK = SYSCLKOUT/(HSPCLKDIV * CLKDIV)
 *												0h (R/W) = /1 (default on reset)
 *												1h (R/W) = /2
 *												2h (R/W) = /4
 *												3h (R/W) = /8
 *												4h (R/W) = /16
 *												5h (R/W) = /32
 *												6h (R/W) = /64
 *												7h (R/W) = /128
 * 9-7		HSPCLKDIV			R/W		0h		High-Speed Time-base Clock Prescale Bits.
 *												高速时基时钟预分频比特。
 *												These bits determine part of the time-base clock prescale value.
 *												这些位决定了时基预分频值的一部分。
 *												TBCLK = SYSCLKOUT/(HSPCLKDIV * CLKDIV).
 *												This divisor emulates the HSPCLK in the TMS320x281x system as
 *												used on the Event Manager (EV) peripheral.
 *												这个除数仿真了事件管理器（EV）外设上使用的TMS320x281x系统中的HSPCLK。
 *												0h (R/W) = /1
 *												1h (R/W) = /2 (default on reset)
 *												2h (R/W) = /4
 *												3h (R/W) = /6
 *												4h (R/W) = /8
 *												5h (R/W) = /10
 *												6h (R/W) = /12
 *												7h (R/W) = /14
 * 6		SWFSYNC				R/W		0h		Software Forced Synchronization Pulse.
 *												软件强制同步脉冲。
 *												0h (R/W) = Writing a 0 has no effect and reads always return a 0.
 *												0h（R / W）=写入0无效，读取总是返回0。
 *												1h (R/W) = Writing a 1 forces a one-time synchronization pulse to be
 *												generated. This event is ORed with the EPWMxSYNCI input of the
 *												ePWM module. SWFSYNC is valid (operates) only when
 *												EPWMxSYNCI is selected by SYNCOSEL = 00.
 *												1h（R / W）=写入1会强制产生一次性同步脉冲。 
 *												该事件与ePWM模块的EPWMxSYNCI输入进行或运算。 
 *												仅当通过SYNCOSEL = 00选择EPWMxSYNCI时，SWFSYNC才有效（操作）。
 * 5-4		SYNCOSEL			R/W		0h		Synchronization Output Select.
 *												同步输出选择。
 *												These bits select the source of the EPWMxSYNCO signal.
 *												这些位选择EPWMxSYNCO信号的来源。
 *												0h (R/W) = EPWMxSYNC:
 *												1h (R/W) = CTR = 0 - Time-base counter equal to zero (TBCNT = 0000h)
 *												1h（R / W）= CTR = 0 - 时基计数器等于零（TBCNT = 0000h）
 *												2h (R/W) = CTR = CMPB - Time-base counter equal to counter-compare 
 *												B (TBCNT = CMPB)
 *												2h（R / W）= CTR = CMPB - 时基计数器等于计数器比较B（TBCNT = CMPB）
 *												3h (R/W) = Disable EPWMxSYNCO signal
 *												3h（R / W）=禁止EPWMxSYNCO信号
 * 3		PRDLD				R/W		0h		Active Period Register Load From Shadow Register Select
 *												有效期寄存器从影子寄存器选择加载
 *												0h (R/W) = The period register (TBPRD) is loaded from its shadow
 *												register when the time-base counter, TBCNT, is equal to zero. A
 *												write or read to the TBPRD register accesses the shadow register.
 *												0h（R / W）=当时基计数器TBCNT等于零时，周期寄存器（TBPRD）从其影子寄存器加载。 
 *												写或读TBPRD寄存器访问影子寄存器。
 *												1h (R/W) = Load the TBPRD register immediately without using a
 *												shadow register. A write or read to the TBPRD register directly
 *												accesses the active register.
 *												1h（R / W）=立即加载TBPRD寄存器，无需使用影子寄存器。 
 *												写或读TBPRD寄存器直接访问活动寄存器。
 * 2		PHSEN				R/W		0h		Counter Register Load From Phase Register Enable
 *												计数器寄存器加载从相位寄存器启用
 *												0h (R/W) = Do not load the time-base counter (TBCNT) from the
 *												time-base phase register (TBPHS)
 *												0h（R / W）=不加载来自时基相位寄存器（TBPHS）的时基计数器（TBCNT）
 *												1h (R/W) = Load the time-base counter with the phase register when
 *												an EPWMxSYNCI input signal occurs or when a software
 *												synchronization is forced by the SWFSYNC bit.
 *												1h（R / W）=当EPWMxSYNCI输入信号发生时或软件同步由SWFSYNC位强制时，
 *												将相位寄存器加载到时基计数器。
 * 1-0		CTRMODE				R/W		0h		Counter Mode.
 *												计数器模式
 *												The time-base counter mode is normally configured once and not
 *												changed during normal operation.
 *												时基计数器模式通常配置一次，在正常操作期间不会改变。
 *												If you change the mode of the counter, the change will take effect at
 *												the next TBCLK edge and the current counter value shall increment
 *												or decrement from the value before the mode change.
 *												如果更改计数器的模式，则更改将在下一个TBCLK边沿生效，
 *												并且当前计数器值将从模式更改前的值开始递增或递减。
 *												These bits set the time-base counter mode of operation as follows:
 *												这些位将时基计数器的操作模式设置如下：
 *												0h (R/W) = Up-count mode	加法计数器模式
 *												1h (R/W) = Down-count mode	减法计数器模式
 *												2h (R/W) = Up-down-count mode	加减计数器模式
 *												3h (R/W) = Stop-freeze counter operation (default on reset)
 *												停止计数器操作（复位时的默认值）
 * */
#define TBSTS_ADDR_OFFSET				(0x2)			/*Time-Base Status Register
														  时基状态寄存器*/
#define TBPHSHR_ADDR_OFFSE				(0x4)			/*Extension for HRPWM Phase Register
														  HRPWM相位扩展寄存器*/
#define TBPHS_ADDR_OFFSET				(0x6)			/*Time-Base Phase Register
														  时基相位寄存器*/
#define TBCNT_ADDR_OFFSET				(0x8)			/*Time-Base Counter Register
														  时基计数寄存器*/
#define TBPRD_ADDR_OFFSET				(0xA)			/*Time-Base Period Register
														  时基预分频寄存器*/
#define CMPCTL_ADDR_OFFSET				(0xE)			/*Counter-Compare Control Register
														  计数器比较控制寄存器*/
#define CMPAHR_ADDR_OFFSET				(0x10)			/*Extension for HRPWM Counter-Compare A Register
														  HRPWM计数器比较扩展寄存器*/
#define CMPA_ADDR_OFFSET				(0x12)			/*Counter-Compare A Register
														  计数器比较A寄存器*/
#define CMPB_ADDR_OFFSET				(0x14)			/*Counter-Compare B Register, 
														  计数器比较A寄存器*/
#define AQCTLA_ADDR_OFFSET				(0x16)			/*Action-Qualifier Control Register for Output A (EPWMxA)
														  用于输出A的动作限定符控制寄存器（EPWMxA）*/
#define AQCTLB_ADDR_OFFSET				(0x18)			/*Action-Qualifier Control Register for Output B (EPWMxB)
														  用于输出B的动作限定符控制寄存器（EPWMxB）*/
#define AQSFRC_ADDR_OFFSET				(0x1A)			/*Action-Qualifier Software Force Register
														  动作限定符软件强制寄存器*/
#define AQCSFRC_ADDR_OFFSET				(0x1C)			/*Action-Qualifier Continuous S/W Force Register Set
														  动作限定器连续软件强制寄存器组*/
#define DBCTL_ADDR_OFFSET				(0x1E)			/*Dead-Band Generator Control Register
														  死区发生器控制寄存器*/
#define DBRED_ADDR_OFFSET				(0x20)			/*Dead-Band Generator Rising Edge Delay Count Register
														  死区发生器上升沿延时计数寄存器*/
#define DBFED_ADDR_OFFSET				(0x22)			/*Dead-Band Generator Falling Edge Delay Count Register
														  死区发生器下降沿延时计数寄存器*/
#define TZSEL_ADDR_OFFSET				(0x24)			/*Trip-Zone Select Register
														  错误输入选择寄存器*/
#define TZCTL_ADDR_OFFSET				(0x28)			/*Trip-Zone Control Register
														  错误输入域控制寄存器*/
#define TZEINT_ADDR_OFFSET				(0x2A)			/*Trip-Zone Enable Interrupt Register
														  错误输入使能中断寄存器*/
#define TZFLG_ADDR_OFFSET				(0x2C)			/*Trip-Zone Flag Register
														  错误输入标志寄存器*/
#define TZCLR_ADDR_OFFSET				(0x2E)			/*Trip-Zone Clear Register
														  错误输入清除寄存器*/
#define TZFRC_ADDR_OFFSET				(0x30)			/*Trip-Zone Force Register
														  错误输入强制寄存器*/
#define ETSEL_ADDR_OFFSET				(0x32)			/*Event-Trigger Selection Register
														  事件触发器选择寄存器*/
#define ETPS_ADDR_OFFSET				(0x34)			/*Event-Trigger Pre-Scale Register
														  事件触发器预分频寄存器*/
#define ETFLG_ADDR_OFFSET				(0x36)			/*Event-Trigger Flag Register
														  事件触发标志寄存器*/
#define ETCLR_ADDR_OFFSET				(0x38)			/*Event-Trigger Clear Register
														  事件触发清除寄存器*/
#define ETFRC_ADDR_OFFSET				(0x3A)			/*Event-Trigger Force Register
														  事件触发强制寄存器*/
#define PCCTL_ADDR_OFFSET				(0x3C)			/*PWM-Chopper Control Register
														  PWM斩波控制寄存器*/
#define HRCNFG_ADDR_OFFSET				(0xC0)			/*HRPWM configuration register (HRCNFG)
														  HRPWM配置寄存器（HRCNFG）*/

/*
 * 软件使用IP修订注册表来跟踪功能，错误和兼容性。(IDVER)
 * 31-30	SCHEME		1		Used to distinguish between the old scheme and current.(用于区分旧方案和电流)
 * 29-28	Reserved	0		Read returns 0.
 * 27-16	FUNC		0
 * 15-11	R_RTL		0		RTL version (R), maintained by IP design owner.(RTL版本（R），由IP设计所有者维护。)
 * 10-8		X_MAJOR		0		Major revision (X), maintained by IP specification owner.(主要修订（X），由IP规范所有者维护。)
 * 7-6		CUSTOM		0
 * 5-0		Y_MINOR		0		Minor revision (Y), maintained by IP specification owner.(次要版本（Y），由IP规范所有者维护。)
 * 系统配置寄存器用于时钟管理配置。(SYSCONFIG)
 * 31-6		Reserved	0		Read returns 0.
 * 5-4		STANDBYMODE			Configuration of the local initiator state management mode. By definition, initiator may
 *								generate read/write transaction as long as it is out of STANDBY state.
 *								0x0 = Force-standby mode: local initiator is unconditionally placed in standby state.
 *								Backup mode, for debug only.
 *								0x1 = No-standby mode: local initiator is unconditionally placed out of standby state.
 *								Backup mode, for debug only.
 *								0x2 = Smart-standby mode: local initiator standby status depends on local conditions, i.e.,
 *								the module's functional requirement from the initiator. IP module should not generate
 *								(initiator-related) wakeup events.
 *								0x3 = Smart-Standby wakeup-capable mode: local initiator standby status depends on
 *								local conditions, i.e., the module's functional requirement from the initiator. IP module may
 *								generate (master-related) wakeup events when in standby state. Mode is only relevant if
 *								the appropriate IP module "mwakeup" output is implemented
 *								配置本地启动器状态管理模式。 根据定义，启动器可以
 *								只要它处于STANDBY状态，就生成读/写事务。
 *								0x0 =强制待机模式：本地启动器无条件置于待机状态。
 *								备份模式，仅用于调试。
 *								0x1 =无备用模式：本地启动器无条件置于待机状态。
 *								备份模式，仅用于调试。
 *								0x2 =智能待机模式：本地启动器待机状态取决于本地条件，即，
 *								该模块的功能需求来自启动器。 IP模块不应该生成
 *								（启动器相关）唤醒事件。
 *								0x3 =智能备用唤醒功能模式：本地启动器待机状态取决于
 *								本地条件，即来自发起者的模块功能需求。 IP模块可能
 *								在待机状态下生成（主相关）唤醒事件。 模式只有相关的
 *								实现相应的IP模块“mwakeup”输出
 * 3-2		IDLEMODE			Configuration of the local target state management mode. By definition, target can handle
 *								read/write transaction as long as it is out of IDLE state.
 *								0x0 = Force-idle mode: local target's idle state follows (acknowledges) the system's idle
 *								requests unconditionally, i.e. regardless of the IP module's internal requirements. Backup
 *								mode, for debug only.
 *								0x1 = No-idle mode: local target never enters idle state. Backup mode, for debug only.
 *								0x2 = Smart-idle mode: local target's idle state eventually follows (acknowledges) the
 *								system's idle requests, depending on the IP module's internal requirements. IP module
 *								shall not generate (IRQ- or DMA-request-related) wakeup events.
 *								0x3 = Smart-idle wakeup-capable mode: local target's idle state eventually follows
 *								(acknowledges) the system's idle requests, depending on the IP module's internal
 *								requirements. IP module may generate (IRQ- or DMA-request-related) wakeup events
 *								when in idle state. Mode is only relevant if the appropriate IP module "swakeup" output(s)
 *								is (are) implemented.
 *								配置本地目标状态管理模式。根据定义，目标可以处理
 *								只要它处于空闲状态，只要读/写事务。
 *								0x0 =强制空闲模式：本地目标的空闲状态跟随（确认）系统的空闲状态
 *								无条件地请求，即不管IP模块的内部要求如何。备用
 *								模式，仅用于调试。
 *								0x1 =无空闲模式：本地目标不进入空闲状态。备份模式，仅用于调试。
 *								0x2 =智能空闲模式：本地目标的空闲状态最终跟随（确认）
 *								系统的空闲请求，取决于IP模块的内部要求。 IP模块
 *								不会产生（IRQ或DMA请求相关）唤醒事件。
 *								0x3 =智能空闲唤醒功能模式：本地目标的空闲状态最终如下
 *								（确认）系统的空闲请求，具体取决于IP模块的内部
 *								要求。 IP模块可能产生（与IRQ或DMA请求有关）的唤醒事件
 *								处于空闲状态。模式仅在相应的IP模块“swakeup”输出
 *								已实施。
 * 1		FREEEMU				Sensitivity to emulation (debug) suspend input signal.
 *								0 = IP module is sensitive to emulation suspend.
 *								1 = IP module is not sensitive to emulation suspend.
 *								对仿真灵敏度（调试）挂起输入信号。
 *								0 = IP模块对仿真挂起敏感。
 *								1 = IP模块对仿真暂停不敏感。
 * 0		SOFTRESET	0		Software reset (optional)
 * 时钟配置寄存器用于clkstop req和clk_en控制的PWMSS子模块。(CLKCONFIG)	
 * 31-10	Reserved	0		Read returns 0.
 * 9		ePWM		0		This bit controls the clkstop_req input to the ePWM module.
 *			CLKSTOP_REQ			该位控制ePWM模块的clkstop_req输入。
 * 8		ePWM CLK_EN 1		This bit controls the clk_en input to the ePWM module.该位控制ePWM模块的clk_en输入
 * 7-6		Reserved	0		Read returns 0.
 * 5		eQEP		0		This bit controls the clkstop_req input to the eQEP module
 *			CLKSTOP_REQ			该位控制对eQEP模块的clkstop_req输入
 * 4		eQEP CLK_EN 1		This bit controls the clk_en input to the eQEP module.该位控制到eQEP模块的clk_en输入
 * 3-2		Reserved	0		Read returns 0.
 * 1		eCAP		0		This bit controls the clkstop_req input to the eCAP module.
 *			CLKSTOP_REQ			该位控制eCAP模块的clkstop_req输入
 * 0		eCAP CLK_EN 1		This bit controls the clk_en input to the eCAP module.该位控制eCAP模块的clk_en输入
 * The clock status register is used in the PWMSS submodule for clkstop ack and clk_en ack status.
 * 时钟状态寄存器在PWMSS子模块中用于clkstop ack和clk_en ack状态.(CLKSTATUS)
 * 31-10	Reserved	0		Read returns 0.
 * 9		ePWM		0		This bit is the clkstop_req_ack status output of the ePWM module.
 *			CLKSTOP_ACK			该位是ePWM模块的clkstop_req_ack状态输出
 * 8		ePWM		0		This bit is the clk_en status output of the ePWM module.
 *			CLK_EN_ACK			该位是ePWM模块的clk_en状态输出
 * 7-6		Reserved	0		Read returns 0.
 * 5		eQEP		0		This bit is the clkstop_req_ack status output of the eQEP module.
 *			CLKSTOP_ACK			该位是eQEP模块的clkstop_req_ack状态输出
 * 4		eQEP		0		This bit is the clk_en status output of the eQEP module.
 *			CLK_EN_ACK			该位是eQEP模块的clk_en状态输出
 * 3-2		Reserved	0		Read returns 0.
 * 1		eCAP		0		This bit is the clkstop_req_ack status output of the eCAP module.
 *			CLKSTOP_ACK			该位是eCAP模块的clkstop_req_ack状态输出
 * 0		eCAP		0		This bit is the clk_en status output of the eCAP module.
 *			CLK_EN_ACK			该位是eCAP模块的clk_en状态输出
 *
 * */

/*PWMSSS引脚列表
 * Pin					Type*			Description
 * EPWMxA				O				PWM output A
 * EPWMxB				O				PWM output B
 * EPWM_SYNCIN			I				PWM Sync input
 * EPWM_SYNCOUT			O				PWM Sync output
 * EPWM_TRIPZONE[5:0]	I				PWM Tripzone inputs
 * ECAP_CAPIN_APWMOUT	I/O				eCAP Capture input / PWM output
 * EQEP_A				I/O				eQEP Quadrature input/output
 * EQEP_B				I/O				eQEP Quadrature input/output
 * EQEP_INDEX			I/O				eQEP Index input/output
 * EQEP_STROBE			I/O				eQEP Strobe input/output
 *
 * */

/*********************************************************************************************
 * 每个ePWM模块都有自己的时基子模块来确定所有的事件时序
 * ePWM模块。 内置同步逻辑允许多个ePWM模块的时基工作
 * 一起作为一个单一的系统。
 * ***************************************************************************************/

/***********************************************************************************************
 * •PWM输出信号（EPWMxA和EPWMxB）。								PWM输出信号可用
 *																通过GPIO外设在设备外部描述的系统控制和中断指南
 *																为您的设备。
 * •错误输入信号（TZ1 to TZn）。									这些输入信号警告ePWM模块外部故障
 *																条件。设备上的每个模块都可以配置为使用或忽略任何错误输入域
 *																信号。错误输入信号可以通过GPIO外设配置为异步输入。
 *																请参见第15.1.2节，以确定器件中有多少错误输入域引脚。
 * •时基同步输入（EPWMxSYNCI）和输出（EPWMxSYNCO）信号。		该同步信号将ePWM模块菊花链连接在一起。每个模块都可以配置
 *																使用或忽略其同步输入。时钟同步输入和输出信号为
 *																仅用于ePWM1（ePWM模块＃1）的引脚。  ePWM1的同步输出
 *																（EPWM1SYNCO）也连接到第一个增强型捕获模块（eCAP1）的SYNCI。
 * •外围总线.													外设总线为32位宽，允许16位和32位写入ePWM注册文件。
 * ****************************************************************************************/

/***********************************************************************************************
 * 每个ePWM外设都包含七个子模块。 有些实例包括高分辨率子模块，
 * 可以更精确地控制PWM输出。 这些子模块中的每一个执行可由软件配置的特定任务
 ********************************************************************************************* */

/*
 * 表15-10列出了八个关键子模块及其主要配置参数列表。 对于
 * 例如，如果需要调整或控制PWM波形的占空比，则应该看到
 * 相关细节第15.2.2.4节中的反比较子模块。
 * Time-base (TB)
 *												•相对于系统时钟（SYSCLKOUT）缩放时基时钟（TBCLK）。
 *												•配置PWM时基计数器（TBCNT）频率或周期。
 *												•设置时基计数器的模式：
 *												- 向上计数模式：用于非对称PWM
 *												- 倒计时模式：用于非对称PWM
 *												- 递增和递减模式：用于对称PWM
 *												•配置相对于另一个ePWM模块的时基相位。
 *												•通过硬件或软件同步模块之间的时基计数器。
 *												•在同步事件后配置时基计数器的方向（向上或向下）。
 *												•配置设备由仿真器停止时，时基计数器的运行方式。
 *												•指定ePWM模块的同步输出源：
 *												- 同步输入信号
 *												- 时基计数器等于零
 *												- 时基计数器等于反比较B（CMPB）
 *												- 不产生输出同步信号。
 * Counter-compare (CC)
 *												•指定输出EPWMxA和/或输出EPWMxB的PWM占空比
 *												•指定在EPWMxA或EPWMxB输出上发生切换事件的时间
 * Action-qualifier (AQ)
 *												•指定发生时基或反比较子模块事件时所采取的操作类型：
 *												- 没有采取行动
 *												- 输出EPWMxA和/或EPWMxB切换高电平
 *												- 输出EPWMxA和/或EPWMxB切换低电平
 *												- 输出EPWMxA和/或EPWMxB切换
 *												•通过软件控制强制PWM输出状态
 *												•通过软件配置和控制PWM死区
 * Dead-band (DB)								
 *												•控制上下开关之间的传统互补死区关系
 *												•指定输出上升沿延迟值
 *												•指定输出下降沿延迟值
 *												•完全绕过死区模块。 在这种情况下，PWM波形通过
 *												没有修改。
 * PWM-chopper (PC)
 *												•创建斩波（载波）频率。
 *												•短切脉冲列中第一个脉冲的脉冲宽度。
 *												•第二个和随后的脉冲的占空比。
 *												•完全绕过PWM斩波模块。 在这种情况下，PWM波形通过
 *												没有修改。
 * Trip-zone (TZ)
 *												•配置ePWM模块对一个，全部或没有错误输入域引脚做出反应。
 *												•指定发生故障时所采取的跳闸动作：
 *												- 强制EPWMxA和/或EPWMxB高
 *												- 强制EPWMxA和/或EPWMxB低
 *												- 将EPWMxA和/或EPWMxB强制为高阻态
 *												- 配置EPWMxA和/或EPWMxB以忽略任何行程条件。
 *												•配置ePWM对每个错误输入域引脚的响应频率：
 *												- 一枪
 *												- 逐周期
 *												•启用错误输入域以启动中断。
 *												•完全绕过错误输入域模块
 * Event-trigger (ET)
 *												•启用将触发中断的ePWM事件。
 *												•指定事件引发触发的速率（每出现一次或每隔一秒或三次）
 *												发生）
 *												•轮询，设置或清除事件标志
 * High-Resolution PWM(HRPWM)
 *												•启用扩展时间分辨能力
 *												•配置更精细的时间粒度控制或边缘定位
 * */


//TBCTL (Time-Base Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// TBCNT MODE bits
#define TB_COUNT_UP			0x0
#define TB_COUNT_DOWN		0x1
#define TB_COUNT_UPDOWN		0x2
#define TB_FREEZE			0x3
// PHSEN bit
#define TB_DISABLE			0x0
#define TB_ENABLE			0x1
// PRDLD bit
#define TB_SHADOW			0x0
#define TB_IMMEDIATE		0x1
//  SYNCOSEL bits
#define TB_SYNC_IN			0x0
#define TB_CTR_ZERO			0x1
#define TB_CTR_CMPB			0x2
#define TB_SYNC_DISABLE		0x3
//HSPCLKDIV and CLKDIV bits
#define TB_DIV1				0x0
#define TB_DIV2				0x1
#define TB_DIV4				0x2
// PHSDIR bit
#define TB_DOWN				0x0
#define TB_UP				0x1
//CMPCTL (Compare Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
// LOADAMODE and LOADBMODE bits
#define CC_CTR_ZERO			0x0
#define CC_CTR_PRD			0x1
#define CC_CTR_ZERO_PRD		0x2
#define CC_LD_DISABLE		0x3

//SHDWAMODE and SHDWBMODE bits
#define CC_SHADOW			0x0
#define CC_IMMEDIATE		0x1
//AQCTLA and AQCTLB (Action-qualifier Control)
//= = = = = = = = = = = = = = = = = = = = = = = = = =
//ZRO, PRD, CAU, CAD, CBU, CBD bits
#define AQ_NO_ACTION		0x0
#define AQ_CLEAR			0x1
#define AQ_SET				0x2
#define AQ_TOGGLE			0x3
//  DBCTL (Dead-Band Control)
//= = = = = = = = = = = = = = = = = = = = = = = = = =
// MODE bits
#define DB_DISABLE			0x0
#define DBA_ENABLE			0x1
#define DBB_ENABLE			0x2
#define DB_FULL_ENABLE		0x3
// POLSEL bits
#define DB_ACTV_HI			0x0
#define DB_ACTV_LOC			0x1
#define DB_ACTV_HIC			0x2
#define DB_ACTV_LO			0x3
// PCCTL (chopper control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
//CHPEN bit
#define CHP_ENABLE			0x0
#define CHP_DISABLE			0x1
//CHPFREQ bits
#define CHP_DIV1			0x0
#define CHP_DIV2			0x1
#define CHP_DIV3			0x2
#define CHP_DIV4			0x3
#define CHP_DIV5			0x4
#define CHP_DIV6			0x5
#define CHP_DIV7			0x6
#define CHP_DIV8			0x7
//CHPDUTY bits
#define CHP1_8TH			0x0
#define CHP2_8TH			0x1
#define CHP3_8TH			0x2
#define CHP4_8TH			0x3
#define CHP5_8TH			0x4
#define CHP6_8TH			0x5
#define CHP7_8TH			0x6
//TZSEL (Trip-zone Select)
//= = = = = = = = = = = = = = = = = = = = = = = = = =
//CBCn and OSHTn bits
#define TZ_ENABLE			0x0
#define TZ_DISABLE			0x1
// TZCTL (Trip-zone Control)
// = = = = = = = = = = = = = = = = = = = = = = = = = =
//TZA and TZB bits
#define TZ_HIZ				0x0
#define TZ_FORCE_HI			0x1
#define TZ_FORCE_LO			0x2
#define TZ_DISABLE			0x3
//ETSEL (Event-trigger Select)
//= = = = = = = = = = = = = = = = = = = = = = = = = =
//INTSEL, SOCASEL, SOCBSEL bits
#define ET_CTR_ZERO			0x1
#define ET_CTR_PRD			0x2
#define ET_CTRU_CMPA		0x4
#define ET_CTRD_CMPA		0x5
#define ET_CTRU_CMPB		0x6
#define ET_CTRD_CMPB		0x7
// ETPS (Event-trigger Prescale)
// // = = = = = = = = = = = = = = = = = = = = = = = = = =
// // INTPRD, SOCAPRD, SOCBPRD bits
#define ET_DISABLE			0x0
#define ET_1ST				0x1
#define ET_2ND				0x2
#define ET_3RD				0x3

#if 0
/*
 * 当ePWM外设时钟使能时，由于可能设置中断标志
 * 由于ePWM寄存器未被正确初始化的寄生事件。 适当的程序
 * 初始化ePWM外设是：
 * 1.禁用全局中断（CPU INTM标志）
 * 2.禁用ePWM中断
 * 3.初始化外设寄存器
 * 4.清除任何虚假的ePWM标志
 * 5.启用ePWM中断
 * 6.启用全局中断
 *
 * 您可以配置以下的时基子模块：
 * •指定ePWM时基计数器（TBCNT）频率或周期来控制事件发生的频率。
 * •与其他ePWM模块管理时基同步。
 * •与其他ePWM模块维持相位关系。
 * •将时基计数器设置为递增计数，倒计时或倒计时模式。
 * •生成以下事件：
 * - CTR = PRD：等于指定周期的时基计数器（TBCNT = TBPRD）。
 *   - CTR = 0：时基计数器等于零（TBCNT = 0000h）。
 *   •配置时基时钟的速率; CPU系统时钟的预定版本
 *   （SYSCLKOUT）。 这允许时基计数器以较慢的速率递增/递减。
 * */
#define TBCTL_OFFSET			(0x0)						/*时基控制寄存器偏移地址，其基地址为每个ePWMx起始地址*/
#define TBSTS_OFFSET			(0x2)						/*Time-Base Status Register*/
#define TBPHSHR_OFFSET			(0x4)						/*HRPWM扩展相位寄存器,该寄存器仅适用于包含高分辨率扩展（HRPWM）
															  的ePWM实例。 在ePWM模块上不包括HRPWM，这个位置是保留的。*/
#define TBPHS_OFFSET			(0x6)						/*时基相位寄存器*/
#define TBCNT_OFFSET			(0x8)						/*时基计数寄存器,单片机中TC寄存器是这个么*/
#define TBPRD_OFFSET			(0xA)						/*时基周期寄存器 CTR=PRD时会产生一个信号, 
															  单片机中PR(预分频系数寄存器是这个么)*/
/*
 * 关键时基信号
 * EPWMxSYNCI
 *													时基同步输入。
 *													输入脉冲用于将时基计数器与ePWM模块的计数器同步
 *													同步链。 ePWM外设可以配置为使用或忽略此信号。 对于第一个ePWM
 *													模块（EPWM1），该信号来自设备引脚。 对于随后的ePWM模块，该信号通过
 *													来自另一个ePWM外设。 例如，EPWM2SYNCI由ePWM1外设生成，
 *													EPWM3SYNCI由ePWM2等产生。 有关信息，请参见第15.2.2.3.3.2节
 *													特定设备的同步顺序。
 * EPWMxSYNCO
 *													时基同步输出。
 *													该输出脉冲用于在同步链之后稍后同步ePWM模块的计数器。
 *													ePWM模块从三个事件源之一生成此信号：
 *													1. EPWMxSYNCI（同步输入脉冲）
 *													2. CTR = 0：时基计数器等于零（TBCNT = 0000h）。
 *													3. CTR = CMPB：时基计数器等于计数器比较B（TBCNT = CMPB）寄存器。
 * CTR = PRD
 *													时基计数器等于指定期间。
 *													只要计数器值等于有效期寄存器值，就产生该信号。 那是什么时候
 *													TBCNT = TBPRD。
 * CTR = 0
 *													时基计数器等于零。
 *													只要计数器值为零，就产生该信号。 那就是TBCNT等于0000h。
 * CTR = CMPB
 *													时基计数器等于有效的计数器比较B寄存器（TBCNT = CMPB）。
 *													该事件由对比比较子模块生成并由同步输出逻辑使用。
 * CTR_dir
 *													时基反方向。
 *													表示ePWM时基计数器的当前方向。 当计数器为高电平时，该信号为高电平
 *													降低时增加和减少。
 * CTR_max
 *													时基计数器等于最大值。 （TBCNT = FFFFh）
 *													当TBCNT值达到最大值时生成的事件。 该信号仅用作状态位。
 * TBCLK
 *													时基时钟
 *													这是系统时钟（SYSCLKOUT）的预定版本，由所有子模块使用
 *													的ePWM。 该时钟确定时基计数器递增或递减的速率。
 * */

/*
 * PWM事件的频率由时基周期（TBPRD）寄存器和模式所控制
 * 时基柜台图15-11显示了该周期（Tpwm）和频率（Fpwm）的关系
 * 上限计数，递减计数和上下限计数时基计数器模式
 * （TBPRD = 4）。每个步骤的时间增量由时基（TBCLK）定义，时基（TBCLK）是系统时钟（SYSCLKOUT）的预定版本。
 *
 * 时基计数器具有时基控制寄存器（TBCTL）选择的三种工作模式：
 *
 * •上下计数模式：								在上下计数模式下，时基计数器从零开始
 *												递增直到达到周期（TBPRD）值.当达到期限值时，
 *												时基计数器然后递减直到它达到零。此时，计数器重复该模式
 *												并开始增加。
 * •向上计数模式：								在此模式下，时基计数器从零开始递增直到达到
 *												期间寄存器中的值（TBPRD）。当达到周期值时，时基计数器
 *												重置为零，并再次开始增加。
 * •向下计数模式：								在递减计数模式下，时基计数器从周期（TBPRD）值开始
 *												并减少到零。当其达到零时，时基计数器将重置为周期值，并再次开始递减。
 *
 * 时基寄存器（TBPRD）具有影子寄存器。阴影允许注册更新
 * 与硬件同步。以下定义用于描述其中的所有影子寄存器
 * ePWM模块：
 * •活动寄存器：			活动寄存器控制硬件，并负责该操作
 *							硬件原因或调用。
 * •影子寄存器：			影子寄存器缓冲或提供活动的临时保留位置
 *							寄存器。它对任何控制硬件都没有直接的影响。在一个战略时刻的影子
 *							寄存器的内容被传送到活动寄存器。这可以防止损坏或虚假操作
 *							由于寄存器被软件异步修改。
 *							
 * 阴影期间寄存器的存储器地址与有效寄存器相同。哪个寄存器是
 * 写入或读取由TBCTL [PRDLD]位决定。该位启用和禁用TBPRD影子寄存器如下：
 *
 * •时基暗影模式：			当TBCTL [PRDLD] = 0时，TBPRD影子寄存器被使能
 *							读写TBPRD存储器地址到影子寄存器。影子
 *							寄存器内容被传送到活动寄存器（TBPRD（活动）←TBPRD（阴影））
 *							时基计数器等于零（TBCNT = 0000h）。默认情况下，TBPRD影子寄存器被使能。
 * •时基周期即时加载模式：	如果选择立即加载模式（TBCTL [PRDLD] = 1），
 *							那么对TBPRD存储器地址的读取或写入将直接发送到有效寄存器。
 * 
 * 时基同步方案连接设备上的所有ePWM模块。 每个ePWM模块都具有同步输入（EPWMxSYNCI）和同步输出（EPWMxSYNCO）。 
 * 第一个实例（ePWM1）的输入同步来自一个外部引脚。 其余ePWM模块的可能的同步连接如图15-12所示。
 * Time-Base Counter Synchronization Scheme 1:
 * GPIO MUX -> EPWM1SYNCI ePWM1 EPWM1SYNCO -> EPWMxSYNCI ePWMx EPWMxSYNCO
 * */

/*
 * 每个ePWM模块都可以配置为使用或忽略同步输入。如果TBCTL [PHSEN]
 * 位设置，则ePWM模块的时基计数器（TBCNT）将自动加载
 * 发生以下情况之一时的相位寄存器（TBPHS）内容：
 * •EPWMxSYNCI：同步输入脉冲：				当检测到输入同步脉冲（TBPHS→TBCNT）时，
 *											相位寄存器的值被装入计数器寄存器。这个操作
 *											发生在下一个有效的时基时钟（TBCLK）边沿。
 * •软件强制同步脉冲：						将1写入TBCTL [SWFSYNC]控制位调用软件强制同步。
 *											该脉冲与同步输入信号进行“或”运算，因此与EPWMxSYNCI上的脉冲具有相同的效果。
 *
 * 此功能使ePWM模块能够自动同步到另一个ePWM模块的时基。可以在不同ePWM模块生成的波形中添加引导或滞后相位控制，以使其同步。
 * 在上下计数模式下，TBCTL [PSHDIR]位在同步事件之后立即配置时基计数器的方向。新方向与同步事件之前的方向无关。
 * 在递增或倒计时模式下TBPHS位被忽略。
 * 有关示例，请参见图15-13至图15-16。
 * 清除TBCTL [PHSEN]位将ePWM配置为忽略同步输入脉冲。仍然允许同步脉冲流向EPWMxSYNCO并用于
 * 同步其他ePWM模块。这样，您可以设置主时基（例如ePWM1），下游模块（ePWM2 - ePWMx）可以选择与主机同步运行。
 *
 * 锁定多个ePWM模块的时基时钟
 * 控制模块中PWMSS_CTRL寄存器中的TBCLKEN位可用于全局同步设备上所有启用的ePWM模块的时基时钟。 
 * TBCLKEN位是芯片配置寄存器的一部分，并在第9章中进行了描述。当TBCLKEN = 0时，所有ePWM模块的时基时钟停止（默认）。 
 * 当TBCLKEN = 1时，所有ePWM时基时钟均以TBCLK的上升沿对齐起始。 对于完全同步的TBCLK，TBCTL中的预分频器位
 * 每个ePWM模块的寄存器必须设置相同。 启用ePWM的正确程序
 * 时钟如下：
 * 1.启用ePWM模块时钟。
 * 2.设置TBCLKEN = 0。这将停止任何启用的ePWM模块中的时基时钟。
 * 3.配置预分频值和所需的ePWM模式。
 * 4.设置TBCLKEN = 1。
 *
 * 时基计数器模式和时序波形
 * 时基计数器以四种模式之一运行：
 * •不对称的向上计数模式。
 * •不对称的向下计数模式。
 * •对称的向上计数。
 * •冻结时基计数器保持恒定在当前值。
 * 为了说明前三种模式的操作，图15-13至图15-16显示了何时生成事件以及时基如何响应EPWMxSYNCI信号。
 * */

/*
 * 反比较（CC）子模块
 * 图15-17显示了ePWM中的对比比较子模块。 反比较子模块的基本结构如图15-18所示。
 *
 * 反比较子模块的目的
 * 计数器比较子模块将时基计数器值作为输入。 该值与反比较A（CMPA）和反比较B（CMPB）寄存器不断比较。 
 * 当时基计数器等于比较寄存器之一时，反比较单元产生适当的事件。
 * 反比较子模块：
 * •使用CMPA和CMPB寄存器生成基于可编程时间戳的事件
 * - CTR = CMPA：时基计数器等于计数器比较A寄存器（TBCNT = CMPA）。
 *   - CTR = CMPB：时基计数器等于反比较B寄存器（TBCNT = CMPB）
 *   •如果配置适当的动作限定子子模块，则控制PWM占空比
 *   •阴影新的比较值，以防止在活动PWM周期期间发生损坏或毛刺
 *
 * 控制和监视反比较子模块
 * 表15-13列出了用于控制和监视反比较子模块的寄存器。 表15-14
 * 列出(翻译有误,计数比较器)子模块寄存器。(Counter-Compare Submodule Registers)
 * Acronym				Register Description								Address Offset	Shadowed
 * CMPCTL				Counter-Compare Control Register.					Eh				No
 * CMPAHR				HRPWM Counter-Compare A Extension Register (1)		10h				Yes
 * 该寄存器仅适用于具有高分辨率扩展（HRPWM）的ePWM模块。 在不包括HRPWM的ePWM模块上，保留该位置。 请参见第15.1.2节来确定哪些ePWM实例包括此功能
 * CMPA					Counter-Compare A Register							12h				Yes
 * CMPB					Counter-Compare B Register							14h				Yes
 *
 * 计数比较器子模块关键信号
 * CTR = CMPA		Time-base counter equal to the active counter-compare A value						TBCNT = CMPA
 *					时基计数器等于有效的计数比较器A值
 * CTR = CMPB		Time-base counter equal to the active counter-compare B value						TBCNT = CMPB
 *					时基计数器等于有效的计数比较器B值
 * CTR = PRD		Time-base counter equal to the active period.										TBCNT = TBPRD
 *					时基计数器等于活动期(即时的周期值)
 *					Used to load active counter-compare A and B registers from the shadow register
 *					用于从影子寄存器加载有效的计数比较A和B寄存器
 * CTR = 0			Time-base counter equal to zero.													TBCNT = 0000h
 *					时基计数器等于0
 *					Used to load active counter-compare A and B registers from the shadow register
 *					用于从影子寄存器加载有效的计数比较A和B寄存器
 * 计数器比较模块可以在所有三种计数模式下生成比较事件：
 * •向上计数模式：用于产生不对称的PWM波形。
 * •向下计数模式：用于产生不对称的PWM波形。
 * •Up-down-count模式：用于产生对称PWM波形。
 * 为了说明前三种模式的运行情况，图15-19中的时序图如图15-19所示
 * 图15-22显示何时生成事件以及EPWMxSYNCI信号如何相互作用。
 *
 *
 *
 * ePWM系统中的动作限定符（AQ）子模块（见阴影块）如图15-23所示。该
 * 动作限定子子模块在波形构造和PWM生成中具有最重要的作用。 它
 * 决定将哪些事件转换成各种动作类型，从而产生所需的切换
 * EPWMxA和EPWMxB输出的波形。
 * */

#define CMPCTL_OFFSET				(0xE)
#define CMPAHR_OFFSET				(0x10)
#define CMPA_OFFSET					(0x12)
#define CMPB_OFFSET					(0x14)

 /*
  * action-qualifier子模块负责以下内容：
  * •基于以下事件对动作进行排序和生成（设置，清除，切换）：
  * - CTR = PRD：等于周期的时基计数器（TBCNT = TBPRD）
  *   - CTR = 0：时基计数器等于零（TBCNT = 0000h）
  *   - CTR = CMPA：时基计数器等于计数器比较A寄存器（TBCNT = CMPA）
  *   - CTR = CMPB：时基计数器等于计数器比较B寄存器（TBCNT = CMPB）
  *   •当这些事件同时发生时管理优先级
  *   •在时基计数器增加和减少时提供独立的事件控制
  * Acronym			Register Description									Address Offset				Shadowed
  * AQCTLA			Action-Qualifier Control Register For Output A (EPWMxA)	16h							No
  *					用于输出A的动作限定控制寄存器（EPWMxA）
  * AQCTLB			Action-Qualifier Control Register For Output B (EPWMxB)	18h							No
  * AQSFRC			Action-Qualifier Software Force Register				1Ah							No
  *					动作限定软件强制注册
  * AQCSFRC			Action-Qualifier Continuous Software Force				1Ch							Yes
  *					动作限定连续软件强制
  * */

#define AQCTLA_OFFSET				(0x16)
#define AQCTLB_OFFSET				(0x18)
#define AQSFRC_OFFSET				(0x1A)
#define AQCSFRC_OFFSET				(0x1C)

/*
 * 操作限定符子模块基于事件驱动的逻辑。 
 * 它可以被看作是一个可编程交叉开关，输入端有事件，输出端有动作，所有这些都是通过图15-25所示的一组寄存器进行软件控制的。 
 * 表15-16中再次总结了可能的输入事件。
 * Signal					Description											Registers Compared
 * CTR = PRD				Time-base counter equal to the period value			TBCNT = TBPRD
 * CTR = 0					Time-base counter equal to zero						TBCNT = 0000h
 * CTR = CMPA				Time-base counter equal to the counter-compare A	TBCNT = CMPA
 * CTR = CMPB				Time-base counter equal to the counter-compare B	TBCNT = CMPB
 * Software forced event	Asynchronous event initiated by software
 * */

/*
 * 软件强制操作是一个有用的异步事件。该控制由寄存器AQSFRC和AQCSFRC处理。
 * 操作限定符子模块控制两个输出EPWMxA和EPWMxB在发生特定事件时的行为方式。
 * 输入到动作限定子模块的事件进一步通过计数器方向（向上或向下）进行限定。这允许在递增和递减阶段对输出进行独立的操作。
 * 对产出EPWMxA和EPWMxB可能采取的行动是：
 * •高电平：将输出EPWMxA或EPWMxB设置为高电平。
 * •清除低电平：将输出EPWMxA或EPWMxB设置为低电平。
 * •切换：如果EPWMxA或EPWMxB当前拉高，则拉低输出。如果EPWMxA或EPWMxB当前拉低，则拉高输出。
 * •什么都不做：将输出EPWMxA和EPWMxB保持在与当前设置相同的水平。
 * 尽管“无操作”选项可防止事件对EPWMxA和EPWMxB输出产生操作，但此事件仍可触发中断。
 * 有关详细信息，请参阅第15.2.2.9节中的事件触发器子模块说明。
 *
 * 对于任一输出（EPWMxA或EPWMxB）独立指定操作。 
 * 可以将任何或所有事件配置为在给定的输出上生成操作。 
 * 例如，CTR = CMPA和CTR = CMPB都可以在输出EPWMxA上运行。 
 * 所有限定符操作都是通过本节结尾处的控制寄存器进行配置的。
 * 为了清楚起见，本章中的图形使用一组符号动作。 
 * 这些符号总结在图15-26中。 每个符号代表一个行为作为一个标记的时间。 
 * 有些动作在时间（零点和周期）内是固定的，而CMPA和CMPB动作是可移动的，它们的时间位置分别通过计数器比较A和B寄存器进行编程。 
 * 要关闭或禁用某个操作，请使用“什么都不做选项(Do Nothing Option)“;这是重置时的默认值。
 * */


/*
 * ePWM操作限定符可以同时接收多个事件。 
 * 在这种情况下，事件被硬件分配了一个优先级。 
 * 一般规则是，事件发生后的事件具有较高的优先级，软件强制事件总是具有最高的优先级。 
 * updown-count模式的事件优先级如表15-17所示。 
 * 1的优先级是最高优先级，7级是最低优先级。 根据TBCNT的方向，优先级稍有变化。
 *
 * Table 15-17. Action-Qualifier Event Priority for Up-Down-Count Mode
 * 表15-17。 增减计数模式的动作 - 限定符事件优先级
 *
 *						Event if TBCNT is Incrementing							Event if TBCNT is Decrementing
 * Priority Level		TBCNT = 0 up to TBCNT = TBPRD							TBCNT = TBPRD down to TBCNT = 1
 * 1(Highest)			Software forced event									Software forced event
 *						软件强制事件
 * 2					Counter equals CMPB on up-count (CBU)					Counter equals CMPB on down-count (CBD)
 *						计数器等于CMPB加法计数器（CBU）							计数器等于CMPB减法计数器(CBD)
 * 3					Counter equals CMPA on up-count (CAU)					Counter equals CMPA on down-count (CAD)
 * 4					Counter equals zero										Counter equals period (TBPRD)
 *						计数器等于0												计数器等于预分频
 * 5					Counter equals CMPB on down-count (CBD) (1)				Counter equals CMPB on up-count (CBU) (1)
 *						计数器等于CMPB减法计数器(CBD)
 * 6(Lowest)			Counter equals CMPA on down-count (CAD) (1)				Counter equals CMPA on up-count (CBU) (1)
 * (1) To maintain symmetry for up-down-count mode, both up-events (CAU/CBU) and down-events (CAD/CBD) can be generated for
 * TBPRD. Otherwise, up-events can occur only when the counter is incrementing and down-events can occur only when the
 * counter is decrementing.
 * 为了保持加减计数模式的对称性，可以为TBPRD生成向上事件（CAU / CBU）和向下事件（CAD / CBD）。 
 * 否则，只有当计数器正在递增时才会发生向上事件，只有当计数器递减时才会发生向下事件
 *
 * 加计数模式的操作限定符优先级。 在这种情况下，计数器方向总是被定义为up，因此减计数事件将不会被采用。
 * Action-Qualifier Event Priority for Up-Count Mode
 * Priority Level				Event
 * 1 (Highest)					Software forced event
 * 2							Counter equal to period (TBPRD)
 * 3							Counter equal to CMPB on up-count (CBU)
 * 4							Counter equal to CMPA on up-count (CAU)
 * 5 (Lowest)					Counter equal to Zero
 *
 * 减计数模式的动作限定优先级。 在这种情况下，计数器方向总是被定义为向下的，因此永远不会进行向上计数的事件。
 * Action-Qualifier Event Priority for Down-Count Mode
 * Priority Level				Event
 * 1 (Highest)					Software forced event
 * 2							Counter equal to Zero
 * 3							Counter equal to CMPB on down-count (CBD)
 * 4							Counter equal to CMPA on down-count (CAD)
 * 5 (Lowest)					Counter equal to period (TBPRD)
 * */

#define PCCTL_OFFSET			(0x3C)					/*PWM斩波器控制寄存器*/

/*
 * 第一个脉冲的宽度可以编程为16个可能的脉冲宽度值中的任何一个。 
 * 第一个脉冲的宽度或周期由下式给出：
 *	T_1stpulse = T_SYSCLKOUT × 8 × OSHTWTH
 * 其中T_SYSCLKOUT是系统时钟的周期（SYSCLKOUT），OSHTWTH是四个控制位（值为1到16）
 * */

//Trip-Zone Submodule Registers
#define TZSEL_OFFSET			(0x24)				/*Trip-Zone Select Register*/
#define TZCTL_OFFSET			(0x28)				/*Trip-Zone Control Register*/
#define TZEINT_OFFSET			(0x2A)				/*Trip-Zone Enable Interrupt Register*/
#define TZFLG_OFFSET			(0x2C)				/*Trip-Zone Flag Register*/
#define TZCLR_OFFSET			(0x2E)				/*Trip-Zone Clear Register*/
#define TZFRC_OFFSET			(0x30)				/*Trip-Zone Force Register*/
/*
 * Possible Actions On a Trip Event
 * TZCTL[TZA]				EPWMxA
 * and/or					and/or
 * TZCTL[TZB]				EPWMxB					Comment
 * 0						High-Impedance			Tripped
 * 1h						Force to High State		Tripped
 * 2h						Force to Low State		Tripped
 * 3h						No Change				Do Nothing. No change is made to the output.
 *
 * Trip-Zone Configurations
 * Scenario A:
 * A one-shot trip event on TZ1 pulls both EPWM1A, EPWM1B low and also forces EPWM2A and EPWM2B
 * high.
 * • Configure the ePWM1 registers as follows:
 * – TZSEL[OSHT1] = 1: enables TZ as a one-shot event source for ePWM1
 * – TZCTL[TZA] = 2: EPWM1A will be forced low on a trip event.
 * – TZCTL[TZB] = 2: EPWM1B will be forced low on a trip event.
 * • Configure the ePWM2 registers as follows:
 * – TZSEL[OSHT1] = 1: enables TZ as a one-shot event source for ePWM2
 * – TZCTL[TZA] = 1: EPWM2A will be forced high on a trip event.
 * – TZCTL[TZB] = 1: EPWM2B will be forced high on a trip event.
 * Scenario B:
 * A cycle-by-cycle event on TZ5 pulls both EPWM1A, EPWM1B low.
 * A one-shot event on TZ1 or TZ6 puts EPWM2A into a high impedance state.
 * • Configure the ePWM1 registers as follows:
 * – TZSEL[CBC5] = 1: enables TZ5 as a one-shot event source for ePWM1
 * – TZCTL[TZA] = 2: EPWM1A will be forced low on a trip event.
 * – TZCTL[TZB] = 2: EPWM1B will be forced low on a trip event.
 * • Configure the ePWM2 registers as follows:
 * – TZSEL[OSHT1] = 1: enables TZ1 as a one-shot event source for ePWM2
 * – TZSEL[OSHT6] = 1: enables TZ6 as a one-shot event source for ePWM1
 * – TZCTL[TZA] = 0: EPWM1A will be put into a high-impedance state on a trip event.
 * – TZCTL[TZB] = 3: EPWM1B will ignore the trip event.
 * */

//Event-Trigger Submodule Registers
#define ETSEL_OFFSET			(0x32)				/*Event-Trigger Selection Register*/
#define ETPS_OFFSET				(0x34)				/*Event-Trigger Prescale Register*/
#define EPFLG_OFFSET			(0x36)				/*Event-Trigger Flag Register*/
#define EPCLR_OFFSET			(0x38)				/*Event-Trigger Clear Register*/
#define EPFRC_OFFSET			(0x3A)				/*Event-Trigger Force Register*/

//HRPWM Submodule Registers
#define TBPHSHR_OFFSET			(0x4)				/*Extension Register for HRPWM Phase*/
#define CMPAHR_OFFSET			(0x19)				/*Extension Register for HRPWM Duty*/
#define HRCNFG_OFFSET			(0xC0)				/*HRPWM Configuration Register*/


/*
 * Assumptions for this example:
 * System clock, SYSCLKOUT = 10 ns (100 MHz)
 * PWM frequency = 1.25 MHz (1/800 ns)
 * Required PWM duty cycle, PWMDuty = 0.405 (40.5%)
 * PWM period in terms of coarse steps, = 80
 * PWMperiod (800 ns/10 ns)
 * Number of MEP steps per coarse step at = 55
 * 180 ps (10 ns/180 ps), MEP_SF
 * Value to keep CMPAHR within the range
 * of 1-255 and fractional rounding constant
 * (default value) = 180h
 * Step 1: Percentage Integer Duty value conversion for CMPA register
 * CMPA register value = int(PWMDuty × PWMperiod); int means integer part
 * = int(0.405 × 80)
 * = int(32.4)
 * CMPA register value = 32 (20h)
 * Step 2: Fractional value conversion for CMPAHR register
 * CMPAHR register value = (frac(PWMDuty × PWMperiod) × MEP_SF) << 8) +
 * 180h; frac means fractional part
 * = (frac(32.4) × 55 <<8) + 180h; Shift is to move the
 * value as CMPAHR high byte
 * = ((0.4 × 55) <<8) + 180h
 * = (22 <<8) + 180h
 * = 22 × 256 + 180h ; Shifting left by 8 is the same
 * multiplying by 256.
 * = 5632 + 180h
 * = 1600h + 180h
 * CMPAHR value = 1780h; CMPAHR value = 1700h, lower 8 bits will be
 * ignored by hardware.
 * */
#endif

#endif

