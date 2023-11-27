#include "libstudio.h"
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

typedef struct{
    GstElement *pipeline;
    
    GstElement *video_mixer;
    GstElement *audio_mixer;
    
    GstElement *overlay;
    
    gintptr video_handle;
    
    // tee raw
    GstElement *tee_video_raw;
    GstElement *tee_audio_raw;
    
    GMainContext *context;       /* GLib context used to run the main loop */
    GMainLoop *main_loop;        /* GLib main loop */
    
    // output sinks
    GstElement *streamer_bin;
    GstElement *recorder_bin;
    
    GstElement *video_sink;
    GstElement *audio_sink;
} StudioPrivateContext;


enum OverlayType {
    VIDEO_PLAYER,
 };

typedef struct{
    StudioPrivateContext *context;
    GstElement* source;
} StudioOverlayPrivateContext;



static gboolean
message_cb (GstBus * bus, GstMessage * message, gpointer user_data)
{
    StudioPrivateContext* context = (StudioPrivateContext*)user_data;
    
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_ERROR:{

          break;
        }
        case GST_MESSAGE_WARNING:{

            break;
        }
        case GST_MESSAGE_EOS: {
            g_print ("Got EOS\n");
            break;
        }
        case GST_MESSAGE_ELEMENT: {
            const GstStructure *s = gst_message_get_structure (message);

            if (gst_structure_has_name (s, "GstBinForwarded"))
            {
                GstMessage *forward_msg = NULL;
                
                gst_structure_get (s, "message", GST_TYPE_MESSAGE, &forward_msg, NULL);
                
                
                
                if (GST_MESSAGE_TYPE (forward_msg) == GST_MESSAGE_EOS)
                {
                    g_print ("EOS from element %s\n",
                            GST_OBJECT_NAME (GST_MESSAGE_SRC (forward_msg)));
                    
                    gst_element_set_state(context->streamer_bin, GST_STATE_NULL);
                    gst_bin_remove(GST_BIN(context->pipeline), context->streamer_bin);
                    context->streamer_bin = NULL;
                }
                gst_message_unref (forward_msg);
                
            }
            break;
        }
        default:
            break;
    }

  return TRUE;
}

StudioCtx libstudio_init(){
    
    StudioPrivateContext* context = malloc(sizeof(StudioPrivateContext));
    context->pipeline = gst_pipeline_new(NULL);
    
    GstElement* video_src = gst_element_factory_make("videotestsrc", NULL);
    g_object_set(video_src, "is-live", TRUE, NULL);
    
    //GstElement* convert = gst_element_factory_make("glcolorconvert", NULL);
    //context->overlay = gst_element_factory_make("gdkpixbufoverlay", NULL);
    
    GstElement* audio_src = gst_element_factory_make("audiotestsrc", NULL);
    g_object_set(audio_src, "is-live", TRUE, NULL);
    context->tee_video_raw = gst_element_factory_make("tee", NULL);
    context->tee_audio_raw = gst_element_factory_make("tee", NULL);
    
    
    context->video_mixer = gst_element_factory_make("compositor", NULL);
    context->audio_mixer = gst_element_factory_make("audiomixer", NULL);
    
    GstElement* vqueue = gst_element_factory_make("queue", NULL);
    GstElement* aqueue = gst_element_factory_make("queue", NULL);
    
    gst_bin_add(GST_BIN(context->pipeline), vqueue);
    gst_bin_add(GST_BIN(context->pipeline), aqueue);
    
    
    context->video_sink = gst_element_factory_make("glimagesink", NULL);
    g_object_set(context->video_sink, "sync", FALSE, NULL);
    
    context->audio_sink = gst_element_factory_make("autoaudiosink", NULL);
    g_object_set(context->audio_sink, "sync", FALSE, NULL);
     
    
    GstCaps* caps = gst_caps_new_simple("video/x-raw",
                                            "format", G_TYPE_STRING, "NV12",
                                            "width", G_TYPE_INT, 1920,
                                            "height", G_TYPE_INT, 1080,
                                            "framerate", GST_TYPE_FRACTION, 25, 1,
                                            //"texture-target", G_TYPE_STRING, "rectangle",
                                            NULL);
    
    gst_bin_add_many(GST_BIN_CAST(context->pipeline), video_src, audio_src, context->video_mixer, context->audio_mixer, vqueue, aqueue, context->tee_video_raw, context->tee_audio_raw, context->video_sink, context->audio_sink, NULL);
    

    gst_element_link_filtered(video_src, context->video_mixer, caps);
    gst_caps_unref(caps);

    
    caps = gst_caps_new_simple("video/x-raw",
                                            "format", G_TYPE_STRING, "NV12",
                                            "width", G_TYPE_INT, 1920,
                                            "height", G_TYPE_INT, 1080,
                                            "framerate", GST_TYPE_FRACTION, 25, 1,
                                            //"texture-target", G_TYPE_STRING, "rectangle",
                                            NULL);
    
    //gst_element_link_filtered(context->overlay, context->video_mixer, caps);
    //gst_element_link(context->overlay, context->video_mixer);
    
    caps = gst_caps_new_simple("video/x-raw",
                                            "format", G_TYPE_STRING, "NV12",
                                            "width", G_TYPE_INT, 1920,
                                            "height", G_TYPE_INT, 1080,
                                            "framerate", GST_TYPE_FRACTION, 25, 1,
                                            NULL);

    gst_element_link_filtered(context->video_mixer, context->tee_video_raw, caps);
    
    gst_caps_unref(caps);
    

    gst_element_link(context->tee_video_raw, vqueue);
    gst_element_link(vqueue, context->video_sink);
    
    gst_element_link(audio_src, context->audio_mixer);
    gst_element_link(context->audio_mixer, context->tee_audio_raw);
    gst_element_link(context->tee_audio_raw, aqueue);
    gst_element_link(aqueue, context->audio_sink);
    
    gst_element_set_state(context->pipeline, GST_STATE_READY);
    
    
    return context;
}

