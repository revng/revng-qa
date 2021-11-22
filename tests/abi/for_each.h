#pragma once

/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

/* clang-format off */

#define FE_0(ACTION) ACTION()
#define FE_1(ACTION, X) ACTION(X)
#define FE_2(ACTION, X, ...) ACTION(X) FE_1(ACTION, __VA_ARGS__)
#define FE_3(ACTION, X, ...) ACTION(X) FE_2(ACTION, __VA_ARGS__)
#define FE_4(ACTION, X, ...) ACTION(X) FE_3(ACTION, __VA_ARGS__)
#define FE_5(ACTION, X, ...) ACTION(X) FE_4(ACTION, __VA_ARGS__)
#define FE_6(ACTION, X, ...) ACTION(X) FE_5(ACTION, __VA_ARGS__)
#define FE_7(ACTION, X, ...) ACTION(X) FE_6(ACTION, __VA_ARGS__)
#define FE_8(ACTION, X, ...) ACTION(X) FE_7(ACTION, __VA_ARGS__)
#define FE_9(ACTION, X, ...) ACTION(X) FE_8(ACTION, __VA_ARGS__)
#define FE_10(ACTION, X, ...) ACTION(X) FE_9(ACTION, __VA_ARGS__)
#define FE_11(ACTION, X, ...) ACTION(X) FE_10(ACTION, __VA_ARGS__)
#define FE_12(ACTION, X, ...) ACTION(X) FE_11(ACTION, __VA_ARGS__)
#define FE_13(ACTION, X, ...) ACTION(X) FE_12(ACTION, __VA_ARGS__)
#define FE_14(ACTION, X, ...) ACTION(X) FE_13(ACTION, __VA_ARGS__)
#define FE_15(ACTION, X, ...) ACTION(X) FE_14(ACTION, __VA_ARGS__)
#define FE_16(ACTION, X, ...) ACTION(X) FE_15(ACTION, __VA_ARGS__)
#define FE_17(ACTION, X, ...) ACTION(X) FE_16(ACTION, __VA_ARGS__)
#define FE_18(ACTION, X, ...) ACTION(X) FE_17(ACTION, __VA_ARGS__)
#define FE_19(ACTION, X, ...) ACTION(X) FE_18(ACTION, __VA_ARGS__)
#define FE_20(ACTION, X, ...) ACTION(X) FE_19(ACTION, __VA_ARGS__)
#define FE_21(ACTION, X, ...) ACTION(X) FE_20(ACTION, __VA_ARGS__)
#define FE_22(ACTION, X, ...) ACTION(X) FE_21(ACTION, __VA_ARGS__)
#define FE_23(ACTION, X, ...) ACTION(X) FE_22(ACTION, __VA_ARGS__)
#define FE_24(ACTION, X, ...) ACTION(X) FE_23(ACTION, __VA_ARGS__)
#define FE_25(ACTION, X, ...) ACTION(X) FE_24(ACTION, __VA_ARGS__)
#define FE_26(ACTION, X, ...) ACTION(X) FE_25(ACTION, __VA_ARGS__)
#define FE_27(ACTION, X, ...) ACTION(X) FE_26(ACTION, __VA_ARGS__)
#define FE_28(ACTION, X, ...) ACTION(X) FE_27(ACTION, __VA_ARGS__)
#define FE_29(ACTION, X, ...) ACTION(X) FE_28(ACTION, __VA_ARGS__)
#define FE_30(ACTION, X, ...) ACTION(X) FE_29(ACTION, __VA_ARGS__)
#define FE_31(ACTION, X, ...) ACTION(X) FE_30(ACTION, __VA_ARGS__)
#define FE_32(ACTION, X, ...) ACTION(X) FE_31(ACTION, __VA_ARGS__)

