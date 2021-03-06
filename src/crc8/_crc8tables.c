/*
*********************************************************************************************************
*                              		(c) Copyright 2017-2020, Hexin
*                                           All Rights Reserved
* File    : _crc8tables.c
* Author  : Heyn (heyunhuan@gmail.com)
* Version : V1.3
*
* LICENSING TERMS:
* ---------------
*		New Create at 	2020-03-17 [Heyn] Initialize
*                       2020-03-20 [Heyn] New add hexin_calc_crc8_fletcher
*
*   SEE : http://reveng.sourceforge.net/crc-catalogue/1-15.htm#crc.cat-bits.8
*
*********************************************************************************************************
*/

#include "_crc8tables.h"

unsigned char hexin_reverse8( unsigned char data )
{
    unsigned int  i = 0;
    unsigned char t = 0;
    for ( i=0; i<8; i++ ) {
        t |= ( ( data >> i ) & 0x01 ) << ( 7-i );
    }
    return t;
}

unsigned int hexin_crc8_init_table_poly_is_high( unsigned char polynomial, unsigned char *table )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
            if ( (crc ^ c) & 0x01 )   crc = ( crc >> 1 ) ^ polynomial;
            else                      crc =   crc >> 1;
            c = c >> 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned int hexin_crc8_init_table_poly_is_low( unsigned char polynomial, unsigned char *table )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0, c = 0;

    for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
        crc = 0;
        c   = i;
        for ( j=0; j<8; j++ ) {
			if ( (crc ^ c) & 0x80 ) crc = ( crc << 1 ) ^ polynomial;
			else                    crc =   crc << 1;
			c = c << 1;
        }
        table[i] = crc;
    }
    return TRUE;
}

unsigned char hexin_crc8_poly_calc( unsigned char crc8, unsigned char c, const unsigned char *table )
{
    return table[ crc8 ^ c ];
}

unsigned char hexin_calc_crc8_bcc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc ^= pSrc[i];
    }

	return crc;
}

unsigned char hexin_calc_crc8_lrc( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc += pSrc[i];
	}
    crc = (~crc) + 0x01;

	return crc;
}

unsigned char hexin_calc_crc8_sum( const unsigned char *pSrc, unsigned int len, unsigned char crc8 ) 
{
    unsigned int i = 0;
    unsigned char crc = crc8;

	for ( i=0; i<len; i++ ) {
		crc += pSrc[i];
	}
	return crc;
}

unsigned char hexin_calc_crc8_fletcher( const unsigned char *pSrc, unsigned int len, unsigned char crc8 /*reserved*/ )
{
    unsigned int i = 0;
    unsigned char sum1 = 0, sum2 = 0;

    for ( i = 0; i < len; i++ ) {
        sum1 += pSrc[i];
        sum2 += sum1;
    }
    return ( sum1 & 0xF ) | ( sum2 << 4 );    
}

static unsigned int hexin_crc8_compute_init_table( struct _hexin_crc8 *param )
{
    unsigned int i = 0, j = 0;
    unsigned char crc = 0, c = 0;

    if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) {
        for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
            crc = 0;
            c   = i;
            for ( j=0; j<8; j++ ) {
                if ( (crc ^ c) & 0x01 )   crc = ( crc >> 1 ) ^ param->poly;
                else                      crc =   crc >> 1;
                c = c >> 1;
            }
            param->table[i] = crc;
        }
    } else {
        for ( i=0; i<MAX_TABLE_ARRAY; i++ ) {
            crc = 0;
            c   = i;
            for ( j=0; j<8; j++ ) {
                if ( (crc ^ c) & 0x80 ) crc = ( crc << 1 ) ^ param->poly;
                else                    crc =   crc << 1;
                c = c << 1;
            }
            param->table[i] = crc;
        }
    }
    return TRUE;
}

static unsigned char hexin_crc8_compute_char( unsigned char crc8, unsigned char c, struct _hexin_crc8 *param )
{
    return param->table[ crc8 ^ c ];
}

unsigned char hexin_crc8_compute( const unsigned char *pSrc, unsigned int len, struct _hexin_crc8 *param )
{
    unsigned int i = 0;
    unsigned char crc = param->init;

    if ( param->is_initial == FALSE ) {
        if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) {
            param->poly = hexin_reverse8( param->poly );
        }
        param->is_initial = hexin_crc8_compute_init_table( param );
    }

    if ( HEXIN_REFIN_REFOUT_IS_TRUE( param ) ) { 
        crc = hexin_reverse8( param->init );
    }

	for ( i=0; i<len; i++ ) {
		crc = hexin_crc8_compute_char( crc, pSrc[i], param );
	}
    
	return ( crc ^ param->xorout );
}
