//
//  Module.hpp
//  modular-synth
//
//  Created by Do Thanh on 06/10/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#ifndef Module_hpp
#define Module_hpp

#include <stdio.h>
#include <vector>

struct ProcessArgs {
    int sampleRate;
    float sampleTime;
};

struct Module {
    int id = -1;
    std::vector<float> inputs;
    std::vector<float> outputs;
    std::vector<float> params;
    
    bool running = false;

    Module();

    virtual ~Module();
    
    virtual void process(const ProcessArgs& args) {};
    
    void config(int numInputs, int numOutputs, int numParams) {
        inputs.resize(numInputs);
        outputs.resize(numOutputs);
        params.resize(numParams);
    }
};

#endif /* Module_hpp */