#define RFE_0(ACTION) ACTION()
#define RFE_1(ACTION, X) ACTION(X)
#define RFE_2(ACTION, X, ...) RFE_1(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_3(ACTION, X, ...) RFE_2(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_4(ACTION, X, ...) RFE_3(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_5(ACTION, X, ...) RFE_4(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_6(ACTION, X, ...) RFE_5(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_7(ACTION, X, ...) RFE_6(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_8(ACTION, X, ...) RFE_7(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_9(ACTION, X, ...) RFE_8(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_10(ACTION, X, ...) RFE_9(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_11(ACTION, X, ...) RFE_10(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_12(ACTION, X, ...) RFE_11(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_13(ACTION, X, ...) RFE_12(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_14(ACTION, X, ...) RFE_13(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_15(ACTION, X, ...) RFE_14(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_16(ACTION, X, ...) RFE_15(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_17(ACTION, X, ...) RFE_16(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_18(ACTION, X, ...) RFE_17(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_19(ACTION, X, ...) RFE_18(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_20(ACTION, X, ...) RFE_19(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_21(ACTION, X, ...) RFE_20(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_22(ACTION, X, ...) RFE_21(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_23(ACTION, X, ...) RFE_22(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_24(ACTION, X, ...) RFE_23(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_25(ACTION, X, ...) RFE_24(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_26(ACTION, X, ...) RFE_25(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_27(ACTION, X, ...) RFE_26(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_28(ACTION, X, ...) RFE_27(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_29(ACTION, X, ...) RFE_28(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_30(ACTION, X, ...) RFE_29(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_31(ACTION, X, ...) RFE_30(ACTION, __VA_ARGS__) ACTION(X)
#define RFE_32(ACTION, X, ...) RFE_31(ACTION, __VA_ARGS__) ACTION(X)

#define CFE_0(ACTION) ACTION()
#define CFE_1(ACTION, X) ACTION(X)
#define CFE_2(ACTION, X, ...) ACTION(X), CFE_1(ACTION, __VA_ARGS__)
#define CFE_3(ACTION, X, ...) ACTION(X), CFE_2(ACTION, __VA_ARGS__)
#define CFE_4(ACTION, X, ...) ACTION(X), CFE_3(ACTION, __VA_ARGS__)
#define CFE_5(ACTION, X, ...) ACTION(X), CFE_4(ACTION, __VA_ARGS__)
#define CFE_6(ACTION, X, ...) ACTION(X), CFE_5(ACTION, __VA_ARGS__)
#define CFE_7(ACTION, X, ...) ACTION(X), CFE_6(ACTION, __VA_ARGS__)
#define CFE_8(ACTION, X, ...) ACTION(X), CFE_7(ACTION, __VA_ARGS__)
#define CFE_9(ACTION, X, ...) ACTION(X), CFE_8(ACTION, __VA_ARGS__)
#define CFE_10(ACTION, X, ...) ACTION(X), CFE_9(ACTION, __VA_ARGS__)
#define CFE_11(ACTION, X, ...) ACTION(X), CFE_10(ACTION, __VA_ARGS__)
#define CFE_12(ACTION, X, ...) ACTION(X), CFE_11(ACTION, __VA_ARGS__)
#define CFE_13(ACTION, X, ...) ACTION(X), CFE_12(ACTION, __VA_ARGS__)
#define CFE_14(ACTION, X, ...) ACTION(X), CFE_13(ACTION, __VA_ARGS__)
#define CFE_15(ACTION, X, ...) ACTION(X), CFE_14(ACTION, __VA_ARGS__)
#define CFE_16(ACTION, X, ...) ACTION(X), CFE_15(ACTION, __VA_ARGS__)
#define CFE_17(ACTION, X, ...) ACTION(X), CFE_16(ACTION, __VA_ARGS__)
#define CFE_18(ACTION, X, ...) ACTION(X), CFE_17(ACTION, __VA_ARGS__)
#define CFE_19(ACTION, X, ...) ACTION(X), CFE_18(ACTION, __VA_ARGS__)
#define CFE_20(ACTION, X, ...) ACTION(X), CFE_19(ACTION, __VA_ARGS__)
#define CFE_21(ACTION, X, ...) ACTION(X), CFE_20(ACTION, __VA_ARGS__)
#define CFE_22(ACTION, X, ...) ACTION(X), CFE_21(ACTION, __VA_ARGS__)
#define CFE_23(ACTION, X, ...) ACTION(X), CFE_22(ACTION, __VA_ARGS__)
#define CFE_24(ACTION, X, ...) ACTION(X), CFE_23(ACTION, __VA_ARGS__)
#define CFE_25(ACTION, X, ...) ACTION(X), CFE_24(ACTION, __VA_ARGS__)
#define CFE_26(ACTION, X, ...) ACTION(X), CFE_25(ACTION, __VA_ARGS__)
#define CFE_27(ACTION, X, ...) ACTION(X), CFE_26(ACTION, __VA_ARGS__)
#define CFE_28(ACTION, X, ...) ACTION(X), CFE_27(ACTION, __VA_ARGS__)
#define CFE_29(ACTION, X, ...) ACTION(X), CFE_28(ACTION, __VA_ARGS__)
#define CFE_30(ACTION, X, ...) ACTION(X), CFE_29(ACTION, __VA_ARGS__)
#define CFE_31(ACTION, X, ...) ACTION(X), CFE_30(ACTION, __VA_ARGS__)
#define CFE_32(ACTION, X, ...) ACTION(X), CFE_31(ACTION, __VA_ARGS__)

