#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "status_macros.h"
#include "absl/strings/string_view.h"
#include "absl/strings/str_cat.h"

DEFINE_string(input_file, "", "File to parse");
DEFINE_string(output_file, "", "File to write result to");

namespace {

bool IsHexValue(const char c) {
  return c == '0' ||
    c == '1' ||
    c == '2' ||
    c == '3' ||
    c == '4' ||
    c == '5' ||
    c == '6' ||
    c == '7' ||
    c == '8' ||
    c == '9' ||
    c == 'a' ||
    c == 'b' ||
    c == 'c' ||
    c == 'd' ||
    c == 'e' ||
    c == 'f';
}

absl::StatusOr<uint8_t> AsciiHexToInt(const char c) {
  switch (c) {
   case '0':
    return 0;
   case '1':
    return 1;
   case '2':
    return 2;
   case '3':
    return 3;
   case '4':
    return 4;
   case '5':
    return 5;
   case '6':
    return 6;
   case '7':
    return 7;
   case '8':
    return 8;
   case '9':
    return 9;
   case 'a':
    return 10;
   case 'b':
    return 11;
   case 'c':
    return 12;
   case 'd':
    return 13;
   case 'e':
    return 14;
   case 'f':
    return 15;
   default:
    return absl::InternalError(absl::StrCat("non hexademical value '", (int)c, "' found in ascii conversion"));
  }
}

absl::Status StoreWordInBuffer(std::vector<uint8_t>& buf, const std::string& ascii_str) {
  if (ascii_str.length() == 0) {
    return absl::OkStatus();
  }
  if (ascii_str.length() != 8) {
    return absl::InternalError("ascii string is not a word-size instr");
  }

  for (int i = 7; i >= 0; --i) {
    char c = ascii_str[i];
    ASSIGN_OR_RETURN(const uint8_t as_byte, AsciiHexToInt(c));
    buf.push_back(as_byte);
  }
  return absl::OkStatus();
}

absl::StatusOr<std::string> ConsumeWord(std::ifstream& input) {
  if (!input.is_open()) {
    return absl::InternalError("input not open");
  }
  if (input.eof()) {
    return std::string("");
  }

  char c[8 + 1];
  c[4] = '\0';
  if(!input.get(c, 8 + 1)) {
    return absl::InternalError("Reached EOF while reading instruction");
  }
  LOG(INFO) << "Ate word: " << c;
  return std::string(c);
}

absl::Status ConsumeWhitespace(std::ifstream& input) {
  if (!input.is_open()) {
    return absl::InternalError("input not open");
  }

  char c;
  char peek;
  while (!input.eof()) {
    input.get(c);
    switch (c) {
     case '\n':
     case ' ':
     case '\t':
      break;
     case '0':
      // Peek ahead for 'x'
      if (input.eof()) {
        return absl::InternalError("Reached EOF while parsing whitespace");
      }
      peek = input.peek();
      LOG(INFO) << "Peeking at: " << peek;
      if (IsHexValue(peek)) {
        input.putback(c);
        return absl::OkStatus();
      }
      if (peek == 'x') {
        input.get(c);
        return absl::OkStatus();
      }
      return absl::InternalError("Invalid character after 'x' token");
    case '#':  // Comment symbol
      while (!input.eof() && c != '\n') {
        input.get(c);
      }
      break;
    default:
      if (!IsHexValue(c)) {
        return absl::InternalError(absl::StrCat("Non-hexademical value '", (int)c, "' found"));
      }
      input.putback(c);
      return absl::OkStatus();
    }
  }
  return absl::OkStatus();
}

absl::StatusOr<std::unique_ptr<std::vector<uint8_t>>> Parse(absl::string_view infile) {
  // Open the input file for reading in text mode.
  std::ifstream input(infile.data(), std::ios::in);
  if (!input.is_open()) {
    return absl::NotFoundError(absl::StrCat("Input file '", infile, 
      "' could not be opened"));
  }

  std::unique_ptr<std::vector<uint8_t>> data { new std::vector<uint8_t>() };

  while (!input.eof()) {
    // Consume any whitespace, newlines, tabs, or '0x'.
    RETURN_IF_ERROR(ConsumeWhitespace(input));
    ASSIGN_OR_RETURN(const std::string word, ConsumeWord(input));
    RETURN_IF_ERROR(StoreWordInBuffer(*data, word));
    LOG(INFO) << "J";
  }

  LOG(INFO) << "Successfully parsed file";
  for (uint8_t byte : *data) {
    LOG(INFO) << "Byte: 0x" << std::hex << (uint32_t)byte;
  }
  input.close();
  return data;
}

absl::Status WriteToFile(std::vector<uint8_t>& buf, absl::string_view outfile) {
  // Open the output file for writing in binary mode.
  std::ofstream output(outfile.data(), std::ios::out | std::ios::binary);
  if (!output.is_open()) {
    return absl::NotFoundError(absl::StrCat("Output file '", outfile, 
      "' could not be opened"));
  }
  if (buf.size() == 0) {
     output.close();
    return absl::OkStatus();
  }
  if (buf.size() % 2 != 0) {
     output.close();
    return absl::InternalError("not enough bytes to form a valid instruction");
  }
  for (int i = 0; i < buf.size(); i += 2) {
    // Coallecse the first two bytes.
    const uint8_t lower = buf[i];
    // LOG(INFO) << "Lower byte: " << std::hex << (uint32_t)lower;
    const uint8_t upper = buf[i + 1];
    // LOG(INFO) << "Upper byte: " << std::hex << (uint32_t)upper;
    const uint8_t combined = (upper << 4) | lower;
    LOG(INFO) << "Writing " << std::hex << (uint32_t)combined << " to file";
    output << combined;
  }
  output.close();
  return absl::OkStatus();

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
  absl::Status write_result = WriteToFile(*(parse_result.value()), FLAGS_output_file);
  if (!write_result.ok()) {
    LOG(ERROR) << write_result;
    return EXIT_FAILURE;
  }
  LOG(INFO) << "Finished writing data to file";
  return EXIT_SUCCESS;
}
