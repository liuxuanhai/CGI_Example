/******************************************    *******************************
      > File Name: blk.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Wed 06 Dec 2017 11:24:14 AM CST
      > Modify Time: 
 *********************************    ***************************************/


/*
   block_device_operations结构体
   在块设备驱动中，有一个类似于字符设备驱动中file_operations结构体的block_device_operations结构
   体，它是对块设备操作的集合
   */

struct block_device_operations {
	int (*open) (struct block_device *, fmode_t); /*打开设备*/
	void (*release) (struct gendisk *, fmode_t); /*释放设备*/
	int (*rw_page)(struct block_device *, sector_t, struct page *, int rw);	
	int (*ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);		/*控制*/
	int (*compat_ioctl) (struct block_device *, fmode_t, unsigned, unsigned long); /*64位系统的32位程序调用ioctl()的时候是它*/
	int (*direct_access) (struct block_device *, sector_t, void **, unsigned long *);
	unsigned int (*check_events) (struct gendisk *disk, unsigned int clearing); /*介质改变*/
	/* ->media_changed() is DEPRECATED, use ->check_events() instead */
	int (*media_changed) (struct gendisk *);
	void (*unlock_native_capacity) (struct gendisk *);
	int (*revalidate_disk) (struct gendisk *);									/*介质有效，revalidate_disk（）函数被调用来响应一
																				  个介质改变，它给驱动一个机会来进行必要的工作以
																				  使新介质准备好*/
	int (*getgeo)(struct block_device *, struct hd_geometry *);					/*获取驱动器信息，该函数根据驱动器的几何信息填充
																				  一个hd_geometry结构体，hd_geometry结构体包含磁
																				  头、扇区、柱面等信息*/
	/* this callback is with swap_lock and sometimes page table lock held */
	void (*swap_slot_free_notify) (struct block_device *, unsigned long);
	struct module *owner;
};

/*
  在Linux内核中，使用gendisk（通用磁盘）结构体来表示一个独立的磁盘设备（或分区）

 */
struct gendisk {
	/* major, first_minor and minors are input parameters only,
	 * don't use directly.  Use disk_devt() and disk_max_parts().
	 */
	int major;			/* major number of driver */
	int first_minor;	/* 第一个此设备号 */
	int minors;                     /* maximum number of minors, =1 for
                                         * disks that can't be partitioned. */

	char disk_name[DISK_NAME_LEN];	/* name of major driver */
	char *(*devnode)(struct gendisk *gd, umode_t *mode);

	unsigned int events;		/* supported events */
	unsigned int async_events;	/* async events, subset of all */

	/* Array of pointers to partitions indexed by partno.
	 * Protected with matching bdev lock but stat and other
	 * non-critical accesses use RCU.  Always access through
	 * helpers.
	 */
	struct disk_part_tbl __rcu *part_tbl;			/* 用于容纳分区表 disk->part_tbl->part[0] = &disk->part0*/
	struct hd_struct part0;							/* 磁盘上的分区信息 */

	const struct block_device_operations *fops;		/* 块设备操作集合,等同与字符设备的 */
	struct request_queue *queue;					/* IO请求队列*/
	void *private_data;								/* 私有数据，同字符设备 */

	int flags;
	struct device *driverfs_dev;  // FIXME: remove 表示这是一个设备
	struct kobject *slave_dir;

	struct timer_rand_state *random;
	atomic_t sync_io;		/* RAID */
	struct disk_events *ev;
#ifdef  CONFIG_BLK_DEV_INTEGRITY
	struct kobject integrity_kobj;
#endif	/* CONFIG_BLK_DEV_INTEGRITY */
	int node_id;
};
/*
   分配gendisk
  gendisk结构体是一个动态分配的结构体，它需要特别的内核操作来初始化，驱动不能自己分配这个
  结构体
 */
struct gendisk *alloc_disk(int minors);
/*minors参数是这个磁盘使用的次设备号的数量，一般也就是磁盘分区的数量，此后minors不能被修改*/
/*
   增加gendisk
   gendisk结构体被分配之后，系统还不能使用这个磁盘，需要调用如下函数来注册这个磁盘设备*/
void add_disk(struct gendisk *disk);
/*特别要注意的是：对add_disk（）的调用必须发生在驱动程序的初始化工作完成并能响应磁盘的请求之后。*/
/*释放gendisk
 当不再需要磁盘时，应当使用如下函数释放gendisk。
 */
