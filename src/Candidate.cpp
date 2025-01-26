//
// Created by Olcay Taner YILDIZ on 17.09.2022.
//

#include "Candidate.h"

/**
 * Constructs a new Candidate object with the specified candidate and operator.
 *
 * @param candidate The word candidate to be checked for spelling.
 * @param op The operator to be applied to the candidate in the spell checking process.
 */
Candidate::Candidate(const string& candidate, Operator op) : Word(candidate){
    this->op = op;
}

/**
 * Returns the operator associated with this candidate.
 *
 * @return The operator associated with this candidate.
 */
Operator Candidate::getOperator() const{
    return op;
}
