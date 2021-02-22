//
//  basic-tutorial-4.h
//  gstreamer-tutorial
//
//  Created by Ngo Hong Thai on 21/02/2021.
//

#ifndef basic_tutorial_4_h
#define basic_tutorial_4_h

#include <stdio.h>
#include <gst/gst.h>

/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData4 {
    GstElement *playbin;  /* Our one and only element */
    gboolean playing;      /* Are we in the PLAYING state? */
    gboolean terminate;    /* Should we terminate execution? */
    gboolean seek_enabled; /* Is seeking enabled for this media? */
    gboolean seek_done;    /* Have we performed the seek already? */
    gint64 duration;       /* How long does this media last, in nanoseconds */
} CustomData4;

int basic4Run(int argc, char *argv[]);

#endif /* basic_tutorial_4_h */
