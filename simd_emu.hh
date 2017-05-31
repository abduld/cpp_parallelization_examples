#ifndef __AVX__
union my256 { struct { __m128d d[2]; }; struct { __m128i i[2]; };
              my256(__m128d dhi, __m128d dlo) { d[0] = dlo; d[1] = dhi; }
              my256(__m128i ihi, __m128i ilo) { i[0] = ilo; i[1] = ihi; }
              my256() = delete;
              double operator[](unsigned n) const { return d[n/2][n%2]; }
            };
#define __m256d my256
#define __m256i my256
#define _mm256_dualop(x,y,func,c)      ([](const my256& a, const my256& b) { return my256(func(a.c[1],b.c[1]), func(a.c[0],b.c[0])); })(x,y)
#define _mm256_castpd_si256(x)         (x)
#define _mm256_castsi256_pd(x)         (x)
#define _mm256_castsi256_si128(x)      (x).i[0]
#define _mm256_castsi128_si256(x)      my256(_mm_setzero_si128(), x)
#define _mm256_extractf128_si256(x,n)  (x).i[n]
#define _mm256_and_pd(x,y)             _mm256_dualop(x,y,_mm_and_pd,d)
#define _mm256_or_pd(x,y)              _mm256_dualop(x,y,_mm_or_pd,d)
#define _mm256_add_pd(x,y)             _mm256_dualop(x,y,_mm_add_pd,d)
#define _mm256_sub_pd(x,y)             _mm256_dualop(x,y,_mm_sub_pd,d)
#define _mm256_mul_pd(x,y)             _mm256_dualop(x,y,_mm_mul_pd,d)
#define _mm256_div_pd(x,y)             _mm256_dualop(x,y,_mm_div_pd,d)
#define _mm256_andnot_pd(x,y)          _mm256_dualop(x,y,_mm_andnot_pd,d)
#define _mm256_cmp_pd_CMP_LT_OQ(x,y)   _mm256_dualop(x,y,_mm_cmplt_pd,d)
#define _mm256_cmp_pd_CMP_EQ_OQ(x,y)   _mm256_dualop(x,y,_mm_cmpeq_pd,d)
#define _mm256_cmp_pd(x,y,v)           _mm256_cmp_pd##v(x,y)
#define _mm256_insertf128_si256(x,y,n) ([](my256 xx,__m128i yy,int nn){xx.i[nn] = yy; return xx;}(x,y,n))
#define _mm256_set1_pd(x)              ([](__m128d v){ return my256(v,v); })(_mm_set1_pd(x))
#define _mm256_set1_epi32(x)           ([](__m128i v){ return my256(v,v); })(_mm_set1_epi32(x))
#define _mm256_set1_epi64x(x)          ([](__m128i v){ return my256(v,v); })(_mm_set1_epi64x(x))
#define _mm256_setzero_si256()         ([](__m128i v){ return my256(v,v); })(_mm_setzero_si128())
#define _mm256_set_pd(d,c,b,a)         my256(_mm_set_pd(d,c),_mm_set_pd(b,a))
#define _mm256_extract_epi64(x,n)      ([](const my256& x, unsigned nn) { return _mm_extract_epi64(x.i[nn/2], nn%2); })(x,n)
#define _mm256_testz_si256(x,y)        (_mm_testz_si128((x).i[0],(y).i[0]) && _mm_testz_si128((x).i[1],(y).i[1]))

//#define _mm256_cvtepi32_pd(x)          ([](__v4si v) { return _mm256_set_pd(v[3],v[2],v[1],v[0]); })((__v4si)(x))

#define _mm256_cvtepi32_pd(x)          ([](__m128i a) { return my256(_mm_cvtepi32_pd(_mm_srli_si128(a,8)), \
                                                                     _mm_cvtepi32_pd(a)); })(x)
#define _mm256_cvtepi32_epi64(x)       ([](__m128i a) { return my256(_mm_cvtepi32_epi64(_mm_srli_si128(a,8)), \
                                                                     _mm_cvtepi32_epi64(a)); })(x)
#endif

