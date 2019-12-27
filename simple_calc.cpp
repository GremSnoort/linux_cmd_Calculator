#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

static int s_levels = 0;

class Level {

    double m_val;
    char m_op;
    int m_prior;

    const char get_oper() {
        return m_op;
    }
public:
    Level* prev;
    Level* next;
    Level(const double in_val, const char in_op, const int in_prior, Level* in_prev) {
        m_val = in_val;
        m_op = in_op;
        m_prior = in_prior;
        prev = in_prev;
        next = nullptr;
        ++s_levels;
        std::cout << "create level #" << s_levels << ": val: " << m_val << "; oper: " << m_op << "; prior: " << m_prior << std::endl;
    }
    ~Level() = default;

    const double GetValue() {
        return m_val;
    }
    const int GetPriority() {
        return m_prior;
    }

    void exec() {
        if (next) {
            double rv = next->GetValue();
            switch (m_op) {
                case '+':
                    m_val += rv;
                    break;
                case '-':
                    m_val -= rv;
                    break;
                case '*':
                    m_val *= rv;
                    break;
                case '/':
                    m_val /= rv;
                    break;
                default: break;
            }
            m_op = next->get_oper();
            m_prior = next->GetPriority();
            std::cout << "            Exec: val: " << m_val << "; oper: " << m_op << "; prior: " << m_prior << std::endl;
            delete next;
            --s_levels;
            next = nullptr;
        }
    }
};

enum eCharType {
    eDIGIT,
    ePOINT,
    eOPER,
    eLB,
    eRB,
    eUNDEF
};
static const std::map<char, uint8_t> sc_priorities {{
    {'+', 0},
    {'-', 0},
    {'*', 1},
    {'/', 1}
}};
static const int sc_bracket_weight = 3;

const double process(const std::string& line) {
    std::string value = "";
    char oper = 0;
    long long prior = -1;
    long long bprior = 0;
    eCharType last = eUNDEF;
    Level* curr = nullptr;
    auto chit = std::begin(line);
    do {
        const char ch = *chit;
        if (std::isdigit(ch)) {
            if (last != eRB) {
                value.push_back(ch);
                last = eDIGIT;
            } else {
                std::cout << "Digit '" << ch << "' on pos " << chit - std::begin(line) << std::endl;
                throw std::domain_error("unexpected digit");
            }
        } else if (ch == '.') {
            if (last == eDIGIT) {
                value.push_back(ch);
                last = ePOINT;
            } else {
                std::cout << "Point symbol '" << ch << "' on pos " << chit - std::begin(line) << std::endl;
                throw std::domain_error("unexpected point symbol");
            }
        } else if (sc_priorities.count(ch)) {
            if (last == eDIGIT || last == eRB) {
                oper = ch;
                prior = bprior + sc_priorities.at(ch);
                last = eOPER;
            } else {
                std::cout << "Operator '" << ch << "' on pos " << chit - std::begin(line) << std::endl;
                throw std::domain_error("unexpected operator");
            }
        } else if (ch == '(') {
            if (last != eDIGIT && last != ePOINT && last != eRB) {
                bprior += sc_bracket_weight;
                last = eLB;
            } else {
                std::cout << "Bracket symbol '" << ch << "' on pos " << chit - std::begin(line) << std::endl;
                throw std::domain_error("unexpected left bracket");
            }
        } else if (ch == ')') {
            if ((last == eDIGIT || last == eRB) && bprior - sc_bracket_weight >= 0) {
                bprior -= sc_bracket_weight;
                last = eRB;
            } else {
                std::cout << "Bracket symbol '" << ch << "' on pos " << chit - std::begin(line) << std::endl;
                throw std::domain_error("unexpected right bracket");
            }
        } else if (std::isspace(ch)) {
        } else {
            std::cout << "Unknown symbol '" << ch << "' on pos " << chit - std::begin(line) << std::endl;
            throw std::domain_error("unexpected symbol");
        }
        ++chit;
        if (!value.empty() && (oper || chit == std::end(line))) {
            Level* lev = new Level(std::stod(value), oper, prior, curr);
            if (curr) {
                curr->next = lev;
                curr = curr->next;
            } else curr = lev;
            while (curr->prev && (
                curr->prev->GetPriority() > curr->GetPriority() ||
                curr->prev->GetPriority() == curr->GetPriority())) {
                curr = curr->prev;
                curr->exec();
            }
            value.clear();
            oper = 0;
            prior = -1;
        }
    } while (chit != std::end(line));
    if (!curr) throw std::domain_error("invalid expression");
    return curr->GetValue();
}

int main() {
    std::string line;
    while (!std::cin.eof()) {
        std::getline(std::cin, line);
        if (!line.compare("exit") ||
            !line.compare("quit") ||
            !line.compare("q") ||
            !line.compare("e")) {
            break;
        } else if (line.length() > 0) {
            try {
                std::cout << process(line) << std::endl;
            } catch (std::exception &e) {
                std::cout << "error: " << e.what() << std::endl;
            }
        }
    }
    return 0;
}


