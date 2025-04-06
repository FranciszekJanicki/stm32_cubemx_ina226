#ifndef PID_HPP
#define PID_HPP

#include "utility.hpp"
#include <concepts>
#include <utility>

namespace Utility {

    template <typename T>
    struct PID {
        T operator()(this PID& self, T const error, T const sampling_time) noexcept
        {
            return self.get_sat_control(error, sampling_time);
        }

        T get_sat_control(this PID& self, T const error, T const sampling_time) noexcept
        {
            auto const control = self.get_control(error, sampling_time);
            auto const sat_control = std::clamp(control, -self.saturation, self.saturation);

            self.prev_sat_error = std::exchange(self.sat_error, control - sat_control);
            self.prev_error = error;

            return sat_control;
        }

        T get_control(this PID& self, T const error, T const sampling_time) noexcept
        {
            return self.get_proportion(error) + self.get_integral(error, sampling_time) +
                   self.get_derivative(error, sampling_time);
        }

        T get_proportion(this PID& self, T const error) noexcept
        {
            return self.proportion_gain * error;
        }

        T get_derivative(this PID& self, T const error, T const sampling_time) noexcept
        {
            self.error_derivative = Utility::differentiate(error,
                                                           self.prev_error,
                                                           sampling_time,
                                                           self.error_derivative,
                                                           self.time_constant);

            return self.derivative_gain * self.error_derivative;
        }

        T get_integral(this PID& self, T const error, T const sampling_time) noexcept
        {
            self.error_integral += Utility::integrate(error, self.prev_error, sampling_time);
            self.sat_error_integral += Utility::integrate(self.sat_error, self.prev_sat_error, sampling_time);

            return self.integral_gain * self.error_integral - self.control_gain * self.sat_error_integral;
        }

        // basic PID parameters
        T proportion_gain{};
        T integral_gain{};
        T derivative_gain{};
        T time_constant{};

        // anti windup parameters
        T control_gain{};
        T saturation{};

        // basic PID internal state
        T prev_error{};
        T error_integral{};
        T error_derivative{};

        // anti windup internal state
        T sat_error{};
        T prev_sat_error{};
        T sat_error_integral{};
    };

}; // namespace Utility

#endif // PID_HPP
