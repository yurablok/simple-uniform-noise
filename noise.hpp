// Simple Uniform Noise
//
// Author: Yurii Blok
// License: BSL-1.0
// https://github.com/yurablok/simple-uniform-noise
// History:
// v0.1 2023-Jan-29     First release.

#pragma once
#ifndef SIMPLE_UNIFORM_NOISE
#define SIMPLE_UNIFORM_NOISE
#include "staff.hpp"

namespace noise {

struct int1d {
    uint32_t cellSize = 64; // 2..UINT32_MAX
    uint32_t seed = 0;

    uint32_t value(const uint64_t x) const noexcept {
        uint32_t s = valueRaw(x);
        if (s < UINT32_MAX / 2) {
            return s - getOffsetU32(s);
        }
        s -= UINT32_MAX / 2;
        s = UINT32_MAX / 2 - s;
        s -= getOffsetU32(s);
        s = UINT32_MAX / 2 - s;
        s += UINT32_MAX / 2;
        return s;
    }

    uint32_t valueRaw(const uint64_t x) const noexcept {
        const uint64_t cellIdx = x / cellSize;
        const uint64_t cell = cellIdx * cellSize;
        const uint64_t t = x - cell;

        const uint32_t seed0 = utils::MurmurHash3_x32_32(&cell, sizeof(cell), seed);
        const uint64_t seedSrc = cell + cellSize;
        const uint32_t seed1 = utils::MurmurHash3_x32_32(&seedSrc, sizeof(seedSrc), seed);

        return utils::lerp_u32(t, cellSize - 1, seed0, seed1);
    }

    static uint32_t getOffsetU32(const uint32_t x_) noexcept {
        if (x_ == 0) {
            return 0;
        }
        constexpr uint32_t size = (UINT32_MAX / 2) / 128 + 1;
        const uint64_t x = x_;
        uint64_t offset = 0;
        switch (x_ / size) {
        case 0: offset = (x * UINT64_C(1903768973)) / UINT64_C(2147483648) + UINT64_C(1611352); break;
        case 1: offset = (x * UINT64_C(1837586944)) / UINT64_C(2147483648) + UINT64_C(2170639); break;
        case 2: offset = (x * UINT64_C(1779989555)) / UINT64_C(2147483648) + UINT64_C(3126264); break;
        case 3: offset = (x * UINT64_C(1723515340)) / UINT64_C(2147483648) + UINT64_C(4504459); break;
        case 4: offset = (x * UINT64_C(1668143923)) / UINT64_C(2147483648) + UINT64_C(6288329); break;
        case 5: offset = (x * UINT64_C(1613854924)) / UINT64_C(2147483648) + UINT64_C(8461461); break;
        case 6: offset = (x * UINT64_C(1560628275)) / UINT64_C(2147483648) + UINT64_C(11007901); break;
        case 7: offset = (x * UINT64_C(1508444262)) / UINT64_C(2147483648) + UINT64_C(13912146); break;
        case 8: offset = (x * UINT64_C(1457283328)) / UINT64_C(2147483648) + UINT64_C(17159147); break;
        case 9: offset = (x * UINT64_C(1407125708)) / UINT64_C(2147483648) + UINT64_C(20734327); break;
        case 10: offset = (x * UINT64_C(1357952870)) / UINT64_C(2147483648) + UINT64_C(24623476); break;
        case 11: offset = (x * UINT64_C(1309745715)) / UINT64_C(2147483648) + UINT64_C(28812865); break;
        case 12: offset = (x * UINT64_C(1262485913)) / UINT64_C(2147483648) + UINT64_C(33289143); break;
        case 13: offset = (x * UINT64_C(1216154624)) / UINT64_C(2147483648) + UINT64_C(38039438); break;
        case 14: offset = (x * UINT64_C(1170734387)) / UINT64_C(2147483648) + UINT64_C(43051170); break;
        case 15: offset = (x * UINT64_C(1126207027)) / UINT64_C(2147483648) + UINT64_C(48312251); break;
        case 16: offset = (x * UINT64_C(1082554982)) / UINT64_C(2147483648) + UINT64_C(53810941); break;
        case 17: offset = (x * UINT64_C(1039760998)) / UINT64_C(2147483648) + UINT64_C(59535873); break;
        case 18: offset = (x * UINT64_C(997808076)) / UINT64_C(2147483648) + UINT64_C(65476046); break;
        case 19: offset = (x * UINT64_C(956679065)) / UINT64_C(2147483648) + UINT64_C(71620880); break;
        case 20: offset = (x * UINT64_C(916357836)) / UINT64_C(2147483648) + UINT64_C(77960038); break;
        case 21: offset = (x * UINT64_C(876827596)) / UINT64_C(2147483648) + UINT64_C(84483670); break;
        case 22: offset = (x * UINT64_C(838072422)) / UINT64_C(2147483648) + UINT64_C(91182168); break;
        case 23: offset = (x * UINT64_C(800076492)) / UINT64_C(2147483648) + UINT64_C(98046282); break;
        case 24: offset = (x * UINT64_C(762824345)) / UINT64_C(2147483648) + UINT64_C(105067062); break;
        case 25: offset = (x * UINT64_C(726300211)) / UINT64_C(2147483648) + UINT64_C(112235980); break;
        case 26: offset = (x * UINT64_C(690489753)) / UINT64_C(2147483648) + UINT64_C(119544589); break;
        case 27: offset = (x * UINT64_C(655377152)) / UINT64_C(2147483648) + UINT64_C(126985089); break;
        case 28: offset = (x * UINT64_C(620948633)) / UINT64_C(2147483648) + UINT64_C(134549602); break;
        case 29: offset = (x * UINT64_C(587189248)) / UINT64_C(2147483648) + UINT64_C(142230842); break;
        case 30: offset = (x * UINT64_C(554085376)) / UINT64_C(2147483648) + UINT64_C(150021559); break;
        case 31: offset = (x * UINT64_C(521622937)) / UINT64_C(2147483648) + UINT64_C(157914932); break;
        case 32: offset = (x * UINT64_C(489788313)) / UINT64_C(2147483648) + UINT64_C(165904358); break;
        case 33: offset = (x * UINT64_C(458567936)) / UINT64_C(2147483648) + UINT64_C(173983538); break;
        case 34: offset = (x * UINT64_C(427949158)) / UINT64_C(2147483648) + UINT64_C(182146247); break;
        case 35: offset = (x * UINT64_C(397918720)) / UINT64_C(2147483648) + UINT64_C(190386723); break;
        case 36: offset = (x * UINT64_C(368464230)) / UINT64_C(2147483648) + UINT64_C(198699269); break;
        case 37: offset = (x * UINT64_C(339572940)) / UINT64_C(2147483648) + UINT64_C(207078586); break;
        case 38: offset = (x * UINT64_C(311233228)) / UINT64_C(2147483648) + UINT64_C(215519334); break;
        case 39: offset = (x * UINT64_C(283433216)) / UINT64_C(2147483648) + UINT64_C(224016526); break;
        case 40: offset = (x * UINT64_C(256160921)) / UINT64_C(2147483648) + UINT64_C(232565482); break;
        case 41: offset = (x * UINT64_C(229404876)) / UINT64_C(2147483648) + UINT64_C(241161645); break;
        case 42: offset = (x * UINT64_C(203154585)) / UINT64_C(2147483648) + UINT64_C(249800400); break;
        case 43: offset = (x * UINT64_C(177398425)) / UINT64_C(2147483648) + UINT64_C(258477761); break;
        case 44: offset = (x * UINT64_C(152126310)) / UINT64_C(2147483648) + UINT64_C(267189486); break;
        case 45: offset = (x * UINT64_C(127327795)) / UINT64_C(2147483648) + UINT64_C(275931692); break;
        case 46: offset = (x * UINT64_C(102992486)) / UINT64_C(2147483648) + UINT64_C(284700723); break;
        case 47: offset = (x * UINT64_C(79110553)) / UINT64_C(2147483648) + UINT64_C(293492962); break;
        case 48: offset = (x * UINT64_C(55672576)) / UINT64_C(2147483648) + UINT64_C(302304868); break;
        case 49: offset = (x * UINT64_C(32668979)) / UINT64_C(2147483648) + UINT64_C(311133176); break;
        case 50: offset = (x * UINT64_C(10090496)) / UINT64_C(2147483648) + UINT64_C(319974730); break;
        case 51: offset = UINT64_C(328826302) - (x * UINT64_C(12071424)) / UINT64_C(2147483648); break;
        case 52: offset = UINT64_C(337684969) - (x * UINT64_C(33825587)) / UINT64_C(2147483648); break;
        case 53: offset = UINT64_C(346547845) - (x * UINT64_C(55180390)) / UINT64_C(2147483648); break;
        case 54: offset = UINT64_C(355412152) - (x * UINT64_C(76144025)) / UINT64_C(2147483648); break;
        case 55: offset = UINT64_C(364275198) - (x * UINT64_C(96724428)) / UINT64_C(2147483648); break;
        case 56: offset = UINT64_C(373134294) - (x * UINT64_C(116929126)) / UINT64_C(2147483648); break;
        case 57: offset = UINT64_C(381986860) - (x * UINT64_C(136765491)) / UINT64_C(2147483648); break;
        case 58: offset = UINT64_C(390830511) - (x * UINT64_C(156240947)) / UINT64_C(2147483648); break;
        case 59: offset = UINT64_C(399662731) - (x * UINT64_C(175362252)) / UINT64_C(2147483648); break;
        case 60: offset = UINT64_C(408481040) - (x * UINT64_C(194135910)) / UINT64_C(2147483648); break;
        case 61: offset = UINT64_C(417283185) - (x * UINT64_C(212568576)) / UINT64_C(2147483648); break;
        case 62: offset = UINT64_C(426066744) - (x * UINT64_C(230666240)) / UINT64_C(2147483648); break;
        case 63: offset = UINT64_C(434829413) - (x * UINT64_C(248434841)) / UINT64_C(2147483648); break;
        case 64: offset = UINT64_C(443568948) - (x * UINT64_C(265880166)) / UINT64_C(2147483648); break;
        case 65: offset = UINT64_C(452282979) - (x * UINT64_C(283007488)) / UINT64_C(2147483648); break;
        case 66: offset = UINT64_C(460969318) - (x * UINT64_C(299822182)) / UINT64_C(2147483648); break;
        case 67: offset = UINT64_C(469625521) - (x * UINT64_C(316328908)) / UINT64_C(2147483648); break;
        case 68: offset = UINT64_C(478249365) - (x * UINT64_C(332532531)) / UINT64_C(2147483648); break;
        case 69: offset = UINT64_C(486838465) - (x * UINT64_C(348437401)) / UINT64_C(2147483648); break;
        case 70: offset = UINT64_C(495390423) - (x * UINT64_C(364047667)) / UINT64_C(2147483648); break;
        case 71: offset = UINT64_C(503902857) - (x * UINT64_C(379367321)) / UINT64_C(2147483648); break;
        case 72: offset = UINT64_C(512373273) - (x * UINT64_C(394400000)) / UINT64_C(2147483648); break;
        case 73: offset = UINT64_C(520799176) - (x * UINT64_C(409149184)) / UINT64_C(2147483648); break;
        case 74: offset = UINT64_C(529177916) - (x * UINT64_C(423617945)) / UINT64_C(2147483648); break;
        case 75: offset = UINT64_C(537507034) - (x * UINT64_C(437809561)) / UINT64_C(2147483648); break;
        case 76: offset = UINT64_C(545783479) - (x * UINT64_C(451726182)) / UINT64_C(2147483648); break;
        case 77: offset = UINT64_C(554004649) - (x * UINT64_C(465370624)) / UINT64_C(2147483648); break;
        case 78: offset = UINT64_C(562167573) - (x * UINT64_C(478744985)) / UINT64_C(2147483648); break;
        case 79: offset = UINT64_C(570269171) - (x * UINT64_C(491851110)) / UINT64_C(2147483648); break;
        case 80: offset = UINT64_C(578306469) - (x * UINT64_C(504690944)) / UINT64_C(2147483648); break;
        case 81: offset = UINT64_C(586276248) - (x * UINT64_C(517265971)) / UINT64_C(2147483648); break;
        case 82: offset = UINT64_C(594174863) - (x * UINT64_C(529576960)) / UINT64_C(2147483648); break;
        case 83: offset = UINT64_C(601998920) - (x * UINT64_C(541625036)) / UINT64_C(2147483648); break;
        case 84: offset = UINT64_C(609744880) - (x * UINT64_C(553411072)) / UINT64_C(2147483648); break;
        case 85: offset = UINT64_C(617408856) - (x * UINT64_C(564935372)) / UINT64_C(2147483648); break;
        case 86: offset = UINT64_C(624987032) - (x * UINT64_C(576198348)) / UINT64_C(2147483648); break;
        case 87: offset = UINT64_C(632475117) - (x * UINT64_C(587199692)) / UINT64_C(2147483648); break;
        case 88: offset = UINT64_C(639868917) - (x * UINT64_C(597939251)) / UINT64_C(2147483648); break;
        case 89: offset = UINT64_C(647163949) - (x * UINT64_C(608416460)) / UINT64_C(2147483648); break;
        case 90: offset = UINT64_C(654355644) - (x * UINT64_C(618630656)) / UINT64_C(2147483648); break;
        case 91: offset = UINT64_C(661438983) - (x * UINT64_C(628580556)) / UINT64_C(2147483648); break;
        case 92: offset = UINT64_C(668409025) - (x * UINT64_C(638265036)) / UINT64_C(2147483648); break;
        case 93: offset = UINT64_C(675260430) - (x * UINT64_C(647682457)) / UINT64_C(2147483648); break;
        case 94: offset = UINT64_C(681987820) - (x * UINT64_C(656831180)) / UINT64_C(2147483648); break;
        case 95: offset = UINT64_C(688585437) - (x * UINT64_C(665709107)) / UINT64_C(2147483648); break;
        case 96: offset = UINT64_C(695047358) - (x * UINT64_C(674313984)) / UINT64_C(2147483648); break;
        case 97: offset = UINT64_C(701367334) - (x * UINT64_C(682643200)) / UINT64_C(2147483648); break;
        case 98: offset = UINT64_C(707538979) - (x * UINT64_C(690694041)) / UINT64_C(2147483648); break;
        case 99: offset = UINT64_C(713555721) - (x * UINT64_C(698463641)) / UINT64_C(2147483648); break;
        case 100: offset = UINT64_C(719410365) - (x * UINT64_C(705948416)) / UINT64_C(2147483648); break;
        case 101: offset = UINT64_C(725095989) - (x * UINT64_C(713145241)) / UINT64_C(2147483648); break;
        case 102: offset = UINT64_C(730604666) - (x * UINT64_C(720049817)) / UINT64_C(2147483648); break;
        case 103: offset = UINT64_C(735929187) - (x * UINT64_C(726658867)) / UINT64_C(2147483648); break;
        case 104: offset = UINT64_C(741060687) - (x * UINT64_C(732967168)) / UINT64_C(2147483648); break;
        case 105: offset = UINT64_C(745991649) - (x * UINT64_C(738971289)) / UINT64_C(2147483648); break;
        case 106: offset = UINT64_C(750712608) - (x * UINT64_C(744665548)) / UINT64_C(2147483648); break;
        case 107: offset = UINT64_C(755215037) - (x * UINT64_C(750045542)) / UINT64_C(2147483648); break;
        case 108: offset = UINT64_C(759489399) - (x * UINT64_C(755105792)) / UINT64_C(2147483648); break;
        case 109: offset = UINT64_C(763525852) - (x * UINT64_C(759840614)) / UINT64_C(2147483648); break;
        case 110: offset = UINT64_C(767314730) - (x * UINT64_C(764244684)) / UINT64_C(2147483648); break;
        case 111: offset = UINT64_C(770845263) - (x * UINT64_C(768311552)) / UINT64_C(2147483648); break;
        case 112: offset = UINT64_C(774107026) - (x * UINT64_C(772035328)) / UINT64_C(2147483648); break;
        case 113: offset = UINT64_C(777088774) - (x * UINT64_C(775409356)) / UINT64_C(2147483648); break;
        case 114: offset = UINT64_C(779779064) - (x * UINT64_C(778426931)) / UINT64_C(2147483648); break;
        case 115: offset = UINT64_C(782165831) - (x * UINT64_C(781080832)) / UINT64_C(2147483648); break;
        case 116: offset = UINT64_C(784237169) - (x * UINT64_C(783364198)) / UINT64_C(2147483648); break;
        case 117: offset = UINT64_C(785980261) - (x * UINT64_C(785269350)) / UINT64_C(2147483648); break;
        case 118: offset = UINT64_C(787382157) - (x * UINT64_C(786788659)) / UINT64_C(2147483648); break;
        case 119: offset = UINT64_C(788429205) - (x * UINT64_C(787913932)) / UINT64_C(2147483648); break;
        case 120: offset = UINT64_C(789107606) - (x * UINT64_C(788637030)) / UINT64_C(2147483648); break;
        case 121: offset = UINT64_C(789403132) - (x * UINT64_C(788949555)) / UINT64_C(2147483648); break;
        case 122: offset = UINT64_C(789301160) - (x * UINT64_C(788842905)) / UINT64_C(2147483648); break;
        case 123: offset = UINT64_C(788786079) - (x * UINT64_C(788307660)) / UINT64_C(2147483648); break;
        case 124: offset = UINT64_C(787842503) - (x * UINT64_C(787334860)) / UINT64_C(2147483648); break;
        case 125: offset = UINT64_C(786454436) - (x * UINT64_C(785915136)) / UINT64_C(2147483648); break;
        case 126: offset = UINT64_C(784605055) - (x * UINT64_C(784038502)) / UINT64_C(2147483648); break;
        case 127: offset = UINT64_C(782599926) - (x * UINT64_C(782019072)) / UINT64_C(2147483648); break;
        default: offset = 0; break;
        }
        if (offset > x) {
            offset = x;
        }
        return offset;
    }
};

struct int2d {
    struct uint32v2_t {
        uint32_t x;
        uint32_t y;
    };
    uint32v2_t cellSize = { 64, 64 }; // 2..UINT32_MAX
    uint32_t seed = 0;

