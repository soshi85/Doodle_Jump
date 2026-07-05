#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>

template <typename T>
class ResourceManager{
private:
    std::unordered_map<std::string, std::unique_ptr<T>> resources;

public:
    ResourceManager() = default;

    T &get(const std::string &filename){
        auto it = resources.find(filename);
        if (it != resources.end()){
            return *(it->second);
        }

        auto resource = std::make_unique<T>();

        if (!resource->loadFromFile(filename)){
            throw std::runtime_error("Failed to load resource: " + filename);
        }

        T &ref = *resource;
        resources[filename] = std::move(resource);

        return ref;
    }
};