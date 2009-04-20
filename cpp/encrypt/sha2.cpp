/**
 * Copyright (C) 2008 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-01-01
 */


// #include <standard library headers>
#include <fstream>

// #include <other library headers>

// #include "customer headers"
#include "sha2.h"


#pragma warning(push)
#pragma warning(disable: 4996)


/*  1. PLATFORM SPECIFIC INCLUDES */
#if defined(__GNU_LIBRARY__)
#  include <byteswap.h>
#  include <endian.h>
#elif defined(__CRYPTLIB__)
#  if defined( INC_ALL )
#    include "crypt.h"
#  elif defined( INC_CHILD )
#    include "../crypt.h"
#  else
#    include "crypt.h"
#  endif
#  if defined(DATA_LITTLEENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#  else
#    define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#  endif
#elif defined(_MSC_VER)
#  include <stdlib.h>
#elif !defined(WIN32)
#  include <stdlib.h>
#  if !defined (_ENDIAN_H)
#    include <sys/param.h>
#  else
#    include _ENDIAN_H
#  endif
#endif

/*  2. BYTE ORDER IN 32-BIT WORDS
    To obtain the highest speed on processors with 32-bit words, this code
    needs to determine the order in which bytes are packed into such words.
    The following block of code is an attempt to capture the most obvious
    ways in which various environemnts specify their endian definitions.
    It may well fail, in which case the definitions will need to be set by
    editing at the points marked **** EDIT HERE IF NECESSARY **** below.
*/
#define SHA_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */
#define SHA_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */

#if !defined(PLATFORM_BYTE_ORDER)
#if defined(LITTLE_ENDIAN) || defined(BIG_ENDIAN)
#  if defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#    if defined(BYTE_ORDER)
#      if   (BYTE_ORDER == LITTLE_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#      elif (BYTE_ORDER == BIG_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#      endif
#    endif
#  elif defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#  elif !defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#  endif
#elif defined(_LITTLE_ENDIAN) || defined(_BIG_ENDIAN)
#  if defined(_LITTLE_ENDIAN) && defined(_BIG_ENDIAN)
#    if defined(_BYTE_ORDER)
#      if   (_BYTE_ORDER == _LITTLE_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#      elif (_BYTE_ORDER == _BIG_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#      endif
#    endif
#  elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#  elif !defined(_LITTLE_ENDIAN) && defined(_BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#  endif
#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#elif (('1234' >> 24) == '1')
#  define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#elif (('4321' >> 24) == '1')
#  define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#endif
#endif

#if !defined(PLATFORM_BYTE_ORDER)
#  error Please set undetermined byte order (lines 159 or 161 of sha2.c).
#endif

#ifdef _MSC_VER
#pragma intrinsic(memcpy)
#endif

#define rotr32(x,n)   (((x) >> n) | ((x) << (32 - n)))

#if !defined(bswap_32)
#define bswap_32(x) (rotr32((x), 24) & 0x00ff00ff | rotr32((x), 8) & 0xff00ff00)
#endif

#if (PLATFORM_BYTE_ORDER == SHA_LITTLE_ENDIAN)
#define SWAP_BYTES
#else
#undef  SWAP_BYTES
#endif

#define SHA256_MASK (SHA256_BLOCK_SIZE - 1)

#if defined(SWAP_BYTES)
#define bsw_32(p,n) { int _i = (n); while(_i--) p[_i] = bswap_32(p[_i]); }
#else
#define bsw_32(p,n)
#endif

/* SHA256 mixing function definitions   */
#define ch(x,y,z)   (((x) & (y)) ^ (~(x) & (z)))
#define maj(x,y,z)  (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define s256_0(x) (rotr32((x),  2) ^ rotr32((x), 13) ^ rotr32((x), 22))
#define s256_1(x) (rotr32((x),  6) ^ rotr32((x), 11) ^ rotr32((x), 25))
#define g256_0(x) (rotr32((x),  7) ^ rotr32((x), 18) ^ ((x) >>  3))
#define g256_1(x) (rotr32((x), 17) ^ rotr32((x), 19) ^ ((x) >> 10))

