//
// Created by Olcay Taner YILDIZ on 23.11.2022.
//

#include "SpellCheckerParameter.h"

SpellCheckerParameter::SpellCheckerParameter() = default;

void SpellCheckerParameter::setThreshold(double _threshold) {
    this->threshold = _threshold;
}

void SpellCheckerParameter::setDeMiCheck(bool _deMiCheck) {
    this->deMiCheck = _deMiCheck;
}

void SpellCheckerParameter::setRootNGram(bool _rootNGram) {
    this->rootNGram = _rootNGram;
}

double SpellCheckerParameter::getThreshold() const{
    return threshold;
}

bool SpellCheckerParameter::isDeMiCheck() const {
    return deMiCheck;
}

bool SpellCheckerParameter::isRootNGram() const {
    return rootNGram;
}
