/*
 *   __ _   _ __    ___  (_) | | (_)   ___
 *  / _` | | '_ \  / __| | | | | | |  / __|
 * | (_| | | | | | \__ \ | | | | | | | (__
 *  \__,_| |_| |_| |___/ |_| |_| |_|  \___|
 *
 * Copyright (C):  	Xiamen Guoke Anxin Technology Co., LTD
 * FileName:     	as32x601_canfd.h
 * Author:          ANSILIC APPLICATION GROUP
 * Version :        V1.0
 * Date:			2024/07/30
 * History:
 *               <author>           <time>    <version >      <desc>
 *     ANSILIC APPLICATION GROUP  2024/07/30     1.0     build this module
 * Description:     This file contains all the functions prototypes for the CANFD firmware library.
 */

#ifndef __AS32X601_CANFD_H
#define __AS32X601_CANFD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "as32x601_canfd_regs.h"

/*
 * @brief  Use by parameter examine.
 */
#define IS_CANFD_ALL_PERIPH(PERIPH)    (((PERIPH) == CANFD0) || \
                                        ((PERIPH) == CANFD1) || \
                                        ((PERIPH) == CANFD2) || \
                                        ((PERIPH) == CANFD3))

/*
 * @brief CANFD Software Reset enumeration
 */
typedef enum
{
    CANFD_SRR_RESET  = 0x01,    /* CANFD Reset */
    CANFD_SRR_ENABLE = 0x02,    /* CANFD Enable */
}CANFDSRR_TypeDef;
#define IS_CANFD_SRR(SRESET) (((SRESET) == CANFD_SRR_RESET) || ((SRESET) == CANFD_SRR_ENABLE))

/*
 * @brief CANFD Arbitration Phase (Nominal) Baud Rate Prescaler enumeration
 */
typedef enum
{
    CANFD_APBRPR_1tp   = 0x00,  /* 1 Baud Rate Prescaler */
    CANFD_APBRPR_2tp   = 0x01,  /* 2 Baud Rate Prescaler */
    CANFD_APBRPR_3tp   = 0x02,  /* 3 Baud Rate Prescaler */
    CANFD_APBRPR_4tp   = 0x03,  /* 4 Baud Rate Prescaler */
    CANFD_APBRPR_5tp   = 0x04,  /* 5 Baud Rate Prescaler */
    CANFD_APBRPR_6tp   = 0x05,  /* 6 Baud Rate Prescaler */
    CANFD_APBRPR_7tp   = 0x06,  /* 7 Baud Rate Prescaler */
    CANFD_APBRPR_8tp   = 0x07,  /* 8 Baud Rate Prescaler */
    CANFD_APBRPR_9tp   = 0x08,  /* 9 Baud Rate Prescaler */
    CANFD_APBRPR_10tp  = 0x09,  /* 10 Baud Rate Prescaler */
    CANFD_APBRPR_11tp  = 0x0A,  /* 11 Baud Rate Prescaler */
    CANFD_APBRPR_12tp  = 0x0B,  /* 12 Baud Rate Prescaler */
    CANFD_APBRPR_13tp  = 0x0C,  /* 13 Baud Rate Prescaler */
    CANFD_APBRPR_14tp  = 0x0D,  /* 14 Baud Rate Prescaler */
    CANFD_APBRPR_15tp  = 0x0E,  /* 15 Baud Rate Prescaler */
    CANFD_APBRPR_16tp  = 0x0F,  /* 16 Baud Rate Prescaler */

    CANFD_APBRPR_17tp  = 0x10,  /* 17 Baud Rate Prescaler */
    CANFD_APBRPR_18tp  = 0x11,  /* 18 Baud Rate Prescaler */
    CANFD_APBRPR_19tp  = 0x12,  /* 19 Baud Rate Prescaler */
    CANFD_APBRPR_20tp  = 0x13,  /* 20 Baud Rate Prescaler */
    CANFD_APBRPR_21tp  = 0x14,  /* 21 Baud Rate Prescaler */
    CANFD_APBRPR_22tp  = 0x15,  /* 22 Baud Rate Prescaler */
    CANFD_APBRPR_23tp  = 0x16,  /* 23 Baud Rate Prescaler */
    CANFD_APBRPR_24tp  = 0x17,  /* 24 Baud Rate Prescaler */
    CANFD_APBRPR_25tp  = 0x18,  /* 25 Baud Rate Prescaler */
    CANFD_APBRPR_26tp  = 0x19,  /* 26 Baud Rate Prescaler */
    CANFD_APBRPR_27tp  = 0x1A,  /* 27 Baud Rate Prescaler */
    CANFD_APBRPR_28tp  = 0x1B,  /* 28 Baud Rate Prescaler */
    CANFD_APBRPR_29tp  = 0x1C,  /* 29 Baud Rate Prescaler */
    CANFD_APBRPR_30tp  = 0x1D,  /* 30 Baud Rate Prescaler */
    CANFD_APBRPR_31tp  = 0x1E,  /* 31 Baud Rate Prescaler */
    CANFD_APBRPR_32tp  = 0x1F,  /* 32 Baud Rate Prescaler */

    CANFD_APBRPR_33tp  = 0x20,  /* 33 Baud Rate Prescaler */
    CANFD_APBRPR_34tp  = 0x21,  /* 34 Baud Rate Prescaler */
    CANFD_APBRPR_35tp  = 0x22,  /* 35 Baud Rate Prescaler */
    CANFD_APBRPR_36tp  = 0x23,  /* 36 Baud Rate Prescaler */
    CANFD_APBRPR_37tp  = 0x24,  /* 37 Baud Rate Prescaler */
    CANFD_APBRPR_38tp  = 0x25,  /* 38 Baud Rate Prescaler */
    CANFD_APBRPR_39tp  = 0x26,  /* 39 Baud Rate Prescaler */
    CANFD_APBRPR_40tp  = 0x27,  /* 40 Baud Rate Prescaler */
    CANFD_APBRPR_41tp  = 0x28,  /* 41 Baud Rate Prescaler */
    CANFD_APBRPR_42tp  = 0x29,  /* 42 Baud Rate Prescaler */
    CANFD_APBRPR_43tp  = 0x2A,  /* 43 Baud Rate Prescaler */
    CANFD_APBRPR_44tp  = 0x2B,  /* 44 Baud Rate Prescaler */
    CANFD_APBRPR_45tp  = 0x2C,  /* 45 Baud Rate Prescaler */
    CANFD_APBRPR_46tp  = 0x2D,  /* 46 Baud Rate Prescaler */
    CANFD_APBRPR_47tp  = 0x2E,  /* 47 Baud Rate Prescaler */
    CANFD_APBRPR_48tp  = 0x2F,  /* 48 Baud Rate Prescaler */

    CANFD_APBRPR_49tp  = 0x30,  /* 49 Baud Rate Prescaler */
    CANFD_APBRPR_50tp  = 0x31,  /* 50 Baud Rate Prescaler */
    CANFD_APBRPR_51tp  = 0x32,  /* 51 Baud Rate Prescaler */
    CANFD_APBRPR_52tp  = 0x33,  /* 52 Baud Rate Prescaler */
    CANFD_APBRPR_53tp  = 0x34,  /* 53 Baud Rate Prescaler */
    CANFD_APBRPR_54tp  = 0x35,  /* 54 Baud Rate Prescaler */
    CANFD_APBRPR_55tp  = 0x36,  /* 55 Baud Rate Prescaler */
    CANFD_APBRPR_56tp  = 0x37,  /* 56 Baud Rate Prescaler */
    CANFD_APBRPR_57tp  = 0x38,  /* 57 Baud Rate Prescaler */
    CANFD_APBRPR_58tp  = 0x39,  /* 58 Baud Rate Prescaler */
    CANFD_APBRPR_59tp  = 0x3A,  /* 59 Baud Rate Prescaler */
    CANFD_APBRPR_60tp  = 0x3B,  /* 60 Baud Rate Prescaler */
    CANFD_APBRPR_61tp  = 0x3C,  /* 61 Baud Rate Prescaler */
    CANFD_APBRPR_62tp  = 0x3D,  /* 62 Baud Rate Prescaler */
    CANFD_APBRPR_63tp  = 0x3E,  /* 63 Baud Rate Prescaler */
    CANFD_APBRPR_64tp  = 0x3F,  /* 64 Baud Rate Prescaler */

    CANFD_APBRPR_65tp  = 0x40,  /* 65 Baud Rate Prescaler */
    CANFD_APBRPR_66tp  = 0x41,  /* 66 Baud Rate Prescaler */
    CANFD_APBRPR_67tp  = 0x42,  /* 67 Baud Rate Prescaler */
    CANFD_APBRPR_68tp  = 0x43,  /* 68 Baud Rate Prescaler */
    CANFD_APBRPR_69tp  = 0x44,  /* 69 Baud Rate Prescaler */
    CANFD_APBRPR_70tp  = 0x45,  /* 70 Baud Rate Prescaler */
    CANFD_APBRPR_71tp  = 0x46,  /* 71 Baud Rate Prescaler */
    CANFD_APBRPR_72tp  = 0x47,  /* 72 Baud Rate Prescaler */
    CANFD_APBRPR_73tp  = 0x48,  /* 73 Baud Rate Prescaler */
    CANFD_APBRPR_74tp  = 0x49,  /* 74 Baud Rate Prescaler */
    CANFD_APBRPR_75tp  = 0x4A,  /* 75 Baud Rate Prescaler */
    CANFD_APBRPR_76tp  = 0x4B,  /* 76 Baud Rate Prescaler */
    CANFD_APBRPR_77tp  = 0x4C,  /* 77 Baud Rate Prescaler */
    CANFD_APBRPR_78tp  = 0x4D,  /* 78 Baud Rate Prescaler */
    CANFD_APBRPR_79tp  = 0x4E,  /* 79 Baud Rate Prescaler */
    CANFD_APBRPR_80tp  = 0x4F,  /* 80 Baud Rate Prescaler */
    
    CANFD_APBRPR_81tp  = 0x50,  /* 81 Baud Rate Prescaler */
    CANFD_APBRPR_82tp  = 0x51,  /* 82 Baud Rate Prescaler */
    CANFD_APBRPR_83tp  = 0x52,  /* 83 Baud Rate Prescaler */
    CANFD_APBRPR_84tp  = 0x53,  /* 84 Baud Rate Prescaler */
    CANFD_APBRPR_85tp  = 0x54,  /* 85 Baud Rate Prescaler */
    CANFD_APBRPR_86tp  = 0x55,  /* 86 Baud Rate Prescaler */
    CANFD_APBRPR_87tp  = 0x56,  /* 87 Baud Rate Prescaler */
    CANFD_APBRPR_88tp  = 0x57,  /* 88 Baud Rate Prescaler */
    CANFD_APBRPR_89tp  = 0x58,  /* 89 Baud Rate Prescaler */
    CANFD_APBRPR_90tp  = 0x59,  /* 90 Baud Rate Prescaler */
    CANFD_APBRPR_91tp  = 0x5A,  /* 91 Baud Rate Prescaler */
    CANFD_APBRPR_92tp  = 0x5B,  /* 92 Baud Rate Prescaler */
    CANFD_APBRPR_93tp  = 0x5C,  /* 93 Baud Rate Prescaler */
    CANFD_APBRPR_94tp  = 0x5D,  /* 94 Baud Rate Prescaler */
    CANFD_APBRPR_95tp  = 0x5E,  /* 95 Baud Rate Prescaler */
    CANFD_APBRPR_96tp  = 0x5F,  /* 96 Baud Rate Prescaler */
    
    CANFD_APBRPR_97tp  = 0x60,  /* 97 Baud Rate Prescaler */
    CANFD_APBRPR_98tp  = 0x61,  /* 98 Baud Rate Prescaler */
    CANFD_APBRPR_99tp  = 0x62,  /* 99 Baud Rate Prescaler */
    CANFD_APBRPR_100tp = 0x63,  /* 100 Baud Rate Prescaler */
    CANFD_APBRPR_101tp = 0x64,  /* 101 Baud Rate Prescaler */
    CANFD_APBRPR_102tp = 0x65,  /* 102 Baud Rate Prescaler */
    CANFD_APBRPR_103tp = 0x66,  /* 103 Baud Rate Prescaler */
    CANFD_APBRPR_104tp = 0x67,  /* 104 Baud Rate Prescaler */
    CANFD_APBRPR_105tp = 0x68,  /* 105 Baud Rate Prescaler */
    CANFD_APBRPR_106tp = 0x69,  /* 106 Baud Rate Prescaler */
    CANFD_APBRPR_107tp = 0x6A,  /* 107 Baud Rate Prescaler */
    CANFD_APBRPR_108tp = 0x6B,  /* 108 Baud Rate Prescaler */
    CANFD_APBRPR_109tp = 0x6C,  /* 109 Baud Rate Prescaler */
    CANFD_APBRPR_110tp = 0x6D,  /* 110 Baud Rate Prescaler */
    CANFD_APBRPR_111tp = 0x6E,  /* 111 Baud Rate Prescaler */
    CANFD_APBRPR_112tp = 0x6F,  /* 112 Baud Rate Prescaler */
    
    CANFD_APBRPR_113tp = 0x70,  /* 113 Baud Rate Prescaler */
    CANFD_APBRPR_114tp = 0x71,  /* 114 Baud Rate Prescaler */
    CANFD_APBRPR_115tp = 0x72,  /* 115 Baud Rate Prescaler */
    CANFD_APBRPR_116tp = 0x73,  /* 116 Baud Rate Prescaler */
    CANFD_APBRPR_117tp = 0x74,  /* 117 Baud Rate Prescaler */
    CANFD_APBRPR_118tp = 0x75,  /* 118 Baud Rate Prescaler */
    CANFD_APBRPR_119tp = 0x76,  /* 119 Baud Rate Prescaler */
    CANFD_APBRPR_120tp = 0x77,  /* 120 Baud Rate Prescaler */
    CANFD_APBRPR_121tp = 0x78,  /* 121 Baud Rate Prescaler */
    CANFD_APBRPR_122tp = 0x79,  /* 122 Baud Rate Prescaler */
    CANFD_APBRPR_123tp = 0x7A,  /* 123 Baud Rate Prescaler */
    CANFD_APBRPR_124tp = 0x7B,  /* 124 Baud Rate Prescaler */
    CANFD_APBRPR_125tp = 0x7C,  /* 125 Baud Rate Prescaler */
    CANFD_APBRPR_126tp = 0x7D,  /* 126 Baud Rate Prescaler */
    CANFD_APBRPR_127tp = 0x7E,  /* 127 Baud Rate Prescaler */
    CANFD_APBRPR_128tp = 0x7F,  /* 128 Baud Rate Prescaler */

    CANFD_APBRPR_129tp = 0x80,  /* 129 Baud Rate Prescaler */
    CANFD_APBRPR_130tp = 0x81,  /* 130 Baud Rate Prescaler */
    CANFD_APBRPR_131tp = 0x82,  /* 131 Baud Rate Prescaler */
    CANFD_APBRPR_132tp = 0x83,  /* 132 Baud Rate Prescaler */
    CANFD_APBRPR_133tp = 0x84,  /* 133 Baud Rate Prescaler */
    CANFD_APBRPR_134tp = 0x85,  /* 134 Baud Rate Prescaler */
    CANFD_APBRPR_135tp = 0x86,  /* 135 Baud Rate Prescaler */
    CANFD_APBRPR_136tp = 0x87,  /* 136 Baud Rate Prescaler */
    CANFD_APBRPR_137tp = 0x88,  /* 137 Baud Rate Prescaler */
    CANFD_APBRPR_138tp = 0x89,  /* 138 Baud Rate Prescaler */
    CANFD_APBRPR_139tp = 0x8A,  /* 139 Baud Rate Prescaler */
    CANFD_APBRPR_140tp = 0x8B,  /* 140 Baud Rate Prescaler */
    CANFD_APBRPR_141tp = 0x8C,  /* 141 Baud Rate Prescaler */
    CANFD_APBRPR_142tp = 0x8D,  /* 142 Baud Rate Prescaler */
    CANFD_APBRPR_143tp = 0x8E,  /* 143 Baud Rate Prescaler */
    CANFD_APBRPR_144tp = 0x8F,  /* 144 Baud Rate Prescaler */
    
    CANFD_APBRPR_145tp = 0x90,  /* 145 Baud Rate Prescaler */
    CANFD_APBRPR_146tp = 0x91,  /* 146 Baud Rate Prescaler */
    CANFD_APBRPR_147tp = 0x92,  /* 147 Baud Rate Prescaler */
    CANFD_APBRPR_148tp = 0x93,  /* 148 Baud Rate Prescaler */
    CANFD_APBRPR_149tp = 0x94,  /* 149 Baud Rate Prescaler */
    CANFD_APBRPR_150tp = 0x95,  /* 150 Baud Rate Prescaler */
    CANFD_APBRPR_151tp = 0x96,  /* 151 Baud Rate Prescaler */
    CANFD_APBRPR_152tp = 0x97,  /* 152 Baud Rate Prescaler */
    CANFD_APBRPR_153tp = 0x98,  /* 153 Baud Rate Prescaler */
    CANFD_APBRPR_154tp = 0x99,  /* 154 Baud Rate Prescaler */
    CANFD_APBRPR_155tp = 0x9A,  /* 155 Baud Rate Prescaler */
    CANFD_APBRPR_156tp = 0x9B,  /* 156 Baud Rate Prescaler */
    CANFD_APBRPR_157tp = 0x9C,  /* 157 Baud Rate Prescaler */
    CANFD_APBRPR_158tp = 0x9D,  /* 158 Baud Rate Prescaler */
    CANFD_APBRPR_159tp = 0x9E,  /* 159 Baud Rate Prescaler */
    CANFD_APBRPR_160tp = 0x9F,  /* 160 Baud Rate Prescaler */
    
    CANFD_APBRPR_161tp = 0xA0,  /* 161 Baud Rate Prescaler */
    CANFD_APBRPR_162tp = 0xA1,  /* 162 Baud Rate Prescaler */
    CANFD_APBRPR_163tp = 0xA2,  /* 163 Baud Rate Prescaler */
    CANFD_APBRPR_164tp = 0xA3,  /* 164 Baud Rate Prescaler */
    CANFD_APBRPR_165tp = 0xA4,  /* 165 Baud Rate Prescaler */
    CANFD_APBRPR_166tp = 0xA5,  /* 166 Baud Rate Prescaler */
    CANFD_APBRPR_167tp = 0xA6,  /* 167 Baud Rate Prescaler */
    CANFD_APBRPR_168tp = 0xA7,  /* 168 Baud Rate Prescaler */
    CANFD_APBRPR_169tp = 0xA8,  /* 169 Baud Rate Prescaler */
    CANFD_APBRPR_170tp = 0xA9,  /* 170 Baud Rate Prescaler */
    CANFD_APBRPR_171tp = 0xAA,  /* 171 Baud Rate Prescaler */
    CANFD_APBRPR_172tp = 0xAB,  /* 172 Baud Rate Prescaler */
    CANFD_APBRPR_173tp = 0xAC,  /* 173 Baud Rate Prescaler */
    CANFD_APBRPR_174tp = 0xAD,  /* 174 Baud Rate Prescaler */
    CANFD_APBRPR_175tp = 0xAE,  /* 175 Baud Rate Prescaler */
    CANFD_APBRPR_176tp = 0xAF,  /* 176 Baud Rate Prescaler */

    CANFD_APBRPR_177tp = 0xB0,  /* 177 Baud Rate Prescaler */
    CANFD_APBRPR_178tp = 0xB1,  /* 178 Baud Rate Prescaler */
    CANFD_APBRPR_179tp = 0xB2,  /* 179 Baud Rate Prescaler */
    CANFD_APBRPR_180tp = 0xB3,  /* 180 Baud Rate Prescaler */
    CANFD_APBRPR_181tp = 0xB4,  /* 181 Baud Rate Prescaler */
    CANFD_APBRPR_182tp = 0xB5,  /* 182 Baud Rate Prescaler */
    CANFD_APBRPR_183tp = 0xB6,  /* 183 Baud Rate Prescaler */
    CANFD_APBRPR_184tp = 0xB7,  /* 184 Baud Rate Prescaler */
    CANFD_APBRPR_185tp = 0xB8,  /* 185 Baud Rate Prescaler */
    CANFD_APBRPR_186tp = 0xB9,  /* 186 Baud Rate Prescaler */
    CANFD_APBRPR_187tp = 0xBA,  /* 187 Baud Rate Prescaler */
    CANFD_APBRPR_188tp = 0xBB,  /* 188 Baud Rate Prescaler */
    CANFD_APBRPR_189tp = 0xBC,  /* 189 Baud Rate Prescaler */
    CANFD_APBRPR_190tp = 0xBD,  /* 190 Baud Rate Prescaler */
    CANFD_APBRPR_191tp = 0xBE,  /* 191 Baud Rate Prescaler */
    CANFD_APBRPR_192tp = 0xBF,  /* 192 Baud Rate Prescaler */
    
    CANFD_APBRPR_193tp = 0xC0,  /* 193 Baud Rate Prescaler */
    CANFD_APBRPR_194tp = 0xC1,  /* 194 Baud Rate Prescaler */
    CANFD_APBRPR_195tp = 0xC2,  /* 195 Baud Rate Prescaler */
    CANFD_APBRPR_196tp = 0xC3,  /* 196 Baud Rate Prescaler */
    CANFD_APBRPR_197tp = 0xC4,  /* 197 Baud Rate Prescaler */
    CANFD_APBRPR_198tp = 0xC5,  /* 198 Baud Rate Prescaler */
    CANFD_APBRPR_199tp = 0xC6,  /* 199 Baud Rate Prescaler */
    CANFD_APBRPR_200tp = 0xC7,  /* 200 Baud Rate Prescaler */
    CANFD_APBRPR_201tp = 0xC8,  /* 201 Baud Rate Prescaler */
    CANFD_APBRPR_202tp = 0xC9,  /* 202 Baud Rate Prescaler */
    CANFD_APBRPR_203tp = 0xCA,  /* 203 Baud Rate Prescaler */
    CANFD_APBRPR_204tp = 0xCB,  /* 204 Baud Rate Prescaler */
    CANFD_APBRPR_205tp = 0xCC,  /* 205 Baud Rate Prescaler */
    CANFD_APBRPR_206tp = 0xCD,  /* 206 Baud Rate Prescaler */
    CANFD_APBRPR_207tp = 0xCE,  /* 207 Baud Rate Prescaler */
    CANFD_APBRPR_208tp = 0xCF,  /* 208 Baud Rate Prescaler */

    CANFD_APBRPR_209tp = 0xD0,  /* 209 Baud Rate Prescaler */
    CANFD_APBRPR_210tp = 0xD1,  /* 210 Baud Rate Prescaler */
    CANFD_APBRPR_211tp = 0xD2,  /* 211 Baud Rate Prescaler */
    CANFD_APBRPR_212tp = 0xD3,  /* 212 Baud Rate Prescaler */
    CANFD_APBRPR_213tp = 0xD4,  /* 213 Baud Rate Prescaler */
    CANFD_APBRPR_214tp = 0xD5,  /* 214 Baud Rate Prescaler */
    CANFD_APBRPR_215tp = 0xD6,  /* 215 Baud Rate Prescaler */
    CANFD_APBRPR_216tp = 0xD7,  /* 216 Baud Rate Prescaler */
    CANFD_APBRPR_217tp = 0xD8,  /* 217 Baud Rate Prescaler */
    CANFD_APBRPR_218tp = 0xD9,  /* 218 Baud Rate Prescaler */
    CANFD_APBRPR_219tp = 0xDA,  /* 219 Baud Rate Prescaler */
    CANFD_APBRPR_220tp = 0xDB,  /* 220 Baud Rate Prescaler */
    CANFD_APBRPR_221tp = 0xDC,  /* 221 Baud Rate Prescaler */
    CANFD_APBRPR_222tp = 0xDD,  /* 222 Baud Rate Prescaler */
    CANFD_APBRPR_223tp = 0xDE,  /* 223 Baud Rate Prescaler */
    CANFD_APBRPR_224tp = 0xDF,  /* 224 Baud Rate Prescaler */

    CANFD_APBRPR_225tp = 0xE0,  /* 225 Baud Rate Prescaler */
    CANFD_APBRPR_226tp = 0xE1,  /* 226 Baud Rate Prescaler */
    CANFD_APBRPR_227tp = 0xE2,  /* 227 Baud Rate Prescaler */
    CANFD_APBRPR_228tp = 0xE3,  /* 228 Baud Rate Prescaler */
    CANFD_APBRPR_229tp = 0xE4,  /* 229 Baud Rate Prescaler */
    CANFD_APBRPR_230tp = 0xE5,  /* 230 Baud Rate Prescaler */
    CANFD_APBRPR_231tp = 0xE6,  /* 231 Baud Rate Prescaler */
    CANFD_APBRPR_232tp = 0xE7,  /* 232 Baud Rate Prescaler */
    CANFD_APBRPR_233tp = 0xE8,  /* 233 Baud Rate Prescaler */
    CANFD_APBRPR_234tp = 0xE9,  /* 234 Baud Rate Prescaler */
    CANFD_APBRPR_235tp = 0xEA,  /* 235 Baud Rate Prescaler */
    CANFD_APBRPR_236tp = 0xEB,  /* 236 Baud Rate Prescaler */
    CANFD_APBRPR_237tp = 0xEC,  /* 237 Baud Rate Prescaler */
    CANFD_APBRPR_238tp = 0xED,  /* 238 Baud Rate Prescaler */
    CANFD_APBRPR_239tp = 0xEE,  /* 239 Baud Rate Prescaler */
    CANFD_APBRPR_240tp = 0xEF,  /* 240 Baud Rate Prescaler */

    CANFD_APBRPR_241tp = 0xF0,  /* 241 Baud Rate Prescaler */
    CANFD_APBRPR_242tp = 0xF1,  /* 242 Baud Rate Prescaler */
    CANFD_APBRPR_243tp = 0xF2,  /* 243 Baud Rate Prescaler */
    CANFD_APBRPR_244tp = 0xF3,  /* 244 Baud Rate Prescaler */
    CANFD_APBRPR_245tp = 0xF4,  /* 245 Baud Rate Prescaler */
    CANFD_APBRPR_246tp = 0xF5,  /* 246 Baud Rate Prescaler */
    CANFD_APBRPR_247tp = 0xF6,  /* 247 Baud Rate Prescaler */
    CANFD_APBRPR_248tp = 0xF7,  /* 248 Baud Rate Prescaler */
    CANFD_APBRPR_249tp = 0xF8,  /* 249 Baud Rate Prescaler */
    CANFD_APBRPR_250tp = 0xF9,  /* 250 Baud Rate Prescaler */
    CANFD_APBRPR_251tp = 0xFA,  /* 251 Baud Rate Prescaler */
    CANFD_APBRPR_252tp = 0xFB,  /* 252 Baud Rate Prescaler */
    CANFD_APBRPR_253tp = 0xFC,  /* 253 Baud Rate Prescaler */
    CANFD_APBRPR_254tp = 0xFD,  /* 254 Baud Rate Prescaler */
    CANFD_APBRPR_255tp = 0xFE,  /* 255 Baud Rate Prescaler */
    CANFD_APBRPR_256tp = 0xFF,  /* 256 Baud Rate Prescaler */
}CANFDAPBRPR_TypeDef;
#define IS_CANFD_APBRPR(APBRPRTP)  (((APBRPRTP) == CANFD_APBRPR_1tp)   || ((APBRPRTP) == CANFD_APBRPR_2tp)   || \
                                    ((APBRPRTP) == CANFD_APBRPR_3tp)   || ((APBRPRTP) == CANFD_APBRPR_4tp)   || \
                                    ((APBRPRTP) == CANFD_APBRPR_5tp)   || ((APBRPRTP) == CANFD_APBRPR_6tp)   || \
                                    ((APBRPRTP) == CANFD_APBRPR_7tp)   || ((APBRPRTP) == CANFD_APBRPR_8tp)   || \
                                    ((APBRPRTP) == CANFD_APBRPR_9tp)   || ((APBRPRTP) == CANFD_APBRPR_10tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_11tp)  || ((APBRPRTP) == CANFD_APBRPR_12tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_13tp)  || ((APBRPRTP) == CANFD_APBRPR_14tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_15tp)  || ((APBRPRTP) == CANFD_APBRPR_16tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_17tp)  || ((APBRPRTP) == CANFD_APBRPR_18tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_19tp)  || ((APBRPRTP) == CANFD_APBRPR_20tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_21tp)  || ((APBRPRTP) == CANFD_APBRPR_22tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_23tp)  || ((APBRPRTP) == CANFD_APBRPR_24tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_25tp)  || ((APBRPRTP) == CANFD_APBRPR_26tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_27tp)  || ((APBRPRTP) == CANFD_APBRPR_28tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_29tp)  || ((APBRPRTP) == CANFD_APBRPR_30tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_31tp)  || ((APBRPRTP) == CANFD_APBRPR_32tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_33tp)  || ((APBRPRTP) == CANFD_APBRPR_34tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_35tp)  || ((APBRPRTP) == CANFD_APBRPR_36tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_37tp)  || ((APBRPRTP) == CANFD_APBRPR_38tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_39tp)  || ((APBRPRTP) == CANFD_APBRPR_40tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_41tp)  || ((APBRPRTP) == CANFD_APBRPR_42tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_43tp)  || ((APBRPRTP) == CANFD_APBRPR_44tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_45tp)  || ((APBRPRTP) == CANFD_APBRPR_46tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_47tp)  || ((APBRPRTP) == CANFD_APBRPR_48tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_49tp)  || ((APBRPRTP) == CANFD_APBRPR_50tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_51tp)  || ((APBRPRTP) == CANFD_APBRPR_52tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_53tp)  || ((APBRPRTP) == CANFD_APBRPR_54tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_55tp)  || ((APBRPRTP) == CANFD_APBRPR_56tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_57tp)  || ((APBRPRTP) == CANFD_APBRPR_58tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_59tp)  || ((APBRPRTP) == CANFD_APBRPR_60tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_61tp)  || ((APBRPRTP) == CANFD_APBRPR_62tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_63tp)  || ((APBRPRTP) == CANFD_APBRPR_64tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_65tp)  || ((APBRPRTP) == CANFD_APBRPR_66tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_67tp)  || ((APBRPRTP) == CANFD_APBRPR_68tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_69tp)  || ((APBRPRTP) == CANFD_APBRPR_70tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_71tp)  || ((APBRPRTP) == CANFD_APBRPR_72tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_73tp)  || ((APBRPRTP) == CANFD_APBRPR_74tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_75tp)  || ((APBRPRTP) == CANFD_APBRPR_76tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_77tp)  || ((APBRPRTP) == CANFD_APBRPR_78tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_79tp)  || ((APBRPRTP) == CANFD_APBRPR_80tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_81tp)  || ((APBRPRTP) == CANFD_APBRPR_82tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_83tp)  || ((APBRPRTP) == CANFD_APBRPR_84tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_85tp)  || ((APBRPRTP) == CANFD_APBRPR_86tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_87tp)  || ((APBRPRTP) == CANFD_APBRPR_88tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_89tp)  || ((APBRPRTP) == CANFD_APBRPR_90tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_91tp)  || ((APBRPRTP) == CANFD_APBRPR_92tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_93tp)  || ((APBRPRTP) == CANFD_APBRPR_94tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_95tp)  || ((APBRPRTP) == CANFD_APBRPR_96tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_97tp)  || ((APBRPRTP) == CANFD_APBRPR_98tp)  || \
                                    ((APBRPRTP) == CANFD_APBRPR_99tp)  || ((APBRPRTP) == CANFD_APBRPR_100tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_101tp) || ((APBRPRTP) == CANFD_APBRPR_102tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_103tp) || ((APBRPRTP) == CANFD_APBRPR_104tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_105tp) || ((APBRPRTP) == CANFD_APBRPR_106tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_107tp) || ((APBRPRTP) == CANFD_APBRPR_108tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_109tp) || ((APBRPRTP) == CANFD_APBRPR_110tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_111tp) || ((APBRPRTP) == CANFD_APBRPR_112tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_113tp) || ((APBRPRTP) == CANFD_APBRPR_114tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_115tp) || ((APBRPRTP) == CANFD_APBRPR_116tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_117tp) || ((APBRPRTP) == CANFD_APBRPR_118tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_119tp) || ((APBRPRTP) == CANFD_APBRPR_120tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_121tp) || ((APBRPRTP) == CANFD_APBRPR_122tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_123tp) || ((APBRPRTP) == CANFD_APBRPR_124tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_125tp) || ((APBRPRTP) == CANFD_APBRPR_126tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_127tp) || ((APBRPRTP) == CANFD_APBRPR_128tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_129tp) || ((APBRPRTP) == CANFD_APBRPR_130tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_131tp) || ((APBRPRTP) == CANFD_APBRPR_132tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_133tp) || ((APBRPRTP) == CANFD_APBRPR_134tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_135tp) || ((APBRPRTP) == CANFD_APBRPR_136tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_137tp) || ((APBRPRTP) == CANFD_APBRPR_138tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_139tp) || ((APBRPRTP) == CANFD_APBRPR_140tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_141tp) || ((APBRPRTP) == CANFD_APBRPR_142tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_143tp) || ((APBRPRTP) == CANFD_APBRPR_144tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_145tp) || ((APBRPRTP) == CANFD_APBRPR_146tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_147tp) || ((APBRPRTP) == CANFD_APBRPR_148tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_149tp) || ((APBRPRTP) == CANFD_APBRPR_150tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_151tp) || ((APBRPRTP) == CANFD_APBRPR_152tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_153tp) || ((APBRPRTP) == CANFD_APBRPR_154tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_155tp) || ((APBRPRTP) == CANFD_APBRPR_156tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_157tp) || ((APBRPRTP) == CANFD_APBRPR_158tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_159tp) || ((APBRPRTP) == CANFD_APBRPR_160tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_161tp) || ((APBRPRTP) == CANFD_APBRPR_162tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_163tp) || ((APBRPRTP) == CANFD_APBRPR_164tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_165tp) || ((APBRPRTP) == CANFD_APBRPR_166tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_167tp) || ((APBRPRTP) == CANFD_APBRPR_168tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_169tp) || ((APBRPRTP) == CANFD_APBRPR_170tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_171tp) || ((APBRPRTP) == CANFD_APBRPR_172tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_173tp) || ((APBRPRTP) == CANFD_APBRPR_174tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_175tp) || ((APBRPRTP) == CANFD_APBRPR_176tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_177tp) || ((APBRPRTP) == CANFD_APBRPR_178tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_179tp) || ((APBRPRTP) == CANFD_APBRPR_180tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_181tp) || ((APBRPRTP) == CANFD_APBRPR_182tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_183tp) || ((APBRPRTP) == CANFD_APBRPR_184tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_185tp) || ((APBRPRTP) == CANFD_APBRPR_186tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_187tp) || ((APBRPRTP) == CANFD_APBRPR_188tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_189tp) || ((APBRPRTP) == CANFD_APBRPR_190tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_191tp) || ((APBRPRTP) == CANFD_APBRPR_192tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_193tp) || ((APBRPRTP) == CANFD_APBRPR_194tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_195tp) || ((APBRPRTP) == CANFD_APBRPR_196tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_197tp) || ((APBRPRTP) == CANFD_APBRPR_198tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_199tp) || ((APBRPRTP) == CANFD_APBRPR_200tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_201tp) || ((APBRPRTP) == CANFD_APBRPR_202tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_203tp) || ((APBRPRTP) == CANFD_APBRPR_204tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_205tp) || ((APBRPRTP) == CANFD_APBRPR_206tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_207tp) || ((APBRPRTP) == CANFD_APBRPR_208tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_209tp) || ((APBRPRTP) == CANFD_APBRPR_210tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_211tp) || ((APBRPRTP) == CANFD_APBRPR_212tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_213tp) || ((APBRPRTP) == CANFD_APBRPR_214tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_215tp) || ((APBRPRTP) == CANFD_APBRPR_216tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_217tp) || ((APBRPRTP) == CANFD_APBRPR_218tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_219tp) || ((APBRPRTP) == CANFD_APBRPR_220tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_221tp) || ((APBRPRTP) == CANFD_APBRPR_222tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_223tp) || ((APBRPRTP) == CANFD_APBRPR_224tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_225tp) || ((APBRPRTP) == CANFD_APBRPR_226tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_227tp) || ((APBRPRTP) == CANFD_APBRPR_228tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_229tp) || ((APBRPRTP) == CANFD_APBRPR_230tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_231tp) || ((APBRPRTP) == CANFD_APBRPR_232tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_233tp) || ((APBRPRTP) == CANFD_APBRPR_234tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_235tp) || ((APBRPRTP) == CANFD_APBRPR_236tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_237tp) || ((APBRPRTP) == CANFD_APBRPR_238tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_239tp) || ((APBRPRTP) == CANFD_APBRPR_240tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_241tp) || ((APBRPRTP) == CANFD_APBRPR_242tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_243tp) || ((APBRPRTP) == CANFD_APBRPR_244tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_245tp) || ((APBRPRTP) == CANFD_APBRPR_246tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_247tp) || ((APBRPRTP) == CANFD_APBRPR_248tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_249tp) || ((APBRPRTP) == CANFD_APBRPR_250tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_251tp) || ((APBRPRTP) == CANFD_APBRPR_252tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_253tp) || ((APBRPRTP) == CANFD_APBRPR_254tp) || \
                                    ((APBRPRTP) == CANFD_APBRPR_255tp) || ((APBRPRTP) == CANFD_APBRPR_256tp))

