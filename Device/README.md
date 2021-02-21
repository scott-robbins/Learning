# Devices 

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






###Sources:
______________________________________________________________________________
[1] https://www.kernel.org/doc/html/latest/fb/api.html