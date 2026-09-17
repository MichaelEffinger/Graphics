module;
#if WE_LACK_IMPORT_STD
#include <unordered_map>
#include <string>
#include <memory>
#include <utility>
#endif
export module ES_flyweight;

#if WE_HAVE_IMPORT_STD
import std;
#endif;

import ES_game;
import ES_easy;


export namespace ES{
    template<typename Stored_Asset>
    class flyweight;
}

template<typename Stored_Asset>
class ES::flyweight{
public:
private:
    static inline std::unordered_map<std::string, std::shared_ptr<Stored_Asset>, ES::easy::string_hash, std::equal_to<>> database_{};
    //static inline bool has_been_populated{false};
public:
    flyweight() = delete;

    [[nodiscard]] static std::weak_ptr<Stored_Asset const> request(std::string_view const name);

    template<typename E>
    static std::weak_ptr<Stored_Asset const> store(std::string_view const name , E&& val);

    //horrendous violation of single-responsibility.
    //static void refresh();

};

template<typename Stored_Asset>
std::weak_ptr<Stored_Asset const> ES::flyweight<Stored_Asset>::request(std::string_view const name){
    //if (not has_been_populated) refresh();
    auto const iter = database_.find(name);
    return iter == database_.end() ? nullptr : iter->second;
}

template<typename Stored_Asset>
template<typename E>
std::weak_ptr<Stored_Asset const> ES::flyweight<Stored_Asset>::store(std::string_view const name, E &&val) {
    auto [iter, fruitful] = database_.insert(std::make_pair(name, std::make_shared<std::remove_cvref_t<E>>(std::forward<E>(val))));
    // if (not fruitful) throw std::logic_error("ES::flyweight::store(), unchecked overwrite of an already stored asset!");
    return iter->second;
}









