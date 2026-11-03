#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <vector>
#include <string>
#include <cstdint>
#include "../geometry/Scene.h"
#include "../utility/Utils.h"

namespace vimcube::file
{
    struct GeometryRecord
    {
        uint32_t id = 0;
        std::string sequenceRecord;
        GeometryRecord() = default;
    };

    struct SaveResult
    {
        bool ok = false;
        std::string errMsg;
        SaveResult() = default;
    };

    struct LoadResult
    {
        bool ok = false;
        std::string errMsg;
        std::vector<GeometryRecord> geometries;
        LoadResult() = default;
    };

    class FileManager
    {
    private:
        std::string fileName = "untitled.json";
        std::string defaultPath = setDefaultPath();
        uint32_t version = 0;

        const std::string setDefaultPath()
        {
            vimcube::utility::OS os = vimcube::utility::getOperatingSystem();
            const char* home = getenv("HOME");
            if (home == nullptr)
            {
                return "Path Error: cannot find home directory";
            }
            std::string realPath = std::string(home);
            return realPath + "/Desktop";
        }

    public:
        const std::string& getFileName() const;
        const std::string& getDefaultPath() const;
        const std::string getFilePath() const;
        uint32_t getVersion() const;

        SaveResult replaceDefaultPath(const std::string& path);

        SaveResult writeFile(const std::string& defaultPath, const std::string& fileName, const vimcube::geometry::Scene& scene);
        LoadResult loadFile(const std::string& defaultPath, const std::string& fileName);

        void reset();
    };
}
#endif