/*
 * @brief CANFD Arbitration Phase (Nominal) Bit Timing (Time Segment 1) enumeration
 */
typedef enum
{
    CANFD_APBTR_TS1_1tp   = 0x00,   /* 1 Time Segment 1 */
    CANFD_APBTR_TS1_2tp   = 0x01,	/* 2 Time Segment 1 */
    CANFD_APBTR_TS1_3tp   = 0x02,	/* 3 Time Segment 1 */
    CANFD_APBTR_TS1_4tp   = 0x03,	/* 4 Time Segment 1 */
    CANFD_APBTR_TS1_5tp   = 0x04,	/* 5 Time Segment 1 */
    CANFD_APBTR_TS1_6tp   = 0x05,	/* 6 Time Segment 1 */
    CANFD_APBTR_TS1_7tp   = 0x06,	/* 7 Time Segment 1 */
    CANFD_APBTR_TS1_8tp   = 0x07,	/* 8 Time Segment 1 */
    CANFD_APBTR_TS1_9tp   = 0x08,	/* 9 Time Segment 1 */
    CANFD_APBTR_TS1_10tp  = 0x09,	/* 10 Time Segment 1 */
    CANFD_APBTR_TS1_11tp  = 0x0A,	/* 11 Time Segment 1 */
    CANFD_APBTR_TS1_12tp  = 0x0B,	/* 12 Time Segment 1 */
    CANFD_APBTR_TS1_13tp  = 0x0C,	/* 13 Time Segment 1 */
    CANFD_APBTR_TS1_14tp  = 0x0D,	/* 14 Time Segment 1 */
    CANFD_APBTR_TS1_15tp  = 0x0E,	/* 15 Time Segment 1 */
    CANFD_APBTR_TS1_16tp  = 0x0F,	/* 16 Time Segment 1 */

    CANFD_APBTR_TS1_17tp  = 0x10,	/* 17 Time Segment 1 */
    CANFD_APBTR_TS1_18tp  = 0x11,	/* 18 Time Segment 1 */
    CANFD_APBTR_TS1_19tp  = 0x12,	/* 19 Time Segment 1 */
    CANFD_APBTR_TS1_20tp  = 0x13,	/* 20 Time Segment 1 */
    CANFD_APBTR_TS1_21tp  = 0x14,	/* 21 Time Segment 1 */
    CANFD_APBTR_TS1_22tp  = 0x15,	/* 22 Time Segment 1 */
    CANFD_APBTR_TS1_23tp  = 0x16,	/* 23 Time Segment 1 */
    CANFD_APBTR_TS1_24tp  = 0x17,	/* 24 Time Segment 1 */
    CANFD_APBTR_TS1_25tp  = 0x18,	/* 25 Time Segment 1 */
    CANFD_APBTR_TS1_26tp  = 0x19,	/* 26 Time Segment 1 */
    CANFD_APBTR_TS1_27tp  = 0x1A,	/* 27 Time Segment 1 */
    CANFD_APBTR_TS1_28tp  = 0x1B,	/* 28 Time Segment 1 */
    CANFD_APBTR_TS1_29tp  = 0x1C,	/* 29 Time Segment 1 */
    CANFD_APBTR_TS1_30tp  = 0x1D,	/* 30 Time Segment 1 */
    CANFD_APBTR_TS1_31tp  = 0x1E,	/* 31 Time Segment 1 */
    CANFD_APBTR_TS1_32tp  = 0x1F,	/* 32 Time Segment 1 */

    CANFD_APBTR_TS1_33tp  = 0x20,	/* 33 Time Segment 1 */
    CANFD_APBTR_TS1_34tp  = 0x21,	/* 34 Time Segment 1 */
    CANFD_APBTR_TS1_35tp  = 0x22,	/* 35 Time Segment 1 */
    CANFD_APBTR_TS1_36tp  = 0x23,	/* 36 Time Segment 1 */
    CANFD_APBTR_TS1_37tp  = 0x24,	/* 37 Time Segment 1 */
    CANFD_APBTR_TS1_38tp  = 0x25,	/* 38 Time Segment 1 */
    CANFD_APBTR_TS1_39tp  = 0x26,	/* 39 Time Segment 1 */
    CANFD_APBTR_TS1_40tp  = 0x27,	/* 40 Time Segment 1 */
    CANFD_APBTR_TS1_41tp  = 0x28,	/* 41 Time Segment 1 */
    CANFD_APBTR_TS1_42tp  = 0x29,	/* 42 Time Segment 1 */
    CANFD_APBTR_TS1_43tp  = 0x2A,	/* 43 Time Segment 1 */
    CANFD_APBTR_TS1_44tp  = 0x2B,	/* 44 Time Segment 1 */
    CANFD_APBTR_TS1_45tp  = 0x2C,	/* 45 Time Segment 1 */
    CANFD_APBTR_TS1_46tp  = 0x2D,	/* 46 Time Segment 1 */
    CANFD_APBTR_TS1_47tp  = 0x2E,	/* 47 Time Segment 1 */
    CANFD_APBTR_TS1_48tp  = 0x2F,	/* 48 Time Segment 1 */

    CANFD_APBTR_TS1_49tp  = 0x30,	/* 49 Time Segment 1 */
    CANFD_APBTR_TS1_50tp  = 0x31,	/* 50 Time Segment 1 */
    CANFD_APBTR_TS1_51tp  = 0x32,	/* 51 Time Segment 1 */
    CANFD_APBTR_TS1_52tp  = 0x33,	/* 52 Time Segment 1 */
    CANFD_APBTR_TS1_53tp  = 0x34,	/* 53 Time Segment 1 */
    CANFD_APBTR_TS1_54tp  = 0x35,	/* 54 Time Segment 1 */
    CANFD_APBTR_TS1_55tp  = 0x36,	/* 55 Time Segment 1 */
    CANFD_APBTR_TS1_56tp  = 0x37,	/* 56 Time Segment 1 */
    CANFD_APBTR_TS1_57tp  = 0x38,	/* 57 Time Segment 1 */
    CANFD_APBTR_TS1_58tp  = 0x39,	/* 58 Time Segment 1 */
    CANFD_APBTR_TS1_59tp  = 0x3A,	/* 59 Time Segment 1 */
    CANFD_APBTR_TS1_60tp  = 0x3B,	/* 60 Time Segment 1 */
    CANFD_APBTR_TS1_61tp  = 0x3C,	/* 61 Time Segment 1 */
    CANFD_APBTR_TS1_62tp  = 0x3D,	/* 62 Time Segment 1 */
    CANFD_APBTR_TS1_63tp  = 0x3E,	/* 63 Time Segment 1 */
    CANFD_APBTR_TS1_64tp  = 0x3F,	/* 64 Time Segment 1 */

    CANFD_APBTR_TS1_65tp  = 0x40,	/* 65 Time Segment 1 */
    CANFD_APBTR_TS1_66tp  = 0x41,	/* 66 Time Segment 1 */
    CANFD_APBTR_TS1_67tp  = 0x42,	/* 67 Time Segment 1 */
    CANFD_APBTR_TS1_68tp  = 0x43,	/* 68 Time Segment 1 */
    CANFD_APBTR_TS1_69tp  = 0x44,	/* 69 Time Segment 1 */
    CANFD_APBTR_TS1_70tp  = 0x45,	/* 70 Time Segment 1 */
    CANFD_APBTR_TS1_71tp  = 0x46,	/* 71 Time Segment 1 */
    CANFD_APBTR_TS1_72tp  = 0x47,	/* 72 Time Segment 1 */
    CANFD_APBTR_TS1_73tp  = 0x48,	/* 73 Time Segment 1 */
    CANFD_APBTR_TS1_74tp  = 0x49,	/* 74 Time Segment 1 */
    CANFD_APBTR_TS1_75tp  = 0x4A,	/* 75 Time Segment 1 */
    CANFD_APBTR_TS1_76tp  = 0x4B,	/* 76 Time Segment 1 */
    CANFD_APBTR_TS1_77tp  = 0x4C,	/* 77 Time Segment 1 */
    CANFD_APBTR_TS1_78tp  = 0x4D,	/* 78 Time Segment 1 */
    CANFD_APBTR_TS1_79tp  = 0x4E,	/* 79 Time Segment 1 */
    CANFD_APBTR_TS1_80tp  = 0x4F,	/* 80 Time Segment 1 */

    CANFD_APBTR_TS1_81tp  = 0x50,	/* 81 Time Segment 1 */
    CANFD_APBTR_TS1_82tp  = 0x51,	/* 82 Time Segment 1 */
    CANFD_APBTR_TS1_83tp  = 0x52,	/* 83 Time Segment 1 */
    CANFD_APBTR_TS1_84tp  = 0x53,	/* 84 Time Segment 1 */
    CANFD_APBTR_TS1_85tp  = 0x54,	/* 85 Time Segment 1 */
    CANFD_APBTR_TS1_86tp  = 0x55,	/* 86 Time Segment 1 */
    CANFD_APBTR_TS1_87tp  = 0x56,	/* 87 Time Segment 1 */
    CANFD_APBTR_TS1_88tp  = 0x57,	/* 88 Time Segment 1 */
    CANFD_APBTR_TS1_89tp  = 0x58,	/* 89 Time Segment 1 */
    CANFD_APBTR_TS1_90tp  = 0x59,	/* 90 Time Segment 1 */
    CANFD_APBTR_TS1_91tp  = 0x5A,	/* 91 Time Segment 1 */
    CANFD_APBTR_TS1_92tp  = 0x5B,	/* 92 Time Segment 1 */
    CANFD_APBTR_TS1_93tp  = 0x5C,	/* 93 Time Segment 1 */
    CANFD_APBTR_TS1_94tp  = 0x5D,	/* 94 Time Segment 1 */
    CANFD_APBTR_TS1_95tp  = 0x5E,	/* 95 Time Segment 1 */
    CANFD_APBTR_TS1_96tp  = 0x5F,	/* 96 Time Segment 1 */

    CANFD_APBTR_TS1_97tp  = 0x60,	/* 97 Time Segment 1 */
    CANFD_APBTR_TS1_98tp  = 0x61,	/* 98 Time Segment 1 */
    CANFD_APBTR_TS1_99tp  = 0x62,	/* 99 Time Segment 1 */
    CANFD_APBTR_TS1_100tp = 0x63,	/* 100 Time Segment 1 */
    CANFD_APBTR_TS1_101tp = 0x64,	/* 101 Time Segment 1 */
    CANFD_APBTR_TS1_102tp = 0x65,	/* 102 Time Segment 1 */
    CANFD_APBTR_TS1_103tp = 0x66,	/* 103 Time Segment 1 */
    CANFD_APBTR_TS1_104tp = 0x67,	/* 104 Time Segment 1 */
    CANFD_APBTR_TS1_105tp = 0x68,	/* 105 Time Segment 1 */
    CANFD_APBTR_TS1_106tp = 0x69,	/* 106 Time Segment 1 */
    CANFD_APBTR_TS1_107tp = 0x6A,	/* 107 Time Segment 1 */
    CANFD_APBTR_TS1_108tp = 0x6B,	/* 108 Time Segment 1 */
    CANFD_APBTR_TS1_109tp = 0x6C,	/* 109 Time Segment 1 */
    CANFD_APBTR_TS1_110tp = 0x6D,	/* 110 Time Segment 1 */
    CANFD_APBTR_TS1_111tp = 0x6E,	/* 111 Time Segment 1 */
    CANFD_APBTR_TS1_112tp = 0x6F,	/* 112 Time Segment 1 */

    CANFD_APBTR_TS1_113tp = 0x70,	/* 113 Time Segment 1 */
    CANFD_APBTR_TS1_114tp = 0x71,	/* 114 Time Segment 1 */
    CANFD_APBTR_TS1_115tp = 0x72,	/* 115 Time Segment 1 */
    CANFD_APBTR_TS1_116tp = 0x73,	/* 116 Time Segment 1 */
    CANFD_APBTR_TS1_117tp = 0x74,	/* 117 Time Segment 1 */
    CANFD_APBTR_TS1_118tp = 0x75,	/* 118 Time Segment 1 */
    CANFD_APBTR_TS1_119tp = 0x76,	/* 119 Time Segment 1 */
    CANFD_APBTR_TS1_120tp = 0x77,	/* 120 Time Segment 1 */
    CANFD_APBTR_TS1_121tp = 0x78,	/* 121 Time Segment 1 */
    CANFD_APBTR_TS1_122tp = 0x79,	/* 122 Time Segment 1 */
    CANFD_APBTR_TS1_123tp = 0x7A,	/* 123 Time Segment 1 */
    CANFD_APBTR_TS1_124tp = 0x7B,	/* 124 Time Segment 1 */
    CANFD_APBTR_TS1_125tp = 0x7C,	/* 125 Time Segment 1 */
    CANFD_APBTR_TS1_126tp = 0x7D,	/* 126 Time Segment 1 */
    CANFD_APBTR_TS1_127tp = 0x7E,	/* 127 Time Segment 1 */
    CANFD_APBTR_TS1_128tp = 0x7F,	/* 128 Time Segment 1 */

    CANFD_APBTR_TS1_129tp = 0x80,	/* 129 Time Segment 1 */
    CANFD_APBTR_TS1_130tp = 0x81,	/* 130 Time Segment 1 */
    CANFD_APBTR_TS1_131tp = 0x82,	/* 131 Time Segment 1 */
    CANFD_APBTR_TS1_132tp = 0x83,	/* 132 Time Segment 1 */
    CANFD_APBTR_TS1_133tp = 0x84,	/* 133 Time Segment 1 */
    CANFD_APBTR_TS1_134tp = 0x85,	/* 134 Time Segment 1 */
    CANFD_APBTR_TS1_135tp = 0x86,	/* 135 Time Segment 1 */
    CANFD_APBTR_TS1_136tp = 0x87,	/* 136 Time Segment 1 */
    CANFD_APBTR_TS1_137tp = 0x88,	/* 137 Time Segment 1 */
    CANFD_APBTR_TS1_138tp = 0x89,	/* 138 Time Segment 1 */
    CANFD_APBTR_TS1_139tp = 0x8A,	/* 139 Time Segment 1 */
    CANFD_APBTR_TS1_140tp = 0x8B,	/* 140 Time Segment 1 */
    CANFD_APBTR_TS1_141tp = 0x8C,	/* 141 Time Segment 1 */
    CANFD_APBTR_TS1_142tp = 0x8D,	/* 142 Time Segment 1 */
    CANFD_APBTR_TS1_143tp = 0x8E,	/* 143 Time Segment 1 */
    CANFD_APBTR_TS1_144tp = 0x8F,	/* 144 Time Segment 1 */

    CANFD_APBTR_TS1_145tp = 0x90,	/* 145 Time Segment 1 */
    CANFD_APBTR_TS1_146tp = 0x91,	/* 146 Time Segment 1 */
    CANFD_APBTR_TS1_147tp = 0x92,	/* 147 Time Segment 1 */
    CANFD_APBTR_TS1_148tp = 0x93,	/* 148 Time Segment 1 */
    CANFD_APBTR_TS1_149tp = 0x94,	/* 149 Time Segment 1 */
    CANFD_APBTR_TS1_150tp = 0x95,	/* 150 Time Segment 1 */
    CANFD_APBTR_TS1_151tp = 0x96,	/* 151 Time Segment 1 */
    CANFD_APBTR_TS1_152tp = 0x97,	/* 152 Time Segment 1 */
    CANFD_APBTR_TS1_153tp = 0x98,	/* 153 Time Segment 1 */
    CANFD_APBTR_TS1_154tp = 0x99,	/* 154 Time Segment 1 */
    CANFD_APBTR_TS1_155tp = 0x9A,	/* 155 Time Segment 1 */
    CANFD_APBTR_TS1_156tp = 0x9B,	/* 156 Time Segment 1 */
    CANFD_APBTR_TS1_157tp = 0x9C,	/* 157 Time Segment 1 */
    CANFD_APBTR_TS1_158tp = 0x9D,	/* 158 Time Segment 1 */
    CANFD_APBTR_TS1_159tp = 0x9E,	/* 159 Time Segment 1 */
    CANFD_APBTR_TS1_160tp = 0x9F,	/* 160 Time Segment 1 */

    CANFD_APBTR_TS1_161tp = 0xA0,	/* 161 Time Segment 1 */
    CANFD_APBTR_TS1_162tp = 0xA1,	/* 162 Time Segment 1 */
    CANFD_APBTR_TS1_163tp = 0xA2,	/* 163 Time Segment 1 */
    CANFD_APBTR_TS1_164tp = 0xA3,	/* 164 Time Segment 1 */
    CANFD_APBTR_TS1_165tp = 0xA4,	/* 165 Time Segment 1 */
    CANFD_APBTR_TS1_166tp = 0xA5,	/* 166 Time Segment 1 */
    CANFD_APBTR_TS1_167tp = 0xA6,	/* 167 Time Segment 1 */
    CANFD_APBTR_TS1_168tp = 0xA7,	/* 168 Time Segment 1 */
    CANFD_APBTR_TS1_169tp = 0xA8,	/* 169 Time Segment 1 */
    CANFD_APBTR_TS1_170tp = 0xA9,	/* 170 Time Segment 1 */
    CANFD_APBTR_TS1_171tp = 0xAA,	/* 171 Time Segment 1 */
    CANFD_APBTR_TS1_172tp = 0xAB,	/* 172 Time Segment 1 */
    CANFD_APBTR_TS1_173tp = 0xAC,	/* 173 Time Segment 1 */
    CANFD_APBTR_TS1_174tp = 0xAD,	/* 174 Time Segment 1 */
    CANFD_APBTR_TS1_175tp = 0xAE,	/* 175 Time Segment 1 */
    CANFD_APBTR_TS1_176tp = 0xAF,	/* 176 Time Segment 1 */

    CANFD_APBTR_TS1_177tp = 0xB0,	/* 177 Time Segment 1 */
    CANFD_APBTR_TS1_178tp = 0xB1,	/* 178 Time Segment 1 */
    CANFD_APBTR_TS1_179tp = 0xB2,	/* 179 Time Segment 1 */
    CANFD_APBTR_TS1_180tp = 0xB3,	/* 180 Time Segment 1 */
    CANFD_APBTR_TS1_181tp = 0xB4,	/* 181 Time Segment 1 */
    CANFD_APBTR_TS1_182tp = 0xB5,	/* 182 Time Segment 1 */
    CANFD_APBTR_TS1_183tp = 0xB6,	/* 183 Time Segment 1 */
    CANFD_APBTR_TS1_184tp = 0xB7,	/* 184 Time Segment 1 */
    CANFD_APBTR_TS1_185tp = 0xB8,	/* 185 Time Segment 1 */
    CANFD_APBTR_TS1_186tp = 0xB9,	/* 186 Time Segment 1 */
    CANFD_APBTR_TS1_187tp = 0xBA,	/* 187 Time Segment 1 */
    CANFD_APBTR_TS1_188tp = 0xBB,	/* 188 Time Segment 1 */
    CANFD_APBTR_TS1_189tp = 0xBC,	/* 189 Time Segment 1 */
    CANFD_APBTR_TS1_190tp = 0xBD,	/* 190 Time Segment 1 */
    CANFD_APBTR_TS1_191tp = 0xBE,	/* 191 Time Segment 1 */
    CANFD_APBTR_TS1_192tp = 0xBF,	/* 192 Time Segment 1 */

    CANFD_APBTR_TS1_193tp = 0xC0,	/* 193 Time Segment 1 */
    CANFD_APBTR_TS1_194tp = 0xC1,	/* 194 Time Segment 1 */
    CANFD_APBTR_TS1_195tp = 0xC2,	/* 195 Time Segment 1 */
    CANFD_APBTR_TS1_196tp = 0xC3,	/* 196 Time Segment 1 */
    CANFD_APBTR_TS1_197tp = 0xC4,	/* 197 Time Segment 1 */
    CANFD_APBTR_TS1_198tp = 0xC5,	/* 198 Time Segment 1 */
    CANFD_APBTR_TS1_199tp = 0xC6,	/* 199 Time Segment 1 */
    CANFD_APBTR_TS1_200tp = 0xC7,	/* 200 Time Segment 1 */
    CANFD_APBTR_TS1_201tp = 0xC8,	/* 201 Time Segment 1 */
    CANFD_APBTR_TS1_202tp = 0xC9,	/* 202 Time Segment 1 */
    CANFD_APBTR_TS1_203tp = 0xCA,	/* 203 Time Segment 1 */
    CANFD_APBTR_TS1_204tp = 0xCB,	/* 204 Time Segment 1 */
    CANFD_APBTR_TS1_205tp = 0xCC,	/* 205 Time Segment 1 */
    CANFD_APBTR_TS1_206tp = 0xCD,	/* 206 Time Segment 1 */
    CANFD_APBTR_TS1_207tp = 0xCE,	/* 207 Time Segment 1 */
    CANFD_APBTR_TS1_208tp = 0xCF,	/* 208 Time Segment 1 */

    CANFD_APBTR_TS1_209tp = 0xD0,	/* 209 Time Segment 1 */
    CANFD_APBTR_TS1_210tp = 0xD1,	/* 210 Time Segment 1 */
    CANFD_APBTR_TS1_211tp = 0xD2,	/* 211 Time Segment 1 */
    CANFD_APBTR_TS1_212tp = 0xD3,	/* 212 Time Segment 1 */
    CANFD_APBTR_TS1_213tp = 0xD4,	/* 213 Time Segment 1 */
    CANFD_APBTR_TS1_214tp = 0xD5,	/* 214 Time Segment 1 */
    CANFD_APBTR_TS1_215tp = 0xD6,	/* 215 Time Segment 1 */
    CANFD_APBTR_TS1_216tp = 0xD7,	/* 216 Time Segment 1 */
    CANFD_APBTR_TS1_217tp = 0xD8,	/* 217 Time Segment 1 */
    CANFD_APBTR_TS1_218tp = 0xD9,	/* 218 Time Segment 1 */
    CANFD_APBTR_TS1_219tp = 0xDA,	/* 219 Time Segment 1 */
    CANFD_APBTR_TS1_220tp = 0xDB,	/* 220 Time Segment 1 */
    CANFD_APBTR_TS1_221tp = 0xDC,	/* 221 Time Segment 1 */
    CANFD_APBTR_TS1_222tp = 0xDD,	/* 222 Time Segment 1 */
    CANFD_APBTR_TS1_223tp = 0xDE,	/* 223 Time Segment 1 */
    CANFD_APBTR_TS1_224tp = 0xDF,	/* 224 Time Segment 1 */

    CANFD_APBTR_TS1_225tp = 0xE0,	/* 225 Time Segment 1 */
    CANFD_APBTR_TS1_226tp = 0xE1,	/* 226 Time Segment 1 */
    CANFD_APBTR_TS1_227tp = 0xE2,	/* 227 Time Segment 1 */
    CANFD_APBTR_TS1_228tp = 0xE3,	/* 228 Time Segment 1 */
    CANFD_APBTR_TS1_229tp = 0xE4,	/* 229 Time Segment 1 */
    CANFD_APBTR_TS1_230tp = 0xE5,	/* 230 Time Segment 1 */
    CANFD_APBTR_TS1_231tp = 0xE6,	/* 231 Time Segment 1 */
    CANFD_APBTR_TS1_232tp = 0xE7,	/* 232 Time Segment 1 */
    CANFD_APBTR_TS1_233tp = 0xE8,	/* 233 Time Segment 1 */
    CANFD_APBTR_TS1_234tp = 0xE9,	/* 234 Time Segment 1 */
    CANFD_APBTR_TS1_235tp = 0xEA,	/* 235 Time Segment 1 */
    CANFD_APBTR_TS1_236tp = 0xEB,	/* 236 Time Segment 1 */
    CANFD_APBTR_TS1_237tp = 0xEC,	/* 237 Time Segment 1 */
    CANFD_APBTR_TS1_238tp = 0xED,	/* 238 Time Segment 1 */
    CANFD_APBTR_TS1_239tp = 0xEE,	/* 239 Time Segment 1 */
    CANFD_APBTR_TS1_240tp = 0xEF,	/* 240 Time Segment 1 */

    CANFD_APBTR_TS1_241tp = 0xF0,	/* 241 Time Segment 1 */
    CANFD_APBTR_TS1_242tp = 0xF1,	/* 242 Time Segment 1 */
    CANFD_APBTR_TS1_243tp = 0xF2,	/* 243 Time Segment 1 */
    CANFD_APBTR_TS1_244tp = 0xF3,	/* 244 Time Segment 1 */
    CANFD_APBTR_TS1_245tp = 0xF4,	/* 245 Time Segment 1 */
    CANFD_APBTR_TS1_246tp = 0xF5,	/* 246 Time Segment 1 */
    CANFD_APBTR_TS1_247tp = 0xF6,	/* 247 Time Segment 1 */
    CANFD_APBTR_TS1_248tp = 0xF7,	/* 248 Time Segment 1 */
    CANFD_APBTR_TS1_249tp = 0xF8,	/* 249 Time Segment 1 */
    CANFD_APBTR_TS1_250tp = 0xF9,	/* 250 Time Segment 1 */
    CANFD_APBTR_TS1_251tp = 0xFA,	/* 251 Time Segment 1 */
    CANFD_APBTR_TS1_252tp = 0xFB,	/* 252 Time Segment 1 */
    CANFD_APBTR_TS1_253tp = 0xFC,	/* 253 Time Segment 1 */
    CANFD_APBTR_TS1_254tp = 0xFD,	/* 254 Time Segment 1 */
    CANFD_APBTR_TS1_255tp = 0xFE,	/* 255 Time Segment 1 */
    CANFD_APBTR_TS1_256tp = 0xFF,	/* 256 Time Segment 1 */
}CANFDAPBTRts1_TypeDef;
#define IS_CANFD_APBTR_TS1(APBTRTS1TP) (((APBTRTS1TP) == CANFD_APBTR_TS1_1tp)   || ((APBTRTS1TP) == CANFD_APBTR_TS1_2tp)   || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_3tp)   || ((APBTRTS1TP) == CANFD_APBTR_TS1_4tp)   || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_5tp)   || ((APBTRTS1TP) == CANFD_APBTR_TS1_6tp)   || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_7tp)   || ((APBTRTS1TP) == CANFD_APBTR_TS1_8tp)   || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_9tp)   || ((APBTRTS1TP) == CANFD_APBTR_TS1_10tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_11tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_12tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_13tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_14tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_15tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_16tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_17tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_18tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_19tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_20tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_21tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_22tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_23tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_24tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_25tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_26tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_27tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_28tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_29tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_30tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_31tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_32tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_33tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_34tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_35tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_36tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_37tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_38tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_39tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_40tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_41tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_42tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_43tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_44tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_45tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_46tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_47tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_48tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_49tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_50tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_51tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_52tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_53tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_54tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_55tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_56tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_57tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_58tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_59tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_60tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_61tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_62tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_63tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_64tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_65tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_66tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_67tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_68tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_69tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_70tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_71tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_72tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_73tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_74tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_75tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_76tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_77tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_78tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_79tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_80tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_81tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_82tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_83tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_84tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_85tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_86tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_87tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_88tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_89tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_90tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_91tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_92tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_93tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_94tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_95tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_96tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_97tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_98tp)  || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_99tp)  || ((APBTRTS1TP) == CANFD_APBTR_TS1_100tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_101tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_102tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_103tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_104tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_105tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_106tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_107tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_108tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_109tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_110tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_111tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_112tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_113tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_114tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_115tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_116tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_117tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_118tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_119tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_120tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_121tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_122tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_123tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_124tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_125tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_126tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_127tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_128tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_129tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_130tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_131tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_132tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_133tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_134tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_135tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_136tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_137tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_138tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_139tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_140tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_141tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_142tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_143tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_144tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_145tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_146tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_147tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_148tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_149tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_150tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_151tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_152tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_153tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_154tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_155tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_156tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_157tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_158tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_159tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_160tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_161tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_162tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_163tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_164tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_165tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_166tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_167tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_168tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_169tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_170tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_171tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_172tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_173tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_174tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_175tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_176tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_177tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_178tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_179tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_180tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_181tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_182tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_183tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_184tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_185tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_186tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_187tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_188tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_189tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_190tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_191tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_192tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_193tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_194tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_195tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_196tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_197tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_198tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_199tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_200tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_201tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_202tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_203tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_204tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_205tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_206tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_207tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_208tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_209tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_210tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_211tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_212tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_213tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_214tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_215tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_216tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_217tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_218tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_219tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_220tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_221tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_222tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_223tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_224tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_225tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_226tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_227tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_228tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_229tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_230tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_231tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_232tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_233tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_234tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_235tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_236tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_237tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_238tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_239tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_240tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_241tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_242tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_243tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_244tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_245tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_246tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_247tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_248tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_249tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_250tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_251tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_252tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_253tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_254tp) || \
                                    	((APBTRTS1TP) == CANFD_APBTR_TS1_255tp) || ((APBTRTS1TP) == CANFD_APBTR_TS1_256tp))


