#ifndef KALMAN_HPP
#define KALMAN_HPP

#include "stack_matrix.hpp"
#include <concepts>
#include <print>
#include <stdexcept>
#include <utility>

namespace Utility {

    namespace {
        using namespace Stack;
    };

    template <typename T, std::size_t STATES, std::size_t CONTROLS = 1UL, std::size_t MEASUREMENTS = 1UL>
    struct Kalman {
        template <std::size_t ROWS, std::size_t COLS>
        using Matrix = Matrix<T, ROWS, COLS>;

        [[nodiscard]] Matrix<STATES, 1UL> operator()(this Kalman& self,
                                                     Matrix<1UL, CONTROLS> const& control,
                                                     Matrix<1UL, MEASUREMENTS> const& measurement)
        {
            try {
                self.predict(control);
                self.correct(measurement);
                return self.state;
            }
            catch (std::runtime_error const& error) {
                throw error;
            }
        }

        void predict(this Kalman& self, Matrix<1UL, CONTROLS> const& control)
        {
            try {
                self.state = (self.state_transition * self.state) + (self.control_transition * control);
                self.state_covariance =
                    (self.state_transition * self.state_covariance * matrix_transpose(self.state_transition)) +
                    self.process_noise;
            }
            catch (std::runtime_error const& error) {
                throw error;
            }
        }

        void correct(this Kalman& self, Matrix<1UL, MEASUREMENTS> const& measurement)
        {
            try {
                auto const innovation{measurement - (self.measurement_transition * self.state)};
                auto const residual_covariance{(self.measurement_transition * self.state_covariance *
                                                matrix_transpose(self.measurement_transition)) +
                                               self.measurement_noise};
                auto const kalman_gain{self.state_covariance * matrix_transpose(self.measurement_transition) *
                                       matrix_inverse(residual_covariance)};
                self.state = self.state + (kalman_gain * innovation);
                self.state_covariance =
                    (make_eye<T, STATES>() - kalman_gain * self.measurement_transition) * self.state_covariance;
            }
            catch (std::runtime_error const& error) {
                throw error;
            }
        }

        Matrix<STATES, 1UL> state{};
        Matrix<STATES, STATES> state_covariance{};

        Matrix<STATES, STATES> state_transition{};
        Matrix<STATES, CONTROLS> control_transition{};
        Matrix<MEASUREMENTS, STATES> measurement_transition{};

        Matrix<MEASUREMENTS, MEASUREMENTS> measurement_noise{};
        Matrix<STATES, STATES> process_noise{};
    };

}; // namespace Utility

#endif // KALMAN_HPP