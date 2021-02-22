//
//  basic-tutorial-1.c
//  gstreamer-tutorial
//
//  Created by Ngo Hong Thai on 20/02/2021.
//


/**
 - Cách khởi tạo GStreamer bằng gst_init ().
 - Cách nhanh chóng xây dựng đường dẫn từ mô tả dạng văn bản bằng gst_parse_launch ().
 - Cách tạo đường dẫn phát lại tự động bằng playbin .
 - Cách báo hiệu GStreamer bắt đầu phát lại bằng gst_element_set_state ().
 - Cách ngồi lại và thư giãn, trong khi GStreamer lo mọi việc bằng cách sử dụng gst_element_get_bus () và gst_bus_timed_pop_filtered ().
 */

#include <gst/gst.h>

int basicTutorial1(int argc, char * argv[]) {
    
    GMainLoop *main_loop;
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;
    
    /* Initalize GStreamer */
    /**
     Khởi tạo tất cả các cấu trúc bên trong
     Kiểm tra những plugin nào khả dụng
     Thực thi bất kỳ tùy chọn dòng lệnh nào dành cho GStreamer
     */
    gst_init(&argc, &argv);
    
    /* Build the pipeline */
    pipeline = gst_parse_launch("playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
    
    main_loop = g_main_loop_new(NULL, FALSE);
    
    /* Start playing */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_main_loop_run(main_loop);
    /* Wait until error or EOS (End-Of-Stream) */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    
    /* Free resources */
    if (msg != NULL)
        gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}
