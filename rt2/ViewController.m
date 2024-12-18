//
//  ViewController.m
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#import "ViewController.h"
#import "BitmapView.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    BitmapView *bitmapView = [[BitmapView alloc] initWithFrame:self.view.frame width:300 height:300];
    bitmapView.wantsLayer = YES; // Optional: For layer-backed drawing
    bitmapView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        
    [self.view addSubview:bitmapView];
    // Do any additional setup after loading the view.
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}


@end
