//
// Created by Olcay Taner YILDIZ on 17.09.2022.
//

#ifndef SPELLCHECKER_OPERATOR_H
#define SPELLCHECKER_OPERATOR_H

#endif //SPELLCHECKER_OPERATOR_H

enum Operator {
    /** No change is made to the Word. */
    NO_CHANGE,
    /** The Word is changed into a Word in the misspellings list */
    MISSPELLED_REPLACE,
    /** The Word is changed into a Candidate by deleting, adding, replacing a character or swapping two consecutive characters. */
    SPELL_CHECK,
    /** The Word is split into multiple Candidates. */
    SPLIT,
    /** The Word and the Word after are merged into one Candidate. */
    FORWARD_MERGE,
    /** The Word and the Word before are merged into one Candidate. */
    BACKWARD_MERGE,
    /** The Word is changed into a Candidate based on the context based spell checking algorithm. */
    CONTEXT_BASED,
    /** The Word is changed into a Candidate based on the trie based spell checking algorithm. */
    TRIE_BASED
};