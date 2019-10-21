//
//  WaveFile.cpp
//  modular-synth
//
//  Created by Do Thanh on 12/10/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <fstream>


/*
 network-byte-order: big endian
 host-byte-order: little endian
*/
//int get_endianness() {
//    // return 0 for big-endian system
//    int one = 1;
//    char *p = (char *) &one;
//    return *p;
//}

typedef struct WAVE_HEADER {
    // RIFF chunk descriptor (12 bytes)
    char RIFF[4]; // big endian
    uint32_t ChunkSize;
    char WAVE[4]; // big endian

    // fmt sub-chunk (24 bytes)
    char FmtChunkID[4]; // big endian
    uint32_t FmtChunkSize; // 16 for PCM
    uint16_t AudioFormat;
    uint16_t NumOfChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign; // number of bytes for 1 sample (including all channels)
    uint16_t BitsPerSample; // 8-bit, 16-bit, 32-bit,...
    
    // data sub-chunk (8 bytes + ...)
    char DataChunkID[4]; // big endian
    uint32_t DataChunkSize;
} wav_header;

template<typename T>
struct WaveFile {
    wav_header header;
    std::ofstream f;
    uint32_t numOfSamples = 0;
    float duration = 0.f;

    // default to PCM audio format for now
    WaveFile(std::string filePath, uint16_t numOfChannels, uint32_t sampleRate, uint32_t bitRate) {
        std::strncpy(header.RIFF, "RIFF", 4);
        header.ChunkSize = 36;
        std::strncpy(header.WAVE, "WAVE", 4);

        std::strncpy(header.FmtChunkID, "fmt ", 4);
        header.SampleRate = sampleRate;
        header.NumOfChannels = numOfChannels;
        header.FmtChunkSize = 16;
        header.AudioFormat = 1;
        header.ByteRate = numOfChannels * sampleRate * bitRate / 8;
        header.BlockAlign = numOfChannels * bitRate / 8;
        header.BitsPerSample = bitRate;

        std::strncpy(header.DataChunkID, "data", 4);
        header.DataChunkSize = 0;
        
        f.open(filePath, std::ios::binary);
        writeHeader();
    }
    
    ~WaveFile() {
//        if (f.is_open()) {
//            f.close();
//        }
        finishWrite();
    }
    
    void writeHeader() {
        if (f.is_open()) {
            f.write((char*)&header.RIFF, sizeof(header.RIFF));
            f.write((char*)&header.ChunkSize, sizeof(header.ChunkSize));
            f.write((char*)&header.WAVE, sizeof(header.WAVE));

            f.write((char*)&header.FmtChunkID, sizeof(header.FmtChunkID));
            f.write((char*)&header.FmtChunkSize, sizeof(header.FmtChunkSize));
            f.write((char*)&header.AudioFormat, sizeof(header.AudioFormat));
            f.write(reinterpret_cast<char*>(&header.NumOfChannels), sizeof(header.NumOfChannels));
            f.write(reinterpret_cast<char*>(&header.SampleRate), sizeof(header.SampleRate));
            f.write((char*)&header.ByteRate, sizeof(header.ByteRate));
            f.write((char*)&header.BlockAlign, sizeof(header.BlockAlign));
            f.write((char*)&header.BitsPerSample, sizeof(header.BitsPerSample));
            
            f.write((char*)&header.DataChunkID, sizeof(header.DataChunkID));
            f.write((char*)&header.DataChunkSize, sizeof(header.DataChunkSize));
        }
    }
    
    void writeData(int32_t sample) {
        if (f.is_open()) {
            for (int i = 0; i < header.NumOfChannels; i++) {
                f.write((char*)&sample, sizeof(int32_t));
            }
            numOfSamples++;
        }
    }
    
    void finishWrite() {
        if (f.is_open()) {
            uint32_t dataSize = numOfSamples * header.NumOfChannels * header.BitsPerSample / 8;
            header.ChunkSize += dataSize;
            header.DataChunkSize += dataSize;
            f.seekp(4, std::ios_base::beg);
            f.write((char*)&header.ChunkSize, sizeof(header.ChunkSize));
            f.seekp(40, std::ios_base::beg);
            f.write((char*)&header.DataChunkSize, sizeof(header.DataChunkSize));
            f.close();
        }
    }
};