#ifndef __AVX2__
 #define _mm256_cmpeq_epi32(x,y) ([](const __m256i& a, const __m256i& b) { return compose256(_mm_cmpeq_epi32(extract128(a,0),extract128(b,0)),\
                                                                                             _mm_cmpeq_epi32(extract128(a,1),extract128(b,1))); })(x,y)
 #define _mm256_add_epi32(x,y) ([](const __m256i& a, const __m256i& b) { return compose256(_mm_add_epi32(extract128(a,0),extract128(b,0)),\
                                                                                           _mm_add_epi32(extract128(a,1),extract128(b,1))); })(x,y)
 #define _mm256_sub_epi32(x,y) ([](const __m256i& a, const __m256i& b) { return compose256(_mm_sub_epi32(extract128(a,0),extract128(b,0)),\
                                                                                           _mm_sub_epi32(extract128(a,1),extract128(b,1))); })(x,y)
#endif


#ifndef __AVX512F__
union my512 { struct { __m256d d[2]; }; struct { __m256i i[2]; };
              my512(__m256d dhi, __m256d dlo) { d[0] = dlo; d[1] = dhi; }
              #ifdef __AVX__
              my512(__m256i ihi, __m256i ilo) { i[0] = ilo; i[1] = ihi; }
              #endif
              my512() = delete;
              double operator[](unsigned n) const { return d[n/4][n%4]; }
            };
#define __m512d my512
#define __m512i my512
#define __mmask8 my512
#define _mm512_castpd_si512(x)         (x)
#define _mm512_castsi512_pd(x)         (x)
#define _mm512_castsi512_si256(x)      (x).i[0]
#define _mm512_castsi256_si512(x)      my512(_mm256_setzero_si256(), x)
#define _mm512_extractf256_si512(x,n)  (x).i[n]
#define _mm512_unaop(x,func,c)         ([](const my512& a)                                 { return my512(func(a.c[1]), func(a.c[0])); })(x)
#define _mm512_unaiop(x,y,func,c)      ([](const my512& a, int mm)                         { return my512(func(a.c[1],mm), func(a.c[0],mm)); })(x,y)
#define _mm512_dualop(x,y,func,c)      ([](const my512& a, const my512& b)                 { return my512(func(a.c[1],b.c[1]), func(a.c[0],b.c[0])); })(x,y)
#define _mm512_triop(x,y,z,func,c)     ([](const my512& a, const my512& b, const my512& d) { return my512(func(a.c[1],b.c[1],d.c[1]), func(a.c[0],b.c[0],d.c[0])); })(x,y,z)
#define _mm512_trilop(x,y,z,func,c)    ([](const my512& a, const my512& b)                 { return my512(func(a.c[1],b.c[1],z), func(a.c[0],b.c[0],z)); })(x,y)
#define _mm512_and_pd(x,y)             _mm512_dualop(x,y,_mm256_and_pd,d)
#define _mm512_or_pd(x,y)              _mm512_dualop(x,y,_mm256_or_pd,d)
#define _mm512_add_pd(x,y)             _mm512_dualop(x,y,_mm256_add_pd,d)
#define _mm512_sub_pd(x,y)             _mm512_dualop(x,y,_mm256_sub_pd,d)
#define _mm512_mul_pd(x,y)             _mm512_dualop(x,y,_mm256_mul_pd,d)
#define _mm512_div_pd(x,y)             _mm512_dualop(x,y,_mm256_div_pd,d)
#define _mm512_andnot_pd(x,y)          _mm512_dualop(x,y,_mm256_andnot_pd,d)
#define _mm512_andnot_si512(x,y)       _mm512_dualop(x,y,_mm256_andnot_si256,i)
#define _mm512_and_si512(x,y)          _mm512_dualop(x,y,_mm256_and_si256,i)
#define _mm512_add_epi64(x,y)          _mm512_dualop(x,y,_mm256_add_epi64,i)

#define _mm512_set1_pd(x)              ([](__m256d v){ return my512(v,v); })(_mm256_set1_pd(x))
#define _mm512_set1_epi64(x)           ([](__m256i v){ return my512(v,v); })(_mm256_set1_epi64x(x))

