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
    ~Cable(){};
    
    void step() {
        outputModule->inputs[outputId] = inputModule->outputs[inputId];
    }
};

#endif /* Cable_hpp */
