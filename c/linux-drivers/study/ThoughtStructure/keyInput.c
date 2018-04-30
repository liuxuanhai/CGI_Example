/******************************************    *******************************
      > File Name: keyInput.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     2017年11月29日 星期三 17时11分30秒
      > Modify Time:
 *********************************    ***************************************/


/*输入设备驱动*/
/*
 * 输入设备（如按键、键盘、触摸屏、鼠标等）是典型的字符设备，其一般的工作机理是底层在按键、
 * 触摸等动作发送时产生一个中断（或驱动通过Timer定时查询），然后CPU通过SPI、I2C或外部存储器总
 * 线读取键值、坐标等数据，并将它们放入一个缓冲区，字符设备驱动管理该缓冲区，而驱动的read（）接
 * 口让用户可以读取键值、坐标等数据。
 * 显然，在这些工作中，只是中断、读键值/坐标值是与设备相关的，而输入事件的缓冲区管理以及字
 * 符设备驱动的file_operations接口则对输入设备是通用的。基于此，内核设计了输入子系统，由核心层处理
 * 公共的工作
 * */

/*输入核心提供了底层输入设备驱动程序所需的API，如分配/释放一个输入设备*/
struct input_dev *input_allocate_device(void);
void input_free_device(struct input_dev *dev);
/*input_allocate_device（）返回的是1个input_dev的结构体，此结构体用于表征1个输入设备*/

/*注册/注销输入设备用的接口如下*/
int __must_check input_register_device(struct input_dev *);
void input_unregister_device(struct input_dev *);
/*报告输入事件用的接口如下：
 * */
/* 报告指定type、code的输入事件 */
void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value);
/* 报告键值 */
void input_report_key(struct input_dev *dev, unsigned int code, int value);
/* 报告相对坐标 */
void input_report_rel(struct input_dev *dev, unsigned int code, int value);
/* 报告绝对坐标 */
void input_report_abs(struct input_dev *dev, unsigned int code, int value);
/* 报告同步事件 */
void input_sync(struct input_dev *dev);
/*而对于所有的输入事件，内核都用统一的数据结构来描述，这个数据结构是input_event，*/
struct input_event {
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
};

/*
drivers/input/keyboard/gpio_keys.c基于input架构实现了一个通用的GPIO按键驱动。该驱动是基于
platform_driver架构的，名为“gpio-keys”
。它将与硬件相关的信息（如使用的GPIO号，按下和抬起时的电
平等）屏蔽在板文件platform_device的platform_data中，因此该驱动可应用于各个处理器，具有良好的跨平
台性
*/

static int gpio_keys_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	const struct gpio_keys_platform_data *pdata = dev_get_platdata(dev);
	struct gpio_keys_drvdata *ddata;
	struct input_dev *input;
	size_t size;
	int i, error;
	int wakeup = 0;

	if (!pdata) {
		pdata = gpio_keys_get_devtree_pdata(dev);
		if (IS_ERR(pdata))
			return PTR_ERR(pdata);
	}

	size = sizeof(struct gpio_keys_drvdata) +
		pdata->nbuttons * sizeof(struct gpio_button_data);
	ddata = devm_kzalloc(dev, size, GFP_KERNEL);
	if (!ddata) {
		dev_err(dev, "failed to allocate state\n");
		return -ENOMEM;
	}

	input = devm_input_allocate_device(dev);
	if (!input) {
		dev_err(dev, "failed to allocate input device\n");
		return -ENOMEM;
	}

	ddata->pdata = pdata;
	ddata->input = input;
	mutex_init(&ddata->disable_lock);

	platform_set_drvdata(pdev, ddata);
	input_set_drvdata(input, ddata);

	input->name = pdata->name : pdev->name;
	input->phys = "gpio-keys/input0";
	input->dev.parent = &pdev->dev;
	input->open = gpio_keys_open;
	input->close = gpio_keys_close;

	input->id.bustype = BUS_HOST;
	input->id.vendor = 0x0001;
	input->id.product = 0x0001;
	input->id.version = 0x0100;

	/* Enable auto repeat feature of Linux input subsystem */
	if (pdata->rep)
		__set_bit(EV_REP, input->evbit);

	for (i = 0; i < pdata->nbuttons; i++) {
		const struct gpio_keys_button *button = &pdata->buttons[i];
		struct gpio_button_data *bdata = &ddata->data[i];

		error = gpio_keys_setup_key(pdev, input, bdata, button);
		if (error)
			return error;

		if (button->wakeup)
			wakeup = 1;
	}

	error = sysfs_create_group(&pdev->dev.kobj, &gpio_keys_attr_group);
	...
		error = input_register_device(input);
	...
}

