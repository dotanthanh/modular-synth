//
//  VCA.cpp
//  modular-synth
//
//  Created by Do Thanh on 06/10/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#include <stdio.h>

#include "Module.hpp"
#include <math.h>

struct VCA : Module {
    enum InputIds {
        IN_1,
        MOD_IN,
        NUM_INPUTS
    };
    
    enum OutputIds {
        OUT_1,
        NUM_OUTPUTS
    };
    
    enum ParamIds {
        NUM_PARAMS
    };
    
    VCA() {
        config(NUM_INPUTS, NUM_OUTPUTS, NUM_PARAMS);
    };
    
    void process(const ProcessArgs& args) {
        if (inputs[IN_1]) {
            // temporary fixed rate for amplifier
            outputs[OUT_1] = 1.f * inputs[IN_1];
        }
    }
};
