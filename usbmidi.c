/**
 * See USB Device Class Definition for MIDI Devices,
 * especially Appendix B.
 */

#include <libopencm3/usb/usbd.h>
#include "TUsb.h"

static const struct usb_device_descriptor dev = {
  .bLength = USB_DT_DEVICE_SIZE,
  .bDescriptorType = USB_DT_DEVICE,
  .bcdUSB = 0x0200,
  .bDeviceClass = 0,
  .bDeviceSubClass = 0,
  .bDeviceProtocol = 0,
  .bMaxPacketSize0 = 64,
  .idVendor = 0xdead, //0x1234,
  .idProduct = 0xbeef, //0x1234,
  .bcdDevice = 0x0001, //0x0001,
  .iManufacturer = 1,
  .iProduct = 2,
  .iSerialNumber = 3,
  .bNumConfigurations = 1,
};

struct usb_midi_endpoint_descriptor {
  u8 bLength;
  u8 bDescriptorType;
  u8 bDescriptorSubType;
  u8 bNumEmbMIDIJack;
  u8 BaAssocJackID[1];
} __attribute__((packed));

static const struct usb_midi_endpoint_descriptor midi_ep_extra[2] = {{
    .bLength = sizeof(struct usb_midi_endpoint_descriptor),
    .bDescriptorType = 0x25, // CS_ENDPOINT
    .bDescriptorSubType = 0x01, // MS_GENERAL
    .bNumEmbMIDIJack = 1,
    .BaAssocJackID = { 2 }
  }, {
    .bLength = sizeof(struct usb_midi_endpoint_descriptor),
    .bDescriptorType = 0x25, // CS_ENDPOINT
    .bDescriptorSubType = 0x01, // MS_GENERAL
    .bNumEmbMIDIJack = 1,
    .BaAssocJackID = { 1 }
  }
};

static const struct usb_endpoint_descriptor midi_endpoints[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x01,
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
    .extra = &midi_ep_extra[0],
    .extralen = sizeof(struct usb_midi_endpoint_descriptor)
  }, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x82,
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
    .extra = &midi_ep_extra[1],
    .extralen = sizeof(struct usb_midi_endpoint_descriptor)
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
  .bLength = sizeof(struct usb_audio_class_descriptor_header),
  .bDescriptorType = 0x24, // CS_INTERFACE
  .bDescriptorSubType = 0x01, // HEADER
  .bcdADC = 0x0001,
  .wTotalLength = sizeof(struct usb_audio_class_descriptor_header),
  .bInCollection = 1,
  .baInterfaceNr = 1
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
  struct usb_midistreaming_descriptor_header header;
  struct usb_midistreaming_in_jack_descriptor in;
  struct usb_midistreaming_out_jack_descriptor out;
} __attribute__((packed));

static const struct usb_midistreaming_descriptor midistreaming = {
  { // Descriptor header
    .bLength = sizeof(struct usb_midistreaming_descriptor_header),
    .bDescriptorType = 0x24, // CS_INTERFACE
    .bDescriptorSubType = 0x01, // MS_HEADER
    .bcdADC = 0x0100,
    .wTotalLength = sizeof(struct usb_midistreaming_descriptor_header) + sizeof(struct usb_midistreaming_in_jack_descriptor) + sizeof(struct usb_midistreaming_out_jack_descriptor)
  }, { // MIDI IN jack
    .bLength = sizeof(struct usb_midistreaming_in_jack_descriptor),
    .bDescriptorType = 0x24, // CS_INTERFACE
    .bDescriptorSubType = 0x02, // MIDI_IN_JACK
    .bJackType = 0x01, // EMBEDDED
    .bJackID = 1,
    .iJack = 0
  }, { // MIDI OUT jack
    .bLength = sizeof(struct usb_midistreaming_out_jack_descriptor),
    .bDescriptorType = 0x24, // CS_INTERFACE
    .bDescriptorSubType = 0x03, // MIDI_OUT_JACK
    .bJackType = 0x01, // EMBEDDED
    .bJackID = 2,
    .bNrInputPins = 1,
    .BaSourceID = { 1 },
    .BaSourcePin = { 1 },
    .iJack = 0,
  }};

static const struct usb_interface_descriptor audiocontrol_interface = {
  .bLength = USB_DT_INTERFACE_SIZE,
  .bDescriptorType = USB_DT_INTERFACE,
  .bInterfaceNumber = 0,
  .bAlternateSetting = 0,
  .bNumEndpoints = 0,
  .bInterfaceClass = 0x01, //USB_CLASS_AUDIO
  .bInterfaceSubClass = 0x01, //USB_SUBCLASS_AUDIO_CONTROL
  .bInterfaceProtocol = 0,
  .iInterface = 0,
  .endpoint = 0,
  .extra = &audio_header,
  .extralen = sizeof(audio_header)
};

static const struct usb_interface_descriptor midistreaming_interface = {
  .bLength = USB_DT_INTERFACE_SIZE,
  .bDescriptorType = USB_DT_INTERFACE,
  .bInterfaceNumber = 1,
  .bAlternateSetting = 0,
  .bNumEndpoints = 2,
  .bInterfaceClass = 0x01, //USB_CLASS_AUDIO
  .bInterfaceSubClass = 0x03, //USB_SUBCLASS_MIDISTREAMING
  .bInterfaceProtocol = 0,
  .iInterface = 0,
  .endpoint = midi_endpoints,
  .extra = &midistreaming,
  .extralen = sizeof(midistreaming)
};

static const struct usb_interface interfaces[] = {{
    .num_altsetting = 1,
    .altsetting = &audiocontrol_interface
  }, {
    .num_altsetting = 1,
    .altsetting = &midistreaming_interface
  }};

static const struct usb_config_descriptor config = {
  .bLength = USB_DT_CONFIGURATION_SIZE,
  .bDescriptorType = USB_DT_CONFIGURATION,
  .wTotalLength = 0,
  .bNumInterfaces = 2,
  .bConfigurationValue = 1,
  .iConfiguration = 0,
  .bmAttributes = 0x80,
  .bMaxPower = 0x32, // in 2mA units

  .interface = interfaces,
};

static const char *usb_strings[] = {
  "x",
  "Elemental Instruments Megamind",
  "Knobboard-4",
  "0000000000001",
};

static int ControlRequest(struct usb_setup_data *req, u8 **buf, u16 *len,
			  void (**complete)(struct usb_setup_data *req))
{
  (void)req;
  (void)buf;
  (void)len;
  (void)complete;
  return 0;
}

static void SetConfig(uint16_t value __attribute__((unused)))
{
  usbd_ep_setup(0x01, USB_ENDPOINT_ATTR_BULK, 64, UsbDataCallback);
  usbd_ep_setup(0x82, USB_ENDPOINT_ATTR_BULK, 64, 0);

  usbd_register_control_callback(USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
				 USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
				 ControlRequest);
}

void UsbMidiInit()
{
  usbd_init(&stm32f103_usb_driver, &dev, &config, usb_strings);
  usbd_register_set_config_callback(SetConfig);
}
