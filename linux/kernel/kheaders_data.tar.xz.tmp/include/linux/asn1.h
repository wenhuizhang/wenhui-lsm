/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _LINUX_ASN1_H
#define _LINUX_ASN1_H


enum asn1_class {
	ASN1_UNIV	= 0,	
	ASN1_APPL	= 1,	
	ASN1_CONT	= 2,	
	ASN1_PRIV	= 3	
};
#define ASN1_CLASS_BITS	0xc0


enum asn1_method {
	ASN1_PRIM	= 0,	
	ASN1_CONS	= 1	
};
#define ASN1_CONS_BIT	0x20


enum asn1_tag {
	ASN1_EOC	= 0,	
	ASN1_BOOL	= 1,	
	ASN1_INT	= 2,	
	ASN1_BTS	= 3,	
	ASN1_OTS	= 4,	
	ASN1_NULL	= 5,	
	ASN1_OID	= 6,	
	ASN1_ODE	= 7,	
	ASN1_EXT	= 8,	
	ASN1_REAL	= 9,	
	ASN1_ENUM	= 10,	
	ASN1_EPDV	= 11,	
	ASN1_UTF8STR	= 12,	
	ASN1_RELOID	= 13,	
	
	
	ASN1_SEQ	= 16,	
	ASN1_SET	= 17,	
	ASN1_NUMSTR	= 18,	
	ASN1_PRNSTR	= 19,	
	ASN1_TEXSTR	= 20,	
	ASN1_VIDSTR	= 21,	
	ASN1_IA5STR	= 22,	
	ASN1_UNITIM	= 23,	
	ASN1_GENTIM	= 24,	
	ASN1_GRASTR	= 25,	
	ASN1_VISSTR	= 26,	
	ASN1_GENSTR	= 27,	
	ASN1_UNISTR	= 28,	
	ASN1_CHRSTR	= 29,	
	ASN1_BMPSTR	= 30,	
	ASN1_LONG_TAG	= 31	
};

#define ASN1_INDEFINITE_LENGTH 0x80

#endif 
