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
    BitmapView *bitmapView = [[[BitmapView alloc] initWithFrame:self.view.frame] autorelease];
    bitmapView.wantsLayer = YES;
    bitmapView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [self.view addSubview:bitmapView];
}

@end
