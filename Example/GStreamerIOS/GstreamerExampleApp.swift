//
//  GstreamerExampleApp.swift
//  GStreamerIOS_Example
//
//  Created by Ludovic Bouguerra on 25/11/2023.
//  Copyright © 2023 CocoaPods. All rights reserved.
//

import SwiftUI
import GStreamerIOS

@main
struct SwitcherAppIOSApp: App {
    
    init() {
        gst_ios_init()
    }
    
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}