    uint32_t value(const uint64_t x, const uint64_t y) const noexcept {
        uint32_t s = valueShifted(x, y);
        if (s < UINT32_MAX / 2) {
            return s - getOffsetU32(s);
        }
        s -= UINT32_MAX / 2;
        s = UINT32_MAX / 2 - s;
        s -= getOffsetU32(s);
        s = UINT32_MAX / 2 - s;
        s += UINT32_MAX / 2;
        return s;
    }

    uint32_t valueShifted(const uint64_t x, const uint64_t y) const noexcept {
        int1d n;
        n.seed = 12;
        n.cellSize = cellSize.x;
        const uint32_t offset_x = utils::lerp_u32(n.value(x), UINT32_MAX, cellSize.x / 2);
        n.seed = 34;
        n.cellSize = cellSize.y;
        const uint32_t offset_y = utils::lerp_u32(n.value(y), UINT32_MAX, cellSize.y / 2);
        return valueRaw(x + offset_y, y + offset_x);
    }

    uint32_t valueRaw(const uint64_t x, const uint64_t y) const noexcept {
        const uint64_t cellIdx_x = x / cellSize.x;
        const uint64_t cellIdx_y = y / cellSize.y;
        const uint64_t cell_x = cellIdx_x * cellSize.x;
        const uint64_t cell_y = cellIdx_y * cellSize.y;
        const uint64_t t_x = x - cell_x;
        const uint64_t t_y = y - cell_y;
        uint64_t seedSrc[2];

        constexpr uint64_t x_c = UINT64_MAX;
        constexpr uint64_t cellSize_x_c = 32;
        constexpr uint64_t cellIdx_x_c = x_c / cellSize_x_c;
        constexpr uint64_t cell_x_c = cellIdx_x_c * cellSize_x_c;
        constexpr uint64_t t_x_c = x_c - cell_x_c;
        constexpr uint64_t v_c = utils::lerp_u32(t_x_c, cellSize_x_c - 1, INT32_MAX, UINT32_MAX);
        static_assert(v_c == UINT32_MAX, "error");

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y;
        const uint32_t seed00 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y;
        const uint32_t seed01 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y + cellSize.y;
        const uint32_t seed10 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y + cellSize.y;
        const uint32_t seed11 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        const uint32_t seed0 = utils::lerp_u32(t_x, cellSize.x - 1, seed00, seed01);
        const uint32_t seed1 = utils::lerp_u32(t_x, cellSize.x - 1, seed10, seed11);

        return utils::lerp_u32(t_y, cellSize.y - 1, seed0, seed1);
    }

