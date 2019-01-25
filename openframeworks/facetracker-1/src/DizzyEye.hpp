//
//  DizzyEye.hpp
//  facetracker-1
//
//  Created by Lo Wing Ellen on 1/22/19.
//

#ifndef DizzyEye_hpp
#define DizzyEye_hpp

#include "ofMain.h"
#include <stdio.h>

class DizzyEye {
    public:
        void setup(ofImage * frames_, int framesSize_);
        void update(int x_, int y_, int w_, int h_);
        ofPixels & draw();
    
        int gifindex;
        int gifsize;
        vector<ofImage> frames;
        ofImage frame;
    
        ofFbo fbo;
        int x;
        int y;
        int w;
        int h;
        int rectSize;
        float scale;
};

#endif /* DizzyEye_hpp */
