#include <algorithm>
#include <deque>
#include <numeric>
#include <array>
#include <unordered_map>
#include <forward_list>
#include <ranges>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include "ES_test_util.hpp"

import ES_easy;
import ES_cyclical_iterator;



TEST_CASE("ES capitalizing and lowercasing ranges") {

    SECTION("Capitalize_range") {
        std::string str = "aaa", st2 = ES::easy::capitalize_range(str);
        CHECK("AAA" == st2);

        CHECK(ES::easy::capitalize_range_in_place(str) == st2);
    }

    SECTION("Lowercase range!"){
        std::string stemp{"Long string that would have to call new."}, st4{stemp}, st3{ES::easy::lowercase_range(std::move(stemp))};
        ES::easy::lowercase_range_in_place(st4);
        CHECK(st4 == st3);
    }

    SECTION("Empty!?") {
        std::string empty;
        CHECK(ES::easy::lowercase_range(empty).empty());
        CHECK(ES::easy::capitalize_range(empty).empty());
    }

    SECTION("Not neccessairly a string!?") {
        std::deque<char> deck;
        std::array<char, 26> alpha{};
        std::ranges::iota(alpha, 'A');
        ES::easy::lowercase_range_in_place(alpha);
        CHECK_FALSE(std::ranges::equal(alpha, ES::easy::capitalize_range(alpha)));
        std::ranges::copy(alpha, std::back_inserter(deck));
        CHECK(std::ranges::equal(ES::easy::capitalize_range_in_place(alpha), ES::easy::capitalize_range_in_place(deck)));
    }

}

TEST_CASE("ES::easy::min_max","[ES::easy::min_max][minmax][min_max]") {
    std::vector<std::byte> empty;
    CHECK_THROWS_AS(ES::easy::min_max(empty), std::out_of_range);
}


TEST_CASE("ES::easy::trim_whitespace","[trim_whitespace]") {

    SECTION("Left, right, and middle full of white space."){
        std::string miserable{"\t\t\t\t\t\t\tI'm ugly!!11    \t 111 :((((\n\t"};

        CHECK(ES::easy::trim_whitespace(miserable) == "I'm ugly!!11    \t 111 :((((");
    }

    SECTION("Empty string") {
        std::string empty;
        CHECK(ES::easy::trim_whitespace_in_place(empty).empty());
    }

    SECTION("Left side with whitespace") {
        std::string str{"             \v\t\t                  wow!"};
        CHECK(ES::easy::trim_whitespace(str) == "wow!");
    }

    SECTION("Right side with whitespace") {
        std::string str{"neat!        \t\t\t\v\t\n\n                  \t\n"};
        CHECK(ES::easy::trim_whitespace_in_place(str) == "neat!");
    }

    SECTION("Complex left side with whitespace") {
        std::string str{"    q         \v\t\t          r        wow!"};
        CHECK(ES::easy::trim_whitespace(str) == "q         \v\t\t          r        wow!");
    }

    SECTION("Complex right side with whitespace") {
        std::string str{"neat!    e    \t\t\t\v\t\n\n         w         \t\n"};
        CHECK(ES::easy::trim_whitespace_in_place(str) == "neat!    e    \t\t\t\v\t\n\n         w");
    }

}

TEST_CASE("ES::easy::where_am_I, Where in the world...","[ES::easy::where_am_I][filesystem][diskio]") {
    auto const you_give_love_a_bad_name = ES::easy::where_am_I().string();

    CHECK(you_give_love_a_bad_name.contains("cmake"));

}

