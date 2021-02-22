//
//  basic-tutorial-8.h
//  gstreamer-tutorial
//
//  Created by Ngo Hong Thai on 21/02/2021.
//

#ifndef basic_tutorial_8_h
#define basic_tutorial_8_h

#include <stdio.h>
#include <gst/gst.h>

#define CHUNK_SIZE 1024   /* Amount of bytes we are sending in each buffer */
#define SAMPLE_RATE 44100 /* Samples per second we are sending */

/* Structure to contain all our information, so we can pass it to callbacks */
typedef struct _CustomData8 {
  GstElement *pipeline, *app_source, *tee, *audio_queue, *audio_convert1, *audio_resample, *audio_sink;
  GstElement *video_queue, *audio_convert2, *visual, *video_convert, *video_sink;
  GstElement *app_queue, *app_sink;

  guint64 num_samples;   /* Number of samples generated so far (for timestamp generation) */
  gfloat a, b, c, d;     /* For waveform generation */

  guint sourceid;        /* To control the GSource */

  GMainLoop *main_loop;  /* GLib's Main Loop */
} CustomData8;

int basic8Run(int argc, char *argv[]);

#endif /* basic_tutorial_8_h */
