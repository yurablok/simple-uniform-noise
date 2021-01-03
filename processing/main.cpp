#include <iostream>
#include <iomanip>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../polyfit.hpp"
#include "../noise.hpp"

#define INT4D

namespace {
# ifdef NDEBUG
    constexpr size_t g_statisticsSize = 512;
    constexpr uint32_t g_statisticsReps = 100'000'000;
    constexpr uint32_t g_tableRows = 10000;
# else
    constexpr size_t g_statisticsSize = 64;
    constexpr uint32_t g_statisticsReps = 1'000'000;
    constexpr uint32_t g_tableRows = 40;
#   define DEBUG_CALC_COUT
# endif
    constexpr uint32_t g_offsetLines = 128;
# if defined(INT1D)
    utils::Polyfit<double, 5> g_polyfit;
# elif defined(INT2D)
    utils::Polyfit<double, 6> g_polyfit;
# elif defined(INT3D)
    utils::Polyfit<double, 6> g_polyfit;
# elif defined(INT4D)
    utils::Polyfit<double, 6> g_polyfit;
# endif
} // namespace


void displayImg(const sf::Image& img) {
    sf::RenderWindow window(sf::VideoMode(img.getSize().x, img.getSize().y), "test");
    sf::Texture tex;
    tex.loadFromImage(img);
    sf::Sprite spr;
    spr.setTexture(tex);
    window.draw(spr);
    window.display();
    std::cout << "Press any key to continue . . ." << std::endl;
    std::cin.get();
}

