//
// Created by Olcay Taner YILDIZ on 17.09.2022.
//

#include "Candidate.h"

Candidate::Candidate(const string& candidate, Operator op) : Word(candidate){
    this->op = op;
}

Operator Candidate::getOperator() const{
    return op;
}
