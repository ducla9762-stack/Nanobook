#pragma once

#include <cstdint>

// Section 13. Two enums, deliberately not interchangeable: a cancel reason is a
// single alpha char on CANCELED, a reject reason is a 2-byte numeric on REJECTED.
//
// Both are part of the event stream and are therefore compared byte-for-byte by
// the differential tester, so the numbering is borrowed from OUCH 5.0 rather than
// invented. Do not renumber, and leave the gaps where deferred features sit.

namespace nanobook {

enum class CancelReason : char {
    // Reachable in v0.
    User = 'U',   // client sent CANCEL or a reducing MODIFY
    Ioc = 'I',    // IOC remainder killed after matching
    System = 'Z', // replace failed validation and the original was killed (9.2)

    // Defined but unreachable in v0. Present so the enum does not change shape
    // when the corresponding feature lands.
    Timeout = 'T',
    SelfMatch = 'Q',
    Supervisory = 'S',
    Halted = 'H',
    MarketCollar = 'K',
    PostOnlyPrice = 'F',
    PostOnlyCross = 'G',
    Regulatory = 'D',
    Closed = 'E',
    OpenProtection = 'X',
};

enum class RejectReason : std::uint16_t {
    InvalidSide = 0x0009,
    ProcessingError = 0x000A, // slab exhausted
    InvalidQuantity = 0x0013,
    ReplaceNotAllowed = 0x0015,
    InvalidSymbol = 0x0017,
    InvalidPrice = 0x001D,
    SharesExceedThreshold = 0x001E, // chain cumulative cap
    // 0x8000 and above is reserved for engine-internal reasons with no OUCH
    // equivalent. Nothing in v0 uses that range.
};

} // namespace nanobook