/*
 * @brief CANFD Arbitration Phase (Nominal) Bit Timing (Time Segment 2) enumeration
 */
typedef enum
{
    CANFD_APBTR_TS2_1tp   = 0x00,	/* 1 Time Segment 2 */
    CANFD_APBTR_TS2_2tp   = 0x01,	/* 2 Time Segment 2 */
    CANFD_APBTR_TS2_3tp   = 0x02,	/* 3 Time Segment 2 */
    CANFD_APBTR_TS2_4tp   = 0x03,	/* 4 Time Segment 2 */
    CANFD_APBTR_TS2_5tp   = 0x04,	/* 5 Time Segment 2 */
    CANFD_APBTR_TS2_6tp   = 0x05,	/* 6 Time Segment 2 */
    CANFD_APBTR_TS2_7tp   = 0x06,	/* 7 Time Segment 2 */
    CANFD_APBTR_TS2_8tp   = 0x07,	/* 8 Time Segment 2 */
    CANFD_APBTR_TS2_9tp   = 0x08,	/* 9 Time Segment 2 */
    CANFD_APBTR_TS2_10tp  = 0x09,	/* 10 Time Segment 2 */
    CANFD_APBTR_TS2_11tp  = 0x0A,	/* 11 Time Segment 2 */
    CANFD_APBTR_TS2_12tp  = 0x0B,	/* 12 Time Segment 2 */
    CANFD_APBTR_TS2_13tp  = 0x0C,	/* 13 Time Segment 2 */
    CANFD_APBTR_TS2_14tp  = 0x0D,	/* 14 Time Segment 2 */
    CANFD_APBTR_TS2_15tp  = 0x0E,	/* 15 Time Segment 2 */
    CANFD_APBTR_TS2_16tp  = 0x0F,	/* 16 Time Segment 2 */

    CANFD_APBTR_TS2_17tp  = 0x10,	/* 17 Time Segment 2 */
    CANFD_APBTR_TS2_18tp  = 0x11,	/* 18 Time Segment 2 */
    CANFD_APBTR_TS2_19tp  = 0x12,	/* 19 Time Segment 2 */
    CANFD_APBTR_TS2_20tp  = 0x13,	/* 20 Time Segment 2 */
    CANFD_APBTR_TS2_21tp  = 0x14,	/* 21 Time Segment 2 */
    CANFD_APBTR_TS2_22tp  = 0x15,	/* 22 Time Segment 2 */
    CANFD_APBTR_TS2_23tp  = 0x16,	/* 23 Time Segment 2 */
    CANFD_APBTR_TS2_24tp  = 0x17,	/* 24 Time Segment 2 */
    CANFD_APBTR_TS2_25tp  = 0x18,	/* 25 Time Segment 2 */
    CANFD_APBTR_TS2_26tp  = 0x19,	/* 26 Time Segment 2 */
    CANFD_APBTR_TS2_27tp  = 0x1A,	/* 27 Time Segment 2 */
    CANFD_APBTR_TS2_28tp  = 0x1B,	/* 28 Time Segment 2 */
    CANFD_APBTR_TS2_29tp  = 0x1C,	/* 29 Time Segment 2 */
    CANFD_APBTR_TS2_30tp  = 0x1D,	/* 30 Time Segment 2 */
    CANFD_APBTR_TS2_31tp  = 0x1E,	/* 31 Time Segment 2 */
    CANFD_APBTR_TS2_32tp  = 0x1F,	/* 32 Time Segment 2 */

    CANFD_APBTR_TS2_33tp  = 0x20,	/* 33 Time Segment 2 */
    CANFD_APBTR_TS2_34tp  = 0x21,	/* 34 Time Segment 2 */
    CANFD_APBTR_TS2_35tp  = 0x22,	/* 35 Time Segment 2 */
    CANFD_APBTR_TS2_36tp  = 0x23,	/* 36 Time Segment 2 */
    CANFD_APBTR_TS2_37tp  = 0x24,	/* 37 Time Segment 2 */
    CANFD_APBTR_TS2_38tp  = 0x25,	/* 38 Time Segment 2 */
    CANFD_APBTR_TS2_39tp  = 0x26,	/* 39 Time Segment 2 */
    CANFD_APBTR_TS2_40tp  = 0x27,	/* 40 Time Segment 2 */
    CANFD_APBTR_TS2_41tp  = 0x28,	/* 41 Time Segment 2 */
    CANFD_APBTR_TS2_42tp  = 0x29,	/* 42 Time Segment 2 */
    CANFD_APBTR_TS2_43tp  = 0x2A,	/* 43 Time Segment 2 */
    CANFD_APBTR_TS2_44tp  = 0x2B,	/* 44 Time Segment 2 */
    CANFD_APBTR_TS2_45tp  = 0x2C,	/* 45 Time Segment 2 */
    CANFD_APBTR_TS2_46tp  = 0x2D,	/* 46 Time Segment 2 */
    CANFD_APBTR_TS2_47tp  = 0x2E,	/* 47 Time Segment 2 */
    CANFD_APBTR_TS2_48tp  = 0x2F,	/* 48 Time Segment 2 */

    CANFD_APBTR_TS2_49tp  = 0x30,	/* 49 Time Segment 2 */
    CANFD_APBTR_TS2_50tp  = 0x31,	/* 50 Time Segment 2 */
    CANFD_APBTR_TS2_51tp  = 0x32,	/* 51 Time Segment 2 */
    CANFD_APBTR_TS2_52tp  = 0x33,	/* 52 Time Segment 2 */
    CANFD_APBTR_TS2_53tp  = 0x34,	/* 53 Time Segment 2 */
    CANFD_APBTR_TS2_54tp  = 0x35,	/* 54 Time Segment 2 */
    CANFD_APBTR_TS2_55tp  = 0x36,	/* 55 Time Segment 2 */
    CANFD_APBTR_TS2_56tp  = 0x37,	/* 56 Time Segment 2 */
    CANFD_APBTR_TS2_57tp  = 0x38,	/* 57 Time Segment 2 */
    CANFD_APBTR_TS2_58tp  = 0x39,	/* 58 Time Segment 2 */
    CANFD_APBTR_TS2_59tp  = 0x3A,	/* 59 Time Segment 2 */
    CANFD_APBTR_TS2_60tp  = 0x3B,	/* 60 Time Segment 2 */
    CANFD_APBTR_TS2_61tp  = 0x3C,	/* 61 Time Segment 2 */
    CANFD_APBTR_TS2_62tp  = 0x3D,	/* 62 Time Segment 2 */
    CANFD_APBTR_TS2_63tp  = 0x3E,	/* 63 Time Segment 2 */
    CANFD_APBTR_TS2_64tp  = 0x3F,	/* 64 Time Segment 2 */

    CANFD_APBTR_TS2_65tp  = 0x40,	/* 65 Time Segment 2 */
    CANFD_APBTR_TS2_66tp  = 0x41,	/* 66 Time Segment 2 */
    CANFD_APBTR_TS2_67tp  = 0x42,	/* 67 Time Segment 2 */
    CANFD_APBTR_TS2_68tp  = 0x43,	/* 68 Time Segment 2 */
    CANFD_APBTR_TS2_69tp  = 0x44,	/* 69 Time Segment 2 */
    CANFD_APBTR_TS2_70tp  = 0x45,	/* 70 Time Segment 2 */
    CANFD_APBTR_TS2_71tp  = 0x46,	/* 71 Time Segment 2 */
    CANFD_APBTR_TS2_72tp  = 0x47,	/* 72 Time Segment 2 */
    CANFD_APBTR_TS2_73tp  = 0x48,	/* 73 Time Segment 2 */
    CANFD_APBTR_TS2_74tp  = 0x49,	/* 74 Time Segment 2 */
    CANFD_APBTR_TS2_75tp  = 0x4A,	/* 75 Time Segment 2 */
    CANFD_APBTR_TS2_76tp  = 0x4B,	/* 76 Time Segment 2 */
    CANFD_APBTR_TS2_77tp  = 0x4C,	/* 77 Time Segment 2 */
    CANFD_APBTR_TS2_78tp  = 0x4D,	/* 78 Time Segment 2 */
    CANFD_APBTR_TS2_79tp  = 0x4E,	/* 79 Time Segment 2 */
    CANFD_APBTR_TS2_80tp  = 0x4F,	/* 80 Time Segment 2 */

    CANFD_APBTR_TS2_81tp  = 0x50,	/* 81 Time Segment 2 */
    CANFD_APBTR_TS2_82tp  = 0x51,	/* 82 Time Segment 2 */
    CANFD_APBTR_TS2_83tp  = 0x52,	/* 83 Time Segment 2 */
    CANFD_APBTR_TS2_84tp  = 0x53,	/* 84 Time Segment 2 */
    CANFD_APBTR_TS2_85tp  = 0x54,	/* 85 Time Segment 2 */
    CANFD_APBTR_TS2_86tp  = 0x55,	/* 86 Time Segment 2 */
    CANFD_APBTR_TS2_87tp  = 0x56,	/* 87 Time Segment 2 */
    CANFD_APBTR_TS2_88tp  = 0x57,	/* 88 Time Segment 2 */
    CANFD_APBTR_TS2_89tp  = 0x58,	/* 89 Time Segment 2 */
    CANFD_APBTR_TS2_90tp  = 0x59,	/* 90 Time Segment 2 */
    CANFD_APBTR_TS2_91tp  = 0x5A,	/* 91 Time Segment 2 */
    CANFD_APBTR_TS2_92tp  = 0x5B,	/* 92 Time Segment 2 */
    CANFD_APBTR_TS2_93tp  = 0x5C,	/* 93 Time Segment 2 */
    CANFD_APBTR_TS2_94tp  = 0x5D,	/* 94 Time Segment 2 */
    CANFD_APBTR_TS2_95tp  = 0x5E,	/* 95 Time Segment 2 */
    CANFD_APBTR_TS2_96tp  = 0x5F,	/* 96 Time Segment 2 */

    CANFD_APBTR_TS2_97tp  = 0x60,	/* 97 Time Segment 2 */
    CANFD_APBTR_TS2_98tp  = 0x61,	/* 98 Time Segment 2 */
    CANFD_APBTR_TS2_99tp  = 0x62,	/* 99 Time Segment 2 */
    CANFD_APBTR_TS2_100tp = 0x63,	/* 100 Time Segment 2 */
    CANFD_APBTR_TS2_101tp = 0x64,	/* 101 Time Segment 2 */
    CANFD_APBTR_TS2_102tp = 0x65,	/* 102 Time Segment 2 */
    CANFD_APBTR_TS2_103tp = 0x66,	/* 103 Time Segment 2 */
    CANFD_APBTR_TS2_104tp = 0x67,	/* 104 Time Segment 2 */
    CANFD_APBTR_TS2_105tp = 0x68,	/* 105 Time Segment 2 */
    CANFD_APBTR_TS2_106tp = 0x69,	/* 106 Time Segment 2 */
    CANFD_APBTR_TS2_107tp = 0x6A,	/* 107 Time Segment 2 */
    CANFD_APBTR_TS2_108tp = 0x6B,	/* 108 Time Segment 2 */
    CANFD_APBTR_TS2_109tp = 0x6C,	/* 109 Time Segment 2 */
    CANFD_APBTR_TS2_110tp = 0x6D,	/* 110 Time Segment 2 */
    CANFD_APBTR_TS2_111tp = 0x6E,	/* 111 Time Segment 2 */
    CANFD_APBTR_TS2_112tp = 0x6F,	/* 112 Time Segment 2 */

    CANFD_APBTR_TS2_113tp = 0x70,	/* 113 Time Segment 2 */
    CANFD_APBTR_TS2_114tp = 0x71,	/* 114 Time Segment 2 */
    CANFD_APBTR_TS2_115tp = 0x72,	/* 115 Time Segment 2 */
    CANFD_APBTR_TS2_116tp = 0x73,	/* 116 Time Segment 2 */
    CANFD_APBTR_TS2_117tp = 0x74,	/* 117 Time Segment 2 */
    CANFD_APBTR_TS2_118tp = 0x75,	/* 118 Time Segment 2 */
    CANFD_APBTR_TS2_119tp = 0x76,	/* 119 Time Segment 2 */
    CANFD_APBTR_TS2_120tp = 0x77,	/* 120 Time Segment 2 */
    CANFD_APBTR_TS2_121tp = 0x78,	/* 121 Time Segment 2 */
    CANFD_APBTR_TS2_122tp = 0x79,	/* 122 Time Segment 2 */
    CANFD_APBTR_TS2_123tp = 0x7A,	/* 123 Time Segment 2 */
    CANFD_APBTR_TS2_124tp = 0x7B,	/* 124 Time Segment 2 */
    CANFD_APBTR_TS2_125tp = 0x7C,	/* 125 Time Segment 2 */
    CANFD_APBTR_TS2_126tp = 0x7D,	/* 126 Time Segment 2 */
    CANFD_APBTR_TS2_127tp = 0x7E,	/* 127 Time Segment 2 */
    CANFD_APBTR_TS2_128tp = 0x7F,   /* 128 Time Segment 2 */
}CANFDAPBTRts2_TypeDef;
#define IS_CANFD_APBTR_TS2(APBTRTS2TP) (((APBTRTS2TP) == CANFD_APBTR_TS2_1tp)   || ((APBTRTS2TP) == CANFD_APBTR_TS2_2tp)   || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_3tp)   || ((APBTRTS2TP) == CANFD_APBTR_TS2_4tp)   || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_5tp)   || ((APBTRTS2TP) == CANFD_APBTR_TS2_6tp)   || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_7tp)   || ((APBTRTS2TP) == CANFD_APBTR_TS2_8tp)   || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_9tp)   || ((APBTRTS2TP) == CANFD_APBTR_TS2_10tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_11tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_12tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_13tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_14tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_15tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_16tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_17tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_18tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_19tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_20tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_21tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_22tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_23tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_24tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_25tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_26tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_27tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_28tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_29tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_30tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_31tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_32tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_33tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_34tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_35tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_36tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_37tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_38tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_39tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_40tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_41tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_42tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_43tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_44tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_45tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_46tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_47tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_48tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_49tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_50tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_51tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_52tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_53tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_54tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_55tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_56tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_57tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_58tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_59tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_60tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_61tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_62tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_63tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_64tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_65tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_66tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_67tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_68tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_69tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_70tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_71tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_72tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_73tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_74tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_75tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_76tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_77tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_78tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_79tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_80tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_81tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_82tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_83tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_84tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_85tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_86tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_87tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_88tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_89tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_90tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_91tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_92tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_93tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_94tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_95tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_96tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_97tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_98tp)  || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_99tp)  || ((APBTRTS2TP) == CANFD_APBTR_TS2_100tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_101tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_102tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_103tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_104tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_105tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_106tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_107tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_108tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_109tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_110tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_111tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_112tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_113tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_114tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_115tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_116tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_117tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_118tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_119tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_120tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_121tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_122tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_123tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_124tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_125tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_126tp) || \
                                    	((APBTRTS2TP) == CANFD_APBTR_TS2_127tp) || ((APBTRTS2TP) == CANFD_APBTR_TS2_128tp))