/* rotated SHA256 round definition. Rather than swapping variables as in    */
/* FIPS-180, different variables are 'rotated' on each round, returning     */
/* to their starting positions every eight rounds                           */
#define h2(i) _ctx.wbuf[i & 15] +=                                      \
        g256_1(_ctx.wbuf[(i + 14) & 15]) + _ctx.wbuf[(i + 9) & 15] + g256_0(_ctx.wbuf[(i + 1) & 15])

#define h2_cycle(i,j)                                                   \
    v[(7 - i) & 7] += (j ? h2(i) : _ctx.wbuf[i & 15]) + k256[i + j]     \
        +s256_1(v[(4 - i) & 7]) + ch(v[(4 - i) & 7], v[(5 - i) & 7], v[(6 - i) & 7]); \
    v[(3 - i) & 7] += v[(7 - i) & 7];                                   \
    v[(7 - i) & 7] += s256_0(v[(0 - i) & 7]) + maj(v[(0 - i) & 7], v[(1 - i) & 7], v[(2 - i) & 7])

#define SHA512_MASK (SHA512_BLOCK_SIZE - 1)

#define rotr64(x,n)   (((x) >> n) | ((x) << (64 - n)))

#if !defined(bswap_64)
#define bswap_64(x) (((sha2_64t)(bswap_32((sha2_32t)(x)))) << 32 | bswap_32((sha2_32t)((x) >> 32)))
#endif

#if defined(SWAP_BYTES)
#define bsw_64(p,n) { int _i = (n); while(_i--) p[_i] = bswap_64(p[_i]); }
#else
#define bsw_64(p,n)
#endif


namespace encrypt
{

    /* SHA256 mixing data   */
    const sha2_32t k256[64] =
    {
        n_u32(428a2f98), n_u32(71374491), n_u32(b5c0fbcf), n_u32(e9b5dba5),
        n_u32(3956c25b), n_u32(59f111f1), n_u32(923f82a4), n_u32(ab1c5ed5),
        n_u32(d807aa98), n_u32(12835b01), n_u32(243185be), n_u32(550c7dc3),
        n_u32(72be5d74), n_u32(80deb1fe), n_u32(9bdc06a7), n_u32(c19bf174),
        n_u32(e49b69c1), n_u32(efbe4786), n_u32(0fc19dc6), n_u32(240ca1cc),
        n_u32(2de92c6f), n_u32(4a7484aa), n_u32(5cb0a9dc), n_u32(76f988da),
        n_u32(983e5152), n_u32(a831c66d), n_u32(b00327c8), n_u32(bf597fc7),
        n_u32(c6e00bf3), n_u32(d5a79147), n_u32(06ca6351), n_u32(14292967),
        n_u32(27b70a85), n_u32(2e1b2138), n_u32(4d2c6dfc), n_u32(53380d13),
        n_u32(650a7354), n_u32(766a0abb), n_u32(81c2c92e), n_u32(92722c85),
        n_u32(a2bfe8a1), n_u32(a81a664b), n_u32(c24b8b70), n_u32(c76c51a3),
        n_u32(d192e819), n_u32(d6990624), n_u32(f40e3585), n_u32(106aa070),
        n_u32(19a4c116), n_u32(1e376c08), n_u32(2748774c), n_u32(34b0bcb5),
        n_u32(391c0cb3), n_u32(4ed8aa4a), n_u32(5b9cca4f), n_u32(682e6ff3),
        n_u32(748f82ee), n_u32(78a5636f), n_u32(84c87814), n_u32(8cc70208),
        n_u32(90befffa), n_u32(a4506ceb), n_u32(bef9a3f7), n_u32(c67178f2),
    };

    /* SHA256 initialisation data */
    const sha2_32t i256[8] =
    {
        n_u32(6a09e667), n_u32(bb67ae85), n_u32(3c6ef372), n_u32(a54ff53a),
        n_u32(510e527f), n_u32(9b05688c), n_u32(1f83d9ab), n_u32(5be0cd19)
    };

