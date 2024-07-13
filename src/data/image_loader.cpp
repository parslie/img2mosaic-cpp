#include "image.hpp"

#include <cstdint>

namespace fs = std::filesystem;

static constexpr size_t CACHE_SIZE = 10;

Image ImageLoader::load(const fs::path &path)
{
    // TODO: check if this actually speeds things up generally
    if (m_cache.contains(path))
    {
        m_cache_hits += 1;
        ImageLoaderItem &item{ m_cache.at(path) };
        item.times_accessed += 1;
        return item.img;
    }

    m_cache_misses += 1;
    ImageLoaderItem item{ Image(path), 1 };

    if (m_cache.size() >= CACHE_SIZE)
    {
        fs::path least_used_path;
        size_t least_used_times_accessed{ SIZE_MAX };
        for (auto &[cached_path, cached_item] : m_cache)
        {
            if (cached_item.times_accessed < least_used_times_accessed)
            {
                least_used_times_accessed = cached_item.times_accessed;
                least_used_path = cached_path;
            }
        }
        m_cache.erase(least_used_path);
    }
    m_cache[path] = item;

    return item.img;
}

std::string ImageLoader::to_string() const
{
    std::stringstream ss{};
    ss << "ImageLoader(hits: " << m_cache_hits << ", misses: " << m_cache_misses << ')';
    return ss.str();
}
