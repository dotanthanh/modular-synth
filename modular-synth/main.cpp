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
//#include "Engine.cpp"

const float SAMPLE_RATE = 44100.f;
const float SAMPLE_TIME = 1.f / SAMPLE_RATE;

Module::Module() {};
Module::~Module() {};

WaveFile<int32_t> f ("/Users/thdo/Downloads/test.wav", 2, 44100, 32);

struct Engine {
    struct Internal;
    Internal* internal;
    
    Engine();
    
    void addModule(Module* module);
    
    void addCable(Cable* cable);
    
    void run();
};

struct ModuleRunner {
    int id;
    Engine* engine;
    std::thread thread;
    
    bool isRunning = false;
    
    void run();
    
    void start() {
        thread = std::thread([&] {
            run();
        });
    }
};

struct Engine::Internal {
    std::vector<Module*> modules;
    std::vector<Cable*> cables;
    
    std::vector<ModuleRunner*> moduleRunners;
    
    float sampleRate;
    float sampleTime;
    
    int threadCount = 0;
};

Engine::Engine() {
    internal = new Internal();
    internal->cables.resize(0);
    internal->modules.resize(0);
}

static void launchModuleRunners(Engine* engine) {
    Engine::Internal* internal = engine->internal;
    
    if (internal->threadCount < 1) {
        internal->threadCount = 1;
        internal->moduleRunners.resize(internal->threadCount);
        
        for (int i = 0; i < internal->threadCount; i++) {
            ModuleRunner* m = new ModuleRunner();
            m->engine = engine;
            m->id = i;
            m->start();
            internal->moduleRunners[i] = m;
        }
    }
}

void Engine::addModule(Module* module) {
    internal->modules.push_back(module);
}

void Engine::addCable(Cable* cable) {
    internal->cables.push_back(cable);
}

static void stepCable(Engine* engine) {
    for (Cable* c: engine->internal->cables) {
        c->step();
    }
}

static void stepModules(Engine* engine) {
    Engine::Internal* internal = engine->internal;
    ProcessArgs processArgs;
    processArgs.sampleRate = internal->sampleRate;
    processArgs.sampleTime = internal->sampleTime;
    
    for (Module* module: internal->modules) {
        std::lock_guard<std::mutex> lock(module->mutex);
        if (!module->bypass) {
            module->process(processArgs);
        }
    }
}

static void process(Engine* engine) {
    auto curTime = std::chrono::high_resolution_clock::now();
    auto lastTime = curTime;
    double overheadTime;
    
    auto firstTime = curTime;
    float duration;
    float x;
    float sampleTime = engine->internal->sampleTime;
    
    while (1) {
        if (engine->internal->threadCount < 1) {
            launchModuleRunners(engine);
        }
        stepCable(engine);
        stepModules(engine);

        x = engine->internal->modules[engine->internal->modules.size() - 1]->outputs[VCA::OUT_1];
        f.writeData((int32_t) (x * 1000000000));

        curTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(curTime - firstTime).count();
        
        overheadTime = std::chrono::duration<double>(curTime - lastTime).count();
        lastTime = curTime;
        std::this_thread::sleep_for(std::chrono::duration<double>(sampleTime - overheadTime));
    }
};

void Engine::run() {
    process(this);
}

void ModuleRunner::run() {
    stepModules(engine);
}


int main(int argc, const char * argv[]) {
//     std::vector<Module*> modules;
//     std::vector<Cable*> cables;

//     // test patch: VCO -> VCA
//     VCO* vco = new VCO();
//     VCA* vca = new VCA();
//     Cable* cable = new Cable();
//     cable->inputId = VCO::SINE;
//     cable->outputId = VCA::IN_1;
//     cable->inputModule = vco;
//     cable->outputModule = vca;

//     modules.push_back(vco);
//     modules.push_back(vca);
//     cables.push_back(cable);

//     ProcessArgs args;
//     args.sampleRate = SRATE;
//     args.sampleTime = SAMPLE_TIME;
//     WaveFile<int32_t> f ("/Users/thdo/Downloads/test.wav", 2, 44100, 32);

//     auto curTime = std::chrono::high_resolution_clock::now();
//     auto lastTime = curTime;
//     double overheadTime;

//     // this is for printing to file (so we can plot and test)
//     float x;
//     auto firstTime = curTime;
//     float duration;
//     while (true) {
//         for (Cable* c : cables) {
//             c->step();
//         }
//         for (Module* m : modules) {
//             m->process(args);
//         }
//         x = vca->outputs[VCA::OUT_1];

//         f.writeData((int32_t) (x * 1000000000));

//         curTime = std::chrono::high_resolution_clock::now();
//         duration = std::chrono::duration<double>(curTime - firstTime).count();
// //        myfile << x << " " << duration << " " << overheadTime << std::endl;
        
//         if (duration >= 3) {
//             break;
//         }
//         std::cout << std::endl;
//         overheadTime = std::chrono::duration<double>(curTime - lastTime).count();
//         lastTime = curTime;
//         std::this_thread::sleep_for(std::chrono::duration<double>(SAMPLE_TIME - overheadTime));
//     }
//     f.finishWrite();
//     std::cout << f.numOfSamples << std::endl;
    Engine* engine = new Engine();
    engine->internal->sampleRate = SAMPLE_RATE;
    engine->internal->sampleTime = SAMPLE_TIME;

    VCO* vco = new VCO();
    VCA* vca = new VCA();    
    engine->addModule(vco);
    engine->addModule(vca);
    Cable* cable = new Cable();
    cable->setInputModule(vco, VCO::SINE);
    cable->setOutputModule(vca, VCA::IN_1);
    engine->addCable(cable);

    // add 10 more VCA next to each other
    for (int i = 1; i < 10; i++) {
       VCA* nextVCA = new VCA();
       engine->addModule(nextVCA);
       Cable* c = new Cable();
       c->setInputModule(engine->internal->modules[i], VCA::OUT_1);
       c->setOutputModule(engine->internal->modules[i + 1], VCA::IN_1);
       engine->addCable(c);
    }

    engine->run();
    std::cout << "Running .." << std::endl;

    return 0;
}
