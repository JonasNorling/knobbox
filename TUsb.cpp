#include "TUsb.h"
#include "device.h"
#include "TControllers.h"

#ifndef HOST

/**
 * See USB Device Class Definition for MIDI Devices,
 * especially Appendix B.
 */

static const struct usb_device_descriptor dev = {
  /* .bLength = */ USB_DT_DEVICE_SIZE,
  /* .bDescriptorType = */ USB_DT_DEVICE,
  /* .bcdUSB = */ 0x0200,
  /* .bDeviceClass = */ 0,
  /* .bDeviceSubClass = */ 0,
  /* .bDeviceProtocol = */ 0,
  /* .bMaxPacketSize0 = */ 8,
  /* .idVendor = */ 0x1234,
  /* .idProduct = */ 0x1234,
  /* .bcdDevice = */ 0x0001,
  /* .iManufacturer = */ 1,
  /* .iProduct = */ 2,
  /* .iSerialNumber = */ 3,
  /* .bNumConfigurations = */ 1,
};

static const struct usb_endpoint_descriptor data_endp[] = {{
    /* .bLength = */ USB_DT_ENDPOINT_SIZE,
    /* .bDescriptorType = */ USB_DT_ENDPOINT,
    /* .bEndpointAddress = */ 0x01,
    /* .bmAttributes = */ USB_ENDPOINT_ATTR_BULK,
    /* .wMaxPacketSize = */ 64,
    /* .bInterval = */ 0,
  }, {
    /* .bLength = */ USB_DT_ENDPOINT_SIZE,
    /* .bDescriptorType = */ USB_DT_ENDPOINT,
    /* .bEndpointAddress = */ 0x81,
    /* .bmAttributes = */ USB_ENDPOINT_ATTR_BULK,
    /* .wMaxPacketSize = */ 64,
    /* .bInterval = */ 0,
  }};

struct usb_audio_class_descriptor_header {
  u8 bLength;
  u8 bDescriptorType;
  u8 bDescriptorSubType;
  u16 bcdADC;
  u16 wTotalLength;
  u8 bInCollection;
  u8 baInterfaceNr;
} __attribute__((packed));

static const struct usb_audio_class_descriptor_header audio_header = {
  /* .bLength = */ sizeof(usb_audio_class_descriptor_header),
  /* .bDescriptorType = */ 0x24, // CS_INTERFACE
  /* .bDescriptorSubType = */ 0x01, // HEADER
  /* .bcdADC = */ 0x0001,
  /* .wTotalLength = */ sizeof(usb_audio_class_descriptor_header),
  /* .bInCollection = */ 1,
  /* .baInterfaceNr = */ 1
};

struct usb_midistreaming_descriptor_header {
  u8 bLength;
  u8 bDescriptorType;
  u8 bDescriptorSubType;
  u16 bcdADC;
  u16 wTotalLength;
} __attribute__((packed));

struct usb_midistreaming_in_jack_descriptor {
  u8 bLength;
  u8 bDescriptorType;
  u8 bDescriptorSubType;
  u8 bJackType;
  u8 bJackID;
  u8 iJack;
} __attribute__((packed));

struct usb_midistreaming_out_jack_descriptor {
  u8 bLength;
  u8 bDescriptorType;
  u8 bDescriptorSubType;
  u8 bJackType;
  u8 bJackID;
  u8 bNrInputPins;
  u8 BaSourceID[1];
  u8 BaSourcePin[1];
  u8 iJack;
} __attribute__((packed));

struct usb_midistreaming_descriptor {
  usb_midistreaming_descriptor_header header;
  usb_midistreaming_in_jack_descriptor in;
  usb_midistreaming_out_jack_descriptor out;
} __attribute__((packed));