#define SFE_0(ACTION) ACTION()
#define SFE_1(ACTION, X) ACTION(X)
#define SFE_2(ACTION, X, ...) ACTION(X); SFE_1(ACTION, __VA_ARGS__)
#define SFE_3(ACTION, X, ...) ACTION(X); SFE_2(ACTION, __VA_ARGS__)
#define SFE_4(ACTION, X, ...) ACTION(X); SFE_3(ACTION, __VA_ARGS__)
#define SFE_5(ACTION, X, ...) ACTION(X); SFE_4(ACTION, __VA_ARGS__)
#define SFE_6(ACTION, X, ...) ACTION(X); SFE_5(ACTION, __VA_ARGS__)
#define SFE_7(ACTION, X, ...) ACTION(X); SFE_6(ACTION, __VA_ARGS__)
#define SFE_8(ACTION, X, ...) ACTION(X); SFE_7(ACTION, __VA_ARGS__)
#define SFE_9(ACTION, X, ...) ACTION(X); SFE_8(ACTION, __VA_ARGS__)
#define SFE_10(ACTION, X, ...) ACTION(X); SFE_9(ACTION, __VA_ARGS__)
#define SFE_11(ACTION, X, ...) ACTION(X); SFE_10(ACTION, __VA_ARGS__)
#define SFE_12(ACTION, X, ...) ACTION(X); SFE_11(ACTION, __VA_ARGS__)
#define SFE_13(ACTION, X, ...) ACTION(X); SFE_12(ACTION, __VA_ARGS__)
#define SFE_14(ACTION, X, ...) ACTION(X); SFE_13(ACTION, __VA_ARGS__)
#define SFE_15(ACTION, X, ...) ACTION(X); SFE_14(ACTION, __VA_ARGS__)
#define SFE_16(ACTION, X, ...) ACTION(X); SFE_15(ACTION, __VA_ARGS__)
#define SFE_17(ACTION, X, ...) ACTION(X); SFE_16(ACTION, __VA_ARGS__)
#define SFE_18(ACTION, X, ...) ACTION(X); SFE_17(ACTION, __VA_ARGS__)
#define SFE_19(ACTION, X, ...) ACTION(X); SFE_18(ACTION, __VA_ARGS__)
#define SFE_20(ACTION, X, ...) ACTION(X); SFE_19(ACTION, __VA_ARGS__)
#define SFE_21(ACTION, X, ...) ACTION(X); SFE_20(ACTION, __VA_ARGS__)
#define SFE_22(ACTION, X, ...) ACTION(X); SFE_21(ACTION, __VA_ARGS__)
#define SFE_23(ACTION, X, ...) ACTION(X); SFE_22(ACTION, __VA_ARGS__)
#define SFE_24(ACTION, X, ...) ACTION(X); SFE_23(ACTION, __VA_ARGS__)
#define SFE_25(ACTION, X, ...) ACTION(X); SFE_24(ACTION, __VA_ARGS__)
#define SFE_26(ACTION, X, ...) ACTION(X); SFE_25(ACTION, __VA_ARGS__)
#define SFE_27(ACTION, X, ...) ACTION(X); SFE_26(ACTION, __VA_ARGS__)
#define SFE_28(ACTION, X, ...) ACTION(X); SFE_27(ACTION, __VA_ARGS__)
#define SFE_29(ACTION, X, ...) ACTION(X); SFE_28(ACTION, __VA_ARGS__)
#define SFE_30(ACTION, X, ...) ACTION(X); SFE_29(ACTION, __VA_ARGS__)
#define SFE_31(ACTION, X, ...) ACTION(X); SFE_30(ACTION, __VA_ARGS__)
#define SFE_32(ACTION, X, ...) ACTION(X); SFE_31(ACTION, __VA_ARGS__)

