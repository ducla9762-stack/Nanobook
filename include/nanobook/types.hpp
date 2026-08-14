#pragma once

#include <cstddef>
#include <cstdint>

// Scalar types, units and hard limits shared by every engine implementation.
// Spec: matching-engine-spec-v0.md sections 2 and 3.
//
// No floating point appears anywhere below this line, in either engine, or in
// any test. Prices are integer ticks; currency formatting is a gateway concern.

namespace nanobook {

using Price = std::int32_t;     // integer half-pennies ($0.005 per tick)
using Qty = std::uint32_t;      // whole units
using UserRef = std::uint32_t;  // client-assigned, strictly increasing per session
using OrderRef = std::uint64_t; // engine-assigned, reissued on every priority change
using ChainId = std::uint64_t;  // stable across replaces
using MatchId = std::uint64_t;  // shared by the two sides of one fill
using Ts = std::uint64_t;       // nanoseconds since midnight, supplied by the sequencer
using SeqNo = std::uint64_t;    // gap-free, strictly increasing, supplied by the sequencer

// Section 3.1. The window is [PRICE_MIN, PRICE_MAX] inclusive; anything outside is
// rejected rather than handled by a slow path, so v0 has no untested overflow branch.
inline constexpr Price PRICE_BASE = 200;              // $1.00 in half-pennies
inline constexpr std::size_t PRICE_LEVELS = 1u << 18; // 64^3, per side
inline constexpr Price PRICE_MIN = PRICE_BASE;
inline constexpr Price PRICE_MAX = PRICE_BASE + static_cast<Price>(PRICE_LEVELS) - 1;

// Section 2. Quantities are strictly below one million, on entry and per chain.
inline constexpr Qty QTY_LIMIT = 1'000'000;

// Section 3.2. The reference engine has no slab, but it must reject at the same
// point the fast engine runs out of slots, or the two disagree under load.
inline constexpr std::size_t MAX_LIVE_ORDERS = 1u << 20;

enum class Side : std::uint8_t { Buy, Sell };
enum class Tif : std::uint8_t { Day, Ioc };

// Carried on ACCEPTED and REPLACED. 'D' means accepted and immediately killed:
// no further events will ever reference that user_ref.
enum class OrderState : char { Live = 'L', Dead = 'D' };

constexpr Side opposite(Side s) noexcept {
    return s == Side::Buy ? Side::Sell : Side::Buy;
}

constexpr bool price_in_window(Price p) noexcept {
    return p >= PRICE_MIN && p <= PRICE_MAX;
}

// Section 9.1: zero and one-million-or-more are both rejected on ENTER.
constexpr bool qty_valid_for_enter(Qty q) noexcept {
    return q > 0 && q < QTY_LIMIT;
}

// Section 8.2. The trade price is always the resting order's, so marketability is
// asked in terms of the aggressor's limit against a resting price.
constexpr bool marketable(Side aggressor, Price limit, Price resting) noexcept {
    return aggressor == Side::Buy ? limit >= resting : limit <= resting;
}

} // namespace nanobook
