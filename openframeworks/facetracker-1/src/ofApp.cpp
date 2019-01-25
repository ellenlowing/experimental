#include "ofApp.h"

using namespace ofxCv;


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    camWidth = 1280;
    camHeight = 720;
    cam.initGrabber(camWidth, camHeight);
    vidtexture.allocate(camWidth, camHeight, GL_RGB);
    vidimage.allocate(camWidth, camHeight);
    grayvidimage.allocate(camWidth, camHeight);
    
    tracker.setup();
    effectNum = 0;
    
    // effect 0
    box2d.init();
    box2d.enableEvents();   // <-- turn on the event listener
    box2d.setGravity(0, 10);
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    for(int i = 0; i < 9; i++) {
        ofColor c;
        c.r = ofRandom(0, 255);
        c.g = ofRandom(0, 255);
        c.b = ofRandom(0, 255);
        featureColors[i] = c;
    }
    
    // effect 1
    blurdata.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    for(int i = 0; i < blurdata.size(); i++) {
        blurdata[i] = 0;
    }
    
    // effect 2
    for(int i = 0; i < 10; i++) {
        dizzyFrames[i] = ofImage("dizzy/" + to_string(i+1) + ".tiff");
    }
    leftEye.setup(dizzyFrames, static_cast<int>(sizeof(dizzyFrames) / sizeof(dizzyFrames[0])));
    rightEye.setup(dizzyFrames, static_cast<int>(sizeof(dizzyFrames) / sizeof(dizzyFrames[0])));
    faceRotateAng = 0;
    
    // effect 3
    for(int i = 0; i < 4; i++) {
        faceFeatureIndices[i] = i;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()) {
        ofPixels pixels = cam.getPixels();
        pixels.mirror(false, true);
        viddata = pixels;
        vidtexture.loadData(viddata);
        tracker.update(toCv(viddata));
    }
    
    switch(effectNum) {
        case 0:
        {
            faceCentroid = tracker.getImageFeature(ofxFaceTracker::NOSE_BRIDGE).getCentroid2D();
            // add particles
            if(ofRandom(1) < 0.1f) {
                float r = ofRandom(4, 10);
                circles.push_back(shared_ptr<Circle>(new Circle));
                circles.back().get()->setPhysics(3.0, 0.53, 0.1);
                circles.back().get()->setup(box2d.getWorld(), ofRandom(faceCentroid.x - 100, faceCentroid.x + 100), 0, r);
                circles.back().get()->color = ofColor(30,144,255);
            }
            if(ofRandom(1) > 0.9f) {
                float w = ofRandom(4, 20);
                float h = ofRandom(4, 20);
                boxes.push_back(shared_ptr<Rectangle>(new Rectangle));
                boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
                boxes.back().get()->setup(box2d.getWorld(), ofRandom(faceCentroid.x - 100, faceCentroid.x + 100), 0, w, h);
                boxes.back().get()->color = ofColor(102,205,170);
            }
            box2d.update();
        }
            break;
        case 1:
        {
            // update blurdata opacity buffer
            for(int i = 0; i < blurdata.size(); i++) {
                if(blurdata[i] > 0) {
                    blurdata[i] -= 5;
                }
            }
        }
            break;
        case 2:
        {
            // update facial features position
            ofPolyline leftEyePoly = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
            ofPoint leftCentroid = leftEyePoly.getCentroid2D();
            ofPoint leftEyeCorner = leftEyePoly.getBoundingBox().getTopLeft();
            ofPolyline rightEyePoly = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
            ofPoint rightCentroid = rightEyePoly.getCentroid2D();
            ofPoint rightEyeCorner = rightEyePoly.getBoundingBox().getTopLeft();
            
            leftEye.update(int(leftCentroid.x), int(leftCentroid.y), leftEyePoly.getBoundingBox().getWidth(), leftEyePoly.getBoundingBox().getHeight());
            rightEye.update(int(rightCentroid.x), int(rightCentroid.y), rightEyePoly.getBoundingBox().getWidth(), rightEyePoly.getBoundingBox().getHeight());
            
            if(tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT) > 1.8f) {
                faceRotateAng += 5;
                if(faceRotateAng > 359) faceRotateAng = 0;
            }
        }
            break;