#define IFE_0(ACTION) ACTION()
#define IFE_1(ACTION, X) ACTION(X, 0)
#define IFE_2(ACTION, X, ...) ACTION(X, 1) IFE_1(ACTION, __VA_ARGS__)
#define IFE_3(ACTION, X, ...) ACTION(X, 2) IFE_2(ACTION, __VA_ARGS__)
#define IFE_4(ACTION, X, ...) ACTION(X, 3) IFE_3(ACTION, __VA_ARGS__)
#define IFE_5(ACTION, X, ...) ACTION(X, 4) IFE_4(ACTION, __VA_ARGS__)
#define IFE_6(ACTION, X, ...) ACTION(X, 5) IFE_5(ACTION, __VA_ARGS__)
#define IFE_7(ACTION, X, ...) ACTION(X, 6) IFE_6(ACTION, __VA_ARGS__)
#define IFE_8(ACTION, X, ...) ACTION(X, 7) IFE_7(ACTION, __VA_ARGS__)
#define IFE_9(ACTION, X, ...) ACTION(X, 8) IFE_8(ACTION, __VA_ARGS__)
#define IFE_10(ACTION, X, ...) ACTION(X, 9) IFE_9(ACTION, __VA_ARGS__)
#define IFE_11(ACTION, X, ...) ACTION(X, 10) IFE_10(ACTION, __VA_ARGS__)
#define IFE_12(ACTION, X, ...) ACTION(X, 11) IFE_11(ACTION, __VA_ARGS__)
#define IFE_13(ACTION, X, ...) ACTION(X, 12) IFE_12(ACTION, __VA_ARGS__)
#define IFE_14(ACTION, X, ...) ACTION(X, 13) IFE_13(ACTION, __VA_ARGS__)
#define IFE_15(ACTION, X, ...) ACTION(X, 14) IFE_14(ACTION, __VA_ARGS__)
#define IFE_16(ACTION, X, ...) ACTION(X, 15) IFE_15(ACTION, __VA_ARGS__)
#define IFE_17(ACTION, X, ...) ACTION(X, 16) IFE_16(ACTION, __VA_ARGS__)
#define IFE_18(ACTION, X, ...) ACTION(X, 17) IFE_17(ACTION, __VA_ARGS__)
#define IFE_19(ACTION, X, ...) ACTION(X, 18) IFE_18(ACTION, __VA_ARGS__)
#define IFE_20(ACTION, X, ...) ACTION(X, 19) IFE_19(ACTION, __VA_ARGS__)
#define IFE_21(ACTION, X, ...) ACTION(X, 20) IFE_20(ACTION, __VA_ARGS__)
#define IFE_22(ACTION, X, ...) ACTION(X, 21) IFE_21(ACTION, __VA_ARGS__)
#define IFE_23(ACTION, X, ...) ACTION(X, 22) IFE_22(ACTION, __VA_ARGS__)
#define IFE_24(ACTION, X, ...) ACTION(X, 23) IFE_23(ACTION, __VA_ARGS__)
#define IFE_25(ACTION, X, ...) ACTION(X, 24) IFE_24(ACTION, __VA_ARGS__)
#define IFE_26(ACTION, X, ...) ACTION(X, 25) IFE_25(ACTION, __VA_ARGS__)
#define IFE_27(ACTION, X, ...) ACTION(X, 26) IFE_26(ACTION, __VA_ARGS__)
#define IFE_28(ACTION, X, ...) ACTION(X, 27) IFE_27(ACTION, __VA_ARGS__)
#define IFE_29(ACTION, X, ...) ACTION(X, 28) IFE_28(ACTION, __VA_ARGS__)
#define IFE_30(ACTION, X, ...) ACTION(X, 29) IFE_29(ACTION, __VA_ARGS__)
#define IFE_31(ACTION, X, ...) ACTION(X, 30) IFE_30(ACTION, __VA_ARGS__)
#define IFE_32(ACTION, X, ...) ACTION(X, 31) IFE_31(ACTION, __VA_ARGS__)

