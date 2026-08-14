#pragma once

#include <ostream>
#include <variant>

#include "events.hpp"
#include "messages.hpp"

// Human-readable event printing. Test support only - the engine never formats
// anything, and nothing in this header is on any hot path.
//
// GoogleTest finds these by ADL, so an EXPECT_EQ on two EventLists prints the
// actual divergence instead of two byte blobs.

namespace nanobook {

inline std::ostream& operator<<(std::ostream& os, Side s) {
    return os << (s == Side::Buy ? "BUY" : "SELL");
}

inline std::ostream& operator<<(std::ostream& os, Tif t) {
    return os << (t == Tif::Day ? "DAY" : "IOC");
}

inline std::ostream& operator<<(std::ostream& os, OrderState s) {
    return os << static_cast<char>(s);
}

inline std::ostream& operator<<(std::ostream& os, CancelReason r) {
    return os << static_cast<char>(r);
}

inline std::ostream& operator<<(std::ostream& os, RejectReason r) {
    const auto value = static_cast<unsigned>(r);
    const char* name = "UNKNOWN";
    switch (r) {
    case RejectReason::InvalidSide: name = "INVALID_SIDE"; break;
    case RejectReason::ProcessingError: name = "PROCESSING_ERROR"; break;
    case RejectReason::InvalidQuantity: name = "INVALID_QUANTITY"; break;
    case RejectReason::ReplaceNotAllowed: name = "REPLACE_NOT_ALLOWED"; break;
    case RejectReason::InvalidSymbol: name = "INVALID_SYMBOL"; break;
    case RejectReason::InvalidPrice: name = "INVALID_PRICE"; break;
    case RejectReason::SharesExceedThreshold: name = "SHARES_EXCEED_THRESHOLD"; break;
    }
    return os << name << "(0x" << std::hex << value << std::dec << ")";
}

inline std::ostream& operator<<(std::ostream& os, const Accepted& e) {
    return os << "ACCEPTED{ref=" << e.user_ref << " oref=" << e.order_ref << " "
              << e.side << " px=" << e.price << " qty=" << e.qty << " " << e.tif
              << " state=" << e.order_state << "}";
}

inline std::ostream& operator<<(std::ostream& os, const Replaced& e) {
    return os << "REPLACED{orig=" << e.orig_user_ref << " ref=" << e.user_ref
              << " oref=" << e.order_ref << " " << e.side << " px=" << e.price
              << " leaves=" << e.qty << " " << e.tif << " state=" << e.order_state << "}";
}

inline std::ostream& operator<<(std::ostream& os, const Canceled& e) {
    return os << "CANCELED{ref=" << e.user_ref << " decrement=" << e.qty
              << " reason=" << e.reason << "}";
}

inline std::ostream& operator<<(std::ostream& os, const Executed& e) {
    return os << "EXECUTED{ref=" << e.user_ref << " qty=" << e.qty << " px=" << e.price
              << " match=" << e.match_id << " " << e.side << "}";
}

inline std::ostream& operator<<(std::ostream& os, const Rejected& e) {
    return os << "REJECTED{ref=" << e.user_ref << " reason=" << e.reason << "}";
}

inline std::ostream& operator<<(std::ostream& os, const Modified& e) {
    return os << "MODIFIED{ref=" << e.user_ref << " leaves=" << e.qty << "}";
}

inline std::ostream& operator<<(std::ostream& os, const PriorityUpdate& e) {
    return os << "PRIORITY_UPDATE{ref=" << e.user_ref << " oref=" << e.order_ref << " "
              << e.side << " px=" << e.price << "}";
}

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
    std::visit([&os](const auto& concrete) { os << concrete; }, e);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const EventList& events) {
    os << "[";
    for (std::size_t i = 0; i < events.size(); ++i) {
        os << (i == 0 ? " " : ", ") << events[i];
    }
    return os << " ]";
}

} // namespace nanobook
