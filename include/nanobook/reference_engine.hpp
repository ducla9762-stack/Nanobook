#pragma once

#include <variant >
#include "messages.hpp"
#include "events.hpp"
#include "types.hpp"

namespace nanobook {

// One link of a chain, resting on the book.
//
// Private to this engine. The fast engine will pack this into a slab slot with a
// different layout; only messages.hpp and events.hpp are shared between them.
struct Order {
    UserRef user_ref{};
    OrderRef order_ref{};
    ChainId chain_id{};
    Qty qty{};
    Qty executed_qty{};
    Side side{};
    Price price{};
    Tif tif{}; 
};

} // namespace nanobook