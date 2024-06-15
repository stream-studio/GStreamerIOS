import SwiftUI
import GStreamerIOS

@available(iOS 14.0, *)
struct ContentView: View {
    @StateObject private var videoManager: BridgingVideoManager
    @State private var showSheet = false

    @State var fileName = ""
    @State var openFile = false

    
    
    init() {
         let manager = BridgingVideoManager()
         self._videoManager = StateObject(wrappedValue: manager)
     }
    
    var body: some View {
        VStack{
            Spacer()
            Video(bridgingVideoManager: videoManager)
                .frame(
                    maxWidth: .infinity,
                    maxHeight: 720,
                    alignment: .topLeading
                )
            Spacer()
            HStack{
                Button("Plan 1", action: {
                    videoManager.vc.switchScene(sceneId: 0)
                }).foregroundColor(.white).padding()
                Button("Plan 2", action: {
                    videoManager.vc.switchScene(sceneId: 1)
                }).foregroundColor(.white).padding()
                Button("Plan 3", action: {
                    videoManager.vc.switchScene(sceneId: 2)
                }).foregroundColor(.white).padding()
                VStack{
                    Button("Démarrer la diffusion", action: {
                        showSheet = true;
                    }).foregroundColor(.white).padding()
                    Button("Options", action: {
                        videoManager.vc.createCameraSource()
                    }).foregroundColor(.white).padding()
                    Button("Ajouter une image", action: {
                        openFile = true;
                    }).foregroundColor(.white).padding()
                }
            }.background(Color(.black))
        }.background(Color(.black)).frame(
            minWidth: 0,
            maxWidth: .infinity,
            minHeight: 0,
            maxHeight: .infinity,
            alignment: .topLeading
        ).fileImporter( isPresented: $openFile, allowedContentTypes: [.image], allowsMultipleSelection: false, onCompletion: {
            (Result) in
            
            do{
                let fileURL = try Result.get()
                print(fileURL.first!.absoluteString)
                
                //self.fileName = fileURL.first?.lastPathComponent ?? "file not available"
                videoManager.vc.createImageSource(path: fileURL.first!.absoluteString)
            }
            catch{
               print("error reading file \(error.localizedDescription)")
            }
            
        }).sheet(isPresented: $showSheet) {
            SheetView()
        }
    }
}

struct SheetView: View {
   @Environment(\.dismiss) var dismiss

    var body: some View {
        ZStack {
           Button {
              dismiss()
           } label: {
               Image(systemName: "xmark.circle")
                 .font(.largeTitle)
                 .foregroundColor(.gray)
           }
         }
         .frame(maxWidth: .infinity, maxHeight: .infinity, alignment: .topTrailing)
         .padding()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

