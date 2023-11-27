#ifndef libstudio_h
#define libstudio_h

#include <stdio.h>

typedef void* StudioCtx;
typedef void* StudioOverlayCtx;



/**
    Create Lib Studio Context
    This method preprare Video Pipeline
    The next step is to call libstudio_start();

    @return StudioCtx LibStudio Opaque Context or NULL if there is an error
 */
StudioCtx libstudio_init(void);

/**
    Start the pipeline, you must set overlay
 
    @param ctx LibStudio opaque context
    @param view : a view handle to show preview
    @param overlay : Path to an overlay file ( must be png )
 */
void libstudio_start(StudioCtx ctx, void* vie);

/**
    Start the pipeline the streaming part of the pipeline
    @param ctx LibStudio opaque context
    @param destination rtmp destination, could be rtmps too
 */
void libstudio_start_streaming(StudioCtx ctx, const char* destination);

/**
    Stop the streaming part of the pipeline
 */
void libstudio_stop_streaming(StudioCtx ctx);

/**
    Play Video with given path
    @param ctx : LibStudio Opaque Context given by libstudio_init
    @param video : Video Path
 */
StudioOverlayCtx libstudio_play_video(StudioCtx ctx, const char* video);

/**
    Release libstudio_resources
    @param ctx : LibStudio Context
 */
void libstudio_stop(StudioCtx ctx);

#endif /* libstudio_h */