/*
 * @brief CANFD Arbitration Phase (Nominal) Bit Timing (Synchronization Jump Width) enumeration
 */
typedef enum
{
    CANFD_APBTR_SJW_1tp   = 0x00,	/* 1 Synchronization Jump Width */
    CANFD_APBTR_SJW_2tp   = 0x01,	/* 2 Synchronization Jump Width */
    CANFD_APBTR_SJW_3tp   = 0x02,	/* 3 Synchronization Jump Width */
    CANFD_APBTR_SJW_4tp   = 0x03,	/* 4 Synchronization Jump Width */
    CANFD_APBTR_SJW_5tp   = 0x04,	/* 5 Synchronization Jump Width */
    CANFD_APBTR_SJW_6tp   = 0x05,	/* 6 Synchronization Jump Width */
    CANFD_APBTR_SJW_7tp   = 0x06,	/* 7 Synchronization Jump Width */
    CANFD_APBTR_SJW_8tp   = 0x07,	/* 8 Synchronization Jump Width */
    CANFD_APBTR_SJW_9tp   = 0x08,	/* 9 Synchronization Jump Width */
    CANFD_APBTR_SJW_10tp  = 0x09,	/* 10 Synchronization Jump Width */
    CANFD_APBTR_SJW_11tp  = 0x0A,	/* 11 Synchronization Jump Width */
    CANFD_APBTR_SJW_12tp  = 0x0B,	/* 12 Synchronization Jump Width */
    CANFD_APBTR_SJW_13tp  = 0x0C,	/* 13 Synchronization Jump Width */
    CANFD_APBTR_SJW_14tp  = 0x0D,	/* 14 Synchronization Jump Width */
    CANFD_APBTR_SJW_15tp  = 0x0E,	/* 15 Synchronization Jump Width */
    CANFD_APBTR_SJW_16tp  = 0x0F,	/* 16 Synchronization Jump Width */

    CANFD_APBTR_SJW_17tp  = 0x10,	/* 17 Synchronization Jump Width */
    CANFD_APBTR_SJW_18tp  = 0x11,	/* 18 Synchronization Jump Width */
    CANFD_APBTR_SJW_19tp  = 0x12,	/* 19 Synchronization Jump Width */
    CANFD_APBTR_SJW_20tp  = 0x13,	/* 20 Synchronization Jump Width */
    CANFD_APBTR_SJW_21tp  = 0x14,	/* 21 Synchronization Jump Width */
    CANFD_APBTR_SJW_22tp  = 0x15,	/* 22 Synchronization Jump Width */
    CANFD_APBTR_SJW_23tp  = 0x16,	/* 23 Synchronization Jump Width */
    CANFD_APBTR_SJW_24tp  = 0x17,	/* 24 Synchronization Jump Width */
    CANFD_APBTR_SJW_25tp  = 0x18,	/* 25 Synchronization Jump Width */
    CANFD_APBTR_SJW_26tp  = 0x19,	/* 26 Synchronization Jump Width */
    CANFD_APBTR_SJW_27tp  = 0x1A,	/* 27 Synchronization Jump Width */
    CANFD_APBTR_SJW_28tp  = 0x1B,	/* 28 Synchronization Jump Width */
    CANFD_APBTR_SJW_29tp  = 0x1C,	/* 29 Synchronization Jump Width */
    CANFD_APBTR_SJW_30tp  = 0x1D,	/* 30 Synchronization Jump Width */
    CANFD_APBTR_SJW_31tp  = 0x1E,	/* 31 Synchronization Jump Width */
    CANFD_APBTR_SJW_32tp  = 0x1F,	/* 32 Synchronization Jump Width */

    CANFD_APBTR_SJW_33tp  = 0x20,	/* 33 Synchronization Jump Width */
    CANFD_APBTR_SJW_34tp  = 0x21,	/* 34 Synchronization Jump Width */
    CANFD_APBTR_SJW_35tp  = 0x22,	/* 35 Synchronization Jump Width */
    CANFD_APBTR_SJW_36tp  = 0x23,	/* 36 Synchronization Jump Width */
    CANFD_APBTR_SJW_37tp  = 0x24,	/* 37 Synchronization Jump Width */
    CANFD_APBTR_SJW_38tp  = 0x25,	/* 38 Synchronization Jump Width */
    CANFD_APBTR_SJW_39tp  = 0x26,	/* 39 Synchronization Jump Width */
    CANFD_APBTR_SJW_40tp  = 0x27,	/* 40 Synchronization Jump Width */
    CANFD_APBTR_SJW_41tp  = 0x28,	/* 41 Synchronization Jump Width */
    CANFD_APBTR_SJW_42tp  = 0x29,	/* 42 Synchronization Jump Width */
    CANFD_APBTR_SJW_43tp  = 0x2A,	/* 43 Synchronization Jump Width */
    CANFD_APBTR_SJW_44tp  = 0x2B,	/* 44 Synchronization Jump Width */
    CANFD_APBTR_SJW_45tp  = 0x2C,	/* 45 Synchronization Jump Width */
    CANFD_APBTR_SJW_46tp  = 0x2D,	/* 46 Synchronization Jump Width */
    CANFD_APBTR_SJW_47tp  = 0x2E,	/* 47 Synchronization Jump Width */
    CANFD_APBTR_SJW_48tp  = 0x2F,	/* 48 Synchronization Jump Width */

    CANFD_APBTR_SJW_49tp  = 0x30,	/* 49 Synchronization Jump Width */
    CANFD_APBTR_SJW_50tp  = 0x31,	/* 50 Synchronization Jump Width */
    CANFD_APBTR_SJW_51tp  = 0x32,	/* 51 Synchronization Jump Width */
    CANFD_APBTR_SJW_52tp  = 0x33,	/* 52 Synchronization Jump Width */
    CANFD_APBTR_SJW_53tp  = 0x34,	/* 53 Synchronization Jump Width */
    CANFD_APBTR_SJW_54tp  = 0x35,	/* 54 Synchronization Jump Width */
    CANFD_APBTR_SJW_55tp  = 0x36,	/* 55 Synchronization Jump Width */
    CANFD_APBTR_SJW_56tp  = 0x37,	/* 56 Synchronization Jump Width */
    CANFD_APBTR_SJW_57tp  = 0x38,	/* 57 Synchronization Jump Width */
    CANFD_APBTR_SJW_58tp  = 0x39,	/* 58 Synchronization Jump Width */
    CANFD_APBTR_SJW_59tp  = 0x3A,	/* 59 Synchronization Jump Width */
    CANFD_APBTR_SJW_60tp  = 0x3B,	/* 60 Synchronization Jump Width */
    CANFD_APBTR_SJW_61tp  = 0x3C,	/* 61 Synchronization Jump Width */
    CANFD_APBTR_SJW_62tp  = 0x3D,	/* 62 Synchronization Jump Width */
    CANFD_APBTR_SJW_63tp  = 0x3E,	/* 63 Synchronization Jump Width */
    CANFD_APBTR_SJW_64tp  = 0x3F,	/* 64 Synchronization Jump Width */

    CANFD_APBTR_SJW_65tp  = 0x40,	/* 65 Synchronization Jump Width */
    CANFD_APBTR_SJW_66tp  = 0x41,	/* 66 Synchronization Jump Width */
    CANFD_APBTR_SJW_67tp  = 0x42,	/* 67 Synchronization Jump Width */
    CANFD_APBTR_SJW_68tp  = 0x43,	/* 68 Synchronization Jump Width */
    CANFD_APBTR_SJW_69tp  = 0x44,	/* 69 Synchronization Jump Width */
    CANFD_APBTR_SJW_70tp  = 0x45,	/* 70 Synchronization Jump Width */
    CANFD_APBTR_SJW_71tp  = 0x46,	/* 71 Synchronization Jump Width */
    CANFD_APBTR_SJW_72tp  = 0x47,	/* 72 Synchronization Jump Width */
    CANFD_APBTR_SJW_73tp  = 0x48,	/* 73 Synchronization Jump Width */
    CANFD_APBTR_SJW_74tp  = 0x49,	/* 74 Synchronization Jump Width */
    CANFD_APBTR_SJW_75tp  = 0x4A,	/* 75 Synchronization Jump Width */
    CANFD_APBTR_SJW_76tp  = 0x4B,	/* 76 Synchronization Jump Width */
    CANFD_APBTR_SJW_77tp  = 0x4C,	/* 77 Synchronization Jump Width */
    CANFD_APBTR_SJW_78tp  = 0x4D,	/* 78 Synchronization Jump Width */
    CANFD_APBTR_SJW_79tp  = 0x4E,	/* 79 Synchronization Jump Width */
    CANFD_APBTR_SJW_80tp  = 0x4F,	/* 80 Synchronization Jump Width */

    CANFD_APBTR_SJW_81tp  = 0x50,	/* 81 Synchronization Jump Width */
    CANFD_APBTR_SJW_82tp  = 0x51,	/* 82 Synchronization Jump Width */
    CANFD_APBTR_SJW_83tp  = 0x52,	/* 83 Synchronization Jump Width */
    CANFD_APBTR_SJW_84tp  = 0x53,	/* 84 Synchronization Jump Width */
    CANFD_APBTR_SJW_85tp  = 0x54,	/* 85 Synchronization Jump Width */
    CANFD_APBTR_SJW_86tp  = 0x55,	/* 86 Synchronization Jump Width */
    CANFD_APBTR_SJW_87tp  = 0x56,	/* 87 Synchronization Jump Width */
    CANFD_APBTR_SJW_88tp  = 0x57,	/* 88 Synchronization Jump Width */
    CANFD_APBTR_SJW_89tp  = 0x58,	/* 89 Synchronization Jump Width */
    CANFD_APBTR_SJW_90tp  = 0x59,	/* 90 Synchronization Jump Width */
    CANFD_APBTR_SJW_91tp  = 0x5A,	/* 91 Synchronization Jump Width */
    CANFD_APBTR_SJW_92tp  = 0x5B,	/* 92 Synchronization Jump Width */
    CANFD_APBTR_SJW_93tp  = 0x5C,	/* 93 Synchronization Jump Width */
    CANFD_APBTR_SJW_94tp  = 0x5D,	/* 94 Synchronization Jump Width */
    CANFD_APBTR_SJW_95tp  = 0x5E,	/* 95 Synchronization Jump Width */
    CANFD_APBTR_SJW_96tp  = 0x5F,	/* 96 Synchronization Jump Width */

    CANFD_APBTR_SJW_97tp  = 0x60,	/* 97 Synchronization Jump Width */
    CANFD_APBTR_SJW_98tp  = 0x61,	/* 98 Synchronization Jump Width */
    CANFD_APBTR_SJW_99tp  = 0x62,	/* 99 Synchronization Jump Width */
    CANFD_APBTR_SJW_100tp = 0x63,	/* 100 Synchronization Jump Width */
    CANFD_APBTR_SJW_101tp = 0x64,	/* 101 Synchronization Jump Width */
    CANFD_APBTR_SJW_102tp = 0x65,	/* 102 Synchronization Jump Width */
    CANFD_APBTR_SJW_103tp = 0x66,	/* 103 Synchronization Jump Width */
    CANFD_APBTR_SJW_104tp = 0x67,	/* 104 Synchronization Jump Width */
    CANFD_APBTR_SJW_105tp = 0x68,	/* 105 Synchronization Jump Width */
    CANFD_APBTR_SJW_106tp = 0x69,	/* 106 Synchronization Jump Width */
    CANFD_APBTR_SJW_107tp = 0x6A,	/* 107 Synchronization Jump Width */
    CANFD_APBTR_SJW_108tp = 0x6B,	/* 108 Synchronization Jump Width */
    CANFD_APBTR_SJW_109tp = 0x6C,	/* 109 Synchronization Jump Width */
    CANFD_APBTR_SJW_110tp = 0x6D,	/* 110 Synchronization Jump Width */
    CANFD_APBTR_SJW_111tp = 0x6E,	/* 111 Synchronization Jump Width */
    CANFD_APBTR_SJW_112tp = 0x6F,	/* 112 Synchronization Jump Width */

    CANFD_APBTR_SJW_113tp = 0x70,	/* 113 Synchronization Jump Width */
    CANFD_APBTR_SJW_114tp = 0x71,	/* 114 Synchronization Jump Width */
    CANFD_APBTR_SJW_115tp = 0x72,	/* 115 Synchronization Jump Width */
    CANFD_APBTR_SJW_116tp = 0x73,	/* 116 Synchronization Jump Width */
    CANFD_APBTR_SJW_117tp = 0x74,	/* 117 Synchronization Jump Width */
    CANFD_APBTR_SJW_118tp = 0x75,	/* 118 Synchronization Jump Width */
    CANFD_APBTR_SJW_119tp = 0x76,	/* 119 Synchronization Jump Width */
    CANFD_APBTR_SJW_120tp = 0x77,	/* 120 Synchronization Jump Width */
    CANFD_APBTR_SJW_121tp = 0x78,	/* 121 Synchronization Jump Width */
    CANFD_APBTR_SJW_122tp = 0x79,	/* 122 Synchronization Jump Width */
    CANFD_APBTR_SJW_123tp = 0x7A,	/* 123 Synchronization Jump Width */
    CANFD_APBTR_SJW_124tp = 0x7B,	/* 124 Synchronization Jump Width */
    CANFD_APBTR_SJW_125tp = 0x7C,	/* 125 Synchronization Jump Width */
    CANFD_APBTR_SJW_126tp = 0x7D,	/* 126 Synchronization Jump Width */
    CANFD_APBTR_SJW_127tp = 0x7E,	/* 127 Synchronization Jump Width */
    CANFD_APBTR_SJW_128tp = 0x7F,	/* 128 Synchronization Jump Width */
}CANFDAPBTRsjw_TypeDef;
#define IS_CANFD_APBTR_SJW(APBTRSJWTP) (((APBTRSJWTP) == CANFD_APBTR_SJW_1tp)   || ((APBTRSJWTP) == CANFD_APBTR_SJW_2tp)   || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_3tp)   || ((APBTRSJWTP) == CANFD_APBTR_SJW_4tp)   || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_5tp)   || ((APBTRSJWTP) == CANFD_APBTR_SJW_6tp)   || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_7tp)   || ((APBTRSJWTP) == CANFD_APBTR_SJW_8tp)   || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_9tp)   || ((APBTRSJWTP) == CANFD_APBTR_SJW_10tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_11tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_12tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_13tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_14tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_15tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_16tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_17tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_18tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_19tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_20tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_21tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_22tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_23tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_24tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_25tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_26tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_27tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_28tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_29tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_30tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_31tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_32tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_33tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_34tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_35tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_36tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_37tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_38tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_39tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_40tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_41tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_42tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_43tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_44tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_45tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_46tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_47tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_48tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_49tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_50tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_51tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_52tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_53tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_54tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_55tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_56tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_57tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_58tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_59tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_60tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_61tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_62tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_63tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_64tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_65tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_66tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_67tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_68tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_69tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_70tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_71tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_72tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_73tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_74tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_75tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_76tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_77tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_78tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_79tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_80tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_81tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_82tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_83tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_84tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_85tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_86tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_87tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_88tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_89tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_90tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_91tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_92tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_93tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_94tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_95tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_96tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_97tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_98tp)  || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_99tp)  || ((APBTRSJWTP) == CANFD_APBTR_SJW_100tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_101tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_102tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_103tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_104tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_105tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_106tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_107tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_108tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_109tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_110tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_111tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_112tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_113tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_114tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_115tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_116tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_117tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_118tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_119tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_120tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_121tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_122tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_123tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_124tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_125tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_126tp) || \
                                    	((APBTRSJWTP) == CANFD_APBTR_SJW_127tp) || ((APBTRSJWTP) == CANFD_APBTR_SJW_128tp))

