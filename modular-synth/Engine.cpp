//
//  Engine.cpp
//  modular-synth
//
//  Created by Do Thanh on 19/10/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <thread>
#include <condition_variable>

#include "Module.hpp"
#include "Cable.hpp"

//struct Engine {
//    struct Internal;
//    Internal* internal;
//
//    Engine();
//
//    void addModule(Module* module);
//
//    void addCable(Cable* cable);
//
//    void run();
//};
//
//struct ModuleRunner {
//    int id;
//    Engine* engine;
//    std::thread thread;
//
//    bool isRunning = false;
//
//    void run();
//
//    void start() {
//        thread = std::thread([&] {
//            run();
//        });
//    }
//};
//
//struct Engine::Internal {
//    std::vector<Module*> modules;
//    std::vector<Cable*> cables;
//
//    std::vector<ModuleRunner*> moduleRunners;
//
//    float sampleRate;
//    float sampleTime;
//
//    int threadCount = 0;
//};
//
//Engine::Engine() {
//    internal = new Internal();
//
//    if (internal->threadCount < 1) {
//        internal->threadCount = 1;
//
//        for (int i = 0; i < internal->threadCount; i++) {
//            ModuleRunner* m = new ModuleRunner();
//            m->id = i;
//            m->start();
//            internal->moduleRunners[i] = m;
//        }
//    }
//}
//
//void Engine::addModule(Module* module) {
//    internal->modules.push_back(module);
//}
//
//void Engine::addCable(Cable* cable) {
//    internal->cables.push_back(cable);
//}
//
//static void stepCable(Engine* engine) {
//    for (Cable* c: engine->internal->cables) {
//        c->step();
//    }
//}
//
//static void stepModules(Engine* engine) {
//    Engine::Internal* internal = engine->internal;
//    ProcessArgs processArgs;
//    processArgs.sampleRate = internal->sampleRate;
//    processArgs.sampleTime = internal->sampleTime;
//
//    for (Module* module: internal->modules) {
//        std::mutex mutex;
//        std::unique_lock<std::mutex> lock(mutex);
//        if (!module->bypass) {
//            module->process(processArgs);
//        }
//        lock.unlock();
//    }
//}
//
//static void process(Engine* engine) {
//    auto curTime = std::chrono::high_resolution_clock::now();
//    auto lastTime = curTime;
//    double overheadTime;
//
//    auto firstTime = curTime;
//    float duration;
//
//    float sampleTime = engine->internal->sampleTime;
//
//    while (1) {
//        stepCable(engine);
//        stepModules(engine);
//
//        curTime = std::chrono::high_resolution_clock::now();
//        duration = std::chrono::duration<double>(curTime - firstTime).count();
//
//        overheadTime = std::chrono::duration<double>(curTime - lastTime).count();
//        lastTime = curTime;
//        std::this_thread::sleep_for(std::chrono::duration<double>(sampleTime - overheadTime));
//    }
//};
//
//void Engine::run() {
//    process(this);
//}
//
//void ModuleRunner::run() {
//    stepModules(engine);
//}