    /* SHA256 Final padding and digest calculation  */
    static sha2_32t  m1[4] =
    {
        n_u32(00000000), n_u32(ff000000), n_u32(ffff0000), n_u32(ffffff00)
    };
    static sha2_32t  b1[4] =
    {
        n_u32(80000000), n_u32(00800000), n_u32(00008000), n_u32(00000080)
    };


    /* SHA512 mixing function definitions   */
#define s512_0(x) (rotr64((x), 28) ^ rotr64((x), 34) ^ rotr64((x), 39))
#define s512_1(x) (rotr64((x), 14) ^ rotr64((x), 18) ^ rotr64((x), 41))
#define g512_0(x) (rotr64((x),  1) ^ rotr64((x),  8) ^ ((x) >>  7))
#define g512_1(x) (rotr64((x), 19) ^ rotr64((x), 61) ^ ((x) >>  6))

    /* rotated SHA512 round definition. Rather than swapping variables as in */
    /* FIPS-180, different variables are 'rotated' on each round, returning  */
    /* to their starting positions every eight rounds                        */
#define h5(i) _ctx.wbuf[i & 15] +=                                      \
        g512_1(_ctx.wbuf[(i + 14) & 15]) + _ctx.wbuf[(i + 9) & 15] + g512_0(_ctx.wbuf[(i + 1) & 15])

#define h5_cycle(i,j)                                                   \
    v[(7 - i) & 7] += (j ? h5(i) : _ctx.wbuf[i & 15]) + k512[i + j]     \
        +s512_1(v[(4 - i) & 7]) + ch(v[(4 - i) & 7], v[(5 - i) & 7], v[(6 - i) & 7]); \
    v[(3 - i) & 7] += v[(7 - i) & 7];                                   \
    v[(7 - i) & 7] += s512_0(v[(0 - i) & 7]) + maj(v[(0 - i) & 7], v[(1 - i) & 7], v[(2 - i) & 7])

    /* SHA384/SHA512 mixing data    */
    const sha2_64t  k512[80] =
    {
        n_u64(428a2f98d728ae22), n_u64(7137449123ef65cd),
        n_u64(b5c0fbcfec4d3b2f), n_u64(e9b5dba58189dbbc),
        n_u64(3956c25bf348b538), n_u64(59f111f1b605d019),
        n_u64(923f82a4af194f9b), n_u64(ab1c5ed5da6d8118),
        n_u64(d807aa98a3030242), n_u64(12835b0145706fbe),
        n_u64(243185be4ee4b28c), n_u64(550c7dc3d5ffb4e2),
        n_u64(72be5d74f27b896f), n_u64(80deb1fe3b1696b1),
        n_u64(9bdc06a725c71235), n_u64(c19bf174cf692694),
        n_u64(e49b69c19ef14ad2), n_u64(efbe4786384f25e3),
        n_u64(0fc19dc68b8cd5b5), n_u64(240ca1cc77ac9c65),
        n_u64(2de92c6f592b0275), n_u64(4a7484aa6ea6e483),
        n_u64(5cb0a9dcbd41fbd4), n_u64(76f988da831153b5),
        n_u64(983e5152ee66dfab), n_u64(a831c66d2db43210),
        n_u64(b00327c898fb213f), n_u64(bf597fc7beef0ee4),
        n_u64(c6e00bf33da88fc2), n_u64(d5a79147930aa725),
        n_u64(06ca6351e003826f), n_u64(142929670a0e6e70),
        n_u64(27b70a8546d22ffc), n_u64(2e1b21385c26c926),
        n_u64(4d2c6dfc5ac42aed), n_u64(53380d139d95b3df),
        n_u64(650a73548baf63de), n_u64(766a0abb3c77b2a8),
        n_u64(81c2c92e47edaee6), n_u64(92722c851482353b),
        n_u64(a2bfe8a14cf10364), n_u64(a81a664bbc423001),
        n_u64(c24b8b70d0f89791), n_u64(c76c51a30654be30),
        n_u64(d192e819d6ef5218), n_u64(d69906245565a910),
        n_u64(f40e35855771202a), n_u64(106aa07032bbd1b8),
        n_u64(19a4c116b8d2d0c8), n_u64(1e376c085141ab53),
        n_u64(2748774cdf8eeb99), n_u64(34b0bcb5e19b48a8),
        n_u64(391c0cb3c5c95a63), n_u64(4ed8aa4ae3418acb),
        n_u64(5b9cca4f7763e373), n_u64(682e6ff3d6b2b8a3),
        n_u64(748f82ee5defb2fc), n_u64(78a5636f43172f60),
        n_u64(84c87814a1f0ab72), n_u64(8cc702081a6439ec),
        n_u64(90befffa23631e28), n_u64(a4506cebde82bde9),
        n_u64(bef9a3f7b2c67915), n_u64(c67178f2e372532b),
        n_u64(ca273eceea26619c), n_u64(d186b8c721c0c207),
        n_u64(eada7dd6cde0eb1e), n_u64(f57d4f7fee6ed178),
        n_u64(06f067aa72176fba), n_u64(0a637dc5a2c898a6),
        n_u64(113f9804bef90dae), n_u64(1b710b35131c471b),
        n_u64(28db77f523047d84), n_u64(32caab7b40c72493),
        n_u64(3c9ebe0a15c9bebc), n_u64(431d67c49c100d4c),
        n_u64(4cc5d4becb3e42b6), n_u64(597f299cfc657e2a),
        n_u64(5fcb6fab3ad6faec), n_u64(6c44198c4a475817)
    };

