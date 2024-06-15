import Foundation
import UIKit
import QuartzCore


public class BridgingVideoManager: ObservableObject {
    public var vc: VideoViewController!
    public init() {}
}

public class VideoViewController: UIViewController {
    
    private var backend : GstreamerBackend?
    private var videoView: UIView = UIView()
    var bridgingVideoManager: BridgingVideoManager!
    
    deinit {
        let notificationCenter = NotificationCenter.default
        notificationCenter.removeObserver(self)
    }
    
    public override func viewDidLoad() {
        super.viewDidLoad()
        bridgingVideoManager.vc = self

        let notificationCenter = NotificationCenter.default
        notificationCenter.addObserver(self,
                                       selector: #selector(appMovedToBackground),
                                       name: NSNotification.Name.UIApplicationWillResignActive,
                                       object: nil)


    }
    

    
    public override func loadView() {
        view = UIView()
        view.addSubview(videoView)
        videoView.translatesAutoresizingMaskIntoConstraints = false
        videoView.topAnchor.constraint(equalTo: view.topAnchor, constant: 0).isActive = true
        videoView.bottomAnchor.constraint(equalTo: view.bottomAnchor, constant: 0).isActive = true
        videoView.leftAnchor.constraint(equalTo: view.leftAnchor, constant: 0).isActive = true
        videoView.rightAnchor.constraint(equalTo: view.rightAnchor, constant: 0).isActive = true
        backend = GstreamerBackend(videoView)

        /*videoView.frame = CGRect(x: 0, y: 0, width: 1280, height: 720)


         view.addSubview(videoView)*/

          
    }

    public func switchScene(sceneId : Int32) {
        backend?.switchScene(sceneId)
    }
    
    public func createCameraSource() {
        backend?.createCameraSource()
    }
    
    public func createImageSource(path: String) {
        backend?.createImageSource(path);
    }
    
    public func startStreaming(url : String) {
        backend?.startStreaming(url)
    }
    
    public func stopStreaming() {
        backend?.stopStreaming()
    }
    
    public func playVideo(path : String) {
        backend?.playVideo(path)
    }
    
    @objc func appMovedToBackground() {
        print("App moved to background!")
    }
    

    
    
}