TEST_CASE("ES::cyclical_iterator on std::forward_list", "[cyclical_iterator][forward]") {
    using ES::cyclical_iterator;
    std::forward_list<int> data{10, 20, 30, 40};
    auto b = data.begin();
    auto e = data.end();

    SECTION("dereference at construction yields the first element") {
        cyclical_iterator it(b, b, e);
        REQUIRE(*it == 10);
    }

    SECTION("single increments walk the underlying sequence in order") {
        cyclical_iterator it(b, b, e);
        REQUIRE(*it == 10);
        ++it;
        REQUIRE(*it == 20);
        ++it;
        REQUIRE(*it == 30);
        ++it;
        REQUIRE(*it == 40);
    }

    SECTION("incrementing past the last element wraps to the first") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 4); // exactly one full lap
        REQUIRE(*it == 10);
    }

    SECTION("multiple full laps return to the same logical position") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 4 * 3); // three full laps
        REQUIRE(*it == 10);
        ++it; ++it;
        REQUIRE(*it == 30);
    }

    SECTION("post-increment returns the pre-increment value and still advances") {
        cyclical_iterator it(b, b, e);
        auto prev = it++;
        REQUIRE(*prev == 10);
        REQUIRE(*it == 20);
    }

    SECTION("post-increment wraps correctly at the boundary") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 3); // now at 40, the last element
        auto prev = it++;
        REQUIRE(*prev == 40);
        REQUIRE(*it == 10);
    }

    SECTION("two iterators at the same logical position compare equal") {
        cyclical_iterator it1(b, b, e);
        cyclical_iterator it2(b, b, e);
        std::advance(it1, 4); // full lap back to start
        REQUIRE(it1 == it2);
    }

    SECTION("two iterators at different logical positions compare unequal") {
        cyclical_iterator it1(b, b, e);
        cyclical_iterator it2(b, b, e);
        ++it2;
        REQUIRE_FALSE(it1 == it2);
    }

    SECTION("std::advance works across one or more wraparounds") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 4 + 2); // one full lap plus two more steps
        REQUIRE(*it == 30);
    }

    SECTION("default-constructed iterators of the same type compare equal") {
        using It = cyclical_iterator<decltype(b), decltype(b), decltype(e)>;
        It it1{};
        It it2{};
        REQUIRE(it1 == it2);
    }

    SECTION("type satisfies forward_iterator but not bidirectional_iterator") {
        using It = cyclical_iterator<decltype(b), decltype(b), decltype(e)>;
        STATIC_REQUIRE(std::forward_iterator<It>);
        STATIC_REQUIRE_FALSE(std::bidirectional_iterator<It>);
    }
}

TEST_CASE("ES::cyclical_iterator both ways!?", "[cyclical_iterator][bidirectional]") {
    using ES::cyclical_iterator;
    std::list<int> data{10, 20, 30, 40};
    auto b = data.begin();
    auto e = data.end();

    SECTION("single decrement from the first element wraps to the last") {
        cyclical_iterator it(b, b, e);
        --it;
        REQUIRE(*it == 40);
    }

    SECTION("decrementing walks the underlying sequence backward") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 3); // now at 40
        --it;
        REQUIRE(*it == 30);
        --it;
        REQUIRE(*it == 20);
        --it;
        REQUIRE(*it == 10);
    }

    SECTION("decrementing past the first element wraps repeatedly through a full lap") {
        cyclical_iterator it(b, b, e);
        --it; --it; --it; --it; // one full lap backward
        REQUIRE(*it == 10);
    }

    SECTION("multiple full backward laps return to the same logical position") {
        cyclical_iterator it(b, b, e);
        std::advance(it, -(4 * 3)); // three full laps backward
        REQUIRE(*it == 10);
    }

    SECTION("post-decrement returns the pre-decrement value and still moves back") {
        cyclical_iterator it(b, b, e);
        auto prev = it--;
        REQUIRE(*prev == 10);
        REQUIRE(*it == 40);
    }

    SECTION("post-decrement wraps correctly across the boundary") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 1); // now at 20
        --it;                // back to 10
        auto prev = it--;
        REQUIRE(*prev == 10);
        REQUIRE(*it == 40);
    }

    SECTION("n steps forward followed by n steps back return to the start") {
        cyclical_iterator it(b, b, e);
        cyclical_iterator start(b, b, e);
        for (int n = 0; n < 10; ++n) ++it;
        for (int n = 0; n < 10; ++n) --it;
        REQUIRE(it == start);
    }

    SECTION("a full forward lap and a full backward lap are mutual inverses") {
        cyclical_iterator it(b, b, e);
        ++it; ++it; ++it; ++it; // one full forward lap
        REQUIRE(*it == 10);
        --it; --it; --it; --it; // one full backward lap
        REQUIRE(*it == 10);
    }

    SECTION("std::advance accepts negative distances across wraparound") {
        cyclical_iterator it(b, b, e);
        std::advance(it, -1);
        REQUIRE(*it == 40);
        std::advance(it, -5); // one full lap plus one more step back
        REQUIRE(*it == 30);
    }

    SECTION("two iterators reached via different paths to the same element compare equal") {
        cyclical_iterator it1(b, b, e);
        std::advance(it1, 6);  // 6 forward, wraps once, lands on 30
        cyclical_iterator it2(b, b, e);
        std::advance(it2, -2); // 2 back from start, wraps once, lands on 30
        REQUIRE(it1 == it2);
    }

    SECTION("type satisfies bidirectional_iterator") {
        using It = cyclical_iterator<decltype(b), decltype(b), decltype(e)>;
        STATIC_REQUIRE(std::bidirectional_iterator<It>);
    }
}