    static uint32_t getOffsetU32(const uint32_t x_) noexcept {
        if (x_ == 0) {
            return 0;
        }
        constexpr uint32_t size = (UINT32_MAX / 2) / 128 + 1;
        const uint64_t x = x_;
        uint64_t offset = 0;
        switch (x_ / size) {
        case 0: offset = (x * UINT64_C(2142268877)) / UINT64_C(2147483648) - UINT64_C(39056); break;
        case 1: offset = (x * UINT64_C(2141451468)) / UINT64_C(2147483648) - UINT64_C(1817); break;
        case 2: offset = (x * UINT64_C(2129167462)) / UINT64_C(2147483648) + UINT64_C(202859); break;
        case 3: offset = (x * UINT64_C(2115453798)) / UINT64_C(2147483648) + UINT64_C(538389); break;
        case 4: offset = (x * UINT64_C(2100348825)) / UINT64_C(2147483648) + UINT64_C(1025874); break;
        case 5: offset = (x * UINT64_C(2083890534)) / UINT64_C(2147483648) + UINT64_C(1685533); break;
        case 6: offset = (x * UINT64_C(2066116044)) / UINT64_C(2147483648) + UINT64_C(2536735); break;
        case 7: offset = (x * UINT64_C(2047062220)) / UINT64_C(2147483648) + UINT64_C(3597995); break;
        case 8: offset = (x * UINT64_C(2026765977)) / UINT64_C(2147483648) + UINT64_C(4886959); break;
        case 9: offset = (x * UINT64_C(2005262899)) / UINT64_C(2147483648) + UINT64_C(6420504); break;
        case 10: offset = (x * UINT64_C(1982589081)) / UINT64_C(2147483648) + UINT64_C(8214629); break;
        case 11: offset = (x * UINT64_C(1958778931)) / UINT64_C(2147483648) + UINT64_C(10284639); break;
        case 12: offset = (x * UINT64_C(1933867827)) / UINT64_C(2147483648) + UINT64_C(12644938); break;
        case 13: offset = (x * UINT64_C(1907889971)) / UINT64_C(2147483648) + UINT64_C(15309221); break;
        case 14: offset = (x * UINT64_C(1880879001)) / UINT64_C(2147483648) + UINT64_C(18290445); break;
        case 15: offset = (x * UINT64_C(1852868556)) / UINT64_C(2147483648) + UINT64_C(21600776); break;
        case 16: offset = (x * UINT64_C(1823891507)) / UINT64_C(2147483648) + UINT64_C(25251693); break;
        case 17: offset = (x * UINT64_C(1793980928)) / UINT64_C(2147483648) + UINT64_C(29253872); break;
        case 18: offset = (x * UINT64_C(1763168716)) / UINT64_C(2147483648) + UINT64_C(33617384); break;
        case 19: offset = (x * UINT64_C(1731486720)) / UINT64_C(2147483648) + UINT64_C(38351557); break;
        case 20: offset = (x * UINT64_C(1698966476)) / UINT64_C(2147483648) + UINT64_C(43465024); break;
        case 21: offset = (x * UINT64_C(1665638809)) / UINT64_C(2147483648) + UINT64_C(48965796); break;
        case 22: offset = (x * UINT64_C(1631534643)) / UINT64_C(2147483648) + UINT64_C(54861144); break;
        case 23: offset = (x * UINT64_C(1596684083)) / UINT64_C(2147483648) + UINT64_C(61157763); break;
        case 24: offset = (x * UINT64_C(1561117081)) / UINT64_C(2147483648) + UINT64_C(67861669); break;
        case 25: offset = (x * UINT64_C(1524863180)) / UINT64_C(2147483648) + UINT64_C(74978258); break;
        case 26: offset = (x * UINT64_C(1487951462)) / UINT64_C(2147483648) + UINT64_C(82512328); break;
        case 27: offset = (x * UINT64_C(1450410854)) / UINT64_C(2147483648) + UINT64_C(90468026); break;
        case 28: offset = (x * UINT64_C(1412269414)) / UINT64_C(2147483648) + UINT64_C(98849014); break;
        case 29: offset = (x * UINT64_C(1373555660)) / UINT64_C(2147483648) + UINT64_C(107658192); break;
        case 30: offset = (x * UINT64_C(1334296780)) / UINT64_C(2147483648) + UINT64_C(116898103); break;
        case 31: offset = (x * UINT64_C(1294520320)) / UINT64_C(2147483648) + UINT64_C(126570567); break;
        case 32: offset = (x * UINT64_C(1254253670)) / UINT64_C(2147483648) + UINT64_C(136676797); break;
        case 33: offset = (x * UINT64_C(1213522841)) / UINT64_C(2147483648) + UINT64_C(147217720); break;
        case 34: offset = (x * UINT64_C(1172354406)) / UINT64_C(2147483648) + UINT64_C(158193507); break;
        case 35: offset = (x * UINT64_C(1130774476)) / UINT64_C(2147483648) + UINT64_C(169603829); break;
        case 36: offset = (x * UINT64_C(1088808345)) / UINT64_C(2147483648) + UINT64_C(181447976); break;
        case 37: offset = (x * UINT64_C(1046481766)) / UINT64_C(2147483648) + UINT64_C(193724516); break;
        case 38: offset = (x * UINT64_C(1003819315)) / UINT64_C(2147483648) + UINT64_C(206431760); break;
        case 39: offset = (x * UINT64_C(960846080)) / UINT64_C(2147483648) + UINT64_C(219567286); break;
        case 40: offset = (x * UINT64_C(917585971)) / UINT64_C(2147483648) + UINT64_C(233128457); break;
        case 41: offset = (x * UINT64_C(874063360)) / UINT64_C(2147483648) + UINT64_C(247111925); break;
        case 42: offset = (x * UINT64_C(830301952)) / UINT64_C(2147483648) + UINT64_C(261513989); break;
        case 43: offset = (x * UINT64_C(786325043)) / UINT64_C(2147483648) + UINT64_C(276330533); break;
        case 44: offset = (x * UINT64_C(742155827)) / UINT64_C(2147483648) + UINT64_C(291556928); break;
        case 45: offset = (x * UINT64_C(697817241)) / UINT64_C(2147483648) + UINT64_C(307188093); break;
        case 46: offset = (x * UINT64_C(653331712)) / UINT64_C(2147483648) + UINT64_C(323218593); break;
        case 47: offset = (x * UINT64_C(608721817)) / UINT64_C(2147483648) + UINT64_C(339642411); break;
        case 48: offset = (x * UINT64_C(564009164)) / UINT64_C(2147483648) + UINT64_C(356453367); break;
        case 49: offset = (x * UINT64_C(519215411)) / UINT64_C(2147483648) + UINT64_C(373644755); break;
        case 50: offset = (x * UINT64_C(474362265)) / UINT64_C(2147483648) + UINT64_C(391209342); break;
        case 51: offset = (x * UINT64_C(429470976)) / UINT64_C(2147483648) + UINT64_C(409139569); break;
        case 52: offset = (x * UINT64_C(384562176)) / UINT64_C(2147483648) + UINT64_C(427427628); break;
        case 53: offset = (x * UINT64_C(339656652)) / UINT64_C(2147483648) + UINT64_C(446065167); break;
        case 54: offset = (x * UINT64_C(294774681)) / UINT64_C(2147483648) + UINT64_C(465043562); break;
        case 55: offset = (x * UINT64_C(249936742)) / UINT64_C(2147483648) + UINT64_C(484353624); break;
        case 56: offset = (x * UINT64_C(205162188)) / UINT64_C(2147483648) + UINT64_C(503986179); break;
        case 57: offset = (x * UINT64_C(160471193)) / UINT64_C(2147483648) + UINT64_C(523931234); break;
        case 58: offset = (x * UINT64_C(115882752)) / UINT64_C(2147483648) + UINT64_C(544178857); break;
        case 59: offset = (x * UINT64_C(71416217)) / UINT64_C(2147483648) + UINT64_C(564718506); break;
        case 60: offset = (x * UINT64_C(27090432)) / UINT64_C(2147483648) + UINT64_C(585539428); break;
        case 61: offset = UINT64_C(606630474) - (x * UINT64_C(17075865)) / UINT64_C(2147483648); break;
        case 62: offset = UINT64_C(627980131) - (x * UINT64_C(61064089)) / UINT64_C(2147483648); break;
        case 63: offset = UINT64_C(649576629) - (x * UINT64_C(104856012)) / UINT64_C(2147483648); break;
        case 64: offset = UINT64_C(671407845) - (x * UINT64_C(148433561)) / UINT64_C(2147483648); break;
        case 65: offset = UINT64_C(693461494) - (x * UINT64_C(191779174)) / UINT64_C(2147483648); break;
        case 66: offset = UINT64_C(715724697) - (x * UINT64_C(234874931)) / UINT64_C(2147483648); break;
        case 67: offset = UINT64_C(738184419) - (x * UINT64_C(277703424)) / UINT64_C(2147483648); break;
        case 68: offset = UINT64_C(760827329) - (x * UINT64_C(320247449)) / UINT64_C(2147483648); break;
        case 69: offset = UINT64_C(783639913) - (x * UINT64_C(362490214)) / UINT64_C(2147483648); break;
        case 70: offset = UINT64_C(806608011) - (x * UINT64_C(404414464)) / UINT64_C(2147483648); break;
        case 71: offset = UINT64_C(829717542) - (x * UINT64_C(446003814)) / UINT64_C(2147483648); break;
        case 72: offset = UINT64_C(852953897) - (x * UINT64_C(487241625)) / UINT64_C(2147483648); break;
        case 73: offset = UINT64_C(876302429) - (x * UINT64_C(528111872)) / UINT64_C(2147483648); break;
        case 74: offset = UINT64_C(899747906) - (x * UINT64_C(568598169)) / UINT64_C(2147483648); break;
        case 75: offset = UINT64_C(923275044) - (x * UINT64_C(608684697)) / UINT64_C(2147483648); break;
        case 76: offset = UINT64_C(946868157) - (x * UINT64_C(648355584)) / UINT64_C(2147483648); break;
        case 77: offset = UINT64_C(970511279) - (x * UINT64_C(687595110)) / UINT64_C(2147483648); break;
        case 78: offset = UINT64_C(994188452) - (x * UINT64_C(726388172)) / UINT64_C(2147483648); break;
        case 79: offset = UINT64_C(1017882889) - (x * UINT64_C(764718899)) / UINT64_C(2147483648); break;
        case 80: offset = UINT64_C(1041578198) - (x * UINT64_C(802572646)) / UINT64_C(2147483648); break;
        case 81: offset = UINT64_C(1065257356) - (x * UINT64_C(839934310)) / UINT64_C(2147483648); break;
        case 82: offset = UINT64_C(1088902884) - (x * UINT64_C(876788633)) / UINT64_C(2147483648); break;
        case 83: offset = UINT64_C(1112497510) - (x * UINT64_C(913121228)) / UINT64_C(2147483648); break;
        case 84: offset = UINT64_C(1136023393) - (x * UINT64_C(948917350)) / UINT64_C(2147483648); break;
        case 85: offset = UINT64_C(1159462617) - (x * UINT64_C(984162662)) / UINT64_C(2147483648); break;
        case 86: offset = UINT64_C(1182796686) - (x * UINT64_C(1018842470)) / UINT64_C(2147483648); break;
        case 87: offset = UINT64_C(1206007322) - (x * UINT64_C(1052942899)) / UINT64_C(2147483648); break;
        case 88: offset = UINT64_C(1229075672) - (x * UINT64_C(1086449715)) / UINT64_C(2147483648); break;
        case 89: offset = UINT64_C(1251982550) - (x * UINT64_C(1119348684)) / UINT64_C(2147483648); break;
        case 90: offset = UINT64_C(1274708869) - (x * UINT64_C(1151626188)) / UINT64_C(2147483648); break;
        case 91: offset = UINT64_C(1297234894) - (x * UINT64_C(1183268147)) / UINT64_C(2147483648); break;
        case 92: offset = UINT64_C(1319541115) - (x * UINT64_C(1214261248)) / UINT64_C(2147483648); break;
        case 93: offset = UINT64_C(1341607337) - (x * UINT64_C(1244591667)) / UINT64_C(2147483648); break;
        case 94: offset = UINT64_C(1363413114) - (x * UINT64_C(1274245683)) / UINT64_C(2147483648); break;
        case 95: offset = UINT64_C(1384938174) - (x * UINT64_C(1303210240)) / UINT64_C(2147483648); break;
        case 96: offset = UINT64_C(1406161633) - (x * UINT64_C(1331471872)) / UINT64_C(2147483648); break;
        case 97: offset = UINT64_C(1427062291) - (x * UINT64_C(1359017113)) / UINT64_C(2147483648); break;
        case 98: offset = UINT64_C(1447619101) - (x * UINT64_C(1385833113)) / UINT64_C(2147483648); break;
        case 99: offset = UINT64_C(1467810242) - (x * UINT64_C(1411906406)) / UINT64_C(2147483648); break;
        case 100: offset = UINT64_C(1487613935) - (x * UINT64_C(1437223987)) / UINT64_C(2147483648); break;
        case 101: offset = UINT64_C(1507008301) - (x * UINT64_C(1461773107)) / UINT64_C(2147483648); break;
        case 102: offset = UINT64_C(1525970998) - (x * UINT64_C(1485540812)) / UINT64_C(2147483648); break;
        case 103: offset = UINT64_C(1544479215) - (x * UINT64_C(1508513945)) / UINT64_C(2147483648); break;
        case 104: offset = UINT64_C(1562510208) - (x * UINT64_C(1530679808)) / UINT64_C(2147483648); break;
        case 105: offset = UINT64_C(1580040850) - (x * UINT64_C(1552025600)) / UINT64_C(2147483648); break;
        case 106: offset = UINT64_C(1597047885) - (x * UINT64_C(1572538726)) / UINT64_C(2147483648); break;
        case 107: offset = UINT64_C(1613507461) - (x * UINT64_C(1592206233)) / UINT64_C(2147483648); break;
        case 108: offset = UINT64_C(1629395943) - (x * UINT64_C(1611015782)) / UINT64_C(2147483648); break;
        case 109: offset = UINT64_C(1644688752) - (x * UINT64_C(1628954265)) / UINT64_C(2147483648); break;
        case 110: offset = UINT64_C(1659361658) - (x * UINT64_C(1646009344)) / UINT64_C(2147483648); break;
        case 111: offset = UINT64_C(1673390105) - (x * UINT64_C(1662168627)) / UINT64_C(2147483648); break;
        case 112: offset = UINT64_C(1686748522) - (x * UINT64_C(1677418905)) / UINT64_C(2147483648); break;
        case 113: offset = UINT64_C(1699412299) - (x * UINT64_C(1691748403)) / UINT64_C(2147483648); break;
        case 114: offset = UINT64_C(1711355273) - (x * UINT64_C(1705143910)) / UINT64_C(2147483648); break;
        case 115: offset = UINT64_C(1722551704) - (x * UINT64_C(1717593036)) / UINT64_C(2147483648); break;
        case 116: offset = UINT64_C(1732975518) - (x * UINT64_C(1729083340)) / UINT64_C(2147483648); break;
        case 117: offset = UINT64_C(1742600209) - (x * UINT64_C(1739602227)) / UINT64_C(2147483648); break;
        case 118: offset = UINT64_C(1751399116) - (x * UINT64_C(1749137254)) / UINT64_C(2147483648); break;
        case 119: offset = UINT64_C(1759344906) - (x * UINT64_C(1757675571)) / UINT64_C(2147483648); break;
        case 120: offset = UINT64_C(1766410283) - (x * UINT64_C(1765204684)) / UINT64_C(2147483648); break;
        case 121: offset = UINT64_C(1772567461) - (x * UINT64_C(1771711897)) / UINT64_C(2147483648); break;
        case 122: offset = UINT64_C(1777788553) - (x * UINT64_C(1777184716)) / UINT64_C(2147483648); break;
        case 123: offset = UINT64_C(1782045084) - (x * UINT64_C(1781610342)) / UINT64_C(2147483648); break;
        case 124: offset = UINT64_C(1785308528) - (x * UINT64_C(1784976230)) / UINT64_C(2147483648); break;
        case 125: offset = UINT64_C(1787549565) - (x * UINT64_C(1787269324)) / UINT64_C(2147483648); break;
        case 126: offset = UINT64_C(1788739322) - (x * UINT64_C(1788477337)) / UINT64_C(2147483648); break;
        case 127: offset = UINT64_C(1788906289) - (x * UINT64_C(1788645888)) / UINT64_C(2147483648); break;
        default: offset = 0; break;
        }
        if (offset > x) {
            offset = x;
        }
        return offset;
    }
};

struct int3d {
    struct uint32v3_t {
        uint32_t x;
        uint32_t y;
        uint32_t z;
    };
    uint32v3_t cellSize = { 64, 64, 64 }; // 2..UINT32_MAX
    uint32_t seed = 0;

