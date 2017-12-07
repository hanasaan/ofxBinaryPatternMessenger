//
//  ofxBinaryPatternMessenger.h
//  example-binarypattern
//
//  Created by Yuya Hanai on 2017/12/07.
//
//
#pragma once

#include "ofMain.h"

namespace ofxBinaryPatternMessenger
{
    bool encodeToImage(const unsigned char* data, size_t length, ofPixels& out, size_t offset_pix = 0);
    
    size_t fetchSizeFromImage(const ofPixels& in, size_t offset_pix = 0, size_t decode_ch_offset = 0);
    bool decodeFromImage(unsigned char* data, const ofPixels& in, size_t offset_pix = 0, size_t decode_ch_offset = 0);
}