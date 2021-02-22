//
//  basic-tutorial-3.h
//  gstreamer-tutorial
//
//  Created by Ngo Hong Thai on 20/02/2021.
//

#ifndef basic_tutorial_3_h
#define basic_tutorial_3_h

#include <stdio.h>
#include <gst/gst.h>

/* Structure to contain all our information, so we can pass it to callbacks */
typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *resample;
    GstElement *sink;
} CustomData;

int basic3Run(int argc, char *argv[]);

#endif /* basic_tutorial_3_h */