    uint32_t value(const uint64_t x, const uint64_t y, const uint64_t z) const noexcept {
        uint32_t s = valueShifted(x, y, z);
        if (s < UINT32_MAX / 2) {
            return s - getOffsetU32(s);
        }
        s -= UINT32_MAX / 2;
        s = UINT32_MAX / 2 - s;
        s -= getOffsetU32(s);
        s = UINT32_MAX / 2 - s;
        s += UINT32_MAX / 2;
        return s;
    }

    uint32_t valueShifted(const uint64_t x, const uint64_t y, const uint64_t z) const noexcept {
        int1d n;
        n.seed = 12;
        n.cellSize = cellSize.x;
        const uint32_t offset_x = utils::lerp_u32(n.value(x), UINT32_MAX, cellSize.x / 2);
        n.seed = 34;
        n.cellSize = cellSize.y;
        const uint32_t offset_y = utils::lerp_u32(n.value(y), UINT32_MAX, cellSize.y / 2);
        n.seed = 56;
        n.cellSize = cellSize.z;
        const uint32_t offset_z = utils::lerp_u32(n.value(z), UINT32_MAX, cellSize.z / 2);
        return valueRaw(x + offset_y, y + offset_z, z + offset_x);
    }

    uint32_t valueRaw(const uint64_t x, const uint64_t y, const uint64_t z) const noexcept {
        const uint64_t cellIdx_x = x / cellSize.x;
        const uint64_t cellIdx_y = y / cellSize.y;
        const uint64_t cellIdx_z = z / cellSize.z;
        const uint64_t cell_x = cellIdx_x * cellSize.x;
        const uint64_t cell_y = cellIdx_y * cellSize.y;
        const uint64_t cell_z = cellIdx_z * cellSize.z;
        const uint64_t t_x = x - cell_x;
        const uint64_t t_y = y - cell_y;
        const uint64_t t_z = z - cell_z;
        uint64_t seedSrc[3];

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z;
        const uint32_t seed000 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z;
        const uint32_t seed001 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z;
        const uint32_t seed010 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z;
        const uint32_t seed011 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z + cellSize.z;
        const uint32_t seed100 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z + cellSize.z;
        const uint32_t seed101 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z + cellSize.z;
        const uint32_t seed110 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z + cellSize.z;
        const uint32_t seed111 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        const uint32_t seed00 = utils::lerp_u32(t_x, cellSize.x - 1, seed000, seed001);
        const uint32_t seed01 = utils::lerp_u32(t_x, cellSize.x - 1, seed010, seed011);
        const uint32_t seed10 = utils::lerp_u32(t_x, cellSize.x - 1, seed100, seed101);
        const uint32_t seed11 = utils::lerp_u32(t_x, cellSize.x - 1, seed110, seed111);

        const uint32_t seed0 = utils::lerp_u32(t_y, cellSize.y - 1, seed00, seed01);
        const uint32_t seed1 = utils::lerp_u32(t_y, cellSize.y - 1, seed10, seed11);

        return utils::lerp_u32(t_z, cellSize.z - 1, seed0, seed1);
    }