    /* SHA384/512 Final padding and digest calculation  */
    static sha2_64t  m2[8] =
    {
        n_u64(0000000000000000), n_u64(ff00000000000000),
        n_u64(ffff000000000000), n_u64(ffffff0000000000),
        n_u64(ffffffff00000000), n_u64(ffffffffff000000),
        n_u64(ffffffffffff0000), n_u64(ffffffffffffff00)
    };
    static sha2_64t  b2[8] =
    {
        n_u64(8000000000000000), n_u64(0080000000000000),
        n_u64(0000800000000000), n_u64(0000008000000000),
        n_u64(0000000080000000), n_u64(0000000000800000),
        n_u64(0000000000008000), n_u64(0000000000000080)
    };

    /* SHA384 initialisation data   */
    const sha2_64t  i384[80] =
    {
        n_u64(cbbb9d5dc1059ed8), n_u64(629a292a367cd507),
        n_u64(9159015a3070dd17), n_u64(152fecd8f70e5939),
        n_u64(67332667ffc00b31), n_u64(8eb44a8768581511),
        n_u64(db0c2e0d64f98fa7), n_u64(47b5481dbefa4fa4)
    };

    /* SHA512 initialisation data   */
    const sha2_64t  i512[80] =
    {
        n_u64(6a09e667f3bcc908), n_u64(bb67ae8584caa73b),
        n_u64(3c6ef372fe94f82b), n_u64(a54ff53a5f1d36f1),
        n_u64(510e527fade682d1), n_u64(9b05688c2b3e6c1f),
        n_u64(1f83d9abfb41bd6b), n_u64(5be0cd19137e2179)
    };


    /**
     * description
     */
    SHA256::SHA256()
    {
        Init();
    }


    /**
     * description
     */
    SHA256::~SHA256()
    {
    }


    void SHA256::Init()
    {
        _ctx.count[0] = _ctx.count[1] = 0;
        memcpy(_ctx.hash, i256, 8 * sizeof(sha2_32t));
    }


    /* SHA256 hash data in an array of bytes into hash buffer   */
    /* and call the hash_compile function as required.          */
    void SHA256::Update(const void* in, unsigned long len)
    {
        const unsigned char* data = (const unsigned char*)in;
        sha2_32t pos = (sha2_32t)(_ctx.count[0] & SHA256_MASK),
            space = SHA256_BLOCK_SIZE - pos;
        const unsigned char *sp = data;

        if((_ctx.count[0] += len) < len)
            ++(_ctx.count[1]);

        while(len >= space)     /* tranfer whole blocks while possible  */
        {
            memcpy(((unsigned char*)_ctx.wbuf) + pos, sp, space);
            sp += space; len -= space; space = SHA256_BLOCK_SIZE; pos = 0;
            bsw_32(_ctx.wbuf, SHA256_BLOCK_SIZE >> 2)
                Compile();
        }

        memcpy(((unsigned char*)_ctx.wbuf) + pos, sp, len);
    }


