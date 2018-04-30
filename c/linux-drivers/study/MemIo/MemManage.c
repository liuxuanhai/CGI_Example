/******************************************    *******************************
      > File Name: MemManage.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月28日 星期二 16时59分03秒
      > Modify Time:
 *********************************    ***************************************/


/*
 * 内存管理单元
 * 高性能处理器一般会提供一个内存管理单元（MMU），该单元辅助操作系统进行内存管理，提供虚
 * 拟地址和物理地址的映射、内存访问权限保护和Cache缓存控制等硬件支持。操作系统内核借助MMU可以
 * 让用户感觉到程序好像可以使用非常大的内存空间，从而使得编程人员在写程序时不用考虑计算机中物理
 * 内存的实际容量
 *
 * 为了理解基本的MMU操作原理，需先明晰几个概念。
 * 1）TLB（Translation Lookaside Buffer）：即转换旁路缓存，TLB是MMU的核心部件，它缓存少量的
 * 虚拟地址与物理地址的转换关系，是转换表的Cache，因此也经常被称为“快表”
 * 。
 * 2）TTW（Translation Table walk）：即转换表漫游，当TLB中没有缓冲对应的地址转换关系时，需要
 * 通过对内存中转换表（大多数处理器的转换表为多级页表，如图11.2所示）的访问来获得虚拟地址和物理
 * 地址的对应关系。TTW成功后，结果应写入TLB中。
 *
 * 当ARM要访问
 * 存储器时，MMU先查找TLB中的虚拟地址表。如果ARM的结构支持分开的数据TLB（DTLB）和指令
 * TLB（ITLB），则除了取指令使用ITLB外，其他的都使用DTLB
 *
 * 若TLB中没有虚拟地址的入口，则转换表遍历硬件并从存放于主存储器内的转换表中获取地址转换信
 * 息和访问权限（即执行TTW），同时将这些信息放入TLB，它或者被放在一个没有使用的入口或者替换
 * 一个已经存在的入口。之后，在TLB条目中控制信息的控制下，当访问权限允许时，对真实物理地址的访
 * 问将在Cache或者在内存中发生
 *
 * ARM内TLB条目中的控制信息用于控制对对应地址的访问权限以及Cache的操作。
 * ·C（高速缓存）和B（缓冲）位被用来控制对应地址的高速缓存和写缓冲，并决定是否进行高速缓
 * 存。
 * ·访问权限和域位用来控制读写访问是否被允许。如果不允许，MMU则向ARM处理器发送一个存储
 * 器异常，否则访问将被允许进行
 *
 * MMU具有虚拟地址和物理地址转换、内存访问权限保护等功能，这将使得Linux操作系统能单独为系
 * 统的每个用户进程分配独立的内存空间并保证用户空间不能访问内核空间的地址，为操作系统的虚拟内存
 * 管理模块提供硬件基础
 *
 *
 * */

/*Linux的四级页表与页表查询*/
static int
pin_page_for_write(const void __user *_addr, pte_t **ptep, spinlock_t **ptlp)
{
	unsigned long addr = (unsigned long)_addr;
	pgd_t *pgd;
	pmd_t *pmd;
	pte_t *pte;
	pud_t *pud;
	spinlock_t *ptl;

	pgd = pgd_offset(current->mm, addr);
	if (unlikely(pgd_none(*pgd) || pgd_bad(*pgd)))
		return 0;

	pud = pud_offset(pgd, addr);
	if (unlikely(pud_none(*pud) || pud_bad(*pud)))
		return 0;

	pmd = pmd_offset(pud, addr);
	if (unlikely(pmd_none(*pmd)))
		return 0;

	/*
	 * A pmd can be bad if it refers to a HugeTLB or THP page.
	 *
	 * Both THP and HugeTLB pages have the same pmd layout
	 * and should not be manipulated by the pte functions.
	 *
	 * Lock the page table for the destination and check
	 * to see that it's still huge and whether or not we will
	 * need to fault on write, or if we have a splitting THP.
	 */
	if (unlikely(pmd_thp_or_huge(*pmd))) {
		ptl = &current->mm->page_table_lock;
		spin_lock(ptl);
		if (unlikely(!pmd_thp_or_huge(*pmd)
					|| pmd_hugewillfault(*pmd)
					|| pmd_trans_splitting(*pmd))) {
			spin_unlock(ptl);
			return 0;
		}

		*ptep = NULL;
		*ptlp = ptl;
		return 1;
	}

	if (unlikely(pmd_bad(*pmd)))
		return 0;

	pte = pte_offset_map_lock(current->mm, pmd, addr, &ptl);
	if (unlikely(!pte_present(*pte) || !pte_young(*pte) ||
				!pte_write(*pte) || !pte_dirty(*pte))) {
		pte_unmap_unlock(pte, ptl);
		return 0;
	}

	*ptep = pte;
	*ptlp = ptl;

	return 1;
}

