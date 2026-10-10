#include "CommandParser.h"
#include "Command.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <stdexcept>

namespace vimcube::command {

    namespace
    {
        struct Verb
        {
            std::string_view name;
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

        /*
            Function Name : findVerb
            Parameters : const std::string& input, size_t pos
            Return Type : const Verb*
            Description : Find verb in the given input statements
        */
        const Verb* findVerb(const std::string& input, size_t pos)
        {
            const Verb* bestCand = nullptr;
            for (const auto& v : vimcube::command::verbTable)
            {
                if ((input.compare(pos, v.name.size(), v.name) == 0))
                {
                    if (bestCand == nullptr || v.name.length() > bestCand->name.length())
                    {
                        bestCand = &v;
                    }
                }
            }
            return bestCand;
        }

        /*
            Function Name : readNumber
            Parameters : const std::string& input, size_t& pos, float& out
            Return Type : bool
            Description : Read number following by verb
        */
        bool readNumber(const std::string& input, size_t& pos, float& out)
        {
            size_t i = pos;
            size_t start = pos;

            size_t digitsBefore = 0;
            if (i < input.size())
            {
                // Read plus/minus
                if (input[i] == '-' || input[i] == '+')
                    i++;

                // Read integer part
                while (i < input.size() && input[i] >= '0' && input[i] <= '9')
                {
                    i++;
                    digitsBefore++;
                }
            }

            // Read float part
            size_t digitsAfter = 0;
            if (i < input.size() && input[i] == '.')
            {
                i++;
                while (i < input.size() && input[i] >= '0' && input[i] <= '9')
                {
                    i++;
                    digitsAfter++;
                }
            }
            
            if (digitsBefore + digitsAfter == 0) return false;

            // End parsing
            try
            {
                out = std::stof(input.substr(start, i - start));
                pos = i;
                return true;
            }
            catch (const std::out_of_range& e)
            {
                return false;
            }
        }

        /*
            Function Name : readAxis
            Parameters : char c, vimcube::math::Axis& axis
            Return Type : bool
            Description : Read axis following by number
        */
        bool readAxis(char c, vimcube::math::Axis& axis)
        {
            if (c == 'x')
            {
                axis = vimcube::math::Axis::X;
                return true;
            }
            else if (c == 'y')
            {
                axis = vimcube::math::Axis::Y;
                return true;
            }
            else if (c == 'z')
            {
                axis = vimcube::math::Axis::Z;
                return true;
            }
            return false;
        }

        /* Define error types */
        enum class ErrorType { UNKNOWN_VERB, NUMBER_EXPECTED, AXIS_EXPECTED };

        /*
            Function Name : makeError
            Parameters : ParseResult& result, const std::string& input, ErrorType err, const std::string& errorMsg, size_t pos
            Return Type : ParseResult
            Description : Make an error object
        */
        ParseResult makeError(ParseResult& result, const std::string& input, ErrorType err, const std::string& errorMsg, size_t pos)
        {
            ParseResult returnResult(false, errorMsg, pos);
            returnResult.partialCommands = std::move(result.commands);

            // Check imcomplete
            if (err == ErrorType::UNKNOWN_VERB)
                returnResult.incomplete = false;
            else if (err == ErrorType::NUMBER_EXPECTED)
            {
                bool isIncomplete = true;
                for (size_t i = pos; i < input.size(); i++)
                {
                    if (input[i] != '+' && input[i] != '-' && input[i] != '.' && input[i] != ' ')
                    {
                        returnResult.incomplete = false;
                        isIncomplete = false;
                        break;
                    }
                }
                if (isIncomplete)
                    returnResult.incomplete = true;
            }

            else if (err == ErrorType::AXIS_EXPECTED)
            {
                if (pos == input.size())
                    returnResult.incomplete = true;
            }

            return returnResult;
        }
    }

    /*
        Function Name : parse
        Parameters : const std::string& input
        Return Type : ParseResult
        Description : Parse the given keybinding input
    */
    ParseResult parse(const std::string& input)
    {
        ParseResult result;
        size_t pos = 0;

        while (pos < input.size())
        {
            const Verb* v = findVerb(input, pos);
            if (v == nullptr)
            {
                return makeError(result, input, ErrorType::UNKNOWN_VERB, "unknown verb", pos);
            }

            Command cmd;
            cmd.op = v->op;

            pos += v->name.size();

            if (v->needsValue)
            {
                if(!readNumber(input, pos, cmd.value))
                    return makeError(result, input, ErrorType::NUMBER_EXPECTED, "number expected", pos);
            }

            if (v->needsAxis)
            {
                if (!readAxis(input[pos], cmd.axis))
                    return makeError(result, input, ErrorType::AXIS_EXPECTED, "axis(x/y/z) expected", pos);
                pos++;
            }
            result.commands.push_back(cmd);
        }

        result.ok = true;
        return result;
    }
}
