#include "heval.hpp"

Eval::hbeta () {
    int count = 0;
    while (TA->Term->isEvalable && cout <= 1000) {
        TA->Term->apply();
        cout++;
    } 
}

Eval::isEvalable() {
    if (TA->Term->getValueID() == AppTermID) {
        if (TA->Term->Terms[0]->getValueID() == AbsTermID &&
                TA->Term->Terms.size() >= 2) {
            return true;
        }
    }
    return false;
}
