module;
#if WE_LACK_IMPORT_STD
#include <random>
#include <numeric>
#include <ranges>
#include <iostream>
#include <stacktrace>
#include <chrono>
#include <string>
#include <filesystem>


#ifdef _WIN32
#include <windows.h>
#endif

#endif

export module ES_easy;

#if WE_HAVE_IMPORT_STD
import std;
#endif


#define NDCR [[nodiscard]] constexpr
#define NDAO [[nodiscard]] auto
#define NDCRAO [[nodiscard]] constexpr auto


/**
 *@namespace ES::easy Your one-stop-shop for hard things that require thought turned one line!
 */
export namespace ES::easy {

//----------------- Easy random! --------------------
    template<typename L, typename  R>
    [[nodiscard]] std::common_type_t<L,R> random(L low, R high);

    /**
     * Generates a value from [0, High] if integral, or [0, High) if floating point. (Blame the committee for that one.)
     * @param High the upper bound.
     * @return A value from [0, High] if integral, or [0, High) if floating point.
     */
    template<typename L>
    [[nodiscard]] L random(L High);

    template<std::floating_point F, std::size_t Bits>
    [[nodiscard]] F random();

    template<typename  L, typename R>
    [[nodiscard]] auto random_seeded_callable(std::uint32_t, L, R);

    template<typename  L>
    [[nodiscard]] auto random_seeded_callable(std::uint32_t, L);

    template<std::floating_point F, std::size_t Bits>
    [[nodiscard]] auto random_seeded_callable(std::uint32_t);

    template<typename T>
    [[nodiscard]] bool coin_flip(T odds_of_heads = 0.5);

//------------------ Easy ranges! ------------------------
    template<std::ranges::range R>
    NDCRAO sum_range(R &&);

    template<std::ranges::range R>
    void shuffle(R&&);

    /**
     * Step right up! Get yourself an element from this range at random!
     * @tparam R a range, specifically an object with begin() and end().
     * @return a random object in the range
     */
    template<std::ranges::range R>
    [[nodiscard]] decltype(auto) raffle(R&);

    /**
     * Finds the min and max element of a non-empty range and shoves them into a pair.
     * @return A pair of (min, max)
     * @throws std::out_of_range if the array is empty.
     */
    template<std::ranges::range R>
    NDCRAO min_max(R &r);

    template <typename PosIter, typename BeginIter, typename EndIter>
    class cyclical_iterator;

//------------------ DEBUG ----------------
    void snap_stacktrace(std::ostream &where_to_print = std::cerr, const std::stacktrace& trace = std::stacktrace::current());

    bool enforce_stacktrace(const bool cond, std::string_view const msg) noexcept;

    /**
     * Easy timer that takes a function and returns how many seconds it took.
     * @tparam func The callable to be tested.
     * @tparam Args The (optional) args to send to the callable.
     * @return The time, in seconds, it took for this function to complete.
     */
    template<std::invocable func, typename... Args>
    [[nodiscard]] double time_it(func&&, Args&&...);

//----------------- FORMATTING CHARACTERS ---------------
    template<std::ranges::range R>
    [[nodiscard]] auto capitalize_range(R&&, std::locale const &rules = std::locale());

    template<std::ranges::range R>
    R& capitalize_range_in_place(R &, std::locale const &rules = std::locale());

    template<std::ranges::range R>
    [[nodiscard]] auto lowercase_range(R&&, std::locale const &rules = std::locale());

    template<std::ranges::range R>
    R& lowercase_range_in_place(R &, std::locale const &rules = std::locale());

    template<std::ranges::range R>
    [[nodiscard]] constexpr auto trim_whitespace(R&&);

    template<std::ranges::range R>
    constexpr R& trim_whitespace_in_place(R&);




    // --------------- miscellaneous -----------------

    //An STL-conformant hash functor for when you do not want std::string reallocating every hash.
    struct string_hash;

    struct empty_t{};

    /**
     * Since C++ staggeringly lacks a way to find where the executable is...
     * @return The path of where the program lives.
     */
    std::filesystem::path where_am_I();


}

//----------------------- impl? detail? Secret! ---------------------
namespace ES::easy::Secret {
    using quick_engine = std::minstd_rand;
    std::minstd_rand &get_quick_engine() {
        thread_local quick_engine eng{std::random_device()()};
        return eng;
    }

    template<typename L, typename R>
    using uniform_dist = std::conditional_t<std::is_integral_v<std::common_type_t<L, R>>, std::uniform_int_distribution<std::common_type_t<L, R>>, std::uniform_real_distribution<std::common_type_t<L, R>>>;

