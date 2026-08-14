#pragma once

#include <variant>

#include "types.hpp"

// Section 6.1. The four inbound message types.
//
// Priority is decided by the message tag, never by diffing an inbound order
// against resting state (section 7). That is why CANCEL and MODIFY exist as
// separate messages from REPLACE even though all three can shrink an order.

namespace nanobook {

// Supplied by the sequencer. The engine never reads a clock (section 10), so this
// is the only source of time in the system.
struct MsgHeader {
    SeqNo seq_no{};
    Ts timestamp{};

    friend bool operator==(const MsgHeader&, const MsgHeader&) = default;
};

// Assigns priority. qty is cumulative, but a fresh chain has no fills so it
// equals leaves.
struct Enter {
    UserRef user_ref{};
    Side side{};
    Qty qty{};
    Price price{};
    Tif tif{};

    friend bool operator==(const Enter&, const Enter&) = default;
};

// Cancel-and-replace in one message. Always takes a new timestamp and a new
// order_ref, even when it only shrinks the order (section 7).
//
// There is no side field: a replace cannot change side, it inherits the original
// link's. qty is cumulative for the whole chain, including prior fills.
struct Replace {
    UserRef orig_user_ref{};
    UserRef user_ref{};
    Qty qty{};
    Price price{};
    Tif tif{};

    friend bool operator==(const Replace&, const Replace&) = default;
};

// Reduce to an intended cumulative size, or to zero for a full cancel.
// Preserves priority. Can never increase an order.
struct Cancel {
    UserRef user_ref{};
    Qty qty{};

    friend bool operator==(const Cancel&, const Cancel&) = default;
};

// Reduce-only and priority-preserving. v0 carries quantity alone; the side field
// arrives with Reg SHO short marking (section 12).
struct Modify {
    UserRef user_ref{};
    Qty qty{};

    friend bool operator==(const Modify&, const Modify&) = default;
};

using MessageBody = std::variant<Enter, Replace, Cancel, Modify>;

struct Message {
    MsgHeader hdr{};
    MessageBody body{};

    friend bool operator==(const Message&, const Message&) = default;
};

} // namespace nanobook
