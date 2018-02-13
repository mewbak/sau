#####################################################################
###
#
# Makefile for compiling StormLib under linux
#
# Author: Marko Friedemann <marko.friedemann@bmx-chemnitz.de>
# Created at: Mon Jan 29 18:26:01 CEST 2001
# Computer: whiplash.flachland-chemnitz.de
# System: Linux 2.4.0 on i686
#
# Copyright (c) 2001 BMX-Chemnitz.DE All rights reserved.
#
#####################################################################
###

##############################################################
# updated on Monday 3, 2010 by Christopher Chedeau aka vjeux #
# updated on April 24, 2010 by Ivan Komissarov aka Nevermore #
##############################################################

C++ = g++
CC = gcc
AR = ar
DFLAGS =
OFLAGS =
CFLAGS = -D_7ZIP_ST
CFLAGS += $(OFLAGS) $(DFLAGS)

OBJS =  adpcm/adpcm.o \
	FileStream.o \
	huffman/huff.o \
	SBaseCommon.o \
	SBaseFileTable.o \
	SCompression.o \
	SFileAddFile.o \
	SFileAttributes.o \
	SFileCompactArchive.o \
	SFileCreateArchive.o \
	SFileExtractFile.o \
	SFileFindFile.o \
	SFileListFile.o \
	SFileOpenArchive.o \
	SFileOpenFileEx.o \
	SFilePatchArchives.o \
	SFileReadFile.o \
	SFileVerify.o \
	sparse/sparse.o

