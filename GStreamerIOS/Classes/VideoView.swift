import Foundation
import SwiftUI


public struct Video: UIViewControllerRepresentable {
    
    public typealias UIViewControllerType = VideoViewController
    var bridgingVideoManager: BridgingVideoManager

    public init(bridgingVideoManager: BridgingVideoManager) {
        self.bridgingVideoManager = bridgingVideoManager
    }
    public func makeUIViewController(context: Context) -> VideoViewController {
        let video = VideoViewController()
        video.bridgingVideoManager = bridgingVideoManager
        return video
    }
    
    public func updateUIViewController(_ nsViewController: VideoViewController, context: Context) {
        
    }
    
    class VideoManager: NSObject {
        let parent: Video
        init(_ view: Video) {
            self.parent = view
        }
    }
    


}
