//
//  ViewController.swift
//  GStreamerIOS
//
//  Created by 828542 on 11/24/2023.
//  Copyright (c) 2023 828542. All rights reserved.
//

import UIKit
import GStreamerIOS


class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        gst_ios_init();

        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

}