#define _mm512_set_pd(h,g,f,e,d,c,b,a)    my512(_mm256_set_pd(h,g,f,e),    _mm256_set_pd(d,c,b,a))
#define _mm512_set_epi64(h,g,f,e,d,c,b,a) my512(_mm256_set_epi64x(h,g,f,e),_mm256_set_epi64x(d,c,b,a))

#define _mm512_cmp_pd_MM_CMPINT_LT(x,y) ([](const my512& a, const my512& b) { return my512(_mm256_cmp_pd(a.d[1],b.d[1],_CMP_LT_OQ), _mm256_cmp_pd(a.d[0],b.d[0],_CMP_LT_OQ)); })(x,y)
#define _mm512_cmp_pd_MM_CMPINT_EQ(x,y) ([](const my512& a, const my512& b) { return my512(_mm256_cmp_pd(a.d[1],b.d[1],_CMP_EQ_OQ), _mm256_cmp_pd(a.d[0],b.d[0],_CMP_EQ_OQ)); })(x,y)
#define _mm512_cmp_pd_mask(x,y,v)       _mm512_cmp_pd##v(x,y)

#define _mm512_srli_epi64(x,m)         _mm512_unaiop(x,m, _mm256_srli_epi64,i)
#define _mm512_fmadd_pd(x,y,z)         _mm512_triop(x,y,z, _mm256_fmadd_pd, d)

#define _mm512_mask_mov_epi64(x,m,y)   _mm512_triop(x,y,m, _mm256_blendv_pd, d)

#define _mm512_cvtepi64_pd(x)          _mm512_unaop(x, _mm256_cvtepi64_pd, i)

#define _mm512_kand(a,b)               _mm512_and_pd(a,b)

#define _mm512_cvtepi32_pd(x)          ([](__m256i a) { return my512(_mm256_cvtepi32_pd(extract128(a,1)), \
                                                                     _mm256_cvtepi32_pd(extract128(a,0))); })(x)
#define _mm512_cvtepi32_epi64(x)       ([](__m256i a) { return my512(_mm256_cvtepi32_epi64(extract128(a,1)), \
                                                                     _mm256_cvtepi32_epi64(extract128(a,0))); })(x)

#define _mm256_cmp_epi32_MM_CMPINT_NE(x,y) (x) /* ASSUMPTIONS */
#define _mm256_cmp_epi32_mask(x,y,v)       _mm256_cmp_epi32##v(x,y)
#define _mm512_mask_mov_pd(x,m,y)          _mm512_triop(x,y,_mm512_cvtepi32_epi64(m), _mm256_blendv_pd, d)

/* ASSUMPTIONS  -- Convert m (__m512d that is really __m512i) into __m256i */
 #ifdef __AVX2__
  #define _mm256_mask_mov_epi32(a,m,b)   _mm256_insertf128_si256(_mm256_permute4x64_epi64(_mm256_shuffle_epi32(m.i[0],232),232), \
                                          _mm256_castsi256_si128(_mm256_permute4x64_epi64(_mm256_shuffle_epi32(m.i[1],232),232)),1) /* ASSUMPTIONS */
 #else
  #define _mm256_mask_mov_epi32(a,m,b)    compose256(_mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(extract128(m.i[0],0)),_mm_castsi128_ps(extract128(m.i[0],1)),136)), \
                                                     _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(extract128(m.i[1],0)),_mm_castsi128_ps(extract128(m.i[1],1)),136)))
 #endif
#endif

#ifndef __FMA__
 #define _mm256_fmadd_pd(a,b,c) _mm256_add_pd(_mm256_mul_pd(a,b), c)
#endif

#define compose512d(a,b)               _mm512_insertf64x4(_mm512_castpd256_pd512(a),b,1)
#define extract256(c,n)                (n ? _mm512_extracti64x4_epi64(c,1) : _mm512_castsi512_si256(c))

#define compose256(a,b)                _mm256_insertf128_si256(_mm256_castsi128_si256(a),b,1)
#define extract128(c,n)                (n ? _mm256_extractf128_si256(c, n) : _mm256_castsi256_si128(c))

#ifndef __AVX512VL__
 #ifdef __AVX512F__
  #define _mm256_mask_mov_epi32(a,b,c) _mm512_castsi512_si256(_mm512_mask_mov_epi32(_mm512_castsi256_si512(a),b,_mm512_castsi256_si512(c)))
  #define _mm256_cmp_epi32_mask(a,b,c) _mm512_cmp_epi32_mask(_mm512_castsi256_si512(a),_mm512_castsi256_si512(b),c)
 #endif