void del_gendisk(struct gendisk *gp);
/*
 .gendisk引用计数
 通过get_disk（）和put_disk（）函数可操作gendisk的引用计数，这个工作一般不需要驱动亲自做。这
 两个函数的原型分别为
 */
struct kobject *get_disk(struct gendisk *disk);			//+1
void put_disk(struct gendisk *disk);					//-1

/*
 bio、request和request_queue
 通常一个bio对应上层传递给块层的I/O请求。
 每个bio结构体实例及其包含的bvec_iter、bio_vec结构体
 实例描述了该I/O请求的开始扇区、数据方向（读还是写）、数据放入的页
 */
struct bvec_iter {
	sector_t		bi_sector;	/* device address in 512 byte sectors 设备地址在512字节扇区*/
	unsigned int	bi_size;	/* residual I/O count 剩余的I / O数量*/
	unsigned int	bi_idx;		/* current index into bvl_vec 将当前索引转换为bvl_vec*/
	unsigned int	bi_bvec_done;	/* number of bytes completed incurrent bvec 已完成的字节数bvec*/
};

struct bio_vec {
	struct page		*bv_page;
	unsigned int	bv_len;
	unsigned int	bv_offset;
};

/*与bio对应的数据每次存放的内存不一定是连续的，bio_vec结构体用来描述与这个bio请求对应的所有
 的内存，它可能不总是在一个页面里面，因此需要一个向量，定义如代码清单13.4所示。向量中的每个元
 素实际是一个[page，offset，len]，我们一般也称它为一个片段*/

/*
 * main unit of I/O for the block layer and lower layers (ie drivers and
 * stacking drivers)
 */
struct bio {
	struct bio		*bi_next;	/* request queue link 指向链表中下一个bio的指针bi_next */
	struct block_device	*bi_bdev;
	unsigned int		bi_flags;	/* status, command, etc */
	int			bi_error;
	unsigned long		bi_rw;		/* bottom bits READ/WRITE,
						 * top bits priority
						  bi_rw低位表示读写READ/WRITE, 高位表示优先级
						 */

	struct bvec_iter	bi_iter;

	/* Number of segments in this BIO after
	 * physical address coalescing is performed.
	 */
	unsigned int		bi_phys_segments;

	/*
	 * To keep track of the max segment size, we account for the
	 * sizes of the first and last mergeable segments in this bio.
	 */
	unsigned int		bi_seg_front_size;
	unsigned int		bi_seg_back_size;

	atomic_t		__bi_remaining;

	bio_end_io_t		*bi_end_io;

	void			*bi_private;
#ifdef CONFIG_BLK_CGROUP
	/*
	 * Optional ioc and css associated with this bio.  Put on bio
	 * release.  Read comment on top of bio_associate_current().
	 */
	struct io_context	*bi_ioc;
	struct cgroup_subsys_state *bi_css;
#endif
	union {
#if defined(CONFIG_BLK_DEV_INTEGRITY)
		struct bio_integrity_payload *bi_integrity; /* data integrity */
#endif
	};

	unsigned short		bi_vcnt;	/* how many bio_vec's bio对象包含bio_vec对象的数目 */

	/*
	 * Everything starting with bi_max_vecs will be preserved by bio_reset()
	 */

	unsigned short		bi_max_vecs;	/* max bvl_vecs we can hold 个bio能承载的最大的io_vec的数目 */

	atomic_t		__bi_cnt;	/* pin count */

	struct bio_vec		*bi_io_vec;	/* the actual vec list */

	struct bio_set		*bi_pool;

	/*
	 * We can inline a number of vecs at the end of the bio, to avoid
	 * double allocations for a small number of bio_vecs. This member
	 * MUST obviously be kept at the very end of the bio.
	 */
	struct bio_vec		bi_inline_vecs[0]; /* */
};


