#pragma once

#include <variant>
#include <vector>

#include "reason_codes.hpp"
#include "types.hpp"

// Section 6.2. The outbound event stream.
//
// This is the comparison unit for differential testing: two engines fed the same
// message sequence must produce equal std::vector<Event>. Every field here is
// therefore load-bearing, and nothing derived from a clock or an address may
// appear in one (section 10).
//
// Quantity conventions (section 5) differ per event and are the single easiest
// thing to get wrong, so each one is spelled out at its field.

namespace nanobook {

struct Accepted {
    UserRef user_ref{};
    OrderRef order_ref{};
    Side side{};
    Price price{};
    Qty qty{};             // absolute: shares accepted, as sent
    Tif tif{};
    OrderState order_state{}; // D if the order was killed before it could rest

    friend bool operator==(const Accepted&, const Accepted&) = default;
};

struct Replaced {
    UserRef orig_user_ref{};
    UserRef user_ref{};
    OrderRef order_ref{}; // always newly issued
    Side side{};
    Price price{};
    Qty qty{};            // remaining: Q - executed_qty, before any re-entry fills
    Tif tif{};
    OrderState order_state{};

    friend bool operator==(const Replaced&, const Replaced&) = default;
};

struct Canceled {
    UserRef user_ref{};
    Qty qty{}; // incremental: shares removed by this action alone
    CancelReason reason{};

    friend bool operator==(const Canceled&, const Canceled&) = default;
};

struct Executed {
    UserRef user_ref{};
    Qty qty{};   // incremental: shares filled by this fill alone
    Price price{}; // always the resting order's price
    MatchId match_id{};
    Side side{};

    friend bool operator==(const Executed&, const Executed&) = default;
};

struct Rejected {
    UserRef user_ref{};
    RejectReason reason{};

    friend bool operator==(const Rejected&, const Rejected&) = default;
};

struct Modified {
    UserRef user_ref{};
    Qty qty{}; // remaining: leaves after the modify

    friend bool operator==(const Modified&, const Modified&) = default;
};

// Section 7.1. Unreachable in v0, which has no repricing. It exists now so the
// event stream does not change shape when repricing lands, and so the
// differential tester is already comparing it.
struct PriorityUpdate {
    UserRef user_ref{};
    OrderRef order_ref{}; // newly assigned
    Side side{};
    Price price{};

    friend bool operator==(const PriorityUpdate&, const PriorityUpdate&) = default;
};

using Event = std::variant<Accepted, Replaced, Canceled, Executed, Rejected,
                           Modified, PriorityUpdate>;

using EventList = std::vector<Event>;

} // namespace nanobook
