//
//  dragBox.cpp
//  Upmixer
//
//  Created by Rithesh Kumar Ravikumar on 4/25/16.
//
//

#ifndef ThisIsCoptaa_dragBox_h
#define ThisIsCoptaa_dragBox_h

#include "../JuceLibraryCode/JuceHeader.h"

class dragBox : public Component {
public:
    dragBox(): _dragVal(0.35), _isSlider(true) {}
    ~dragBox(){}
    
    
    void mouseDrag (const MouseEvent& e) override   {
        
        if(e.y < getHeight() && e.y >= 0) {
            
            _dragVal = (1 - e.y*1.0f/getHeight()*1.0f);
            
            
        }
        repaint();
        
    }
    
    void paint(Graphics &g){
        g.fillAll(Colours::black);
        
        g.setColour(Colours::yellow);
        
        if (_isSlider){
            
            Rectangle<int> r(getLocalBounds().reduced(50));
            
            _dragVal = (_dragVal < 0.066f)?0.066f:_dragVal;
            _dragVal = (_dragVal > 0.990f)?0.990f:_dragVal;
            _dragVal *= 3.14/2;
            Path filledArc;
            filledArc.addPieSegment(r.getTopLeft().getX(), r.getTopLeft().getY(), r.getWidth(), r.getHeight()*2,
                                    - _dragVal,  + _dragVal, 0);
            g.fillPath (filledArc);
            
            
        }
        else {
            _dragVal = (_dragVal<0.34f)?0.34f:_dragVal;
            Rectangle<int> r(getLocalBounds().reduced(50/_dragVal));
            g.fillEllipse( r.getX() + getWidth()/12 + getWidth()/20, r.getY(), r.getHeight(), r.getHeight() );
        }
    }
    
    void setDragVal(float val) { _dragVal = val; }
    void setSlider(bool isSlider){ _isSlider = isSlider; }
    
private:
    float _dragVal;
    bool _isSlider;
    
};
#endif
