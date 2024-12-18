#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

namespace utils {

  /// @brief Read next, potentially multi-digit integer from the current position in `file`
  /// @tparam T Type of integer.
  /// @param file Input file.
  /// @param value Integer to read into.
  /// @return True if an integer was successfully read.
  ///
  /// False if end-of-file was reached first.
  template <typename T>
  bool read_next_integer(std::ifstream& file, T& value);

  /// @brief Read a rectangular grid from the current position in `file`.
  ///
  /// Add padding to each end along both axes. 
  /// @param file Input file.
  /// @param grid Output grid.
  /// @param nx_input Size of input grid along x.
  /// @param ny_input Size of input grid along y.
  /// @param padding Number of characters to pad along each axis.
  /// @param padding_char Padding character.
  void read_grid_2D(std::ifstream& file, std::vector<char>& grid, int nx_input, int ny_input, int padding, char padding_char);

  /// @brief Helper class for simple time measurements.
  class Timer {
  private:
    std::chrono::_V2::system_clock::time_point starting_time;
    std::chrono::_V2::system_clock::time_point ending_time;
  public:
    /// @brief Start the timer.
    void tic();

    /// @brief Measure elapsed time since starting.
    /// @return Time in milliseconds since the last call to `tic()`.
    double toc();

    /// @brief Get elapsed time between latest calls to `tic()` and `toc()`.
    /// @return Time in milliseconds between latest calls to `tic()` and `toc()`.
    double duration();

    /// @brief Print elapsed time between latest calls to `tic()` and `toc()`.
    /// @param indentation Level of indentation in console. Spaces per level: 2.
    void print(int indentation = 1);
  };
}