/*
 * @brief CANFD Data Phase Baud Rate Prescaler enumeration
 */
typedef enum
{
    CANFD_DPBRPR_1tp   = 0x00,  /* 1 Baud Rate Prescaler */
    CANFD_DPBRPR_2tp   = 0x01,  /* 2 Baud Rate Prescaler */
    CANFD_DPBRPR_3tp   = 0x02,  /* 3 Baud Rate Prescaler */
    CANFD_DPBRPR_4tp   = 0x03,  /* 4 Baud Rate Prescaler */
    CANFD_DPBRPR_5tp   = 0x04,  /* 5 Baud Rate Prescaler */
    CANFD_DPBRPR_6tp   = 0x05,  /* 6 Baud Rate Prescaler */
    CANFD_DPBRPR_7tp   = 0x06,  /* 7 Baud Rate Prescaler */
    CANFD_DPBRPR_8tp   = 0x07,  /* 8 Baud Rate Prescaler */
    CANFD_DPBRPR_9tp   = 0x08,  /* 9 Baud Rate Prescaler */
    CANFD_DPBRPR_10tp  = 0x09,  /* 10 Baud Rate Prescaler */
    CANFD_DPBRPR_11tp  = 0x0A,  /* 11 Baud Rate Prescaler */
    CANFD_DPBRPR_12tp  = 0x0B,  /* 12 Baud Rate Prescaler */
    CANFD_DPBRPR_13tp  = 0x0C,  /* 13 Baud Rate Prescaler */
    CANFD_DPBRPR_14tp  = 0x0D,  /* 14 Baud Rate Prescaler */
    CANFD_DPBRPR_15tp  = 0x0E,  /* 15 Baud Rate Prescaler */
    CANFD_DPBRPR_16tp  = 0x0F,  /* 16 Baud Rate Prescaler */

    CANFD_DPBRPR_17tp  = 0x10,  /* 17 Baud Rate Prescaler */
    CANFD_DPBRPR_18tp  = 0x11,  /* 18 Baud Rate Prescaler */
    CANFD_DPBRPR_19tp  = 0x12,  /* 19 Baud Rate Prescaler */
    CANFD_DPBRPR_20tp  = 0x13,  /* 20 Baud Rate Prescaler */
    CANFD_DPBRPR_21tp  = 0x14,  /* 21 Baud Rate Prescaler */
    CANFD_DPBRPR_22tp  = 0x15,  /* 22 Baud Rate Prescaler */
    CANFD_DPBRPR_23tp  = 0x16,  /* 23 Baud Rate Prescaler */
    CANFD_DPBRPR_24tp  = 0x17,  /* 24 Baud Rate Prescaler */
    CANFD_DPBRPR_25tp  = 0x18,  /* 25 Baud Rate Prescaler */
    CANFD_DPBRPR_26tp  = 0x19,  /* 26 Baud Rate Prescaler */
    CANFD_DPBRPR_27tp  = 0x1A,  /* 27 Baud Rate Prescaler */
    CANFD_DPBRPR_28tp  = 0x1B,  /* 28 Baud Rate Prescaler */
    CANFD_DPBRPR_29tp  = 0x1C,  /* 29 Baud Rate Prescaler */
    CANFD_DPBRPR_30tp  = 0x1D,  /* 30 Baud Rate Prescaler */
    CANFD_DPBRPR_31tp  = 0x1E,  /* 31 Baud Rate Prescaler */
    CANFD_DPBRPR_32tp  = 0x1F,  /* 32 Baud Rate Prescaler */

    CANFD_DPBRPR_33tp  = 0x20,  /* 33 Baud Rate Prescaler */
    CANFD_DPBRPR_34tp  = 0x21,  /* 34 Baud Rate Prescaler */
    CANFD_DPBRPR_35tp  = 0x22,  /* 35 Baud Rate Prescaler */
    CANFD_DPBRPR_36tp  = 0x23,  /* 36 Baud Rate Prescaler */
    CANFD_DPBRPR_37tp  = 0x24,  /* 37 Baud Rate Prescaler */
    CANFD_DPBRPR_38tp  = 0x25,  /* 38 Baud Rate Prescaler */
    CANFD_DPBRPR_39tp  = 0x26,  /* 39 Baud Rate Prescaler */
    CANFD_DPBRPR_40tp  = 0x27,  /* 40 Baud Rate Prescaler */
    CANFD_DPBRPR_41tp  = 0x28,  /* 41 Baud Rate Prescaler */
    CANFD_DPBRPR_42tp  = 0x29,  /* 42 Baud Rate Prescaler */
    CANFD_DPBRPR_43tp  = 0x2A,  /* 43 Baud Rate Prescaler */
    CANFD_DPBRPR_44tp  = 0x2B,  /* 44 Baud Rate Prescaler */
    CANFD_DPBRPR_45tp  = 0x2C,  /* 45 Baud Rate Prescaler */
    CANFD_DPBRPR_46tp  = 0x2D,  /* 46 Baud Rate Prescaler */
    CANFD_DPBRPR_47tp  = 0x2E,  /* 47 Baud Rate Prescaler */
    CANFD_DPBRPR_48tp  = 0x2F,  /* 48 Baud Rate Prescaler */

    CANFD_DPBRPR_49tp  = 0x30,  /* 49 Baud Rate Prescaler */
    CANFD_DPBRPR_50tp  = 0x31,  /* 50 Baud Rate Prescaler */
    CANFD_DPBRPR_51tp  = 0x32,  /* 51 Baud Rate Prescaler */
    CANFD_DPBRPR_52tp  = 0x33,  /* 52 Baud Rate Prescaler */
    CANFD_DPBRPR_53tp  = 0x34,  /* 53 Baud Rate Prescaler */
    CANFD_DPBRPR_54tp  = 0x35,  /* 54 Baud Rate Prescaler */
    CANFD_DPBRPR_55tp  = 0x36,  /* 55 Baud Rate Prescaler */
    CANFD_DPBRPR_56tp  = 0x37,  /* 56 Baud Rate Prescaler */
    CANFD_DPBRPR_57tp  = 0x38,  /* 57 Baud Rate Prescaler */
    CANFD_DPBRPR_58tp  = 0x39,  /* 58 Baud Rate Prescaler */
    CANFD_DPBRPR_59tp  = 0x3A,  /* 59 Baud Rate Prescaler */
    CANFD_DPBRPR_60tp  = 0x3B,  /* 60 Baud Rate Prescaler */
    CANFD_DPBRPR_61tp  = 0x3C,  /* 61 Baud Rate Prescaler */
    CANFD_DPBRPR_62tp  = 0x3D,  /* 62 Baud Rate Prescaler */
    CANFD_DPBRPR_63tp  = 0x3E,  /* 63 Baud Rate Prescaler */
    CANFD_DPBRPR_64tp  = 0x3F,  /* 64 Baud Rate Prescaler */

    CANFD_DPBRPR_65tp  = 0x40,  /* 65 Baud Rate Prescaler */
    CANFD_DPBRPR_66tp  = 0x41,  /* 66 Baud Rate Prescaler */
    CANFD_DPBRPR_67tp  = 0x42,  /* 67 Baud Rate Prescaler */
    CANFD_DPBRPR_68tp  = 0x43,  /* 68 Baud Rate Prescaler */
    CANFD_DPBRPR_69tp  = 0x44,  /* 69 Baud Rate Prescaler */
    CANFD_DPBRPR_70tp  = 0x45,  /* 70 Baud Rate Prescaler */
    CANFD_DPBRPR_71tp  = 0x46,  /* 71 Baud Rate Prescaler */
    CANFD_DPBRPR_72tp  = 0x47,  /* 72 Baud Rate Prescaler */
    CANFD_DPBRPR_73tp  = 0x48,  /* 73 Baud Rate Prescaler */
    CANFD_DPBRPR_74tp  = 0x49,  /* 74 Baud Rate Prescaler */
    CANFD_DPBRPR_75tp  = 0x4A,  /* 75 Baud Rate Prescaler */
    CANFD_DPBRPR_76tp  = 0x4B,  /* 76 Baud Rate Prescaler */
    CANFD_DPBRPR_77tp  = 0x4C,  /* 77 Baud Rate Prescaler */
    CANFD_DPBRPR_78tp  = 0x4D,  /* 78 Baud Rate Prescaler */
    CANFD_DPBRPR_79tp  = 0x4E,  /* 79 Baud Rate Prescaler */
    CANFD_DPBRPR_80tp  = 0x4F,  /* 80 Baud Rate Prescaler */

    CANFD_DPBRPR_81tp  = 0x50,  /* 81 Baud Rate Prescaler */
    CANFD_DPBRPR_82tp  = 0x51,  /* 82 Baud Rate Prescaler */
    CANFD_DPBRPR_83tp  = 0x52,  /* 83 Baud Rate Prescaler */
    CANFD_DPBRPR_84tp  = 0x53,  /* 84 Baud Rate Prescaler */
    CANFD_DPBRPR_85tp  = 0x54,  /* 85 Baud Rate Prescaler */
    CANFD_DPBRPR_86tp  = 0x55,  /* 86 Baud Rate Prescaler */
    CANFD_DPBRPR_87tp  = 0x56,  /* 87 Baud Rate Prescaler */
    CANFD_DPBRPR_88tp  = 0x57,  /* 88 Baud Rate Prescaler */
    CANFD_DPBRPR_89tp  = 0x58,  /* 89 Baud Rate Prescaler */
    CANFD_DPBRPR_90tp  = 0x59,  /* 90 Baud Rate Prescaler */
    CANFD_DPBRPR_91tp  = 0x5A,  /* 91 Baud Rate Prescaler */
    CANFD_DPBRPR_92tp  = 0x5B,  /* 92 Baud Rate Prescaler */
    CANFD_DPBRPR_93tp  = 0x5C,  /* 93 Baud Rate Prescaler */
    CANFD_DPBRPR_94tp  = 0x5D,  /* 94 Baud Rate Prescaler */
    CANFD_DPBRPR_95tp  = 0x5E,  /* 95 Baud Rate Prescaler */
    CANFD_DPBRPR_96tp  = 0x5F,  /* 96 Baud Rate Prescaler */

    CANFD_DPBRPR_97tp  = 0x60,  /* 97 Baud Rate Prescaler */
    CANFD_DPBRPR_98tp  = 0x61,  /* 98 Baud Rate Prescaler */
    CANFD_DPBRPR_99tp  = 0x62,  /* 99 Baud Rate Prescaler */
    CANFD_DPBRPR_100tp = 0x63,  /* 100 Baud Rate Prescaler */
    CANFD_DPBRPR_101tp = 0x64,  /* 101 Baud Rate Prescaler */
    CANFD_DPBRPR_102tp = 0x65,  /* 102 Baud Rate Prescaler */
    CANFD_DPBRPR_103tp = 0x66,  /* 103 Baud Rate Prescaler */
    CANFD_DPBRPR_104tp = 0x67,  /* 104 Baud Rate Prescaler */
    CANFD_DPBRPR_105tp = 0x68,  /* 105 Baud Rate Prescaler */
    CANFD_DPBRPR_106tp = 0x69,  /* 106 Baud Rate Prescaler */
    CANFD_DPBRPR_107tp = 0x6A,  /* 107 Baud Rate Prescaler */
    CANFD_DPBRPR_108tp = 0x6B,  /* 108 Baud Rate Prescaler */
    CANFD_DPBRPR_109tp = 0x6C,  /* 109 Baud Rate Prescaler */
    CANFD_DPBRPR_110tp = 0x6D,  /* 110 Baud Rate Prescaler */
    CANFD_DPBRPR_111tp = 0x6E,  /* 111 Baud Rate Prescaler */
    CANFD_DPBRPR_112tp = 0x6F,  /* 112 Baud Rate Prescaler */

    CANFD_DPBRPR_113tp = 0x70,  /* 113 Baud Rate Prescaler */
    CANFD_DPBRPR_114tp = 0x71,  /* 114 Baud Rate Prescaler */
    CANFD_DPBRPR_115tp = 0x72,  /* 115 Baud Rate Prescaler */
    CANFD_DPBRPR_116tp = 0x73,  /* 116 Baud Rate Prescaler */
    CANFD_DPBRPR_117tp = 0x74,  /* 117 Baud Rate Prescaler */
    CANFD_DPBRPR_118tp = 0x75,  /* 118 Baud Rate Prescaler */
    CANFD_DPBRPR_119tp = 0x76,  /* 119 Baud Rate Prescaler */
    CANFD_DPBRPR_120tp = 0x77,  /* 120 Baud Rate Prescaler */
    CANFD_DPBRPR_121tp = 0x78,  /* 121 Baud Rate Prescaler */
    CANFD_DPBRPR_122tp = 0x79,  /* 122 Baud Rate Prescaler */
    CANFD_DPBRPR_123tp = 0x7A,  /* 123 Baud Rate Prescaler */
    CANFD_DPBRPR_124tp = 0x7B,  /* 124 Baud Rate Prescaler */
    CANFD_DPBRPR_125tp = 0x7C,  /* 125 Baud Rate Prescaler */
    CANFD_DPBRPR_126tp = 0x7D,  /* 126 Baud Rate Prescaler */
    CANFD_DPBRPR_127tp = 0x7E,  /* 127 Baud Rate Prescaler */
    CANFD_DPBRPR_128tp = 0x7F,  /* 128 Baud Rate Prescaler */

    CANFD_DPBRPR_129tp = 0x80,  /* 129 Baud Rate Prescaler */
    CANFD_DPBRPR_130tp = 0x81,  /* 130 Baud Rate Prescaler */
    CANFD_DPBRPR_131tp = 0x82,  /* 131 Baud Rate Prescaler */
    CANFD_DPBRPR_132tp = 0x83,  /* 132 Baud Rate Prescaler */
    CANFD_DPBRPR_133tp = 0x84,  /* 133 Baud Rate Prescaler */
    CANFD_DPBRPR_134tp = 0x85,  /* 134 Baud Rate Prescaler */
    CANFD_DPBRPR_135tp = 0x86,  /* 135 Baud Rate Prescaler */
    CANFD_DPBRPR_136tp = 0x87,  /* 136 Baud Rate Prescaler */
    CANFD_DPBRPR_137tp = 0x88,  /* 137 Baud Rate Prescaler */
    CANFD_DPBRPR_138tp = 0x89,  /* 138 Baud Rate Prescaler */
    CANFD_DPBRPR_139tp = 0x8A,  /* 139 Baud Rate Prescaler */
    CANFD_DPBRPR_140tp = 0x8B,  /* 140 Baud Rate Prescaler */
    CANFD_DPBRPR_141tp = 0x8C,  /* 141 Baud Rate Prescaler */
    CANFD_DPBRPR_142tp = 0x8D,  /* 142 Baud Rate Prescaler */
    CANFD_DPBRPR_143tp = 0x8E,  /* 143 Baud Rate Prescaler */
    CANFD_DPBRPR_144tp = 0x8F,  /* 144 Baud Rate Prescaler */

    CANFD_DPBRPR_145tp = 0x90,  /* 145 Baud Rate Prescaler */
    CANFD_DPBRPR_146tp = 0x91,  /* 146 Baud Rate Prescaler */
    CANFD_DPBRPR_147tp = 0x92,  /* 147 Baud Rate Prescaler */
    CANFD_DPBRPR_148tp = 0x93,  /* 148 Baud Rate Prescaler */
    CANFD_DPBRPR_149tp = 0x94,  /* 149 Baud Rate Prescaler */
    CANFD_DPBRPR_150tp = 0x95,  /* 150 Baud Rate Prescaler */
    CANFD_DPBRPR_151tp = 0x96,  /* 151 Baud Rate Prescaler */
    CANFD_DPBRPR_152tp = 0x97,  /* 152 Baud Rate Prescaler */
    CANFD_DPBRPR_153tp = 0x98,  /* 153 Baud Rate Prescaler */
    CANFD_DPBRPR_154tp = 0x99,  /* 154 Baud Rate Prescaler */
    CANFD_DPBRPR_155tp = 0x9A,  /* 155 Baud Rate Prescaler */
    CANFD_DPBRPR_156tp = 0x9B,  /* 156 Baud Rate Prescaler */
    CANFD_DPBRPR_157tp = 0x9C,  /* 157 Baud Rate Prescaler */
    CANFD_DPBRPR_158tp = 0x9D,  /* 158 Baud Rate Prescaler */
    CANFD_DPBRPR_159tp = 0x9E,  /* 159 Baud Rate Prescaler */
    CANFD_DPBRPR_160tp = 0x9F,  /* 160 Baud Rate Prescaler */

    CANFD_DPBRPR_161tp = 0xA0,  /* 161 Baud Rate Prescaler */
    CANFD_DPBRPR_162tp = 0xA1,  /* 162 Baud Rate Prescaler */
    CANFD_DPBRPR_163tp = 0xA2,  /* 163 Baud Rate Prescaler */
    CANFD_DPBRPR_164tp = 0xA3,  /* 164 Baud Rate Prescaler */
    CANFD_DPBRPR_165tp = 0xA4,  /* 165 Baud Rate Prescaler */
    CANFD_DPBRPR_166tp = 0xA5,  /* 166 Baud Rate Prescaler */
    CANFD_DPBRPR_167tp = 0xA6,  /* 167 Baud Rate Prescaler */
    CANFD_DPBRPR_168tp = 0xA7,  /* 168 Baud Rate Prescaler */
    CANFD_DPBRPR_169tp = 0xA8,  /* 169 Baud Rate Prescaler */
    CANFD_DPBRPR_170tp = 0xA9,  /* 170 Baud Rate Prescaler */
    CANFD_DPBRPR_171tp = 0xAA,  /* 171 Baud Rate Prescaler */
    CANFD_DPBRPR_172tp = 0xAB,  /* 172 Baud Rate Prescaler */
    CANFD_DPBRPR_173tp = 0xAC,  /* 173 Baud Rate Prescaler */
    CANFD_DPBRPR_174tp = 0xAD,  /* 174 Baud Rate Prescaler */
    CANFD_DPBRPR_175tp = 0xAE,  /* 175 Baud Rate Prescaler */
    CANFD_DPBRPR_176tp = 0xAF,  /* 176 Baud Rate Prescaler */

    CANFD_DPBRPR_177tp = 0xB0,  /* 177 Baud Rate Prescaler */
    CANFD_DPBRPR_178tp = 0xB1,  /* 178 Baud Rate Prescaler */
    CANFD_DPBRPR_179tp = 0xB2,  /* 179 Baud Rate Prescaler */
    CANFD_DPBRPR_180tp = 0xB3,  /* 180 Baud Rate Prescaler */
    CANFD_DPBRPR_181tp = 0xB4,  /* 181 Baud Rate Prescaler */
    CANFD_DPBRPR_182tp = 0xB5,  /* 182 Baud Rate Prescaler */
    CANFD_DPBRPR_183tp = 0xB6,  /* 183 Baud Rate Prescaler */
    CANFD_DPBRPR_184tp = 0xB7,  /* 184 Baud Rate Prescaler */
    CANFD_DPBRPR_185tp = 0xB8,  /* 185 Baud Rate Prescaler */
    CANFD_DPBRPR_186tp = 0xB9,  /* 186 Baud Rate Prescaler */
    CANFD_DPBRPR_187tp = 0xBA,  /* 187 Baud Rate Prescaler */
    CANFD_DPBRPR_188tp = 0xBB,  /* 188 Baud Rate Prescaler */
    CANFD_DPBRPR_189tp = 0xBC,  /* 189 Baud Rate Prescaler */
    CANFD_DPBRPR_190tp = 0xBD,  /* 190 Baud Rate Prescaler */
    CANFD_DPBRPR_191tp = 0xBE,  /* 191 Baud Rate Prescaler */
    CANFD_DPBRPR_192tp = 0xBF,  /* 192 Baud Rate Prescaler */

    CANFD_DPBRPR_193tp = 0xC0,  /* 193 Baud Rate Prescaler */
    CANFD_DPBRPR_194tp = 0xC1,  /* 194 Baud Rate Prescaler */
    CANFD_DPBRPR_195tp = 0xC2,  /* 195 Baud Rate Prescaler */
    CANFD_DPBRPR_196tp = 0xC3,  /* 196 Baud Rate Prescaler */
    CANFD_DPBRPR_197tp = 0xC4,  /* 197 Baud Rate Prescaler */
    CANFD_DPBRPR_198tp = 0xC5,  /* 198 Baud Rate Prescaler */
    CANFD_DPBRPR_199tp = 0xC6,  /* 199 Baud Rate Prescaler */
    CANFD_DPBRPR_200tp = 0xC7,  /* 200 Baud Rate Prescaler */
    CANFD_DPBRPR_201tp = 0xC8,  /* 201 Baud Rate Prescaler */
    CANFD_DPBRPR_202tp = 0xC9,  /* 202 Baud Rate Prescaler */
    CANFD_DPBRPR_203tp = 0xCA,  /* 203 Baud Rate Prescaler */
    CANFD_DPBRPR_204tp = 0xCB,  /* 204 Baud Rate Prescaler */
    CANFD_DPBRPR_205tp = 0xCC,  /* 205 Baud Rate Prescaler */
    CANFD_DPBRPR_206tp = 0xCD,  /* 206 Baud Rate Prescaler */
    CANFD_DPBRPR_207tp = 0xCE,  /* 207 Baud Rate Prescaler */
    CANFD_DPBRPR_208tp = 0xCF,  /* 208 Baud Rate Prescaler */

    CANFD_DPBRPR_209tp = 0xD0,  /* 209 Baud Rate Prescaler */
    CANFD_DPBRPR_210tp = 0xD1,  /* 210 Baud Rate Prescaler */
    CANFD_DPBRPR_211tp = 0xD2,  /* 211 Baud Rate Prescaler */
    CANFD_DPBRPR_212tp = 0xD3,  /* 212 Baud Rate Prescaler */
    CANFD_DPBRPR_213tp = 0xD4,  /* 213 Baud Rate Prescaler */
    CANFD_DPBRPR_214tp = 0xD5,  /* 214 Baud Rate Prescaler */
    CANFD_DPBRPR_215tp = 0xD6,  /* 215 Baud Rate Prescaler */
    CANFD_DPBRPR_216tp = 0xD7,  /* 216 Baud Rate Prescaler */
    CANFD_DPBRPR_217tp = 0xD8,  /* 217 Baud Rate Prescaler */
    CANFD_DPBRPR_218tp = 0xD9,  /* 218 Baud Rate Prescaler */
    CANFD_DPBRPR_219tp = 0xDA,  /* 219 Baud Rate Prescaler */
    CANFD_DPBRPR_220tp = 0xDB,  /* 220 Baud Rate Prescaler */
    CANFD_DPBRPR_221tp = 0xDC,  /* 221 Baud Rate Prescaler */
    CANFD_DPBRPR_222tp = 0xDD,  /* 222 Baud Rate Prescaler */
    CANFD_DPBRPR_223tp = 0xDE,  /* 223 Baud Rate Prescaler */
    CANFD_DPBRPR_224tp = 0xDF,  /* 224 Baud Rate Prescaler */

    CANFD_DPBRPR_225tp = 0xE0,  /* 225 Baud Rate Prescaler */
    CANFD_DPBRPR_226tp = 0xE1,  /* 226 Baud Rate Prescaler */
    CANFD_DPBRPR_227tp = 0xE2,  /* 227 Baud Rate Prescaler */
    CANFD_DPBRPR_228tp = 0xE3,  /* 228 Baud Rate Prescaler */
    CANFD_DPBRPR_229tp = 0xE4,  /* 229 Baud Rate Prescaler */
    CANFD_DPBRPR_230tp = 0xE5,  /* 230 Baud Rate Prescaler */
    CANFD_DPBRPR_231tp = 0xE6,  /* 231 Baud Rate Prescaler */
    CANFD_DPBRPR_232tp = 0xE7,  /* 232 Baud Rate Prescaler */
    CANFD_DPBRPR_233tp = 0xE8,  /* 233 Baud Rate Prescaler */
    CANFD_DPBRPR_234tp = 0xE9,  /* 234 Baud Rate Prescaler */
    CANFD_DPBRPR_235tp = 0xEA,  /* 235 Baud Rate Prescaler */
    CANFD_DPBRPR_236tp = 0xEB,  /* 236 Baud Rate Prescaler */
    CANFD_DPBRPR_237tp = 0xEC,  /* 237 Baud Rate Prescaler */
    CANFD_DPBRPR_238tp = 0xED,  /* 238 Baud Rate Prescaler */
    CANFD_DPBRPR_239tp = 0xEE,  /* 239 Baud Rate Prescaler */
    CANFD_DPBRPR_240tp = 0xEF,  /* 240 Baud Rate Prescaler */

    CANFD_DPBRPR_241tp = 0xF0,  /* 241 Baud Rate Prescaler */
    CANFD_DPBRPR_242tp = 0xF1,  /* 242 Baud Rate Prescaler */
    CANFD_DPBRPR_243tp = 0xF2,  /* 243 Baud Rate Prescaler */
    CANFD_DPBRPR_244tp = 0xF3,  /* 244 Baud Rate Prescaler */
    CANFD_DPBRPR_245tp = 0xF4,  /* 245 Baud Rate Prescaler */
    CANFD_DPBRPR_246tp = 0xF5,  /* 246 Baud Rate Prescaler */
    CANFD_DPBRPR_247tp = 0xF6,  /* 247 Baud Rate Prescaler */
    CANFD_DPBRPR_248tp = 0xF7,  /* 248 Baud Rate Prescaler */
    CANFD_DPBRPR_249tp = 0xF8,  /* 249 Baud Rate Prescaler */
    CANFD_DPBRPR_250tp = 0xF9,  /* 250 Baud Rate Prescaler */
    CANFD_DPBRPR_251tp = 0xFA,  /* 251 Baud Rate Prescaler */
    CANFD_DPBRPR_252tp = 0xFB,  /* 252 Baud Rate Prescaler */
    CANFD_DPBRPR_253tp = 0xFC,  /* 253 Baud Rate Prescaler */
    CANFD_DPBRPR_254tp = 0xFD,  /* 254 Baud Rate Prescaler */
    CANFD_DPBRPR_255tp = 0xFE,  /* 255 Baud Rate Prescaler */
    CANFD_DPBRPR_256tp = 0xFF,  /* 256 Baud Rate Prescaler */
}CANFDDPBRPR_TypeDef;
#define IS_CANFD_DPBRPR(DPBRPRTP)  (((DPBRPRTP) == CANFD_DPBRPR_1tp)   || ((DPBRPRTP) == CANFD_DPBRPR_2tp)   || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_3tp)   || ((DPBRPRTP) == CANFD_DPBRPR_4tp)   || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_5tp)   || ((DPBRPRTP) == CANFD_DPBRPR_6tp)   || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_7tp)   || ((DPBRPRTP) == CANFD_DPBRPR_8tp)   || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_9tp)   || ((DPBRPRTP) == CANFD_DPBRPR_10tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_11tp)  || ((DPBRPRTP) == CANFD_DPBRPR_12tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_13tp)  || ((DPBRPRTP) == CANFD_DPBRPR_14tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_15tp)  || ((DPBRPRTP) == CANFD_DPBRPR_16tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_17tp)  || ((DPBRPRTP) == CANFD_DPBRPR_18tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_19tp)  || ((DPBRPRTP) == CANFD_DPBRPR_20tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_21tp)  || ((DPBRPRTP) == CANFD_DPBRPR_22tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_23tp)  || ((DPBRPRTP) == CANFD_DPBRPR_24tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_25tp)  || ((DPBRPRTP) == CANFD_DPBRPR_26tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_27tp)  || ((DPBRPRTP) == CANFD_DPBRPR_28tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_29tp)  || ((DPBRPRTP) == CANFD_DPBRPR_30tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_31tp)  || ((DPBRPRTP) == CANFD_DPBRPR_32tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_33tp)  || ((DPBRPRTP) == CANFD_DPBRPR_34tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_35tp)  || ((DPBRPRTP) == CANFD_DPBRPR_36tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_37tp)  || ((DPBRPRTP) == CANFD_DPBRPR_38tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_39tp)  || ((DPBRPRTP) == CANFD_DPBRPR_40tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_41tp)  || ((DPBRPRTP) == CANFD_DPBRPR_42tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_43tp)  || ((DPBRPRTP) == CANFD_DPBRPR_44tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_45tp)  || ((DPBRPRTP) == CANFD_DPBRPR_46tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_47tp)  || ((DPBRPRTP) == CANFD_DPBRPR_48tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_49tp)  || ((DPBRPRTP) == CANFD_DPBRPR_50tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_51tp)  || ((DPBRPRTP) == CANFD_DPBRPR_52tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_53tp)  || ((DPBRPRTP) == CANFD_DPBRPR_54tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_55tp)  || ((DPBRPRTP) == CANFD_DPBRPR_56tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_57tp)  || ((DPBRPRTP) == CANFD_DPBRPR_58tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_59tp)  || ((DPBRPRTP) == CANFD_DPBRPR_60tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_61tp)  || ((DPBRPRTP) == CANFD_DPBRPR_62tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_63tp)  || ((DPBRPRTP) == CANFD_DPBRPR_64tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_65tp)  || ((DPBRPRTP) == CANFD_DPBRPR_66tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_67tp)  || ((DPBRPRTP) == CANFD_DPBRPR_68tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_69tp)  || ((DPBRPRTP) == CANFD_DPBRPR_70tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_71tp)  || ((DPBRPRTP) == CANFD_DPBRPR_72tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_73tp)  || ((DPBRPRTP) == CANFD_DPBRPR_74tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_75tp)  || ((DPBRPRTP) == CANFD_DPBRPR_76tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_77tp)  || ((DPBRPRTP) == CANFD_DPBRPR_78tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_79tp)  || ((DPBRPRTP) == CANFD_DPBRPR_80tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_81tp)  || ((DPBRPRTP) == CANFD_DPBRPR_82tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_83tp)  || ((DPBRPRTP) == CANFD_DPBRPR_84tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_85tp)  || ((DPBRPRTP) == CANFD_DPBRPR_86tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_87tp)  || ((DPBRPRTP) == CANFD_DPBRPR_88tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_89tp)  || ((DPBRPRTP) == CANFD_DPBRPR_90tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_91tp)  || ((DPBRPRTP) == CANFD_DPBRPR_92tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_93tp)  || ((DPBRPRTP) == CANFD_DPBRPR_94tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_95tp)  || ((DPBRPRTP) == CANFD_DPBRPR_96tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_97tp)  || ((DPBRPRTP) == CANFD_DPBRPR_98tp)  || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_99tp)  || ((DPBRPRTP) == CANFD_DPBRPR_100tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_101tp) || ((DPBRPRTP) == CANFD_DPBRPR_102tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_103tp) || ((DPBRPRTP) == CANFD_DPBRPR_104tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_105tp) || ((DPBRPRTP) == CANFD_DPBRPR_106tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_107tp) || ((DPBRPRTP) == CANFD_DPBRPR_108tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_109tp) || ((DPBRPRTP) == CANFD_DPBRPR_110tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_111tp) || ((DPBRPRTP) == CANFD_DPBRPR_112tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_113tp) || ((DPBRPRTP) == CANFD_DPBRPR_114tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_115tp) || ((DPBRPRTP) == CANFD_DPBRPR_116tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_117tp) || ((DPBRPRTP) == CANFD_DPBRPR_118tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_119tp) || ((DPBRPRTP) == CANFD_DPBRPR_120tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_121tp) || ((DPBRPRTP) == CANFD_DPBRPR_122tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_123tp) || ((DPBRPRTP) == CANFD_DPBRPR_124tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_125tp) || ((DPBRPRTP) == CANFD_DPBRPR_126tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_127tp) || ((DPBRPRTP) == CANFD_DPBRPR_128tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_129tp) || ((DPBRPRTP) == CANFD_DPBRPR_130tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_131tp) || ((DPBRPRTP) == CANFD_DPBRPR_132tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_133tp) || ((DPBRPRTP) == CANFD_DPBRPR_134tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_135tp) || ((DPBRPRTP) == CANFD_DPBRPR_136tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_137tp) || ((DPBRPRTP) == CANFD_DPBRPR_138tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_139tp) || ((DPBRPRTP) == CANFD_DPBRPR_140tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_141tp) || ((DPBRPRTP) == CANFD_DPBRPR_142tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_143tp) || ((DPBRPRTP) == CANFD_DPBRPR_144tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_145tp) || ((DPBRPRTP) == CANFD_DPBRPR_146tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_147tp) || ((DPBRPRTP) == CANFD_DPBRPR_148tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_149tp) || ((DPBRPRTP) == CANFD_DPBRPR_150tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_151tp) || ((DPBRPRTP) == CANFD_DPBRPR_152tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_153tp) || ((DPBRPRTP) == CANFD_DPBRPR_154tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_155tp) || ((DPBRPRTP) == CANFD_DPBRPR_156tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_157tp) || ((DPBRPRTP) == CANFD_DPBRPR_158tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_159tp) || ((DPBRPRTP) == CANFD_DPBRPR_160tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_161tp) || ((DPBRPRTP) == CANFD_DPBRPR_162tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_163tp) || ((DPBRPRTP) == CANFD_DPBRPR_164tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_165tp) || ((DPBRPRTP) == CANFD_DPBRPR_166tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_167tp) || ((DPBRPRTP) == CANFD_DPBRPR_168tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_169tp) || ((DPBRPRTP) == CANFD_DPBRPR_170tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_171tp) || ((DPBRPRTP) == CANFD_DPBRPR_172tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_173tp) || ((DPBRPRTP) == CANFD_DPBRPR_174tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_175tp) || ((DPBRPRTP) == CANFD_DPBRPR_176tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_177tp) || ((DPBRPRTP) == CANFD_DPBRPR_178tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_179tp) || ((DPBRPRTP) == CANFD_DPBRPR_180tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_181tp) || ((DPBRPRTP) == CANFD_DPBRPR_182tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_183tp) || ((DPBRPRTP) == CANFD_DPBRPR_184tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_185tp) || ((DPBRPRTP) == CANFD_DPBRPR_186tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_187tp) || ((DPBRPRTP) == CANFD_DPBRPR_188tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_189tp) || ((DPBRPRTP) == CANFD_DPBRPR_190tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_191tp) || ((DPBRPRTP) == CANFD_DPBRPR_192tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_193tp) || ((DPBRPRTP) == CANFD_DPBRPR_194tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_195tp) || ((DPBRPRTP) == CANFD_DPBRPR_196tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_197tp) || ((DPBRPRTP) == CANFD_DPBRPR_198tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_199tp) || ((DPBRPRTP) == CANFD_DPBRPR_200tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_201tp) || ((DPBRPRTP) == CANFD_DPBRPR_202tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_203tp) || ((DPBRPRTP) == CANFD_DPBRPR_204tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_205tp) || ((DPBRPRTP) == CANFD_DPBRPR_206tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_207tp) || ((DPBRPRTP) == CANFD_DPBRPR_208tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_209tp) || ((DPBRPRTP) == CANFD_DPBRPR_210tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_211tp) || ((DPBRPRTP) == CANFD_DPBRPR_212tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_213tp) || ((DPBRPRTP) == CANFD_DPBRPR_214tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_215tp) || ((DPBRPRTP) == CANFD_DPBRPR_216tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_217tp) || ((DPBRPRTP) == CANFD_DPBRPR_218tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_219tp) || ((DPBRPRTP) == CANFD_DPBRPR_220tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_221tp) || ((DPBRPRTP) == CANFD_DPBRPR_222tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_223tp) || ((DPBRPRTP) == CANFD_DPBRPR_224tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_225tp) || ((DPBRPRTP) == CANFD_DPBRPR_226tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_227tp) || ((DPBRPRTP) == CANFD_DPBRPR_228tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_229tp) || ((DPBRPRTP) == CANFD_DPBRPR_230tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_231tp) || ((DPBRPRTP) == CANFD_DPBRPR_232tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_233tp) || ((DPBRPRTP) == CANFD_DPBRPR_234tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_235tp) || ((DPBRPRTP) == CANFD_DPBRPR_236tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_237tp) || ((DPBRPRTP) == CANFD_DPBRPR_238tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_239tp) || ((DPBRPRTP) == CANFD_DPBRPR_240tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_241tp) || ((DPBRPRTP) == CANFD_DPBRPR_242tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_243tp) || ((DPBRPRTP) == CANFD_DPBRPR_244tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_245tp) || ((DPBRPRTP) == CANFD_DPBRPR_246tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_247tp) || ((DPBRPRTP) == CANFD_DPBRPR_248tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_249tp) || ((DPBRPRTP) == CANFD_DPBRPR_250tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_251tp) || ((DPBRPRTP) == CANFD_DPBRPR_252tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_253tp) || ((DPBRPRTP) == CANFD_DPBRPR_254tp) || \
                                    ((DPBRPRTP) == CANFD_DPBRPR_255tp) || ((DPBRPRTP) == CANFD_DPBRPR_256tp))