    static uint32_t getOffsetU32(const uint32_t x_) noexcept {
        if (x_ == 0) {
            return 0;
        }
        constexpr uint32_t size = (UINT32_MAX / 2) / 128 + 1;
        const uint64_t x = x_;
        uint64_t offset = 0;
        switch (x_ / size) {
        case 0: offset = (x * UINT64_C(2121864090)) / UINT64_C(2147483648) - UINT64_C(196487); break;
        case 1: offset = (x * UINT64_C(2167263078)) / UINT64_C(2147483648) - UINT64_C(448241); break;
        case 2: offset = (x * UINT64_C(2162528256)) / UINT64_C(2147483648) - UINT64_C(369701); break;
        case 3: offset = (x * UINT64_C(2157885184)) / UINT64_C(2147483648) - UINT64_C(256373); break;
        case 4: offset = (x * UINT64_C(2153261209)) / UINT64_C(2147483648) - UINT64_C(107350); break;
        case 5: offset = (x * UINT64_C(2148585216)) / UINT64_C(2147483648) + UINT64_C(79915); break;
        case 6: offset = (x * UINT64_C(2143788646)) / UINT64_C(2147483648) + UINT64_C(309518); break;
        case 7: offset = (x * UINT64_C(2138805094)) / UINT64_C(2147483648) + UINT64_C(587035); break;
        case 8: offset = (x * UINT64_C(2133569996)) / UINT64_C(2147483648) + UINT64_C(919487); break;
        case 9: offset = (x * UINT64_C(2128021452)) / UINT64_C(2147483648) + UINT64_C(1315214); break;
        case 10: offset = (x * UINT64_C(2122098534)) / UINT64_C(2147483648) + UINT64_C(1783932); break;
        case 11: offset = (x * UINT64_C(2115744051)) / UINT64_C(2147483648) + UINT64_C(2336463); break;
        case 12: offset = (x * UINT64_C(2108901478)) / UINT64_C(2147483648) + UINT64_C(2984901); break;
        case 13: offset = (x * UINT64_C(2101517004)) / UINT64_C(2147483648) + UINT64_C(3742389); break;
        case 14: offset = (x * UINT64_C(2093538611)) / UINT64_C(2147483648) + UINT64_C(4623134); break;
        case 15: offset = (x * UINT64_C(2084916480)) / UINT64_C(2147483648) + UINT64_C(5642302); break;
        case 16: offset = (x * UINT64_C(2075602739)) / UINT64_C(2147483648) + UINT64_C(6815981); break;
        case 17: offset = (x * UINT64_C(2065551616)) / UINT64_C(2147483648) + UINT64_C(8161101); break;
        case 18: offset = (x * UINT64_C(2054719334)) / UINT64_C(2147483648) + UINT64_C(9695382); break;
        case 19: offset = (x * UINT64_C(2043064064)) / UINT64_C(2147483648) + UINT64_C(11437276); break;
        case 20: offset = (x * UINT64_C(2030545715)) / UINT64_C(2147483648) + UINT64_C(13405945); break;
        case 21: offset = (x * UINT64_C(2017126451)) / UINT64_C(2147483648) + UINT64_C(15621119); break;
        case 22: offset = (x * UINT64_C(2002770227)) / UINT64_C(2147483648) + UINT64_C(18103103); break;
        case 23: offset = (x * UINT64_C(1987443251)) / UINT64_C(2147483648) + UINT64_C(20872643); break;
        case 24: offset = (x * UINT64_C(1971113062)) / UINT64_C(2147483648) + UINT64_C(23951022); break;
        case 25: offset = (x * UINT64_C(1953749504)) / UINT64_C(2147483648) + UINT64_C(27359835); break;
        case 26: offset = (x * UINT64_C(1935324262)) / UINT64_C(2147483648) + UINT64_C(31121008); break;
        case 27: offset = (x * UINT64_C(1915811072)) / UINT64_C(2147483648) + UINT64_C(35256692); break;
        case 28: offset = (x * UINT64_C(1895185049)) / UINT64_C(2147483648) + UINT64_C(39789354); break;
        case 29: offset = (x * UINT64_C(1873423769)) / UINT64_C(2147483648) + UINT64_C(44741484); break;
        case 30: offset = (x * UINT64_C(1850506291)) / UINT64_C(2147483648) + UINT64_C(50135746); break;
        case 31: offset = (x * UINT64_C(1826413158)) / UINT64_C(2147483648) + UINT64_C(55994937); break;
        case 32: offset = (x * UINT64_C(1801127577)) / UINT64_C(2147483648) + UINT64_C(62341640); break;
        case 33: offset = (x * UINT64_C(1774633830)) / UINT64_C(2147483648) + UINT64_C(69198555); break;
        case 34: offset = (x * UINT64_C(1746918246)) / UINT64_C(2147483648) + UINT64_C(76588204); break;
        case 35: offset = (x * UINT64_C(1717969305)) / UINT64_C(2147483648) + UINT64_C(84532836); break;
        case 36: offset = (x * UINT64_C(1687776460)) / UINT64_C(2147483648) + UINT64_C(93054699); break;
        case 37: offset = (x * UINT64_C(1656331673)) / UINT64_C(2147483648) + UINT64_C(102175559); break;
        case 38: offset = (x * UINT64_C(1623628032)) / UINT64_C(2147483648) + UINT64_C(111917036); break;
        case 39: offset = (x * UINT64_C(1589660723)) / UINT64_C(2147483648) + UINT64_C(122300271); break;
        case 40: offset = (x * UINT64_C(1554426624)) / UINT64_C(2147483648) + UINT64_C(133345986); break;
        case 41: offset = (x * UINT64_C(1517924403)) / UINT64_C(2147483648) + UINT64_C(145074401); break;
        case 42: offset = (x * UINT64_C(1480154009)) / UINT64_C(2147483648) + UINT64_C(157505345); break;
        case 43: offset = (x * UINT64_C(1441117081)) / UINT64_C(2147483648) + UINT64_C(170658083); break;
        case 44: offset = (x * UINT64_C(1400817715)) / UINT64_C(2147483648) + UINT64_C(184550991); break;
        case 45: offset = (x * UINT64_C(1359260569)) / UINT64_C(2147483648) + UINT64_C(199202151); break;
        case 46: offset = (x * UINT64_C(1316452864)) / UINT64_C(2147483648) + UINT64_C(214628614); break;
        case 47: offset = (x * UINT64_C(1272402790)) / UINT64_C(2147483648) + UINT64_C(230846901); break;
        case 48: offset = (x * UINT64_C(1227120281)) / UINT64_C(2147483648) + UINT64_C(247872692); break;
        case 49: offset = (x * UINT64_C(1180617164)) / UINT64_C(2147483648) + UINT64_C(265720703); break;
        case 50: offset = (x * UINT64_C(1132906752)) / UINT64_C(2147483648) + UINT64_C(284404791); break;
        case 51: offset = (x * UINT64_C(1084003584)) / UINT64_C(2147483648) + UINT64_C(303938011); break;
        case 52: offset = (x * UINT64_C(1033924147)) / UINT64_C(2147483648) + UINT64_C(324332287); break;
        case 53: offset = (x * UINT64_C(982686361)) / UINT64_C(2147483648) + UINT64_C(345598558); break;
        case 54: offset = (x * UINT64_C(930309580)) / UINT64_C(2147483648) + UINT64_C(367746740); break;
        case 55: offset = (x * UINT64_C(876814438)) / UINT64_C(2147483648) + UINT64_C(390785742); break;
        case 56: offset = (x * UINT64_C(822223718)) / UINT64_C(2147483648) + UINT64_C(414723050); break;
        case 57: offset = (x * UINT64_C(766561587)) / UINT64_C(2147483648) + UINT64_C(439564994); break;
        case 58: offset = (x * UINT64_C(709852928)) / UINT64_C(2147483648) + UINT64_C(465317014); break;
        case 59: offset = (x * UINT64_C(652124979)) / UINT64_C(2147483648) + UINT64_C(491982881); break;
        case 60: offset = (x * UINT64_C(593405747)) / UINT64_C(2147483648) + UINT64_C(519565365); break;
        case 61: offset = (x * UINT64_C(533725440)) / UINT64_C(2147483648) + UINT64_C(548065528); break;
        case 62: offset = (x * UINT64_C(473114931)) / UINT64_C(2147483648) + UINT64_C(577483401); break;
        case 63: offset = (x * UINT64_C(411606784)) / UINT64_C(2147483648) + UINT64_C(607817461); break;
        case 64: offset = (x * UINT64_C(349235200)) / UINT64_C(2147483648) + UINT64_C(639064597); break;
        case 65: offset = (x * UINT64_C(286035609)) / UINT64_C(2147483648) + UINT64_C(671220272); break;
        case 66: offset = (x * UINT64_C(222044723)) / UINT64_C(2147483648) + UINT64_C(704278460); break;
        case 67: offset = (x * UINT64_C(157300531)) / UINT64_C(2147483648) + UINT64_C(738231600); break;
        case 68: offset = (x * UINT64_C(91842611)) / UINT64_C(2147483648) + UINT64_C(773070399); break;
        case 69: offset = (x * UINT64_C(25711923)) / UINT64_C(2147483648) + UINT64_C(808783888); break;
        case 70: offset = UINT64_C(845359577) - (x * UINT64_C(41049548)) / UINT64_C(2147483648); break;
        case 71: offset = UINT64_C(882783054) - (x * UINT64_C(108398131)) / UINT64_C(2147483648); break;
        case 72: offset = UINT64_C(921038392) - (x * UINT64_C(176289280)) / UINT64_C(2147483648); break;
        case 73: offset = UINT64_C(960107713) - (x * UINT64_C(244676864)) / UINT64_C(2147483648); break;
        case 74: offset = UINT64_C(999971437) - (x * UINT64_C(313513676)) / UINT64_C(2147483648); break;
        case 75: offset = UINT64_C(1040607884) - (x * UINT64_C(382750822)) / UINT64_C(2147483648); break;
        case 76: offset = UINT64_C(1081993863) - (x * UINT64_C(452338790)) / UINT64_C(2147483648); break;
        case 77: offset = UINT64_C(1124104067) - (x * UINT64_C(522226483)) / UINT64_C(2147483648); break;
        case 78: offset = UINT64_C(1166911336) - (x * UINT64_C(592361728)) / UINT64_C(2147483648); break;
        case 79: offset = UINT64_C(1210386147) - (x * UINT64_C(662690508)) / UINT64_C(2147483648); break;
        case 80: offset = UINT64_C(1254497740) - (x * UINT64_C(733158860)) / UINT64_C(2147483648); break;
        case 81: offset = UINT64_C(1299212571) - (x * UINT64_C(803710412)) / UINT64_C(2147483648); break;
        case 82: offset = UINT64_C(1344495506) - (x * UINT64_C(874288384)) / UINT64_C(2147483648); break;
        case 83: offset = UINT64_C(1390309053) - (x * UINT64_C(944834406)) / UINT64_C(2147483648); break;
        case 84: offset = UINT64_C(1436613848) - (x * UINT64_C(1015289344)) / UINT64_C(2147483648); break;
        case 85: offset = UINT64_C(1483368116) - (x * UINT64_C(1085592524)) / UINT64_C(2147483648); break;
        case 86: offset = UINT64_C(1530528249) - (x * UINT64_C(1155682662)) / UINT64_C(2147483648); break;
        case 87: offset = UINT64_C(1578048052) - (x * UINT64_C(1225496780)) / UINT64_C(2147483648); break;
        case 88: offset = UINT64_C(1625879361) - (x * UINT64_C(1294971187)) / UINT64_C(2147483648); break;
        case 89: offset = UINT64_C(1673972020) - (x * UINT64_C(1364041472)) / UINT64_C(2147483648); break;
        case 90: offset = UINT64_C(1722272778) - (x * UINT64_C(1432640972)) / UINT64_C(2147483648); break;
        case 91: offset = UINT64_C(1770726892) - (x * UINT64_C(1500703129)) / UINT64_C(2147483648); break;
        case 92: offset = UINT64_C(1819277139) - (x * UINT64_C(1568160102)) / UINT64_C(2147483648); break;
        case 93: offset = UINT64_C(1867863553) - (x * UINT64_C(1634942464)) / UINT64_C(2147483648); break;
        case 94: offset = UINT64_C(1916424236) - (x * UINT64_C(1700980377)) / UINT64_C(2147483648); break;
        case 95: offset = UINT64_C(1964894667) - (x * UINT64_C(1766202675)) / UINT64_C(2147483648); break;
        case 96: offset = UINT64_C(2013208156) - (x * UINT64_C(1830537523)) / UINT64_C(2147483648); break;
        case 97: offset = UINT64_C(2061295397) - (x * UINT64_C(1893911859)) / UINT64_C(2147483648); break;
        case 98: offset = UINT64_C(2109084707) - (x * UINT64_C(1956251750)) / UINT64_C(2147483648); break;
        case 99: offset = UINT64_C(2156501554) - (x * UINT64_C(2017481830)) / UINT64_C(2147483648); break;
        case 100: offset = UINT64_C(2203469764) - (x * UINT64_C(2077526886)) / UINT64_C(2147483648); break;
        case 101: offset = UINT64_C(2249909903) - (x * UINT64_C(2136309811)) / UINT64_C(2147483648); break;
        case 102: offset = UINT64_C(2295740159) - (x * UINT64_C(2193752780)) / UINT64_C(2147483648); break;
        case 103: offset = UINT64_C(2340876688) - (x * UINT64_C(2249777715)) / UINT64_C(2147483648); break;
        case 104: offset = UINT64_C(2385232275) - (x * UINT64_C(2304304640)) / UINT64_C(2147483648); break;
        case 105: offset = UINT64_C(2428717494) - (x * UINT64_C(2357253171)) / UINT64_C(2147483648); break;
        case 106: offset = UINT64_C(2471240910) - (x * UINT64_C(2408542771)) / UINT64_C(2147483648); break;
        case 107: offset = UINT64_C(2512707227) - (x * UINT64_C(2458090547)) / UINT64_C(2147483648); break;
        case 108: offset = UINT64_C(2553019725) - (x * UINT64_C(2505814220)) / UINT64_C(2147483648); break;
        case 109: offset = UINT64_C(2592078361) - (x * UINT64_C(2551629875)) / UINT64_C(2147483648); break;
        case 110: offset = UINT64_C(2629780462) - (x * UINT64_C(2595452825)) / UINT64_C(2147483648); break;
        case 111: offset = UINT64_C(2666021238) - (x * UINT64_C(2637198233)) / UINT64_C(2147483648); break;
        case 112: offset = UINT64_C(2700692475) - (x * UINT64_C(2676779622)) / UINT64_C(2147483648); break;
        case 113: offset = UINT64_C(2733683662) - (x * UINT64_C(2714110208)) / UINT64_C(2147483648); break;
        case 114: offset = UINT64_C(2764881660) - (x * UINT64_C(2749102540)) / UINT64_C(2147483648); break;
        case 115: offset = UINT64_C(2794170358) - (x * UINT64_C(2781668147)) / UINT64_C(2147483648); break;
        case 116: offset = UINT64_C(2821431014) - (x * UINT64_C(2811717939)) / UINT64_C(2147483648); break;
        case 117: offset = UINT64_C(2846542380) - (x * UINT64_C(2839162368)) / UINT64_C(2147483648); break;
        case 118: offset = UINT64_C(2869379876) - (x * UINT64_C(2863910553)) / UINT64_C(2147483648); break;
        case 119: offset = UINT64_C(2889816795) - (x * UINT64_C(2885871616)) / UINT64_C(2147483648); break;
        case 120: offset = UINT64_C(2907723242) - (x * UINT64_C(2904953548)) / UINT64_C(2147483648); break;
        case 121: offset = UINT64_C(2922966681) - (x * UINT64_C(2921063833)) / UINT64_C(2147483648); break;
        case 122: offset = UINT64_C(2935411772) - (x * UINT64_C(2934109286)) / UINT64_C(2147483648); break;
        case 123: offset = UINT64_C(2944920647) - (x * UINT64_C(2943996364)) / UINT64_C(2147483648); break;
        case 124: offset = UINT64_C(2951351957) - (x * UINT64_C(2950630195)) / UINT64_C(2147483648); break;
        case 125: offset = UINT64_C(2954562275) - (x * UINT64_C(2953916057)) / UINT64_C(2147483648); break;
        case 126: offset = UINT64_C(2954404645) - (x * UINT64_C(2953757900)) / UINT64_C(2147483648); break;
        case 127: offset = UINT64_C(2951425655) - (x * UINT64_C(2950758656)) / UINT64_C(2147483648); break;
        default: offset = 0; break;
        }
        if (offset > x) {
            offset = x;
        }
        return offset;
    }
};

struct int4d {
    struct uint32v4_t {
        uint32_t x;
        uint32_t y;
        uint32_t z;
        uint32_t w;
    };
    uint32v4_t cellSize = { 64, 64, 64, 64 }; // 2..UINT32_MAX
    uint32_t seed = 0;

