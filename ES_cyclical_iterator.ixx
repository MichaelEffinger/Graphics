module;
#if WE_LACK_IMPORT_STD
#include <type_traits>
#include <iterator>
#include <stdexcept>
#endif
export module ES_cyclical_iterator;

#if WE_HAVE_IMPORT_STD
import std;
#endif

import ES_easy;

export namespace ES {

    template <typename PosIter, typename BeginIter, typename EndIter>
    class cyclical_iterator;

    template<std::ranges::range R>
    cyclical_iterator(R&& range) -> cyclical_iterator<std::invoke_result_t<decltype(std::ranges::begin), R>, std::invoke_result_t<decltype(std::ranges::begin), R>, std::invoke_result_t<decltype(std::ranges::end), R>>;

    template<std::ranges::range R, typename Iter>
    cyclical_iterator(Iter&&, R&& range) -> cyclical_iterator<Iter, std::invoke_result_t<decltype(std::ranges::begin), R>, std::invoke_result_t<decltype(std::ranges::end), R>>;

}




template <typename PosIter, typename BeginIter, typename EndIter>
class ES::cyclical_iterator{
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
    [[no_unique_address]] std::conditional_t<HAS_DISTANCE, difference_type, easy::empty_t> distance_from_begin_{};

    [[nodiscard]] difference_type secret_modulo_helper(difference_type N) const requires(HAS_DISTANCE){
        difference_type const size = std::distance(begin_, end_);
        difference_type const fin = std::distance(pos_, end_);
        N = (N % size + size) % size;
        if (N < fin) return N;
        return N - size;
    }

    [[nodiscard]] static bool these_are_dissonant(cyclical_iterator const& lhs, cyclical_iterator const& rhs){
        return not (lhs.begin_ == rhs.begin_ && lhs.end_ == rhs.end_);
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

    [[nodiscard]] friend bool operator==(const cyclical_iterator& lhs, const cyclical_iterator& rhs) noexcept {
        if (these_are_dissonant(lhs, rhs)) return false;
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
        if (these_are_dissonant(lhs, rhs)) throw std::domain_error("ES::easy::cyclical_iterator::operator-(), begin_ and end_ of both iterators must agree.");
        return lhs.distance_from_begin_ - rhs.distance_from_begin_;
    }


    [[nodiscard]] friend auto operator<=>(const cyclical_iterator & lhs, const cyclical_iterator & rhs) requires (HAS_DISTANCE) {
        if (these_are_dissonant(lhs, rhs)) throw std::domain_error("ES::easy::cyclical_iterator::operator<=>(), begin_ and end_ of both iterators must agree.");
        return lhs.distance_from_begin_ <=> rhs.distance_from_begin_;
    }



};