//        case 3:
//        {
//            ofPolyline leftEyePoly = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
//            ofPolyline rightEyePoly = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
//            ofPolyline noseBridgePoly = tracker.getImageFeature(ofxFaceTracker::NOSE_BRIDGE);
//            ofPolyline noseBasePoly = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE);
//            ofPolyline nosePoly;
//            nosePoly.addVertices(noseBasePoly.getVertices());
//            nosePoly.addVertices(noseBridgePoly.getVertices());
//            ofPolyline mouthPoly = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
//            faceFeatures[0] = leftEyePoly;
//            faceFeatures[1] = rightEyePoly;
//            faceFeatures[2] = nosePoly;
//            faceFeatures[3] = mouthPoly;
//
//            if(int(ofGetElapsedTimef()) % 3 == 0) {
//                for(int i = 0; i < 4; i++) {
//                    if(faceFeatureIndices[i] > 0) faceFeatureIndices[i] = faceFeatureIndices[i] - 1;
//                    else faceFeatureIndices[i] = 3;
//                }
//            }
//        }
//            break;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // switch effects
    switch(effectNum) {
        // Effect 0: Confetti party
        case 0: {
            // gray camera image
            vidimage.setFromPixels(viddata);
            grayvidimage.getTexture().disableAlphaMask();
            grayvidimage.setFromColorImage(vidimage);
            grayvidimage.blur(5);
            grayvidimage.draw(0, 0);
            
            // draw face features
            for(int i = 0; i < 9; i++) {
                ofPolyline pl = tracker.getImageFeature(features[i]);
                ofPath p;
                for(int j = 0; j < pl.getVertices().size(); j++) {
                    if(j == 0) {
                        p.newSubPath();
                        p.moveTo(pl.getVertices()[j]);
                    } else {
                        p.lineTo(pl.getVertices()[j]);
                    }
                }
                if(features[i] != ofxFaceTracker::JAW) {
                    if(!(features[i] == ofxFaceTracker::LEFT_EYEBROW || features[i] == ofxFaceTracker::RIGHT_EYEBROW || features[i] == ofxFaceTracker::NOSE_BASE)) p.close();
                    featureBoxes[i] = pl.getBoundingBox();
                }
                p.simplify();
                p.setFilled(false);
                p.setStrokeWidth(3);
                p.setStrokeColor(featureColors[i]);
                p.draw();
            }
            
            // set up obstacles
            setObstacles();
            
            // draw particles
            for(int i = 0; i < circles.size(); i++) {
                ofVec2f pos = circles[i].get()->getPosition();
                if(pos.x < 0 || pos.x > ofGetScreenWidth() || pos.y > ofGetScreenHeight()) {
                    circles.erase(circles.begin() + i);
                    i--;
                } else {
                    float r = circles[i].get()->getRadius();
                    ofRectangle rect(pos.x - r/2, pos.y - r/2, r, r);
                    for(int j = 0; j < 8; j++) {
                        ofRectangle edgeRect = featureBoxes[j];
                        ofColor color = featureColors[j];
                        if(edgeRect.intersects(rect)) {
                            circles[i].get()->color = color;
                            break;
                        }
                    }
                    circles[i].get()->drawParticle();
                }
            }
            for(int i=0; i<boxes.size(); i++) {
                ofVec2f pos = boxes[i].get()->getPosition();
                if(pos.x < 0 || pos.x > ofGetScreenWidth() || pos.y > ofGetScreenHeight()) {
                    boxes.erase(boxes.begin() + i);
                    i--;
                } else {
                    float w = boxes[i].get()->getWidth();
                    float h = boxes[i].get()->getHeight();
                    ofRectangle rect(pos.x, pos.y, w, h);
                    for(int j = 0; j < 8; j++) {
                        ofRectangle edgeRect = featureBoxes[j];
                        ofColor color = featureColors[j];
                        if(edgeRect.intersects(rect)) {
                            boxes[i].get()->color = color;
                            break;
                        }
                    }
                    boxes[i].get()->drawParticle();
                }
            }
            
            break;
        }
        // Effect 1: Breathing air onto windows
        case 1: {
            if(tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT) > 1.8f) {
                ofPolyline mouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
                ofRectangle rect = mouth.getBoundingBox();
                rect.scale(1.5);
                ofPoint cent = mouth.getCentroid2D();
                int radius = 80;
                for(int y = (cent.y - radius); y < (cent.y + radius); y++) {
                    for(int x = (cent.x - radius); x < (cent.x + radius); x++) {
                        if( (pow(x - cent.x, 2) + pow(y - cent.y, 2)) < radius*radius ) {
                            blurdata.setColor(x, y, 255);
                        }
                    }
                }
            }
            vidimage.setFromPixels(viddata);
            vidimage.blur(31);
            ofPixels blurpixels = vidimage.getPixels();
            
            for(int y = 0; y < camHeight; y++) {
                for(int x = 0; x < camWidth; x++) {
                    if(blurdata.getColor(x, y)[0] > 0) {
                        ofColor blurcolor = blurpixels.getColor(x, y);
                        blurcolor.a = blurdata.getColor(x, y)[0];
                        viddata.setColor(x, y, blurcolor);
                    }
                }
            }
            
            vidtexture.loadData(viddata);
            vidtexture.draw(0, 0);
            break;
        }
        // Effect 2: Anime-like dizzy eyes (x-ray spex comic strip)
        case 2: {
            
            vidimage.setFromPixels(viddata);
            grayvidimage.setFromColorImage(vidimage);
            grayvidimage.blur(5);
            
            if(tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT) > 1.8f) {
                float bottomY = tracker.getImageFeature(ofxFaceTracker::JAW).getBoundingBox().getBottom();
                ofPoint center = tracker.getImageFeature(ofxFaceTracker::NOSE_BRIDGE).getBoundingBox().getCenter();
                float topY = center.y - (bottomY - center.y);
                int faceRadius = int(bottomY - center.y) * 0.8;
                
                ofBackground(0);
                faceMask.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGBA);
                faceMask.begin();
                ofClear(0, 0, 0, 0);
                ofSetColor(255);
                ofDrawCircle(center.x, center.y, faceRadius);
                faceMask.end();
                grayvidimage.getTexture().setAlphaMask(faceMask.getTexture());
                ofPixels & grayvidimagepix = grayvidimage.getPixels();
                
                // Draw left dizzy eye
                ofPixels & leftPixels = leftEye.draw();
                ofPolyline leftEyePoly = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
                int leftRectSize = (leftEyePoly.getBoundingBox().getWidth() > leftEyePoly.getBoundingBox().getHeight() ) ? leftEyePoly.getBoundingBox().getWidth() * 2 : leftEyePoly.getBoundingBox().getHeight() * 2;
                ofPoint leftCentroid = leftEyePoly.getCentroid2D();
                ofPoint leftDrawPoint = leftCentroid;
                leftDrawPoint.x = leftDrawPoint.x - leftRectSize/2;
                leftDrawPoint.y = leftDrawPoint.y - leftRectSize/2;
                for(int y = 0; y < leftRectSize; y++) {
                    for(int x = 0; x < leftRectSize; x++) {
                        if( (pow(x - leftRectSize/2, 2) + pow(y - leftRectSize/2, 2)) < pow(leftRectSize/2, 2) ) {
                            ofColor leftPixelColor = leftPixels.getColor(x, y);
                            ofColor gray(leftPixelColor.getLightness());
                            grayvidimagepix.setColor(leftDrawPoint.x + x, leftDrawPoint.y + y, gray);
                        }
                    }
                }
                // Draw right dizzy eye
                ofPixels & rightPixels = rightEye.draw();
                ofPolyline rightEyePoly = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
                int rightRectSize = (rightEyePoly.getBoundingBox().getWidth() > rightEyePoly.getBoundingBox().getHeight() ) ? rightEyePoly.getBoundingBox().getWidth() * 2 : rightEyePoly.getBoundingBox().getHeight() * 2;
                ofPoint rightCentroid = rightEyePoly.getCentroid2D();
                ofPoint rightDrawPoint = rightCentroid;
                rightDrawPoint.x = rightDrawPoint.x - rightRectSize/2;
                rightDrawPoint.y = rightDrawPoint.y - rightRectSize/2;
                for(int y = 0; y < rightRectSize; y++) {
                    for(int x = 0; x < rightRectSize; x++) {
                        if( (pow(x - rightRectSize/2, 2) + pow(y - rightRectSize/2, 2)) < pow(rightRectSize/2, 2) ) {
                            ofColor rightPixelColor = rightPixels.getColor(x, y);
                            ofColor gray(rightPixelColor.getLightness());
                            grayvidimagepix.setColor(rightDrawPoint.x + x, rightDrawPoint.y + y, gray);
                        }
                    }
                }
                
                grayvidimage.setFromPixels(grayvidimagepix);
                grayvidimage.rotate(faceRotateAng, center.x, center.y);
                grayvidimage.draw(0, 0);
                
            } else {
                vidtexture.draw(0, 0);
            }
            break;
        }
        //Effect 3: swap facial features
