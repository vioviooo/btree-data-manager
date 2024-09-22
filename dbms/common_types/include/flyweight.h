#ifndef OS_CW_FLYWEIGHT_H
#define OS_CW_FLYWEIGHT_H


#include <map>
#include <memory>
#include <string>
#include <mutex>
#include <iostream>

class flyweight_string {
private:
    std::string data;

public:
    flyweight_string(const std::string& str) : data(str) {}

    std::string& get_data() {
        return data;
    }
};

class flyweight_factory {
private:
    std::map<std::string, std::shared_ptr<flyweight_string>> flyweight_pool;

    inline static std::shared_ptr<flyweight_factory> instance;
    inline static std::mutex mtx;

    flyweight_factory() {}

public:
    flyweight_factory(const flyweight_factory&) = delete;
    flyweight_factory& operator=(const flyweight_factory&) = delete;

    static std::shared_ptr<flyweight_factory> get_instance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = std::shared_ptr<flyweight_factory>(new flyweight_factory());
        }
        return instance;
    }

    std::shared_ptr<flyweight_string> get_flyweight_instance(const std::string& str) {
        auto it = flyweight_pool.find(str);
        if (it != flyweight_pool.end()) {
            return it->second;
        }

        std::shared_ptr<flyweight_string> flyweight = std::make_shared<flyweight_string>(str);
        flyweight_pool[str] = flyweight;
        return flyweight;
    }

    // * дебах
    size_t get_flyweight_pool_size() {
        return flyweight_pool.size();
    }

    void output_flyweight_pool() {
        for (auto it = flyweight_pool.begin(); it != flyweight_pool.end(); it++) {
            std::cout << it->first <<  ' ' << it->second.use_count() << std::endl;
        }
    }

    // * ремувы

    void remove_unused_flyweights() {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto it = flyweight_pool.begin(); it != flyweight_pool.end(); ) {
            if (it->second.use_count() == 1) {
                it = flyweight_pool.erase(it);
            } else {
                ++it;
            }
        }
    }

    void erase_if_unused(const std::string& str, bool flag) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = flyweight_pool.find(str);
        if (flag && it != flyweight_pool.end() && it->second.use_count() == 2) {
            flyweight_pool.erase(it);
        } else if (!flag && it != flyweight_pool.end() && it->second.use_count() == 3) {
            flyweight_pool.erase(it);
        }
    }
};


#endif //OS_CW_FLYWEIGHT_H