/*
   read()系统调用最终会调用一个适当的VFS函数(read()-->sys_read()-->vfs_read())，将文件描述符fd和文件内的偏移量offset传递给它。
   VFS会判断这个SCI的处理方式，如果访问的内容已经被缓存在RAM中（磁盘高速缓存机制），就直接访问，否则从磁盘中读取
   为了从物理磁盘中读取，内核依赖映射层mapping layer，即上图中的磁盘文件系统
   确定该文件所在文件系统的块的大小，并根据文件块的大小计算所请求数据的长度。本质上，文件被拆成很多块，因此内核需要确定请求数据所在的块
   映射层调用一个具体的文件系统的函数，这个层的函数会访问文件的磁盘节点，然后根据逻辑块号确定所请求数据在磁盘上的位置。
   内核利用通用块层(generic block layer)启动IO操作来传达所请求的数据，通常，一个IO操作只针对磁盘上一组连续的块。
   IO调度程序根据预先定义的内核策略将待处理的IO进行重排和合并
   块设备驱动程序向磁盘控制器硬件接口发送适当的指令，进行实际的数据操作


   核心结构
   gendisk是一个物理磁盘或分区在内核中的描述
   block_device_operations描述磁盘的操作方法集，block_device_operations之于gendisk，类似于file_operations之于cdev
   request_queue对象表示针对一个gendisk对象的所有请求的队列，是相应gendisk对象的一个域
   request表示经过IO调度之后的针对一个gendisk(磁盘)的一个"请求"，是request_queue的一个节点。多个request构成了一个request_queue
   bio表示应用程序对一个gendisk(磁盘)原始的访问请求，一个bio由多个bio_vec，多个bio经过IO调度和合并之后可以形成一个request。
   bio_vec描述的应用层准备读写一个gendisk(磁盘)时需要使用的内存页page的一部分，即上文中的"段"，多个bio_vec和bio_iter形成一个bio
   bvec_iter描述一个bio_vec中的一个sector信息

   核心方法
   set_capacity()设置gendisk对应的磁盘的物理参数
   blk_init_queue()分配+初始化+绑定一个有IO调度的gendisk的requst_queue，处理函数是**void (request_fn_proc) (struct request_queue *q);**类型
   blk_alloc_queue() 分配+初始化一个没有IO调度的gendisk的request_queue,
   blk_queue_make_request()绑定处理函数到一个没有IO调度的request_queue，处理函数函数是void (make_request_fn) (struct request_queue q, struct bio bio);类型
   __rq_for_each_bio()遍历一个request中的所有的bio
   bio_for_each_segment()遍历一个bio中所有的segment
   rq_for_each_segment()遍历一个request中的所有的bio中的所有的segment
   最后三个遍历算法都是用在request_queue绑定的处理函数中，这个函数负责对上层请求的处理。
 */

/*
request_queue
每一个gendisk对象都有一个request_queue对象，前文说过，块设备有两种访问接口，一种是/dev下，一种是通过文件系统，后者经过IO调度在这个gendisk->request_queue上增加请求，最终回调与request_queue绑定的处理函数，将这些请求向下变成具体的硬件操作
*/
294 struct request_queue {
	298         struct list_head        queue_head; 
	300         struct elevator_queue   *elevator;
	472 };
struct request_queue
/*
--298-->请求队列的链表头
--300-->请求队列使用的IO调度算法, 通过内核启动参数来选择: kernel elevator=deadline
request_queue_t和gendisk一样需要使用内核API来分配并初始化,里面大量的成员不要直接操作, 此外, 请求队列如果要正常工作还需要绑定到一个处理函数中, 当请求队列不为空时, 处理函数会被回调, 这就是块设备驱动中处理请求的核心部分!
从驱动模型的角度来说, 块设备主要分为两类需要IO调度的和不需要IO调度的, 前者包括磁盘, 光盘等, 后者包括Flash, SD卡等, 为了保证模型的统一性 , Linux中对这两种使用同样的模型但是通过不同的API来完成上述的初始化和绑定
*/
/*
  I/O调度算法可将连续的bio合并成一个请求。请求是bio经由I/O调度进行调整后的结果，这是请求和
  bio的区别。因此，一个request可以包含多个bio。当bio被提交给I/O调度器时，I/O调度器可能会将这个bio
  插入现存的请求中，也可能生成新的请求。
  每个块设备或者块设备的分区都对应有自身的request_queue，从I/O调度器合并和排序出来的请求会被
  分发（Dispatch）到设备级的request_queue
 */
/*驱动中涉及的处理bio、request和request_queue的主要API*/
/*初始化请求队列
  该函数的第一个参数是请求处理函数的指针，第二个参数是控制访问队列权限的自旋锁，这个函数会
  发生内存分配的行为，它可能会失败，因此一定要检查它的返回值。这个函数一般在块设备驱动的初始化
  过程中调用
 */