void libstudio_start(StudioCtx ctx, void* view){
        
    GstBus *bus;
    
    StudioPrivateContext* context = (StudioPrivateContext*)ctx;
        
    context->context = g_main_context_new ();
    g_main_context_push_thread_default(context->context);
    
    
    /* Create a GLib Main Loop and set it to run */
    GST_DEBUG ("Entering main loop...");
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(context->video_sink), (guintptr) view);

    gst_element_set_state(context->pipeline, GST_STATE_PLAYING);
    
    context->main_loop = g_main_loop_new (context->context, FALSE);
    
    bus = gst_element_get_bus (context->pipeline);
    gst_bus_add_signal_watch(bus);
    g_signal_connect(G_OBJECT(bus), "message", G_CALLBACK(message_cb), context);
    
    g_main_loop_run (context->main_loop);
    
    GST_DEBUG ("Exited main loop");
    context->video_handle = (gintptr) NULL;
    g_main_loop_unref (context->main_loop);
    context->main_loop = NULL;

    /* Free resources */
    g_main_context_pop_thread_default(context->context);
    g_main_context_unref (context->context);
    gst_element_set_state (context->pipeline, GST_STATE_NULL);
    gst_object_unref (context->pipeline);
    context->pipeline = NULL;
    
    gst_object_unref(context->pipeline);
    free(context);

}


void libstudio_start_streaming(StudioCtx ctx, const char* destination){
    printf("Lib Studio - start streaming");
    StudioPrivateContext* context = (StudioPrivateContext*)ctx;

    if (context->streamer_bin == NULL){
        
        context->streamer_bin = gst_bin_new("streaming_bin");
        g_object_set(G_OBJECT(context->streamer_bin), "message-forward", TRUE, NULL);
        
        GstElement *vqueue = gst_element_factory_make("queue", NULL);
        g_object_set(vqueue, "leaky", 2, NULL);
        
        GstElement *vencoder = gst_element_factory_make("vtenc_h264", NULL);
        g_object_set(vencoder, "realtime", TRUE, "max-keyframe-interval", 50, "bitrate", 4500, "quality", 1, NULL);
        
        
        GstElement *parse = gst_element_factory_make("h264parse", NULL);
        
        GstElement *aqueue = gst_element_factory_make("queue", NULL);
        g_object_set(aqueue, "leaky", 2, NULL);
        
        GstElement *audioconvert = gst_element_factory_make("audioconvert", NULL);
        GstElement *aencoder = gst_element_factory_make("avenc_aac", NULL);
        
        GstElement *mux = gst_element_factory_make("flvmux", NULL);
        
        GstElement *queue = gst_element_factory_make("queue", NULL);
        GstElement *cqueue = gst_element_factory_make("queue", NULL);
        
        GstElement *sink = gst_element_factory_make("rtmp2sink", NULL);
        g_object_set(sink, "location", "", NULL);
        
        gst_bin_add_many(GST_BIN(context->streamer_bin), vqueue, cqueue, vencoder, parse, aqueue, audioconvert, aencoder, mux, queue, sink, NULL);
        gst_element_link_many(vqueue, cqueue, NULL);
        
        
        gst_element_link_many(cqueue, vencoder, parse, NULL);
        
        gst_element_link_many(aqueue, audioconvert, aencoder, NULL);
        
        gst_element_link_pads(parse, "src", mux, "video");
        gst_element_link_pads(aencoder, "src", mux, "audio");
        
        
        gst_element_link_many(mux, queue, sink, NULL);
        
        gst_bin_add(GST_BIN(context->pipeline), context->streamer_bin);
        
        
        GstPad *video_sink_pad = gst_element_get_static_pad(vqueue, "sink");
        GstPad *audio_sink_pad = gst_element_get_static_pad(aqueue, "sink");
        
        gst_element_add_pad(context->streamer_bin, gst_ghost_pad_new("video_sink", video_sink_pad));
        gst_element_add_pad(context->streamer_bin, gst_ghost_pad_new("audio_sink", audio_sink_pad));
        
        gst_object_unref(video_sink_pad);
        gst_object_unref(audio_sink_pad);
        
        gst_element_sync_state_with_parent(context->streamer_bin);
        
        
        gst_element_link_pads(context->tee_video_raw, NULL, context->streamer_bin, "video_sink");
        gst_element_link_pads(context->tee_audio_raw, NULL, context->streamer_bin, "audio_sink");
        
    }
}



