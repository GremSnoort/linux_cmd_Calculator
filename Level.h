#include <stdlib.h>
#include <string.h>
#include <string>

namespace cmd_calc {

    class Level {

        double m_val;
        char m_op;
        int m_prior;
        uint64_t m_level_id;

        const char get_oper() { return m_op; }

    public:
        Level* prev;
        Level* next;
        Level(const double in_val, const char in_op, const int in_prior, Level* in_prev);
        ~Level() = default;

        void exec();

        const double GetValue() { return m_val; }
        const int GetPriority() { return m_prior; }
    };
};


