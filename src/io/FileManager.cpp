#include "FileManager.h"
#include <cstddef>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../command/CommandParser.h"

namespace vimcube::file
{
    namespace
    {
        /*
            Function Name : opToString
            Parameters : -
            Return Type : const vimcube::command::Op op
            Description : Convert Op enum class to string
        */
        std::string opToString(const vimcube::command::Op op)
        {
            switch (op)
            {
                case vimcube::command::Op::CreateCube:
                    return "CreateCube";
                case vimcube::command::Op::CreateSphere:
                    return "CreateSphere";
                case vimcube::command::Op::Move:
                    return "Move";
                case vimcube::command::Op::Scale:
                    return "Scale";
                case vimcube::command::Op::ScaleUniform:
                    return "ScaleUniform";
                case vimcube::command::Op::Rotate:
                    return "Rotate";
                case vimcube::command::Op::Nothing:
                    return "Nothing";
                case vimcube::command::Op::Delete:
                    return "Delete";

            }
            return "Nothing";
        }

        /*
            Function Name : axisToString
            Parameters : -
            Return Type : const vimcube::math::Axis axis
            Description : Convert Axis enum class to string
        */
        std::string axisToString(const vimcube::math::Axis axis)
        {
            switch (axis)
            {
                case vimcube::math::Axis::X:
                    return "x";
                case vimcube::math::Axis::Y:
                    return "y";
                case vimcube::math::Axis::Z:
                    return "z";
            }
            return "x";
        }
    }

    /*
        Function Name : getFilePath
        Parameters : -
        Return Type : const std::string&
        Description : Get file path
    */
    const std::string& FileManager::getFilePath() const
    {
        return this->filePath;
    }

    /*
        Function Name : getVersion
        Parameters : -
        Return Type : uint32_t
        Description : Get file version
    */
    uint32_t FileManager::getVersion() const
    {
        return this->version;
    }
    
    /*
        Function Name : writeFile
        Parameters : const std::string& path, const vimcube::geometry::Scene& scene
        Return Type : SaveResult
        Description : Write JSON file
    */
    SaveResult FileManager::writeFile(const std::string& path, const vimcube::geometry::Scene& scene)
    {
        SaveResult result;
        uint32_t currentVersion = this->version + 1;

        nlohmann::json j;
        j["version"] = currentVersion;

        // geometries array
        nlohmann::json bigArr = nlohmann::json::array();

        for (const auto& g : scene.sceneGeos)
        {
            // geometries info object
            nlohmann::json infoObj = nlohmann::json::object();
            infoObj["id"] = g.getId();
            infoObj["source_sequence"] = g.getSequenceRecord();

            // command array
            nlohmann::json commandsArr = nlohmann::json::array();
            vimcube::command::ParseResult parsed = vimcube::command::parse(g.getSequenceRecord());
            if (parsed.ok)
            {
                if (parsed.commands.size() == 0)
                {
                    result.errMsg = std::string("Command does not exist") + "(ID : " + std::to_string(g.getId()) + ")";
                    return result;
                }
                for (size_t i = 0; i < parsed.commands.size(); i++)
                {
                    vimcube::command::Command cmd = parsed.commands.at(i);
                    nlohmann::json commandObj = nlohmann::json::object();
                    commandObj["step"] = i;
                    commandObj["op"] = opToString(cmd.op);
                    if (vimcube::command::getIsAxisNeeded(cmd.op))
                    {
                        commandObj["axis"] = axisToString(cmd.axis);
                    }
                    if (vimcube::command::getIsValueNeeded(cmd.op))
                    {
                        commandObj["value"] = cmd.value;
                    }
                    commandsArr.push_back(commandObj);
                }
                infoObj["commands"] = commandsArr;
            }
            else
            {
                result.errMsg = std::string("Cannot parse commands") + "(ID : " + std::to_string(g.getId()) + ")";
                return result;
            }
            bigArr.push_back(infoObj);
        }
        j["geometries"] = bigArr;

        std::ofstream fout(path);
        if(!fout.is_open())
        {
            result.errMsg = "Cannot write a file";
            return result;
        }
        fout << j.dump(4);
        fout.close();
        if (!fout.fail())
        {
            result.ok = true;
            this->filePath = path;
            this->version = currentVersion;
        }
        else
        {
            result.errMsg = "File writing failed";
            return result;
        }
        return result;
    }
}
