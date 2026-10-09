#include "CommandParser.h"
#include <array>
#include "Command.h"


namespace vimcube::command
{
    namespace verb
    {
        struct Verb
        {
            const char* name;
            Op op;
            bool needsValue = false;
            bool needsAxis = false;
        };

        const std::array verbTable = { 

            Verb{ "c", Op::CreateCube, 1, 0 },
            Verb{ "p", Op::CreateSphere, 1, 0 },
            Verb{ "m", Op::Move, 1, 1 },
            Verb{ "ww", Op::ScaleUniform, 1, 0 },
            Verb{ "w", Op::Scale, 1, 1 },
            Verb{ "r", Op::Rotate, 1, 1 }
            
        };
    }

    namespace parser
    {
        const verb::Verb* findVerb(const std::string& input, size_t pos)
        {

        }
    }
    ParseResult parse(const std::string& input)
    {

    }
}