#endif
#ifndef __AVX512DQ__
 #ifdef __AVX2__
  #define _mm256_cvtepi64_pd(d) _mm256_cvtepi32_pd(_mm256_castsi256_si128(_mm256_permute4x64_epi64(_mm256_shuffle_epi32(d,232),232)))
 #else
  #define _mm256_cvtepi64_pd(d) ([](const __m256i& x){ return _mm256_set_pd(_mm256_extract_epi64(x,3),\
                                                                            _mm256_extract_epi64(x,2),\
                                                                            _mm256_extract_epi64(x,1),\
                                                                            _mm256_extract_epi64(x,0)); }(d))
 #endif
 #ifdef __AVX512F
  #define _mm512_or_pd(a,b)     _mm512_castsi512_pd(_mm512_or_si512(_mm512_castpd_si512(a), _mm512_castpd_si512(b)))
  #define _mm512_and_pd(a,b)    _mm512_castsi512_pd(_mm512_and_si512(_mm512_castpd_si512(a), _mm512_castpd_si512(b)))
  #define _mm512_andnot_pd(a,b) _mm512_castsi512_pd(_mm512_andnot_si512(_mm512_castpd_si512(a), _mm512_castpd_si512(b)))
  #define _mm512_cvtepi64_pd(a) ([](const __m512i& d){ return compose512d(_mm256_cvtepi64_pd(extract256(d,0)), \
                                                                          _mm256_cvtepi64_pd(extract256(d,1))); })(a)
 #endif
#endif


#ifndef __AVX2__
 #define _mm256_blendv_pd(nok,ok,cond) _mm256_or_pd(_mm256_and_pd(cond,ok), _mm256_andnot_pd(cond,nok))
 #ifdef __AVX__
  #define _mm256_cvtepi32_epi64(c)       compose256(_mm_cvtepi32_epi64(c), _mm_cvtepi32_epi64(_mm_shuffle_epi32(c,78)))
 #endif
 #define _mm256_add_epi64(a,b) compose256(_mm_add_epi64(extract128(a,0), extract128(b,0)), \
                                          _mm_add_epi64(extract128(a,1), extract128(b,1)))
 #define _mm256_and_si256(a,b)         _mm256_castpd_si256(_mm256_and_pd(_mm256_castsi256_pd(a), _mm256_castsi256_pd(b)))
 #define _mm256_andnot_si256(a,b)      _mm256_castpd_si256(_mm256_andnot_pd(_mm256_castsi256_pd(a), _mm256_castsi256_pd(b)))
 #define _mm256_shuffle_epi32(x,n)     _mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(x),_mm256_setzero_ps(), n))
 #define _mm256_srli_epi64(a,n) compose256(_mm_srli_epi64(extract128(a,0), n), \
                                           _mm_srli_epi64(extract128(a,1), n))
#endif

