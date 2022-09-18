//
// Created by Olcay Taner YILDIZ on 17.09.2022.
//

#ifndef SPELLCHECKER_CANDIDATE_H
#define SPELLCHECKER_CANDIDATE_H

#include <string>
#include "Operator.h"
#include "Dictionary/Word.h"

using namespace  std;

class Candidate : public Word{
private:
    Operator op;
public:
    Candidate(string candidate, Operator op);
    Operator getOperator();
};


#endif //SPELLCHECKER_CANDIDATE_H
