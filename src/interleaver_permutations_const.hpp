#pragma once

// This file contains the interleaving patterns of the various supported modes. For the
// most part, they are copied from existing projects. Since everyone implements the
// de-interleaving somewhat differently, there is no common structure in these arrays.
// This could easily be addressed but somehow we cannot be bothered... Thus we suggest
// to refer to the codecserver-softmbe project and softmbesession.cpp in particular
// to learn how to de-interleave the MBE frames.

// https://github.com/HB9UF/gr-ysf/issues/12
const int ysf_dn_decoder_interleave_permutation[49] =
{
    0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 41, 43, 45, 47,
    1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 42, 44, 46, 48,
    2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38,
};

// This was reverse enginerred by S. Munaut and M. Weyland
// https://github.com/HB9UF/gr-ysf/blob/master/lib/fr_vch_decoder_bb_impl.cc#L111
const int ysf_vw_decoder_interleave_permutation[144] = {
      0,   7,  12,  19,  24,  31,  36,  43,  48,  55,  60,  67, // [  0 -  11] yellow message
     72,  79,  84,  91,  96, 103, 108, 115, 120, 127, 132,      // [ 12 -  22] yellow FEC
    139,   1,   6,  13,  18,  25,  30,  37,  42,  49,  54,  61, // [ 23 -  34] orange message
     66,  73,  78,  85,  90,  97, 102, 109, 114, 121, 126,      // [ 35 -  45] orange FEC
    133, 138,   2,   9,  14,  21,  26,  33,  38,  45,  50,  57, // [ 46 -  57] red message
     62,  69,  74,  81,  86,  93,  98, 105, 110, 117, 122,      // [ 58 -  68] red FEC
    129, 134, 141,   3,   8,  15,  20,  27,  32,  39,  44,  51, // [ 69 -  80] pink message
     56,  63,  68,  75,  80,  87,  92,  99, 104, 111, 116,      // [ 81 -  91] pink FEC
    123, 128, 135, 140,   4,  11,  16,  23,  28,  35,  40,      // [ 92 - 102] dark blue message
     47,  52,  59,  64,                                         // [103 - 106] dark blue FEC
     71,  76,  83,  88,  95, 100, 107, 112, 119, 124, 131,      // [107 - 117] light blue message
    136, 143,   5,  10,                                         // [118 - 121] light blue FEC
     17,  22,  29,  34,  41,  46,  53,  58,  65,  70,  77,      // [122 - 132] green message
     82,  89,  94, 101,                                         // [133 - 136] green FEC
    106, 113, 118, 125, 130, 137, 142,                          // [137 - 143] unprotected
};

// Source: https://github.com/szechyjs/dsd/blob/master/include/dstar_const.h
// Note that this is assuming that the frame is unpacked LSB first
const int dstar_decoder_vector_permutation[72] =
{
    // 0-11
    0, 0,
    3, 2,
    1, 1,
    0, 0,
    1, 1,
    0, 0,

    // 12-23
    3, 2,
    1, 1,
    3, 2,
    1, 1,
    0, 0,
    3, 2,

    // 24-35
    0, 0,
    3, 2,
    1, 1,
    0, 0,
    1, 1,
    0, 0,

    // 36-47
    3, 2,
    1, 1,
    3, 2,
    1, 1,
    0, 0,
    3, 2,

            // 48-59
    0, 0,
    3, 2,
    1, 1,
    0, 0,
    1, 1,
    0, 0,

    // 60-71
    3, 2,
    1, 1,
    3, 3,
    2, 1,
    0, 0,
    3, 3,
};

const int dstar_decoder_bit_permutation[72] =
{
    // 0-11
    10, 22,
    11, 9,
    10, 22,
    11, 23,
    8, 20,
    9, 21,

    // 12-23
    10, 8,
    9, 21,
    8, 6,
    7, 19,
    8, 20,
    9, 7,

    // 24-35
    6, 18,
    7, 5,
    6, 18,
    7, 19,
    4, 16,
    5, 17,

    // 36-47
    6, 4,
    5, 17,
    4, 2,
    3, 15,
    4, 16,
    5, 3,

    // 48-59
    2, 14,
    3, 1,
    2, 14,
    3, 15,
    0, 12,
    1, 13,

    // 60-71
    2, 0,
    1, 13,
    0, 12,
    10, 11,
    0, 12,
    1, 13,
};

// Original source for this table:
// https://github.com/jketterl/digiham/blob/release-0.4/dmr/bitmappings.h
//
// We modified this to map a 72-bit table instead of the original
// "blown up" 96-bit table, and we computed the inverse mapping for
// consistency.
const int dmr_interleave_permutation[72] =
{
    21, 17, 13,  9,  5,  1, 68, 64,
    60, 56, 52, 48, 44, 40, 36, 32,
    28, 24, 20, 16, 12,  8,  4,  0,
    42, 38, 34, 30, 26, 22, 18, 14,
    10,  6,  2, 69, 65, 61, 57, 53,
    49, 45, 41, 37, 33, 29, 25, 15,
    11,  7,  3, 70, 66, 62, 58, 54,
    50, 46, 71, 67, 63, 59, 55, 51,
    47, 43, 39, 35, 31, 27, 23, 19,
};