TEST_CASE("ES::cyclical_iterator evil Random-access tests","[cyclical_iterator][random_access]") {
    SECTION("std::sort interop") {
        std::deque<int> d1(20);
        std::ranges::iota(d1, 0);
        auto d2(d1);
        ES::easy::shuffle(d2);
        std::sort(ES::cyclical_iterator(d2), ES::cyclical_iterator(d2.end(), d2));
        CHECK(d1 == d2);
    }
}

TEST_CASE("cyclical_iterator random access over std::vector", "[cyclical_iterator][random_access]") {
    std::vector<int> data{10, 20, 30, 40};
    auto b = data.begin();
    auto e = data.end();
    using It = ES::cyclical_iterator<decltype(b), decltype(b), decltype(e)>;

    SECTION("type satisfies random_access_iterator when PosIter does") {
        STATIC_REQUIRE(std::random_access_iterator<It>);
    }

    SECTION("operator- reports absolute step distance, not wrapped position distance") {
        It it1(b, b, e);
        It it2(b, b, e);
        std::advance(it2, 6); // one full lap (4) plus 2 more steps
        CHECK(it2 - it1 == 6);
        CHECK(it1 - it2 == -6);
    }

    SECTION("operator- is zero for iterators at the same absolute step, even across construction") {
        It it1(b, b, e);
        It it2(b, b, e);
        std::advance(it1, 4);
        std::advance(it2, 4);
        REQUIRE(it1 - it2 == 0);
    }

    SECTION("operator< orders by absolute step, not by underlying element index") {
        It it1(b, b, e);
        It it2(b, b, e);
        std::advance(it2, 6); // wraps once; dereferences to same element as 2 steps in
        CHECK(it1 < it2);
        REQUIRE_FALSE(it2 < it1);
    }

    SECTION("two iterators that alias to the same element after a lap are NOT equal") {
        // this is the crux of the axiom fix: pos_-equality would say these are
        // the same iterator, but a valid total order requires them to differ.
        It it1(b, b, e);
        It it2(b, b, e);
        std::advance(it1, 2);
        std::advance(it2, 6); // wraps once, dereferences to same element as it1
        CHECK(*it1 == *it2);       // same element
        CHECK_FALSE(it1 == it2);   // but NOT the same iterator
        CHECK(it1 < it2);          // and orderable, consistently
    }

    SECTION("equality and less-than never both hold — totally_ordered axiom spot check") {
        It it1(b, b, e);
        It it2(b, b, e);
        std::advance(it2, 6);
        bool eq  = (it1 == it2);
        bool lt1 = (it1 < it2);
        bool lt2 = (it2 < it1);
        // if eq is true, both lt1 and lt2 must be false
        if (eq) {
            CHECK_FALSE(lt1);
            CHECK_FALSE(lt2);
        }
        // never both strictly less than each other
        //REQUIRE_FALSE(lt1 && lt2);
    }

    SECTION("operator[] reaches forward across a wraparound boundary") {
        It it(b, b, e);
        std::advance(it, 3); // sits on 40, the last element
        REQUIRE(it[0] == 40);
        REQUIRE(it[1] == 10); // wraps
        REQUIRE(it[2] == 20);
    }

    SECTION("operator[] is consistent with repeated operator++") {
        It it(b, b, e);
        It stepped(b, b, e);
        for (int n = 0; n < 9; ++n) ++stepped; // 9 steps, more than two laps
        REQUIRE(it[9] == *stepped);
    }

    SECTION("subtraction and indexing agree: it[n] equals *(it advanced by n)") {
        It it(b, b, e);
        for (int n = 0; n < 12; ++n) {
            It advanced(b, b, e);
            std::advance(advanced, n);
            REQUIRE(it[n] == *advanced);
            REQUIRE((advanced - it) == n);
        }
    }

    SECTION("negative distance via operator- is the mirror of forward distance") {
        It it1(b, b, e);
        It it2(b, b, e);
        std::advance(it1, 10);
        CHECK(it1 - it2 == 10);
        REQUIRE(it2 - it1 == -10);
    }

    SECTION("std::sort-style comparisons don't infinite loop or misbehave on aliased elements") {
        // regression test for the exact bug flagged earlier: sort algorithms trust
        // == and < to agree. Build a small vector of iterators spanning > one lap
        // and confirm std::is_sorted / std::sort behave sanely.
        std::vector<It> its;
        for (int n = 0; n < 10; ++n) {
            It cur(b, b, e);
            std::advance(cur, n);
            its.push_back(cur);
        }
        std::sort(its.begin(), its.end());
        CHECK(std::is_sorted(its.begin(), its.end()));
        // sorted order should exactly match ascending step_, i.e. original order here
        for (std::size_t i = 0; i + 1 < its.size(); ++i) {
            CHECK(its[i] < its[i + 1]);
            CHECK_FALSE(its[i] == its[i + 1]);
        }
    }

    SECTION("std::distance matches operator- for random access category") {
        It it1(b, b, e);
        It it2(b, b, e);
        std::advance(it2, 7);
        REQUIRE(std::distance(it1, it2) == 7);
    }
}