    [[maybe_unused]] [[deprecated("Use the std::locale equivalent.")]] constexpr auto to_upper = [](const unsigned char c){return static_cast<char>(std::toupper(c));};
    [[maybe_unused]] [[deprecated("Use the std::locale equivalent.")]] constexpr auto to_lower = [](const unsigned char c){return static_cast<char>(std::tolower(c));};
    [[maybe_unused]] [[deprecated("Use the std::locale equivalent.")]] constexpr auto is_whitespace = [](const unsigned char c){return static_cast<bool>(std::isspace(c));};
}

//--------------------DEFINITIONS------------------------

template<typename L, typename R>
std::common_type_t<L, R> ES::easy::random(const L low, const R high) {
    auto dist = low <= high ? Secret::uniform_dist<L,R>(low, high) : Secret::uniform_dist<L,R>(high, low);
    return dist(Secret::get_quick_engine());
}

template<typename L>
L ES::easy::random(const L High) {
    return random(L{0}, High);
}

template<std::floating_point F = float, std::size_t Bits = std::numeric_limits<F>::digits>
F ES::easy::random() {
    return std::generate_canonical<F, Bits>(Secret::get_quick_engine());
}

template<typename L, typename R>
auto ES::easy::random_seeded_callable(std::uint32_t seed, L min, R max) {
    return [
                eng = Secret::quick_engine(seed),
                dist = Secret::uniform_dist<L,R> (min, max)
        ] mutable {
        return dist(eng);
    };
}

template<typename L>
auto ES::easy::random_seeded_callable(std::uint32_t seed, L max) {
    return random_seeded_callable(seed, L{0}, max);
}

template<std::floating_point F = float, std::size_t Bits = std::numeric_limits<F>::digits>
auto ES::easy::random_seeded_callable(std::uint32_t seed) {
    return [
                eng = Secret::quick_engine(seed)
        ] mutable {
        return std::generate_canonical<F, Bits>(eng);
    };
}

template<typename T>
bool ES::easy::coin_flip(T const odds_of_heads) {
    std::bernoulli_distribution dist(odds_of_heads);
    return dist(Secret::get_quick_engine());
}

template<std::ranges::range R>
constexpr auto ES::easy::sum_range(R &&arr) {
    return std::reduce(std::ranges::begin(arr), std::ranges::end(arr));
}

template<std::ranges::range R>
constexpr auto ES::easy::min_max(R &r) {
    if (std::ranges::empty(r)) throw std::out_of_range("ES::easy::min_max(), range must not be empty!");
    const auto retval = std::ranges::minmax_element(r);
    return std::pair{*retval.min, *retval.max};
}

template <typename PosIter, typename BeginIter, typename EndIter>
class ES::easy::cyclical_iterator{
public:
    using iterator_category =
        std::conditional_t<
            std::contiguous_iterator<PosIter>,
            std::contiguous_iterator_tag,
            std::conditional_t<
                std::random_access_iterator<PosIter>,
                std::random_access_iterator_tag,
                std::conditional_t<
                    std::bidirectional_iterator<PosIter>,
                    std::bidirectional_iterator_tag,
                    std::forward_iterator_tag>>>;
    using value_type        = PosIter::value_type;
    using difference_type   = std::ptrdiff_t;
    using pointer           = PosIter::pointer;
    using reference          = PosIter::reference;

private:
    PosIter pos_;
    BeginIter begin_;
    EndIter end_;
    static constexpr bool HAS_DISTANCE{std::random_access_iterator<PosIter>};
    [[no_unique_address]] std::conditional_t<HAS_DISTANCE, difference_type, empty_t> distance_from_begin_{};

    [[nodiscard]] difference_type secret_modulo_helper(difference_type N) const requires(HAS_DISTANCE){
        difference_type const size = std::distance(begin_, end_);
        difference_type const fin = std::distance(pos_, end_);
        N = (N % size + size) % size;
        if (N < fin) return N;
        return N - size;
    }

    [[nodiscard]] static bool secret_dissonance_test(cyclical_iterator const& lhs, cyclical_iterator const& rhs){
        return lhs.begin_ == rhs.begin_ && lhs.end_ == rhs.end_;
    }

public:

    cyclical_iterator() = default;

    explicit cyclical_iterator(PosIter Start, BeginIter Begin, EndIter End) :
    pos_(Start), begin_(Begin), end_(End)
    {
        if constexpr (HAS_DISTANCE) {
            distance_from_begin_ = std::distance(begin_, pos_);
            if (not (begin_ <= pos_ && pos_ <= end_)) throw std::invalid_argument("ES::easy::cyclical_iterator(), iterators given do not represent a valid range!");
        }
    }

    explicit cyclical_iterator(BeginIter Begin, EndIter End) : cyclical_iterator(Begin, Begin, End) {}


