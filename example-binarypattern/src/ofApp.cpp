#include "ofMain.h"

#include "ofxBinaryPatternMessanger.h"

struct ScopedTimeProfile
{
    float ts;
    std::string label;
    
    ScopedTimeProfile(std::string label) : label(label) {
        ts = ofGetElapsedTimef();
    }
    ~ScopedTimeProfile(){
        std::cerr << label << " : " << (ofGetElapsedTimef() - ts) * 1000 * 1000 << "usec" << std::endl;
    }
};

class ofApp : public ofBaseApp{
    ofImage img;
    string data = "Hello! This message is sent via binary image";
    string text;
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        
        stringstream ss;
        for (int i=0; i<51200; ++i) {
            ss << (char)ofRandom(70, 100);
        }
        data += ss.str();
        
        img.allocate(1024, 512, OF_IMAGE_COLOR);
        {
            ScopedTimeProfile s("encode");
            ofxBinaryPatternMessanger::encodeToImage((const unsigned char*)data.c_str(), data.size(), img.getPixels());
        }
        img.update();
        {
            ScopedTimeProfile s("decode");
            auto sz = ofxBinaryPatternMessanger::fetchSizeFromImage(img.getPixels());
            ofBuffer b;
            b.allocate(sz);
            ofxBinaryPatternMessanger::decodeFromImage((unsigned char*)b.getData(), img.getPixels());
            text = b;
        }
    }
    
    void update()
    {
    }
    
    void draw()
    {
        ofClear(128);
        
        ofDrawBitmapStringHighlight(data, 10, 20, ofColor::red);
        ofDrawBitmapStringHighlight(text, 10, 40, ofColor::blue);
        auto sz = ofxBinaryPatternMessanger::fetchSizeFromImage(img.getPixels());
        ofDrawBitmapStringHighlight(ofToString(sz), 10, 60, ofColor::green);
        ofDrawBitmapStringHighlight(ofToString(data.size()), 10, 80, ofColor::green);
        
        img.draw(0, 100);
        
        
    }
    
    void keyPressed(int key)
    {
        if (key == 'f') {
            ofToggleFullscreen();
        }
    }
    
    void keyReleased(int key) {}
    void mouseMoved(int x, int y ) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void windowResized(int w, int h) {}
    void dragEvent(ofDragInfo dragInfo) {}
    void gotMessage(ofMessage msg) {}
    
};

//========================================================================
int main( ){
    ofSetupOpenGL(1280,720,OF_WINDOW);            // <-------- setup the GL context
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