TEST_CASE("cyclical_iterator random access over std::deque, starting mid-range", "[cyclical_iterator][random_access][deque]") {
    std::deque<int> data{100, 200, 300, 400, 500, 600}; // size 6
    auto b = data.begin();
    auto e = data.end();
    using It = ES::cyclical_iterator<decltype(b), decltype(b), decltype(e)>;

    // convenience: an iterator into `data` sitting at logical index `n` from begin
    auto at = [&](int n) { auto p = b; std::advance(p, n); return p; };

    SECTION("type still satisfies random_access_iterator over deque (non-contiguous)") {
        STATIC_REQUIRE(std::random_access_iterator<It>);
    }

    SECTION("(Iter, Range) constructor starting mid-range reports correct absolute step") {
        It mid(at(3), data); // sitting on 400, range is all of data
        CHECK(*mid == 400);

        It begin_it(data); // sole-range ctor: pos == begin
        CHECK(*begin_it == 100);

        // step_ must reflect true offset from begin_, not zero
        CHECK((mid - begin_it) == 3);
        CHECK((begin_it - mid) == -3);
    }

    SECTION("two mid-range starts constructed independently subtract to their true offset") {
        It it3(at(3), data); // 400
        It it5(at(5), data); // 600
        CHECK((it5 - it3) == 2);
        CHECK((it3 - it5) == -2);
    }

    SECTION("mid-range iterator advanced by size() returns to same element, step_ moves by size()") {
        It mid(at(2), data); // 300
        It snapshot = mid;
        std::advance(mid, 6); // one full lap
        CHECK(*mid == 300);
        CHECK((mid - snapshot) == 6);
        CHECK_FALSE(mid == snapshot); // same element, different lap => must NOT be equal
    }

    SECTION("mid-range iterator decremented past begin wraps and step_ goes negative correctly") {
        It mid(at(1), data); // 200
        std::advance(mid, -3); // 200 -> 100 -> wrap -> 600 -> 500
        CHECK(*mid == 500);
        It reference(at(1), data);
        CHECK((mid - reference) == -3);
    }

    SECTION("operator[] from a mid-range start indexes forward relative to its own position") {
        It mid(at(4), data); // 500
        CHECK(mid[0] == 500);
        CHECK(mid[1] == 600);
        CHECK(mid[2] == 100); // wraps
        CHECK(mid[7] == 600); // more than one full lap (6) + 1
    }

    SECTION("ordering between two mid-range iterators matches their true step_ difference, not element index") {
        It it1(at(5), data); // 600, near the "end" of the underlying storage
        It it2(at(1), data); // 200, but constructed independently, no lap difference
        // it1's element index (5) > it2's element index (1), but with equal lap count,
        // ordering should still follow step_, which for these equals element index here
        CHECK(it1 - it2 == 4);
        CHECK(it2 < it1);
        CHECK_FALSE(it1 < it2);

        // now advance it2 two full laps past it1's position — step_ ordering must flip
        std::advance(it2, 12); // 2 full laps, lands back on 200
        CHECK(*it2 == 200);
        CHECK(it1 < it2); // it1's absolute step is still less, despite dereferencing to an "earlier" element
    }

    SECTION("(Range) sole constructor and (Iter,Range) constructor agree when pos == begin") {
        It viaRange(data);
        It viaPos(at(0), data);
        CHECK(viaRange == viaPos);
        CHECK((viaRange - viaPos) == 0);
        CHECK(*viaRange == *viaPos);
    }

    SECTION("three independently-constructed mid-range iterators sort by absolute step, not element value") {
        std::deque<It> its;
        its.push_back(It(at(4), data)); // 500, step 4
        its.push_back(It(at(1), data)); // 200, step 1
        auto laterStart = It(at(2), data);
        std::advance(laterStart, 6); // 300, but one full lap later, step 9
        its.push_back(laterStart);

        std::sort(its.begin(), its.end());
        CHECK(*its[0] == 200);
        CHECK(*its[1] == 500);
        CHECK(*its[2] == 300);
        CHECK(std::is_sorted(its.begin(), its.end()));
    }

    SECTION("equality is false for same element reached via different lap counts, from mid-range starts") {
        It a(at(2), data); // 300, step 2
        It b_it(at(2), data);
        std::advance(b_it, 6); // still 300, but step 8
        CHECK(*a == *b_it);
        CHECK_FALSE(a == b_it);
        CHECK(a < b_it);
        CHECK((b_it - a) == 6);
    }

    SECTION("std::distance agrees with operator- for mid-range starts, forward direction only") {
        It a(at(1), data);
        It c(at(1), data);
        std::advance(c, 4);
        CHECK(std::distance(a, c) == 4);
        CHECK((c - a) == 4);
    }

    SECTION("full round trip: advance forward then backward by same amount returns to identical step_, from a mid-range start") {
        It mid(at(3), data);
        It snapshot = mid;
        std::advance(mid, 10);
        std::advance(mid, -10);
        CHECK(mid == snapshot); // must be true equality here: same step_, same pos_
        CHECK((mid - snapshot) == 0);
    }
}