/*
 * 第1行的类型为struct，mm_struct的参数mm用于描述Linux进程所占有的内存资源。上述代码中的
 * pgd_offset、pud_offset、pmd_offset分别用于得到一级页表、二级页表和三级页表的入口，最后通过
 * pte_offset_map_lock得到目标页表项pte。而且第33行还通过pmd_thp_or_huge（）判断是否有巨页的情况，
 * 如果是巨页，就直接访问pmd。
 * 但是，MMU并不是对所有的处理器都是必需的，例如常用的SAMSUNG基于ARM7TDMI系列的
 * S3C44B0X不附带MMU，新版的Linux 2.6支持不带MMU的处理器。在嵌入式系统中，仍存在大量无MMU
 * 的处理器，Linux 2.6为了更广泛地应用于嵌入式系统，融合了mClinux，以支持这些无MMU系统，如
 * Dragonball、ColdFire、Hitachi H8/300、Blackfin等。
 * */

/*
 * Linux内存管理
 * 对于包含MMU的处理器而言，Linux系统提供了复杂的存储管理系统，使得进程所能访问的内存达到
 * 4GB。
 * 在Linux系统中，进程的4GB内存空间被分为两个部分——用户空间与内核空间。用户空间的地址一
 * 般分布为0~3GB（即PAGE_OFFSET，在0x86中它等于0xC0000000），这样，剩下的3~4GB为内核空间，
 * 如图11.5所示。用户进程通常只能访问用户空间的虚拟地址，不能访问内核空间的虚拟地址。用户进程只
 * 有通过系统调用（代表用户进程在内核态执行）等方式才可以访问到内核空间
 *
 * Linux中1GB的内核地址空间又被划分为物理内存映射区、虚拟内存分配区、高端页面映射区、专用
 * 页面映射区和系统保留映射区这几个区域
 *
 * 保留区								(4GB)
 * 专用页面映射区
 * 高端内存映射区
 * (隔离带)
 * vmalloc虚拟内存分配区
 * (隔离带)
 * 物理内存映射区						base=3BG(0~896MB)
 *
 * 对于x86系统而言，一般情况下，物理内存映射区最大长度为896MB，系统的物理内存被顺序映射在
 * 内核空间的这个区域中。当系统物理内存大于896MB时，超过物理内存映射区的那部分内存称为高端内
 * 存（而未超过物理内存映射区的内存通常被称为常规内存），内核在存取高端内存时必须将它们映射到高
 * 端页面映射区。
 *
 * Linux保留内核空间最顶部FIXADDR_TOP~4GB的区域作为保留区。
 * 紧接着最顶端的保留区以下的一段区域为专用页面映射区（FIXADDR_START~FIXADDR_TOP），
 * 它的总尺寸和每一页的用途由fixed_address枚举结构在编译时预定义，用__fix_to_virt（index）可获取专用
 * 区内预定义页面的逻辑地址。其开始地址和结束地址宏定义如下
 * */