request_queue_t *blk_init_queue(request_fn_proc *rfn, spinlock_t *lock);
/*
 清除请求队列
这个函数完成将请求队列返回给系统的任务，一般在块设备驱动卸载过程中调用
 */
void blk_cleanup_queue(request_queue_t * q);
/*
 分配请求队列
 */
request_queue_t *blk_alloc_queue(int gfp_mask);
/*
 对于RAMDISK这种完全随机访问的非机械设备，并不需要进行复杂的I/O调度，这个时候，可以直
 接“踢开”I/O调度器，使用如下函数来绑定请求队列和“制造请求”函数（make_request_fn）。
 */
void blk_queue_make_request(request_queue_t * q, make_request_fn * mfn);
/*
blk_alloc_queue（）和blk_queue_make_request（）结合起来使用的逻辑一般是：
*/
xxx_queue = blk_alloc_queue(GFP_KERNEL);
blk_queue_make_request(xxx_queue, xxx_make_request);
/*
 提取请求
 */
struct request * blk_peek_request(struct request_queue *q);
/*
 上述函数用于返回下一个要处理的请求（由I/O调度器决定），如果没有请求则返回NULL。它不会清
 除请求，而是仍然将这个请求保留在队列上。原先的老的函数elv_next_request（）已经不再存在
 */
/*
 启动请求
 从请求队列中移除请求。原先的老的API blkdev_dequeue_request（）会在blk_start_request（）内部被
 调用
 */
void blk_start_request(struct request *req);
/*
 我们可以考虑使用blk_fetch_request（）函数，它同时做完了blk_peek_request（）和
 blk_start_request（）的工作
 */
struct request *blk_fetch_request(struct request_queue *q)
{
	struct request *rq;

	rq = blk_peek_request(q);
	if (rq)
		blk_start_request(rq);
	return rq;
}

/*
 遍历bio和片段
 */
/*__rq_for_each_bio（）遍历一个请求的所有bio*/
#define __rq_for_each_bio(_bio, rq) \
	if ((rq->bio)) \
		for (_bio = (rq)->bio; _bio; _bio = _bio->bi_next)

/*bio_for_each_segment（）遍历一个bio的所有bio_vec*/
#define __bio_for_each_segment(bvl, bio, iter, start) \
	for (iter = (start); \
			(iter).bi_size && \
			((bvl = bio_iter_iovec((bio), (iter))), 1); \
			bio_advance_iter((bio), &(iter), (bvl).bv_len))
#define bio_for_each_segment(bvl, bio, iter) \
	__bio_for_each_segment(bvl, bio, iter, (bio)->bi_iter)

/*rq_for_each_segment（）迭代遍历一个请求所有bio中的所有segment*/
#define rq_for_each_segment(bvl, _rq, _iter) \
	__rq_for_each_bio(_iter.bio, _rq) \
		bio_for_each_segment(bvl, _iter.bio, _iter.iter)
/*
 报告完成
 */

void __blk_end_request_all(struct request *rq, int error);
void blk_end_request_all(struct request *rq, int error);
/*上述两个函数用于报告请求是否完成，error为0表示成功，小于0表示失败。__blk_end_request_all（）
 需要在持有队列锁的场景下调用*/

/*若我们用blk_queue_make_request（）绕开I/O调度，但是在bio处理完成后应该使用bio_endio（）函数
 通知处理结束*/
void bio_endio(struct bio *bio, int error);
/*如果是I/O操作故障，可以调用快捷函数bio_io_error（），它定义为*/
#define bio_io_error(bio) bio_endio((bio), -EIO)



/*request*/
97 struct request {
	98         struct list_head queuelist;
	104         struct request_queue *q;
	117         struct bio *bio;
	118         struct bio *biotail;
	119 
		120         struct hlist_node hash; /* merge hash */
	126         union {
		127                 struct rb_node rb_node; /* sort/lookup */
		128                 void *completion_data;
		129         };
	137         union {
		138                 struct {
			139                         struct io_cq            *icq;
			140                         void                    *priv[2];
			141                 } elv;
		142 
			143                 struct {
				144                         unsigned int            seq;
				145                         struct list_head        list;            
				146                         rq_end_io_fn            *saved_end_io;
				147                 } flush;
		148         };
	149 
		150         struct gendisk *rq_disk;
	151         struct hd_struct *part;           
	199 };
