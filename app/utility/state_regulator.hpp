#ifndef STATE_REGULATOR_HPP
#define STATE_REGULATOR_HPP

#include "stack_matrix.hpp"

namespace Utility {
    template <typename T, std::size_t STATES, std::size_t INPUTS>
    struct StateRegulator {
    public:
        template <std::size_t N, std::size_t M>
        using Matrix = StackMatrix<T, N, M>;

        operator()(this StateRegulator & self, Matrix<1UL, INPUTS> const& input, Matrix<STATES, 1UL> const& state)
        {
            try {
                return input - self.state_gain * state;
            }
            catch (std::runtime_error const& error) {
                throw error;
            }
        }

        Matrix<1UL, STATES> state_gain{};
    };

}; // namespace Utility

#endif // STATE_REGULATOR_HPP