    uint32_t value(const uint64_t x, const uint64_t y, const uint64_t z, const uint64_t w) const noexcept {
        uint32_t s = valueShifted(x, y, z, w);
        if (s < UINT32_MAX / 2) {
            return s - getOffsetU32(s);
        }
        s -= UINT32_MAX / 2;
        s = UINT32_MAX / 2 - s;
        s -= getOffsetU32(s);
        s = UINT32_MAX / 2 - s;
        s += UINT32_MAX / 2;
        return s;
    }

    uint32_t valueShifted(const uint64_t x, const uint64_t y, const uint64_t z, const uint64_t w) const noexcept {
        int1d n;
        n.seed = 12;
        n.cellSize = cellSize.x;
        const uint32_t offset_x = utils::lerp_u32(n.value(x), UINT32_MAX, cellSize.x / 2);
        n.seed = 34;
        n.cellSize = cellSize.y;
        const uint32_t offset_y = utils::lerp_u32(n.value(y), UINT32_MAX, cellSize.y / 2);
        n.seed = 56;
        n.cellSize = cellSize.z;
        const uint32_t offset_z = utils::lerp_u32(n.value(z), UINT32_MAX, cellSize.z / 2);
        n.seed = 78;
        n.cellSize = cellSize.w;
        const uint32_t offset_w = utils::lerp_u32(n.value(w), UINT32_MAX, cellSize.w / 2);
        return valueRaw(x + offset_y, y + offset_z, z + offset_w, w + offset_x);
    }