//        case 3: {
//            vidtexture.draw(0, 0);
//            vidimage.setFromPixels(viddata);
//            ofPixels vidpixels = vidimage.getPixels();
//            ofRectangle rects [4];
//            ofPoint coords[4];
//            for(int i = 0; i < 4; i++) {
//                ofPolyline poly = faceFeatures[i];
//                ofRectangle rect = poly.getBoundingBox();
//                rect.scaleFromCenter(1.2);
//                if(i == 0 || i == 1) {
//                    rect.scaleFromCenter(1.8, 2);
//                    if(i == 0) rect.setX(rect.getLeft() - rect.getWidth() * 0.1);
//                    if(i == 1) rect.setX(rect.getLeft() + rect.getWidth() * 0.1);
//                }
//                if(i == 2) rect.scaleFromCenter(1.3, 1.2);
//                if(i == 3) rect.scaleFromCenter(1.2, 1.5);
//                rects[i] = rect;
//                coords[i] = rect.getTopLeft();
//            }
//
//            for(int i = 0; i < 4; i++) {
//                vidimage.resetROI();
//                int index = i;
//                index = faceFeatureIndices[i];
//                ofRectangle original = rects[i];
//                ofRectangle feature = rects[index];
//                float sX = original.getWidth() / feature.getWidth();
//                float sY = original.getHeight() / feature.getHeight();
//                vidimage.setROI(feature);
//                vidimage.resize(original.getWidth(), original.getHeight());
//                vidimage.drawROI(coords[i].x, coords[i].y);
//            }
//
//            break;
//        }
    }
    
    
    // display framerate and effect number
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    ofDrawBitmapString(ofToString((int) effectNum), 1270, 20);
    ofDrawBitmapString(ofToString((float) tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT)), 1200, 700);
    
}

void ofApp::setObstacles() {
    edges.clear();
    for(int i = 0; i < 9; i++) {
        ofPolyline pl = tracker.getImageFeature(features[i]);
        shared_ptr<ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
        edge->addVertexes(pl);
        if(i==8) edge->setPhysics(0, 0.5f, 0.5f);
        else edge->setPhysics(1.0f, 1.0f, 0.1f);
        edge->create(box2d.getWorld());
        edges.push_back(edge);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(effectNum < 2) effectNum++;
    else effectNum = 0;
}