TEST_CASE("cyclical_iterator comparisons across mismatched or differing ranges","[cyclical_iterator][comparison][random_access]") {
    std::deque<int> data{0, 1, 2, 3, 4, 5, 6, 7}; // size 8
    auto full_b = data.begin();
    auto full_e = data.end();

    auto at = [&](int n) { auto p = full_b; std::advance(p, n); return p; };

    // a genuine SUB-range: [begin+2, begin+6) => logical contents {2,3,4,5}
    auto sub_b = at(2);
    auto sub_e = at(6);

    using It = ES::cyclical_iterator<decltype(full_b), decltype(full_b), decltype(full_e)>;

    SECTION("same pos_, same step_ from own begin_, but different logical ranges: are they really 'equal'?") {
        // full-range iterator sitting at element 2, step_ == 2 (relative to full_b)
        It full_it(at(2), full_b, full_e);
        // sub-range iterator ALSO sitting at element 2, ALSO step_ == 0 relative to sub_b...
        // wait: step_ is relative to each one's own begin_, so these are NOT the same step_ at all.
        It sub_it(sub_b, sub_b, sub_e); // pos_ == element 2, step_ == 0 (relative to sub_b)

        CHECK(*full_it == *sub_it); // both dereference to element value 2 -- same underlying element
        // The real question: does operator== conflate "same element, same pos_" with "same iterator"?
        // If == only compares pos_ (or only step_, coincidentally both could differ here), this line
        // documents whatever your current answer is -- and if it's `true`, that's the bug to fix,
        // because these two iterators do NOT walk the same cycle: full_it wraps over 8 elements,
        // sub_it wraps over 4. They will diverge on the very next ++.
        INFO("full_it and sub_it point at the same element but iterate DIFFERENT ranges");
        CHECK_FALSE(full_it == sub_it); // <-- if this fails, == is not range-aware
    }

    SECTION("same pos_, deliberately matched step_, still different ranges -> next ++ proves they were never comparable") {
        It full_it(at(2), full_b, full_e); // step_ = 2
        It sub_it(sub_b, sub_b, sub_e);     // step_ = 0
        // force step_ parity by advancing sub_it by 2 so both step_ == 2, if your == uses step_
        std::advance(sub_it, 2); // sub range size 4: pos now at element 4 (index 2 within sub-range)

        // now diverge them and see if they behave as "the same iterator" would have to
        auto full_copy = full_it;
        auto sub_copy = sub_it;
        std::advance(full_copy, 6); // full range size 8: wraps partially
        std::advance(sub_copy, 6);  // sub range size 4: wraps one full lap + 2

        CHECK(*full_copy != *sub_copy); // <-- if these were truly "equal" iterators, this should never happen after equal advances
    }

    SECTION("full-range iterator vs a second full-range iterator over the SAME bounds: comparisons must be trustworthy") {
        // sanity control case: identical begin_/end_, this MUST behave correctly,
        // contrasting against the mismatched-range cases above
        It it1(at(3), full_b, full_e);
        It it2(at(3), full_b, full_e);
        CHECK(it1 == it2);
        CHECK_FALSE(it1 < it2);
        CHECK((it1 - it2) == 0);
    }

    SECTION("constructing pos_ outside [begin_, end_) entirely -- garbage bounds, documenting current (mis)behavior") {
        CHECK_THROWS_AS(It (at(0), sub_b, sub_e), std::invalid_argument);
    }

    SECTION("two full-range iterators built from DIFFERENT calls to begin()/end() but identical logical range") {
        // paranoia check: begin()/end() called twice on the same container should yield
        // iterators that compare == to each other even though they're different objects
        It it1(at(5), data.begin(), data.end());
        It it2(at(5), data.begin(), data.end());
        CHECK(it1 == it2);
        CHECK((it1 - it2) == 0);
    }
}

