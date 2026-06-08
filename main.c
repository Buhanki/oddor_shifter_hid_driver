#include "stdio.h"
#include "libusb.h"
#include "hidapi_libusb.h"

#define VENDOR_ID  	0x4785
#define PRODUCT_ID 	0x7353
#define ENDPOINT_IN 0x81

int main(){
	libusb_context *ctx = NULL;
	int init = 0;
	libusb_device_handle * usb_device_handle = NULL;
  int claim = 0; 
	int transferred = 0;
	int resault = 0;
	unsigned char description[68] = {0};
	unsigned char stream[49] = {0};

	init = libusb_init_context(&ctx, NULL, 0);

	usb_device_handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
	libusb_set_auto_detach_kernel_driver(usb_device_handle, 0);

	if(init < 0)
		return init;

	libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING );

	if(!usb_device_handle)
		printf("Error device not found\n");
	else
		printf("Device found\n");
	
	claim = libusb_claim_interface(usb_device_handle, 0 );
  
	if(claim < 0)
		printf("Device not claimed\n");

	while(true){
  	resault = libusb_interrupt_transfer(usb_device_handle, ENDPOINT_IN, stream, sizeof(stream), &transferred, 100);
		printf("%d\n", stream[0]);
	}

	if(resault == 0)
		for(int i = 0; i < transferred; i++)
			printf("%d ", stream[i]);

	libusb_release_interface( usb_device_handle, 0);
	libusb_close(usb_device_handle);
	libusb_exit(ctx);
	return 0;
}
