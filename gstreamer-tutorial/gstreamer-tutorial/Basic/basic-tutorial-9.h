//
//  basic-tutorial-9.h
//  gstreamer-tutorial
//
//  Created by Ngo Hong Thai on 21/02/2021.
//

#ifndef basic_tutorial_9_h
#define basic_tutorial_9_h

#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>

/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData9 {
  GstDiscoverer *discoverer;
  GMainLoop *loop;
} CustomData9;

int basic9Run(int argc, char *argv[]);

#endif /* basic_tutorial_9_h */