void calc() {
# if defined(INT1D)
    noise::int1d int1d;
    int1d.cellSize = 64;
# elif defined(INT2D)
    noise::int2d int2d;
    int2d.cellSize.x = 64;
    int2d.cellSize.y = 64;
# elif defined(INT3D)
    noise::int3d int3d;
    int3d.cellSize.x = 64;
    int3d.cellSize.y = 64;
    int3d.cellSize.z = 64;
# elif defined(INT4D)
    noise::int4d int4d;
    int4d.cellSize.x = 64;
    int4d.cellSize.y = 64;
    int4d.cellSize.z = 64;
    int4d.cellSize.w = 64;
# endif

    std::cout << "Step 1. Collection of the distribution statistics" << std::endl;
    utils::rng64 rng;
    std::array<uint32_t, g_statisticsSize> count = { 0 };
    uint32_t countMax = 0;
    for (uint32_t i = 0; i < g_statisticsReps; ++i) {
#     if defined(INT1D)
        const uint32_t u32 = int1d.valueRaw(rng());
#     elif defined(INT2D)
        const uint32_t u32 = int2d.valueShifted(rng(), rng());
#     elif defined(INT3D)
        const uint32_t u32 = int3d.valueShifted(rng(), rng(), rng());
#     elif defined(INT4D)
        const uint32_t u32 = int4d.valueShifted(rng(), rng(), rng(), rng());
#     endif
        constexpr uint32_t size = UINT32_MAX / count.size() + 1;
        const uint32_t idx = u32 / size;
        ++count[idx];
        countMax = std::max(countMax, count[idx]);
    }

    sf::Image img;
    img.create(512, 512, sf::Color::White);
    for (uint32_t y = 0; y < img.getSize().y; ++y) {
        // 0  y  img-1
        // 0  c  cMax
        const uint32_t c = utils::lerp_u32(y, img.getSize().y - 1, countMax);

        for (uint32_t x = 0; x < img.getSize().x; ++x) {
            // 0   x    img-1
            // 0  idx  count-1
            const size_t idx = utils::lerp_u32(x, img.getSize().x - 1, count.size() - 1);

            if (c < count[idx]) {
                img.setPixel(x, img.getSize().y - 1 - y, sf::Color(80, 80, 255));
            }
        }
    }
    displayImg(img);

    std::cout << "Step 2. Polynomial regression 1" << std::endl;
    utils::Polyfit<double, 6> polyfit;
    for (size_t x = 0; x < count.size() / 2; ++x) {
        // 0  x  count-1
        // 0  h   U32/2
        const uint32_t h = utils::lerp_u32(x, count.size() / 2 - 1, UINT32_MAX / 2);

        polyfit.add(h, count[x]);
    }
# ifdef DEBUG_CALC_COUT
    for (const auto& weight : polyfit.weights()) {
        std::cout << "  " << weight;
    }
    std::cout << std::endl;
# endif // DEBUG_CALC_COUT

    for (uint32_t x = 0; x < img.getSize().x / 2; ++x) {
        // 0  x  img/2-1
        // 0  xx  U32/2
        const uint32_t xx = utils::lerp_u32(x, img.getSize().x / 2 - 1, UINT32_MAX / 2);
        const auto yy = polyfit.y(xx);
        constexpr size_t size = UINT32_MAX / count.size() + 1;
        const size_t idx = xx / size;
#     ifdef DEBUG_CALC_COUT
        std::cout << xx << ": " << count[idx] << " vs " << yy << "\n";
#     endif // DEBUG_CALC_COUT

        const uint32_t y = utils::lerp_u32(yy, countMax, img.getSize().y - 1, 0);
        if (y >= img.getSize().y) {
            continue;
        }
        img.setPixel(x, y, sf::Color::Red);
    }
    displayImg(img);

    std::cout << "Step 3. Building the distribution table" << std::endl;
    //  1                                                                                 | 1
    //  2                                                                                 | 1
    //  3 3                                                                               | 2
    //  4 4 4                                                                             | 3
    //  5 5 5 5                                                                           | 4
    //  6 6 6 6 6                                                                         | 5
    //  7 7 7 7 7 7                                                                       | 6
    //  8 8 8 8 8 8 8                                                                     | 7
    //  9 9 9 9 9 9 9 9                                                                   | 8
    // 10101010101010101010                                                               |10
    // 1111111111111111111111                                                             |11
    // 12121212121212121212121212                                                         |13
    // 1313131313131313131313131313                                                       |14
    // 14141414141414141414141414141414                                                   |16
    // 151515151515151515151515151515151515                                               |18
    // 16161616161616161616161616161616161616                                             |19
    // 171717171717171717171717171717171717171717                                         |21
    // 18181818181818181818181818181818181818181818                                       |22
    // 191919191919191919191919191919191919191919191919                                   |24
    // 20202020202020202020202020202020202020202020202020                                 |25
    // 212121212121212121212121212121212121212121212121212121                             |27
    // 22222222222222222222222222222222222222222222222222222222                           |28
    // 2323232323232323232323232323232323232323232323232323232323                         |29
    // 24242424242424242424242424242424242424242424242424242424242424                     |31
    // 2525252525252525252525252525252525252525252525252525252525252525                   |32
    // 262626262626262626262626262626262626262626262626262626262626262626                 |33
    // 27272727272727272727272727272727272727272727272727272727272727272727               |34
    // 2828282828282828282828282828282828282828282828282828282828282828282828             |35
    // 292929292929292929292929292929292929292929292929292929292929292929292929           |36
    // 30303030303030303030303030303030303030303030303030303030303030303030303030         |37
    // 3131313131313131313131313131313131313131313131313131313131313131313131313131       |38
    // 323232323232323232323232323232323232323232323232323232323232323232323232323232     |39
    // 333333333333333333333333333333333333333333333333333333333333333333333333333333     |39
    // 34343434343434343434343434343434343434343434343434343434343434343434343434343434   |40
    // 3535353535353535353535353535353535353535353535353535353535353535353535353535353535 |41
    // 3636363636363636363636363636363636363636363636363636363636363636363636363636363636 |41
    // 3737373737373737373737373737373737373737373737373737373737373737373737373737373737 |41
    // 3838383838383838383838383838383838383838383838383838383838383838383838383838383838 |41
    // 
    // 38 rows, 872 numbers
    // 23 number per row
    // 
    // -9  -7  4:23:7 -8.13
    // a1 = -9, a2 = -7, b1 = 4, b2 = 7, b3 = b1 + b2, c1 = b1 / b3, c2 = b2 / b3,
    // d1 = a1 + c1, d2 = a2 - c2, d3 = (d1 + d2) / 2, d3 = -8.13
    // 
    //  1 2 3 3 4 4 4 5 5 5 5 6 6 6 6 6 7 7 7 7 7 7 8 | 1| -0
    //  8 8 8 8 8 8 9 9 9 9 9 9 9 9101010101010101010 | 2| -1
    // 1011111111111111111111111212121212121212121212 | 3| -2
    // 1212131313131313131313131313131314141414141414 | 4| -3
    // 1414141414141414141515151515151515151515151515 | 5| -4
    // 1515151516161616161616161616161616161616161616 | 6| -5
    // 1717171717171717171717171717171717171717171818 | 7| -6
    // 1818181818181818181818181818181818181818191919 | 8| -7  -6  1:6  -6.85
    // 1919191919191919191919191919191919191919192020 | 9| -7
    // 2020202020202020202020202020202020202020202020 |10| -8  -7  9:1  -7.1
    // 2121212121212121212121212121212121212121212121 |11| -8
    // 2121212122222222222222222222222222222222222222 |12| -9  -8 11:2  -8.15
    // 2222222222222222222323232323232323232323232323 |13| -9 
    // 2323232323232323232323232323232424242424242424 |14|-10  -9  7:9  -9.56
    // 2424242424242424242424242424242424242424242424 |15|-10  -9 14:4  -9.22
    // 2525252525252525252525252525252525252525252525 |16|-10
    // 2525252525252525252626262626262626262626262626 |17|-10
    // 2626262626262626262626262626262626262627272727 |18|-11 -10  2:20 -10.9
    // 2727272727272727272727272727272727272727272727 |19|-11 -10  3:21 -10.87
    // 2727272727272728282828282828282828282828282828 |20|-11 -10  2:23 -10.92
    // 2828282828282828282828282828282828282829292929 |21|-10  -9 23:4   -9.14
    // 2929292929292929292929292929292929292929292929 |22|-10  -9 19:9   -9.32
    // 2929292929292929293030303030303030303030303030 |23|-10  -9 14:15  -9.51
    // 3030303030303030303030303030303030303030303030 |24|-10  -9  8:23  -9.74
    // 3131313131313131313131313131313131313131313131 |25| -9  -8 23:9   -8.28
    // 3131313131313131313131313131313232323232323232 |26| -9  -8 14:19   -8.57
    // 3232323232323232323232323232323232323232323232 |27| -9  -7  4:23:7 -8.13
    // 3232323232323232333333333333333333333333333333 |28| -8  -7 16:19   -7.54
    // 3333333333333333333333333333333333333333333333 |29| -8  -6  4:23:9 -7.19
    // 3334343434343434343434343434343434343434343434 |30| -7  -6 14:23   -6.62
    // 3434343434343434343434343434343434343535353535 |31| -6  -5 23:15   -5.39
    // 3535353535353535353535353535353535353535353535 |32| -6  -4  8:23:8 -5
    // 3535353535353535353535353536363636363636363636 |33| -5  -3 15:23:1 -3.56
    // 3636363636363636363636363636363636363636363636 |34| -4  -3 22:18   -3.45
    // 3636363636363636373737373737373737373737373737 |35| -4  -2  5:23:13 -3.22
    // 3737373737373737373737373737373737373737373737 |36| -3  -1 10:23:8 -1.94
    // 3737373838383838383838383838383838383838383838 |37| -2  -0 15:23:3 -0.66
    // 383838383838383838383838383838383838383838     |38| -1  -0 20:21   -0.51
    const double yMin = polyfit.y(0.0);
    const double yMax = polyfit.y(UINT32_MAX / 2);
    //assert(yMin < yMax);
# ifdef DEBUG_CALC_COUT
    std::cout << "yMin=" << yMin << " yMax=" << yMax << std::endl;
# endif // DEBUG_CALC_COUT
    constexpr uint32_t heightMax = g_tableRows;
    uint32_t countTotal = 0;
    for (uint32_t row = 1; row <= g_tableRows; ++row) {
        const uint32_t x = utils::lerp_u32(
            1, row, g_tableRows,
            0, UINT32_MAX / 2
        );
        const double y = polyfit.y(x);
        const uint32_t h = utils::lerp_f64(
            0, y, yMax,
            1, heightMax
        );

        countTotal += h;
#     ifdef DEBUG_CALC_COUT
        for (uint32_t i = 0; i < h; ++i) {
            std::cout << std::setw(3) << row;
        }
        std::cout << std::endl;
#     endif // DEBUG_CALC_COUT
    }
    const double heightAverage = static_cast<double>(countTotal) / g_tableRows;

    std::cout << "Step 4. Calculating offsets" << std::endl;
    struct RowMeta {
        uint32_t aOffset = 0;
        uint32_t bOffset = 0;
        uint32_t aCount = 0;
        uint32_t bCount = 0;
    };
    std::vector<RowMeta> offsets(g_tableRows);
    uint32_t countRow = 0;
    countTotal = 0;
    uint32_t rowNew = 1;
    for (uint32_t rowSrc = 1; rowSrc <= g_tableRows; ++rowSrc) {
        const uint32_t x = utils::lerp_u32(
            1, rowSrc, g_tableRows,
            0, UINT32_MAX / 2
        );
        const double y = polyfit.y(x);
        const uint32_t h = utils::lerp_f64(
            0, y, yMax,
            1, heightMax
        );

        auto& offset = offsets[rowSrc - 1];
        offset.aOffset = rowSrc - rowNew;
        uint32_t countBegin = countRow;
        for (uint32_t i = 0; i < h; ++i) {
#         ifdef DEBUG_CALC_COUT
            std::cout << std::setw(3) << rowSrc;
#         endif // DEBUG_CALC_COUT
            ++countRow;
            ++countTotal;
            if (countTotal >= heightAverage * static_cast<double>(rowNew)) {
                if (offset.aCount == 0) {
                    offset.aCount = countRow - countBegin;
                }
                countRow = 0;
                countBegin = 0;
                ++rowNew;
                offset.bOffset = rowSrc - rowNew;
#             ifdef DEBUG_CALC_COUT
                std::cout << std::endl;
#             endif // DEBUG_CALC_COUT
            }
        }
        offset.bCount = countRow - countBegin;
    }

    double offsetMax = 0.0;
    for (uint32_t row = 1; row <= g_tableRows; ++row) {
        const auto& offset = offsets[row - 1];
        // -9  -7  4:23:7 -8.13
        // a1 = -9, a2 = -7, b1 = 4, b2 = 7, b3 = b1 + b2, c1 = b1 / b3, c2 = b2 / b3,
        // d1 = a1 + c1, d2 = a2 - c2, d3 = (d1 + d2) / 2, d3 = -8.13
        float abOffset = 0.0f;
        if (offset.aCount != 0 && offset.bCount != 0) {
            const float b3 = static_cast<float>(offset.aCount + offset.bCount);
            const float c1 = static_cast<float>(offset.aCount) / b3;
            const float c2 = static_cast<float>(offset.bCount) / b3;
            const float d1 = static_cast<float>(offset.aOffset) + c1;
            const float d2 = static_cast<float>(offset.bOffset) - c2;
            abOffset = (d1 + d2) * 0.5f;
        }
        else {
            abOffset = static_cast<float>(offset.aOffset);
        }
#     ifdef DEBUG_CALC_COUT
        std::cout << std::setw(2) << row << " | "
            << std::setw(2) << offset.aOffset << " " << std::setw(2) << offset.bOffset << " | "
            << std::setw(2) << offset.aCount << " " << std::setw(2) << offset.bCount << " | "
            << abOffset << std::endl;
#     endif // DEBUG_CALC_COUT
        const double x = utils::lerp_f64(
            1, row, g_tableRows,
            0, UINT32_MAX / 2
        );
        // 0  abOffset  g_tableRows-1
        // 0     y         U32/2
        const double y = utils::lerp_f64(
            0, abOffset, g_tableRows - 1,
            0, UINT32_MAX / 2
        );
        offsetMax = std::max(y, offsetMax);
        g_polyfit.add(x, y);
    }

    std::fill(
        reinterpret_cast<uint32_t*>(const_cast<sf::Uint8*>(img.getPixelsPtr())),
        reinterpret_cast<uint32_t*>(const_cast<sf::Uint8*>(img.getPixelsPtr()))
            + img.getSize().x * img.getSize().y,
        sf::Color::White.toInteger()
    );

    std::cout << "Step 5. Building the offsets table" << std::endl;
    utils::Polyfit<double, 1> pfLines;
    uint32_t idxPrev = 0;
    std::cout <<
        "uint32_t getOffsetU32(const uint32_t x_) noexcept {\n"
        "    if (x_ == 0) {\n"
        "        return 0;\n"
        "    }\n"
        "    constexpr uint32_t size = (UINT32_MAX / 2) / " << g_offsetLines << " + 1;\n"
        "    const uint64_t x = x_;\n"
        "    uint64_t offset = 0;\n"
        "    switch (x_ / size) {\n";
    constexpr double k = UINT64_C(1) << 31;
    constexpr uint64_t ki = static_cast<uint64_t>(k);
    static const auto pri = [](const uint32_t idx, const double w1, const double w0) {
        const uint64_t w1i = std::abs(static_cast<int64_t>(w1 * k));
        const uint64_t w0i = std::abs(static_cast<int64_t>(w0));
        if (w1i > UINT32_MAX) {
            std::cout << "Warning: |" << w1 << " * " << ki << "| > UINT32_MAX\n";
        }
        if (w1 < 0.0) {
            if (w0 < 0.0) { // w1 < 0, w0 < 0
                std::cout << "Error: w1 < 0 && w0 < 0\n";
            }
            else { // w1 < 0, w0 > 0
                std::cout
                    << "    case " << idx << ": offset = UINT64_C(" << w0i
                    << ") - (x * UINT64_C(" << w1i << ")) / UINT64_C("
                    << ki << "); break;\n";
            }
        }
        else {
            if (w0 < 0.0) { // w1 > 0, w0 < 0
                std::cout
                    << "    case " << idx << ": offset = (x * UINT64_C(" << w1i
                    << ")) / UINT64_C(" << ki << ") - UINT64_C(" << w0i
                    << "); break;\n";
            }
            else { // w1 > 0, w0 > 0
                std::cout
                    << "    case " << idx << ": offset = (x * UINT64_C(" << w1i
                    << ")) / UINT64_C(" << ki << ") + UINT64_C(" << w0i
                    << "); break;\n";
            }
        }
    };
    for (uint32_t x = 0; x < img.getSize().x; ++x) {
        // 0  x  img-1
        // 0  xx  U32/2
        const uint32_t xx = utils::lerp_u32(x, img.getSize().x, UINT32_MAX / 2);
        const uint32_t yy = g_polyfit.y(xx);
        // 0  yy  offsetMax
        // 0  h   img-1
        const uint32_t h = utils::lerp_u32(yy, offsetMax, img.getSize().y - 1);

        for (uint32_t y = 0; y < img.getSize().y; ++y) {
            if (y < h) {
                img.setPixel(x, img.getSize().y - y - 1, sf::Color(80, 80, 255));
            }
        }

        constexpr uint32_t size = UINT32_MAX / 2 / g_offsetLines + 1;
        const uint32_t idx = xx / size;
        if (idxPrev != idx) {
            for (uint32_t size2 = img.getSize().x / g_offsetLines,
                    x2 = idxPrev * size2,
                    x2Max = (idxPrev + 1) * size2;
                    x2 < x2Max; ++x2) {
                // 0  x  img-1
                // 0  xx  U32/2
                const uint32_t xx2 = utils::lerp_u32(x2, img.getSize().x, UINT32_MAX / 2);
                if (xx2 == 0) {
                    continue;
                }
                const uint32_t yy2 = (static_cast<int64_t>(pfLines.weights()[1] * k)
                    * static_cast<int64_t>(xx2)) / static_cast<int64_t>(k)
                    + static_cast<int64_t>(pfLines.weights()[0]);
                // 0  yy2  offsetMax
                // 0  y2   img-1
                const uint32_t y2 = utils::lerp_u32(yy2, offsetMax, img.getSize().y - 1);
                img.setPixel(x2, img.getSize().y - y2 - 1, sf::Color::Red);
            }
            pri(idxPrev, pfLines.weights()[1], pfLines.weights()[0]);
            idxPrev = idx;
            pfLines.reset();
        }
        pfLines.add(xx, yy);
    }
    for (uint32_t size2 = img.getSize().x / g_offsetLines,
            x2 = idxPrev * size2,
            x2Max = (idxPrev + 1) * size2;
            x2 < x2Max; ++x2) {
        // 0  x  img-1
        // 0  xx  U32/2
        const uint32_t xx2 = utils::lerp_u32(x2, img.getSize().x, UINT32_MAX / 2);
        if (xx2 == 0) {
            continue;
        }
        const uint32_t yy2 = (static_cast<int64_t>(pfLines.weights()[1] * k)
            * static_cast<int64_t>(xx2)) / static_cast<int64_t>(k)
            + static_cast<int64_t>(pfLines.weights()[0]);
        // 0  yy2  offsetMax
        // 0  y2   img-1
        const uint32_t y2 = utils::lerp_u32(yy2, offsetMax, img.getSize().y - 1);
        //img.setPixel(x2, img.getSize().y - y2 - 1, sf::Color::Red);
    }
    pri(idxPrev, pfLines.weights()[1], pfLines.weights()[0]);
    std::cout <<
        "    default: offset = 0; break;\n"
        "    }\n"
        "    if (offset > x) {\n"
        "        offset = x;\n"
        "    }\n"
        "    return offset;\n"
        "}" << std::endl;

    displayImg(img);
# ifdef DEBUG_CALC_COUT
    for (const auto& weight : g_polyfit.weights()) {
        std::cout << "  " << weight;
    }
    std::cout << std::endl;
# endif // DEBUG_CALC_COUT

    //NOTE: You have to apply the new offsets table and recompile the program
    std::cout << "Step 6. Collection of the new distribution statistics" << std::endl;
    std::fill(count.begin(), count.end(), 0);
    countMax = 0;
    for (uint32_t i = 0; i < g_statisticsReps; ++i) {
#     if defined(INT1D)
        const uint32_t u32 = int1d.value(rng());
#     elif defined(INT2D)
        const uint32_t u32 = int2d.value(rng(), rng());
#     elif defined(INT3D)
        const uint32_t u32 = int3d.value(rng(), rng(), rng());
#     elif defined(INT4D)
        const uint32_t u32 = int4d.value(rng(), rng(), rng(), rng());
#     endif
        constexpr uint32_t size = UINT32_MAX / count.size() + 1;
        const uint32_t idx = u32 / size;
        ++count[idx];
        countMax = std::max(countMax, count[idx]);
    }

    std::fill(
        reinterpret_cast<uint32_t*>(const_cast<sf::Uint8*>(img.getPixelsPtr())),
        reinterpret_cast<uint32_t*>(const_cast<sf::Uint8*>(img.getPixelsPtr()))
        + img.getSize().x * img.getSize().y,
        sf::Color::White.toInteger()
    );
    for (uint32_t y = 0; y < img.getSize().y; ++y) {
        // 0  y  img-1
        // 0  c  cMax
        const uint32_t c = utils::lerp_u32(y, img.getSize().y - 1, countMax);

        for (uint32_t x = 0; x < img.getSize().x; ++x) {
            // 0   x    img-1
            // 0  idx  count-1
            const size_t idx = utils::lerp_u32(x, img.getSize().x - 1, count.size() - 1);

            if (c < count[idx]) {
                img.setPixel(x, img.getSize().y - 1 - y, sf::Color(80, 80, 255));
            }
        }
    }
    displayImg(img);

    std::cout << "Done!" << std::endl;
    std::cout << "Press any key to continue . . ." << std::endl;
    std::cin.get();
}

