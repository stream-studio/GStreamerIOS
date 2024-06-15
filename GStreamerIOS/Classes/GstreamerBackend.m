#import <Foundation/Foundation.h>
#import "GstreamerBackend.h"
#include <gst/gst.h>
#include "libstudio.h"


@interface GstreamerBackend()
-(void)check_initialization_complete;
@end

@implementation GstreamerBackend {
    gboolean initialized;        /* To avoid informing the UI multiple times about the initialization */
    UIView *ui_video_view;       /* UIView that holds the video */
    StudioCtx ctx;
}

/*
 * Interface methods
 */

-(id) init:(UIView *)video_view
{
    if (self = [super init])
    {
        self->ui_video_view = video_view;
        self->ctx = libstudio_init();
        
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            libstudio_start(self->ctx, (guintptr) ui_video_view);
        });
    }
    return self;
}



-(void)createCameraSource{
    libstudio_create_camera_source(self->ctx);
}

-(void)createImageSource:(NSString*) path{
    libstudio_create_image_source(self->ctx, [path UTF8String]);
}

-(void)switchScene:(int) sceneId{
    libstudio_switch_scene(self->ctx, sceneId);
}



-(void)startStreaming:(NSString*) url{
    libstudio_start_streaming(self->ctx, [url UTF8String]);
}

-(void)stopStreaming{
    libstudio_stop_streaming(self->ctx);
}

-(void)playVideo:(NSString*) url{
    libstudio_play_video(self->ctx, [url UTF8String]);
}

-(void) deinit
{
    if (self->ctx){
        libstudio_stop(self->ctx);
    }
}

@end