/*在注册输入设备后，底层输入设备驱动的核心工作只剩下在按键、触摸等人为动作发生时报告事件*/
static irqreturn_t gpio_keys_irq_isr(int irq, void *dev_id)
{
	struct gpio_button_data *bdata = dev_id;
	const struct gpio_keys_button *button = bdata->button;
	struct input_dev *input = bdata->input;
	unsigned long flags;

	BUG_ON(irq != bdata->irq);

	spin_lock_irqsave(&bdata->lock, flags);

	if (!bdata->key_pressed) {
		if (bdata->button->wakeup)
			pm_wakeup_event(bdata->input->dev.parent, 0);

		input_event(input, EV_KEY, button->code, 1);
		input_sync(input);

		if (!bdata->timer_debounce) {
			input_event(input, EV_KEY, button->code, 0);
			input_sync(input);
			goto out;
		}

		bdata->key_pressed = true;
	}

	if (bdata->timer_debounce)
		mod_timer(&bdata->timer,
				jiffies + msecs_to_jiffies(bdata->timer_debounce));
out:
	spin_unlock_irqrestore(&bdata->lock, flags);
	return IRQ_HANDLED;
}

/*
 * GPIO按键驱动通过input_event（）、input_sync（）这样的函数来汇报按键事件以及同步事件。从底
 * 层的GPIO按键驱动可以看出，该驱动中没有任何file_operations的动作，也没有各种I/O模型，注册进入系
 * 统也用的是input_register_device（）这样的与input相关的API
 * */

static ssize_t evdev_read(struct file *file, char __user *buffer,
		size_t count, loff_t *ppos)
{
	struct evdev_client *client = file->private_data;
	struct evdev *evdev = client->evdev;
	struct input_event event;
	size_t read = 0;
	int error;

	if (count != 0&& count < input_event_size())
		return -EINVAL;

	for (;;) {
		if (!evdev->exist || client->revoked)
			return -ENODEV;

		if (client->packet_head == client->tail &&
				(file->f_flags & O_NONBLOCK))
			return -EAGAIN;

		/*
		 * count == 0is special - no IO is done but we check
		 * for error conditions (see above).
		 */
		if (count == 0)
			break;

		while (read + input_event_size() <= count &&
				evdev_fetch_next_event(client, &event)) {

			if (input_event_to_user(buffer + read, &event))
				return -EFAULT;

			read += input_event_size();
		}

		if (read)
			break;

		if (!(file->f_flags & O_NONBLOCK)) {
			error = wait_event_interruptible(evdev->wait,
					client->packet_head != client->tail ||
					!evdev->exist || client->revoked);
			if (error)
				return error;
		}
	}
	return read;
}

static const struct file_operations evdev_fops = {
	.owner = THIS_MODULE,
	.read = evdev_read,
	.write = evdev_write,
	.pol = evdev_poll,
	.open = evdev_open,
	.release = evdev_release,
	.unlocked_ioct= evdev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioct= evdev_ioctl_compat,
#endif
	.fasync = evdev_fasync,
	.flush = evdev_flush,
	.llseek = no_llseek,
};
/*
 * 在检查出是非阻塞访问后，立即返回EAGAIN错误，而第29行和第41~43行的代
 * 码则处理了阻塞的睡眠情况。回过头来想，其实gpio_keys驱动里面调用的input_event（）、input_sync（）
 * 有间接唤醒这个等待队列evdev->wait的功能，只不过这些代码都隐藏在其内部实现里了。
 * */


