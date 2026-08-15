#pragma once

#include <list>
#include <map>
#include <unordered_map>

#include "events.hpp"
#include "messages.hpp"
#include "types.hpp"

namespace nanobook {

// One link of a chain, resting on the book.
//
// Private to this engine. The fast engine will pack this into a slab slot with a
// different layout; only messages.hpp and events.hpp are shared between them.
//
// This engine is deliberately naive. Its job is to be the correctness oracle in a
// differential test: obviously right, even where that costs speed.
struct Order {
    UserRef user_ref{};
    OrderRef order_ref{};
    ChainId chain_id{};
    Qty qty{};
    Qty executed_qty{};
    Side side{};
    Price price{};
    Tif tif{};
    Ts priority_ts{};
};


class ReferenceOrderEngine {
public:
    using Level = std::list<Order>; // (section 3.1).
    using Book = std::map<Price, Level>;
    EventList apply(const Message& msg); // Section 10: the engine is the pure function (seq_no, timestamp, message) -> event[]. 

private:
    Book bids_;
    Book asks_;

    // user_ref -> the node that order occupies. The client only ever knows its own
    // user_ref, so this is the only way in. The reference engine's stand-in for
    // section 3.3's direct-indexed ring.
    std::unordered_map<UserRef, Level::iterator> live_;

    // Section 10 rule 8: identifiers come from deterministic counters seeded at
    // replay start, never from a clock or an address. Starting at 1 keeps 0 usable
    // as "none".
    OrderRef next_order_ref_{1};
    ChainId next_chain_id_{1};
    MatchId next_match_id_{1};

    // Section 6.3, as refined by MEMORY.md item 4: a high-water mark, advanced only
    // when an outbound event names a ref, not on mere arrival.
    UserRef last_consumed_user_ref_{0};
};

} // namespace nanobook
