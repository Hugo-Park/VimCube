#include "FileManager.h"
#include <set>
#include <cstdint>
#include <cstddef>
#include <exception>
#include <fstream>
#include <limits>
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
        uint32_t newVersion = this->version + 1;

        nlohmann::ordered_json j;
        j["version"] = newVersion;

        // geometries array
        nlohmann::ordered_json bigArr = nlohmann::ordered_json::array();

        for (const auto& g : scene.sceneGeos)
        {
            // geometries info object
            nlohmann::ordered_json infoObj = nlohmann::ordered_json::object();
            infoObj["id"] = g.getId();
            infoObj["source_sequence"] = g.getSequenceRecord();

            // command array
            nlohmann::ordered_json commandsArr = nlohmann::ordered_json::array();
            vimcube::command::ParseResult parsed = vimcube::command::parse(g.getSequenceRecord());
            if (parsed.ok)
            {
                if (parsed.commands.size() == 0)
                {
                    result.errMsg = std::string("JSON write error: no commands") + "(Geo ID : " + std::to_string(g.getId()) + ")";
                    return result;
                }
                for (size_t i = 0; i < parsed.commands.size(); i++)
                {
                    vimcube::command::Command cmd = parsed.commands.at(i);
                    nlohmann::ordered_json commandObj = nlohmann::ordered_json::object();
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
                result.errMsg = std::string("JSON write error: invalid commands ") + "(Geo ID : " + std::to_string(g.getId()) + ")";
                return result;
            }
            bigArr.push_back(infoObj);
        }
        j["geometries"] = bigArr;

        std::ofstream fout(path);
        if(!fout.is_open())
        {
            result.errMsg = "JSON write error: cannot write a file";
            return result;
        }
        fout << j.dump(4);
        fout.close();
        if (!fout.fail())
        {
            result.ok = true;
            result.errMsg = "Successfully saved to " + path;
            this->filePath = path;
            this->version = newVersion;
        }
        else
        {
            result.errMsg = "JSON write error: cannot write a file";
            return result;
        }
        return result;
    }

    /*
        Function Name : loadFile
        Parameters : const std::string& path
        Return Type : LoadResult
        Description : Load JSON file
    */
    LoadResult FileManager::loadFile(const std::string& path)
    {
        LoadResult result;
        std::ifstream fin(path);
        if (!fin.is_open())
        {
            result.errMsg = "Cannot open a file";
            return result;
        }

        try
        {
            nlohmann::ordered_json j = nlohmann::ordered_json::parse(fin);

            if (!j.is_object())
            {
                result.errMsg = "JSON parse error: not an object";
                return result;
            }
            if (!j.contains("version") || !j["version"].is_number_unsigned())
            {
                result.errMsg = "JSON parse error: invalid version";
                return result;
            }
            if (j["version"].get<uint64_t>() > std::numeric_limits<uint32_t>::max())
            {
                result.errMsg = "JSON parse error: invalid version";
                return result;
            }
            if (!j.contains("geometries") || !j["geometries"].is_array())
            {
                result.errMsg = "JSON parse error: invalid geometry records";
                return result;
            }

            std::set<uint32_t> testIdSet;
            for (const auto& g : j["geometries"])
            {
                if (!g.is_object())
                {
                    result.errMsg = "JSON parse error: not an object";
                    return result;
                }
                if (!g.contains("id"))
                {
                    result.errMsg = "JSON parse error: invalid ID";
                    return result;
                }
                if (!g["id"].is_number_unsigned())
                {
                    result.errMsg = "JSON parse error: invalid ID";
                    return result;
                }
                if (g["id"].get<uint64_t>() > std::numeric_limits<uint32_t>::max())
                {
                    result.errMsg = "JSON parse error: invalid ID";
                    return result;
                }
                if (testIdSet.find(g["id"].get<uint32_t>()) != testIdSet.end())
                {
                    result.errMsg = "JSON parse error: identical ID exists";
                    return result;
                }
                testIdSet.insert(g["id"].get<uint32_t>());

                if (!g.contains("source_sequence"))
                {
                    result.errMsg = std::string("JSON parse error: invalid sequence") + "(Geo ID : " + std::to_string(g["id"].get<uint32_t>()) + ")";
                    return result;
                }
                if (!g["source_sequence"].is_string() || g["source_sequence"].get<std::string>().empty())
                {
                    result.errMsg = std::string("JSON parse error: invalid sequence") + "(Geo ID : " + std::to_string(g["id"].get<uint32_t>()) + ")";
                    return result;
                }
                if (!vimcube::command::parse(g["source_sequence"]).ok)
                {
                    result.errMsg = std::string("JSON parse error: sequence parse failed") + "(Geo ID : " + std::to_string(g["id"].get<uint32_t>()) + ")";
                    return result;
                }

                // Everything successfully parsed
                GeometryRecord geoRecord;
                geoRecord.id = g["id"].get<uint32_t>();
                geoRecord.sequenceRecord = g["source_sequence"].get<std::string>();
                result.geometries.push_back(geoRecord);
            }

            fin.close();
            this->filePath = path;
            this->version = j["version"].get<uint32_t>();
            result.ok = true;
            result.errMsg = "Successfully loaded";

            return result;
        }
        catch (const nlohmann::ordered_json::parse_error& e)
        {
            result.errMsg = std::string("JSON parse error: ") + e.what();
            return result;
        }
        catch (const std::exception& e)
        {
            result.errMsg = std::string("Error: ") + e.what();
            return result;
        }
    }

    /*
        Function Name : reset
        Parameters : -
        Return Type : void
        Description : Reset FileManager
    */
    void FileManager::reset()
    {
        this->filePath.clear();
        this->version = 0;
    }
}
