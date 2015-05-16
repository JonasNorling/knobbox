/* -*- c++ -*- */
/* This font is based on the Liquid font family:
 * http://www.theiling.de/projects/liquid.html
 */

static const int FONT_WIDTH = 6;
static const int FONT_GLYPHS = 128;

const uint8_t Font[FONT_GLYPHS][FONT_WIDTH] = {
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x00 */
  { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }, /* \001 top line */
  { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 }, /* \002 bottom line */
  { 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* \003 left line */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0xff }, /* \004 right line */
  { 0xf8, 0x04, 0x02, 0x01, 0x01, 0x01 }, /* \005 top left */
  { 0x01, 0x01, 0x01, 0x01, 0x01, 0xff }, /* \006 top right */
  { 0xff, 0x80, 0x80, 0x80, 0x80, 0x80 }, /* \007 bottom left */
  { 0x80, 0x80, 0x80, 0x80, 0x80, 0xff }, /* \010 bottom right */
  { 0x60, 0x78, 0x7e, 0x78, 0x60, 0x00 }, /* \011 up arrow */
  { 0x06, 0x1e, 0x7e, 0x1e, 0x06, 0x00 }, /* \012 down arrow */
  { 0x00, 0x00, 0x7e, 0x3c, 0x18, 0x00 }, /* \013, play symbol */
  { 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00 }, /* \014, stop symbol */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },

  { 0xff, 0xff, 0x07, 0x01, 0x01, 0x00 }, /* 0x10, \020: Tab left */
  { 0x00, 0x01, 0x01, 0x07, 0xff, 0xff }, /* 0x11, \021: Tab right */
  { 0x20, 0x00, 0x20, 0x00, 0x20, 0x00 }, /* 0x12, \022: ellipsis */
  { 0x02, 0x3f, 0x00, 0x00, 0x00, 0x03 }, /* 0x13, \023: 1' */
  { 0x31, 0x29, 0x26, 0x00, 0x00, 0x03 }, /* 0x14, \023: 2' */
  { 0x21, 0x25, 0x1b, 0x00, 0x00, 0x03 }, /* 0x15, \023: 3' */
  { 0x0e, 0x08, 0x3f, 0x00, 0x00, 0x03 }, /* 0x16, \023: 4' */
  { 0x27, 0x25, 0x19, 0x00, 0x00, 0x03 }, /* 0x17, \023: 5' */
  { 0x00, 0x60, 0xf0, 0xf0, 0x7f, 0x15 }, /* 0x18, \030: thirtysecond note */
  { 0x00, 0x60, 0xf0, 0xf0, 0x7f, 0x05 }, /* 0x19, \031: sixteenth note */
  { 0x00, 0x60, 0xf0, 0xf0, 0x7f, 0x01 }, /* 0x1a, \032: eigth note */
  { 0x00, 0x60, 0xf0, 0xf0, 0x7f, 0x00 }, /* 0x1b, \033: quarter note */
  { 0x00, 0x60, 0x90, 0x90, 0x7f, 0x00 }, /* 0x1c, \034: half note */
  { 0x00, 0x18, 0x24, 0x24, 0x18, 0x00 }, /* 0x1d, \035: whole note */
  { 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00 }, /* 0x1e, \036: marker */
  { 0x0e, 0x11, 0x11, 0xf8, 0x20, 0xf8 }, /* 0x1f, \037: CH */

  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  /* 0x20 */
  { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },
  { 0x00, 0x00, 0x03, 0x00, 0x03, 0x00 },
  { 0x00, 0x12, 0x3f, 0x12, 0x3f, 0x12 },
  { 0x00, 0x24, 0x7a, 0x2f, 0x12, 0x00 },
  { 0x00, 0x12, 0x08, 0x24, 0x00, 0x00 },
  { 0x00, 0x1a, 0x25, 0x2a, 0x10, 0x28 },
  { 0x00, 0x00, 0x00, 0x03, 0x00, 0x00 },
  { 0x00, 0x00, 0x1e, 0x21, 0x00, 0x00 },
  { 0x00, 0x00, 0x21, 0x1e, 0x00, 0x00 },
  { 0x00, 0x00, 0x2a, 0x1c, 0x2a, 0x00 },
  { 0x00, 0x00, 0x08, 0x1c, 0x08, 0x00 },
  { 0x00, 0x00, 0x40, 0x30, 0x00, 0x00 },
  { 0x00, 0x00, 0x08, 0x08, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x20, 0x00, 0x00 },
  { 0x00, 0x30, 0x0c, 0x03, 0x00, 0x00 },

  { 0x00, 0x00, 0x1e, 0x21, 0x1e, 0x00 },  /* 0x30 */
  { 0x00, 0x00, 0x02, 0x3f, 0x00, 0x00 },
  { 0x00, 0x00, 0x31, 0x29, 0x26, 0x00 },
  { 0x00, 0x00, 0x21, 0x25, 0x1b, 0x00 },
  { 0x00, 0x00, 0x0e, 0x08, 0x3f, 0x00 },
  { 0x00, 0x00, 0x27, 0x25, 0x19, 0x00 },
  { 0x00, 0x00, 0x1e, 0x25, 0x19, 0x00 },
  { 0x00, 0x00, 0x01, 0x39, 0x07, 0x00 },
  { 0x00, 0x00, 0x1a, 0x25, 0x1a, 0x00 },
  { 0x00, 0x00, 0x26, 0x29, 0x1e, 0x00 },
  { 0x00, 0x00, 0x00, 0x24, 0x00, 0x00 },
  { 0x00, 0x00, 0x80, 0x64, 0x00, 0x00 },
  { 0x00, 0x00, 0x08, 0x14, 0x22, 0x00 },
  { 0x00, 0x00, 0x14, 0x14, 0x14, 0x00 },
  { 0x00, 0x00, 0x22, 0x14, 0x08, 0x00 },
  { 0x00, 0x00, 0x29, 0x05, 0x02, 0x00 },

  { 0x00, 0x3e, 0x41, 0x49, 0x55, 0x1e },  /* 0x40 */
  { 0x00, 0x00, 0x3e, 0x09, 0x3e, 0x00 },
  { 0x00, 0x00, 0x3f, 0x25, 0x1a, 0x00 },
  { 0x00, 0x00, 0x1e, 0x21, 0x21, 0x00 },
  { 0x00, 0x00, 0x3f, 0x21, 0x1e, 0x00 },
  { 0x00, 0x00, 0x3f, 0x25, 0x21, 0x00 },
  { 0x00, 0x00, 0x3f, 0x05, 0x01, 0x00 },
  { 0x00, 0x00, 0x1e, 0x21, 0x39, 0x00 },
  { 0x00, 0x00, 0x3f, 0x04, 0x3f, 0x00 },
  { 0x00, 0x00, 0x21, 0x3f, 0x21, 0x00 },
  { 0x00, 0x00, 0x40, 0x3f, 0x00, 0x00 },
  { 0x00, 0x00, 0x3f, 0x04, 0x3b, 0x00 },
  { 0x00, 0x00, 0x3f, 0x20, 0x20, 0x00 },
  { 0x00, 0x3f, 0x02, 0x04, 0x02, 0x3f },
  { 0x00, 0x3f, 0x02, 0x04, 0x3f, 0x00 },
  { 0x00, 0x00, 0x1e, 0x21, 0x1e, 0x00 },

  { 0x00, 0x00, 0x3f, 0x09, 0x06, 0x00 },  /* 0x50 */
  { 0x00, 0x00, 0x1e, 0x61, 0x5e, 0x00 },
  { 0x00, 0x00, 0x3f, 0x09, 0x36, 0x00 },
  { 0x00, 0x00, 0x22, 0x25, 0x19, 0x00 },
  { 0x00, 0x00, 0x01, 0x3f, 0x01, 0x00 },
  { 0x00, 0x00, 0x1f, 0x20, 0x3f, 0x00 },
  { 0x00, 0x00, 0x3f, 0x10, 0x0f, 0x00 },
  { 0x00, 0x1f, 0x20, 0x18, 0x20, 0x1f },
  { 0x00, 0x00, 0x33, 0x0c, 0x33, 0x00 },
  { 0x00, 0x00, 0x07, 0x38, 0x07, 0x00 },
  { 0x00, 0x00, 0x31, 0x2d, 0x23, 0x00 },
  { 0x00, 0x00, 0x3f, 0x21, 0x00, 0x00 },
  { 0x00, 0x00, 0x03, 0x0c, 0x30, 0x00 },
  { 0x00, 0x00, 0x21, 0x3f, 0x00, 0x00 },
  { 0x00, 0x04, 0x02, 0x3f, 0x02, 0x04 },
  { 0x00, 0x00, 0x40, 0x40, 0x40, 0x00 },

  { 0x00, 0x24, 0x3e, 0x25, 0x21, 0x00 },  /* 0x60 */
  { 0x00, 0x00, 0x18, 0x24, 0x3c, 0x00 },
  { 0x00, 0x00, 0x3f, 0x24, 0x18, 0x00 },
  { 0x00, 0x00, 0x18, 0x24, 0x00, 0x00 },
  { 0x00, 0x00, 0x18, 0x24, 0x3f, 0x00 },
  { 0x00, 0x00, 0x18, 0x34, 0x2c, 0x00 },
  { 0x00, 0x00, 0x00, 0x3e, 0x05, 0x00 },
  { 0x00, 0x00, 0x98, 0xa4, 0x7c, 0x00 },
  { 0x00, 0x00, 0x3f, 0x04, 0x38, 0x00 },
  { 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00 },
  { 0x00, 0x00, 0x80, 0x7d, 0x00, 0x00 },
  { 0x00, 0x00, 0x3f, 0x08, 0x34, 0x00 },
  { 0x00, 0x00, 0x01, 0x3f, 0x00, 0x00 },
  { 0x00, 0x3c, 0x04, 0x3c, 0x04, 0x38 },
  { 0x00, 0x00, 0x3c, 0x04, 0x38, 0x00 },
  { 0x00, 0x00, 0x18, 0x24, 0x18, 0x00 },

  { 0x00, 0x00, 0xfc, 0x24, 0x18, 0x00 },  /* 0x70 */
  { 0x00, 0x00, 0x18, 0x24, 0xfc, 0x00 },
  { 0x00, 0x00, 0x38, 0x04, 0x00, 0x00 },
  { 0x00, 0x00, 0x2c, 0x34, 0x00, 0x00 },
  { 0x00, 0x00, 0x1e, 0x24, 0x00, 0x00 },
  { 0x00, 0x00, 0x1c, 0x20, 0x3c, 0x00 },
  { 0x00, 0x00, 0x3c, 0x10, 0x0c, 0x00 },
  { 0x00, 0x1c, 0x20, 0x18, 0x20, 0x1c },
  { 0x00, 0x00, 0x24, 0x18, 0x24, 0x00 },
  { 0x00, 0x00, 0x9c, 0x60, 0x1c, 0x00 },
  { 0x00, 0x00, 0x34, 0x2c, 0x00, 0x00 },
  { 0x00, 0x00, 0x04, 0x3b, 0x21, 0x00 },
  { 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00 },
  { 0x00, 0x21, 0x3b, 0x04, 0x00, 0x00 },
  { 0x00, 0x02, 0x01, 0x02, 0x01, 0x00 },
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }  /* 0x7f */
};