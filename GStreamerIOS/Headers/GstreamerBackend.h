#ifndef StudioBackend_h
#define StudioBackend_h


@interface GstreamerBackend : NSObject

/*
    @param video_view view handle to preview
    @param overlay overlay file's path
 */
-(id) init:(UIView*) video_view;


/**
    Start the pipeline the streaming part of the pipeline
    @param url rtmp destination, could be rtmps too
 */
-(void)startStreaming:(NSString*) url;


/**
    Stop the pipeline streaming part
 */
-(void)stopStreaming;


/**
    Play video
    @param url: Video Path
 */
-(void)playVideo:(NSString*) url;

/* Quit the main loop and free all resources, including the pipeline and
 * the references to the ui delegate and the UIView used for rendering, so
 * these objects can be deallocated. */
-(void) deinit;

@end

#endif /* StudioBackend_h */
