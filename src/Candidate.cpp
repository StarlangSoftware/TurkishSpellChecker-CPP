//
// Created by Olcay Taner YILDIZ on 17.09.2022.
//

#include "Candidate.h"

Candidate::Candidate(string candidate, Operator op) : Word(candidate){
    this->op = op;
}

Operator Candidate::getOperator() {
    return op;
}
