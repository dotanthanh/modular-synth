//
//  VCO.cpp
//  modular-synth
//
//  Created by Do Thanh on 06/10/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#include "Module.hpp"
#include <math.h>

struct VCO : Module {
    float freq = 440.f;
    float phase = 0.f;

    enum InputIds {
        NUM_INPUTS
    };
    
    enum OutputIds {
        SINE,
        NUM_OUTPUTS
    };

    enum ParamIds {
        NUM_PARAMS
    };

    VCO() {
        config(NUM_INPUTS, NUM_OUTPUTS, NUM_PARAMS);
    };
    
    void process(const ProcessArgs& args) {
        phase += freq * args.sampleTime;
        phase -= phase > 1 ? 1.f : 0.f;
        outputs[SINE] = sin(2 * M_PI * phase);
    }
};
