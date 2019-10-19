//
//  Cable.hpp
//  modular-synth
//
//  Created by Do Thanh on 06/10/2019.
//  Copyright © 2019 Do Thanh. All rights reserved.
//

#ifndef Cable_hpp
#define Cable_hpp

#include <stdio.h>

struct Cable {
    int id = -1;
    int inputId;
    int outputId;
    Module* inputModule = NULL;
    Module* outputModule = NULL;
    
    Cable(){};

    ~Cable() {
        inputModule = NULL;
        outputModule = NULL;
    };

    // set module (and specified port) for intake signal
    void setInputModule(Module* module, int moduleOutputId) {
        inputModule = module;
        inputId = moduleOutputId;
    }

    // set module (and specified port) for output signal
    void setOutputModule(Module* module, int moduleInputId) {
        outputModule = module;
        outputId = moduleInputId;
    }
    
    // copy output from inputModule to input in outputModule
    void step() {
        outputModule->inputs[outputId] = inputModule->outputs[inputId];
    }
};

#endif /* Cable_hpp */
