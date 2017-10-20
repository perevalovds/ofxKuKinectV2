//
//  ofRGBPacketProcessor.h
//
//  Created by Theodore Watson on 6/23/14.
//
//

#pragma once

#include "ofImage.h"

#include <libfreenect2/rgb_packet_processor.h>
#include <libfreenect2/frame_listener.hpp>

namespace libfreenect2{

class ofRGBPacketProcessor : public RgbPacketProcessor{

    public:
        ofRGBPacketProcessor(bool active = true);
        ~ofRGBPacketProcessor();
    
         void process(const libfreenect2::RgbPacket &packet);

    protected:
        void newFrame();
        Frame *frame;
        ofPixels pix; 
		bool active_;

};

};