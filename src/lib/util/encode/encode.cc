// Copyright (C) 2024 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <exceptions/exceptions.h>
#include <exceptions/isc_assert.h>
#include <util/encode/encode.h>

#include <iostream>
#include <stdint.h>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace isc {
namespace util {
namespace encode {

std::string
encodeBaseN(const std::vector<uint8_t>& input, const char* digit_set, size_t bits_per_digit,
       size_t digits_per_group, const char pad_char) {
    std::string encoded_output;
    if (input.empty()) {
        return(encoded_output);
    }

    // Turn the input data into a "bit stream"
    /// @todo Can we devize a bit-stream class that can iterate over the input
    /// without copying it?  The weakness here is inbits could be rather large
    /// for long strings since it input size * 8 bytes.
    bool inbits[input.size() * 8];
    bool* inbit = &inbits[0];
    for (auto b : input) {
        for (auto i = 0; i < 8; i++) {
            bool val = b & 0x80;
            *inbit++ = val;
            b <<= 1;
        }
    }

    // Now encode the bit stream.
    int cnt = 0;
    int digit_idx = 0;
    auto inbit_end = inbit;
    inbit = &inbits[0];
    for (inbit = &inbits[0]; inbit != inbit_end; ++inbit) {
        if (cnt < bits_per_digit) {
            // Shift the index one to accomodate next bit.
            digit_idx <<= 1;
        } else {
            // Have a complete digit index, look it the digit and add it.
            encoded_output.push_back(digit_set[digit_idx]);
            digit_idx = 0;
            cnt = 0;
        }

        // Add the current bit to the digit index.
        digit_idx |= *inbit;
        ++cnt;
    }

    // We've exhausted bits, but have left over
    if (cnt) {
        digit_idx <<= (bits_per_digit - cnt);
        encoded_output.push_back(digit_set[digit_idx]);
    }

    // Add padding as needed.
    if (digits_per_group) {
        auto rem = encoded_output.size() % digits_per_group;
        if (rem) {
            auto need = digits_per_group - rem + 1;
            while (--need) {
                encoded_output.push_back(pad_char);
            }
        }
    }

    return(encoded_output);
}

void
decodeBaseN(const std::string& algorithm,
            const std::string& encoded_str, std::vector<uint8_t>& output,
            const uint8_t* lookup_table,
            size_t bits_per_digit,
            size_t digits_per_group,
            const char pad_char,
            size_t max_pad) {

    output.clear();
    bool inbits[encoded_str.size() * bits_per_digit];
    bool* inbit = &inbits[0];
    size_t dig_cnt = 0;
    size_t pad_cnt = 0;
    size_t shift_bits = 8 - bits_per_digit;
    for (const auto enc_digit : encoded_str) {
        if (pad_char && enc_digit == pad_char) {
           pad_cnt++;
           continue;
        }

        // translate the b64 digit to bits.
        uint8_t dig_bits = lookup_table[static_cast<uint8_t>(enc_digit)];

        if (dig_bits == 0xee) {
            // skip whitespace
            continue;
        }

        if (dig_bits == 0xff) {
            isc_throw(isc::BadValue, "attempt to decode a value not in "
                      << algorithm << " char set" << ": " << encoded_str);
        }

        if (pad_cnt) {
            isc_throw(isc::BadValue, "pad mixed with digits in "
                      << algorithm << ": " << encoded_str);
        }

        dig_cnt++;
        dig_bits <<= shift_bits;
        for (auto i = 0; i < bits_per_digit; ++i) {
            *inbit++ = ((dig_bits & 0x80) == 0x80);
            dig_bits <<= 1;
        }
    }

    if (pad_char) {
        // Check for invalid number of pad characters.
        if (pad_cnt > max_pad) {
            isc_throw(isc::BadValue, "too many pad characters for "
                      << algorithm << ": " << encoded_str);
        }

        // Check for invalid number of pad characters.
        /// @todo is this valid
        const size_t padbits = ((pad_cnt * bits_per_digit) + 7) & ~7;
        if (padbits > bits_per_digit * (pad_cnt + 1)) {
            isc_throw(isc::BadValue, "Invalid padding for "
                      << algorithm << ": " << encoded_str);
        }
    }

    // Check for an invalid total of encoded characters.
    if ((pad_cnt + dig_cnt) % digits_per_group) {
        isc_throw (isc::BadValue, "Incomplete input for "
                   << algorithm << ": " << encoded_str);
    }

    int cnt = 0;
    int digit_idx = 0;

    auto inbit_end = inbit;
    inbit = &inbits[0];
    for (inbit = &inbits[0]; inbit != inbit_end; ++inbit) {
        if (cnt < 8) {
            digit_idx <<= 1;
        } else {
            output.push_back(digit_idx);
            digit_idx = 0;
            cnt = 0;
        }

        digit_idx |= *inbit;
        ++cnt;
    }

    if (cnt == 8) {
        // Whole one left to add.
        output.push_back(digit_idx);
    } else if (cnt && digit_idx) {
        // Left over bits that are not zero.
        isc_throw(BadValue, "non-zero bits left over " << encoded_str);
    }
}


string
encodeBase64(const vector<uint8_t>& binary) {
    static char B64_DIG[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz"
                            "0123456789"
                             "+/";
    return(encodeBaseN(binary, B64_DIG, 6, 4, '='));
}

void
decodeBase64 (const std::string& encoded_str, std::vector<uint8_t>& output) {
    static const uint8_t lookup_table[] = {
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xee,0xee,0xee,0xee,0xee,0xff,0xff, // 00-0f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 10-1f
        0xee,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,62,0xff,0xff,0xff,63,     // 20-2f
        52,53,54,55,56,57,58,59,60,61,0xff,0xff,0xff, 0,0xff,0xff,                       // 30-3f
        0xff, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,                               // 40-4f
        15,16,17,18,19,20,21,22,23,24,25,0xff,0xff,0xff,0xff,0xff,                       // 50-5f
        0xff,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,                               // 60-6f
        41,42,43,44,45,46,47,48,49,50,51,0xff,0xff,0xff,0xff,0xff,                       // 70-7f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 80-8f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 90-9f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // a0-af
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // b0-bf
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // c0-cf
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // d0-df
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // e0-ef
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff  // f0-ff
    };

   decodeBaseN("base64", encoded_str, output, lookup_table, 6, 4, '=', 2);
}

string
encodeBase32Hex(const vector<uint8_t>& binary) {
    static char B32_DIG[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    return(encodeBaseN(binary, B32_DIG, 5, 8, '='));
}

void
decodeBase32Hex(const std::string& encoded_str, std::vector<uint8_t>& output) {
    static const uint8_t lookup_table[] = {
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xee,0xee,0xee,0xee,0xee,0xff,0xff, // 00-0f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 10-1f
        0xee,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 20-2f
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0xff,0xff,0xff,0xff,0xff,0xff,                      // 30-3f
        0xff,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,                               // 40-4f
        25,26,27,28,29,30,31,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,               // 50-5f
        0xff,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,                               // 60-6f
        25,26,27,28,29,30,31,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,               // 70-7f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 80-8f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 90-9f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // a0-af
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // b0-bf
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // c0-cf
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // d0-df
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // e0-ef
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff  // f0-ff
    };

   decodeBaseN("base32hex", encoded_str, output, lookup_table, 5, 8, '=', 6);
}

string
encodeHex(const vector<uint8_t>& binary) {
    static char B16_DIG[] = "0123456789ABCDEF";
    return(encodeBaseN(binary, B16_DIG, 4, 1, 0));
}

void
decodeHex(const string& encoded_str, vector<uint8_t>& output) {
    static const uint8_t lookup_table[] = {
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xee,0xee,0xee,0xee,0xee,0xff,0xff, // 00-0f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 10-1f
        0xee,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 20-2f
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0xff,0xff,0xff,0xff,0xff,0xff,                      // 30-3f
        0xff,10,11,12,13,14,15,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,             // 40-4f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 50-5f
        0xff,10,11,12,13,14,15,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,             // 60-6f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 70-7f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 80-8f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // 90-9f
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // a0-af
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // b0-bf
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // c0-cf
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // d0-df
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, // e0-ef
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff  // f0-ff
    };

   decodeBaseN("base16", encoded_str, output, lookup_table, 4, 1, 0, 0);
}

} // namespace encode
} // namespace util
} // namespace isc