COBJS = libtomcrypt/src/hashes/sha1.o \
	libtomcrypt/src/hashes/hash_memory.o \
	libtomcrypt/src/hashes/md5.o \
	libtomcrypt/src/misc/crypt_hash_is_valid.o \
	libtomcrypt/src/misc/crypt_prng_descriptor.o \
	libtomcrypt/src/misc/crypt_register_prng.o \
	libtomcrypt/src/misc/crypt_ltc_mp_descriptor.o \
	libtomcrypt/src/misc/crypt_find_hash.o \
	libtomcrypt/src/misc/zeromem.o \
	libtomcrypt/src/misc/base64_decode.o \
	libtomcrypt/src/misc/crypt_register_hash.o \
	libtomcrypt/src/misc/crypt_find_prng.o \
	libtomcrypt/src/misc/crypt_prng_is_valid.o \
	libtomcrypt/src/misc/crypt_hash_descriptor.o \
	libtomcrypt/src/misc/crypt_libc.o \
	libtomcrypt/src/misc/crypt_argchk.o \
	libtomcrypt/src/math/multi.o \
	libtomcrypt/src/math/ltm_desc.o \
	libtomcrypt/src/math/rand_prime.o \
	libtomcrypt/src/pk/asn1/der_length_ia5_string.o \
	libtomcrypt/src/pk/asn1/der_decode_utctime.o \
	libtomcrypt/src/pk/asn1/der_length_boolean.o \
	libtomcrypt/src/pk/asn1/der_decode_object_identifier.o \
	libtomcrypt/src/pk/asn1/der_decode_sequence_multi.o \
	libtomcrypt/src/pk/asn1/der_decode_octet_string.o \
	libtomcrypt/src/pk/asn1/der_length_object_identifier.o \
	libtomcrypt/src/pk/asn1/der_length_bit_string.o \
	libtomcrypt/src/pk/asn1/der_decode_ia5_string.o \
	libtomcrypt/src/pk/asn1/der_length_integer.o \
	libtomcrypt/src/pk/asn1/der_length_sequence.o \
	libtomcrypt/src/pk/asn1/der_decode_choice.o \
	libtomcrypt/src/pk/asn1/der_length_octet_string.o \
	libtomcrypt/src/pk/asn1/der_decode_sequence_flexi.o \
	libtomcrypt/src/pk/asn1/der_decode_printable_string.o \
	libtomcrypt/src/pk/asn1/der_decode_bit_string.o \
	libtomcrypt/src/pk/asn1/der_decode_short_integer.o \
	libtomcrypt/src/pk/asn1/der_length_utctime.o \
	libtomcrypt/src/pk/asn1/der_decode_utf8_string.o \
	libtomcrypt/src/pk/asn1/der_decode_integer.o \
	libtomcrypt/src/pk/asn1/der_decode_boolean.o \
	libtomcrypt/src/pk/asn1/der_sequence_free.o \
	libtomcrypt/src/pk/asn1/der_decode_sequence_ex.o \
	libtomcrypt/src/pk/asn1/der_length_short_integer.o \
	libtomcrypt/src/pk/asn1/der_length_printable_string.o \
	libtomcrypt/src/pk/asn1/der_length_utf8_string.o \
	libtomcrypt/src/pk/rsa/rsa_make_key.o \
	libtomcrypt/src/pk/rsa/rsa_free.o \
	libtomcrypt/src/pk/rsa/rsa_verify_simple.o \
	libtomcrypt/src/pk/rsa/rsa_import.o \
	libtomcrypt/src/pk/rsa/rsa_verify_hash.o \
	libtomcrypt/src/pk/rsa/rsa_exptmod.o \
	libtomcrypt/src/pk/pkcs1/pkcs_1_v1_5_decode.o \
	libtomcrypt/src/pk/pkcs1/pkcs_1_pss_decode.o \
	libtomcrypt/src/pk/pkcs1/pkcs_1_mgf1.o \
	libtomcrypt/src/pk/pkcs1/pkcs_1_oaep_decode.o \
	libtomcrypt/src/pk/ecc/ltc_ecc_projective_dbl_point.o \
	libtomcrypt/src/pk/ecc/ltc_ecc_mulmod.o \
	libtomcrypt/src/pk/ecc/ltc_ecc_projective_add_point.o \
	libtomcrypt/src/pk/ecc/ltc_ecc_map.o \
	libtomcrypt/src/pk/ecc/ltc_ecc_points.o \
	libtomcrypt/src/pk/ecc/ltc_ecc_mul2add.o \
	libtommath/bn_mp_exptmod_fast.o \
	libtommath/bn_mp_jacobi.o \
	libtommath/bn_mp_mod.o \
	libtommath/bn_mp_signed_bin_size.o \
	libtommath/bn_mp_invmod.o \
	libtommath/bn_mp_is_square.o \
	libtommath/bn_mp_neg.o \
	libtommath/bn_mp_reduce_2k.o \
	libtommath/bn_mp_xor.o \
	libtommath/bn_mp_karatsuba_mul.o \
	libtommath/bn_mp_dr_setup.o \
	libtommath/bn_mp_mul.o \
	libtommath/bn_mp_init_multi.o \
	libtommath/bn_mp_clear.o \
	libtommath/bn_s_mp_sqr.o \
	libtommath/bn_mp_rshd.o \
	libtommath/bn_s_mp_sub.o \
	libtommath/bn_mp_sub.o \
	libtommath/bn_mp_toradix.o \
	libtommath/bn_mp_reduce.o \
	libtommath/bn_mp_prime_is_prime.o \
	libtommath/bn_mp_prime_next_prime.o \
	libtommath/bn_mp_exptmod.o \
	libtommath/bn_mp_mod_2d.o \
	libtommath/bn_reverse.o \
	libtommath/bn_mp_init.o \
	libtommath/bn_fast_s_mp_sqr.o \
	libtommath/bn_mp_sqr.o \
	libtommath/bn_mp_cnt_lsb.o \
	libtommath/bn_mp_clear_multi.o \
	libtommath/bn_mp_exch.o \
	libtommath/bn_fast_s_mp_mul_digs.o \
	libtommath/bn_mp_grow.o \
	libtommath/bn_mp_read_radix.o \
	libtommath/bn_mp_mul_2.o \
	libtommath/bn_mp_shrink.o \
	libtommath/bn_mp_div_2.o \
	libtommath/bn_fast_mp_invmod.o \
	libtommath/bn_mp_prime_miller_rabin.o \
	libtommath/bn_mp_to_unsigned_bin.o \
	libtommath/bn_mp_prime_rabin_miller_trials.o \
	libtommath/bn_mp_2expt.o \
	libtommath/bn_mp_cmp_mag.o \
	libtommath/bn_mp_to_signed_bin.o \
	libtommath/bn_mp_get_int.o \
	libtommath/bn_mp_montgomery_reduce.o \
	libtommath/bn_mp_dr_reduce.o \
	libtommath/bn_mp_fwrite.o \
	libtommath/bn_mp_and.o \
	libtommath/bn_mp_exteuclid.o \
	libtommath/bn_fast_mp_montgomery_reduce.o \
	libtommath/bn_s_mp_mul_high_digs.o \
	libtommath/bn_mp_reduce_setup.o \
	libtommath/bn_mp_lcm.o \
	libtommath/bn_mp_abs.o \
	libtommath/bn_mp_cmp.o \
	libtommath/bn_mp_submod.o \
	libtommath/bn_mp_div_d.o \
	libtommath/bn_s_mp_mul_digs.o \
	libtommath/bn_mp_mul_d.o \
	libtommath/bn_mp_to_unsigned_bin_n.o \
	libtommath/bn_mp_prime_random_ex.o \
	libtommath/bn_mp_rand.o \
	libtommath/bn_mp_div_2d.o \
	libtommath/bn_mp_addmod.o \
	libtommath/bn_mp_init_copy.o \
	libtommath/bn_mp_read_unsigned_bin.o \
	libtommath/bn_mp_toradix_n.o \
	libtommath/bn_fast_s_mp_mul_high_digs.o \
	libtommath/bn_mp_toom_sqr.o \
	libtommath/bn_mp_to_signed_bin_n.o \
	libtommath/bn_mp_reduce_2k_setup_l.o \
	libtommath/bn_mp_div.o \
	libtommath/bn_prime_tab.o \
	libtommath/bn_mp_karatsuba_sqr.o \
	libtommath/bn_mp_gcd.o \
	libtommath/bn_mp_prime_is_divisible.o \
	libtommath/bn_mp_set_int.o \
	libtommath/bn_mp_prime_fermat.o \
	libtommath/bn_mp_cmp_d.o \
	libtommath/bn_mp_add.o \
	libtommath/bn_mp_sub_d.o \
	libtommath/bn_s_mp_exptmod.o \
	libtommath/bn_mp_init_size.o \
	libtommath/bncore.o \
	libtommath/bn_mp_radix_smap.o \
	libtommath/bn_mp_reduce_2k_l.o \
	libtommath/bn_mp_montgomery_calc_normalization.o \
	libtommath/bn_mp_mod_d.o \
	libtommath/bn_mp_set.o \
	libtommath/bn_mp_or.o \
	libtommath/bn_mp_sqrt.o \
	libtommath/bn_mp_invmod_slow.o \
	libtommath/bn_mp_count_bits.o \
	libtommath/bn_mp_read_signed_bin.o \
	libtommath/bn_mp_div_3.o \
	libtommath/bn_mp_unsigned_bin_size.o \
	libtommath/bn_mp_mulmod.o \
	libtommath/bn_mp_clamp.o \
	libtommath/bn_mp_reduce_2k_setup.o \
	libtommath/bn_mp_toom_mul.o \
	libtommath/bn_mp_montgomery_setup.o \
	libtommath/bn_mp_expt_d.o \
	libtommath/bn_mp_copy.o \
	libtommath/bn_mp_dr_is_modulus.o \
	libtommath/bn_mp_sqrmod.o \
	libtommath/bn_mp_reduce_is_2k_l.o \
	libtommath/bn_mp_mul_2d.o \
	libtommath/bn_mp_fread.o \
	libtommath/bn_mp_init_set.o \
	libtommath/bn_mp_add_d.o \
	libtommath/bn_mp_zero.o \
	libtommath/bn_s_mp_add.o \
	libtommath/bn_mp_radix_size.o \
	libtommath/bn_mp_init_set_int.o \
	libtommath/bn_mp_n_root.o \
	libtommath/bn_mp_lshd.o \
	libtommath/bn_mp_reduce_is_2k.o \
	pklib/implode.o \
	pklib/crc32.o \
	pklib/explode.o \
	lzma/C/LzFind.o \
	lzma/C/LzmaEnc.o \
	lzma/C/LzmaDec.o \
	jenkins/lookup3.o

LIB = libStorm.so
SLIB = libStorm.a


#all: $(OBJS) $(COBJS) $(LIB) $(SLIB)
all: $(COBJS) $(OBJS) $(SLIB)

#$(LIB): $(OBJS) $(COBJS)
#	$(C++) $(ARCH) -shared -o $(LIB) $(OBJS) $(COBJS) $(LFLAGS)

$(SLIB): $(OBJS) $(COBJS)
	$(AR) rcs $(SLIB) $(COBJS) $(OBJS)

clean:
	rm -f $(OBJS) $(COBJS) $(LIB)
	rm -f $(SLIB)

$(OBJS): %.o: %.cpp
	$(C++) -o $@ $(CFLAGS) -c $<

$(COBJS): %.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

$(LIB): $(COBJS) $(OBJS)

all: $(LIB)

