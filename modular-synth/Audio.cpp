//
//  Audio.cpp
//  modular-synth
//
//  Created by Do Thanh on 06/10/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#include <stdio.h>
#include "Module.hpp"

struct Audio : Module {
    
    enum InputIds {
        IN,
        NUM_INPUTS
    };
    
    enum OutputIds {
        OUT,
        NUM_OUTPUTS
    };
    
    Audio() {};
    
    void process(const ProcessArgs& args) {
        outputs[OUT] = inputs[IN];
    }
};
