#include "Level.h"

#include <iostream>

namespace cmd_calc {

    static uint64_t gs_levels = 0;

    Level::Level(const double in_val, const char in_op, const int in_prior, Level* in_prev)
        : m_val(in_val), m_op(in_op), m_prior(in_prior), m_level_id(++gs_levels), prev(in_prev), next(nullptr) {
        std::cout << " Created level #:" << m_level_id << ": " << m_val << " " << m_op << " : " << m_prior << std::endl;
    }

    void Level::exec() {
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
            std::cout << "Executed level #:" << m_level_id << ": " << m_val << " " << m_op << " : " << m_prior << std::endl;
            delete next;
            --gs_levels;
            next = nullptr;
        }
    }
};