struct request
/*
--98-->将这个request挂接到链表的节点
--104-->这个request从属的request_queue
--117-->组成这个request的bio链表的头指针
--118-->组成这个request的bio链表的尾指针
--120-->内核hash表头指针
*/
/*
   遍历request_queue，绑定函数的一个必要的工作就是将request_queue中的数据取出, 所以遍历是必不可少的, 针对有IO调度的设备, 我们需要从中提取请求再继续操作, 对于没有IO调度的设备, 我们可以直接从request_queue中提取bio进行操作, 这两种处理函数的接口就不一样，下面的例子是对LDD3中的代码进行了修剪而来的，相应的API使用的是3.14版本，可以看出这两种模式的使用方法的不同
sbull_init
└── setup_device
		├──sbull_make_request
		│            ├──sbull_xfer_bio
		│            └──sbull_transfer
		└──sbull_full_request
			├──blk_fetch_request
			└──sbull_xfer_request
				├── __rq_for_each_bio
				└── sbull_xfer_bio
					└──sbull_transfer
					*/

/*Linux块设备驱动的初始化*/
/*
 在块设备的注册和初始化阶段，与字符设备驱动类似，块设备驱动要注册它们自己到内核，申请设备
 号，完成这个任务的函数是register_blkdev（）
 */
int register_blkdev(unsigned int major, const char *name);
/*major:主设备号
  name:设备名，会显示在/proc/devices中
  major为0会自动分配一个，错误返回负值*/
/*注销,传参必须与注册一致，否则返回-EINVAL*/
int unregister_blkdev(unsigned int major, const char *name);
/*
 除此之外，在块设备驱动初始化过程中，通常需要完成分配、初始化请求队列，绑定请求队列和请求
 处理函数的工作，并且可能会分配、初始化gendisk，给gendisk的major、fops、queue等成员赋值，最后添
 加gendisk
 见example
 */

/*
 块设备驱动的ioctl函数
 与字符设备驱动一样，块设备可以包含一个ioctl（）函数以提供对设备的I/O控制能力。实际上，高
 层的块设备层代码处理了绝大多数I/O控制，如BLKFLSBUF、BLKROSET、BLKDISCARD、
 HDIO_GETGEO、BLKROGET和BLKSECTGET等，因此，在具体的块设备驱动中通常只需要实现与设备
 相关的特定ioctl命令。例如，源代码文件为drivers/block/floppy.c实现了与软驱相关的命令（如FDEJECT、
 FDSETPRM、FDFMTTRK等）。
 Linux MMC子系统支持一个IOCTL命令MMC_IOC_CMD，drivers/mmc/card/block.c实现了这个命令的
 处理
 */
static int mmc_blk_ioctl(struct block_device *bdev, fmode_t mode, unsigned int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	if (cmd == MMC_IOC_CMD)
		ret = mmc_blk_ioctl_cmd(bdev, (struct mmc_ioc_cmd __user *)arg);
	return ret;
}

/*
 使用请求队列
 块设备驱动在使用请求队列的场景下，会用blk_init_queue（）初始化request_queue，而该函数的第一
 个参数就是请求处理函数的指针。request_queue会作为参数传递给我们在调用blk_init_queue（）时指定的
 请求处理函数，块设备驱动请求处理函数的原型为
 */
static void xxx_req(struct request_queue *q);
/*这个函数不能由驱动自己调用，只有当内核认为是时候让驱动处理对设备的读写等操作时，它才调用
 这个函数。该函数的主要工作就是发起与request对应的块设备I/O动作（但是具体的I/O工作不一定要在该
 函数内同步完成）*/

/*块设备驱动请求函数例程*/
static void msb_submit_req(struct request_queue *q)
{
	struct memstick_dev *card = q->queuedata;
	struct msb_data *msb = memstick_get_drvdata(card);
	struct request *req = NULL;

	dbg_verbose("Submit request");
	/*成立的，但是无法处理*/
	if (msb->card_dead) {
		dbg("Refusing requests on removed card");

		WARN_ON(!msb->io_queue_stopped);
		/*获得队列第一个为完成请求*/
		while ((req = blk_fetch_request(q)) != NULL)
			__blk_end_request_all(req, -ENODEV);			/*返回错误*/
		return;
	}

	if (msb->req)
		return;

	if (!msb->io_queue_stopped)
		queue_work(msb->io_queue, &msb->io_work);
}

