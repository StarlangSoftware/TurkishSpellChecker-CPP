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
    bool suffixCheck = true;
    bool rootNGram = true;
    int minWordLength = 4;
    string domain;
public:
    SpellCheckerParameter();
    void setThreshold(double _threshold);
    void setSuffixCheck(bool _suffixCheck);
    void setRootNGram(bool _rootNGram);
    void setMinWordLength(int _minWordLength);
    void setDomain(const string& _domain);
    [[nodiscard]] double getThreshold() const;
    [[nodiscard]] bool isSuffixCheck() const;
    [[nodiscard]] bool isRootNGram() const;
    [[nodiscard]] int getMinWordLength() const;
    [[nodiscard]] string getDomain() const;
};


#endif //SPELLCHECKER_SPELLCHECKERPARAMETER_H