#define RPT_0(ACTION, ...)
#define RPT_1(ACTION, ...) ACTION(0, __VA_ARGS__)
#define RPT_2(ACTION, ...) RPT_1(ACTION, __VA_ARGS__) ACTION(1, __VA_ARGS__)
#define RPT_3(ACTION, ...) RPT_2(ACTION, __VA_ARGS__) ACTION(2, __VA_ARGS__)
#define RPT_4(ACTION, ...) RPT_3(ACTION, __VA_ARGS__) ACTION(3, __VA_ARGS__)
#define RPT_5(ACTION, ...) RPT_4(ACTION, __VA_ARGS__) ACTION(4, __VA_ARGS__)
#define RPT_6(ACTION, ...) RPT_5(ACTION, __VA_ARGS__) ACTION(5, __VA_ARGS__)
#define RPT_7(ACTION, ...) RPT_6(ACTION, __VA_ARGS__) ACTION(6, __VA_ARGS__)
#define RPT_8(ACTION, ...) RPT_7(ACTION, __VA_ARGS__) ACTION(7, __VA_ARGS__)
#define RPT_9(ACTION, ...) RPT_8(ACTION, __VA_ARGS__) ACTION(8, __VA_ARGS__)
#define RPT_10(ACTION, ...) RPT_9(ACTION, __VA_ARGS__) ACTION(9, __VA_ARGS__)
#define RPT_11(ACTION, ...) RPT_10(ACTION, __VA_ARGS__) ACTION(10, __VA_ARGS__)
#define RPT_12(ACTION, ...) RPT_11(ACTION, __VA_ARGS__) ACTION(11, __VA_ARGS__)
#define RPT_13(ACTION, ...) RPT_12(ACTION, __VA_ARGS__) ACTION(12, __VA_ARGS__)
#define RPT_14(ACTION, ...) RPT_13(ACTION, __VA_ARGS__) ACTION(13, __VA_ARGS__)
#define RPT_15(ACTION, ...) RPT_14(ACTION, __VA_ARGS__) ACTION(14, __VA_ARGS__)
#define RPT_16(ACTION, ...) RPT_15(ACTION, __VA_ARGS__) ACTION(15, __VA_ARGS__)
#define RPT_17(ACTION, ...) RPT_16(ACTION, __VA_ARGS__) ACTION(16, __VA_ARGS__)
#define RPT_18(ACTION, ...) RPT_17(ACTION, __VA_ARGS__) ACTION(17, __VA_ARGS__)
#define RPT_19(ACTION, ...) RPT_18(ACTION, __VA_ARGS__) ACTION(18, __VA_ARGS__)
#define RPT_20(ACTION, ...) RPT_19(ACTION, __VA_ARGS__) ACTION(19, __VA_ARGS__)
#define RPT_21(ACTION, ...) RPT_20(ACTION, __VA_ARGS__) ACTION(20, __VA_ARGS__)
#define RPT_22(ACTION, ...) RPT_21(ACTION, __VA_ARGS__) ACTION(21, __VA_ARGS__)
#define RPT_23(ACTION, ...) RPT_22(ACTION, __VA_ARGS__) ACTION(22, __VA_ARGS__)
#define RPT_24(ACTION, ...) RPT_23(ACTION, __VA_ARGS__) ACTION(23, __VA_ARGS__)
#define RPT_25(ACTION, ...) RPT_24(ACTION, __VA_ARGS__) ACTION(24, __VA_ARGS__)
#define RPT_26(ACTION, ...) RPT_25(ACTION, __VA_ARGS__) ACTION(25, __VA_ARGS__)
#define RPT_27(ACTION, ...) RPT_26(ACTION, __VA_ARGS__) ACTION(26, __VA_ARGS__)
#define RPT_28(ACTION, ...) RPT_27(ACTION, __VA_ARGS__) ACTION(27, __VA_ARGS__)
#define RPT_29(ACTION, ...) RPT_28(ACTION, __VA_ARGS__) ACTION(28, __VA_ARGS__)
#define RPT_30(ACTION, ...) RPT_29(ACTION, __VA_ARGS__) ACTION(29, __VA_ARGS__)
#define RPT_31(ACTION, ...) RPT_30(ACTION, __VA_ARGS__) ACTION(30, __VA_ARGS__)
#define RPT_32(ACTION, ...) RPT_31(ACTION, __VA_ARGS__) ACTION(31, __VA_ARGS__)

#define GET_34TH(_0, _1, _2, _3, _4, _5, _6, _7, \
                 _8, _9, _10, _11, _12, _13, _14, _15, \
                 _16, _17, _18, _19, _20, _21, _22, _23, \
                 _24, _25, _26, _27, _28, _29, _30, _31, \
                 _32, THIS_ONE, ...) THIS_ONE

