//
//  peakBar.cpp
//  Vibrato2Plugin
//
//  Created by Rithesh Kumar Ravikumar on 4/19/16.
//
//

#ifndef ThisIsCoptaa_CopterBody_h
#define ThisIsCoptaa_CopterBody_h

#include "../JuceLibraryCode/JuceHeader.h"

class peakBar : public Component {
public:
    peakBar(): _peakVal(0.5f), _isVert(false){}
    ~peakBar(){}
    
    
    void mouseDrag (const MouseEvent& e) override   {
        
        //        dragger.dragComponent (this, e, &constrainer);
        if(e.y < getHeight() && e.y >= 0) {
            _peakVal = (1 - e.y*1.0f/getHeight()*1.0f); //std::cout<<_peakVal<<std::endl;
        }
        repaint();
        
    }
    
    void paint(Graphics &g){
        g.setColour(Colours::yellow);
        g.fillRect( 0, 0, getWidth(), getHeight() );
        g.setColour(Colours::green);
        
        if (_isVert) {
            int peakLevel = _peakVal*getHeight();
            g.fillRect( getWidth()*0.13, getHeight()-peakLevel, getWidth()*0.75, getHeight() );
        }
        else {
            int peakLevel = _peakVal*getWidth();
            g.fillRect( 0, getHeight()*(0.13), peakLevel , getHeight()*0.75 );
        }
        
    }
    
    void resized() override {
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(), getHeight(), getWidth());
    }
    
    
    
    void setPeakVal(float val) { _peakVal = val; }
    void setVertical(bool isVert){ _isVert = isVert; }
    
private:
    float _peakVal;
    bool _isVert;
    
    
    ComponentBoundsConstrainer constrainer;
    ComponentDragger dragger;
};
#endif