int32_t main() {
    //calc();

    sf::RenderWindow window(sf::VideoMode(512, 512), "test");
#ifdef _DEBUG
    window.setFramerateLimit(10);
#endif
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    sf::Image img;
    img.create(window.getSize().x, window.getSize().y);
    sf::Texture tex;
    sf::Sprite spr;

# if defined(INT1D)
    noise::int1d int1d;
    int1d.cellSize = 32;
# elif defined(INT2D)
    noise::int2d int2d;
    int2d.cellSize.x = 64;
    int2d.cellSize.y = 64;
# elif defined(INT3D)
    noise::int3d int3d;
    int3d.cellSize.x = 64;
    int3d.cellSize.y = 64;
    int3d.cellSize.z = 64;
# elif defined(INT4D)
    noise::int4d int4d;
    int4d.cellSize.x = 64;
    int4d.cellSize.y = 64;
    int4d.cellSize.z = 64;
    int4d.cellSize.w = 64;
# endif

    uint64_t z = 0;
    uint64_t w = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::W:
                    z += 1;
                    break;
                case sf::Keyboard::S:
                    z -= 1;
                    break;
                case sf::Keyboard::A:
                    w -= 1;
                    break;
                case sf::Keyboard::D:
                    w += 1;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        window.clear();

        //const sf::Vector2i pos = sf::Mouse::getPosition(window);
        const sf::Vector2i pos(0, 0);

        for (uint64_t y = 0; y < img.getSize().y; ++y) {
            for (uint64_t x = 0; x < img.getSize().x; ++x) {
#             if defined(INT1D)
                const uint32_t r = int1d.value(x - pos.x);
#             elif defined(INT2D)
                const uint32_t r = int2d.value(x - pos.x, y - pos.y);
#             elif defined(INT3D)
                const uint32_t r = int3d.value(x - pos.x, y - pos.y, z);
#             elif defined(INT4D)
                const uint32_t r = int4d.value(x - pos.x, y - pos.y, z, w);
#             endif
                const float clr = (static_cast<float>(r) / static_cast<float>(UINT32_MAX)) * 255.0f;
                img.setPixel(x, y, sf::Color(clr, clr, clr));
            }
        }

        tex.loadFromImage(img);
        spr.setTexture(tex);
        window.draw(spr);
        window.display();
    }
    return 0;
}
