#include "heval.hpp"

void Eval::hbeta () {
    int count = 0;
    while (TA->getTerm()->isEvalable() && count <= 1000) {
        TA->getTerm()->apply();
        count++;
    } 
}

bool Eval::isEvalable() {
    if (TA->getTerm()->getValueID() == AppTermID) {
        if (TA->getTerm()->getTerm(0)->getValueID() == AbsTermID &&
                TA->getTerm()->getTermssize() >= 2) {
            return true;
        }
    }
    return false;
}