static const struct usb_midistreaming_descriptor midistreaming = {
  { // Descriptor header
    /* .bLength = */ sizeof(usb_midistreaming_descriptor_header),
    /* .bDescriptorType = */ 0x24, // CS_INTERFACE
    /* .bDescriptorSubType = */ 0x01, // MS_HEADER
    /* .bcdADC = */ 0x0100,
    /* .wTotalLength = */ 0
  }, { // MIDI IN jack
    /* .bLength = */ sizeof(usb_midistreaming_in_jack_descriptor),
    /* .bDescriptorType = */ 0x24, // CS_INTERFACE
    /* .bDescriptorSubType = */ 0x02, // MIDI_IN_JACK
    /* .bJackType = */ 0x01, // EMBEDDED
    /* .bJackID = */ 1,
    /* .iJack = */ 0
  }, { // MIDI OUT jack
    /* .bLength = */ sizeof(usb_midistreaming_in_jack_descriptor),
    /* .bDescriptorType = */ 0x24, // CS_INTERFACE
    /* .bDescriptorSubType = */ 0x03, // MIDI_OUT_JACK
    /* .bJackType = */ 0x01, // EMBEDDED
    /* .bJackID = */ 2,
    /* .bNrInputPins = */ 1,
    /* .BaSourceID = */ { 1 },
    /* .BaSourcePin = */ { 1 },
    /* .iJack = */ 0,
  }};

static const struct usb_interface_descriptor interface_descs[] = {{
    /* .bLength = */ USB_DT_INTERFACE_SIZE,
    /* .bDescriptorType = */ USB_DT_INTERFACE,
    /* .bInterfaceNumber = */ 0,
    /* .bAlternateSetting = */ 0,
    /* .bNumEndpoints = */ 0,
    /* .bInterfaceClass = */ 0x01, //USB_CLASS_AUDIO
    /* .bInterfaceSubClass = */ 0x01, //USB_SUBCLASS_AUDIO_CONTROL
    /* .bInterfaceProtocol = */ 0,
    /* .iInterface = */ 0,
    /* .endpoint = */ 0,
    /* .extra = */ &audio_header,
    /* .extralen = */ sizeof(audio_header)
  }, {
    /* .bLength = */ USB_DT_INTERFACE_SIZE,
    /* .bDescriptorType = */ USB_DT_INTERFACE,
    /* .bInterfaceNumber = */ 1,
    /* .bAlternateSetting = */ 0,
    /* .bNumEndpoints = */ 2,
    /* .bInterfaceClass = */ 0x01, //USB_CLASS_AUDIO
    /* .bInterfaceSubClass = */ 0x03, //USB_SUBCLASS_MIDISTREAMING
    /* .bInterfaceProtocol = */ 0,
    /* .iInterface = */ 0,
    /* .endpoint = */ 0,
    /* .extra = */ &midistreaming,
    /* .extralen = */ sizeof(midistreaming)
  }};

static const usb_config_descriptor::usb_interface interface = {
  /* .num_altsetting = */ 1,
  /* .iface_assoc = */ 0,
  /* .altsetting = */ interface_descs
};

static const struct usb_config_descriptor config = {
  /* .bLength = */ USB_DT_CONFIGURATION_SIZE,
  /* .bDescriptorType = */ USB_DT_CONFIGURATION,
  /* .wTotalLength = */ 0,
  /* .bNumInterfaces = */ 1,
  /* .bConfigurationValue = */ 1,
  /* .iConfiguration = */ 0,
  /* .bmAttributes = */ 0x80,
  /* .bMaxPower = */ 0x32, // in 2mA units

  /* .interface = */ &interface,
};

static const char *usb_strings[] = {
  "x",
  "Manufacturer",
  "Product",
  "Serial",
};

#endif

void TUsb::SetConfig(uint16_t value)
{
  Controllers.IncreaseValue(3, 1);
  /*
  usbd_ep_setup(0x01, USB_ENDPOINT_ATTR_BULK, 64, cdcacm_data_rx_cb);
  usbd_ep_setup(0x82, USB_ENDPOINT_ATTR_BULK, 64, NULL);
  usbd_ep_setup(0x83, USB_ENDPOINT_ATTR_INTERRUPT, 16, NULL);

  usbd_register_control_callback(
				 USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
				 USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
				 cdcacm_control_request);
  */
}

void TUsb::Init()
{
#ifndef HOST
  usbd_init(&stm32f103_usb_driver, &dev, &config, usb_strings);
  usbd_register_set_config_callback(SetConfig);
#endif
}
