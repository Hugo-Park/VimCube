#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <vector>
#include <string>
#include <cstdint>
#include "../geometry/Scene.h"

namespace vimcube::file
{
    struct GeometryRecord
    {
        uint32_t id = 0;
        std::string sequenceRecord;
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
        std::string filePath;
        uint32_t version = 0;

    public:
        const std::string& getFilePath() const;
        uint32_t getVersion() const;

        SaveResult writeFile(const std::string& path, const vimcube::geometry::Scene& scene);
        LoadResult loadFile();

        void reset();
    };
}
#endif
