#include "Processor.h"
#include "Level.h"

#include <iostream>
#include <stdexcept>

namespace cmd_calc {

    const double Processor::run() {
        Level* curr = nullptr;
        auto chit = std::begin(m_line);
        do {
            switch_char(chit);
            ++chit;
            if (!m_value.empty() && (m_oper || chit == std::end(m_line))) {
                Level* lev = new Level(std::stod(m_value), m_oper, m_prior, curr);
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
                flush();
            }
        } while (chit != std::end(m_line));
        if (!curr) throw std::domain_error("invalid expression");
        else return curr->GetValue();
    }

    void Processor::switch_char(const std::string::iterator chit) {
        const char ch = *chit;
        if (std::isdigit(ch)) {
            if (is_valid_digit()) {
                m_value.push_back(ch);
                m_last = eDIGIT;
            } else {
                std::cout << "Digit '" << ch << "' on pos " << chit - std::begin(m_line) << std::endl;
                throw std::domain_error("unexpected digit");
            }
        } else if (is_point(ch)) {
            if (is_valid_point()) {
                m_value.push_back(ch);
                m_last = ePOINT;
            } else {
                std::cout << "Point symbol '" << ch << "' on pos " << chit - std::begin(m_line) << std::endl;
                throw std::domain_error("unexpected point symbol");
            }
        } else if (is_oper(ch)) {
            if (is_valid_oper()) {
                m_oper = ch;
                m_prior = m_bprior + sc_priorities.at(ch);
                m_last = eOPER;
            } else {
                std::cout << "Operator '" << ch << "' on pos " << chit - std::begin(m_line) << std::endl;
                throw std::domain_error("unexpected operator");
            }
        } else if (is_lb(ch)) {
            if (is_valid_lb()) {
                m_bprior += sc_bracket_weight;
                m_last = eLB;
            } else {
                std::cout << "Bracket symbol '" << ch << "' on pos " << chit - std::begin(m_line) << std::endl;
                throw std::domain_error("unexpected left bracket");
            }
        } else if (is_rb(ch)) {
            if (is_valid_rb()) {
                m_bprior -= sc_bracket_weight;
                m_last = eRB;
            } else {
                std::cout << "Bracket symbol '" << ch << "' on pos " << chit - std::begin(m_line) << std::endl;
                throw std::domain_error("unexpected right bracket");
            }
        } else if (std::isspace(ch)) {
        } else {
            std::cout << "Unknown symbol '" << ch << "' on pos " << chit - std::begin(m_line) << std::endl;
            throw std::domain_error("unexpected symbol");
        }
    }

    inline bool Processor::is_point(const char ch) {
        return ch == '.';
    }
    inline bool Processor::is_oper(const char ch) {
        return sc_priorities.count(ch);
    }
    inline bool Processor::is_lb(const char ch) {
        return ch == '(';
    }
    inline bool Processor::is_rb(const char ch) {
        return ch == ')';
    }

    inline bool Processor::is_valid_digit() {
        return m_last != eRB;
    }
    inline bool Processor::is_valid_point() {
        return m_last == eDIGIT;
    }
    inline bool Processor::is_valid_oper() {
        return m_last == eDIGIT || m_last == eRB;
    }
    inline bool Processor::is_valid_lb() {
        return m_last != eDIGIT && m_last != ePOINT && m_last != eRB;
    }
    inline bool Processor::is_valid_rb() {
        return (m_last == eDIGIT || m_last == eRB) && m_bprior - sc_bracket_weight >= 0;
    }
};


