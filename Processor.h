#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>

namespace cmd_calc {

    #define PRIORITY_LOW    0
    #define PRIORITY_HIGH   1
    #define BRACKET_LEVEL   3

    enum eCharType {
        eDIGIT,
        ePOINT,
        eOPER,
        eLB,
        eRB,
        eUNDEF
    };

    static const std::map<char, uint8_t> sc_priorities {{
        {'+', PRIORITY_LOW},
        {'-', PRIORITY_LOW},
        {'*', PRIORITY_HIGH},
        {'/', PRIORITY_HIGH}
    }};
    static const int sc_bracket_weight = BRACKET_LEVEL;

    class Processor {

        std::string m_line;
        std::string m_value;
        char m_oper;
        long long m_prior;
        long long m_bprior;
        eCharType m_last;

        inline bool is_point(const char ch);
        inline bool is_oper(const char ch);
        inline bool is_lb(const char ch);
        inline bool is_rb(const char ch);

        inline bool is_valid_digit();
        inline bool is_valid_point();
        inline bool is_valid_oper();
        inline bool is_valid_lb();
        inline bool is_valid_rb();

        void switch_char(const std::string::iterator chit);

        void flush() {
            m_value.clear();
            m_oper = 0;
            m_prior = -1;
        }
            
    public:
        explicit Processor(const std::string& line) : m_line(line) {
            flush();
            m_bprior = 0;
            m_last = eUNDEF;
        }
        ~Processor() = default;
        const double run();
    };
};