    void SHA256::Final()
    {
        sha2_32t    i = (sha2_32t)(_ctx.count[0] & SHA256_MASK);

        bsw_32(_ctx.wbuf, (i + 3) >> 2)
            /* bytes in the buffer are now in an order in which references  */
            /* to 32-bit words will put bytes with lower addresses into the */
            /* top of 32 bit words on BOTH big and little endian machines   */

            /* we now need to mask valid bytes and add the padding which is */
            /* a single 1 bit and as many zero bits as necessary.           */
            _ctx.wbuf[i >> 2] = (_ctx.wbuf[i >> 2] & m1[i & 3]) | b1[i & 3];

        /* we need 9 or more empty positions, one for the padding byte  */
        /* (above) and eight for the length count.  If there is not     */
        /* enough space pad and empty the buffer                        */
        if(i > SHA256_BLOCK_SIZE - 9)
        {
            if(i < 60) _ctx.wbuf[15] = 0;
            Compile();
            i = 0;
        }
        else    /* compute a word index for the empty buffer positions  */
            i = (i >> 2) + 1;

        while(i < 14) /* and zero pad all but last two positions      */
            _ctx.wbuf[i++] = 0;

        /* the following 32-bit length fields are assembled in the      */
        /* wrong byte order on little endian machines but this is       */
        /* corrected later since they are only ever used as 32-bit      */
        /* word values.                                                 */

        _ctx.wbuf[14] = (_ctx.count[1] << 3) | (_ctx.count[0] >> 29);
        _ctx.wbuf[15] = _ctx.count[0] << 3;

        Compile();

        /* extract the hash value as bytes in case the hash buffer is   */
        /* mislaigned for 32-bit words                                  */
        for(i = 0; i < SHA256_DIGEST_SIZE; ++i)
        {
            _digest[i] = (unsigned char)(_ctx.hash[i >> 2] >> 8 * (~i & 3));
        }
    }


    /**
     * Convert unsigned char array to hex string.
     */
    std::string SHA256::BytesToHexString(const unsigned char* input,
                                         unsigned int length)
    {
        std::string str;
        str.reserve(length << 1);
        char b[3];
        for (unsigned int i = 0; i < length; i++)
        {
            sprintf(b, "%02X", input[i]);
            str.append(1, b[0]);
            str.append(1, b[1]);
        }
        return str;
    }

    /* Compile 64 bytes of hash data into SHA256 digest value   */
    /* NOTE: this routine assumes that the byte order in the    */
    /* _ctx.wbuf[] at this point is in such an order that low   */
    /* address bytes in the ORIGINAL byte stream placed in this */
    /* buffer will now go to the high end of words on BOTH big  */
    /* and little endian systems                                */
    void SHA256::Compile()
    {
        sha2_32t        v[8], j;

        memcpy(v, _ctx.hash, 8 * sizeof(sha2_32t));

        for(j = 0; j < 64; j += 16)
        {
            h2_cycle( 0, j); h2_cycle( 1, j); h2_cycle( 2, j); h2_cycle( 3, j);
            h2_cycle( 4, j); h2_cycle( 5, j); h2_cycle( 6, j); h2_cycle( 7, j);
            h2_cycle( 8, j); h2_cycle( 9, j); h2_cycle(10, j); h2_cycle(11, j);
            h2_cycle(12, j); h2_cycle(13, j); h2_cycle(14, j); h2_cycle(15, j);
        }

        _ctx.hash[0] += v[0];
        _ctx.hash[1] += v[1];
        _ctx.hash[2] += v[2];
        _ctx.hash[3] += v[3];
        _ctx.hash[4] += v[4];
        _ctx.hash[5] += v[5];
        _ctx.hash[6] += v[6];
        _ctx.hash[7] += v[7];
    }


    /**
     * description
     */
    SHA384::SHA384()
    {
        Init();
    }


    /**
     * description
     */
    SHA384::~SHA384()
    {
    }


    void SHA384::Init()
    {
        _ctx.count[0] = _ctx.count[1] = 0;
        memcpy(_ctx.hash, i384, 8 * sizeof(sha2_64t));
    }