static GstPadProbeReturn libstudio_stop_callback (GstPad * pad, GstPadProbeInfo * info, gpointer user_data){
    
    gst_pad_remove_probe (pad, GST_PAD_PROBE_INFO_ID (info));
    
    printf("Lib Studio - stop callback");
        
    GstPad *sink_pad = gst_pad_get_peer(pad);
    gst_pad_send_event(sink_pad, gst_event_new_eos());
    
    gst_pad_unlink(pad, sink_pad);
    g_object_unref(sink_pad);
 
    return GST_PAD_PROBE_DROP;
}


void libstudio_stop_streaming(StudioCtx ctx){
    printf("Lib Studio - stop streaming");
    StudioPrivateContext* context = (StudioPrivateContext*)ctx;

    if (context->streamer_bin != NULL){
        
        GstPad *video_sink_pad = gst_element_get_static_pad(context->streamer_bin, "video_sink");
        GstPad *audio_sink_pad = gst_element_get_static_pad(context->streamer_bin, "audio_sink");
        
        GstPad *video_src_pad = gst_pad_get_peer(video_sink_pad);
        GstPad *audio_src_pad = gst_pad_get_peer(audio_sink_pad);
        
        gst_pad_add_probe(video_src_pad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, libstudio_stop_callback, ctx, NULL);
        gst_pad_add_probe(audio_src_pad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM, libstudio_stop_callback, ctx, NULL);
        
        g_object_unref(video_src_pad);
        g_object_unref(audio_src_pad);
        g_object_unref(video_sink_pad);
        g_object_unref(audio_sink_pad);
        
    }
    
}

static void pad_added_handler (GstElement *src, GstPad *new_pad, StudioOverlayPrivateContext *context){

    GstPad *sink_pad = NULL;
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    
    const gchar *new_pad_type = NULL;

    /* Check the new pad's type */
    new_pad_caps = gst_pad_get_current_caps (new_pad);
    new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
    new_pad_type = gst_structure_get_name (new_pad_struct);

    g_print ("Received new pad '%s' from '%s with type %s ':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src), new_pad_type);

    
    if (g_str_has_prefix (new_pad_type, "audio/x-raw")) {
        gst_element_add_pad(context->source, gst_ghost_pad_new("audio_src", new_pad));
        gst_element_link_pads(context->source, "audio_src", context->context->audio_mixer, NULL);
    } else if (g_str_has_prefix (new_pad_type, "video/x-raw")) {
        gst_element_add_pad(context->source, gst_ghost_pad_new("video_src", new_pad));
        gst_element_link_pads(context->source, "video_src", context->context->video_mixer, NULL);
    }

    if (new_pad_caps != NULL)
        gst_caps_unref (new_pad_caps);

}

StudioOverlayCtx libstudio_play_video(StudioCtx ctx, const char* video){
    StudioOverlayPrivateContext* context = malloc(sizeof(StudioPrivateContext));
    context->context = ctx;
    
    context->source = gst_bin_new(NULL);
    
    GstElement *source = gst_element_factory_make("filesrc", "player_src");
    g_object_set(source, "location", video, NULL);
    
    GstElement *decoder = gst_element_factory_make("decodebin", "player_decoder");
    g_signal_connect (decoder, "pad-added", G_CALLBACK (pad_added_handler), context);

    gst_bin_add_many(GST_BIN(context->source), source, decoder, NULL);
    gst_element_link(source, decoder);
    
    gst_bin_add(GST_BIN(context->context->pipeline), context->source);
    
    gst_element_sync_state_with_parent(context->source);
    
    return context;
    
}

void libstudio_stop(StudioCtx ctx){
    StudioPrivateContext* context = (StudioPrivateContext*)ctx;
    g_main_loop_quit(context->main_loop);
}
