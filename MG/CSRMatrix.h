// Initially generated by ChatGPT from OpenAI as of August 3, 2023.
// Modified to conform to this project's coding standards.
// For additional context, visit https://openai.com/research/chatgpt

#pragma once

#include <vector>

// CSR Matrix structure
struct CSRMatrix {
  std::vector<size_t> Offsets;
  std::vector<size_t> ColIdx;
  std::vector<double> Values;

  size_t Rows, Cols;

  // Constructor to initialize dimensions
  CSRMatrix(size_t R, size_t C): Rows(R), Cols(C) {
    Offsets.resize(Rows + 1, 0); // Initialize Offsets with zeros
  }

  // Function to set a value at (R, C)
  void setValue(size_t R, size_t C, double Value) {

    // First, let's find the appropriate position in the ColIdx array for the new value
    size_t Start = Offsets[R];
    size_t End = Offsets[R + 1];

    // Check if the column already exists
    for (size_t I = Start; I < End; ++I) {
      if (ColIdx[I] == C) {
        Values[I] = Value; // Update the value
        return;
      }
    }

    // If the column doesn't exist, we insert a new value
    // First, make room in the Values and ColIdx arrays
    Values.insert(Values.begin() + End, Value);
    ColIdx.insert(ColIdx.begin() + End, C);

    // Update the Offsets array
    for (size_t I = R + 1; I <= Rows; ++I) {
      Offsets[I]++;
    }
  }

  // Function to get a value at (row, col)
  double getValue(size_t R, size_t C) const {
    // Search for the column index between Offsets[row] and Offsets[row+1] in ColIdx
    for (size_t I = Offsets[R]; I < Offsets[R + 1]; ++I) {
      if (ColIdx[I] == C) {
        return Values[I];
      }
    }
    return 0.0; // Return zero if the entry is not explicitly stored
  }

  // Convert to Dense
  std::vector<std::vector<double>> toDense(void) const {

    std::vector<std::vector<double>> Dense(Rows, std::vector<double>(Cols, 0.0));

    for (int R = 0; R < Rows; ++R) {
      for (size_t Idx = Offsets[R]; Idx < Offsets[R + 1]; ++Idx) {
        size_t Col = ColIdx[Idx];
        Dense[R][Col] = Values[Idx];
      }
    }

    return Dense;
  }

};