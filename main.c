#include <linux/input-event-codes.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>

#include <linux/uinput.h>
#include <hidapi.h>

#include <wchar.h>

#define VENDOR_ID  	0x4785
#define PRODUCT_ID 	0x7353
#define ENDPOINT_IN 0x81
#define MAX_STR 255

int main()
{
	while (1) {
		int resault = 0;
		int fd = 0;
		int rc = 1;
	
		struct libevdev *dev = NULL;
		struct libevdev_uinput *uinput_dev = NULL;
		
		wchar_t wstr[MAX_STR];
	
		unsigned char description[68] = {0};
		unsigned char stream[49] = {0};
		
		hid_device *usb_device_handle;
		resault = hid_init();
		while (1) {
			usb_device_handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
			if (usb_device_handle)
				break;
			sleep(2);
		}
	
		resault = hid_get_manufacturer_string(usb_device_handle, wstr, MAX_STR);	
		printf("%ls\n", wstr);
	
		resault = hid_get_product_string(usb_device_handle, wstr, MAX_STR);	
		printf("%ls\n", wstr);
		
		resault = hid_get_serial_number_string(usb_device_handle, wstr, MAX_STR);	
		printf("%d - %ls\n", wstr[0], wstr);
	
		resault = hid_get_indexed_string(usb_device_handle, 1, wstr, MAX_STR);	
		printf("%ls\n", wstr);
	
		//fd = open("/dev/input/event0", O_RDONLY|O_NONBLOCK);
		//rc = libevdev_new_from_fd(fd, &dev);
		dev = libevdev_new();
	
		libevdev_set_name(dev, "ODDOR Shifter 7+R");
		libevdev_set_id_bustype(dev, BUS_USB);
		libevdev_set_id_vendor(dev, VENDOR_ID);
		libevdev_set_id_product(dev, PRODUCT_ID);
	
		libevdev_enable_event_type(dev, EV_KEY);
		libevdev_enable_event_code(dev, EV_KEY, BTN_0, NULL);
		libevdev_enable_event_code(dev, EV_KEY, BTN_1, NULL);
		libevdev_enable_event_code(dev, EV_KEY, BTN_2, NULL);
		libevdev_enable_event_code(dev, EV_KEY, BTN_3, NULL);
		libevdev_enable_event_code(dev, EV_KEY, BTN_4, NULL);
		libevdev_enable_event_code(dev, EV_KEY, BTN_5, NULL);
		libevdev_enable_event_code(dev, EV_KEY, BTN_6, NULL);
		libevdev_enable_event_code(dev, EV_KEY, BTN_7, NULL);

		libevdev_enable_event_type(dev, EV_ABS);
		libevdev_enable_event_code(dev, EV_ABS, ABS_X, NULL);
		libevdev_enable_event_code(dev, EV_ABS, ABS_Y, NULL);
	
		libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uinput_dev);
	
		sleep(1);	
	
	  	while (1) {
			resault = hid_read(usb_device_handle, stream, 49);
			if (resault == -1) {
				break;
			}
			switch (stream[0]) {
			case 0 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_0, 0);
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_1, 0);
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_2, 0);
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_3, 0);
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_4, 0);
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_5, 0);
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_6, 0);
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_7, 0);
	    			libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 1 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_0, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 2 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_1, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 4 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_2, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 8 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_3, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 16 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_4, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 32 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_5, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 64 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_6, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			case 128 :
				libevdev_uinput_write_event(uinput_dev, EV_KEY, BTN_7, 1);
				libevdev_uinput_write_event(uinput_dev, EV_SYN, SYN_REPORT, 0);
				break;
			}
		}
	
		libevdev_uinput_destroy(uinput_dev);
		libevdev_free(dev);
	
		hid_close(usb_device_handle);
		resault = hid_exit();
		sleep(5);
	}
	return 0;
}
