# Devices 
Browsing the linux api... What can I learn? 

## The Linux FrameBuffer 
Applications get information about the display using FBIOGET_FSCREENINFO and FBIOGET_VSCREENINFO ioctls, using a pointer to a fb_fix_screeninfo and fb_var_screeninfo structure respectively.

The "struct fb_fix_screeninfo stores device independent unchangeable information about the frame buffer device and the current format"[1] and should be treated as immutabe properties.

```
struct fb_fix_screeninfo {
      char id[16];                    /* identification string eg "TT Builtin" */
      unsigned long smem_start;       /* Start of frame buffer mem */
                                      /* (physical address) */
      __u32 smem_len;                 /* Length of frame buffer mem */
      __u32 type;                     /* see FB_TYPE_*                */
      __u32 type_aux;                 /* Interleave for interleaved Planes */
      __u32 visual;                   /* see FB_VISUAL_*              */
      __u16 xpanstep;                 /* zero if no hardware panning  */
      __u16 ypanstep;                 /* zero if no hardware panning  */
      __u16 ywrapstep;                /* zero if no hardware ywrap    */
      __u32 line_length;              /* length of a line in bytes    */
      unsigned long mmio_start;       /* Start of Memory Mapped I/O   */
                                      /* (physical address) */
      __u32 mmio_len;                 /* Length of Memory Mapped I/O  */
      __u32 accel;                    /* Indicate to driver which     */
                                      /*  specific chip/card we have  */
      __u16 capabilities;             /* see FB_CAP_*                 */
      __u16 reserved[2];              /* Reserved for future compatibility */
};

```

Programmers can use FBIOPUT_VSCREENINFO ioctl with a pointer to a fb_var_screeninfo structure to access the screen information stored in
`fb_var_screeninfo`. The structure as documented in the linux framebuffer api is shown below.

```
struct fb_var_screeninfo {
      __u32 xres;                     /* visible resolution           */
      __u32 yres;
      __u32 xres_virtual;             /* virtual resolution           */
      __u32 yres_virtual;
      __u32 xoffset;                  /* offset from virtual to visible */
      __u32 yoffset;                  /* resolution                   */

      __u32 bits_per_pixel;           /* guess what                   */
      __u32 grayscale;                /* 0 = color, 1 = grayscale,    */
                                      /* >1 = FOURCC                  */
      struct fb_bitfield red;         /* bitfield in fb mem if true color, */
      struct fb_bitfield green;       /* else only length is significant */
      struct fb_bitfield blue;
      struct fb_bitfield transp;      /* transparency                 */

      __u32 nonstd;                   /* != 0 Non standard pixel format */

      __u32 activate;                 /* see FB_ACTIVATE_*            */

      __u32 height;                   /* height of picture in mm    */
      __u32 width;                    /* width of picture in mm     */

      __u32 accel_flags;              /* (OBSOLETE) see fb_info.flags */

      /* Timing: All values in pixclocks, except pixclock (of course) */
      __u32 pixclock;                 /* pixel clock in ps (pico seconds) */
      __u32 left_margin;              /* time from sync to picture    */
      __u32 right_margin;             /* time from picture to sync    */
      __u32 upper_margin;             /* time from sync to picture    */
      __u32 lower_margin;
      __u32 hsync_len;                /* length of horizontal sync    */
      __u32 vsync_len;                /* length of vertical sync      */
      __u32 sync;                     /* see FB_SYNC_*                */
      __u32 vmode;                    /* see FB_VMODE_*               */
      __u32 rotate;                   /* angle we rotate counter clockwise */
      __u32 colorspace;               /* colorspace for FOURCC-based modes */
      __u32 reserved[4];              /* Reserved for future compatibility */
};
```

### Framebuffer Format
According to the Linux API the format of framebuffer image data is changed when programs "set the fb_var_screeninfo bits_per_pixel field to the desired frame buffer depth. Values up to 8 will usually map to monochrome, grayscale or pseudocolor visuals, although this is not required."[1]

```
struct fb_bitfield {
    __u32 offset;                   /* beginning of bitfield        */
    __u32 length;                   /* length of bitfield           */
    __u32 msb_right;                /* != 0 : Most significant bit is */
                                    /* right */
};
```

Some notes on how these fields actually affect the framebuffer according to kernel documentation:
* 	For grayscale formats, applications set the grayscale field to one. The
 	red, blue, green and transp fields must be set to 0 by applications and ignored by drivers. Drivers must fill the red, blue and green offsets to 0 and lengths to the bits_per_pixel value.

* 	For pseudocolor formats, applications set the grayscale field to zero. The 	   red, blue, green and transp fields must be set to 0 by applications and		ignored by drivers. Drivers must fill the red, blue and green offsets to 0	  and lengths to the bits_per_pixel value.

*	For truecolor and directcolor formats, applications set the grayscale 
	field to zero, and the red, blue, green and transp fields to describe the layout of color components in memory:

______________________________________________________________________________
### Sources:

	[1] https://www.kernel.org/doc/html/latest/fb/api.html