    /* SHA384 hash data in an array of bytes into hash buffer   */
    /* and call the hash_compile function as required.          */
    void SHA384::Update(const void* in, unsigned long len)
    {
        const unsigned char* data = (const unsigned char*)in;
        sha2_32t pos = (sha2_32t)(_ctx.count[0] & SHA512_MASK),
            space = SHA512_BLOCK_SIZE - pos;
        const unsigned char *sp = data;

        if((_ctx.count[0] += len) < len)
            ++(_ctx.count[1]);

        while(len >= space)     /* tranfer whole blocks while possible  */
        {
            memcpy(((unsigned char*)_ctx.wbuf) + pos, sp, space);
            sp += space; len -= space; space = SHA512_BLOCK_SIZE; pos = 0;
            bsw_64(_ctx.wbuf, SHA512_BLOCK_SIZE >> 3);
            Compile();
        }

        memcpy(((unsigned char*)_ctx.wbuf) + pos, sp, len);
    }


    void SHA384::Final()
    {
        sha2_32t    i = (sha2_32t)(_ctx.count[0] & SHA512_MASK);

        bsw_64(_ctx.wbuf, (i + 7) >> 3);

        /* bytes in the buffer are now in an order in which references  */
        /* to 64-bit words will put bytes with lower addresses into the */
        /* top of 64 bit words on BOTH big and little endian machines   */

        /* we now need to mask valid bytes and add the padding which is */
        /* a single 1 bit and as many zero bits as necessary.           */
        _ctx.wbuf[i >> 3] = (_ctx.wbuf[i >> 3] & m2[i & 7]) | b2[i & 7];

        /* we need 17 or more empty byte positions, one for the padding */
        /* byte (above) and sixteen for the length count.  If there is  */
        /* not enough space pad and empty the buffer                    */
        if(i > SHA512_BLOCK_SIZE - 17)
        {
            if(i < 120) _ctx.wbuf[15] = 0;
            Compile();
            i = 0;
        }
        else
            i = (i >> 3) + 1;

        while(i < 14)
            _ctx.wbuf[i++] = 0;

        /* the following 64-bit length fields are assembled in the      */
        /* wrong byte order on little endian machines but this is       */
        /* corrected later since they are only ever used as 64-bit      */
        /* word values.                                                 */

        _ctx.wbuf[14] = (_ctx.count[1] << 3) | (_ctx.count[0] >> 61);
        _ctx.wbuf[15] = _ctx.count[0] << 3;

        Compile();

        /* extract the hash value as bytes in case the hash buffer is   */
        /* misaligned for 32-bit words                                  */
        for(i = 0; i < SHA384_DIGEST_SIZE; ++i)
        {
            _digest[i] = (unsigned char)(_ctx.hash[i >> 3] >> 8 * (~i & 7));
        }

    }


    /**
     * Convert unsigned char array to hex string.
     */
    std::string SHA384::BytesToHexString(const unsigned char* input,
                                         unsigned int length)
    {
        std::string str;
        str.reserve(length << 1);
        char b[3];
        for (unsigned int i = 0; i < length; i++)
        {
            sprintf(b, "%02X", input[i]);
            str.append(1, b[0]);
            str.append(1, b[1]);
        }
        return str;
    }


    /* Compile 64 bytes of hash data into SHA384 digest value   */
    /* NOTE: this routine assumes that the byte order in the    */
    /* _ctx.wbuf[] at this point is in such an order that low   */
    /* address bytes in the ORIGINAL byte stream placed in this */
    /* buffer will now go to the high end of words on BOTH big  */
    /* and little endian systems                                */
    void SHA384::Compile()
    {
        sha2_64t    v[8];
        sha2_32t    j;

        memcpy(v, _ctx.hash, 8 * sizeof(sha2_64t));

        for(j = 0; j < 80; j += 16)
        {
            h5_cycle( 0, j); h5_cycle( 1, j); h5_cycle( 2, j); h5_cycle( 3, j);
            h5_cycle( 4, j); h5_cycle( 5, j); h5_cycle( 6, j); h5_cycle( 7, j);
            h5_cycle( 8, j); h5_cycle( 9, j); h5_cycle(10, j); h5_cycle(11, j);
            h5_cycle(12, j); h5_cycle(13, j); h5_cycle(14, j); h5_cycle(15, j);
        }

        _ctx.hash[0] += v[0];
        _ctx.hash[1] += v[1];
        _ctx.hash[2] += v[2];
        _ctx.hash[3] += v[3];
        _ctx.hash[4] += v[4];
        _ctx.hash[5] += v[5];
        _ctx.hash[6] += v[6];
        _ctx.hash[7] += v[7];
    }


