#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kern_levels.h>
#include <linux/usb.h>
// #include <linux/fs.h>
// #include <linux/buffer_head.h>
 
#define IS_NEW_METHOD_USED  ( 0 )
#define USB_VENDOR_ID       ( 0x2e8a )      //USB device's vendor ID  2e8a:0005
#define USB_PRODUCT_ID      ( 0x0005 )      //USB device's product ID


static int usb_device_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    // Device plugged in
    // printk(KERN_INFO "\n*****USB device plugged in*****\n\n");

    dev_info(&interface->dev, "***** USB Driver Probed: Vendor ID : 0x%02x,\t"
                "Product ID : 0x%02x ******\n", id->idVendor, id->idProduct);
    printk(KERN_INFO "USB LICENECE GRANTED\n");

    return 0;  //return 0 indicates we are managing this device
}

static void usb_device_disconnect(struct usb_interface *interface)
{
    // Device removed
    // printk(KERN_INFO "\n*****USB device removed*****\n\n");
    // dev_info(&interface->dev, "\n*****USB Driver Disconnected*****\n\n");
    //printk(KERN_INFO "\n*****USB Device Disconnected*****\n\n");
    dev_info(&interface->dev, "*****USB Device Disconnected*****\n");
    printk(KERN_INFO "USB LICENECE REVOKED\n");
}

static struct usb_device_id usb_device_table[] = {
    { USB_DEVICE(USB_VENDOR_ID,USB_PRODUCT_ID) }, 
    { } // Terminating entry        // driver will detect devices with
                                    // vendor ID 0x1234 and product ID 0x5678
                                    // where USB_DEVICE(vendor_ID, product_ID);
};   

static struct usb_driver usb_device_driver = {
    .name = "USB_KEY_DRIVER",
    .id_table = usb_device_table,
    .probe = usb_device_probe,
    .disconnect = usb_device_disconnect,
};

MODULE_DEVICE_TABLE(usb, usb_device_table);

#if ( IS_NEW_METHOD_USED == 1 )
//This will replaces module_init and module_exit.
module_usb_driver(usb_device_driver);

#else
static int __init usb_device_init(void)
{
    // Register the driver
    int ret = usb_register(&usb_device_driver);
    if (ret < 0) {
                pr_err("usb_register failed for the %s driver. Error number %d\n",
                       usb_device_driver.name, ret);
                return -1;
        }
    else{
        printk(KERN_INFO "\n*****USB DETECTOR DRIVER STARTED*****\n\n");
    }
    return 0;
}

static void __exit usb_device_exit(void)
{
    // Unregister the driver
    usb_deregister(&usb_device_driver);
    printk(KERN_INFO "\n*****USB DETECTOR DRIVER ENDED*****\n\n");

}

module_init(usb_device_init);
module_exit(usb_device_exit);
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Angel Lopez-Montano, Samuel");
MODULE_DESCRIPTION("USB device driver");