/* Calls ACTION(VA_ARG) for each VA_ARG in ... */
#define FOR_EACH(ACTION, ...) \
  GET_34TH(_0, __VA_ARGS__, FE_32, \
           FE_31, FE_30, FE_29, FE_28, FE_27, FE_26, FE_25, FE_24, \
           FE_23, FE_22, FE_21, FE_20, FE_19, FE_18, FE_17, FE_16, \
           FE_15, FE_14, FE_13, FE_12, FE_11, FE_10, FE_9, FE_8, \
           FE_7, FE_6, FE_5, FE_4, FE_3, FE_2, FE_1, FE_0) \
    (ACTION, __VA_ARGS__)

/* Calls ACTION(VA_ARG) for each VA_ARG in ... in reverse order */
#define REVERSE_FOR_EACH(ACTION, ...) \
  GET_34TH(_0, __VA_ARGS__, RFE_32, \
           RFE_31, RFE_30, RFE_29, RFE_28, RFE_27, RFE_26, RFE_25, RFE_24, \
           RFE_23, RFE_22, RFE_21, RFE_20, RFE_19, RFE_18, RFE_17, RFE_16, \
           RFE_15, RFE_14, RFE_13, RFE_12, RFE_11, RFE_10, RFE_9, RFE_8, \
           RFE_7, RFE_6, RFE_5, RFE_4, RFE_3, RFE_2, RFE_1, RFE_0) \
    (ACTION, __VA_ARGS__)

/* Calls ACTION(VA_ARG) for each VA_ARG in ... and adds `, ` as a separator. */
#define COMMA_SEPARATED_FOR_EACH(ACTION, ...) \
  GET_34TH(_0, __VA_ARGS__, CFE_32, \
           CFE_31, CFE_30, CFE_29, CFE_28, CFE_27, CFE_26, CFE_25, CFE_24, \
           CFE_23, CFE_22, CFE_21, CFE_20, CFE_19, CFE_18, CFE_17, CFE_16, \
           CFE_15, CFE_14, CFE_13, CFE_12, CFE_11, CFE_10, CFE_9, CFE_8, \
           CFE_7, CFE_6, CFE_5, CFE_4, CFE_3, CFE_2, CFE_1, CFE_0) \
    (ACTION, __VA_ARGS__)

/* Calls ACTION(VA_ARG) for each VA_ARG in ... and adds `; ` as a separator. */
#define SEMICOLON_SEPARATED_FOR_EACH(ACTION, ...) \
  GET_34TH(_0, __VA_ARGS__, SFE_32, \
           SFE_31, SFE_30, SFE_29, SFE_28, SFE_27, SFE_26, SFE_25, SFE_24, \
           SFE_23, SFE_22, SFE_21, SFE_20, SFE_19, SFE_18, SFE_17, SFE_16, \
           SFE_15, SFE_14, SFE_13, SFE_12, SFE_11, SFE_10, SFE_9, SFE_8, \
           SFE_7, SFE_6, SFE_5, SFE_4, SFE_3, SFE_2, SFE_1, SFE_0) \
    (ACTION, __VA_ARGS__)

/* Calls ACTION(VA_ARG, INDEX) for each VA_ARG in ... */
#define INDEXED_FOR_EACH(ACTION, ...) \
  GET_34TH(_0, __VA_ARGS__, IFE_32, \
           IFE_31, IFE_30, IFE_29, IFE_28, IFE_27, IFE_26, IFE_25, IFE_24, \
           IFE_23, IFE_22, IFE_21, IFE_20, IFE_19, IFE_18, IFE_17, IFE_16, \
           IFE_15, IFE_14, IFE_13, IFE_12, IFE_11, IFE_10, IFE_9, IFE_8, \
           IFE_7, IFE_6, IFE_5, IFE_4, IFE_3, IFE_2, IFE_1, IFE_0) \
    (ACTION, __VA_ARGS__)

/* Calls ACTION(INDEX, __VA_ARGS__) COUNT times */
#define REPEAT(ACTION, COUNT, ...) \
  CONCATENATE(RPT_, COUNT)(ACTION, __VA_ARGS__)

/* Determines the number of arguments passed to the macro */
#define COUNT_ARGUMENTS(...) \
  GET_34TH(__VA_ARGS__, 33, \
           32, 31, 30, 29, 28, 27, 26, 25, \
           24, 23, 22, 21, 20, 19, 18, 17, \
           16, 15, 14, 13, 12, 11, 10, 9, \
           8, 7, 6, 5, 4, 3, 2, 1)

/* clang-format on */