    uint32_t valueRaw(const uint64_t x, const uint64_t y, const uint64_t z, const uint64_t w) const noexcept {
        const uint64_t cellIdx_x = x / cellSize.x;
        const uint64_t cellIdx_y = y / cellSize.y;
        const uint64_t cellIdx_z = z / cellSize.z;
        const uint64_t cellIdx_w = w / cellSize.w;
        const uint64_t cell_x = cellIdx_x * cellSize.x;
        const uint64_t cell_y = cellIdx_y * cellSize.y;
        const uint64_t cell_z = cellIdx_z * cellSize.z;
        const uint64_t cell_w = cellIdx_w * cellSize.w;
        const uint64_t t_x = x - cell_x;
        const uint64_t t_y = y - cell_y;
        const uint64_t t_z = z - cell_z;
        const uint64_t t_w = w - cell_w;
        uint64_t seedSrc[4];

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w;
        const uint32_t seed0000 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w;
        const uint32_t seed0001 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w;
        const uint32_t seed0010 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w;
        const uint32_t seed0011 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w;
        const uint32_t seed0100 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w;
        const uint32_t seed0101 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w;
        const uint32_t seed0110 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w;
        const uint32_t seed0111 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1000 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1001 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1010 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1011 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1100 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1101 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1110 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        seedSrc[0] = cell_x + cellSize.x;
        seedSrc[1] = cell_y + cellSize.y;
        seedSrc[2] = cell_z + cellSize.z;
        seedSrc[3] = cell_w + cellSize.w;
        const uint32_t seed1111 = utils::MurmurHash3_x32_32(seedSrc, sizeof(seedSrc), seed);

        const uint32_t seed000 = utils::lerp_u32(t_x, cellSize.x - 1, seed0000, seed0001);
        const uint32_t seed001 = utils::lerp_u32(t_x, cellSize.x - 1, seed0010, seed0011);
        const uint32_t seed010 = utils::lerp_u32(t_x, cellSize.x - 1, seed0100, seed0101);
        const uint32_t seed011 = utils::lerp_u32(t_x, cellSize.x - 1, seed0110, seed0111);
        const uint32_t seed100 = utils::lerp_u32(t_x, cellSize.x - 1, seed1000, seed1001);
        const uint32_t seed101 = utils::lerp_u32(t_x, cellSize.x - 1, seed1010, seed1011);
        const uint32_t seed110 = utils::lerp_u32(t_x, cellSize.x - 1, seed1100, seed1101);
        const uint32_t seed111 = utils::lerp_u32(t_x, cellSize.x - 1, seed1110, seed1111);

        const uint32_t seed00 = utils::lerp_u32(t_y, cellSize.y - 1, seed000, seed001);
        const uint32_t seed01 = utils::lerp_u32(t_y, cellSize.y - 1, seed010, seed011);
        const uint32_t seed10 = utils::lerp_u32(t_y, cellSize.y - 1, seed100, seed101);
        const uint32_t seed11 = utils::lerp_u32(t_y, cellSize.y - 1, seed110, seed111);

        const uint32_t seed0 = utils::lerp_u32(t_z, cellSize.z - 1, seed00, seed01);
        const uint32_t seed1 = utils::lerp_u32(t_z, cellSize.z - 1, seed10, seed11);

        return utils::lerp_u32(t_w, cellSize.w - 1, seed0, seed1);
    }