    /**
     * description
     */
    SHA512::SHA512()
    {
        Init();
    }


    /**
     * description
     */
    SHA512::~SHA512()
    {
    }


    void SHA512::Init()
    {
        _ctx.count[0] = _ctx.count[1] = 0;
        memcpy(_ctx.hash, i512, 8 * sizeof(sha2_64t));
    }


    /* SHA512 hash data in an array of bytes into hash buffer   */
    /* and call the hash_compile function as required.          */
    void SHA512::Update(const void* in, unsigned long len)
    {
        const unsigned char* data = (const unsigned char*)in;
        sha2_32t pos = (sha2_32t)(_ctx.count[0] & SHA512_MASK),
            space = SHA512_BLOCK_SIZE - pos;
        const unsigned char *sp = data;

        if((_ctx.count[0] += len) < len)
            ++(_ctx.count[1]);

        while(len >= space)     /* tranfer whole blocks while possible  */
        {
            memcpy(((unsigned char*)_ctx.wbuf) + pos, sp, space);
            sp += space; len -= space; space = SHA512_BLOCK_SIZE; pos = 0;
            bsw_64(_ctx.wbuf, SHA512_BLOCK_SIZE >> 3);
            Compile();
        }

        memcpy(((unsigned char*)_ctx.wbuf) + pos, sp, len);
    }


    void SHA512::Final()
    {
        sha2_32t    i = (sha2_32t)(_ctx.count[0] & SHA512_MASK);

        bsw_64(_ctx.wbuf, (i + 7) >> 3);

        /* bytes in the buffer are now in an order in which references  */
        /* to 64-bit words will put bytes with lower addresses into the */
        /* top of 64 bit words on BOTH big and little endian machines   */

        /* we now need to mask valid bytes and add the padding which is */
        /* a single 1 bit and as many zero bits as necessary.           */
        _ctx.wbuf[i >> 3] = (_ctx.wbuf[i >> 3] & m2[i & 7]) | b2[i & 7];

        /* we need 17 or more empty byte positions, one for the padding */
        /* byte (above) and sixteen for the length count.  If there is  */
        /* not enough space pad and empty the buffer                    */
        if(i > SHA512_BLOCK_SIZE - 17)
        {
            if(i < 120) _ctx.wbuf[15] = 0;
            Compile();
            i = 0;
        }
        else
            i = (i >> 3) + 1;

        while(i < 14)
            _ctx.wbuf[i++] = 0;

        /* the following 64-bit length fields are assembled in the      */
        /* wrong byte order on little endian machines but this is       */
        /* corrected later since they are only ever used as 64-bit      */
        /* word values.                                                 */

        _ctx.wbuf[14] = (_ctx.count[1] << 3) | (_ctx.count[0] >> 61);
        _ctx.wbuf[15] = _ctx.count[0] << 3;

        Compile();

        /* extract the hash value as bytes in case the hash buffer is   */
        /* misaligned for 32-bit words                                  */
        for(i = 0; i < SHA512_DIGEST_SIZE; ++i)
        {
            _digest[i] = (unsigned char)(_ctx.hash[i >> 3] >> 8 * (~i & 7));
        }
    }


    /**
     * Convert unsigned char array to hex string.
     */
    std::string SHA512::BytesToHexString(const unsigned char* input,
                                         unsigned int length)
    {
        std::string str;
        str.reserve(length << 1);
        char b[3];
        for (unsigned int i = 0; i < length; i++)
        {
            sprintf(b, "%02X", input[i]);
            str.append(1, b[0]);
            str.append(1, b[1]);
        }
        return str;
    }


