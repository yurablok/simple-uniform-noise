#pragma once
#ifndef SIMPLE_UNIFORM_NOISE_STAFF
#define SIMPLE_UNIFORM_NOISE_STAFF
#include <cstdint>

namespace utils {

#ifdef __GNUG__
#   define SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
#endif

#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC push_options
# pragma GCC optimize ("O0")
#endif
inline uint64_t MurmurHash2_x64_64A(
        const void* key, const uint64_t len, const uint64_t seed) {
#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC pop_options
#endif
    constexpr uint64_t m = UINT64_C(0xC6A4A7935BD1E995);
    constexpr uint64_t r = 47;
    uint64_t h = seed ^ (len * m);
    const uint64_t* data = static_cast<const uint64_t*>(key);
    const uint64_t* end = data + (len >> 3);
    while (data != end) {
        uint64_t k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
    }
    const uint8_t* tail = reinterpret_cast<const uint8_t*>(data);
    switch (len & 7) {
    case 7: h ^= static_cast<uint64_t>(tail[6]) << 48; [[fallthrough]];
    case 6: h ^= static_cast<uint64_t>(tail[5]) << 40; [[fallthrough]];
    case 5: h ^= static_cast<uint64_t>(tail[4]) << 32; [[fallthrough]];
    case 4: h ^= static_cast<uint64_t>(tail[3]) << 24; [[fallthrough]];
    case 3: h ^= static_cast<uint64_t>(tail[2]) << 16; [[fallthrough]];
    case 2: h ^= static_cast<uint64_t>(tail[1]) << 8;  [[fallthrough]];
    case 1: h ^= static_cast<uint64_t>(tail[0]);
        h *= m;
    };
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}
#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC push_options
# pragma GCC optimize ("O0")
#endif
inline constexpr uint64_t MurmurHash2_x64_64A(
        const uint64_t key, const uint64_t seed) {
#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC pop_options
#endif
    constexpr uint64_t m = UINT64_C(0xC6A4A7935BD1E995);
    constexpr uint64_t m8 = sizeof(key) * UINT64_C(0xC6A4A7935BD1E995);
    constexpr uint64_t r = 47;
    uint64_t h = seed ^ m8;
    uint64_t k = key;
    k *= m;
    k ^= k >> r;
    k *= m;
    h ^= k;
    h *= m;
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}

#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC push_options
# pragma GCC optimize ("O0")
#endif
inline uint32_t MurmurHash2_x32_32A(
        const void* key, const uint32_t len, const uint32_t seed) {
#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC pop_options
#endif
    constexpr uint32_t m = 0x5BD1E995;
    constexpr uint32_t r = 24;
    uint32_t h = seed;
    const uint32_t* data = static_cast<const uint32_t*>(key);
    const uint32_t* end = data + (len >> 2);
    while (data != end) {
        uint32_t k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
    }
    const uint8_t* tail = reinterpret_cast<const uint8_t*>(data);
    uint32_t t = 0;
    switch (len & 3) {
    case 3: t ^= static_cast<uint32_t>(tail[2]) << 16; [[fallthrough]];
    case 2: t ^= static_cast<uint32_t>(tail[1]) << 8;  [[fallthrough]];
    case 1: t ^= static_cast<uint32_t>(tail[0]);
    };
    t *= m;
    t ^= t >> r;
    t *= m;
    h *= m;
    h ^= t;
    //mmix(h, len);
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC push_options
# pragma GCC optimize ("O0")
#endif
inline uint32_t MurmurHash3_x32_32(
        const void* key, const uint32_t len, const uint32_t seed) {
#ifdef SIMPLE_UNIFORM_NOISE_STAFF_GCC_WORKAROUND
# pragma GCC pop_options
#endif
    constexpr uint32_t c1 = 0xCC9E2D51;
    constexpr uint32_t c2 = 0x1B873593;
    uint32_t h1 = seed;
    const uint32_t* data = static_cast<const uint32_t*>(key);
    const uint32_t* end = data + (len >> 2);
    while (data != end) {
        uint32_t k1 = *data++;
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;
        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> (32 - 13));
        h1 = h1 * 5 + 0xE6546B64;
    }
    const uint8_t* tail = reinterpret_cast<const uint8_t*>(data);
    uint32_t k1 = 0;
    switch (len & 3) {
    case 3: k1 ^= static_cast<uint32_t>(tail[2]) << 16; [[fallthrough]];
    case 2: k1 ^= static_cast<uint32_t>(tail[1]) << 8;  [[fallthrough]];
    case 1: k1 ^= static_cast<uint32_t>(tail[0]);
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;
        h1 ^= k1;
    };
    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85EBCA6B;
    h1 ^= h1 >> 13;
    h1 *= 0xC2B2AE35;
    h1 ^= h1 >> 16;
    return h1;
}

class lcg32 {
public:
    lcg32() = default;
    lcg32(const uint32_t seed_) noexcept {
        seed(seed_);
    }
    void seed(const uint32_t seed_) noexcept {
        //m_x = seed_ == 0 ? 1 : seed_;
        m_x = seed_ | (1 << 31);
    }
    //void set(const uint64_t a, const uint64_t c) {
    //    m_a = a;
    //    m_c = c;
    //}
    uint32_t operator()() noexcept { // 2147483647 4294967295
        //m_x = (UINT64_C(1035118825) * m_x + UINT64_C(718786826)) % INT32_MAX; // 63.4056 %
        //m_x = (UINT64_C(1664525) * m_x + UINT64_C(1013904223)) % INT32_MAX; // 63.3217 %
        //m_x = (UINT64_C(48271) * m_x + UINT64_C(0)) % INT32_MAX; // 63.2122 %
        
        m_x = (UINT64_C(1260864976) * m_x + UINT64_C(1379216869)) % UINT32_MAX; // 68.2358 %
        //m_x = (UINT64_C(1664525) * m_x + UINT64_C(1013904223)) % UINT32_MAX; // 12.1635 %
        
        //m_x = (m_a * m_x + m_c) % INT32_MAX;
        return m_x;
    }
private:
    //uint64_t m_a = 1;
    //uint64_t m_c = 1;
    uint32_t m_x = 1;
};

class lcg64 {
public:
    lcg64() = default;
    lcg64(const uint64_t seed_) noexcept {
        seed(seed_);
    }
    void seed(const uint64_t seed_) noexcept {
        m_lo.seed(seed_ & UINT32_MAX);
        m_hi.seed(seed_ >> 32);
    }
    uint64_t operator()() noexcept {
        return (static_cast<uint64_t>(m_hi()) << 32) | static_cast<uint64_t>(m_lo());
    }
private:
    lcg32 m_lo;
    lcg32 m_hi;
};

class rng64 {
public:
    void seed(const uint64_t value) {
        m_x = value;
    }
    uint64_t operator()() noexcept {
        // MurmurHash64A
        constexpr uint64_t m = UINT64_C(0xC6A4A7935BD1E995);
        constexpr uint64_t m8 = sizeof(uint64_t) * UINT64_C(0xC6A4A7935BD1E995);
        constexpr uint64_t r = 47;
        uint64_t h = m8;
        uint64_t k = m_x;
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
        h ^= h >> r;
        h *= m;
        h ^= h >> r;
        m_x = h;
        return m_x;
    }
private:
    uint64_t m_x = 1;
};

// from_a  from_t  from_b
//  to_a   result   to_b
inline constexpr uint32_t lerp_u32(
        const uint32_t from_a, const uint32_t from_t, const uint32_t from_b,
        const uint32_t to_a, const uint32_t to_b) {
    const uint64_t from_a_ = from_a;
    const uint64_t from_t_ = from_t;
    const uint64_t from_b_ = from_b;
    const uint64_t to_a_ = to_a;
    const uint64_t to_b_ = to_b;
    if (to_a < to_b) {
        // 0  from_t-from_a  from_b-from_a
        // 0   result-to_a     to_b-to_a
        return ((from_t_ - from_a_) * (to_b_ - to_a_)) / (from_b_ - from_a_) + to_a_;
    }
    else {
        // 0  from_t-from_a  from_b-from_a
        // 0   to_a-result     to_a-to_b
        return to_a_ - ((from_t_ - from_a_) * (to_a_ - to_b_)) / (from_b_ - from_a_);
    }
}
//   0   from_t  from_b
// to_a  result   to_b
inline constexpr uint32_t lerp_u32(
        const uint32_t from_t, const uint32_t from_b,
        const uint32_t to_a, const uint32_t to_b) {
    const uint64_t from_t_ = from_t;
    const uint64_t from_b_ = from_b;
    const uint64_t to_a_ = to_a;
    const uint64_t to_b_ = to_b;
    if (to_a < to_b) {
        // 0     from_t      from_b
        // 0  result-to_a  to_b-to_a
        return (from_t_ * (to_b_ - to_a_)) / from_b_ + to_a_;
    }
    else {
        // 0     from_t      from_b
        // 0  to_a-result  to_a-to_b
        return to_a_ - (from_t_ * (to_a_ - to_b_)) / from_b_;
    }
}
//   0   from_t  from_b
//   0   result   to_b
inline constexpr uint32_t lerp_u32(
        const uint32_t from_t, const uint32_t from_b,
        const uint32_t to_b) {
    const uint64_t from_t_ = from_t;
    const uint64_t from_b_ = from_b;
    const uint64_t to_b_ = to_b;
    return (from_t_ * to_b_) / from_b_;
}
// from_a  from_t  from_b
//  to_a   result   to_b
inline constexpr double lerp_f64(
        const double from_a, const double from_t, const double from_b,
        const double to_a, const double to_b) {
    // 0  from_t-from_a  from_b-from_a
    // 0   result-to_a     to_b-to_a
    return ((from_t - from_a) * (to_b - to_a)) / (from_b - from_a) + to_a;
};

} // namespace utils

#endif // SIMPLE_UNIFORM_NOISE_STAFF
