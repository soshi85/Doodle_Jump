#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <filesystem>

template <typename T>
class ResourceManager {
private:
    std::unordered_map<std::string, std::unique_ptr<T>> resources;


    template <typename U>
    auto loadResource(U& res, const std::filesystem::path& path, int) -> decltype(res.loadFromFile(path), bool()) {
        return res.loadFromFile(path);
    }


    template <typename U>
    auto loadResource(U& res, const std::filesystem::path& path, long) -> decltype(res.openFromFile(path), bool()) {
        return res.openFromFile(path);
    }

public:
    ResourceManager() = default;

    T& get(const std::string& filename) {
        auto it = resources.find(filename);
        if (it != resources.end()) {
            return *(it->second);
        }

        auto resource = std::make_unique<T>();
        
        
        if (!loadResource(*resource, std::filesystem::path(filename), 0)) {
            throw std::runtime_error("Failed to load resource: " + filename);
        }

        T& ref = *resource;
        resources[filename] = std::move(resource);
        return ref;
    }
};