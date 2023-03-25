//
// Created by Olcay Taner YILDIZ on 23.11.2022.
//

#ifndef SPELLCHECKER_SPELLCHECKERPARAMETER_H
#define SPELLCHECKER_SPELLCHECKERPARAMETER_H

#include <string>

using namespace std;

class SpellCheckerParameter {
private:
    double threshold = 0.0;
    bool deMiCheck = true;
    bool rootNGram = true;
    int minWordLength = 4;
    string domain;
public:
    SpellCheckerParameter();
    void setThreshold(double _threshold);
    void setDeMiCheck(bool _deMiCheck);
    void setRootNGram(bool _rootNGram);
    void setMinWordLength(int _minWordLength);
    void setDomain(const string& _domain);
    double getThreshold() const;
    bool isDeMiCheck() const;
    bool isRootNGram() const;
    int getMinWordLength() const;
    string getDomain() const;
};


#endif //SPELLCHECKER_SPELLCHECKERPARAMETER_H
