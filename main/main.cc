#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "absl/strings/str_cat.h"

DEFINE_string(input_file, "", "File to parse");
DEFINE_string(output_file, "", "File to write result to");

namespace {

absl::StatusOr<std::unique_ptr<std::vector<uint8_t>>> Parse(absl::string_view infile) {
  // Open the input file for reading in text mode.
  std::ifstream input(infile.data(), std::ios::in);
  if (!input.is_open()) {
    return absl::NotFoundError(absl::StrCat("Input file '", infile, 
      "' could not be opened"));
  }
  std::unique_ptr<std::vector<uint8_t>> data { new std::vector<uint8_t>() };

  

  input.close();
  return data;
}

}  // namespace

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (FLAGS_input_file.empty()) {
    LOG(ERROR) << "Please enter a valid input file path";
    return EXIT_FAILURE;
  }
  if (FLAGS_output_file.empty()) {
    LOG(ERROR) << "Please enter a output valid file path";
    return EXIT_FAILURE;
  }

  LOG(INFO) << "Parsing input file '" << FLAGS_input_file << "'";
  absl::StatusOr<std::unique_ptr<std::vector<uint8_t>>> parse_result = Parse(FLAGS_input_file);
  if (!parse_result.ok()) {
    LOG(ERROR) << parse_result.status();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
