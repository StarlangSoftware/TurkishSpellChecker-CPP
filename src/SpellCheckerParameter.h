//
// Created by Olcay Taner YILDIZ on 23.11.2022.
//

#ifndef SPELLCHECKER_SPELLCHECKERPARAMETER_H
#define SPELLCHECKER_SPELLCHECKERPARAMETER_H


class SpellCheckerParameter {
private:
    double threshold = 0.0;
    bool deMiCheck = true;
    bool rootNGram = true;
public:
    SpellCheckerParameter();
    void setThreshold(double _threshold);
    void setDeMiCheck(bool _deMiCheck);
    void setRootNGram(bool _rootNGram);
    double getThreshold() const;
    bool isDeMiCheck() const;
    bool isRootNGram() const;
};


#endif //SPELLCHECKER_SPELLCHECKERPARAMETER_H
