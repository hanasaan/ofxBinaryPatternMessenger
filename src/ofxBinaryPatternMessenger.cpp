//
//  ofxBinaryPatternMessenger.cpp
//  example-binarypattern
//
//  Created by Yuya Hanai on 2017/12/07.
//
//

#include "ofxBinaryPatternMessenger.h"

namespace ofxBinaryPatternMessenger
{
    bool encodeToImage(const unsigned char* data, size_t length, ofPixels& out, size_t offset_pix)
    {
        uint32_t sz = length;
        size_t length_act = sizeof(sz) + length;
        if (!out.isAllocated() || out.size() < length_act * out.getNumChannels() * 8 + offset_pix * out.getNumChannels()) {
            return false;
        }
        int numch = out.getNumChannels();
        for (int i=0; i<length_act; ++i) {
            int ptr = (offset_pix + i * 8) * numch;
            unsigned char v = i < sizeof(sz) ? (((unsigned char*)(&sz))[i]) : data[i - sizeof(sz)];
            for (int j=0; j<8; ++j) {
                unsigned char outv = (v & 0x01) * 255;
                for (int k=0; k<numch; ++k, ++ptr) {
                    out.getData()[ptr] = outv;
                }
                v >>= 1;
            }
        }
        
        return true;
    }
    
    bool decodeFromImageInternal(unsigned char* data, size_t length, const ofPixels& in, size_t offset_pix, size_t decode_ch_offset)
    {
        if (!in.isAllocated() || in.size() < length * in.getNumChannels() * 8 + offset_pix * in.getNumChannels()) {
            return false;
        }
        int numch = in.getNumChannels();
        for (int i=0; i<length; ++i) {
            int ptr = (offset_pix + i * 8) * numch + decode_ch_offset;
            unsigned char v = 0;
            for (int j=0; j<8; ++j, ptr+=numch) {
                if (in.getData()[ptr] >= 128) {
                    v |= 0x01 << j;
                }
            }
            data[i] = v;
        }
        
        return true;
    }
    
    size_t fetchSizeFromImage(const ofPixels& in, size_t offset_pix, size_t decode_ch_offset)
    {
        size_t length = 4;
        uint32_t sz = 0;
        if (decodeFromImageInternal((unsigned char*)&sz, length, in, offset_pix, decode_ch_offset) && sz > 0) {
            return sz;
        }
        return sz;
    }


    bool decodeFromImage(unsigned char* data, const ofPixels& in, size_t offset_pix, size_t decode_ch_offset)
    {
        size_t length = 4;
        uint32_t sz = 0;
        if (decodeFromImageInternal((unsigned char*)&sz, length, in, offset_pix, decode_ch_offset) && sz > 0) {
            return decodeFromImageInternal(data, sz, in,
                                           offset_pix + sizeof(sz)*8, decode_ch_offset);
        }
        return false;
    }
}