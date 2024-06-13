﻿/**
 * LUTs taken from the original firmware source code.
 * https://github.com/FrameworkComputer/inputmodule-rs
 **/

#include <stdint.h>

#include "is3741.h"

#if IS3741_USE_EVT_LUT
const uint16_t LUT_PIXEL[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT] = {
    0x3c01, // x: 9, y: 1, sw: 9, cs: 1, id:  9
    0x1e01, // x: 8, y: 1, sw: 8, cs: 1, id:  8
    0x0001, // x: 7, y: 1, sw: 7, cs: 1, id:  7
    0x9600, // x: 6, y: 1, sw: 6, cs: 1, id:  6
    0x7800, // x: 5, y: 1, sw: 5, cs: 1, id:  5
    0x5a00, // x: 4, y: 1, sw: 4, cs: 1, id:  4
    0x3c00, // x: 3, y: 1, sw: 3, cs: 1, id:  3
    0x1e00, // x: 2, y: 1, sw: 2, cs: 1, id:  2
    0x0000, // x: 1, y: 1, sw: 1, cs: 1, id:  1
    0x3d01, // x: 9, y: 2, sw: 9, cs: 2, id: 18
    0x1f01, // x: 8, y: 2, sw: 8, cs: 2, id: 17
    0x0101, // x: 7, y: 2, sw: 7, cs: 2, id: 16
    0x9700, // x: 6, y: 2, sw: 6, cs: 2, id: 15
    0x7900, // x: 5, y: 2, sw: 5, cs: 2, id: 14
    0x5b00, // x: 4, y: 2, sw: 4, cs: 2, id: 13
    0x3d00, // x: 3, y: 2, sw: 3, cs: 2, id: 12
    0x1f00, // x: 2, y: 2, sw: 2, cs: 2, id: 11
    0x0100, // x: 1, y: 2, sw: 1, cs: 2, id: 10
    0x3e01, // x: 9, y: 3, sw: 9, cs: 3, id: 27
    0x2001, // x: 8, y: 3, sw: 8, cs: 3, id: 26
    0x0201, // x: 7, y: 3, sw: 7, cs: 3, id: 25
    0x9800, // x: 6, y: 3, sw: 6, cs: 3, id: 24
    0x7a00, // x: 5, y: 3, sw: 5, cs: 3, id: 23
    0x5c00, // x: 4, y: 3, sw: 4, cs: 3, id: 22
    0x3e00, // x: 3, y: 3, sw: 3, cs: 3, id: 21
    0x2000, // x: 2, y: 3, sw: 2, cs: 3, id: 20
    0x0200, // x: 1, y: 3, sw: 1, cs: 3, id: 19
    0x3f01, // x: 9, y: 4, sw: 9, cs: 4, id: 36
    0x2101, // x: 8, y: 4, sw: 8, cs: 4, id: 35
    0x0301, // x: 7, y: 4, sw: 7, cs: 4, id: 34
    0x9900, // x: 6, y: 4, sw: 6, cs: 4, id: 33
    0x7b00, // x: 5, y: 4, sw: 5, cs: 4, id: 32
    0x5d00, // x: 4, y: 4, sw: 4, cs: 4, id: 31
    0x3f00, // x: 3, y: 4, sw: 3, cs: 4, id: 30
    0x2100, // x: 2, y: 4, sw: 2, cs: 4, id: 29
    0x0300, // x: 1, y: 4, sw: 1, cs: 4, id: 28
    0x4001, // x: 9, y: 5, sw: 9, cs: 5, id: 69
    0x2201, // x: 8, y: 5, sw: 8, cs: 5, id: 65
    0x0401, // x: 7, y: 5, sw: 7, cs: 5, id: 61
    0x9a00, // x: 6, y: 5, sw: 6, cs: 5, id: 57
    0x7c00, // x: 5, y: 5, sw: 5, cs: 5, id: 53
    0x5e00, // x: 4, y: 5, sw: 4, cs: 5, id: 49
    0x4000, // x: 3, y: 5, sw: 3, cs: 5, id: 45
    0x2200, // x: 2, y: 5, sw: 2, cs: 5, id: 41
    0x0400, // x: 1, y: 5, sw: 1, cs: 5, id: 37
    0x4101, // x: 9, y: 6, sw: 9, cs: 6, id: 70
    0x2301, // x: 8, y: 6, sw: 8, cs: 6, id: 66
    0x0501, // x: 7, y: 6, sw: 7, cs: 6, id: 62
    0x9b00, // x: 6, y: 6, sw: 6, cs: 6, id: 58
    0x7d00, // x: 5, y: 6, sw: 5, cs: 6, id: 54
    0x5f00, // x: 4, y: 6, sw: 4, cs: 6, id: 50
    0x4100, // x: 3, y: 6, sw: 3, cs: 6, id: 46
    0x2300, // x: 2, y: 6, sw: 2, cs: 6, id: 42
    0x0500, // x: 1, y: 6, sw: 1, cs: 6, id: 38
    0x4201, // x: 9, y: 7, sw: 9, cs: 7, id: 71
    0x2401, // x: 8, y: 7, sw: 8, cs: 7, id: 67
    0x0601, // x: 7, y: 7, sw: 7, cs: 7, id: 63
    0x9c00, // x: 6, y: 7, sw: 6, cs: 7, id: 59
    0x7e00, // x: 5, y: 7, sw: 5, cs: 7, id: 55
    0x6000, // x: 4, y: 7, sw: 4, cs: 7, id: 51
    0x4200, // x: 3, y: 7, sw: 3, cs: 7, id: 47
    0x2400, // x: 2, y: 7, sw: 2, cs: 7, id: 43
    0x0600, // x: 1, y: 7, sw: 1, cs: 7, id: 39
    0x4301, // x: 9, y: 8, sw: 9, cs: 8, id: 72
    0x2501, // x: 8, y: 8, sw: 8, cs: 8, id: 68
    0x0701, // x: 7, y: 8, sw: 7, cs: 8, id: 64
    0x9d00, // x: 6, y: 8, sw: 6, cs: 8, id: 60
    0x7f00, // x: 5, y: 8, sw: 5, cs: 8, id: 56
    0x6100, // x: 4, y: 8, sw: 4, cs: 8, id: 52
    0x4300, // x: 3, y: 8, sw: 3, cs: 8, id: 48
    0x2500, // x: 2, y: 8, sw: 2, cs: 8, id: 44
    0x0700, // x: 1, y: 8, sw: 1, cs: 8, id: 40
    0x4401, // x: 9, y: 9, sw: 9, cs: 9, id:137
    0x2601, // x: 8, y: 9, sw: 8, cs: 9, id:129
    0x0801, // x: 7, y: 9, sw: 7, cs: 9, id:121
    0x9e00, // x: 6, y: 9, sw: 6, cs: 9, id:113
    0x8000, // x: 5, y: 9, sw: 5, cs: 9, id:105
    0x6200, // x: 4, y: 9, sw: 4, cs: 9, id: 97
    0x4400, // x: 3, y: 9, sw: 3, cs: 9, id: 89
    0x2600, // x: 2, y: 9, sw: 2, cs: 9, id: 81
    0x0800, // x: 1, y: 9, sw: 1, cs: 9, id: 73
    0x4501, // x: 9, y:10, sw: 9, cs:10, id:138
    0x2701, // x: 8, y:10, sw: 8, cs:10, id:130
    0x0901, // x: 7, y:10, sw: 7, cs:10, id:122
    0x9f00, // x: 6, y:10, sw: 6, cs:10, id:114
    0x8100, // x: 5, y:10, sw: 5, cs:10, id:106
    0x6300, // x: 4, y:10, sw: 4, cs:10, id: 98
    0x4500, // x: 3, y:10, sw: 3, cs:10, id: 90
    0x2700, // x: 2, y:10, sw: 2, cs:10, id: 82
    0x0900, // x: 1, y:10, sw: 1, cs:10, id: 74
    0x4601, // x: 9, y:11, sw: 9, cs:11, id:139
    0x2801, // x: 8, y:11, sw: 8, cs:11, id:131
    0x0a01, // x: 7, y:11, sw: 7, cs:11, id:123
    0xa000, // x: 6, y:11, sw: 6, cs:11, id:115
    0x8200, // x: 5, y:11, sw: 5, cs:11, id:107
    0x6400, // x: 4, y:11, sw: 4, cs:11, id: 99
    0x4600, // x: 3, y:11, sw: 3, cs:11, id: 91
    0x2800, // x: 2, y:11, sw: 2, cs:11, id: 83
    0x0a00, // x: 1, y:11, sw: 1, cs:11, id: 75
    0x4701, // x: 9, y:12, sw: 9, cs:12, id:140
    0x2901, // x: 8, y:12, sw: 8, cs:12, id:132
    0x0b01, // x: 7, y:12, sw: 7, cs:12, id:124
    0xa100, // x: 6, y:12, sw: 6, cs:12, id:116
    0x8300, // x: 5, y:12, sw: 5, cs:12, id:108
    0x6500, // x: 4, y:12, sw: 4, cs:12, id:100
    0x4700, // x: 3, y:12, sw: 3, cs:12, id: 92
    0x2900, // x: 2, y:12, sw: 2, cs:12, id: 84
    0x0b00, // x: 1, y:12, sw: 1, cs:12, id: 76
    0x4801, // x: 9, y:13, sw: 9, cs:13, id:141
    0x2a01, // x: 8, y:13, sw: 8, cs:13, id:133
    0x0c01, // x: 7, y:13, sw: 7, cs:13, id:125
    0xa200, // x: 6, y:13, sw: 6, cs:13, id:117
    0x8400, // x: 5, y:13, sw: 5, cs:13, id:109
    0x6600, // x: 4, y:13, sw: 4, cs:13, id:101
    0x4800, // x: 3, y:13, sw: 3, cs:13, id: 93
    0x2a00, // x: 2, y:13, sw: 2, cs:13, id: 85
    0x0c00, // x: 1, y:13, sw: 1, cs:13, id: 77
    0x4901, // x: 9, y:14, sw: 9, cs:14, id:142
    0x2b01, // x: 8, y:14, sw: 8, cs:14, id:134
    0x0d01, // x: 7, y:14, sw: 7, cs:14, id:126
    0xa300, // x: 6, y:14, sw: 6, cs:14, id:118
    0x8500, // x: 5, y:14, sw: 5, cs:14, id:110
    0x6700, // x: 4, y:14, sw: 4, cs:14, id:102
    0x4900, // x: 3, y:14, sw: 3, cs:14, id: 94
    0x2b00, // x: 2, y:14, sw: 2, cs:14, id: 86
    0x0d00, // x: 1, y:14, sw: 1, cs:14, id: 78
    0x4a01, // x: 9, y:15, sw: 9, cs:15, id:143
    0x2c01, // x: 8, y:15, sw: 8, cs:15, id:135
    0x0e01, // x: 7, y:15, sw: 7, cs:15, id:127
    0xa400, // x: 6, y:15, sw: 6, cs:15, id:119
    0x8600, // x: 5, y:15, sw: 5, cs:15, id:111
    0x6800, // x: 4, y:15, sw: 4, cs:15, id:103
    0x4a00, // x: 3, y:15, sw: 3, cs:15, id: 95
    0x2c00, // x: 2, y:15, sw: 2, cs:15, id: 87
    0x0e00, // x: 1, y:15, sw: 1, cs:15, id: 79
    0x4b01, // x: 9, y:16, sw: 9, cs:16, id:144
    0x2d01, // x: 8, y:16, sw: 8, cs:16, id:136
    0x0f01, // x: 7, y:16, sw: 7, cs:16, id:128
    0xa500, // x: 6, y:16, sw: 6, cs:16, id:120
    0x8700, // x: 5, y:16, sw: 5, cs:16, id:112
    0x6900, // x: 4, y:16, sw: 4, cs:16, id:104
    0x4b00, // x: 3, y:16, sw: 3, cs:16, id: 96
    0x2d00, // x: 2, y:16, sw: 2, cs:16, id: 88
    0x0f00, // x: 1, y:16, sw: 1, cs:16, id: 80
    0x4c01, // x: 9, y:17, sw: 9, cs:17, id:273
    0x2e01, // x: 8, y:17, sw: 8, cs:17, id:257
    0x1001, // x: 7, y:17, sw: 7, cs:17, id:241
    0xa600, // x: 6, y:17, sw: 6, cs:17, id:225
    0x8800, // x: 5, y:17, sw: 5, cs:17, id:209
    0x6a00, // x: 4, y:17, sw: 4, cs:17, id:193
    0x4c00, // x: 3, y:17, sw: 3, cs:17, id:177
    0x2e00, // x: 2, y:17, sw: 2, cs:17, id:161
    0x1000, // x: 1, y:17, sw: 1, cs:17, id:145
    0x4d01, // x: 9, y:18, sw: 9, cs:18, id:274
    0x2f01, // x: 8, y:18, sw: 8, cs:18, id:258
    0x1101, // x: 7, y:18, sw: 7, cs:18, id:242
    0xa700, // x: 6, y:18, sw: 6, cs:18, id:226
    0x8900, // x: 5, y:18, sw: 5, cs:18, id:210
    0x6b00, // x: 4, y:18, sw: 4, cs:18, id:194
    0x4d00, // x: 3, y:18, sw: 3, cs:18, id:178
    0x2f00, // x: 2, y:18, sw: 2, cs:18, id:162
    0x1100, // x: 1, y:18, sw: 1, cs:18, id:146
    0x4e01, // x: 9, y:19, sw: 9, cs:19, id:275
    0x3001, // x: 8, y:19, sw: 8, cs:19, id:259
    0x1201, // x: 7, y:19, sw: 7, cs:19, id:243
    0xa800, // x: 6, y:19, sw: 6, cs:19, id:227
    0x8a00, // x: 5, y:19, sw: 5, cs:19, id:211
    0x6c00, // x: 4, y:19, sw: 4, cs:19, id:195
    0x4e00, // x: 3, y:19, sw: 3, cs:19, id:179
    0x3000, // x: 2, y:19, sw: 2, cs:19, id:163
    0x1200, // x: 1, y:19, sw: 1, cs:19, id:147
    0x4f01, // x: 9, y:20, sw: 9, cs:20, id:276
    0x3101, // x: 8, y:20, sw: 8, cs:20, id:260
    0x1301, // x: 7, y:20, sw: 7, cs:20, id:244
    0xa900, // x: 6, y:20, sw: 6, cs:20, id:228
    0x8b00, // x: 5, y:20, sw: 5, cs:20, id:212
    0x6d00, // x: 4, y:20, sw: 4, cs:20, id:196
    0x4f00, // x: 3, y:20, sw: 3, cs:20, id:180
    0x3100, // x: 2, y:20, sw: 2, cs:20, id:164
    0x1300, // x: 1, y:20, sw: 1, cs:20, id:148
    0x5001, // x: 9, y:21, sw: 9, cs:21, id:277
    0x3201, // x: 8, y:21, sw: 8, cs:21, id:261
    0x1401, // x: 7, y:21, sw: 7, cs:21, id:245
    0xaa00, // x: 6, y:21, sw: 6, cs:21, id:229
    0x8c00, // x: 5, y:21, sw: 5, cs:21, id:213
    0x6e00, // x: 4, y:21, sw: 4, cs:21, id:197
    0x5000, // x: 3, y:21, sw: 3, cs:21, id:181
    0x3200, // x: 2, y:21, sw: 2, cs:21, id:165
    0x1400, // x: 1, y:21, sw: 1, cs:21, id:149
    0x5101, // x: 9, y:22, sw: 9, cs:22, id:278
    0x3301, // x: 8, y:22, sw: 8, cs:22, id:262
    0x1501, // x: 7, y:22, sw: 7, cs:22, id:246
    0xab00, // x: 6, y:22, sw: 6, cs:22, id:230
    0x8d00, // x: 5, y:22, sw: 5, cs:22, id:214
    0x6f00, // x: 4, y:22, sw: 4, cs:22, id:198
    0x5100, // x: 3, y:22, sw: 3, cs:22, id:182
    0x3300, // x: 2, y:22, sw: 2, cs:22, id:166
    0x1500, // x: 1, y:22, sw: 1, cs:22, id:150
    0x5201, // x: 9, y:23, sw: 9, cs:23, id:279
    0x3401, // x: 8, y:23, sw: 8, cs:23, id:263
    0x1601, // x: 7, y:23, sw: 7, cs:23, id:247
    0xac00, // x: 6, y:23, sw: 6, cs:23, id:231
    0x8e00, // x: 5, y:23, sw: 5, cs:23, id:215
    0x7000, // x: 4, y:23, sw: 4, cs:23, id:199
    0x5200, // x: 3, y:23, sw: 3, cs:23, id:183
    0x3400, // x: 2, y:23, sw: 2, cs:23, id:167
    0x1600, // x: 1, y:23, sw: 1, cs:23, id:151
    0x5301, // x: 9, y:24, sw: 9, cs:24, id:280
    0x3501, // x: 8, y:24, sw: 8, cs:24, id:264
    0x1701, // x: 7, y:24, sw: 7, cs:24, id:248
    0xad00, // x: 6, y:24, sw: 6, cs:24, id:232
    0x8f00, // x: 5, y:24, sw: 5, cs:24, id:216
    0x7100, // x: 4, y:24, sw: 4, cs:24, id:200
    0x5300, // x: 3, y:24, sw: 3, cs:24, id:184
    0x3500, // x: 2, y:24, sw: 2, cs:24, id:168
    0x1700, // x: 1, y:24, sw: 1, cs:24, id:152
    0x5401, // x: 9, y:25, sw: 9, cs:25, id:281
    0x3601, // x: 8, y:25, sw: 8, cs:25, id:265
    0x1801, // x: 7, y:25, sw: 7, cs:25, id:249
    0xae00, // x: 6, y:25, sw: 6, cs:25, id:233
    0x9000, // x: 5, y:25, sw: 5, cs:25, id:217
    0x7200, // x: 4, y:25, sw: 4, cs:25, id:201
    0x5400, // x: 3, y:25, sw: 3, cs:25, id:185
    0x3600, // x: 2, y:25, sw: 2, cs:25, id:169
    0x1800, // x: 1, y:25, sw: 1, cs:25, id:153
    0x5501, // x: 9, y:26, sw: 9, cs:26, id:282
    0x3701, // x: 8, y:26, sw: 8, cs:26, id:266
    0x1901, // x: 7, y:26, sw: 7, cs:26, id:250
    0xaf00, // x: 6, y:26, sw: 6, cs:26, id:234
    0x9100, // x: 5, y:26, sw: 5, cs:26, id:218
    0x7300, // x: 4, y:26, sw: 4, cs:26, id:202
    0x5500, // x: 3, y:26, sw: 3, cs:26, id:186
    0x3700, // x: 2, y:26, sw: 2, cs:26, id:170
    0x1900, // x: 1, y:26, sw: 1, cs:26, id:154
    0x5601, // x: 9, y:27, sw: 9, cs:27, id:283
    0x3801, // x: 8, y:27, sw: 8, cs:27, id:267
    0x1a01, // x: 7, y:27, sw: 7, cs:27, id:251
    0xb000, // x: 6, y:27, sw: 6, cs:27, id:235
    0x9200, // x: 5, y:27, sw: 5, cs:27, id:219
    0x7400, // x: 4, y:27, sw: 4, cs:27, id:203
    0x5600, // x: 3, y:27, sw: 3, cs:27, id:187
    0x3800, // x: 2, y:27, sw: 2, cs:27, id:171
    0x1a00, // x: 1, y:27, sw: 1, cs:27, id:155
    0x5701, // x: 9, y:28, sw: 9, cs:28, id:284
    0x3901, // x: 8, y:28, sw: 8, cs:28, id:268
    0x1b01, // x: 7, y:28, sw: 7, cs:28, id:252
    0xb100, // x: 6, y:28, sw: 6, cs:28, id:236
    0x9300, // x: 5, y:28, sw: 5, cs:28, id:220
    0x7500, // x: 4, y:28, sw: 4, cs:28, id:204
    0x5700, // x: 3, y:28, sw: 3, cs:28, id:188
    0x3900, // x: 2, y:28, sw: 2, cs:28, id:172
    0x1b00, // x: 1, y:28, sw: 1, cs:28, id:156
    0x5801, // x: 9, y:29, sw: 9, cs:29, id:285
    0x3a01, // x: 8, y:29, sw: 8, cs:29, id:269
    0x1c01, // x: 7, y:29, sw: 7, cs:29, id:253
    0xb200, // x: 6, y:29, sw: 6, cs:29, id:237
    0x9400, // x: 5, y:29, sw: 5, cs:29, id:221
    0x7600, // x: 4, y:29, sw: 4, cs:29, id:205
    0x5800, // x: 3, y:29, sw: 3, cs:29, id:189
    0x3a00, // x: 2, y:29, sw: 2, cs:29, id:173
    0x1c00, // x: 1, y:29, sw: 1, cs:29, id:157
    0x5901, // x: 9, y:30, sw: 9, cs:30, id:286
    0x3b01, // x: 8, y:30, sw: 8, cs:30, id:270
    0x1d01, // x: 7, y:30, sw: 7, cs:30, id:254
    0xb300, // x: 6, y:30, sw: 6, cs:30, id:238
    0x9500, // x: 5, y:30, sw: 5, cs:30, id:222
    0x7700, // x: 4, y:30, sw: 4, cs:30, id:206
    0x5900, // x: 3, y:30, sw: 3, cs:30, id:190
    0x3b00, // x: 2, y:30, sw: 2, cs:30, id:174
    0x1d00, // x: 1, y:30, sw: 1, cs:30, id:158
    0xa201, // x: 9, y:31, sw: 9, cs:31, id:287
    0x9901, // x: 8, y:31, sw: 8, cs:31, id:271
    0x9001, // x: 7, y:31, sw: 7, cs:31, id:255
    0x8701, // x: 6, y:31, sw: 6, cs:31, id:239
    0x7e01, // x: 5, y:31, sw: 5, cs:31, id:223
    0x7501, // x: 4, y:31, sw: 4, cs:31, id:207
    0x6c01, // x: 3, y:31, sw: 3, cs:31, id:191
    0x6301, // x: 2, y:31, sw: 2, cs:31, id:175
    0x5a01, // x: 1, y:31, sw: 1, cs:31, id:159
    0xa301, // x: 9, y:32, sw: 9, cs:32, id:288
    0x9a01, // x: 8, y:32, sw: 8, cs:32, id:272
    0x9101, // x: 7, y:32, sw: 7, cs:32, id:256
    0x8801, // x: 6, y:32, sw: 6, cs:32, id:240
    0x7f01, // x: 5, y:32, sw: 5, cs:32, id:224
    0x7601, // x: 4, y:32, sw: 4, cs:32, id:208
    0x6d01, // x: 3, y:32, sw: 3, cs:32, id:192
    0x6401, // x: 2, y:32, sw: 2, cs:32, id:176
    0x5b01, // x: 1, y:32, sw: 1, cs:32, id:160
    0xa401, // x: 9, y:33, sw: 9, cs:33, id:297
    0x9b01, // x: 8, y:33, sw: 8, cs:33, id:296
    0x9201, // x: 7, y:33, sw: 7, cs:33, id:295
    0x8901, // x: 6, y:33, sw: 6, cs:33, id:294
    0x8001, // x: 5, y:33, sw: 5, cs:33, id:293
    0x7701, // x: 4, y:33, sw: 4, cs:33, id:292
    0x6e01, // x: 3, y:33, sw: 3, cs:33, id:291
    0x6501, // x: 2, y:33, sw: 2, cs:33, id:290
    0x5c01, // x: 1, y:33, sw: 1, cs:33, id:289
    0xa501, // x: 9, y:34, sw: 9, cs:34, id:306
    0x9c01, // x: 8, y:34, sw: 8, cs:34, id:305
    0x9301, // x: 7, y:34, sw: 7, cs:34, id:304
    0x8a01, // x: 6, y:34, sw: 6, cs:34, id:303
    0x8101, // x: 5, y:34, sw: 5, cs:34, id:302
    0x7801, // x: 4, y:34, sw: 4, cs:34, id:301
    0x6f01, // x: 3, y:34, sw: 3, cs:34, id:300
    0x6601, // x: 2, y:34, sw: 2, cs:34, id:299
    0x5d01, // x: 1, y:34, sw: 1, cs:34, id:298
};
#else // USE_EVT_LUT
const uint16_t LUT_PIXEL[LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT] = {
    0x5f01, // x: 9, y: 1, sw: 1, cs:36, id:  9
    0x1e01, // x: 8, y: 1, sw: 8, cs: 1, id:  8
    0x0001, // x: 7, y: 1, sw: 7, cs: 1, id:  7
    0x9600, // x: 6, y: 1, sw: 6, cs: 1, id:  6
    0x7800, // x: 5, y: 1, sw: 5, cs: 1, id:  5
    0x5a00, // x: 4, y: 1, sw: 4, cs: 1, id:  4
    0x3c00, // x: 3, y: 1, sw: 3, cs: 1, id:  3
    0x1e00, // x: 2, y: 1, sw: 2, cs: 1, id:  2
    0x0000, // x: 1, y: 1, sw: 1, cs: 1, id:  1
    0x6001, // x: 9, y: 2, sw: 1, cs:37, id: 18
    0x1f01, // x: 8, y: 2, sw: 8, cs: 2, id: 17
    0x0101, // x: 7, y: 2, sw: 7, cs: 2, id: 16
    0x9700, // x: 6, y: 2, sw: 6, cs: 2, id: 15
    0x7900, // x: 5, y: 2, sw: 5, cs: 2, id: 14
    0x5b00, // x: 4, y: 2, sw: 4, cs: 2, id: 13
    0x3d00, // x: 3, y: 2, sw: 3, cs: 2, id: 12
    0x1f00, // x: 2, y: 2, sw: 2, cs: 2, id: 11
    0x0100, // x: 1, y: 2, sw: 1, cs: 2, id: 10
    0x6101, // x: 9, y: 3, sw: 1, cs:38, id: 27
    0x2001, // x: 8, y: 3, sw: 8, cs: 3, id: 26
    0x0201, // x: 7, y: 3, sw: 7, cs: 3, id: 25
    0x9800, // x: 6, y: 3, sw: 6, cs: 3, id: 24
    0x7a00, // x: 5, y: 3, sw: 5, cs: 3, id: 23
    0x5c00, // x: 4, y: 3, sw: 4, cs: 3, id: 22
    0x3e00, // x: 3, y: 3, sw: 3, cs: 3, id: 21
    0x2000, // x: 2, y: 3, sw: 2, cs: 3, id: 20
    0x0200, // x: 1, y: 3, sw: 1, cs: 3, id: 19
    0x6201, // x: 9, y: 4, sw: 1, cs:39, id: 36
    0x2101, // x: 8, y: 4, sw: 8, cs: 4, id: 35
    0x0301, // x: 7, y: 4, sw: 7, cs: 4, id: 34
    0x9900, // x: 6, y: 4, sw: 6, cs: 4, id: 33
    0x7b00, // x: 5, y: 4, sw: 5, cs: 4, id: 32
    0x5d00, // x: 4, y: 4, sw: 4, cs: 4, id: 31
    0x3f00, // x: 3, y: 4, sw: 3, cs: 4, id: 30
    0x2100, // x: 2, y: 4, sw: 2, cs: 4, id: 29
    0x0300, // x: 1, y: 4, sw: 1, cs: 4, id: 28
    0x5e01, // x: 9, y: 5, sw: 1, cs:35, id: 69
    0x2201, // x: 8, y: 5, sw: 8, cs: 5, id: 65
    0x0401, // x: 7, y: 5, sw: 7, cs: 5, id: 61
    0x9a00, // x: 6, y: 5, sw: 6, cs: 5, id: 57
    0x7c00, // x: 5, y: 5, sw: 5, cs: 5, id: 53
    0x5e00, // x: 4, y: 5, sw: 4, cs: 5, id: 49
    0x4000, // x: 3, y: 5, sw: 3, cs: 5, id: 45
    0x2200, // x: 2, y: 5, sw: 2, cs: 5, id: 41
    0x0400, // x: 1, y: 5, sw: 1, cs: 5, id: 37
    0x6801, // x: 9, y: 6, sw: 2, cs:36, id: 70
    0x2301, // x: 8, y: 6, sw: 8, cs: 6, id: 66
    0x0501, // x: 7, y: 6, sw: 7, cs: 6, id: 62
    0x9b00, // x: 6, y: 6, sw: 6, cs: 6, id: 58
    0x7d00, // x: 5, y: 6, sw: 5, cs: 6, id: 54
    0x5f00, // x: 4, y: 6, sw: 4, cs: 6, id: 50
    0x4100, // x: 3, y: 6, sw: 3, cs: 6, id: 46
    0x2300, // x: 2, y: 6, sw: 2, cs: 6, id: 42
    0x0500, // x: 1, y: 6, sw: 1, cs: 6, id: 38
    0x6901, // x: 9, y: 7, sw: 2, cs:37, id: 71
    0x2401, // x: 8, y: 7, sw: 8, cs: 7, id: 67
    0x0601, // x: 7, y: 7, sw: 7, cs: 7, id: 63
    0x9c00, // x: 6, y: 7, sw: 6, cs: 7, id: 59
    0x7e00, // x: 5, y: 7, sw: 5, cs: 7, id: 55
    0x6000, // x: 4, y: 7, sw: 4, cs: 7, id: 51
    0x4200, // x: 3, y: 7, sw: 3, cs: 7, id: 47
    0x2400, // x: 2, y: 7, sw: 2, cs: 7, id: 43
    0x0600, // x: 1, y: 7, sw: 1, cs: 7, id: 39
    0x6a01, // x: 9, y: 8, sw: 2, cs:38, id: 72
    0x2501, // x: 8, y: 8, sw: 8, cs: 8, id: 68
    0x0701, // x: 7, y: 8, sw: 7, cs: 8, id: 64
    0x9d00, // x: 6, y: 8, sw: 6, cs: 8, id: 60
    0x7f00, // x: 5, y: 8, sw: 5, cs: 8, id: 56
    0x6100, // x: 4, y: 8, sw: 4, cs: 8, id: 52
    0x4300, // x: 3, y: 8, sw: 3, cs: 8, id: 48
    0x2500, // x: 2, y: 8, sw: 2, cs: 8, id: 44
    0x0700, // x: 1, y: 8, sw: 1, cs: 8, id: 40
    0x6b01, // x: 9, y: 9, sw: 2, cs:39, id:137
    0x2601, // x: 8, y: 9, sw: 8, cs: 9, id:129
    0x0801, // x: 7, y: 9, sw: 7, cs: 9, id:121
    0x9e00, // x: 6, y: 9, sw: 6, cs: 9, id:113
    0x8000, // x: 5, y: 9, sw: 5, cs: 9, id:105
    0x6200, // x: 4, y: 9, sw: 4, cs: 9, id: 97
    0x4400, // x: 3, y: 9, sw: 3, cs: 9, id: 89
    0x2600, // x: 2, y: 9, sw: 2, cs: 9, id: 81
    0x0800, // x: 1, y: 9, sw: 1, cs: 9, id: 73
    0x6701, // x: 9, y:10, sw: 2, cs:35, id:138
    0x2701, // x: 8, y:10, sw: 8, cs:10, id:130
    0x0901, // x: 7, y:10, sw: 7, cs:10, id:122
    0x9f00, // x: 6, y:10, sw: 6, cs:10, id:114
    0x8100, // x: 5, y:10, sw: 5, cs:10, id:106
    0x6300, // x: 4, y:10, sw: 4, cs:10, id: 98
    0x4500, // x: 3, y:10, sw: 3, cs:10, id: 90
    0x2700, // x: 2, y:10, sw: 2, cs:10, id: 82
    0x0900, // x: 1, y:10, sw: 1, cs:10, id: 74
    0x7101, // x: 9, y:11, sw: 3, cs:36, id:139
    0x2801, // x: 8, y:11, sw: 8, cs:11, id:131
    0x0a01, // x: 7, y:11, sw: 7, cs:11, id:123
    0xa000, // x: 6, y:11, sw: 6, cs:11, id:115
    0x8200, // x: 5, y:11, sw: 5, cs:11, id:107
    0x6400, // x: 4, y:11, sw: 4, cs:11, id: 99
    0x4600, // x: 3, y:11, sw: 3, cs:11, id: 91
    0x2800, // x: 2, y:11, sw: 2, cs:11, id: 83
    0x0a00, // x: 1, y:11, sw: 1, cs:11, id: 75
    0x7201, // x: 9, y:12, sw: 3, cs:37, id:140
    0x2901, // x: 8, y:12, sw: 8, cs:12, id:132
    0x0b01, // x: 7, y:12, sw: 7, cs:12, id:124
    0xa100, // x: 6, y:12, sw: 6, cs:12, id:116
    0x8300, // x: 5, y:12, sw: 5, cs:12, id:108
    0x6500, // x: 4, y:12, sw: 4, cs:12, id:100
    0x4700, // x: 3, y:12, sw: 3, cs:12, id: 92
    0x2900, // x: 2, y:12, sw: 2, cs:12, id: 84
    0x0b00, // x: 1, y:12, sw: 1, cs:12, id: 76
    0x7301, // x: 9, y:13, sw: 3, cs:38, id:141
    0x2a01, // x: 8, y:13, sw: 8, cs:13, id:133
    0x0c01, // x: 7, y:13, sw: 7, cs:13, id:125
    0xa200, // x: 6, y:13, sw: 6, cs:13, id:117
    0x8400, // x: 5, y:13, sw: 5, cs:13, id:109
    0x6600, // x: 4, y:13, sw: 4, cs:13, id:101
    0x4800, // x: 3, y:13, sw: 3, cs:13, id: 93
    0x2a00, // x: 2, y:13, sw: 2, cs:13, id: 85
    0x0c00, // x: 1, y:13, sw: 1, cs:13, id: 77
    0x7001, // x: 9, y:14, sw: 3, cs:35, id:142
    0x2b01, // x: 8, y:14, sw: 8, cs:14, id:134
    0x0d01, // x: 7, y:14, sw: 7, cs:14, id:126
    0xa300, // x: 6, y:14, sw: 6, cs:14, id:118
    0x8500, // x: 5, y:14, sw: 5, cs:14, id:110
    0x6700, // x: 4, y:14, sw: 4, cs:14, id:102
    0x4900, // x: 3, y:14, sw: 3, cs:14, id: 94
    0x2b00, // x: 2, y:14, sw: 2, cs:14, id: 86
    0x0d00, // x: 1, y:14, sw: 1, cs:14, id: 78
    0x7a01, // x: 9, y:15, sw: 4, cs:36, id:143
    0x2c01, // x: 8, y:15, sw: 8, cs:15, id:135
    0x0e01, // x: 7, y:15, sw: 7, cs:15, id:127
    0xa400, // x: 6, y:15, sw: 6, cs:15, id:119
    0x8600, // x: 5, y:15, sw: 5, cs:15, id:111
    0x6800, // x: 4, y:15, sw: 4, cs:15, id:103
    0x4a00, // x: 3, y:15, sw: 3, cs:15, id: 95
    0x2c00, // x: 2, y:15, sw: 2, cs:15, id: 87
    0x0e00, // x: 1, y:15, sw: 1, cs:15, id: 79
    0x7b01, // x: 9, y:16, sw: 4, cs:37, id:144
    0x2d01, // x: 8, y:16, sw: 8, cs:16, id:136
    0x0f01, // x: 7, y:16, sw: 7, cs:16, id:128
    0xa500, // x: 6, y:16, sw: 6, cs:16, id:120
    0x8700, // x: 5, y:16, sw: 5, cs:16, id:112
    0x6900, // x: 4, y:16, sw: 4, cs:16, id:104
    0x4b00, // x: 3, y:16, sw: 3, cs:16, id: 96
    0x2d00, // x: 2, y:16, sw: 2, cs:16, id: 88
    0x0f00, // x: 1, y:16, sw: 1, cs:16, id: 80
    0x7c01, // x: 9, y:17, sw: 4, cs:38, id:273
    0x2e01, // x: 8, y:17, sw: 8, cs:17, id:257
    0x1001, // x: 7, y:17, sw: 7, cs:17, id:241
    0xa600, // x: 6, y:17, sw: 6, cs:17, id:225
    0x8800, // x: 5, y:17, sw: 5, cs:17, id:209
    0x6a00, // x: 4, y:17, sw: 4, cs:17, id:193
    0x4c00, // x: 3, y:17, sw: 3, cs:17, id:177
    0x2e00, // x: 2, y:17, sw: 2, cs:17, id:161
    0x1000, // x: 1, y:17, sw: 1, cs:17, id:145
    0x7901, // x: 9, y:18, sw: 4, cs:35, id:274
    0x2f01, // x: 8, y:18, sw: 8, cs:18, id:258
    0x1101, // x: 7, y:18, sw: 7, cs:18, id:242
    0xa700, // x: 6, y:18, sw: 6, cs:18, id:226
    0x8900, // x: 5, y:18, sw: 5, cs:18, id:210
    0x6b00, // x: 4, y:18, sw: 4, cs:18, id:194
    0x4d00, // x: 3, y:18, sw: 3, cs:18, id:178
    0x2f00, // x: 2, y:18, sw: 2, cs:18, id:162
    0x1100, // x: 1, y:18, sw: 1, cs:18, id:146
    0x8301, // x: 9, y:19, sw: 5, cs:36, id:275
    0x3001, // x: 8, y:19, sw: 8, cs:19, id:259
    0x1201, // x: 7, y:19, sw: 7, cs:19, id:243
    0xa800, // x: 6, y:19, sw: 6, cs:19, id:227
    0x8a00, // x: 5, y:19, sw: 5, cs:19, id:211
    0x6c00, // x: 4, y:19, sw: 4, cs:19, id:195
    0x4e00, // x: 3, y:19, sw: 3, cs:19, id:179
    0x3000, // x: 2, y:19, sw: 2, cs:19, id:163
    0x1200, // x: 1, y:19, sw: 1, cs:19, id:147
    0x8401, // x: 9, y:20, sw: 5, cs:37, id:276
    0x3101, // x: 8, y:20, sw: 8, cs:20, id:260
    0x1301, // x: 7, y:20, sw: 7, cs:20, id:244
    0xa900, // x: 6, y:20, sw: 6, cs:20, id:228
    0x8b00, // x: 5, y:20, sw: 5, cs:20, id:212
    0x6d00, // x: 4, y:20, sw: 4, cs:20, id:196
    0x4f00, // x: 3, y:20, sw: 3, cs:20, id:180
    0x3100, // x: 2, y:20, sw: 2, cs:20, id:164
    0x1300, // x: 1, y:20, sw: 1, cs:20, id:148
    0x8501, // x: 9, y:21, sw: 5, cs:38, id:277
    0x3201, // x: 8, y:21, sw: 8, cs:21, id:261
    0x1401, // x: 7, y:21, sw: 7, cs:21, id:245
    0xaa00, // x: 6, y:21, sw: 6, cs:21, id:229
    0x8c00, // x: 5, y:21, sw: 5, cs:21, id:213
    0x6e00, // x: 4, y:21, sw: 4, cs:21, id:197
    0x5000, // x: 3, y:21, sw: 3, cs:21, id:181
    0x3200, // x: 2, y:21, sw: 2, cs:21, id:165
    0x1400, // x: 1, y:21, sw: 1, cs:21, id:149
    0x8201, // x: 9, y:22, sw: 5, cs:35, id:278
    0x3301, // x: 8, y:22, sw: 8, cs:22, id:262
    0x1501, // x: 7, y:22, sw: 7, cs:22, id:246
    0xab00, // x: 6, y:22, sw: 6, cs:22, id:230
    0x8d00, // x: 5, y:22, sw: 5, cs:22, id:214
    0x6f00, // x: 4, y:22, sw: 4, cs:22, id:198
    0x5100, // x: 3, y:22, sw: 3, cs:22, id:182
    0x3300, // x: 2, y:22, sw: 2, cs:22, id:166
    0x1500, // x: 1, y:22, sw: 1, cs:22, id:150
    0x8c01, // x: 9, y:23, sw: 6, cs:36, id:279
    0x3401, // x: 8, y:23, sw: 8, cs:23, id:263
    0x1601, // x: 7, y:23, sw: 7, cs:23, id:247
    0xac00, // x: 6, y:23, sw: 6, cs:23, id:231
    0x8e00, // x: 5, y:23, sw: 5, cs:23, id:215
    0x7000, // x: 4, y:23, sw: 4, cs:23, id:199
    0x5200, // x: 3, y:23, sw: 3, cs:23, id:183
    0x3400, // x: 2, y:23, sw: 2, cs:23, id:167
    0x1600, // x: 1, y:23, sw: 1, cs:23, id:151
    0x8d01, // x: 9, y:24, sw: 6, cs:37, id:280
    0x3501, // x: 8, y:24, sw: 8, cs:24, id:264
    0x1701, // x: 7, y:24, sw: 7, cs:24, id:248
    0xad00, // x: 6, y:24, sw: 6, cs:24, id:232
    0x8f00, // x: 5, y:24, sw: 5, cs:24, id:216
    0x7100, // x: 4, y:24, sw: 4, cs:24, id:200
    0x5300, // x: 3, y:24, sw: 3, cs:24, id:184
    0x3500, // x: 2, y:24, sw: 2, cs:24, id:168
    0x1700, // x: 1, y:24, sw: 1, cs:24, id:152
    0x8e01, // x: 9, y:25, sw: 6, cs:38, id:281
    0x3601, // x: 8, y:25, sw: 8, cs:25, id:265
    0x1801, // x: 7, y:25, sw: 7, cs:25, id:249
    0xae00, // x: 6, y:25, sw: 6, cs:25, id:233
    0x9000, // x: 5, y:25, sw: 5, cs:25, id:217
    0x7200, // x: 4, y:25, sw: 4, cs:25, id:201
    0x5400, // x: 3, y:25, sw: 3, cs:25, id:185
    0x3600, // x: 2, y:25, sw: 2, cs:25, id:169
    0x1800, // x: 1, y:25, sw: 1, cs:25, id:153
    0x8b01, // x: 9, y:26, sw: 6, cs:35, id:282
    0x3701, // x: 8, y:26, sw: 8, cs:26, id:266
    0x1901, // x: 7, y:26, sw: 7, cs:26, id:250
    0xaf00, // x: 6, y:26, sw: 6, cs:26, id:234
    0x9100, // x: 5, y:26, sw: 5, cs:26, id:218
    0x7300, // x: 4, y:26, sw: 4, cs:26, id:202
    0x5500, // x: 3, y:26, sw: 3, cs:26, id:186
    0x3700, // x: 2, y:26, sw: 2, cs:26, id:170
    0x1900, // x: 1, y:26, sw: 1, cs:26, id:154
    0x9501, // x: 9, y:27, sw: 7, cs:36, id:283
    0x3801, // x: 8, y:27, sw: 8, cs:27, id:267
    0x1a01, // x: 7, y:27, sw: 7, cs:27, id:251
    0xb000, // x: 6, y:27, sw: 6, cs:27, id:235
    0x9200, // x: 5, y:27, sw: 5, cs:27, id:219
    0x7400, // x: 4, y:27, sw: 4, cs:27, id:203
    0x5600, // x: 3, y:27, sw: 3, cs:27, id:187
    0x3800, // x: 2, y:27, sw: 2, cs:27, id:171
    0x1a00, // x: 1, y:27, sw: 1, cs:27, id:155
    0x9601, // x: 9, y:28, sw: 7, cs:37, id:284
    0x3901, // x: 8, y:28, sw: 8, cs:28, id:268
    0x1b01, // x: 7, y:28, sw: 7, cs:28, id:252
    0xb100, // x: 6, y:28, sw: 6, cs:28, id:236
    0x9300, // x: 5, y:28, sw: 5, cs:28, id:220
    0x7500, // x: 4, y:28, sw: 4, cs:28, id:204
    0x5700, // x: 3, y:28, sw: 3, cs:28, id:188
    0x3900, // x: 2, y:28, sw: 2, cs:28, id:172
    0x1b00, // x: 1, y:28, sw: 1, cs:28, id:156
    0x9701, // x: 9, y:29, sw: 7, cs:38, id:285
    0x3a01, // x: 8, y:29, sw: 8, cs:29, id:269
    0x1c01, // x: 7, y:29, sw: 7, cs:29, id:253
    0xb200, // x: 6, y:29, sw: 6, cs:29, id:237
    0x9400, // x: 5, y:29, sw: 5, cs:29, id:221
    0x7600, // x: 4, y:29, sw: 4, cs:29, id:205
    0x5800, // x: 3, y:29, sw: 3, cs:29, id:189
    0x3a00, // x: 2, y:29, sw: 2, cs:29, id:173
    0x1c00, // x: 1, y:29, sw: 1, cs:29, id:157
    0x9401, // x: 9, y:30, sw: 7, cs:35, id:286
    0x3b01, // x: 8, y:30, sw: 8, cs:30, id:270
    0x1d01, // x: 7, y:30, sw: 7, cs:30, id:254
    0xb300, // x: 6, y:30, sw: 6, cs:30, id:238
    0x9500, // x: 5, y:30, sw: 5, cs:30, id:222
    0x7700, // x: 4, y:30, sw: 4, cs:30, id:206
    0x5900, // x: 3, y:30, sw: 3, cs:30, id:190
    0x3b00, // x: 2, y:30, sw: 2, cs:30, id:174
    0x1d00, // x: 1, y:30, sw: 1, cs:30, id:158
    0x9e01, // x: 9, y:31, sw: 8, cs:36, id:287
    0x9901, // x: 8, y:31, sw: 8, cs:31, id:271
    0x9001, // x: 7, y:31, sw: 7, cs:31, id:255
    0x8701, // x: 6, y:31, sw: 6, cs:31, id:239
    0x7e01, // x: 5, y:31, sw: 5, cs:31, id:223
    0x7501, // x: 4, y:31, sw: 4, cs:31, id:207
    0x6c01, // x: 3, y:31, sw: 3, cs:31, id:191
    0x6301, // x: 2, y:31, sw: 2, cs:31, id:175
    0x5a01, // x: 1, y:31, sw: 1, cs:31, id:159
    0x9f01, // x: 9, y:32, sw: 8, cs:37, id:288
    0x9a01, // x: 8, y:32, sw: 8, cs:32, id:272
    0x9101, // x: 7, y:32, sw: 7, cs:32, id:256
    0x8801, // x: 6, y:32, sw: 6, cs:32, id:240
    0x7f01, // x: 5, y:32, sw: 5, cs:32, id:224
    0x7601, // x: 4, y:32, sw: 4, cs:32, id:208
    0x6d01, // x: 3, y:32, sw: 3, cs:32, id:192
    0x6401, // x: 2, y:32, sw: 2, cs:32, id:176
    0x5b01, // x: 1, y:32, sw: 1, cs:32, id:160
    0xa001, // x: 9, y:33, sw: 8, cs:38, id:297
    0x9b01, // x: 8, y:33, sw: 8, cs:33, id:296
    0x9201, // x: 7, y:33, sw: 7, cs:33, id:295
    0x8901, // x: 6, y:33, sw: 6, cs:33, id:294
    0x8001, // x: 5, y:33, sw: 5, cs:33, id:293
    0x7701, // x: 4, y:33, sw: 4, cs:33, id:292
    0x6e01, // x: 3, y:33, sw: 3, cs:33, id:291
    0x6501, // x: 2, y:33, sw: 2, cs:33, id:290
    0x5c01, // x: 1, y:33, sw: 1, cs:33, id:289
    0x9d01, // x: 9, y:34, sw: 8, cs:35, id:306
    0x9c01, // x: 8, y:34, sw: 8, cs:34, id:305
    0x9301, // x: 7, y:34, sw: 7, cs:34, id:304
    0x8a01, // x: 6, y:34, sw: 6, cs:34, id:303
    0x8101, // x: 5, y:34, sw: 5, cs:34, id:302
    0x7801, // x: 4, y:34, sw: 4, cs:34, id:301
    0x6f01, // x: 3, y:34, sw: 3, cs:34, id:300
    0x6601, // x: 2, y:34, sw: 2, cs:34, id:299
    0x5d01 // x: 1, y:34, sw: 1, cs:34, id:298
};
#endif // USE_EVT_LUT
