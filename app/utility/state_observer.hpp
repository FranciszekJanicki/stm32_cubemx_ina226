#ifndef STATE_OBSERVER_HPP
#define STATE_OBSERVER_HPP

#include "stack_matrix.hpp"

namespace Utility {

    template <typename T, std::size_t STATES, std::size_t CONTROLS = 1UL, std::size_t MEASUREMENTS = 1UL>
    struct StateObserver {
    public:
        template <std::size_t N, std::size_t M>
        using Matrix = StackMatrix<T, N, M>;

        Matrix<STATES, 1UL> operator()(this StateObserver& self,
                                       Matrix<1UL, CONTROLS> const& control,
                                       Matrix<1UL, MEASUREMENTS> const& measurement)
        {
            try {
                self.predict(control);
                self.correct(measurement);
            }
            catch (std::runtime_error const& error) {
                throw error;
            }
            return self.state;
        }

        void predict(this StateObserver& self, Matrix<1UL, CONTROLS> const& control)
        {
            try {
                self.state = self.state_transition * self.state + self.control_transition * control;
            }
            catch (std::runtime_error const& error) {
                throw error;
            }
        }

        void predict(this StateObserver& self, Matrix<1UL, MEASUREMENTS> const& measurement)
        {
            try {
                self.state = self.state + self.state_gain * (measurement - self.measurement_transition * self.state);
            }
            catch (std::runtime_error const& error) {
                throw error;
            }
        }

        Matrix<STATES, 1UL> state{};
        Matrix<STATES, 1UL> state_gain{};
        Matrix<STATES, STATES> state_transition{};
        Matrix<STATES, CONTROLS> control_transition{};
        Matrix<MEASUREMENTS, 1UL> measurement_transition{};
    };
}; // namespace Utility

#endif // STATE_OBSERVER_HPP