/*
 * @brief CANFD Data Phase Bit Timing (Data Phase Time Segment 1) enumeration
 */
typedef enum
{
    CANFD_DPBTR_TS1_1tp  = 0x00,    /* 1 Time Segment 1 */
    CANFD_DPBTR_TS1_2tp  = 0x01,    /* 2 Time Segment 1 */
    CANFD_DPBTR_TS1_3tp  = 0x02,    /* 3 Time Segment 1 */
    CANFD_DPBTR_TS1_4tp  = 0x03,    /* 4 Time Segment 1 */
    CANFD_DPBTR_TS1_5tp  = 0x04,    /* 5 Time Segment 1 */
    CANFD_DPBTR_TS1_6tp  = 0x05,    /* 6 Time Segment 1 */
    CANFD_DPBTR_TS1_7tp  = 0x06,    /* 7 Time Segment 1 */
    CANFD_DPBTR_TS1_8tp  = 0x07,    /* 8 Time Segment 1 */
    CANFD_DPBTR_TS1_9tp  = 0x08,    /* 9 Time Segment 1 */
    CANFD_DPBTR_TS1_10tp = 0x09,    /* 10 Time Segment 1 */
    CANFD_DPBTR_TS1_11tp = 0x0A,    /* 11 Time Segment 1 */
    CANFD_DPBTR_TS1_12tp = 0x0B,    /* 12 Time Segment 1 */
    CANFD_DPBTR_TS1_13tp = 0x0C,    /* 13 Time Segment 1 */
    CANFD_DPBTR_TS1_14tp = 0x0D,    /* 14 Time Segment 1 */
    CANFD_DPBTR_TS1_15tp = 0x0E,    /* 15 Time Segment 1 */
    CANFD_DPBTR_TS1_16tp = 0x0F,    /* 16 Time Segment 1 */
    CANFD_DPBTR_TS1_17tp = 0x10,    /* 17 Time Segment 1 */
    CANFD_DPBTR_TS1_18tp = 0x11,    /* 18 Time Segment 1 */
    CANFD_DPBTR_TS1_19tp = 0x12,    /* 19 Time Segment 1 */
    CANFD_DPBTR_TS1_20tp = 0x13,    /* 20 Time Segment 1 */
    CANFD_DPBTR_TS1_21tp = 0x14,    /* 21 Time Segment 1 */
    CANFD_DPBTR_TS1_22tp = 0x15,    /* 22 Time Segment 1 */
    CANFD_DPBTR_TS1_23tp = 0x16,    /* 23 Time Segment 1 */
    CANFD_DPBTR_TS1_24tp = 0x17,    /* 24 Time Segment 1 */
    CANFD_DPBTR_TS1_25tp = 0x18,    /* 25 Time Segment 1 */
    CANFD_DPBTR_TS1_26tp = 0x19,    /* 26 Time Segment 1 */
    CANFD_DPBTR_TS1_27tp = 0x1A,    /* 27 Time Segment 1 */
    CANFD_DPBTR_TS1_28tp = 0x1B,    /* 28 Time Segment 1 */
    CANFD_DPBTR_TS1_29tp = 0x1C,    /* 29 Time Segment 1 */
    CANFD_DPBTR_TS1_30tp = 0x1D,    /* 30 Time Segment 1 */
    CANFD_DPBTR_TS1_31tp = 0x1E,    /* 31 Time Segment 1 */
    CANFD_DPBTR_TS1_32tp = 0x1F,    /* 32 Time Segment 1 */
}CANFDDPBTRts1_TypeDef;
#define IS_CANFD_DPBTR_TS1(DPBTRTS1TP) (((DPBTRTS1TP) == CANFD_DPBTR_TS1_1tp)   || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_2tp)   || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_3tp)   || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_4tp)   || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_5tp)   || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_6tp)   || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_7tp)   || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_8tp)   || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_9tp)   || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_10tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_11tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_12tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_13tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_14tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_15tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_16tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_17tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_18tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_19tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_20tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_21tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_22tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_23tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_24tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_25tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_26tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_27tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_28tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_29tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_30tp)  || \
                                    	((DPBTRTS1TP) == CANFD_DPBTR_TS1_31tp)  || ((DPBTRTS1TP) == CANFD_DPBTR_TS1_32tp))

/*
 * @brief CANFD Data Phase Bit Timing (Data Phase Time Segment 2) enumeration
 */