    template<std::ranges::range R>
    explicit cyclical_iterator(PosIter Start, R&& range) :
    cyclical_iterator(Start, std::ranges::begin(range), std::ranges::end(range))
    {}



    template<std::ranges::range R>
    explicit cyclical_iterator(R&& range) :
    cyclical_iterator(std::ranges::begin(range), range)
    {}


    reference operator*() const { return *pos_; }
    [[nodiscard]] pointer operator->() const { return std::addressof(*pos_); }


    cyclical_iterator& operator++() {if constexpr (HAS_DISTANCE) ++distance_from_begin_; ++pos_; if (pos_ == end_) pos_ = begin_; return *this; }
    cyclical_iterator  operator++(int) { auto tmp = *this; ++*this; return tmp; }

    friend [[nodiscard]] bool operator==(const cyclical_iterator& lhs, const cyclical_iterator& rhs) noexcept {
        if (not secret_dissonance_test(lhs, rhs)) return false;
        if constexpr (HAS_DISTANCE)
            return lhs.distance_from_begin_ == rhs.distance_from_begin_;
        else
            return lhs.pos_ == rhs.pos_;
    }

    //------------------   BIDIRECTIONAL FUNCTIONS ---------------------

    cyclical_iterator& operator--() requires std::bidirectional_iterator<PosIter> {
        if constexpr (HAS_DISTANCE) --distance_from_begin_;
        if (pos_ == begin_) pos_ = end_; --pos_; return *this;
    }
    cyclical_iterator operator--(int) requires std::bidirectional_iterator<PosIter> {
        auto tmp = *this; --*this; return tmp;
    }

    //----------------------- RANDOM ACCESS FUNCTIONS ------------------
    [[nodiscard]] reference operator[](difference_type const N) const requires std::random_access_iterator<PosIter> {
        return pos_[secret_modulo_helper(N)];
    }

    cyclical_iterator& operator+=(difference_type const rhs) requires std::random_access_iterator<PosIter> {
        if constexpr (HAS_DISTANCE) distance_from_begin_ += rhs;
        pos_ += secret_modulo_helper(rhs);
        return *this;
    }

    cyclical_iterator& operator-=(difference_type const rhs) requires std::random_access_iterator<PosIter> {
        if constexpr (HAS_DISTANCE) distance_from_begin_ -= rhs;
        pos_ -= secret_modulo_helper(rhs);
        return *this;
    }

    [[nodiscard]] friend cyclical_iterator operator+(cyclical_iterator const& lhs, difference_type const rhs) requires std::random_access_iterator<PosIter> {
        auto tmp{lhs};
        return tmp += rhs;
    }

    [[nodiscard]] friend cyclical_iterator operator+(difference_type const rhs, cyclical_iterator const& lhs) requires std::random_access_iterator<PosIter> {
        return operator+(lhs, rhs);
    }

    [[nodiscard]] friend cyclical_iterator operator-(cyclical_iterator const& lhs, difference_type const rhs) requires std::random_access_iterator<PosIter> {
        auto tmp{lhs};
        return tmp -= rhs;
    }

    [[nodiscard]] friend difference_type operator-(cyclical_iterator const& lhs, cyclical_iterator const& rhs) requires std::random_access_iterator<PosIter> {
        return lhs.distance_from_begin_ - rhs.distance_from_begin_;
    }


    friend auto operator<=>(const cyclical_iterator & lhs, const cyclical_iterator & rhs) requires (HAS_DISTANCE) {
        if (not secret_dissonance_test(lhs, rhs)) throw std::domain_error("ES::easy::cyclical_iterator::operator<=>(), begin_ and end_ of both iterators must agree.");
        return lhs.distance_from_begin_ <=> rhs.distance_from_begin_;
    }



};
namespace ES::easy{
    template<std::ranges::range R>
    cyclical_iterator(R&& range) -> cyclical_iterator<std::invoke_result_t<decltype(std::ranges::begin), R>, std::invoke_result_t<decltype(std::ranges::begin), R>, std::invoke_result_t<decltype(std::ranges::end), R>>;

    template<std::ranges::range R, typename Iter>
    cyclical_iterator(Iter&&, R&& range) -> cyclical_iterator<Iter, std::invoke_result_t<decltype(std::ranges::begin), R>, std::invoke_result_t<decltype(std::ranges::end), R>>;


}


void ES::easy::snap_stacktrace(std::ostream &where_to_print, const std::stacktrace& trace) {
    where_to_print << std::to_string(trace) << std::endl;
}

