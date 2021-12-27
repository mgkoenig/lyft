/*
 * hal_nvm.h
 *
 * Created: 09/12/2021 11:48:28
 *  Author: Sire
 */ 


#ifndef HAL_NVM_H_
#define HAL_NVM_H_

#include <asf.h>
#include "conf_nvmmap.h"

#define NVM_BASE_ADDRESS		(NVMCTRL_FLASH_SIZE - NVMCTRL_ROW_SIZE)				// for 128kB memory device variant (SAMD-21E17)

enum nvm_memorymap {
	NVM_INDEX_0					= 0,
	NVM_INDEX_1					= 1,
	NVM_INDEX_2					= 2,
	NVM_INDEX_3					= 3,
	NVM_INDEX_4					= 4,
	NVM_INDEX_5					= 5,
	NVM_INDEX_6					= 6,
	NVM_INDEX_7					= 7,
	NVM_INDEX_8					= 8,
	NVM_INDEX_9					= 9,
	NVM_INDEX_10				= 10,
	NVM_INDEX_11				= 11,
	NVM_INDEX_12				= 12,
	NVM_INDEX_13				= 13,
	NVM_INDEX_14				= 14,
	NVM_INDEX_15				= 15,
	NVM_INDEX_16				= 16,
	NVM_INDEX_17				= 17,
	NVM_INDEX_18				= 18,
	NVM_INDEX_19				= 19,
	NVM_INDEX_20				= 20,
	NVM_INDEX_21				= 21,
	NVM_INDEX_22				= 22,
	NVM_INDEX_23				= 23,
	NVM_INDEX_24				= 24,
	NVM_INDEX_25				= 25,
	NVM_INDEX_26				= 26,
	NVM_INDEX_27				= 27,
	NVM_INDEX_28				= 28,
	NVM_INDEX_29				= 29,
	NVM_INDEX_30				= 30,
	NVM_INDEX_31				= 31,
	NVM_INDEX_32				= 32,
	NVM_INDEX_33				= 33,
	NVM_INDEX_34				= 34,
	NVM_INDEX_35				= 35,
	NVM_INDEX_36				= 36,
	NVM_INDEX_37				= 37,
	NVM_INDEX_38				= 38,
	NVM_INDEX_39				= 39,
	NVM_INDEX_40				= 40,
	NVM_INDEX_41				= 41,
	NVM_INDEX_42				= 42,
	NVM_INDEX_43				= 43,
	NVM_INDEX_44				= 44,
	NVM_INDEX_45				= 45,
	NVM_INDEX_46				= 46,
	NVM_INDEX_47				= 47,
	NVM_INDEX_48				= 48,
	NVM_INDEX_49				= 49,
	NVM_INDEX_50				= 50,
	NVM_INDEX_51				= 51,
	NVM_INDEX_52				= 52,
	NVM_INDEX_53				= 53,
	NVM_INDEX_54				= 54,
	NVM_INDEX_55				= 55,
	NVM_INDEX_56				= 56,
	NVM_INDEX_57				= 57,
	NVM_INDEX_58				= 58,
	NVM_INDEX_59				= 59,
	NVM_INDEX_60				= 60,
	NVM_INDEX_61				= 61,
	NVM_INDEX_62				= 62,
	NVM_INDEX_63				= 63,
	NVM_INDEX_64				= 64,
	NVM_INDEX_65				= 65,
	NVM_INDEX_66				= 66,
	NVM_INDEX_67				= 67,
	NVM_INDEX_68				= 68,
	NVM_INDEX_69				= 69,
	NVM_INDEX_70				= 70,
	NVM_INDEX_71				= 71,
	NVM_INDEX_72				= 72,
	NVM_INDEX_73				= 73,
	NVM_INDEX_74				= 74,
	NVM_INDEX_75				= 75,
	NVM_INDEX_76				= 76,
	NVM_INDEX_77				= 77,
	NVM_INDEX_78				= 78,
	NVM_INDEX_79				= 79,
	NVM_INDEX_80				= 80,
	NVM_INDEX_81				= 81,
	NVM_INDEX_82				= 82,
	NVM_INDEX_83				= 83,
	NVM_INDEX_84				= 84,
	NVM_INDEX_85				= 85,
	NVM_INDEX_86				= 86,
	NVM_INDEX_87				= 87,
	NVM_INDEX_88				= 88,
	NVM_INDEX_89				= 89,
	NVM_INDEX_90				= 90,
	NVM_INDEX_91				= 91,
	NVM_INDEX_92				= 92,
	NVM_INDEX_93				= 93,
	NVM_INDEX_94				= 94,
	NVM_INDEX_95				= 95,
	NVM_INDEX_96				= 96,
	NVM_INDEX_97				= 97,
	NVM_INDEX_98				= 98,
	NVM_INDEX_99				= 99,
	NVM_INDEX_100				= 100,
	NVM_INDEX_101				= 101,
	NVM_INDEX_102				= 102,
	NVM_INDEX_103				= 103,
	NVM_INDEX_104				= 104,
	NVM_INDEX_105				= 105,
	NVM_INDEX_106				= 106,
	NVM_INDEX_107				= 107,
	NVM_INDEX_108				= 108,
	NVM_INDEX_109				= 109,
	NVM_INDEX_110				= 110,
	NVM_INDEX_111				= 111,
	NVM_INDEX_112				= 112,
	NVM_INDEX_113				= 113,
	NVM_INDEX_114				= 114,
	NVM_INDEX_115				= 115,
	NVM_INDEX_116				= 116,
	NVM_INDEX_117				= 117,
	NVM_INDEX_118				= 118,
	NVM_INDEX_119				= 119,
	NVM_INDEX_120				= 120,
	NVM_INDEX_121				= 121,
	NVM_INDEX_122				= 122,
	NVM_INDEX_123				= 123,
	NVM_INDEX_124				= 124,
	NVM_INDEX_125				= 125,
	NVM_INDEX_126				= 126,
	NVM_INDEX_127				= 127,
	NVM_INDEX_128				= 128,
	NVM_INDEX_129				= 129,
	NVM_INDEX_130				= 130,
	NVM_INDEX_131				= 131,
	NVM_INDEX_132				= 132,
	NVM_INDEX_133				= 133,
	NVM_INDEX_134				= 134,
	NVM_INDEX_135				= 135,
	NVM_INDEX_136				= 136,
	NVM_INDEX_137				= 137,
	NVM_INDEX_138				= 138,
	NVM_INDEX_139				= 139,
	NVM_INDEX_140				= 140,
	NVM_INDEX_141				= 141,
	NVM_INDEX_142				= 142,
	NVM_INDEX_143				= 143,
	NVM_INDEX_144				= 144,
	NVM_INDEX_145				= 145,
	NVM_INDEX_146				= 146,
	NVM_INDEX_147				= 147,
	NVM_INDEX_148				= 148,
	NVM_INDEX_149				= 149,
	NVM_INDEX_150				= 150,
	NVM_INDEX_151				= 151,
	NVM_INDEX_152				= 152,
	NVM_INDEX_153				= 153,
	NVM_INDEX_154				= 154,
	NVM_INDEX_155				= 155,
	NVM_INDEX_156				= 156,
	NVM_INDEX_157				= 157,
	NVM_INDEX_158				= 158,
	NVM_INDEX_159				= 159,
	NVM_INDEX_160				= 160,
	NVM_INDEX_161				= 161,
	NVM_INDEX_162				= 162,
	NVM_INDEX_163				= 163,
	NVM_INDEX_164				= 164,
	NVM_INDEX_165				= 165,
	NVM_INDEX_166				= 166,
	NVM_INDEX_167				= 167,
	NVM_INDEX_168				= 168,
	NVM_INDEX_169				= 169,
	NVM_INDEX_170				= 170,
	NVM_INDEX_171				= 171,
	NVM_INDEX_172				= 172,
	NVM_INDEX_173				= 173,
	NVM_INDEX_174				= 174,
	NVM_INDEX_175				= 175,
	NVM_INDEX_176				= 176,
	NVM_INDEX_177				= 177,
	NVM_INDEX_178				= 178,
	NVM_INDEX_179				= 179,
	NVM_INDEX_180				= 180,
	NVM_INDEX_181				= 181,
	NVM_INDEX_182				= 182,
	NVM_INDEX_183				= 183,
	NVM_INDEX_184				= 184,
	NVM_INDEX_185				= 185,
	NVM_INDEX_186				= 186,
	NVM_INDEX_187				= 187,
	NVM_INDEX_188				= 188,
	NVM_INDEX_189				= 189,
	NVM_INDEX_190				= 190,
	NVM_INDEX_191				= 191,
	NVM_INDEX_192				= 192,
	NVM_INDEX_193				= 193,
	NVM_INDEX_194				= 194,
	NVM_INDEX_195				= 195,
	NVM_INDEX_196				= 196,
	NVM_INDEX_197				= 197,
	NVM_INDEX_198				= 198,
	NVM_INDEX_199				= 199,
	NVM_INDEX_200				= 200,
	NVM_INDEX_201				= 201,
	NVM_INDEX_202				= 202,
	NVM_INDEX_203				= 203,
	NVM_INDEX_204				= 204,
	NVM_INDEX_205				= 205,
	NVM_INDEX_206				= 206,
	NVM_INDEX_207				= 207,
	NVM_INDEX_208				= 208,
	NVM_INDEX_209				= 209,
	NVM_INDEX_210				= 210,
	NVM_INDEX_211				= 211,
	NVM_INDEX_212				= 212,
	NVM_INDEX_213				= 213,
	NVM_INDEX_214				= 214,
	NVM_INDEX_215				= 215,
	NVM_INDEX_216				= 216,
	NVM_INDEX_217				= 217,
	NVM_INDEX_218				= 218,
	NVM_INDEX_219				= 219,
	NVM_INDEX_220				= 220,
	NVM_INDEX_221				= 221,
	NVM_INDEX_222				= 222,
	NVM_INDEX_223				= 223,
	NVM_INDEX_224				= 224,
	NVM_INDEX_225				= 225,
	NVM_INDEX_226				= 226,
	NVM_INDEX_227				= 227,
	NVM_INDEX_228				= 228,
	NVM_INDEX_229				= 229,
	NVM_INDEX_230				= 230,
	NVM_INDEX_231				= 231,
	NVM_INDEX_232				= 232,
	NVM_INDEX_233				= 233,
	NVM_INDEX_234				= 234,
	NVM_INDEX_235				= 235,
	NVM_INDEX_236				= 236,
	NVM_INDEX_237				= 237,
	NVM_INDEX_238				= 238,
	NVM_INDEX_239				= 239,
	NVM_INDEX_240				= 240,
	NVM_INDEX_241				= 241,
	NVM_INDEX_242				= 242,
	NVM_INDEX_243				= 243,
	NVM_INDEX_244				= 244,
	NVM_INDEX_245				= 245,
	NVM_INDEX_246				= 246,
	NVM_INDEX_247				= 247,
	NVM_INDEX_248				= 248,
	NVM_INDEX_249				= 249,
	NVM_INDEX_250				= 250,
	NVM_INDEX_251				= 251,
	NVM_INDEX_252				= 252,
	NVM_INDEX_253				= 253,
	NVM_INDEX_254				= 254,
	NVM_INDEX_255				= 255,
	NVM_INDEX_MAX				= 256
};

void hal_nvm_init(void);
void hal_nvm_read(uint8_t nvm_idx, uint8_t *data, uint16_t len);
void hal_nvm_write(uint8_t nvm_idx, uint8_t *data, uint16_t len);


#endif /* HAL_NVM_H_ */