#define FIXADDR_START (FIXADDR_TOP - _ _FIXADDR_SIZE)
#define FIXADDR_TOP ((unsigned long)_ _FIXADDR_TOP)
#define _ _FIXADDR_TOP 0xfffff000

/*接下来，如果系统配置了高端内存，则位于专用页面映射区之下的就是一段高端内存映射区，其起始
 * 地址为PKMAP_BASE，定义如下*/
#define PKMAP_BASE ( (FIXADDR_BOOT_START - PAGE_SIZE*(LAST_PKMAP + 1)) & PMD_MASK )
/*其中所涉及的宏定义如下：
 * */
#define FIXADDR_BOOT_START (FIXADDR_TOP - _ _FIXADDR_BOOT_SIZE)
#define LAST_PKMAP PTRS_PER_PTE
#define PTRS_PER_PTE 512
#define PMD_MASK (~(PMD_SIZE-1))
# define PMD_SIZE (1UL << PMD_SHIFT)
#define PMD_SHIFT 21

/*
 * 在物理区和高端映射区之间为虚拟内存分配器区（VMALLOC_START~VMALLOC_END），用于
 * vmalloc（）函数，它的前部与物理内存映射区有一个隔离带，后部与高端映射区也有一个隔离带，
 * vmalloc区域定义如下
 * */
#define VMALLOC_OFFSET (8*1024*1024)
#define VMALLOC_START (((unsigned long) high_memory + \
			vmalloc_earlyreserve + 2*VMALLOC_OFFSET-1) & ~(VMALLOC_OFFSET-1))