bool ES::easy::enforce_stacktrace(const bool cond, std::string_view const msg) noexcept {
    if (cond) return cond;
    std::cerr <<
        "Owie! An ES::easy::enforce_stacktrace() was tripped!\n"
        "msg: " << msg << std::endl;
    snap_stacktrace(std::cerr, std::stacktrace::current(1));
    std::abort();
}

template<std::ranges::range R>
void ES::easy::shuffle(R && arr) {
    std::shuffle(std::ranges::begin(arr), std::ranges::end(arr), Secret::get_quick_engine());
}

template<std::ranges::range R>
decltype(auto) ES::easy::raffle(R &r) {
    if constexpr (std::ranges::random_access_range<R>){
        if (std::ranges::empty(r)) throw std::out_of_range("ES::easy::raffle(), we can't host a raffle with an empty range!");
        const auto size = std::ranges::distance(r);
        return r[ES::easy::random(size - decltype(size){1})];
    } else {
        std::ranges::range_value_t<R> chosen{};
        std::size_t count = 0;
        for (auto&& element : r) {
            ++count;
            if (ES::easy::random(count - 1uz) == 0) chosen = std::forward<decltype(element)>(element);
        }
        return chosen;
    }
}

template<std::invocable func, typename... Args>
double ES::easy::time_it(func&& f, Args&&... args) {
    auto begin = std::chrono::steady_clock::now();
    (void) f(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>{end-begin}.count();
}

template<std::ranges::range R>
auto ES::easy::capitalize_range(R &&r, std::locale const &rules) {
    using objtype = std::ranges::range_value_t<R>;
    std::remove_cvref_t<R> retval(std::forward<R>(r));
    const auto& capitalization_rule = std::use_facet<std::ctype<objtype>>(rules);
    if (not std::ranges::empty(retval))
        if constexpr (std::ranges::contiguous_range<R>) {
            objtype* const start_pointer = std::addressof(*std::ranges::begin(retval));
            auto const len = std::ranges::size(retval);
            capitalization_rule.toupper(start_pointer, start_pointer + len);
        } else {
            const auto internal_to_upper = [&](objtype const c){return capitalization_rule.toupper(c);};
            std::ranges::transform(retval, std::begin(retval), internal_to_upper);
        }
    return retval;
}

template<std::ranges::range R>
R& ES::easy::capitalize_range_in_place(R &r, std::locale const &rules) {
    r = capitalize_range(std::move(r), rules);
    return r;
}

template<std::ranges::range R>
auto ES::easy::lowercase_range(R &&r, std::locale const &rules)  {
    using objtype = std::ranges::range_value_t<R>;
    std::remove_cvref_t<R> retval(std::forward<R>(r));
    const auto& capitalization_rule = std::use_facet<std::ctype<objtype>>(rules);
    if (not std::ranges::empty(retval))
        if constexpr (std::ranges::contiguous_range<R>) {
            objtype* const start_pointer = std::addressof(*std::ranges::begin(retval));
            auto const len = std::ranges::size(retval);
            capitalization_rule.tolower(start_pointer, start_pointer + len);
        } else {
            const auto internal_to_lower = [&](objtype const c){return capitalization_rule.tolower(c);};
            std::ranges::transform(retval, std::begin(retval), internal_to_lower);
        }
    return retval;
}

template<std::ranges::range R>
R & ES::easy::lowercase_range_in_place(R &r, std::locale const &rules) {
    return r = lowercase_range(std::move(r), rules);
}

template<std::ranges::range R>
constexpr auto ES::easy::trim_whitespace(R &&r) {
    std::remove_cvref_t<R> retval(std::forward<R>(r));
    auto front_end = std::ranges::find_if_not(retval, Secret::is_whitespace);
    retval.erase(std::ranges::begin(retval), front_end);
    auto back_begin = std::ranges::find_last_if_not(retval, Secret::is_whitespace);
    if (not back_begin.empty()) retval.erase(std::ranges::next(back_begin.begin()), std::ranges::end(retval));
    return retval;
}

template<std::ranges::range R>
constexpr R & ES::easy::trim_whitespace_in_place(R & r) {
    return r = ES::easy::trim_whitespace(std::move(r));
}

struct ES::easy::string_hash {
    using is_transparent = void;

    std::size_t operator()(char const* txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    std::size_t operator()(std::string_view txt) const {
        return std::hash<std::string_view>{}(txt);
    }
    std::size_t operator()(std::string const& txt) const {
        return std::hash<std::string_view>{}(txt);
    }
};


std::filesystem::path ES::easy::where_am_I() {
#ifdef _WIN32
    wchar_t hideous_windows_buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, hideous_windows_buffer, MAX_PATH);
    return std::filesystem::path(hideous_windows_buffer).parent_path();
#else
    return std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
}
