import SwiftUI
import GStreamerIOS

@available(iOS 14.0, *)
struct ContentView: View {
    @StateObject private var videoManager: BridgingVideoManager

    init() {
         let manager = BridgingVideoManager()
         self._videoManager = StateObject(wrappedValue: manager)
     }
    
    var body: some View {
        ZStack{
            Video(bridgingVideoManager: videoManager)
                .padding()
                .aspectRatio(contentMode: .fill)
            HStack{
                Button("Start Streaming", action: {
                    videoManager.vc.startStreaming(url: "")
                }).padding()
                Button("Stop Streaming", action: {
                    videoManager.vc.stopStreaming()
                }).padding()
                Button("Play video", action: {
                    print("Play Video")
                    let path = Bundle.main.url(forResource: "test", withExtension: "mp4")?.path
                    if (path != nil){
                        print("Play video ", path!)
                        videoManager.vc.playVideo(path: path!)
                    }
                }).padding()
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