#ifdef CONFIG_HIGHMEM /* 支持高端内存 */
# define VMALLOC_END (PKMAP_BASE-2*PAGE_SIZE)
#else /* 不支持高端内存 */
# define VMALLOC_END (FIXADDR_START-2*PAGE_SIZE)
#endif
/*
 * 当系统物理内存超过4GB时，必须使用CPU的扩展分页（PAE）模式所提供的64位页目录项才能存取
 * 到4GB以上的物理内存，这需要CPU的支持。加入了PAE功能的Intel Pentium Pro及以后的CPU允许内存最
 * 大可配置到64GB，它们具备36位物理地址空间寻址能力。
 *
 * 由此可见，对于32位的x86而言，在3~4GB之间的内核空间中，从低地址到高地址依次为：
 * 物理内存映射区→隔离带→vmalloc虚拟内存分配器区→隔离带→高端内存映射区→专用页面映射区→保留区。
 *
 * 直接进行映射的896MB物理内存其实又分为两个区域，
 * 在低于16MB的区域，ISA设备可以做DMA，
 * 所以该区域为DMA区域（内核为了保证ISA驱动在申请DMA缓冲区的时候，通过GFP_DMA标记可以确保
 *
 * 申请到16MB以内的内存，所以必须把这个区域列为一个单独的区域管理）；
 * 16MB~896MB之间的为常规区域。高于896MB的就称为高端内存区域了。
 *
 * 32位ARM Linux的内核空间地址映射与x86不太一样，内核文档Documentation/arm/memory.txt给出了
 * ARM Linux的内存映射情况。0xffff0000~0xffff0fff是“CPU vector page”
 * ，即向量表的地址。
 *
 * 0xffc00000~0xffefffff是DMA内存映射区域，dma_alloc_xxx族函数把DMA缓冲区映射在这一段，
 *
 * 1.VMALLOC_START~VMALLOC_END-1是vmalloc和ioremap区域（在vmalloc区域的大小可以配置，通
 * 过“vmalloc=”这个启动参数可以指定），
 * 2.PAGE_OFFSET~high_memory-1是DMA和正常区域的映射区域，
 * 3.MODULES_VADDR~MODULES_END-1是内核模块区域，
 * 4.PKMAP_BASE~PAGE_OFFSET-1是高端内存映射区。
 *
 * 假设我们把PAGE_OFFSET定义为3GB，实际上Linux内核模块位于3GB-16MB~3GB-2MB，高端内
 * 存映射区则通常位于3GB-2MB~3GB。
 *
 * 图11.7给出了32位ARM系统Linux内核地址空间中的内核模块区域、高端内存映射区、vmalloc、向量
 * 表区域等。
 * 我们假定编译内核的时候选择的是VMSPLIT_3G（3G/1G user/kernel split）。
 * 如果用户选择的是VMSPLIT_2G（2G/2G user/kernel split），
 * 则图11.7中的内核模块开始于2GB-16MB，DMA和常规内存区域映射区也开始于2GB。
 *
 *
 * ARM系统的Linux之所以把内核模块安置在3GB或者2GB附近的16MB范围内，主要是为了实现内核模
 * 块和内核本身的代码段之间的短跳转。
 *
 * 对于ARM SoC而言，如果芯片内部有的硬件组件的DMA引擎访问内存时有地址空间限制（某些空间
 * 访问不到），比如假设UART控制器的DMA只能访问32MB，那么这个低32MB就是DMA区域；32MB到高
 * 端内存地址的这段称为常规区域；再之上的称为高端内存区域。
 *
 * 图11.8给出了几种DMA、常规、高端内存区域可能的分布，在第一种情况下，有硬件的DMA引擎不
 * 能访问全部地址，且内存较大而无法全部在内核空间虚拟地址映射下，存放有3个区域；第二种情况下，
 * 没有硬件的DMA引擎不能访问全部地址，且内存较大而无法全部在内核空间虚拟地址映射下，则常规区
 * 域实际退化为0；第三种情况下，有硬件的DMA引擎不能访问全部地址，且内存较小可以全部在内核空间
 * 虚拟地址映射下，则高端内存区域实际退化为0；第四种情况下，没有硬件的DMA引擎不能访问全部地
 * 址，且内存较小可以全部在内核空间虚拟地址映射下，则常规和高端内存区域实际退化为0。
 *
 * DMA区域		常规区域		前端内存区域				内存大，有硬件的DMA访问地址受阻
 * DMA区域		前端内存区域								内存大，没有硬件的DNA地址访问受阻
 * DMA区域		常规区域									内存小，有硬件的DMA访问地址受阻
 * DMA区域													内存小，没有硬件的DNA地址访问受阻
 *
 * DMA、常规、高端内存这3个区域都采用buddy算法进行管理，把空闲的页面以2的n次
 * 方为单位进行管理，因此Linux最底层的内存申请都是以2
 * n为单位的。Buddy算法最主要的优点是避免了外
 * 部碎片，任何时候区域里的空闲内存都能以2的n次方进行拆分或合并
 *
 * /proc/buddyinfo会显示每个区域里面2n的空闲页面分布情况，比如：
 * $cat /proc/buddyinfo
 * Node 0, zone DMA 8 5 2 7 8 3 0 0
 * 0 1 0
 * Node 0, zone Normal 2002 1252 524 187 183 71 7 0
 * 0 1 1
 *
 * 上述结果显示高端内存区域为0，DMA区域里1页空闲的内存还有8个，连续2页空闲的有5个，连续4
 * 页空闲的有2个，以此类推；常规区域里面1页空闲的还有2002个，连续2页空闲的有1252个，以此类推。
 * 对于内核物理内存映射区的虚拟内存（即从DMA和常规区域映射过来的），使用virt_to_phys（）可
* 以实现内核虚拟地址转化为物理地址。与之对应的函数为phys_to_virt（），它将物理地址转化为内核虚拟
* 地址。
* 注意：上述virt_to_phys（）和phys_to_virt（）方法仅适用于DMA和常规区域，高端内存的虚拟地址
* 与物理地址之间不存在如此简单的换算关系。
*
*
 * */