typedef enum
{
    CANFD_DPBTR_TS2_1tp  = 0x00,    /* 1 Time Segment 2 */
    CANFD_DPBTR_TS2_2tp  = 0x01,    /* 2 Time Segment 2 */
    CANFD_DPBTR_TS2_3tp  = 0x02,    /* 3 Time Segment 2 */
    CANFD_DPBTR_TS2_4tp  = 0x03,    /* 4 Time Segment 2 */
    CANFD_DPBTR_TS2_5tp  = 0x04,    /* 5 Time Segment 2 */
    CANFD_DPBTR_TS2_6tp  = 0x05,    /* 6 Time Segment 2 */
    CANFD_DPBTR_TS2_7tp  = 0x06,    /* 7 Time Segment 2 */
    CANFD_DPBTR_TS2_8tp  = 0x07,    /* 8 Time Segment 2 */
    CANFD_DPBTR_TS2_9tp  = 0x08,    /* 9 Time Segment 2 */
    CANFD_DPBTR_TS2_10tp = 0x09,    /* 10 Time Segment 2 */
    CANFD_DPBTR_TS2_11tp = 0x0A,    /* 11 Time Segment 2 */
    CANFD_DPBTR_TS2_12tp = 0x0B,    /* 12 Time Segment 2 */
    CANFD_DPBTR_TS2_13tp = 0x0C,    /* 13 Time Segment 2 */
    CANFD_DPBTR_TS2_14tp = 0x0D,    /* 14 Time Segment 2 */
    CANFD_DPBTR_TS2_15tp = 0x0E,    /* 15 Time Segment 2 */
    CANFD_DPBTR_TS2_16tp = 0x0F,    /* 16 Time Segment 2 */
}CANFDDPBTRts2_TypeDef;
#define IS_CANFD_DPBTR_TS2(DPBTRTS2TP) (((DPBTRTS2TP) == CANFD_DPBTR_TS2_1tp)   || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_2tp)   || \
                                    	((DPBTRTS2TP) == CANFD_DPBTR_TS2_3tp)   || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_4tp)   || \
                                    	((DPBTRTS2TP) == CANFD_DPBTR_TS2_5tp)   || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_6tp)   || \
                                    	((DPBTRTS2TP) == CANFD_DPBTR_TS2_7tp)   || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_8tp)   || \
                                    	((DPBTRTS2TP) == CANFD_DPBTR_TS2_9tp)   || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_10tp)  || \
                                    	((DPBTRTS2TP) == CANFD_DPBTR_TS2_11tp)  || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_12tp)  || \
                                    	((DPBTRTS2TP) == CANFD_DPBTR_TS2_13tp)  || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_14tp)  || \
                                    	((DPBTRTS2TP) == CANFD_DPBTR_TS2_15tp)  || ((DPBTRTS2TP) == CANFD_DPBTR_TS2_16tp))

/*
 * @brief CANFD Data Phase Bit Timing (Data Phase Synchronization Jump Width) enumeration
 */
typedef enum
{
    CANFD_DPBTR_SJW_1tp  = 0x00,    /* 1 Synchronization Jump Width */
    CANFD_DPBTR_SJW_2tp  = 0x01,    /* 2 Synchronization Jump Width */
    CANFD_DPBTR_SJW_3tp  = 0x02,    /* 3 Synchronization Jump Width */
    CANFD_DPBTR_SJW_4tp  = 0x03,    /* 4 Synchronization Jump Width */
    CANFD_DPBTR_SJW_5tp  = 0x04,    /* 5 Synchronization Jump Width */
    CANFD_DPBTR_SJW_6tp  = 0x05,    /* 6 Synchronization Jump Width */
    CANFD_DPBTR_SJW_7tp  = 0x06,    /* 7 Synchronization Jump Width */
    CANFD_DPBTR_SJW_8tp  = 0x07,    /* 8 Synchronization Jump Width */
    CANFD_DPBTR_SJW_9tp  = 0x08,    /* 9 Synchronization Jump Width */
    CANFD_DPBTR_SJW_10tp = 0x09,    /* 10 Synchronization Jump Width */
    CANFD_DPBTR_SJW_11tp = 0x0A,    /* 11 Synchronization Jump Width */
    CANFD_DPBTR_SJW_12tp = 0x0B,    /* 12 Synchronization Jump Width */
    CANFD_DPBTR_SJW_13tp = 0x0C,    /* 13 Synchronization Jump Width */
    CANFD_DPBTR_SJW_14tp = 0x0D,    /* 14 Synchronization Jump Width */
    CANFD_DPBTR_SJW_15tp = 0x0E,    /* 15 Synchronization Jump Width */
    CANFD_DPBTR_SJW_16tp = 0x0F,    /* 16 Synchronization Jump Width */
}CANFDDPBTRsjw_TypeDef;
#define IS_CANFD_DPBTR_SJW(DPBTRSJWTP) (((DPBTRSJWTP) == CANFD_DPBTR_SJW_1tp)   || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_2tp)   || \
                                    	((DPBTRSJWTP) == CANFD_DPBTR_SJW_3tp)   || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_4tp)   || \
                                    	((DPBTRSJWTP) == CANFD_DPBTR_SJW_5tp)   || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_6tp)   || \
                                    	((DPBTRSJWTP) == CANFD_DPBTR_SJW_7tp)   || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_8tp)   || \
                                    	((DPBTRSJWTP) == CANFD_DPBTR_SJW_9tp)   || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_10tp)  || \
                                    	((DPBTRSJWTP) == CANFD_DPBTR_SJW_11tp)  || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_12tp)  || \
                                    	((DPBTRSJWTP) == CANFD_DPBTR_SJW_13tp)  || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_14tp)  || \
                                    	((DPBTRSJWTP) == CANFD_DPBTR_SJW_15tp)  || ((DPBTRSJWTP) == CANFD_DPBTR_SJW_16tp))

/*
 * @brief CANFD init structure definition
 */
typedef struct
{
    CANFDSRR_TypeDef CANFD_SRR;	                /* CANFD Software Reset Register */
    CANFDAPBRPR_TypeDef CANFD_APBRPR;		/* CANFD Arbitration Phase (Nominal) Baud Rate Prescaler */
    CANFDAPBTRts1_TypeDef CANFD_APBTR_APTS1;    /* CANFD Arbitration Phase Time Segment 1 */
    CANFDAPBTRts2_TypeDef CANFD_APBTR_APTS2;    /* CANFD Arbitration Phase Time Segment 2 */
    CANFDAPBTRsjw_TypeDef CANFD_APBTR_APSJW;    /* CANFD Arbitration Phase Synchronization Jump Width */
    CANFDDPBRPR_TypeDef CANFD_DPBRPR;           /* CANFD Data Phase Baud Rate Prescaler */
    CANFDDPBTRts1_TypeDef CANFD_DPBTR_DPTS1;    /* CANFD Data Phase Time Segment 1 */
    CANFDDPBTRts2_TypeDef CANFD_DPBTR_DPTS2;    /* CANFD Data Phase Time Segment 2 */
    CANFDDPBTRsjw_TypeDef CANFD_DPBTR_DPSJW;    /* CANFD Data Phase Synchronization Jump Width */
}CANFD_InitTypeDef;

/*
 * @brief CANFD TX Frame Format enumeration
 */
typedef enum
{
    STANDARD 	    = 0x00, /* CAN  Standard Frame  */
    EXTENDED 	    = 0x01, /* CANFD  Extanded Frame  */
}CANFDTXFORMAT_TypeDef;
#define IS_CANFD_TXFORMAT(TXFORMAT) (((TXFORMAT) == STANDARD) || ((TXDELBRS) == EXTENDED))

/*
 * @brief CANFD TX Frame Type enumeration
 */
typedef enum
{
    DATA 	    = 0x00, /* CAN  Standard Frame  */
    REMOTE 	    = 0x01, /* CANFD  Extanded Frame  */
}CANFDTXTYPE_TypeDef;
#define IS_CANFD_TXTYPE(TXTYPE) (((TXTYPE) == DATA) || ((TXTYPE) == REMOTE))


/*
 * @brief CANFD TX Frame Protocol enumeration
 */
typedef enum
{
    CAN	        = 0x00, /* CAN  Frame Protocol */
    CANFD	    = 0x02, /* CANFD  Frame Protocol */
    CANFD_BRS	= 0x03, /* CANFD Bit Rate Switch */
}CANFDTXPROTOCOL_TypeDef;
#define IS_CANFD_TXPROTOCOL(TXPROTOCOL) (((TXPROTOCOL) == CAN) || ((TXPROTOCOL) == CANFD) || \
                                     ((TXPROTOCOL) == CANFD_BRS))

/*
 * @brief CANFD TX Data Length enumeration
 */
typedef enum
{
    CANFD_DLC_0B	= 0x00, /* CANFD Data Length 0B */
    CANFD_DLC_1B	= 0x01, /* CANFD Data Length 1B */
    CANFD_DLC_2B	= 0x02, /* CANFD Data Length 2B */
    CANFD_DLC_3B	= 0x03, /* CANFD Data Length 3B */
    CANFD_DLC_4B	= 0x04, /* CANFD Data Length 4B */
    CANFD_DLC_5B	= 0x05, /* CANFD Data Length 5B */
    CANFD_DLC_6B	= 0x06, /* CANFD Data Length 6B */
    CANFD_DLC_7B	= 0x07, /* CANFD Data Length 7B */
    CANFD_DLC_8B	= 0x08, /* CANFD Data Length 8B */
    CANFD_DLC_12B	= 0x09, /* CANFD Data Length 12B */
    CANFD_DLC_16B	= 0x0A, /* CANFD Data Length 16B */
    CANFD_DLC_20B	= 0x0B, /* CANFD Data Length 20B */
    CANFD_DLC_24B	= 0x0C, /* CANFD Data Length 24B */
    CANFD_DLC_32B	= 0x0D, /* CANFD Data Length 32B */
    CANFD_DLC_48B	= 0x0E, /* CANFD Data Length 48B */
    CANFD_DLC_64B	= 0x0F, /* CANFD Data Length 64B */
}CANFDTXDLC_TypeDef;
#define IS_CANFD_TXDLC(TXDLC)  (((TXDLC) == CANFD_DLC_0B) || ((TXDLC) == CANFD_DLC_1B) || \
                                ((TXDLC) == CANFD_DLC_2B) || ((TXDLC) == CANFD_DLC_3B) || \
                                ((TXDLC) == CANFD_DLC_4B) || ((TXDLC) == CANFD_DLC_5B) || \
                                ((TXDLC) == CANFD_DLC_6B) || ((TXDLC) == CANFD_DLC_7B) || \
                                ((TXDLC) == CANFD_DLC_8B) || ((TXDLC) == CANFD_DLC_12B) || \
                                ((TXDLC) == CANFD_DLC_16B) || ((TXDLC) == CANFD_DLC_20B) || \
                                ((TXDLC) == CANFD_DLC_24B) || ((TXDLC) == CANFD_DLC_32B) || \
                                ((TXDLC) == CANFD_DLC_48B) || ((TXDLC) == CANFD_DLC_64B))

/*
 * @brief CANFD TX init structure definition
 */

typedef struct
{
    CANFDTXPROTOCOL_TypeDef CANFD_TX_PROTOCOL;      /* CANFD Frame Protocol choose */
    CANFDTXFORMAT_TypeDef   CANFD_TX_FORMAT;        /* CANFD Frame Format choose */
    CANFDTXTYPE_TypeDef     CANFD_TX_TYPE;          /* CANFD Frame Type choose */
    uint32_t                CANFD_Frame_ID;         /* CANFD Frame ID */
}CANFD_TXFrameTypeDef;


/*
 * @brief CANFD RX Info structure definition
 */
typedef struct
{
    uint8_t		            CANFD_Message_Count;
    CANFDTXPROTOCOL_TypeDef CANFD_CurrentMessage_PROTOCOL;              /* CANFD Frame Protocol choose */
    CANFDTXFORMAT_TypeDef   CANFD_CurrentMessage_FORMAT;                /* CANFD Frame Format choose */
    CANFDTXTYPE_TypeDef     CANFD_CurrentMessage_TYPE;                  /* CANFD Frame Type choose */
    uint32_t                CANFD_CurrentFrame_ID;
    uint32_t                CANFD_CurrentMessage_DLC;                   /* CANFD Current Data Length */
}CANFD_RXInfoTypeDef;


/*
 * @defgroup CANFD function selection define 
 */ 
/*
 * @brief CANFD Example ID
 */
#define CANFD_ID_FRAME1	    (0x9AE00000)   /* ID1 0x4D7 */
#define CANFD_ID_FRAME2	    (0xF8E00000)   /* ID2 0x7C7 */
#define CANFD_ID_FRAME3	    (0x9BE00000)   /* ID3 0x4DF */
#define CANFD_ID_EXTEND1    (0xF8EB3336)   /* ID1 Extended 0x7C7 */
#define CANFD_ID_147	    (0x28E00000)   /* ID1 0x147 */
#define CANFD_ID_0		    (0x3EE00000)   /* ID0 0x1F7 */
#define CANFD_ID_1		    (0x3CE00000)   /* ID1 0x1E7 */
#define CANFD_ID_2		    (0x3AE00000)   /* ID2 0x1D7 */
#define CANFD_ID_3		    (0x38E00000)   /* ID3 0x1C7 */
#define CANFD_ID_4		    (0x36E00000)   /* ID4 0x1B7 */
#define CANFD_ID_5		    (0x34E00000)   /* ID5 0x1A7 */
#define CANFD_ID_6		    (0x32E00000)   /* ID6 0x197 */
#define CANFD_ID_7		    (0x30E00000)   /* ID7 0x187 */
#define CANFD_ID_8		    (0x2EE00000)   /* ID8 0x177 */
#define CANFD_ID_9		    (0x2CE00000)   /* ID9 0x167 */
#define CANFD_ID_10		    (0x2AE00000)   /* ID10 0x157 */
#define CANFD_ID_11		    (0x28E00000)   /* ID11 0x147 */
#define CANFD_ID_12		    (0x26E00000)   /* ID12 0x137 */
#define CANFD_ID_13		    (0x24E00000)   /* ID13 0x127 */
#define CANFD_ID_14		    (0x22E00000)   /* ID14 0x117 */
#define CANFD_ID_15		    (0x20E00000)   /* ID15 0x107 */

/*
 * @brief CANFD Interrupts Definition
 */
#define CANFD_IT_EARBLST	    CANFD_IER_EARBLST	    /* Arbitration Lost Interrupt Enable */
#define CANFD_IT_ETXOK		    CANFD_IER_ETXOK		    /* Transmission Successful Interrupt Enable */
#define CANFD_IT_EPEE		    CANFD_IER_EPEE		    /* Protocol Exception Event Interrupt Enable */
#define CANFD_IT_EBSFRD		    CANFD_IER_EBSFRD	    /* Bus-Off Recovery Done Interrupt Enable */
#define CANFD_IT_ERXOK		    CANFD_IER_ERXOK		    /* New Message Received Interrupt Enable */
#define CANFD_IT_ETSCNT_OFLW    CANFD_IER_ETSCNT_OFLW   /* Timestamp Counter Overflow Interrupt Enable */
#define CANFD_IT_ERXFOFLW	    CANFD_IER_ERXFOFLW	    /* RX FIFO-0 Overflow Interrupt Enable (Sequential/FIFO Mode) */
#define CANFD_IT_EERROR		    CANFD_IER_EERROR	    /* Error Interrupt Enable */
#define CANFD_IT_EBSOFF		    CANFD_IER_EBSOFF	    /* Bus-Off Interrupt Enable */
#define CANFD_IT_ESLP		    CANFD_IER_ESLP		    /* Sleep Interrupt Enable */
#define CANFD_IT_EWKUP		    CANFD_IER_EWKUP		    /* Wake-Up Interrupt Enable */
#define CANFD_IT_ETXRRS		    CANFD_IER_ETXRRS	    /* TX Buffer Ready Request Served Interrupt Enable */
#define CANFD_IT_ETXCRS		    CANFD_IER_ETXCRS	    /* TX Cancellation Request Served Interrupt Enable */
#define CANFD_IT_ALL		    (0x00006F7F)		    /* ALL Interrupt Enable */
#define IS_CANFD_IT(IT) (((IT) == CANFD_IT_EARBLST)  || ((IT) == CANFD_IT_ETXOK)       ||  \
                         ((IT) == CANFD_IT_EPEE)     || ((IT) == CANFD_IT_EBSFRD)      ||  \
                         ((IT) == CANFD_IT_ERXOK)    || ((IT) == CANFD_IT_ETSCNT_OFLW) ||  \
                         ((IT) == CANFD_IT_ERXFOFLW) || ((IT) == CANFD_IT_EERROR)      ||  \
                         ((IT) == CANFD_IT_EBSOFF)   || ((IT) == CANFD_IT_ESLP)        ||  \
                         ((IT) == CANFD_IT_EWKUP)    || ((IT) == CANFD_IT_ETXRRS)      ||  \
                         ((IT) == CANFD_IT_ETXCRS)   || ((IT) == CANFD_IT_ALL))

/*
 * @brief CANFD Error Flags
 */
#define CANFD_ERROR_FLAG_CRCER	    CANFD_ESR_CRCER	    /* CANFD CRC Error flag */
#define CANFD_ERROR_FLAG_FMER	    CANFD_ESR_FMER	    /* CANFD Form Error flag */
#define CANFD_ERROR_FLAG_STER	    CANFD_ESR_STER	    /* CANFD Stuff Error flag */
#define CANFD_ERROR_FLAG_BERR	    CANFD_ESR_BERR	    /* CANFD Bit Error flag */
#define CANFD_ERROR_FLAG_ACKER	    CANFD_ESR_ACKER	    /* CANFD ACK Error flag */
#define CANFD_ERROR_FLAG_F_CRCER    CANFD_ESR_F_CRCER   /* CANFD CRC Error in CAN FD Data Phase flag */
#define CANFD_ERROR_FLAG_F_FMER	    CANFD_ESR_F_FMER    /* CANFD Form Error in CAN FD Data Phase flag */
#define CANFD_ERROR_FLAG_F_STER	    CANFD_ESR_F_STER    /* CANFD Stuff Error in CAN FD Data Phase flag */
#define CANFD_ERROR_FLAG_F_BERR	    CANFD_ESR_F_BERR    /* CANFD Bit Error in CAN FD Data Phase flag */
#define IS_CANFD_ESR_STATE(ESRSTATE)    (((ESRSTATE) == CANFD_ESR_CRCER)  || ((ESRSTATE) == CANFD_ESR_FMER)    ||  \
                                         ((ESRSTATE) == CANFD_ESR_STER)   || ((ESRSTATE) == CANFD_ESR_BERR)    ||  \
                                         ((ESRSTATE) == CANFD_ESR_ACKER)  || ((ESRSTATE) == CANFD_ESR_F_CRCER) ||  \
                                         ((ESRSTATE) == CANFD_ESR_F_FMER) || ((ESRSTATE) == CANFD_ESR_F_STER)  ||  \
                                         ((ESRSTATE) == CANFD_ESR_F_BERR))

/*
 * @brief CANFD Flags
 */
#define CANFD_FLAG_ARBLST    		CANFD_ISR_ARBLST        /* Arbitration lost interrupt flag. */
#define CANFD_FLAG_TXOK      		CANFD_ISR_TXOK          /* Transmission successful interrupt flag. */
#define CANFD_FLAG_PEE       		CANFD_ISR_PEE           /* Protocol exception event interrupt flag. */
#define CANFD_FLAG_BSFRD     		CANFD_ISR_BSFRD         /* Bus-off recovery done interrupt flag. */
#define CANFD_FLAG_RXOK      		CANFD_ISR_RXOK          /* New message received interrupt flag. */
#define CANFD_FLAG_TSCNT_OFLW		CANFD_ISR_TSCNT_OFLW    /* Timestamp counter overflow interrupt flag. */
#define CANFD_FLAG_RXFOFLW   		CANFD_ISR_RXFOFLW       /* Rx fifo-0 overflow interrupt flag (sequential/fifo mode). */
#define CANFD_FLAG_ERROE     		CANFD_ISR_ERROE         /* Error interrupt flag. */
#define CANFD_FLAG_BSOFF     		CANFD_ISR_BSOFF         /* Bus-off interrupt flag. */
#define CANFD_FLAG_SLP       		CANFD_ISR_SLP           /* Sleep interrupt flag. */
#define CANFD_FLAG_WKUP      		CANFD_ISR_WKUP          /* Wake-up interrupt flag. */
#define CANFD_FLAG_TXRRS     		CANFD_ISR_TXRRS         /* Tx buffer ready request served interrupt flag. */
#define CANFD_FLAG_TXCRS     		CANFD_ISR_TXCRS         /* Tx cancellation request served interrupt flag. */
#define IS_CANFD_FLAG(FLAG) (((FLAG) == CANFD_FLAG_ARBLST)  || ((FLAG) == CANFD_FLAG_TXOK)       || \
                             ((FLAG) == CANFD_FLAG_PEE)     || ((FLAG) == CANFD_FLAG_BSFRD)      || \
                             ((FLAG) == CANFD_FLAG_RXOK)    || ((FLAG) == CANFD_FLAG_TSCNT_OFLW) || \
                             ((FLAG) == CANFD_FLAG_RXFOFLW) || ((FLAG) == CANFD_FLAG_ERROE)      || \
                             ((FLAG) == CANFD_FLAG_BSOFF)   || ((FLAG) == CANFD_FLAG_SLP)        || \
                             ((FLAG) == CANFD_FLAG_WKUP)    || ((FLAG) == CANFD_FLAG_TXRRS)      || \
                             ((FLAG) == CANFD_FLAG_TXCRS))

/*
 * @brief CANFD Interruption Clear Flags
 */
#define CANFD_CLEAR_ARBLST    		CANFD_ICR_CARBLST     	/* Clears arbitration lost interrupt */
#define CANFD_CLEAR_TXOK      		CANFD_ICR_CTXOK       	/* Clears transmission successful interrupt */
#define CANFD_CLEAR_PEE       		CANFD_ICR_CPEE        	/* Clears protocol exception event interrupt */
#define CANFD_CLEAR_BSFRD     		CANFD_ICR_CBSFRD      	/* Clears bus-off recovery done interrupt */
#define CANFD_CLEAR_RXOK      		CANFD_ICR_CRXOK       	/* Clears new message received interrupt */
#define CANFD_CLEAR_TSCNT_OFLW		CANFD_ICR_CTSCNT_OFLW 	/* Clears timestamp counter overflow interrupt */
#define CANFD_CLEAR_RXFOFLW   		CANFD_ICR_CRXFOFLW    	/* Clears rx fifo-0 overflow interrupt (sequential/fifo mode). */
#define CANFD_CLEAR_ERROE     		CANFD_ICR_CERROE      	/* Clears error interrupt */
#define CANFD_CLEAR_BSOFF     		CANFD_ICR_CBSOFF      	/* Clears bus-off interrupt */
#define CANFD_CLEAR_SLP       		CANFD_ICR_CSLP        	/* Clears sleep interrupt */
#define CANFD_CLEAR_WKUP      		CANFD_ICR_CWKUP       	/* Clears wake-up interrupt */
#define CANFD_CLEAR_TXRRS     		CANFD_ICR_CTXRRS      	/* Clears tx buffer ready request served interrupt */
#define CANFD_CLEAR_TXCRS     		CANFD_ICR_CTXCRS      	/* Clears tx cancellation request served interrupt */
#define CANFD_CLEAR_ALL     		0x6F7F      	        /* Clears all interrupt */
#define IS_CANFD_CLEAR(CLEAR) (((CLEAR) == CANFD_CLEAR_ARBLST)  || ((CLEAR) == CANFD_CLEAR_TXOK)       || \
                               ((CLEAR) == CANFD_CLEAR_PEE)     || ((CLEAR) == CANFD_CLEAR_BSFRD)      || \
                               ((CLEAR) == CANFD_CLEAR_RXOK)    || ((CLEAR) == CANFD_CLEAR_TSCNT_OFLW) || \
                               ((CLEAR) == CANFD_CLEAR_RXFOFLW) || ((CLEAR) == CANFD_CLEAR_ERROE) 	   || \
                               ((CLEAR) == CANFD_CLEAR_BSOFF)   || ((CLEAR) == CANFD_CLEAR_SLP)        || \
							   ((CLEAR) == CANFD_CLEAR_WKUP)    || ((CLEAR) == CANFD_CLEAR_TXRRS)      || \
                               ((CLEAR) == CANFD_CLEAR_TXCRS)   || ((CLEAR) == CANFD_CLEAR_ALL) )