    static uint32_t getOffsetU32(const uint32_t x_) noexcept {
        if (x_ == 0) {
            return 0;
        }
        constexpr uint32_t size = (UINT32_MAX / 2) / 128 + 1;
        const uint64_t x = x_;
        uint64_t offset = 0;
        switch (x_ / size) {
        case 0: offset = (x * UINT64_C(2105697178)) / UINT64_C(2147483648) - UINT64_C(210912); break;
        case 1: offset = (x * UINT64_C(2159517900)) / UINT64_C(2147483648) - UINT64_C(525571); break;
        case 2: offset = (x * UINT64_C(2158996787)) / UINT64_C(2147483648) - UINT64_C(516753); break;
        case 3: offset = (x * UINT64_C(2158173388)) / UINT64_C(2147483648) - UINT64_C(496506); break;
        case 4: offset = (x * UINT64_C(2157087692)) / UINT64_C(2147483648) - UINT64_C(461392); break;
        case 5: offset = (x * UINT64_C(2155775232)) / UINT64_C(2147483648) - UINT64_C(408733); break;
        case 6: offset = (x * UINT64_C(2154265241)) / UINT64_C(2147483648) - UINT64_C(336384); break;
        case 7: offset = (x * UINT64_C(2152582963)) / UINT64_C(2147483648) - UINT64_C(242659); break;
        case 8: offset = (x * UINT64_C(2150748416)) / UINT64_C(2147483648) - UINT64_C(126134); break;
        case 9: offset = (x * UINT64_C(2148776550)) / UINT64_C(2147483648) + UINT64_C(14504); break;
        case 10: offset = (x * UINT64_C(2146678220)) / UINT64_C(2147483648) + UINT64_C(180548); break;
        case 11: offset = (x * UINT64_C(2144459980)) / UINT64_C(2147483648) + UINT64_C(373405); break;
        case 12: offset = (x * UINT64_C(2142123468)) / UINT64_C(2147483648) + UINT64_C(594795); break;
        case 13: offset = (x * UINT64_C(2139666534)) / UINT64_C(2147483648) + UINT64_C(846790); break;
        case 14: offset = (x * UINT64_C(2137083494)) / UINT64_C(2147483648) + UINT64_C(1131899); break;
        case 15: offset = (x * UINT64_C(2134363750)) / UINT64_C(2147483648) + UINT64_C(1453348); break;
        case 16: offset = (x * UINT64_C(2131494400)) / UINT64_C(2147483648) + UINT64_C(1814899); break;
        case 17: offset = (x * UINT64_C(2128457625)) / UINT64_C(2147483648) + UINT64_C(2221277); break;
        case 18: offset = (x * UINT64_C(2125233100)) / UINT64_C(2147483648) + UINT64_C(2677976); break;
        case 19: offset = (x * UINT64_C(2121797120)) / UINT64_C(2147483648) + UINT64_C(3191475); break;
        case 20: offset = (x * UINT64_C(2118122547)) / UINT64_C(2147483648) + UINT64_C(3769345); break;
        case 21: offset = (x * UINT64_C(2114179686)) / UINT64_C(2147483648) + UINT64_C(4420218); break;
        case 22: offset = (x * UINT64_C(2109935974)) / UINT64_C(2147483648) + UINT64_C(5153913); break;
        case 23: offset = (x * UINT64_C(2105355673)) / UINT64_C(2147483648) + UINT64_C(5981590); break;
        case 24: offset = (x * UINT64_C(2100401152)) / UINT64_C(2147483648) + UINT64_C(6915602); break;
        case 25: offset = (x * UINT64_C(2095032268)) / UINT64_C(2147483648) + UINT64_C(7969678); break;
        case 26: offset = (x * UINT64_C(2089206476)) / UINT64_C(2147483648) + UINT64_C(9158977); break;
        case 27: offset = (x * UINT64_C(2082879283)) / UINT64_C(2147483648) + UINT64_C(10500067); break;
        case 28: offset = (x * UINT64_C(2076004249)) / UINT64_C(2147483648) + UINT64_C(12010987); break;
        case 29: offset = (x * UINT64_C(2068533196)) / UINT64_C(2147483648) + UINT64_C(13711262); break;
        case 30: offset = (x * UINT64_C(2060416409)) / UINT64_C(2147483648) + UINT64_C(15621905); break;
        case 31: offset = (x * UINT64_C(2051602329)) / UINT64_C(2147483648) + UINT64_C(17765544); break;
        case 32: offset = (x * UINT64_C(2042038579)) / UINT64_C(2147483648) + UINT64_C(20166219); break;
        case 33: offset = (x * UINT64_C(2031671040)) / UINT64_C(2147483648) + UINT64_C(22849650); break;
        case 34: offset = (x * UINT64_C(2020444876)) / UINT64_C(2147483648) + UINT64_C(25843018); break;
        case 35: offset = (x * UINT64_C(2008304588)) / UINT64_C(2147483648) + UINT64_C(29174967); break;
        case 36: offset = (x * UINT64_C(1995193446)) / UINT64_C(2147483648) + UINT64_C(32875791); break;
        case 37: offset = (x * UINT64_C(1981054003)) / UINT64_C(2147483648) + UINT64_C(36977323); break;
        case 38: offset = (x * UINT64_C(1965829273)) / UINT64_C(2147483648) + UINT64_C(41512604); break;
        case 39: offset = (x * UINT64_C(1949460787)) / UINT64_C(2147483648) + UINT64_C(46516463); break;
        case 40: offset = (x * UINT64_C(1931891097)) / UINT64_C(2147483648) + UINT64_C(52024780); break;
        case 41: offset = (x * UINT64_C(1913061888)) / UINT64_C(2147483648) + UINT64_C(58075060); break;
        case 42: offset = (x * UINT64_C(1892914790)) / UINT64_C(2147483648) + UINT64_C(64706191); break;
        case 43: offset = (x * UINT64_C(1871392870)) / UINT64_C(2147483648) + UINT64_C(71957949); break;
        case 44: offset = (x * UINT64_C(1848438681)) / UINT64_C(2147483648) + UINT64_C(79871616); break;
        case 45: offset = (x * UINT64_C(1823995187)) / UINT64_C(2147483648) + UINT64_C(88489680); break;
        case 46: offset = (x * UINT64_C(1798007040)) / UINT64_C(2147483648) + UINT64_C(97855356); break;
        case 47: offset = (x * UINT64_C(1770418892)) / UINT64_C(2147483648) + UINT64_C(108013154); break;
        case 48: offset = (x * UINT64_C(1741176627)) / UINT64_C(2147483648) + UINT64_C(119008421); break;
        case 49: offset = (x * UINT64_C(1710227558)) / UINT64_C(2147483648) + UINT64_C(130887224); break;
        case 50: offset = (x * UINT64_C(1677520076)) / UINT64_C(2147483648) + UINT64_C(143696439); break;
        case 51: offset = (x * UINT64_C(1643004211)) / UINT64_C(2147483648) + UINT64_C(157483500); break;
        case 52: offset = (x * UINT64_C(1606631116)) / UINT64_C(2147483648) + UINT64_C(172296555); break;
        case 53: offset = (x * UINT64_C(1568354457)) / UINT64_C(2147483648) + UINT64_C(188183851); break;
        case 54: offset = (x * UINT64_C(1528129126)) / UINT64_C(2147483648) + UINT64_C(205194206); break;
        case 55: offset = (x * UINT64_C(1485912524)) / UINT64_C(2147483648) + UINT64_C(223376410); break;
        case 56: offset = (x * UINT64_C(1441663897)) / UINT64_C(2147483648) + UINT64_C(242779447); break;
        case 57: offset = (x * UINT64_C(1395344998)) / UINT64_C(2147483648) + UINT64_C(263452136); break;
        case 58: offset = (x * UINT64_C(1346919987)) / UINT64_C(2147483648) + UINT64_C(285443097); break;
        case 59: offset = (x * UINT64_C(1296355891)) / UINT64_C(2147483648) + UINT64_C(308800469); break;
        case 60: offset = (x * UINT64_C(1243622195)) / UINT64_C(2147483648) + UINT64_C(333572006); break;
        case 61: offset = (x * UINT64_C(1188691660)) / UINT64_C(2147483648) + UINT64_C(359804614); break;
        case 62: offset = (x * UINT64_C(1131539660)) / UINT64_C(2147483648) + UINT64_C(387544572); break;
        case 63: offset = (x * UINT64_C(1072145356)) / UINT64_C(2147483648) + UINT64_C(416836863); break;
        case 64: offset = (x * UINT64_C(1010490828)) / UINT64_C(2147483648) + UINT64_C(447725500); break;
        case 65: offset = (x * UINT64_C(946561689)) / UINT64_C(2147483648) + UINT64_C(480253116); break;
        case 66: offset = (x * UINT64_C(880347648)) / UINT64_C(2147483648) + UINT64_C(514460567); break;
        case 67: offset = (x * UINT64_C(811841689)) / UINT64_C(2147483648) + UINT64_C(550387231); break;
        case 68: offset = (x * UINT64_C(741041152)) / UINT64_C(2147483648) + UINT64_C(588070335); break;
        case 69: offset = (x * UINT64_C(667947366)) / UINT64_C(2147483648) + UINT64_C(627545010); break;
        case 70: offset = (x * UINT64_C(592565555)) / UINT64_C(2147483648) + UINT64_C(668844223); break;
        case 71: offset = (x * UINT64_C(514905856)) / UINT64_C(2147483648) + UINT64_C(711998091); break;
        case 72: offset = (x * UINT64_C(434982860)) / UINT64_C(2147483648) + UINT64_C(757033982); break;
        case 73: offset = (x * UINT64_C(352815616)) / UINT64_C(2147483648) + UINT64_C(803976375); break;
        case 74: offset = (x * UINT64_C(268428083)) / UINT64_C(2147483648) + UINT64_C(852846459); break;
        case 75: offset = (x * UINT64_C(181849446)) / UINT64_C(2147483648) + UINT64_C(903661793); break;
        case 76: offset = (x * UINT64_C(93113651)) / UINT64_C(2147483648) + UINT64_C(956436424); break;
        case 77: offset = (x * UINT64_C(2260172)) / UINT64_C(2147483648) + UINT64_C(1011180268); break;
        case 78: offset = UINT64_C(1067898976) - (x * UINT64_C(90666035)) / UINT64_C(2147483648); break;
        case 79: offset = UINT64_C(1126593848) - (x * UINT64_C(185614694)) / UINT64_C(2147483648); break;
        case 80: offset = UINT64_C(1187261195) - (x * UINT64_C(282529433)) / UINT64_C(2147483648); break;
        case 81: offset = UINT64_C(1249892698) - (x * UINT64_C(381348659)) / UINT64_C(2147483648); break;
        case 82: offset = UINT64_C(1314474290) - (x * UINT64_C(482004070)) / UINT64_C(2147483648); break;
        case 83: offset = UINT64_C(1380986579) - (x * UINT64_C(584421632)) / UINT64_C(2147483648); break;
        case 84: offset = UINT64_C(1449403960) - (x * UINT64_C(688520499)) / UINT64_C(2147483648); break;
        case 85: offset = UINT64_C(1519695090) - (x * UINT64_C(794214041)) / UINT64_C(2147483648); break;
        case 86: offset = UINT64_C(1591821836) - (x * UINT64_C(901408563)) / UINT64_C(2147483648); break;
        case 87: offset = UINT64_C(1665739412) - (x * UINT64_C(1010003814)) / UINT64_C(2147483648); break;
        case 88: offset = UINT64_C(1741395888) - (x * UINT64_C(1119892582)) / UINT64_C(2147483648); break;
        case 89: offset = UINT64_C(1818731828) - (x * UINT64_C(1230960486)) / UINT64_C(2147483648); break;
        case 90: offset = UINT64_C(1897680359) - (x * UINT64_C(1343086387)) / UINT64_C(2147483648); break;
        case 91: offset = UINT64_C(1978166119) - (x * UINT64_C(1456141209)) / UINT64_C(2147483648); break;
        case 92: offset = UINT64_C(2060105633) - (x * UINT64_C(1569988812)) / UINT64_C(2147483648); break;
        case 93: offset = UINT64_C(2143406687) - (x * UINT64_C(1684485427)) / UINT64_C(2147483648); break;
        case 94: offset = UINT64_C(2227967409) - (x * UINT64_C(1799478732)) / UINT64_C(2147483648); break;
        case 95: offset = UINT64_C(2313677637) - (x * UINT64_C(1914810060)) / UINT64_C(2147483648); break;
        case 96: offset = UINT64_C(2400416266) - (x * UINT64_C(2030311116)) / UINT64_C(2147483648); break;
        case 97: offset = UINT64_C(2488052376) - (x * UINT64_C(2145805875)) / UINT64_C(2147483648); break;
        case 98: offset = UINT64_C(2576444956) - (x * UINT64_C(2261110528)) / UINT64_C(2147483648); break;
        case 99: offset = UINT64_C(2665441593) - (x * UINT64_C(2376032102)) / UINT64_C(2147483648); break;
        case 100: offset = UINT64_C(2754878577) - (x * UINT64_C(2490368972)) / UINT64_C(2147483648); break;
        case 101: offset = UINT64_C(2844580803) - (x * UINT64_C(2603911065)) / UINT64_C(2147483648); break;
        case 102: offset = UINT64_C(2934360846) - (x * UINT64_C(2716439040)) / UINT64_C(2147483648); break;
        case 103: offset = UINT64_C(3024018963) - (x * UINT64_C(2827724646)) / UINT64_C(2147483648); break;
        case 104: offset = UINT64_C(3113342523) - (x * UINT64_C(2937530368)) / UINT64_C(2147483648); break;
        case 105: offset = UINT64_C(3202105254) - (x * UINT64_C(3045608857)) / UINT64_C(2147483648); break;
        case 106: offset = UINT64_C(3290067955) - (x * UINT64_C(3151704166)) / UINT64_C(2147483648); break;
        case 107: offset = UINT64_C(3376976687) - (x * UINT64_C(3255549900)) / UINT64_C(2147483648); break;
        case 108: offset = UINT64_C(3462562832) - (x * UINT64_C(3356869683)) / UINT64_C(2147483648); break;
        case 109: offset = UINT64_C(3546543261) - (x * UINT64_C(3455377715)) / UINT64_C(2147483648); break;
        case 110: offset = UINT64_C(3628619382) - (x * UINT64_C(3550778009)) / UINT64_C(2147483648); break;
        case 111: offset = UINT64_C(3708476244) - (x * UINT64_C(3642763724)) / UINT64_C(2147483648); break;
        case 112: offset = UINT64_C(3785783224) - (x * UINT64_C(3731018342)) / UINT64_C(2147483648); break;
        case 113: offset = UINT64_C(3860192421) - (x * UINT64_C(3815214182)) / UINT64_C(2147483648); break;
        case 114: offset = UINT64_C(3931339027) - (x * UINT64_C(3895013222)) / UINT64_C(2147483648); break;
        case 115: offset = UINT64_C(3998840138) - (x * UINT64_C(3970066124)) / UINT64_C(2147483648); break;
        case 116: offset = UINT64_C(4062295372) - (x * UINT64_C(4040013312)) / UINT64_C(2147483648); break;
        case 117: offset = UINT64_C(4121285143) - (x * UINT64_C(4104483430)) / UINT64_C(2147483648); break;
        case 118: offset = UINT64_C(4175370627) - (x * UINT64_C(4163093708)) / UINT64_C(2147483648); break;
        case 119: offset = UINT64_C(4224093589) - (x * UINT64_C(4215450163)) / UINT64_C(2147483648); break;
        case 120: offset = UINT64_C(4266975846) - (x * UINT64_C(4261147392)) / UINT64_C(2147483648); break;
        case 121: offset = UINT64_C(4303518622) - (x * UINT64_C(4299768268)) / UINT64_C(2147483648); break;
        case 122: offset = UINT64_C(4333201345) - (x * UINT64_C(4330883072)) / UINT64_C(2147483648); break;
        case 123: offset = UINT64_C(4355482682) - (x * UINT64_C(4354050969)) / UINT64_C(2147483648); break;
        case 124: offset = UINT64_C(4369798260) - (x * UINT64_C(4368818022)) / UINT64_C(2147483648); break;
        case 125: offset = UINT64_C(4375561855) - (x * UINT64_C(4374718822)) / UINT64_C(2147483648); break;
        case 126: offset = UINT64_C(4372163621) - (x * UINT64_C(4371275059)) / UINT64_C(2147483648); break;
        case 127: offset = UINT64_C(4361286635) - (x * UINT64_C(4360323072)) / UINT64_C(2147483648); break;
        default: offset = 0; break;
        }
        if (offset > x) {
            offset = x;
        }
        return offset;
    }
};

} // namespace noise

#endif // SIMPLE_UNIFORM_NOISE