TEST_CASE("cyclical_iterator equality and ordering across disgusting subranges","[cyclical_iterator][random_access][subrange]") {
    std::array<char, 26> alphabet{};
    std::ranges::iota(alphabet, 'A');

    auto ab = alphabet.begin();
    auto ae = alphabet.end();
    auto at = [&](int n) { auto p = ab; std::advance(p, n); return p; };
    using It = ES::cyclical_iterator<decltype(ab), decltype(ab), decltype(ae)>;

    SECTION("two disjoint subranges, coincidentally equal distance_from_begin_, must not be equal") {
        It front(at(0), at(0), at(5));
        It back(at(20), at(20), at(25));
        CHECK(*front == 'A');
        CHECK(*back == 'U');
        CHECK_FALSE(front == back);
        CHECK_FALSE(front == back);
        std::advance(front, 3);
        std::advance(back, 3);
        CHECK(*front == 'D');
        CHECK(*back == 'X');
        CHECK_FALSE(front == back);
    }

    SECTION("partially overlapping subranges sharing an element still differ as iterators") {
        It left(at(2), at(0), at(6));
        It right(at(2), at(2), at(10));
        CHECK(*left == 'C');
        CHECK(*right == 'C');
        CHECK_FALSE(left == right);
        std::advance(left, 14);
        std::advance(right, 14);
        CHECK((*left) != (*right));
    }

    SECTION("nested subrange fully inside a larger range, same starting element") {
        It outer(at(5), at(0), at(20));
        It inner(at(5), at(5), at(9));
        CHECK(*outer == 'F');
        CHECK(*inner == 'F');
        CHECK_FALSE(outer == inner);
        std::advance(outer, 30);
        std::advance(inner, 30);
        CHECK((*outer) != (*inner));
    }

    SECTION("identical subrange bounds constructed independently must compare equal") {
        It s1(at(6), at(6), at(12));
        It s2(at(6), at(6), at(12));
        CHECK(s1 == s2);
        std::advance(s1, 15);
        std::advance(s2, 15);
        CHECK(s1 == s2);
        CHECK(*s1 == *s2);
    }

    SECTION("operator<=> across disjoint ranges with matched distance_from_begin_ should not silently order") {
        It front(at(0), at(0), at(4));
        It back(at(10), at(10), at(14));
        CHECK_THROWS(front < back);
        CHECK_THROWS(back < front);
        CHECK_FALSE(front == back);
    }

    SECTION("full range and its own strict subrange, same current element, diverge after one lap") {
        It full(at(3), ab, ae);
        It sub(at(3), at(3), at(9));
        CHECK(*full == 'D');
        CHECK(*sub == 'D');
        CHECK_FALSE(full == sub);
        std::advance(full, 26);
        std::advance(sub, 6);
        CHECK(*full == 'D');
        CHECK(*sub == 'D');
        CHECK_FALSE(full == sub);
    }

    SECTION("two subranges of equal length, disjoint, advanced through several laps, never alias") {
        It r1(at(0), at(0), at(5));
        It r2(at(15), at(15), at(20));
        for (int n = 0; n < 23; ++n) {
            CHECK_FALSE(r1 == r2);
            ++r1;
            ++r2;
        }
    }

    SECTION("reordering by operator<=> must not mix iterators from different ranges as if comparable") {
        It a(at(0), at(0), at(4));
        It b(at(1), at(0), at(4));
        It c(at(10), at(10), at(14));
        CHECK(a < b);
        CHECK_FALSE(a == c);
        CHECK_FALSE(b == c);
        CHECK_THROWS(a < c);
        CHECK_THROWS(c < a);
    }
}