/*
 * @brief CANFD Channel
 */
#define CANFD_TB0   (0x00000001)   /* TB0 */
#define CANFD_TB1   (0x00000002)   /* TB1 */
#define CANFD_TB2   (0x00000004)   /* TB2 */
#define CANFD_TB3   (0x00000008)   /* TB3 */
#define CANFD_TB4   (0x00000010)   /* TB4 */
#define CANFD_TB5   (0x00000020)   /* TB5 */
#define CANFD_TB6   (0x00000040)   /* TB6 */
#define CANFD_TB7   (0x00000080)   /* TB7 */
#define CANFD_TB8   (0x00000100)   /* TB8 */
#define CANFD_TB9   (0x00000200)   /* TB9 */
#define CANFD_TB10  (0x00000400)   /* TB10 */
#define CANFD_TB11  (0x00000800)   /* TB11 */
#define CANFD_TB12  (0x00001000)   /* TB12 */
#define CANFD_TB13  (0x00002000)   /* TB13 */
#define CANFD_TB14  (0x00004000)   /* TB14 */
#define CANFD_TB15  (0x00008000)   /* TB15 */
#define CANFD_TB16  (0x00010000)   /* TB16 */
#define CANFD_TB17  (0x00020000)   /* TB17 */
#define CANFD_TB18  (0x00040000)   /* TB18 */
#define CANFD_TB19  (0x00080000)   /* TB19 */
#define CANFD_TB20  (0x00100000)   /* TB20 */
#define CANFD_TB21  (0x00200000)   /* TB21 */
#define CANFD_TB22  (0x00400000)   /* TB22 */
#define CANFD_TB23  (0x00800000)   /* TB23 */
#define CANFD_TB24  (0x01000000)   /* TB24 */
#define CANFD_TB25  (0x02000000)   /* TB25 */
#define CANFD_TB26  (0x04000000)   /* TB26 */
#define CANFD_TB27  (0x08000000)   /* TB27 */
#define CANFD_TB28  (0x10000000)   /* TB28 */
#define CANFD_TB29  (0x20000000)   /* TB29 */
#define CANFD_TB30  (0x40000000)   /* TB30 */
#define CANFD_TB31  (0x80000000)   /* TB31 */
#define CANFD_TBALL (0xFFFFFFFF)   /* TB_ALL */
#define IS_CANFD_TB_SPACE(TBSPACE) (((TBSPACE) == CANFD_TB0) || ((TBSPACE) == CANFD_TB1) ||    \
                                    ((TBSPACE) == CANFD_TB2) || ((TBSPACE) == CANFD_TB3) ||    \
                                    ((TBSPACE) == CANFD_TB4) || ((TBSPACE) == CANFD_TB5) ||    \
                                    ((TBSPACE) == CANFD_TB6) || ((TBSPACE) == CANFD_TB7) ||    \
                                    ((TBSPACE) == CANFD_TB8) || ((TBSPACE) == CANFD_TB9) ||    \
                                    ((TBSPACE) == CANFD_TB10) || ((TBSPACE) == CANFD_TB11) ||  \
                                    ((TBSPACE) == CANFD_TB12) || ((TBSPACE) == CANFD_TB13) ||  \
                                    ((TBSPACE) == CANFD_TB14) || ((TBSPACE) == CANFD_TB15) ||  \
                                    ((TBSPACE) == CANFD_TB16) || ((TBSPACE) == CANFD_TB17) ||  \
                                    ((TBSPACE) == CANFD_TB18) || ((TBSPACE) == CANFD_TB19) ||  \
                                    ((TBSPACE) == CANFD_TB20) || ((TBSPACE) == CANFD_TB21) ||  \
                                    ((TBSPACE) == CANFD_TB22) || ((TBSPACE) == CANFD_TB23) ||  \
                                    ((TBSPACE) == CANFD_TB24) || ((TBSPACE) == CANFD_TB25) ||  \
                                    ((TBSPACE) == CANFD_TB26) || ((TBSPACE) == CANFD_TB27) ||  \
                                    ((TBSPACE) == CANFD_TB28) || ((TBSPACE) == CANFD_TB29) ||  \
                                    ((TBSPACE) == CANFD_TB30) || ((TBSPACE) == CANFD_TB31) ||  \
                                    ((TBSPACE) == CANFD_TBALL))

/*
 * @brief CANFD Channel
 */
#define CANFD_Filter0   (0x00000000)   /* Filter0 */
#define CANFD_Filter1   (0x00000001)   /* Filter1 */
#define CANFD_Filter2   (0x00000002)   /* Filter2 */
#define CANFD_Filter3   (0x00000003)   /* Filter3 */
#define CANFD_Filter4   (0x00000004)   /* Filter4 */
#define CANFD_Filter5   (0x00000005)   /* Filter5 */
#define CANFD_Filter6   (0x00000006)   /* Filter6 */
#define CANFD_Filter7   (0x00000007)   /* Filter7 */
#define CANFD_Filter8   (0x00000008)   /* Filter8 */
#define CANFD_Filter9   (0x00000009)   /* Filter9 */
#define CANFD_Filter10  (0x0000000A)   /* Filter10 */
#define CANFD_Filter11  (0x0000000B)   /* Filter11 */
#define CANFD_Filter12  (0x0000000C)   /* Filter12 */
#define CANFD_Filter13  (0x0000000D)   /* Filter13 */
#define CANFD_Filter14  (0x0000000E)   /* Filter14 */
#define CANFD_Filter15  (0x0000000F)   /* Filter15 */
#define CANFD_Filter16  (0x00000010)   /* Filter16 */
#define CANFD_Filter17  (0x00000011)   /* Filter17 */
#define CANFD_Filter18  (0x00000012)   /* Filter18 */
#define CANFD_Filter19  (0x00000013)   /* Filter19 */
#define CANFD_Filter20  (0x00000014)   /* Filter20 */
#define CANFD_Filter21  (0x00000015)   /* Filter21 */
#define CANFD_Filter22  (0x00000016)   /* Filter22 */
#define CANFD_Filter23  (0x00000017)   /* Filter23 */
#define CANFD_Filter24  (0x00000018)   /* Filter24 */
#define CANFD_Filter25  (0x00000019)   /* Filter25 */
#define CANFD_Filter26  (0x0000001A)   /* Filter26 */
#define CANFD_Filter27  (0x0000001B)   /* Filter27 */
#define CANFD_Filter28  (0x0000001C)   /* Filter28 */
#define CANFD_Filter29  (0x0000001D)   /* Filter29 */
#define CANFD_Filter30  (0x0000001E)   /* Filter30 */
#define CANFD_Filter31  (0x0000001F)   /* Filter31 */
#define CANFD_FilterALL (0xFFFFFFFF)   /* Filter_ALL */
#define IS_CANFD_Filter_SPACE(TBSPACE) (((TBSPACE) == CANFD_Filter0) || ((TBSPACE) == CANFD_Filter1) ||    \
                                    ((TBSPACE) == CANFD_Filter2) || ((TBSPACE) == CANFD_Filter3) ||    \
                                    ((TBSPACE) == CANFD_Filter4) || ((TBSPACE) == CANFD_Filter5) ||    \
                                    ((TBSPACE) == CANFD_Filter6) || ((TBSPACE) == CANFD_Filter7) ||    \
                                    ((TBSPACE) == CANFD_Filter8) || ((TBSPACE) == CANFD_Filter9) ||    \
                                    ((TBSPACE) == CANFD_Filter10) || ((TBSPACE) == CANFD_Filter11) ||  \
                                    ((TBSPACE) == CANFD_Filter12) || ((TBSPACE) == CANFD_Filter13) ||  \
                                    ((TBSPACE) == CANFD_Filter14) || ((TBSPACE) == CANFD_Filter15) ||  \
                                    ((TBSPACE) == CANFD_Filter16) || ((TBSPACE) == CANFD_Filter17) ||  \
                                    ((TBSPACE) == CANFD_Filter18) || ((TBSPACE) == CANFD_Filter19) ||  \
                                    ((TBSPACE) == CANFD_Filter20) || ((TBSPACE) == CANFD_Filter21) ||  \
                                    ((TBSPACE) == CANFD_Filter22) || ((TBSPACE) == CANFD_Filter23) ||  \
                                    ((TBSPACE) == CANFD_Filter24) || ((TBSPACE) == CANFD_Filter25) ||  \
                                    ((TBSPACE) == CANFD_Filter26) || ((TBSPACE) == CANFD_Filter27) ||  \
                                    ((TBSPACE) == CANFD_Filter28) || ((TBSPACE) == CANFD_Filter29) ||  \
                                    ((TBSPACE) == CANFD_Filter30) || ((TBSPACE) == CANFD_Filter31) ||  \
                                    ((TBSPACE) == CANFD_FilterALL))
/*
 * @brief CANFD Register space channel
 */
#define TB0	    (0x00000000)    /* Register space TB0 */
#define TB1	    (0x00000001)    /* Register space TB1 */
#define TB2	    (0x00000002)    /* Register space TB2 */
#define TB3	    (0x00000003)    /* Register space TB3 */
#define TB4	    (0x00000004)    /* Register space TB4 */
#define TB5	    (0x00000005)    /* Register space TB5 */
#define TB6	    (0x00000006)    /* Register space TB6 */
#define TB7	    (0x00000007)    /* Register space TB7 */
#define TB8	    (0x00000008)    /* Register space TB8 */
#define TB9	    (0x00000009)    /* Register space TB9 */
#define TB10        (0x0000000A)    /* Register space TB10 */
#define TB11        (0x0000000B)    /* Register space TB11 */
#define TB12        (0x0000000C)    /* Register space TB12 */
#define TB13        (0x0000000D)    /* Register space TB13 */
#define TB14        (0x0000000E)    /* Register space TB14 */
#define TB15        (0x0000000F)    /* Register space TB15 */
#define TB16        (0x00000010)    /* Register space TB16 */
#define TB17        (0x00000011)    /* Register space TB17 */
#define TB18        (0x00000012)    /* Register space TB18 */
#define TB19        (0x00000013)    /* Register space TB19 */
#define TB20        (0x00000014)    /* Register space TB20 */
#define TB21        (0x00000015)    /* Register space TB21 */
#define TB22        (0x00000016)    /* Register space TB22 */
#define TB23        (0x00000017)    /* Register space TB23 */
#define TB24        (0x00000018)    /* Register space TB24 */
#define TB25        (0x00000019)    /* Register space TB25 */
#define TB26        (0x0000001A)    /* Register space TB26 */
#define TB27        (0x0000001B)    /* Register space TB27 */
#define TB28        (0x0000001C)    /* Register space TB28 */
#define TB29        (0x0000001D)    /* Register space TB29 */
#define TB30        (0x0000001E)    /* Register space TB30 */
#define TB31        (0x0000001F)    /* Register space TB31 */
#define IS_CANFD_TB_NUMBER(TBNUMBER)   (((TBNUMBER) == TB0) || ((TBNUMBER) == TB1) ||      \
                                        ((TBNUMBER) == TB2) || ((TBNUMBER) == TB3) ||    \
                                        ((TBNUMBER) == TB4) || ((TBNUMBER) == TB5) ||    \
                                        ((TBNUMBER) == TB6) || ((TBNUMBER) == TB7) ||    \
                                        ((TBNUMBER) == TB8) || ((TBNUMBER) == TB9) ||    \
                                        ((TBNUMBER) == TB10) || ((TBNUMBER) == TB11) ||  \
                                        ((TBNUMBER) == TB12) || ((TBNUMBER) == TB13) ||  \
                                        ((TBNUMBER) == TB14) || ((TBNUMBER) == TB15) ||  \
                                        ((TBNUMBER) == TB16) || ((TBNUMBER) == TB17) ||  \
                                        ((TBNUMBER) == TB18) || ((TBNUMBER) == TB19) ||  \
                                        ((TBNUMBER) == TB20) || ((TBNUMBER) == TB21) ||  \
                                        ((TBNUMBER) == TB22) || ((TBNUMBER) == TB23) ||  \
                                        ((TBNUMBER) == TB24) || ((TBNUMBER) == TB25) ||  \
                                        ((TBNUMBER) == TB26) || ((TBNUMBER) == TB27) ||  \
                                        ((TBNUMBER) == TB28) || ((TBNUMBER) == TB29) ||  \
                                        ((TBNUMBER) == TB30) || ((TBNUMBER) == TB31))

/*
 * @brief CANFD send channel
 */
#define SEND_TB0    CANFD_TRR_RR_0  /* TB0 send */
#define SEND_TB1    CANFD_TRR_RR_1  /* TB1 send */
#define SEND_TB2    CANFD_TRR_RR_2  /* TB2 send */
#define SEND_TB3    CANFD_TRR_RR_3  /* TB3 send */
#define SEND_TB4    CANFD_TRR_RR_4  /* TB4 send */
#define SEND_TB5    CANFD_TRR_RR_5  /* TB5 send */
#define SEND_TB6    CANFD_TRR_RR_6  /* TB6 send */
#define SEND_TB7    CANFD_TRR_RR_7  /* TB7 send */
#define SEND_TB8    CANFD_TRR_RR_8  /* TB8 send */
#define SEND_TB9    CANFD_TRR_RR_9  /* TB9 send */
#define SEND_TB10   CANFD_TRR_RR_10 /* TB10 send */
#define SEND_TB11   CANFD_TRR_RR_11 /* TB11 send */
#define SEND_TB12   CANFD_TRR_RR_12 /* TB12 send */
#define SEND_TB13   CANFD_TRR_RR_13 /* TB13 send */
#define SEND_TB14   CANFD_TRR_RR_14 /* TB14 send */
#define SEND_TB15   CANFD_TRR_RR_15 /* TB15 send */
#define SEND_TB16   CANFD_TRR_RR_16 /* TB16 send */
#define SEND_TB17   CANFD_TRR_RR_17 /* TB17 send */
#define SEND_TB18   CANFD_TRR_RR_18 /* TB18 send */
#define SEND_TB19   CANFD_TRR_RR_19 /* TB19 send */
#define SEND_TB20   CANFD_TRR_RR_20 /* TB20 send */
#define SEND_TB21   CANFD_TRR_RR_21 /* TB21 send */
#define SEND_TB22   CANFD_TRR_RR_22 /* TB22 send */
#define SEND_TB23   CANFD_TRR_RR_23 /* TB23 send */
#define SEND_TB24   CANFD_TRR_RR_24 /* TB24 send */
#define SEND_TB25   CANFD_TRR_RR_25 /* TB25 send */
#define SEND_TB26   CANFD_TRR_RR_26 /* TB26 send */
#define SEND_TB27   CANFD_TRR_RR_27 /* TB27 send */
#define SEND_TB28   CANFD_TRR_RR_28 /* TB28 send */
#define SEND_TB29   CANFD_TRR_RR_29 /* TB29 send */
#define SEND_TB30   CANFD_TRR_RR_30 /* TB30 send */
#define SEND_TB31   CANFD_TRR_RR_31 /* TB31 send */
#define SEND_ALL    CANFD_TRR_RR    /* ALL send */
#define IS_CANFD_TB_SEND(TBSEND)   (((TBSEND) == SEND_TB0) || ((TBSEND) == SEND_TB1) ||      \
                                    ((TBSEND) == SEND_TB2) || ((TBSEND) == SEND_TB3) ||    \
                                    ((TBSEND) == SEND_TB4) || ((TBSEND) == SEND_TB5) ||    \
                                    ((TBSEND) == SEND_TB6) || ((TBSEND) == SEND_TB7) ||    \
                                    ((TBSEND) == SEND_TB8) || ((TBSEND) == SEND_TB9) ||    \
                                    ((TBSEND) == SEND_TB10) || ((TBSEND) == SEND_TB11) ||  \
                                    ((TBSEND) == SEND_TB12) || ((TBSEND) == SEND_TB13) ||  \
                                    ((TBSEND) == SEND_TB14) || ((TBSEND) == SEND_TB15) ||  \
                                    ((TBSEND) == SEND_TB16) || ((TBSEND) == SEND_TB17) ||  \
                                    ((TBSEND) == SEND_TB18) || ((TBSEND) == SEND_TB19) ||  \
                                    ((TBSEND) == SEND_TB20) || ((TBSEND) == SEND_TB21) ||  \
                                    ((TBSEND) == SEND_TB22) || ((TBSEND) == SEND_TB23) ||  \
                                    ((TBSEND) == SEND_TB24) || ((TBSEND) == SEND_TB25) ||  \
                                    ((TBSEND) == SEND_TB26) || ((TBSEND) == SEND_TB27) ||  \
                                    ((TBSEND) == SEND_TB28) || ((TBSEND) == SEND_TB29) ||  \
                                    ((TBSEND) == SEND_TB30) || ((TBSEND) == SEND_TB31) ||  \
                                    ((TBSEND) == SEND_ALL))

/*
 * @brief CANFD Data Length
 */
#define	 Byte_0     (0x00)  /* CANFD Data Length 0B */
#define	 Byte_1     (0x01)  /* CANFD Data Length 1B */
#define	 Byte_2     (0x02)  /* CANFD Data Length 2B */
#define	 Byte_3     (0x03)  /* CANFD Data Length 3B */
#define	 Byte_4     (0x04)  /* CANFD Data Length 4B */
#define	 Byte_5     (0x05)  /* CANFD Data Length 5B */
#define	 Byte_6     (0x06)  /* CANFD Data Length 6B */
#define	 Byte_7     (0x07)  /* CANFD Data Length 7B */
#define	 Byte_8     (0x08)  /* CANFD Data Length 8B */
#define	 Byte_12    (0x0C)  /* CANFD Data Length 12B */
#define	 Byte_16    (0x10)  /* CANFD Data Length 16B */
#define	 Byte_20    (0x14)  /* CANFD Data Length 20B */
#define	 Byte_24    (0x18)  /* CANFD Data Length 24B */
#define	 Byte_32    (0x20)  /* CANFD Data Length 32B */
#define	 Byte_48    (0x30)  /* CANFD Data Length 48B */
#define	 Byte_64    (0x40)  /* CANFD Data Length 64B */
#define IS_CANFD_DATA_LENGHT(DATALENGHT)   (((DATALENGHT) == Byte_0) || ((DATALENGHT) == Byte_1) ||      \
                                            ((DATALENGHT) == Byte_2) || ((DATALENGHT) == Byte_3) ||    \
                                            ((DATALENGHT) == Byte_4) || ((DATALENGHT) == Byte_5) ||    \
                                            ((DATALENGHT) == Byte_6) || ((DATALENGHT) == Byte_7) ||    \
                                            ((DATALENGHT) == Byte_8) || ((DATALENGHT) == Byte_12) ||   \
                                            ((DATALENGHT) == Byte_16) || ((DATALENGHT) == Byte_20) ||  \
                                            ((DATALENGHT) == Byte_24) || ((DATALENGHT) == Byte_32) ||  \
                                            ((DATALENGHT) == Byte_48) || ((DATALENGHT) == Byte_64))

/*
 * @brief Function declaration
 */
void CANFD_StructInit(CANFD_InitTypeDef* CANFD_InitStruct);
void CANFD_Init(CANFD_TypeDef* CANFDx , CANFD_InitTypeDef* CANFD_InitStruct);
void CANFD_Sleep(CANFD_TypeDef* CANFDx);
void CANFD_WakeUp(CANFD_TypeDef* CANFDx);
void CANFD_SetLoopback(CANFD_TypeDef* CANFDx);
void CANFD_CloseLoopback(CANFD_TypeDef* CANFDx);
void CANFD_SetSnoop(CANFD_TypeDef* CANFDx);
void CANFD_CloseSnoop(CANFD_TypeDef* CANFDx);
void CANFD_BitRateConfig(CANFD_TypeDef* CANFDx, EnState NewState);
void CANFD_AutoRetransConfig(CANFD_TypeDef* CANFDx, EnState NewState);
void CANFD_ProtExcConfig(CANFD_TypeDef* CANFDx, EnState NewState);
void CANFD_SetBusOff(CANFD_TypeDef* CANFDx);
void CANFD_AutoBusOffRecoConfig(CANFD_TypeDef* CANFDx, EnState NewState);
void CANFD_TDCConfig(CANFD_TypeDef* CANFDx, EnState NewState, uint8_t TDC_OFFSET);
void CANFD_ReceiveALL(CANFD_TypeDef* CANFDx,EnState NewState);
void CANFD_FilterInit(CANFD_TypeDef* CANFDx, uint32_t CANFD_Filterx, uint32_t Afm, uint32_t Afi);
void CANFD_FrameStructInit(CANFD_TXFrameTypeDef* CANFD_TXFrameStruct);
void CANFD_FrameInit(CANFD_TypeDef* CANFDx, uint32_t RSTBx, CANFD_TXFrameTypeDef* CANFD_TXFrameStruct);
void CANFD_Transmit(CANFD_TypeDef* CANFDx, uint32_t RSTBx, uint8_t* Canfdbuf, uint32_t Buflength);
void CANFD_NormalStatus(CANFD_TypeDef* CANFDx);
uint32_t CANFD_GetStatus(CANFD_TypeDef* CANFDx, uint32_t state);
void CANFD_ITConfig(CANFD_TypeDef* CANFDx, uint32_t ITType, EnState NewState);
void CANFD_SetSendChannel(CANFD_TypeDef* CANFDx , uint32_t SendTb);
FlagStatus CANFD_GetSendCmpStatus(CANFD_TypeDef* CANFDx, uint32_t SendTb);
uint32_t CANFD_GetTimtampValue(CANFD_TypeDef* CANFDx);
void CANFD_ClearTimtamp(CANFD_TypeDef* CANFDx);
FlagStatus CANFD_GetErrorStatus(CANFD_TypeDef* CANFDx, uint32_t ErrorState);
uint32_t CANFD_GetRXErrorCount(CANFD_TypeDef* CANFDx);
uint32_t CANFD_GetTXErrorCount(CANFD_TypeDef* CANFDx);
void CANFD_SetBufferReadyIT(CANFD_TypeDef* CANFDx, uint32_t Cleartb, EnState NewState);
void CANFD_CancelTransmit(CANFD_TypeDef* CANFDx, uint32_t TcrTb);
void CANFD_TxBufferCancelIT(CANFD_TypeDef* CANFDx, uint32_t EcrsTb, EnState NewState);
uint32_t CANFD_GetRxFillLevel(CANFD_TypeDef* CANFDx);
uint32_t CANFD_GetRxReadIndex(CANFD_TypeDef* CANFDx);
void CANFD_SetRxIncReadIndex(CANFD_TypeDef* CANFDx);
void CANFD_Enable(CANFD_TypeDef* CANFDx);
ITStatus CANFD_GetITStatus(CANFD_TypeDef *CANFDx, uint32_t Flag);
void CANFD_ClearITPendingBit(CANFD_TypeDef *CANFDx, uint32_t ClearIT);
uint32_t CANFD_GetReceiveBytes(CANFD_TypeDef* CANFDx);
uint32_t CANFD_GetReceiveData(CANFD_TypeDef* CANFDx, uint8_t* Data);
CANFD_RXInfoTypeDef CANFD_GetReceiveInfo(CANFD_TypeDef* CANFDx);
void CANFD_GetReceiveMessage(CANFD_TypeDef* CANFDx, uint8_t* Data);

#ifdef __cplusplus
}
#endif

#endif 

/*****END OF FILE*****/ 