/*正常的情况下，通过queue_work（msb->io_queue，&msb->io_work）启动工作队列执行
 msb_io_work（struct work_struct*work）这个函数*/

/*msb_io_work（）完成请求处理*/
static void msb_io_work(struct work_struct *work)
{
	struct msb_data *msb = container_of(work, struct msb_data, io_work);
	int page, error, len;
	sector_t lba;
	unsigned long flags;
	struct scatterlist *sg = msb->prealloc_sg;

	dbg_verbose("IO: work started");

	while (1) {
		spin_lock_irqsave(&msb->q_lock, flags);

		if (msb->need_flush_cache) {
			msb->need_flush_cache = false;
			spin_unlock_irqrestore(&msb->q_lock, flags);
			msb_cache_flush(msb);
			continue;
		}

		if (!msb->req) {
			msb->req = blk_fetch_request(msb->queue);
			if (!msb->req) {
				dbg_verbose("IO: no more requests exiting");
				spin_unlock_irqrestore(&msb->q_lock, flags);
				return;
			}
		}

		spin_unlock_irqrestore(&msb->q_lock, flags);

		/* If card was removed meanwhile */
		if (!msb->req)
			return;

		/* process the request */
		dbg_verbose("IO: processing new request");
		/*它通过rq_for_each_bio（）、bio_for_each_segment（）来遍历所有的bio，以及所有
		 的片段，将所有与某请求相关的页组成一个scatter/gather的列表*/
		blk_rq_map_sg(msb->queue, msb->req, sg);

		lba = blk_rq_pos(msb->req);

		sector_div(lba, msb->page_size / 512);
		page = do_div(lba, msb->pages_in_block);

		if (rq_data_dir(msb->req) == READ)
			error = msb_do_read_request(msb, lba, page, sg,
					blk_rq_bytes(msb->req), &len);
		else
			error = msb_do_write_request(msb, lba, page, sg,
					blk_rq_bytes(msb->req), &len);

		spin_lock_irqsave(&msb->q_lock, flags);

		if (len)
			if (!__blk_end_request(msb->req, 0, len))			/*告诉上层请求处理完成*/
				msb->req = NULL;

		if (error && msb->req) {
			dbg_verbose("IO: ending one sector of the request with error");
			if (!__blk_end_request(msb->req, error, msb->page_size))		/*出错的话则把原因上传*/
				msb->req = NULL;
		}

		if (msb->req)
			dbg_verbose("IO: request still pending");

		spin_unlock_irqrestore(&msb->q_lock, flags);
	}
}


/*blk_rq_map_sg（）函数*/
int blk_rq_map_sg(struct request_queue *q, struct request *rq,
		struct scatterlist *sglist)
{
	struct scatterlist *sg = NULL;
	int nsegs = 0;

	if (rq->bio)
		nsegs = __blk_bios_map_sg(q, rq->bio, sglist, &sg);
	...
}

static int __blk_bios_map_sg(struct request_queue *q, struct bio *bio,
		struct scatterlist *sglist,
		struct scatterlist **sg)
{
	struct bio_vec bvec, bvprv = { NULL };
	struct bvec_iter iter;
	int nsegs, cluster;

	nsegs = 0;
	cluster = blk_queue_cluster(q);
	...
		for_each_bio(bio)
		bio_for_each_segment(bvec, bio, iter)
		__blk_segment_map_sg(q, &bvec, sglist, &bvprv, sg,
				&nsegs, &cluster);

	return nsegs;
}

	static inline void
__blk_segment_map_sg(struct request_queue *q, struct bio_vec *bvec,
		struct scatterlist *sglist, struct bio_vec *bvprv,
		struct scatterlist **sg, int *nsegs, int *cluster)
{

	int nbytes = bvec->bv_len;