    /* Compile 64 bytes of hash data into SHA512 digest value   */
    /* NOTE: this routine assumes that the byte order in the    */
    /* _ctx.wbuf[] at this point is in such an order that low   */
    /* address bytes in the ORIGINAL byte stream placed in this */
    /* buffer will now go to the high end of words on BOTH big  */
    /* and little endian systems                                */
    void SHA512::Compile()
    {
        sha2_64t    v[8];
        sha2_32t    j;

        memcpy(v, _ctx.hash, 8 * sizeof(sha2_64t));

        for(j = 0; j < 80; j += 16)
        {
            h5_cycle( 0, j); h5_cycle( 1, j); h5_cycle( 2, j); h5_cycle( 3, j);
            h5_cycle( 4, j); h5_cycle( 5, j); h5_cycle( 6, j); h5_cycle( 7, j);
            h5_cycle( 8, j); h5_cycle( 9, j); h5_cycle(10, j); h5_cycle(11, j);
            h5_cycle(12, j); h5_cycle(13, j); h5_cycle(14, j); h5_cycle(15, j);
        }

        _ctx.hash[0] += v[0];
        _ctx.hash[1] += v[1];
        _ctx.hash[2] += v[2];
        _ctx.hash[3] += v[3];
        _ctx.hash[4] += v[4];
        _ctx.hash[5] += v[5];
        _ctx.hash[6] += v[6];
        _ctx.hash[7] += v[7];
    }


    std::string SHA256Data(const void* data, unsigned int length)
    {
        SHA256 obj;
        obj.Update(data, length);
        obj.Final();
        return obj.ToString();
    }


    std::string SHA256String(const std::string& str)
    {
        SHA256 obj;
        obj.Update((const unsigned char*)str.c_str(),
                   static_cast<unsigned int>(str.length()));
        obj.Final();
        return obj.ToString();
    }


    std::string SHA256File(const std::string& file)
    {
        SHA256 obj;

        std::ifstream fs(file.c_str(), std::ios::binary);
        const unsigned int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        while (!fs.eof())
        {
            fs.read(buffer, BUFFER_SIZE);
            std::streamsize length = fs.gcount();
            if (length > 0)
            {
                obj.Update((const unsigned char*)buffer, length);
            }
        }
        fs.close();

        obj.Final();
        return obj.ToString();
    }


    std::string SHA384Data(const void* data, unsigned int length)
    {
        SHA384 obj;
        obj.Update(data, length);
        obj.Final();
        return obj.ToString();
    }


    std::string SHA384String(const std::string& str)
    {
        SHA384 obj;
        obj.Update((const unsigned char*)str.c_str(),
                   static_cast<unsigned int>(str.length()));
        obj.Final();
        return obj.ToString();
    }


    std::string SHA384File(const std::string& file)
    {
        SHA384 obj;

        std::ifstream fs(file.c_str(), std::ios::binary);
        const unsigned int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        while (!fs.eof())
        {
            fs.read(buffer, BUFFER_SIZE);
            std::streamsize length = fs.gcount();
            if (length > 0)
            {
                obj.Update((const unsigned char*)buffer, length);
            }
        }
        fs.close();

        obj.Final();
        return obj.ToString();
    }


    std::string SHA512Data(const void* data, unsigned int length)
    {
        SHA512 obj;
        obj.Update(data, length);
        obj.Final();
        return obj.ToString();
    }


    std::string SHA512String(const std::string& str)
    {
        SHA512 obj;
        obj.Update((const unsigned char*)str.c_str(),
                   static_cast<unsigned int>(str.length()));
        obj.Final();
        return obj.ToString();
    }


    std::string SHA512File(const std::string& file)
    {
        SHA512 obj;

        std::ifstream fs(file.c_str(), std::ios::binary);
        const unsigned int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        while (!fs.eof())
        {
            fs.read(buffer, BUFFER_SIZE);
            std::streamsize length = fs.gcount();
            if (length > 0)
            {
                obj.Update((const unsigned char*)buffer, length);
            }
        }
        fs.close();

        obj.Final();
        return obj.ToString();
    }


}

#pragma warning(pop)
