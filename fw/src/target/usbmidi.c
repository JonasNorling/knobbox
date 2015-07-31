/**
 * See USB Device Class Definition for MIDI Devices,
 * especially Appendix B.
 */

#include <stddef.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/audio.h>
#include <libopencm3/usb/midi.h>
#include "TUsb.h"
#include "logging.h"

static uint8_t usbd_control_buffer[128];

static const struct usb_device_descriptor dev = {
        .bLength = USB_DT_DEVICE_SIZE,
        .bDescriptorType = USB_DT_DEVICE,
        .bcdUSB = 0x0200,
        .bDeviceClass = 0,
        .bDeviceSubClass = 0,
        .bDeviceProtocol = 0,
        .bMaxPacketSize0 = 64,
        .idVendor = 0xdead,
        .idProduct = 0xbeef,
        .bcdDevice = 0x0001,
        .iManufacturer = 1, // String index
        .iProduct = 2, // String index
        .iSerialNumber = 3, // String index
        .bNumConfigurations = 1,
};

static const struct usb_midi_endpoint_descriptor midi_ep[] = {{
        .head = {
                .bLength = sizeof(struct usb_midi_endpoint_descriptor),
                .bDescriptorType = USB_AUDIO_DT_CS_ENDPOINT,
                .bDescriptorSubType = USB_MIDI_SUBTYPE_MS_GENERAL,
                .bNumEmbMIDIJack = 1,
        },
        .jack[0] = {
                .baAssocJackID = 2
        }
}, {
        .head = {
                .bLength = sizeof(struct usb_midi_endpoint_descriptor),
                .bDescriptorType = USB_AUDIO_DT_CS_ENDPOINT,
                .bDescriptorSubType = USB_MIDI_SUBTYPE_MS_GENERAL,
                .bNumEmbMIDIJack = 1,
        },
        .jack[0] = {
                .baAssocJackID = 1
        }
}};

static const struct usb_endpoint_descriptor midi_endpoints[] = {{
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = 0x01,
        .bmAttributes = USB_ENDPOINT_ATTR_BULK,
        .wMaxPacketSize = MIDI_EP_MAX_SIZE,
        .bInterval = 0,
        .extra = &midi_ep[0],
        .extralen = sizeof(struct usb_midi_endpoint_descriptor)
}, {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = 0x81,
        .bmAttributes = USB_ENDPOINT_ATTR_BULK,
        .wMaxPacketSize = MIDI_EP_MAX_SIZE,
        .bInterval = 0,
        .extra = &midi_ep[1],
        .extralen = sizeof(struct usb_midi_endpoint_descriptor)
}};

struct usb_audio_header_descriptor {
    struct usb_audio_header_descriptor_head head;
    struct usb_audio_header_descriptor_body body;
} __attribute__((packed));

static const struct usb_audio_header_descriptor audio_header = {
        .head = {
                .bLength = sizeof(struct usb_audio_header_descriptor),
                .bDescriptorType = USB_AUDIO_DT_CS_INTERFACE,
                .bDescriptorSubtype = USB_AUDIO_TYPE_HEADER,
                .bcdADC = 0x0001,
                .wTotalLength = sizeof(struct usb_audio_header_descriptor),
                .binCollection = 1,
        },
        .body = {
                .baInterfaceNr = 1
        }
};

struct usb_midistreaming_descriptor {
    struct usb_midi_header_descriptor header;
    struct usb_midi_in_jack_descriptor in;
    struct usb_midi_out_jack_descriptor out;
} __attribute__((packed));

static const struct usb_midistreaming_descriptor midistreaming = {
        .header = {
                .bLength = sizeof(struct usb_midi_header_descriptor),
                .bDescriptorType = USB_AUDIO_DT_CS_INTERFACE,
                .bDescriptorSubtype = USB_MIDI_SUBTYPE_MS_HEADER,
                .bcdMSC = 0x0100,
                .wTotalLength = sizeof(struct usb_midistreaming_descriptor)
        },
        .in = {
                .bLength = sizeof(struct usb_midi_in_jack_descriptor),
                .bDescriptorType = USB_AUDIO_DT_CS_INTERFACE,
                .bDescriptorSubtype = USB_MIDI_SUBTYPE_MIDI_IN_JACK,
                .bJackType = USB_MIDI_JACK_TYPE_EMBEDDED,
                .bJackID = 1,
                .iJack = 0
        },
        .out = {
                .head = {
                        .bLength = sizeof(struct usb_midi_out_jack_descriptor),
                        .bDescriptorType = USB_AUDIO_DT_CS_INTERFACE,
                        .bDescriptorSubtype = USB_MIDI_SUBTYPE_MIDI_OUT_JACK,
                        .bJackType = USB_MIDI_JACK_TYPE_EMBEDDED,
                        .bJackID = 2,
                        .bNrInputPins = 1,
                },
                .source[0] = {
                        .baSourceID = 1,
                        .baSourcePin = 1,
                },
                .tail = {
                        .iJack = 0,
                }
        }};

static const struct usb_interface_descriptor audiocontrol_interface = {
        .bLength = USB_DT_INTERFACE_SIZE,
        .bDescriptorType = USB_DT_INTERFACE,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = USB_CLASS_AUDIO,
        .bInterfaceSubClass = USB_AUDIO_SUBCLASS_CONTROL,
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
        .bInterfaceClass = USB_CLASS_AUDIO,
        .bInterfaceSubClass = USB_AUDIO_SUBCLASS_MIDISTREAMING,
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
        .bMaxPower = 100, // in 2mA units

        .interface = interfaces,
};

static const char *usb_strings[] = {
        "Elemental Instruments Megamind",
        "Knobboard",
        "0000000000001",
        "yyy"
};

static int ControlRequest(usbd_device *usbd_dev, struct usb_setup_data *req,
        uint8_t **buf, uint16_t *len,
        usbd_control_complete_callback *complete)
{
    LOG("USB control request\n");
    (void)usbd_dev;
    (void)req;
    (void)buf;
    (void)len;
    (void)complete;
    return 0;
}

static void SetConfig(usbd_device *usbd_dev, uint16_t value __attribute__((unused)))
{
    LOG("USB set config\n");
    usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, MIDI_EP_MAX_SIZE, UsbDataCallback);
    usbd_ep_setup(usbd_dev, 0x81, USB_ENDPOINT_ATTR_BULK, MIDI_EP_MAX_SIZE, NULL);

    usbd_register_control_callback(usbd_dev, USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
            USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
            ControlRequest);
}

static void ResetCallback(void)
{
    LOG("USB reset\n");
}

usbd_device* UsbMidiInit()
{
    LOG("USB init\n");
    usbd_device* d = usbd_init(&stm32f103_usb_driver, &dev, &config,
            usb_strings, sizeof(usb_strings)/sizeof(usb_strings[0]),
            usbd_control_buffer, sizeof(usbd_control_buffer));

    usbd_register_set_config_callback(d, SetConfig);
    usbd_register_reset_callback(d, ResetCallback);
    return d;
}
