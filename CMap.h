#ifndef DNSCACHE_CMAP_H
#define DNSCACHE_CMAP_H

#include <memory>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <list>
#include <stdexcept>

enum class CMapAddStatus{
    ADD,
    UPDATE,
    REPLACE_OLD
};

/**
 * Custom map.
 *
 * Custom associative (key-value pair) constant-length unordered array based on a hash table (uses std::unordered_map).
 *
 *  When a container is created, @capacity is allocated memory.
 *
 *  When pasted into a fully populated container, the oldest element is updated.
 *
 *  When inserting an element with a key that already exists, the value will be updated.
 *
 *  Insertion on average O(1), at worst O(n).
 *  Update average O(1), at worst O(n).
 *  Deletion not supported.
 *
 *  A capacity of 0 is invalid.
 *
 *  K is the key type, V is the value type.
 *
 * */
template <class K, class V>
class CMap final {

private:
    struct PairValue;
    using PairValuePtr = std::shared_ptr<PairValue>;

    std::unordered_map<K, PairValuePtr> map;
    using MapIterator = typename std::unordered_map<K, PairValuePtr>::iterator;

    std::list<MapIterator> list;
    using ListIterator = typename std::list<MapIterator>::iterator;

    struct PairValue {
        V value;
        ListIterator listIterator;
    };

    std::size_t capacity;

    void oldestItemRemove() {
        // Looking for the oldest item
        const auto& oldestItemIterator = list.front();

        // Erase it
        map.erase(oldestItemIterator);
        list.pop_front();
    }

public:

    /**
     * Allocates memory for a given number of elements.
     * @param capacity maximum number of items in a container.
     *
     */
    explicit CMap(const std::size_t capacity):
        capacity(capacity) {

        if (capacity == 0)
            throw std::length_error("Capacity cannot be 0");

        if (capacity+1 > map.max_size())
            throw std::length_error("Capacity is too large");

        if (capacity+1 > map.bucket_count()) {
            map.max_load_factor(1);

            // Pre-allocate memory
            map.reserve(capacity+1);
        }
    }

    /**
     * Adds a new element to the container. If the container is full, removes the oldest oldest element and then adds.
     *
     * @param key associative array key.
     * @param value associative array value.
     * @return CMapAddStatus. if add, then the element was added. If oldestItemRemove, then the element has been updated.
     */
    CMapAddStatus add(const K& key, const V& value) {
        CMapAddStatus ret = CMapAddStatus::ADD;

        // Try to add new item
        auto newPairValue = std::make_shared<PairValue>();
        newPairValue->value = value;

        const auto& insertedMapItem = map.emplace(key, newPairValue);

        // The update makes the record the youngest
        if (!insertedMapItem.second) {
            list.erase(insertedMapItem.first->second->listIterator);
            // Update
            insertedMapItem.first->second = newPairValue;
            ret = CMapAddStatus::UPDATE;
        }

        // If the array is full
        if (map.size() > capacity) {
            oldestItemRemove();
            ret = CMapAddStatus::REPLACE_OLD;
        }

        // Add iterator to list
        const auto& insertedListItem = list.insert(list.end(), insertedMapItem.first);
        insertedMapItem.first->second->listIterator = insertedListItem;
        return ret;
    }

    /**
     * Returns value by key.
     *
     * @return value.
     * */
    std::string get(const K& k) const {
        const auto& result = map.find(k);
        if (result == map.end())
            return {};
        return result->second->value;
    }

    /**
     * Returns container capacity.
     *
     * @return container capacity.
     * */
    std::size_t getCapacity() const {
        return capacity;
    }

    /**
     * Returns container size.
     *
     * @return container size.
     * */
    std::size_t getSize() const {
        return map.size();
    }

    /**
     * Remove all items from container.
     *
     * */
     void clear() {
        map.clear();
        list.clear();
     }

    virtual ~CMap() = default;

};


#endif //DNSCACHE_CMAP_H