inline __m256d _mm256_log2_pd(__m256d x) /* log2(x) for four positive doubles */
{
    constexpr int mantissa_bits = 52, exponent_bias = 1022;
    //return _mm256_set_pd(std::log2(x[3]),std::log2(x[2]),std::log2(x[1]),std::log2(x[0]));

    __m256d half = _mm256_set1_pd(0.5);
    // x = frexp(x, &e);
    __m256i e = _mm256_srli_epi64(_mm256_castpd_si256(x), mantissa_bits);
    __m256i m = _mm256_and_si256(_mm256_castpd_si256(x), _mm256_set1_epi64x((1ull << mantissa_bits)-1));
    x = _mm256_or_pd(half, _mm256_castsi256_pd(m));

    __m256d ltid = _mm256_cmp_pd(x, _mm256_set1_pd(1/std::sqrt(2.)), _CMP_LT_OQ);
    __m256i lti = _mm256_castpd_si256(ltid);
    __m256d dbl_e = _mm256_sub_pd(_mm256_cvtepi64_pd(_mm256_add_epi64(e,lti)), _mm256_set1_pd(exponent_bias));

    __m256d z = _mm256_sub_pd(x, _mm256_add_pd(half, _mm256_andnot_pd(ltid, half)));
    __m256d y = _mm256_fmadd_pd(half, _mm256_sub_pd(x, _mm256_and_pd(ltid, half)), half);
    x = _mm256_div_pd(z, y);
    z = _mm256_mul_pd(x, x);
    __m256d u = _mm256_add_pd(z, _mm256_set1_pd(-3.56722798256324312549E1));
    __m256d t =                  _mm256_set1_pd(-7.89580278884799154124E-1);
    u = _mm256_fmadd_pd(u, z, _mm256_set1_pd(3.12093766372244180303E2));
    t = _mm256_fmadd_pd(t, z, _mm256_set1_pd(1.63866645699558079767E1));
    u = _mm256_fmadd_pd(u, z, _mm256_set1_pd(-7.69691943550460008604E2));
    t = _mm256_fmadd_pd(t, z, _mm256_set1_pd(-6.41409952958715622951E1));
    y = _mm256_fmadd_pd(z, _mm256_div_pd(t, u), _mm256_add_pd(half,half));
    // Multiply log of fraction by log2(e) and base 2 exponent by 1
    return _mm256_fmadd_pd(x, _mm256_mul_pd(y, _mm256_set1_pd(std::log2(std::exp(1.)))), dbl_e);
}

inline __m512d _mm512_log2_pd(__m512d x) /* log2(x) for eight positive doubles */
{
    constexpr int mantissa_bits = 52, exponent_bias = 1022;
    /*return _mm512_set_pd(std::log2(x[7]),std::log2(x[6]),std::log2(x[5]),std::log2(x[4]),
                         std::log2(x[3]),std::log2(x[2]),std::log2(x[1]),std::log2(x[0]));*/

    __m512d half = _mm512_set1_pd(0.5);
    // x = frexp(x, &e);
    __m512i e = _mm512_srli_epi64(_mm512_castpd_si512(x), mantissa_bits);
    __m512i m = _mm512_and_si512(_mm512_castpd_si512(x), _mm512_set1_epi64((1ull << mantissa_bits)-1));
    x = _mm512_or_pd(half, _mm512_castsi512_pd(m));

    __mmask8 lt = _mm512_cmp_pd_mask(x, _mm512_set1_pd(1/std::sqrt(2.)), _MM_CMPINT_LT);
    __m512d ltid = _mm512_castsi512_pd(_mm512_mask_mov_epi64(_mm512_set1_epi64(0), lt, _mm512_set1_epi64(-1)));
    __m512i lti = _mm512_castpd_si512(ltid);
    __m512d dbl_e = _mm512_sub_pd(_mm512_cvtepi64_pd(_mm512_add_epi64(e,lti)), _mm512_set1_pd(exponent_bias));

    __m512d z = _mm512_sub_pd(x, _mm512_add_pd(half, _mm512_andnot_pd(ltid, half)));
    __m512d y = _mm512_fmadd_pd(half, _mm512_sub_pd(x, _mm512_and_pd(ltid, half)), half);
    x = _mm512_div_pd(z, y);
    z = _mm512_mul_pd(x, x);
    __m512d u = _mm512_add_pd(z, _mm512_set1_pd(-3.56722798512324312549E1));
    __m512d t =                  _mm512_set1_pd(-7.89580278884799154124E-1);
    u = _mm512_fmadd_pd(u, z, _mm512_set1_pd(3.12093766372244180303E2));
    t = _mm512_fmadd_pd(t, z, _mm512_set1_pd(1.63866645699558079767E1));
    u = _mm512_fmadd_pd(u, z, _mm512_set1_pd(-7.69691943550460008604E2));
    t = _mm512_fmadd_pd(t, z, _mm512_set1_pd(-6.41409952958715622951E1));
    y = _mm512_fmadd_pd(z, _mm512_div_pd(t, u), _mm512_add_pd(half,half));
    // Multiply log of fraction by log2(e) and base 2 exponent by 1
    return _mm512_fmadd_pd(x, _mm512_mul_pd(y, _mm512_set1_pd(std::log2(std::exp(1.)))), dbl_e);
}
