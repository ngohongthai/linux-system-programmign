//
//  basic-tutorial-3.c
//  gstreamer-tutorial
//
//  Created by Ngo Hong Thai on 20/02/2021.
//

#include "basic-tutorial-3.h"
#include <gst/gst.h>

static void pad_added_handler (GstElement *src, GstPad *new_pad, CustomData *data);

int basic3Run(int argc, char *argv[]) {
    GMainLoop *main_loop;
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;
    
    /* Initialize GStreamer */
    gst_init (&argc, &argv);
    
    /* Create the elements */
    //uridecodebinsẽ khởi tạo nội bộ tất cả các yếu tố cần thiết (nguồn, bộ tách dòng và bộ giải mã) để biến một URI thành các luồng âm thanh và / hoặc video thô.
    data.source = gst_element_factory_make ("uridecodebin", "source");
    //audioconvert hữu ích cho việc chuyển đổi giữa các định dạng âm thanh khác nhau, đảm bảo rằng ví dụ này sẽ hoạt động trên mọi nền tảng, vì định dạng do bộ giải mã âm thanh tạo ra có thể không giống với định dạng âm thanh mong đợi.
    data.convert = gst_element_factory_make ("audioconvert", "convert");
    //audioresample rất hữu ích để chuyển đổi giữa các audio sample rates khác nhau, tương tự, đảm bảo rằng ví dụ này sẽ hoạt động trên bất kỳ nền tảng nào, vì audio sample rates do bộ giải mã âm thanh tạo ra có thể không phải là audio sample rates hỗ trợ.
    data.resample = gst_element_factory_make ("audioresample", "resample");
    data.sink = gst_element_factory_make ("autoaudiosink", "sink");
    
    /* Create the empty pipeline */
    data.pipeline = gst_pipeline_new ("test-pipeline");
    main_loop = g_main_loop_new(NULL, FALSE);
    
    if (!data.pipeline || !data.source || !data.convert || !data.resample || !data.sink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }
    
    /* Build the pipeline. Note that we are NOT linking the source at this
     * point. We will do it later. */
    //Here we link the elements converter, resample and sink, but we DO NOT link them with the source, since at this point it contains no source pads.
    gst_bin_add_many (GST_BIN (data.pipeline), data.source, data.convert, data.resample, data.sink, NULL);
    if (!gst_element_link_many (data.convert, data.resample, data.sink, NULL)) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (data.pipeline);
        return -1;
    }
    
    /* Set the URI to play */
    g_object_set (data.source, "uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
    
    /* Connect to the pad-added signal */
    g_signal_connect (data.source, "pad-added", G_CALLBACK (pad_added_handler), &data);
    
    /* Start playing */
    ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    g_main_loop_run(main_loop);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (data.pipeline);
        return -1;
    }
    
    /* Listen to the bus */
    bus = gst_element_get_bus (data.pipeline);
    do {
        msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
                                          GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
        
        /* Parse message */
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;
            
            switch (GST_MESSAGE_TYPE (msg)) {
                case GST_MESSAGE_ERROR:
                    gst_message_parse_error (msg, &err, &debug_info);
                    g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                    g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
                    g_clear_error (&err);
                    g_free (debug_info);
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_EOS:
                    g_print ("End-Of-Stream reached.\n");
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_STATE_CHANGED:
                    /* We are only interested in state-changed messages from the pipeline */
                    if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data.pipeline)) {
                        GstState old_state, new_state, pending_state;
                        gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
                        g_print ("Pipeline state changed from %s to %s:\n",
                                 gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
                    }
                    break;
                default:
                    /* We should not reach here */
                    g_printerr ("Unexpected message received.\n");
                    break;
            }
            gst_message_unref (msg);
        }
    } while (!terminate);
    
    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);
    return 0;
}

static void pad_added_handler (GstElement *src, GstPad *new_pad, CustomData *data) {
    GstPad *sink_pad = gst_element_get_static_pad (data->convert, "sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;
    
    g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));
    
    /* If our converter is already linked, we have nothing to do here */
    if (gst_pad_is_linked (sink_pad)) {
        g_print ("We are already linked. Ignoring.\n");
        goto exit;
    }
    
    /* Check the new pad's type */
    new_pad_caps = gst_pad_get_current_caps (new_pad);
    new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
    new_pad_type = gst_structure_get_name (new_pad_struct);
    if (!g_str_has_prefix (new_pad_type, "audio/x-raw")) {
        g_print ("It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
        goto exit;
    }
    
    /* Attempt the link */
    ret = gst_pad_link (new_pad, sink_pad);
    if (GST_PAD_LINK_FAILED (ret)) {
        g_print ("Type is '%s' but link failed.\n", new_pad_type);
    } else {
        g_print ("Link succeeded (type '%s').\n", new_pad_type);
    }
    
exit:
    /* Unreference the new pad's caps, if we got them */
    if (new_pad_caps != NULL)
        gst_caps_unref (new_pad_caps);
    
    /* Unreference the sink pad */
    gst_object_unref (sink_pad);
}
