#pragma once
#ifndef SIMPLE_UNIFORM_NOISE_POLYFIT
#define SIMPLE_UNIFORM_NOISE_POLYFIT
#include <cmath>
#include <array>
#include <vector>

namespace utils {

// https://github.com/splicer/polyfit
// polyfit uses recursive least squares to perform a polynomial regression
// (i.e. it fits a polynomial to a set of data points without requiring a large buffer).
// For details on the math behind this recursive least squares implementation, see
// Gentlemen & Kung's famous 1981 paper "Matrix triangularization by systolic arrays".
template <typename value_t, uint8_t degree>
class Polyfit {
    static constexpr value_t s_smallValue = 1.0E-32;
    static constexpr value_t s_forgettingFactor = 1.0 - 1.0E-11;
    static constexpr size_t s_rows = degree + 1;
    static constexpr size_t s_cols = degree + 2;
public:
    Polyfit() {
        reset();
    }
    void reset() {
        std::fill(m_cells.begin(), m_cells.end(), s_smallValue);
    }
    void add(const value_t x, const value_t y) {
        std::array<value_t, s_cols> in;
        in[0] = 1.0f;
        for (size_t j = 1; j < s_cols - 1; ++j) {
            in[j] = in[j - 1] * x;
        }
        in[s_cols - 1] = y;

        for (size_t i = 0; i < s_rows; ++i) {
            value_t c;
            value_t s;
            boundaryCell(m_cells[i * s_cols + i], c, s, in[i]);
            for (size_t j = i + 1; j < s_cols; ++j) {
                value_t out = internalCell(m_cells[i * s_cols + j], c, s, in[j]);
                if (i < s_rows - 1) {
                    in[j] = out;
                }
            }
        }
        m_weightsNeedUpdate = true;
    }
    const std::array<value_t, s_rows>& weights() const {
        computeWeights();
        return m_weights;
    }
    value_t y(const value_t x) const {
        computeWeights();
        intptr_t i = s_rows - 1;
        value_t y = m_weights[i];
        while (i-- > 0) {
            y = y * x + m_weights[i];
        }
        return y;
    }
private:
    // givens generation
    static void boundaryCell(value_t& cell, value_t& c, value_t& s, value_t in) {
        if (std::abs(in) < s_smallValue) {
            // close enough to zero
            c = 1.0f;
            s = 0.0f;
        }
        else {
            value_t norm = std::sqrt(cell * cell + in * in);
            c = cell / norm;
            s = in / norm;
            cell = s_forgettingFactor * norm;
        }
    }
    // givens rotation
    static value_t internalCell(value_t& cell, value_t c, value_t s, value_t in) {
        value_t out = c * in - s_forgettingFactor * s * cell;
        cell = s * in + s_forgettingFactor * c * cell;
        return out;
    }
    void computeWeights() const {
        if (!m_weightsNeedUpdate) {
            return;
        }
        for (intptr_t i = s_rows - 1; i >= 0; --i) {
            m_weights[i] = m_cells[i * s_cols + s_cols - 1];
            for (size_t j = i + 1; j < s_cols - 1; j++) {
                m_weights[i] -= m_cells[i * s_cols + j] * m_weights[j];
            }
            m_weights[i] /= m_cells[i * s_cols + i];
        }
        m_weightsNeedUpdate = false;
    }

    std::array<value_t, s_rows * s_cols> m_cells;
    mutable std::array<value_t, s_rows> m_weights;
    mutable bool m_weightsNeedUpdate = false;
};

class PolyfitD {
    static constexpr double m_smallValue = 1.0E-32;
    static constexpr double m_forgettingFactor = 1.0 - 1.0E-11;
public:
    using value_t = double;
    void create(const uint8_t degree) {
        m_rows = degree + 1;
        m_cols = degree + 2;

        m_cells.clear();
        m_cells.resize(m_rows * m_cols, m_smallValue);

        m_weights.clear();
        m_weights.resize(m_rows);

        m_in.resize(m_cols);
    }
    void add(const value_t x, const value_t y) {
        m_in[0] = 1.0f;
        for (size_t j = 1; j < m_cols - 1; ++j) {
            m_in[j] = m_in[j - 1] * x;
        }
        m_in[m_cols - 1] = y;

        for (size_t i = 0; i < m_rows; ++i) {
            value_t c;
            value_t s;
            boundaryCell(m_cells[i * m_cols + i], c, s, m_in[i]);
            for (size_t j = i + 1; j < m_cols; ++j) {
                value_t out = internalCell(m_cells[i * m_cols + j], c, s, m_in[j]);
                if (i < m_rows - 1) {
                    m_in[j] = out;
                }
            }
        }
        m_weightsNeedUpdate = true;
    }
    const std::vector<value_t>& weights() const {
        computeWeights();
        return m_weights;
    }
    value_t y(const value_t x) const {
        computeWeights();
        intptr_t i = m_rows - 1;
        value_t y = m_weights[i];
        while (i-- > 0) {
            y = y * x + m_weights[i];
        }
        return y;
    }
private:
    // givens generation
    static void boundaryCell(value_t& cell, value_t& c, value_t& s, value_t in) {
        if (std::abs(in) < m_smallValue) {
            // close enough to zero
            c = 1.0f;
            s = 0.0f;
        }
        else {
            value_t norm = std::sqrt(cell * cell + in * in);
            c = cell / norm;
            s = in / norm;
            cell = m_forgettingFactor * norm;
        }
    }
    // givens rotation
    static value_t internalCell(value_t& cell, value_t c, value_t s, value_t in) {
        value_t out = c * in - m_forgettingFactor * s * cell;
        cell = s * in + m_forgettingFactor * c * cell;
        return out;
    }
    void computeWeights() const {
        if (!m_weightsNeedUpdate) {
            return;
        }
        for (intptr_t i = m_rows - 1; i >= 0; --i) {
            m_weights[i] = m_cells[i * m_cols + m_cols - 1];
            for (size_t j = i + 1; j < m_cols - 1; j++) {
                m_weights[i] -= m_cells[i * m_cols + j] * m_weights[j];
            }
            m_weights[i] /= m_cells[i * m_cols + i];
        }
        m_weightsNeedUpdate = false;
    }

    std::vector<value_t> m_cells;
    mutable std::vector<value_t> m_weights;
    std::vector<value_t> m_in;
    size_t m_rows = 0;
    size_t m_cols = 0;
    mutable bool m_weightsNeedUpdate = false;
};

} // namespace utils

#endif // SIMPLE_UNIFORM_NOISE_POLYFIT
