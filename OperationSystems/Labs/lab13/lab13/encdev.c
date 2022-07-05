#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/fs.h>       /* for register_chrdev */
#include <linux/uaccess.h>  /* for get_user and put_user */
MODULE_LICENSE("GPL");

#define DEVICE_RANGE_NAME "encdev"
#define IOCTL_SET_KEY 20
#define IOCTL_SET_ENCRTPY 30
#define BUF_LEN 128

static int major; // my uniqe id
static int isEncrypt = 0; // if to output encrtpt data
static int key = 0; // key that encrtpyion is done by
static char message[BUF_LEN] = { 0 };

int device_open(struct inode* inode, struct file* filp) {
	key = 0;
	isEncrypt = 0;
	return 0;
}

int device_release(struct inode* inode, struct file* filp) {
	return 0;
}

static ssize_t device_read(struct file* file,
	char __user* buffer,
	size_t       length,
	loff_t* offset) {
	int i;
	char encrtptedChar;
	printk("Invoking device read(%p,%ld): %s\n", file, length, message);

	for (i = 0; i < length && message[*offset + i] && *offset + i < BUF_LEN; ++i) {
		encrtptedChar = message[*offset + i];
		if (isEncrypt) // if to decrypt
			encrtptedChar = encrtptedChar ^ key;
		put_user(encrtptedChar, &buffer[i]);
	}

	*offset += i;
	return i;
}

static ssize_t device_write(struct file* file,
	const char __user* buffer,
	size_t length,
	loff_t* offset) {
	int i;
	printk("Invoking device write(%p,%ld)\n", file, length);

	for (i = 0; i < length && i < BUF_LEN; ++i) {
		get_user(message[i], &buffer[i]);
		message[i] = message[i] ^ key; // encrypt write
	}

	// return the number of input characters used
	return i;
}

static long device_ioctl(struct file* file,
	unsigned int ioctl_command_id,
	unsigned long ioctl_param) {

	if (IOCTL_SET_KEY == ioctl_command_id) { // setting key
		key = ioctl_param;
	}
	if (IOCTL_SET_ENCRTPY == ioctl_command_id) { // setting if to encrypt
		isEncrypt = ioctl_param;
	}
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	.unlocked_ioctl = device_ioctl,
};

int init_module(void) {
	major = register_chrdev(0, DEVICE_RANGE_NAME, &fops);
	if (major < 0) {
		printk(KERN_ALERT "%s registartion failed: %d\n",
			DEVICE_RANGE_NAME, major);
		return major;
	}

	printk("Registration is successful\n");
	printk("Major device number is: %d\n", major);
	printk("To talk to the device driver, create a device file:\n");
	printk("mknod /dev/<name> c %d 0\n", major);
	printk("Use echo/cat to/form device file\n");
	printk("Don't forget to rm the device file and rmmod the module!\n");
	return 0;
}

void cleanup_module(void) {
	unregister_chrdev(major, DEVICE_RANGE_NAME);
	printk("Module %s unregistered\n", DEVICE_RANGE_NAME);
}