import Foundation
import SwiftUI


struct Video: UIViewControllerRepresentable {
    
    typealias UIViewControllerType = VideoViewController
    var bridgingVideoManager: BridgingVideoManager

    
    func makeUIViewController(context: Context) -> VideoViewController {
        let video = VideoViewController()
        video.bridgingVideoManager = bridgingVideoManager
        return video
    }
    
    func updateUIViewController(_ nsViewController: VideoViewController, context: Context) {
        
    }
    
    class VideoManager: NSObject {
        let parent: Video
        init(_ view: Video) {
            self.parent = view
        }
    }
    


}