	if (*sg && *cluster) {
		if ((*sg)->length + nbytes > queue_max_segment_size(q))
			goto new_segment;

		if (!BIOVEC_PHYS_MERGEABLE(bvprv, bvec))
			goto new_segment;
		if (!BIOVEC_SEG_BOUNDARY(q, bvprv, bvec))
			goto new_segment;

		(*sg)->length += nbytes;
	} else {
50new_segment:
		if (!*sg)
			*sg = sglist;
		else {
			/*
			   55 * If the driver previously mapped a shorter
			   56 * list, we could see a termination bit
			   57 * prematurely unless it fully inits the sg
			   58 * table on each mapping. We KNOW that there
			   59 * must be more entries here or the driver
			   60 * would be buggy, so force clear the
			   61 * termination bit to avoid doing a full
			   62 * sg_init_table() in drivers for each command.
			   63 */
			sg_unmark_end(*sg);
			*sg = sg_next(*sg);
		}

		sg_set_page(*sg, bvec->bv_page, nbytes, bvec->bv_offset);
		(*nsegs)++;
	}
	*bvprv = *bvec;
}
/*一般情况下，若外设支持scatter/gather模式的DMA操作，紧接着，它就会执行pci_map_sg（）或者
 dma_map_sg（）来进行上述scatter/gather列表的DMA映射了，之后进行硬件的访问*/

/*不使用请求队列
 使用请求队列对于一个机械磁盘设备而言的确有助于提高系统的性能，但是对于RAMDISK、
 ZRAM（Compressed RAM Block Device）等完全可真正随机访问的设备而言，无法从高级的请求队列逻辑
 中获益。对于这些设备，块层支持“无队列”的操作模式，为使用这个模式，驱动必须提供一个“制造请
 求”函数，而不是一个请求处理函数，
 “制造请求”函数的原型为*/

static void xxx_make_request(struct request_queue *queue, struct bio *bio);
/*块设备驱动初始化的时候不再调用blk_init_queue（），而是调用blk_alloc_queue（）和
 blk_queue_make_request（），xxx_make_request则会成为blk_queue_make_request（）的第2个参数。
 xxx_make_request（）函数的第一个参数仍然是“请求队列”
 ，但是这个“请求队列”实际不包含任何请
 求，因为块层没有必要将bio调整为请求。因此，
 “制造请求”函数的主要参数是bio结构体。代码清单13.12
 所示为一个“制造请求”函数的例子，它取材于drivers/block/zram/zram_drv.c。*/

static void zram_make_request(struct request_queue *queue, struct bio *bio)
{
	...
		__zram_make_request(zram, bio);
	...
}

static void __zram_make_request(struct zram *zram, struct bio *bio)
{
	int offset;
	u32index;
	struct bio_vec bvec;
	struct bvec_iter iter;

	index = bio->bi_iter.bi_sector >> SECTORS_PER_PAGE_SHIFT;
	offset = (bio->bi_iter.bi_sector &
			(SECTORS_PER_PAGE - 1)) << SECTOR_SHIFT;

	if (unlikely(bio->bi_rw & REQ_DISCARD)) {
		zram_bio_discard(zram, index, offset, bio);
		bio_endio(bio, 0);
		return;
	}

	bio_for_each_segment(bvec, bio, iter) {
		int max_transfer_size = PAGE_SIZE - offset;

		if (bvec.bv_len > max_transfer_size) {
			/*
			   30 * zram_bvec_rw() can only make operation on a single
			   31 * zram page. Split the bio vector.
			   32 */
			struct bio_vec bv;

			bv.bv_page = bvec.bv_page;
			bv.bv_len = max_transfer_size;
			bv.bv_offset = bvec.bv_offset;

			if (zram_bvec_rw(zram, &bv, index, offset, bio) < 0)
				goto out;

			bv.bv_len = bvec.bv_len - max_transfer_size;
			bv.bv_offset += max_transfer_size;
			if (zram_bvec_rw(zram, &bv, index + 1, 0, bio) < 0)
				goto out;
		} else
			if (zram_bvec_rw(zram, &bvec, index, offset, bio) < 0)
				goto out;

		update_position(&index, &offset, &bvec);
	}

	set_bit(BIO_UPTODATE, &bio->bi_flags);
	bio_endio(bio, 0);
	return;

out:
	bio_io_error(bio);
}
/*上述代码通过bio_for_each_segment（）迭代bio中的每个segement，最终调用zram_bvec_rw（）完成内
 存的压缩、解压、读取和写入。
 ZRAM是Linux的一种内存优化技术，它划定一片内存区域作为SWAP的交换分区，但是它本身具备自
 动压缩功能，从而可以达到辅助Linux匿名页的交换效果，变相“增大”了内存*/
