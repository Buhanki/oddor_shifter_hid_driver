#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <libusb.h>
#include <fcntl.h>
#include <string.h>

#define VENDOR_ID  	0x4785
#define PRODUCT_ID 	0x7353
#define ENDPOINT_IN 0x81

void emit(int fd, int type, int code, int val)
{
	struct input_event ie;

  	ie.type = type;
	ie.code = code;
	ie.value = val;
  	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0;

	write(fd, &ie, sizeof(ie));
}

int main()
{
	int init = 0;
	int claim = 0; 
	int transferred = 0;
	int resault = 0;

	unsigned char description[68] = {0};
	unsigned char stream[49] = {0};

	libusb_context *ctx = NULL;
	libusb_device_handle * usb_device_handle = NULL;

	struct uinput_setup usetup;

	int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_KEYBIT, BTN_A);
	ioctl(fd, UI_SET_KEYBIT, BTN_B);
	ioctl(fd, UI_SET_KEYBIT, BTN_C);
	ioctl(fd, UI_SET_KEYBIT, BTN_X);
	ioctl(fd, UI_SET_KEYBIT, BTN_Y);
	ioctl(fd, UI_SET_KEYBIT, BTN_TL);
	ioctl(fd, UI_SET_KEYBIT, BTN_TR);
	ioctl(fd, UI_SET_KEYBIT, BTN_SELECT);

	memset(&usetup, 0, sizeof(usetup));
	usetup.id.bustype = BUS_USB;
	usetup.id.vendor = VENDOR_ID;
	usetup.id.product = PRODUCT_ID;
	strcpy(usetup.name, "ODDOR Shifter 7+R");

	ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);
	ioctl(fd, UI_SET_PROPBIT, INPUT_PROP_ACCELEROMETER);

	sleep(1);

	init = libusb_init_context(&ctx, NULL, 0);
	usb_device_handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
	libusb_set_auto_detach_kernel_driver(usb_device_handle, 0);
	libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING );
	claim = libusb_claim_interface(usb_device_handle, 0 );
  
	while (true) {
  		resault = libusb_interrupt_transfer(
			usb_device_handle,
			ENDPOINT_IN,
			stream,
			sizeof(stream), 
			&transferred, 
			1200);
		//printf("%d\n", stream[0]);
		switch (stream[0]) {
		case 1:
			emit(fd, EV_KEY, BTN_A, 1);		
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		case 2:
			emit(fd, EV_KEY, BTN_B, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		case 4:
			emit(fd, EV_KEY, BTN_C, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		case 8:
			emit(fd, EV_KEY, BTN_X, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		case 16:
			emit(fd, EV_KEY, BTN_Y, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		case 32:
			emit(fd, EV_KEY, BTN_TL, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		case 64:
			emit(fd, EV_KEY, BTN_TR, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		case 128:
			emit(fd, EV_KEY, BTN_SELECT, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			break;
		}
	}

	libusb_release_interface( usb_device_handle, 0);
	libusb_close(usb_device_handle);
	libusb_exit(ctx);

	ioctl(fd, UI_DEV_DESTROY);
	close(fd);

	return 0;
}
