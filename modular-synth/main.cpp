//
//  main.cpp
//  modular-synth
//
//  Created by Do Thanh on 29/09/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <random>
#include <thread>
#include <chrono>
#include <math.h>
#include "RingBuffer.hpp"
#include "VCO.cpp"
#include "VCA.cpp"
#include "Cable.hpp"
#include <vector>
#include "Module.hpp"
#include "Audio.cpp"
#include "WaveFile.cpp"

const float SRATE = 44100.f;
const float SAMPLE_TIME = 1.f / SRATE;

Module::Module() {};
Module::~Module() {};

int main(int argc, const char * argv[]) {
    std::vector<Module*> modules;
    std::vector<Cable*> cables;

    // test patch: VCO -> VCA
    VCO* vco = new VCO();
    VCA* vca = new VCA();
    Cable* cable = new Cable();
    cable->inputId = VCO::SINE;
    cable->outputId = VCA::IN_1;
    cable->inputModule = vco;
    cable->outputModule = vca;

    modules.push_back(vco);
    modules.push_back(vca);
    cables.push_back(cable);

    ProcessArgs args;
    args.sampleRate = SRATE;
    args.sampleTime = SAMPLE_TIME;
    WaveFile<int32_t> f ("/Users/thdo/Downloads/test.wav", 2, 44100, 32);

    auto curTime = std::chrono::high_resolution_clock::now();
    auto lastTime = curTime;
    double overheadTime;

    // this is for printing to file (so we can plot and test)
    float x;
    auto firstTime = curTime;
    float duration;
    while (true) {
        for (Cable* c : cables) {
            c->step();
        }
        for (Module* m : modules) {
            m->process(args);
        }
        x = vca->outputs[VCA::OUT_1];

        f.writeData((int32_t) (x * 1000000000));

        curTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(curTime - firstTime).count();
//        myfile << x << " " << duration << " " << overheadTime << std::endl;
        
        if (duration >= 3) {
            break;
        }
        std::cout << std::endl;
        overheadTime = std::chrono::duration<double>(curTime - lastTime).count();
        lastTime = curTime;
        std::this_thread::sleep_for(std::chrono::duration<double>(SAMPLE_TIME - overheadTime));
    }
    f.finishWrite();
    std::cout << f.numOfSamples << std::endl;
    return 0;
}
