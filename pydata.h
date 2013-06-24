/********************** MODIFICATIONS to pydata.h **************************
** This is the list of modifications done to pydata.h
**
** Date       Who  What
**
** 2006/07/30 SE   New condition: Schwarzschacher
**
** 2007/01/28 SE   New condition: NormalPawn
**
** 2007/01/28 SE   New condition: Annan Chess
**
** 2007/04/30 NG   sentinelles variants: naming changed for consistency ...
**
** 2007/05/04 SE   Bugfix: SAT + BlackMustCapture
**
** 2007/05/04 SE   Bugfix: SAT + Ultraschachzwang
**
** 2007/06/01 SE   New piece: Radial knight (invented: C.J.Feather)
**
** 2007/11/08 SE   New conditions: Vaulting kings (invented: J.G.Ingram)
**                 Transmuting/Reflecting Ks now take optional piece list
**                 turning them into vaulting types
**
** 2007/12/20 SE   New condition: Lortap (invented: F.H. von Meyenfeldt)
**
** 2007/12/21 SE   Command-line switch: -maxtime (same func as Option)
**
** 2007/12/26 SE   New piece: Reverse Pawn (for below but independent)
**                 New condition: Protean Chess
**                 New piece type: Protean man (invent A.H.Kniest?)
**                 (Differs from Frankfurt chess in that royal riders
**                 are not 'non-passant'. Too hard to do but possibly
**                 implement as an independent condition later).
**
** 2008/01/02 NG   New condition: Geneva Chess
**
** 2008/01/11 SE   New variant: Special Grids
**
** 2008/01/13 SE   New conditions: White/Black Vaulting Kings
**
** 2008/01/24 SE   New variant: Gridlines
**
** 2008/02/10 SE   New condition: Cheameleon Pursuit (invented? : L.Grolman)
**
** 2008/02/19 SE   New condition: AntiKoeko
**
** 2008/02/19 SE   New piece: RoseLocust
**
** 2008/02/25 SE   New piece type: Magic
**
** 2008/03/13 SE   New condition: Castling Chess (invented: N.A.Bakke?)
**
** 2009/01/03 SE   New condition: Disparate Chess (invented: R.Bedoni)
**
** 2009/02/24 SE   New pieces: 2,0-Spiralknight
**                             4,0-Spiralknight
**                             1,1-Spiralknight
**                             3,3-Spiralknight
**                             Quintessence (invented Joerg Knappen)
**
** 2009/04/25 SE   New condition: Provacateurs
**                 New piece type: Patrol pieces
**
** 2009/06/14 SE   New option: LastCapture
**
** 2012/01/27 NG   AlphabeticChess now possible for white or black only.
**
**************************** End of List ******************************/

#if !defined(PYDATA_H)
#define PYDATA_H

#include "pyproc.h"

#include <limits.h>

#if !defined(EXTERN)
#       define EXTERN extern
#       define WE_ARE_EXTERN
#endif  /* EXTERN */


EXTERN unsigned long  MaxPositions;

EXTERN boolean LaTeXout;
EXTERN boolean flag_regression;

EXTERN char versionString[100];

EXTERN square royal_square[nr_sides];

EXTERN  square          ppridia[maxply + 1];

EXTERN square prev_king_square[nr_sides][maxply+1];

EXTERN  square          pattfld, patti, pattj;
EXTERN move_generation_elmt move_generation_stack[toppile + 1];

EXTERN  ply             nbply;

extern numvec const * const * const CheckDir;

EXTERN unsigned int number_of_pieces[nr_sides][PieceCount];

EXTERN  boolean			rex_mad,rex_circe,
                        rex_mess_ex, rex_wooz_ex;
EXTERN  boolean         pwcprom[maxply + 1];

EXTERN  pilecase current_anticirce_rebirth_square;
EXTERN  numecoup current_move[maxply + 1];

EXTERN  ply      parent_ply[maxply + 1];

EXTERN  boolean platzwechsel_rochade_allowed[nr_sides][maxply+1];

EXTERN  boolean  exist[PieceCount];
EXTERN  boolean  may_exist[PieceCount];
EXTERN  boolean  promonly[PieceCount];
EXTERN  boolean  is_football_substitute[PieceCount];

typedef square  (* renaifunc)(PieNam, Flags, square, square, square, Side);

EXTERN  renaifunc circerenai, antirenai, marsrenai;

EXTERN  boolean         anycirce, anycirprom, anyanticirce, anyanticirprom, anyimmun, anyclone, anygeneva;

/* new anticirce implementation */
EXTERN  boolean         AntiCirCheylan;
EXTERN  enum
{
  singlebox_type1,
  singlebox_type2,
  singlebox_type3
} SingleBoxType;

EXTERN  enum
{
  republican_type1,
  republican_type2
} RepublicanType;

EXTERN  pileside     trait;
EXTERN  boolean         flagfee,
            change_moving_piece,
            supergenre ;

EXTERN  int        zzzaj[square_h8 - square_a1 + 1];  /* gridnumber */

EXTERN  boolean         anymars;
EXTERN  boolean         is_phantomchess;

EXTERN  PieNam          next_football_substitute[PieceCount];       /* it's a inittable ! */

EXTERN  boolean         football_are_substitutes_limited;
EXTERN  PieNam          checkpieces[PieceCount - Leo + 1]; /* only fairies ! */

EXTERN  PieNam          orphanpieces[PieceCount];

EXTERN boolean          IsardamB;

EXTERN boolean          checkhopim;

EXTERN boolean		anyantimars;
EXTERN square           supertrans[maxply+1];
EXTERN PieNam           current_trans_gen;
EXTERN boolean          complex_castling_through_flag;
EXTERN boolean          dont_generate_castling;
EXTERN boolean          flag_synchron;
EXTERN ply      	      tempply;
EXTERN boolean          rex_protean_ex;
EXTERN int              gridvar, currentgridnum;
EXTERN boolean          calc_reflective_king[nr_sides];
EXTERN int         gridlines[112][4];
EXTERN int              numgridlines;
EXTERN boolean			anyparrain;

#if defined(WE_ARE_EXTERN)
	extern PieTable PieNamString[LanguageCount];
#else
	PieTable PieNamString[LanguageCount] = {
	{ /* French PieNamString */
	/*  0*/ {'.',' '},  /* vide */
	/*  1*/ {' ',' '},  /* hors echiquier */
	/*  2*/ {'r',' '},  /* roi */
	/*  3*/ {'p',' '},  /* pion */
	/*  4*/ {'d',' '},  /* dame */
	/*  5*/ {'c',' '},  /* cavalier */
	/*  6*/ {'t',' '},  /* tour */
	/*  7*/ {'f',' '},  /* fou */
	/*  8*/ {'l','e'},  /* leo */
	/*  9*/ {'m','a'},  /* mao */
	/* 10*/ {'p','a'},  /* pao */
	/* 11*/ {'v','a'},  /* vao */
	/* 12*/ {'r','o'},  /* rose */
	/* 13*/ {'s',' '},  /* sauterelle */
	/* 14*/ {'n',' '},  /* noctambule */
	/* 15*/ {'z',' '},  /* zebre */
	/* 16*/ {'c','h'},  /* chameau */
	/* 17*/ {'g','i'},  /* girafe */
	/* 18*/ {'c','c'},  /* cavalier racine carree cinquante */
	/* 19*/ {'b','u'},  /* bucephale  (cheval d'Alexandre le Grand) */
	/* 20*/ {'v','i'},  /* vizir */
	/* 21*/ {'a','l'},  /* alfil */
	/* 22*/ {'f','e'},  /* fers */
	/* 23*/ {'d','a'},  /* dabbabba */
	/* 24*/ {'l','i'},  /* lion */
	/* 25*/ {'e','q'},  /* equisauteur (non-stop) */
	/* 26*/ {'l',' '},  /* locuste */
	/* 27*/ {'p','b'},  /* pion berolina */
	/* 28*/ {'a','m'},  /* amazone */
	/* 29*/ {'i','m'},  /* imperatrice */
	/* 30*/ {'p','r'},  /* princesse */
	/* 31*/ {'g',' '},  /* gnou */
	/* 32*/ {'a','n'},  /* antilope */
	/* 33*/ {'e','c'},  /* ecureuil */
	/* 34*/ {'v',' '},  /* varan */
	/* 35*/ {'d','r'},  /* dragon */
	/* 36*/ {'k','a'},  /* kangourou */
	/* 37*/ {'c','s'},  /* cavalier spirale */
	/* 38*/ {'u','u'},  /* UbiUbi */
	/* 39*/ {'h','a'},  /* hamster: sauterelle a 180 degre */
	/* 40*/ {'e',' '},  /* elan: sauterelle a 45 degre */
	/* 41*/ {'a','i'},  /* aigle: sauterelle a 90 degre */
	/* 42*/ {'m',' '},  /* moineaux: sauterelle a 135 degre */
	/* 43*/ {'a','r'},  /* archeveque */
	/* 44*/ {'f','r'},  /* fou rebondissant */
	/* 45*/ {'c','a'},  /* cardinal */
	/* 46*/ {'s','n'},  /* noctambule sauteur */
	/* 47*/ {'p','i'},  /* piece impuissant */
	/* 48*/ {'c','n'},  /* chameau noctambule */
	/* 49*/ {'z','n'},  /* zebre noctambule */
	/* 50*/ {'g','n'},  /* gnou noctambule */
	/* 51*/ {'s','c'},  /* chameau sauteur */
	/* 52*/ {'s','z'},  /* zebre sauteur */
	/* 53*/ {'s','g'},  /* gnou sauteur */
	/* 54*/ {'c','d'},  /* cavalier spirale diagonale */
	/* 55*/ {'c','r'},  /* cavalier rebondissant */
	/* 56*/ {'e','a'},  /* equisauteur anglais */
	/* 57*/ {'c','t'},  /* CAT (= cavalier de troie) */
	/* 58*/ {'b','s'},  /* berolina superpion */
	/* 59*/ {'s','p'},  /* superpion */
	/* 60*/ {'t','l'},  /* tour-lion */
	/* 61*/ {'f','l'},  /* fou-lion */
	/* 62*/ {'s','i'},  /* sirene */
	/* 63*/ {'t','r'},  /* triton */
	/* 64*/ {'n','e'},  /* nereide */
	/* 65*/ {'o',' '},  /* orphan */
	/* 66*/ {'e','h'},  /* "edgehog", "randschwein" */
	/* 67*/ {'m','o'},  /* moa */
	/* 68*/ {'t','c'},  /* tour/fou chasseur */
	/* 69*/ {'f','c'},  /* fou/tour chasseur */
	/* 70*/ {'a','o'},  /* noctambule mao */
	/* 71*/ {'o','a'},  /* noctambule moa */
	/* 72*/ {'s','t'},  /* sauterelle tour */
	/* 73*/ {'s','f'},  /* sauterelle fou */
	/* 74*/ {'r','e'},  /* roi des elfes */
	/* 75*/ {'b','t'},  /* boy-scout */
	/* 76*/ {'g','t'},  /* girl-scout */
	/* 77*/ {'s','k'},  /* skylla */
	/* 78*/ {'c','y'},  /* charybdis */
	/* 79*/ {'s','a'},  /* sauterelle contA */
	/* 80*/ {'r','l'},  /* rose lion */
	/* 81*/ {'r','s'},  /* rose sauteur */
	/* 82*/ {'o','k'},  /* okapi */
	/* 83*/ {'3','7'},  /* 3:7-cavalier */
	/* 84*/ {'s','2'},  /* sauterelle-2 */
	/* 85*/ {'s','3'},  /* sauterelle-3 */
	/* 86*/ {'1','6'},  /* 1:6-cavalier */
	/* 87*/ {'2','4'},  /* 2:4-cavalier */
	/* 88*/ {'3','5'},  /* 3:5-cavalier */
	/* 89*/ {'d','s'},  /* double sauterelle */
	/* 90*/ {'s','r'},  /* roi sauteur */
	/* 91*/ {'o','r'},  /* orix */
	/* 92*/ {'1','5'},  /* 1:5-cavalier */
	/* 93*/ {'2','5'},  /* 2:5-cavalier */
	/* 94*/ {'g','l'},  /* gral */
    /* 95*/ {'l','t'},  /* tour locuste */
    /* 96*/ {'l','f'},  /* fou locuste */
    /* 97*/ {'l','n'},  /* noctambule locuste */
    /* 98*/ {'v','s'},  /* vizir sauteur */
    /* 99*/ {'f','s'},  /* fers sauteur */
	/*100*/ {'b','i'},  /* bison */
	/*101*/ {'e','l'},  /* elephant */
	/*102*/ {'n','a'},  /* Nao */
	/*103*/ {'e','t'},  /* elan tour: sauterelle tour a 45 degre */
	/*104*/ {'a','t'},  /* aigle tour: sauterelle tour a 90 degre */
	/*105*/ {'m','t'},  /* moineaux tour: sauterelle tour a 135 degre */
	/*106*/ {'e','f'},  /* elan fou: sauterelle fou a 45 degre */
	/*107*/ {'a','f'},  /* aigle fou: sauterelle fou a 90 degre */
	/*108*/ {'m','f'},  /* moineaux fou: sauterelle fou a 135 degre */
	/*109*/ {'r','a'},  /* rao: rose chinois */
	/*110*/ {'s','o'},  /* scorpion: roi des elfes et sauterelle */
	/*111*/ {'m','g'},  /* marguerite */
	/*112*/ {'3','6'},  /* 3:6-cavalier */
    /*113*/ {'n','l'},  /* noctambule lion */
    /*114*/ {'m','l'},  /* noctambule mao lion */
    /*115*/ {'m','m'},  /* noctambule moa lion */
    /*116*/ {'a',' '},  /* ami */
    /*117*/ {'d','n'},  /* dauphin */
    /*118*/ {'l','a'},  /* lapin */
    /*119*/ {'b','o'},  /* bob */
    /*120*/ {'q','a'},  /* equi anglais */
    /*121*/ {'q','f'},  /* equi francais */
    /*122*/ {'q','q'},  /* querquisite */
    /*123*/ {'b','1'},  /* bouncer */
    /*124*/ {'b','2'},  /* tour-bouncer */
    /*125*/ {'b','3'},  /* fou-bouncer */
    /*126*/ {'p','c'},  /* pion chinois */
    /*127*/ {'c','l'},  /* cavalier radial */
    /*128*/ {'p','v'},  /* pion renverse */
    /*129*/ {'l','r'},  /*rose locuste */
    /*130*/ {'z','e'},  /*zebu */
    /*131*/ {'n','r'},  /*noctambule rebondissant */
    /*132*/ {'n','2'},  /*spiralspringer (2,0) */
    /*133*/ {'n','4'},  /*spiralspringer (4,0) */
    /*134*/ {'n','1'},  /*diagonalspiralspringer (1,1) */
    /*135*/ {'n','3'},  /*diagonalspiralspringer (3,3) */
    /*136*/ {'q','n'},  /*quintessence */
    /*137*/ {'d','t'},  /* double tour-sauterelle */
    /*138*/ {'d','f'},  /* double fou-sauterelle */
    /*139*/ {'n','o'},   /* orix (non-stop) */
    /*140*/ {'s','b'},   /* treehopper */
    /*141*/ {'s','e'},   /* leafhopper */
    /*142*/ {'s','m'},   /* greater treehopper */
    /*143*/ {'s','u'},   /* greater leafhopper */
    /*144*/ {'k','l'},   /* kangarou lion */
    /*145*/ {'k','o'},   /* kao */
    /*146*/ {'s','v'},   /* knighthopper */
    /*147*/ {'b','a'},   /* bateau */
    /*148*/ {'g','y'},   /* gryphon */
    /*149*/ {'c','m'},   /* cavalier marin */
    /*150*/ {'p','o'},   /* pos\'eidon */
    /*151*/ {'p','m'},   /* pion marin */
    /*152*/ {'b','m'}    /* bateau marin */
	},{ /* German PieNamString */
	/*  0*/ {'.',' '},  /* leer */
	/*  1*/ {' ',' '},  /* ausserhalb des Brettes */
	/*  2*/ {'k',' '},  /* Koenig     0,1 + 1,1 S */
	/*  3*/ {'b',' '},  /* Bauer */
	/*  4*/ {'d',' '},  /* Dame       0,1 + 1,1 R */
	/*  5*/ {'s',' '},  /* Springer   1,2 S */
	/*  6*/ {'t',' '},  /* Turm       0,1 R */
	/*  7*/ {'l',' '},  /* Laeufer    1,1 R */
	/*  8*/ {'l','e'},  /* Leo */
	/*  9*/ {'m','a'},  /* Mao */
	/* 10*/ {'p','a'},  /* Pao */
	/* 11*/ {'v','a'},  /* Vao */
	/* 12*/ {'r','o'},  /* Rose */
	/* 13*/ {'g',' '},  /* Grashuepfer 0,1 + 1,1 H */
	/* 14*/ {'n',' '},  /* Nachtreiter 1,2 R */
	/* 15*/ {'z',' '},  /* Zebra       2,3 S */
	/* 16*/ {'c','a'},  /* Kamel       1,3 S */
	/* 17*/ {'g','i'},  /* Giraffe     1,4 S */
	/* 18*/ {'w','f'},  /* Wurzel-50-Springer 5,5 + 1,7 S */
	/* 19*/ {'b','u'},  /* Wurzel-25-Springer 3,4 + 5,0 S  (Bukephalos: Pferd Alexanders des Grossen) */
	/* 20*/ {'w','e'},  /* Wesir */
	/* 21*/ {'a','l'},  /* Alfil */
	/* 22*/ {'f','e'},  /* Fers */
	/* 23*/ {'d','a'},  /* Dabbaba */
	/* 24*/ {'l','i'},  /* Lion */
	/* 25*/ {'n','e'},  /* Non-stop Equihopper */
	/* 26*/ {'h',' '},  /* Heuschrecke */
	/* 27*/ {'b','b'},  /* Berolina Bauer */
	/* 28*/ {'a','m'},  /* Amazone    (Dame + Springer) */
	/* 29*/ {'k','n'},  /* Kaiserin   (Turm + Springer) */
	/* 30*/ {'p','r'},  /* Prinzessin (Laeufer + Springer) */
	/* 31*/ {'g','n'},  /* Gnu        (Springer + Kamel) */
	/* 32*/ {'a','n'},  /* Antilope    3,4 S */
	/* 33*/ {'e','i'},  /* Eichhoernchen      (Springer + Alfil + Dabbabba) */
	/* 34*/ {'w','a'},  /* Waran      (Turm + Nachtreiter) */
	/* 35*/ {'d','r'},  /* Drache     (Springer + Bauer) */
	/* 36*/ {'k','a'},  /* Kaenguruh */
	/* 37*/ {'s','s'},  /* Spiralspringer */
	/* 38*/ {'u','u'},  /* UbiUbi */
	/* 39*/ {'h','a'},  /* Hamster: 180 Grad Grashuepfer */
	/* 40*/ {'e','l'},  /* Elch: 45 Grad Grashuepfer */
	/* 41*/ {'a','d'},  /* Adler: 90 Grad Grashuepfer */
	/* 42*/ {'s','p'},  /* Spatz: 135 Grad Grashuepfer */
	/* 43*/ {'e','r'},  /* Erzbischof */
	/* 44*/ {'r','l'},  /* Reflektierender Laeufer */
	/* 45*/ {'k','l'},  /* Kardinal */
	/* 46*/ {'n','h'},  /* Nachtreiterhuepfer */
	/* 47*/ {'d','u'},  /* Dummy */
	/* 48*/ {'c','r'},  /* Kamelreiter */
	/* 49*/ {'z','r'},  /* Zebrareiter */
	/* 50*/ {'g','r'},  /* Gnureiter */
	/* 51*/ {'c','h'},  /* Kamelreiterhuepfer */
	/* 52*/ {'z','h'},  /* Zebrareiterhuepfer */
	/* 53*/ {'g','h'},  /* Gnureiterhuepfer */
	/* 54*/ {'d','s'},  /* DiagonalSpiralSpringer */
	/* 55*/ {'r','s'},  /* Reflektierender Springer */
	/* 56*/ {'e','q'},  /* Equihopper */
	/* 57*/ {'c','t'},  /* CAT (= cavalier de troie) */
	/* 58*/ {'b','s'},  /* Berolina Superbauer */
	/* 59*/ {'s','b'},  /* Superbauer */
	/* 60*/ {'t','l'},  /* Turm-Lion */
	/* 61*/ {'l','l'},  /* Laeufer-Lion */
	/* 62*/ {'s','i'},  /* Sirene */
	/* 63*/ {'t','r'},  /* Triton */
	/* 64*/ {'n','d'},  /* Nereide */
	/* 65*/ {'o',' '},  /* Orphan */
	/* 66*/ {'r','d'},  /* Randschwein */
	/* 67*/ {'m','o'},  /* Moa */
	/* 68*/ {'t','j'},  /* Turm/Laeufer-Jaeger */
	/* 69*/ {'l','j'},  /* Laeufer/Turm-Jaeger */
	/* 70*/ {'a','o'},  /* Maoreiter */
	/* 71*/ {'o','a'},  /* Moareiter */
	/* 72*/ {'t','h'},  /* Turmhuepfer */
	/* 73*/ {'l','h'},  /* Laeuferhuepfer */
	/* 74*/ {'e','k'},  /* Erlkoenig */
	/* 75*/ {'p','f'},  /* Pfadfinder */
	/* 76*/ {'p','n'},  /* Pfadfinderin */
	/* 77*/ {'s','k'},  /* Skylla */
	/* 78*/ {'c','y'},  /* Charybdis */
	/* 79*/ {'k','g'},  /* KontraGrashuepfer */
	/* 80*/ {'r','n'},  /* RosenLion */
	/* 81*/ {'r','h'},  /* RosenHuepfer */
	/* 82*/ {'o','k'},  /* Okapi */
	/* 83*/ {'3','7'},  /* 3:7-Springer */
	/* 84*/ {'g','2'},  /* Grashuepfer-2 */
	/* 85*/ {'g','3'},  /* Grashuepfer-3 */
	/* 86*/ {'1','6'},  /* 1:6-Springer */
	/* 87*/ {'2','4'},  /* 2:4-Springer */
	/* 88*/ {'3','5'},  /* 3:5-Springer */
	/* 89*/ {'d','g'},  /* DoppelGrashuepfer */
	/* 90*/ {'k','h'},  /* Koenighuepfer */
	/* 91*/ {'o','r'},  /* Orix */
	/* 92*/ {'1','5'},  /* 1:5-Springer */
	/* 93*/ {'2','5'},  /* 2:5-Springer */
	/* 94*/ {'g','l'},  /* Gral */
    /* 95*/ {'h','t'},  /* Turmheuschrecke */
    /* 96*/ {'h','l'},  /* Laeuferheuschrecke */
    /* 97*/ {'h','n'},  /* Nachtreiterheuschrecke */
    /* 98*/ {'w','r'},  /* Wesirreiter */
    /* 99*/ {'f','r'},  /* Fersreiter */
	/*100*/ {'b','i'},  /* Bison */
	/*101*/ {'e','t'},  /* Elefant */
	/*102*/ {'n','a'},  /* Nao */
	/*103*/ {'t','c'},  /* Turmelch: 45 Grad Grashuepfer */
	/*104*/ {'t','a'},  /* Turmadler: 90 Grad Grashuepfer */
	/*105*/ {'t','s'},  /* Turmspatz: 135 Grad Grashuepfer */
	/*106*/ {'l','c'},  /* Laeuferelch: 45 Grad Grashuepfer */
	/*107*/ {'l','a'},  /* Laeuferadler: 90 Grad Grashuepfer */
	/*108*/ {'l','s'},  /* Laeuferspatz: 135 Grad Grashuepfer */
	/*109*/ {'r','a'},  /* Rao: Chinesische Rose */
	/*110*/ {'s','o'},  /* Skorpion: Erlkoenig + Grashuepfer */
	/*111*/ {'m','g'},  /* Marguerite */
	/*112*/ {'3','6'},  /* 3:6 Springer */
    /*113*/ {'n','l'},  /* Nachtreiterlion */
    /*114*/ {'m','l'},  /* Maoreiterlion */
    /*115*/ {'m','m'},  /* Moareiterlion */
    /*116*/ {'f',' '},  /* Freund */
    /*117*/ {'d','e'},  /* Delphin */
    /*118*/ {'h','e'},  /* Hase: Lion-Huepfer ueber 2 Boecke */
    /*119*/ {'b','o'},  /* Bob: Lion-Huepfer ueber 4 Boecke */
    /*120*/ {'q','e'},  /* EquiEnglisch */
    /*121*/ {'q','f'},  /* EquiFranzoesisch */
    /*122*/ {'o','d'},  /* Odysseus */
    /*123*/ {'b','1'},  /* Bouncer */
    /*124*/ {'b','2'},  /* Turm-bouncer */
    /*125*/ {'b','3'},  /* Laeufer-bouncer */
    /*126*/ {'c','b'},  /* Chinesischer Bauer */
    /*127*/ {'r','p'},  /* Radialspringer */
    /*128*/ {'r','b'},  /* ReversBauer */
    /*129*/ {'l','r'},  /* RosenHeuschrecke */
    /*130*/ {'z','e'},  /* Zebu */
    /*131*/ {'n','r'},  /* Reflektierender Nachreiter*/
    /*132*/ {'s','2'},  /* spiralspringer (2,0) */
    /*133*/ {'s','4'},  /* spiralspringer (4,0) */
    /*134*/ {'s','1'},  /* diagonalspiralspringer (1,1) */
    /*135*/ {'s','3'},  /* diagonalspiralspringer (3,3) */
    /*136*/ {'q','n'},   /* quintessence */
    /*137*/ {'d','t'},  /* Doppelturmhuepfer */
    /*138*/ {'d','l'},  /* Doppellaeuferhuepfer */
    /*139*/ {'n','o'},   /* orix (non-stop) */
    /*140*/ {'u','h'},   /* treehopper */
    /*141*/ {'b','h'},   /* leafhopper */
    /*142*/ {'g','u'},   /* greater treehopper */
    /*143*/ {'g','b'},  /* greater leafhopper */
    /*144*/ {'l','k'},   /* kangarou lion */
    /*145*/ {'k','o'},   /* kao */
    /*146*/ {'s','h'},   /* springerhuepfer */
    /*147*/ {'s','c'},   /* schiff */
    /*148*/ {'g','y'},   /* gryphon */
    /*149*/ {'m','s'},   /* mariner Springer */
    /*150*/ {'p','o'},   /* Poseidon */
    /*151*/ {'m','b'},   /* mariner Bauer */
    /*152*/ {'m','c'}    /* marines Schiff */
	},{/* English PieNamString */
	/*  0*/ {'.',' '},  /* empty */
	/*  1*/ {' ',' '},  /* outside board */
	/*  2*/ {'k',' '},  /* king */
	/*  3*/ {'p',' '},  /* pawn */
	/*  4*/ {'q',' '},  /* queen */
	/*  5*/ {'s',' '},  /* knight */
	/*  6*/ {'r',' '},  /* rook */
	/*  7*/ {'b',' '},  /* bishop */
	/*  8*/ {'l','e'},  /* leo */
	/*  9*/ {'m','a'},  /* mao */
	/* 10*/ {'p','a'},  /* pao */
	/* 11*/ {'v','a'},  /* vao */
	/* 12*/ {'r','o'},  /* rose */
	/* 13*/ {'g',' '},  /* grashopper */
	/* 14*/ {'n',' '},  /* nightrider */
	/* 15*/ {'z',' '},  /* zebra */
	/* 16*/ {'c','a'},  /* camel */
	/* 17*/ {'g','i'},  /* giraffe */
	/* 18*/ {'r','f'},  /* root-50-leaper */
	/* 19*/ {'b','u'},  /* root-25-leaper  (bucephale: horse of Alexander the Great) */
	/* 20*/ {'w','e'},  /* wazir */
	/* 21*/ {'a','l'},  /* alfil */
	/* 22*/ {'f','e'},  /* fers */
	/* 23*/ {'d','a'},  /* dabbabba */
	/* 24*/ {'l','i'},  /* lion */
	/* 25*/ {'n','e'},  /* nonstop equihopper */
	/* 26*/ {'l',' '},  /* locust */
	/* 27*/ {'b','p'},  /* berolina pawn */
	/* 28*/ {'a','m'},  /* amazon */
	/* 29*/ {'e','m'},  /* empress */
	/* 30*/ {'p','r'},  /* princess */
	/* 31*/ {'g','n'},  /* gnu */
	/* 32*/ {'a','n'},  /* antelope */
	/* 33*/ {'s','q'},  /* squirrel */
	/* 34*/ {'w','a'},  /* waran */
	/* 35*/ {'d','r'},  /* dragon */
	/* 36*/ {'k','a'},  /* kangaroo */
	/* 37*/ {'s','s'},  /* Spiralspringer */
	/* 38*/ {'u','u'},  /* UbiUbi */
	/* 39*/ {'h','a'},  /* hamster: 180 degree grashopper */
	/* 40*/ {'m',' '},  /* moose: 45 degree grashopper */
	/* 41*/ {'e','a'},  /* eagle: 90 degree grashopper */
	/* 42*/ {'s','w'},  /* sparrow: 135 degree grashopper */
	/* 43*/ {'a','r'},  /* archbishop */
	/* 44*/ {'r','b'},  /* reflecting bishop */
	/* 45*/ {'c',' '},  /* cardinal */
	/* 46*/ {'n','h'},  /* nightrider-hopper */
	/* 47*/ {'d','u'},  /* dummy */
	/* 48*/ {'c','r'},  /* camelrider */
	/* 49*/ {'z','r'},  /* zebrarider */
	/* 50*/ {'g','r'},  /* gnurider */
	/* 51*/ {'c','h'},  /* camelrider hopper */
	/* 52*/ {'z','h'},  /* zebrarider hopper */
	/* 53*/ {'g','h'},  /* gnurider hopper */
	/* 54*/ {'d','s'},  /* DiagonalSpiralSpringer */
	/* 55*/ {'b','k'},  /* bouncy knight */
	/* 56*/ {'e','q'},  /* equihoppper */
	/* 57*/ {'c','t'},  /* CAT  ( cavalier de troie) */
	/* 58*/ {'b','s'},  /* berolina superpawn */
	/* 59*/ {'s','p'},  /* superpawn */
	/* 60*/ {'r','l'},  /* rook-lion */
	/* 61*/ {'b','l'},  /* bishop-lion */
	/* 62*/ {'s','i'},  /* sirene */
	/* 63*/ {'t','r'},  /* triton */
	/* 64*/ {'n','d'},  /* nereide */
	/* 65*/ {'o',' '},  /* orphan */
	/* 66*/ {'e','h'},  /* edgehog */
	/* 67*/ {'m','o'},  /* moa */
	/* 68*/ {'r','r'},  /* rook/bishop-hunter */
	/* 69*/ {'b','r'},  /* bishop/rook-hunter */
	/* 70*/ {'a','o'},  /* maorider */
	/* 71*/ {'o','a'},  /* moarider */
	/* 72*/ {'r','h'},  /* rookhopper */
	/* 73*/ {'b','h'},  /* bishophopper */
	/* 74*/ {'e','k'},  /* erlking */
	/* 75*/ {'b','t'},  /* boyscout */
	/* 76*/ {'g','t'},  /* girlscout */
	/* 77*/ {'s','k'},  /* Skylla */
	/* 78*/ {'c','y'},  /* Charybdis */
	/* 79*/ {'c','g'},  /* contra grashopper */
	/* 80*/ {'r','n'},  /* RoseLion */
	/* 81*/ {'r','p'},  /* RoseHopper */
	/* 82*/ {'o','k'},  /* Okapi */
	/* 83*/ {'3','7'},  /* 3:7-leaper */
	/* 84*/ {'g','2'},  /* grasshopper-2 */
	/* 85*/ {'g','3'},  /* grasshopper-3 */
	/* 86*/ {'1','6'},  /* 1:6-leaper */
	/* 87*/ {'2','4'},  /* 2:4-leaper */
	/* 88*/ {'3','5'},  /* 3:5-leaper */
	/* 89*/ {'d','g'},  /* Double Grasshopper */
	/* 90*/ {'k','h'},  /* Kinghopper */
	/* 91*/ {'o','r'},  /* Orix */
	/* 92*/ {'1','5'},  /* 1:5-leaper */
	/* 93*/ {'2','5'},  /* 2:5-leaper */
	/* 94*/ {'g','l'},  /* Gral */
    /* 95*/ {'l','r'},  /* rook locust */
    /* 96*/ {'l','b'},  /* bishop locust */
    /* 97*/ {'l','n'},  /* nightrider locust */
    /* 98*/ {'w','r'},  /* wazirrider */
    /* 99*/ {'f','r'},  /* fersrider */
	/*100*/ {'b','i'},  /* bison */
	/*101*/ {'e','t'},  /* elephant */
	/*102*/ {'n','a'},  /* Nao */
	/*103*/ {'r','m'},  /* rook moose: 45 degree grashopper */
	/*104*/ {'r','e'},  /* rook eagle: 90 degree grashopper */
	/*105*/ {'r','w'},  /* rook sparrow: 135 degree grashopper */
	/*106*/ {'b','m'},  /* bishop moose: 45 degree grashopper */
	/*107*/ {'b','e'},  /* bishop eagle: 90 degree grashopper */
	/*108*/ {'b','w'},  /* bishop sparrow: 135 degree grashopper */
	/*109*/ {'r','a'},  /* rao: chinese rose */
	/*110*/ {'s','o'},  /* scorpion: erlking + grashopper */
	/*111*/ {'m','g'},  /* marguerite */
	/*112*/ {'3','6'},  /* 3:6 leaper */
    /*113*/ {'n','l'},  /* nightriderlion */
    /*114*/ {'m','l'},  /* maoriderlion */
    /*115*/ {'m','m'},  /* moariderlion */
    /*116*/ {'f',' '},  /* friend */
    /*117*/ {'d','o'},  /* dolphin */
    /*118*/ {'r','t'},  /* rabbit */
    /*119*/ {'b','o'},  /* bob */
    /*120*/ {'q','e'},  /* equi english */
    /*121*/ {'q','f'},  /* equi french */
    /*122*/ {'q','q'},  /* querquisite */
    /*123*/ {'b','1'},  /* bouncer */
    /*124*/ {'b','2'},  /* tour-bouncer */
    /*125*/ {'b','3'},  /* fou-bouncer */
    /*126*/ {'c','p'},  /* chinese pawn */
    /*127*/ {'r','k'},  /* radial knight */
    /*128*/ {'p','p'},  /* protean pawn */
    /*129*/ {'l','s'},  /* Rose Locust */
    /*130*/ {'z','e'},  /* zebu */
    /*131*/ {'b','n'},  /* Bouncy Nightrider */
    /*132*/ {'s','2'},  /* spiralspringer (2,0) */
    /*133*/ {'s','4'},  /* spiralspringer (4,0) */
    /*134*/ {'s','1'},  /* diagonalspiralspringer (1,1) */
    /*135*/ {'s','3'},  /* diagonalspiralspringer (3,3) */
    /*136*/ {'q','n'},  /* quintessence */
    /*137*/ {'d','k'},  /* double rookhopper */
    /*138*/ {'d','b'},  /* double bishopper */
    /*139*/ {'n','o'},   /* orix (non-stop) */
    /*140*/ {'t','h'},   /* treehopper */
    /*141*/ {'l','h'},   /* leafhopper */
    /*142*/ {'g','e'},   /* greater treehopper */
    /*143*/ {'g','f'},   /* greater leafhopper */
    /*144*/ {'k','l'},   /* kangarou lion */
    /*145*/ {'k','o'},   /* kao */
    /*146*/ {'k','p'},   /* knighthopper */
    /*147*/ {'s','h'},   /* ship */
    /*148*/ {'g','y'},   /* gryphon */
    /*149*/ {'m','s'},   /* marine knight */
    /*150*/ {'p','o'},   /* poseidon */
    /*151*/ {'m','p'},   /* marine pawn */
    /*152*/ {'m','h'}    /* marine ship */
  }
	};
#endif

/* Make sure that we don't define ambiguous piece name shortcuts */
void enforce_piecename_uniqueness(void);

#if defined(WE_ARE_EXTERN)
	extern char     ActAuthor[];
	extern char     ActOrigin[];
	extern char     ActTitle[];
	extern char     ActTwinning[];
	extern char     ActAward[], ActStip[];
#else
	char            ActAuthor[256];
	char            ActOrigin[256];
	char            ActTitle[256];
	char            ActTwinning[1532];
	char            ActAward[256], ActStip[37];
#endif

#if defined(WE_ARE_EXTERN)
	extern PieceChar *PieceTab;
#else
	PieceChar       *PieceTab=PieNamString[German];
#endif
				/* later set according to language */

#if defined(WE_ARE_EXTERN)
	extern boolean  OptFlag[OptCount];
	extern char     *OptString[LanguageCount][OptCount];
#else
	boolean OptFlag[OptCount];
	char    *OptString[LanguageCount][OptCount] = {
	{
	/* Francais French Franzoesisch */
	/* 0*/  "Defense",
	/* 1*/  "Apparent",
	/* 2*/  "ApparentSeul",
	/* 3*/  "Variantes",
	/* 4*/  "Trace",
	/* 5*/  "Enroute",
	/* 6*/  "SansRB",
	/* 7*/  "Duplex",
	/* 8*/  "SansRN",
	/* 9*/  "Menace",
	/*10*/  "SansMenace",
	/*11*/  "Essais",
	/*12*/  "MaxSolutions",
	/*13*/  "MaxCasesFuites",
	/*14*/  "Search",
	/*15*/  "MultiSolutions",
	/*16*/  "NonSymetrique",
	/*17*/  "NonTrivial",
	/*18*/  "RetiensPieceMatante",
	/*19*/  "EnPassant",
	/*20*/  "SansEchiquier",
	/*21*/  "SansVariantesCourtes",
	/*22*/  "DemiDuplex",
	/*23*/  "ApresCle",
	/*24*/  "Intelligent",
	/*25*/  "MaxTemps",
	/*26*/  "SansRoquer",
	/*27*/  "Quodlibet",
	/*28*/  "FinApresSolutionsCourtes",
	/*29*/  "Bip",
	/*30*/  "SansGrille",
	/*31*/  "AjouteGrille",
	/*32*/  "DernierePrise",
    /*33*/  "RoquesMutuellementExclusifs",
    /*34*/  "ButEstFin",
    /*35*/  "optionnonpubliee"
	},{
	/* Deutsch German Allemand */
	/* 0*/  "Widerlegung",
	/* 1*/  "Satzspiel",
	/* 2*/  "WeissBeginnt",
	/* 3*/  "Varianten",
	/* 4*/  "Zugnummern",
	/* 5*/  "StartZugnummer",
	/* 6*/  "OhneWk",
	/* 7*/  "Duplex",
	/* 8*/  "OhneSk",
	/* 9*/  "Drohung",
	/*10*/  "OhneDrohung",
	/*11*/  "Verfuehrung",
	/*12*/  "MaxLoesungen",
	/*13*/  "MaxFluchtFelder",
	/*14*/  "Suchen",
	/*15*/  "MehrereLoesungen",
	/*16*/  "KeineSymmetrie",
	/*17*/  "NichtTrivial",
	/*18*/  "HalteMattsetzendenStein",
	/*19*/  "EnPassant",
	/*20*/  "OhneBrett",
	/*21*/  "OhneKurzVarianten",
	/*22*/  "HalbDuplex",
	/*23*/  "NachSchluessel",
	/*24*/  "Intelligent",
	/*25*/  "MaxZeit",
	/*26*/  "KeineRochade",
	/*27*/  "Quodlibet",
	/*28*/  "StopNachKurzloesungen",
	/*29*/  "Pieps",
	/*30*/  "OhneGitter",
	/*31*/  "ZeichneGitter",
	/*32*/  "LetzterSchlag",
    /*33*/  "RochadenGegenseitigAusschliessend",
    /*34*/  "ZielIstEnde",
    /*35*/  "nichtpublizierteoption"
	},{
	/* English Anglais Englisch */
	/* 0*/  "Defence",
	/* 1*/  "SetPlay",
	/* 2*/  "WhiteToPlay",
	/* 3*/  "Variations",
	/* 4*/  "MoveNumbers",
	/* 5*/  "StartMoveNumber",
	/* 6*/  "NoWk",
	/* 7*/  "Duplex",
	/* 8*/  "NoBk",
	/* 9*/  "Threat",
	/*10*/  "NoThreat",
	/*11*/  "Try",
	/*12*/  "MaxSolutions",
	/*13*/  "MaxFlightsquares",
	/*14*/  "Search",
	/*15*/  "MultiSolutions",
	/*16*/  "NoSymmetry",
	/*17*/  "NonTrivial",
	/*18*/  "KeepMatingPiece",
	/*19*/  "EnPassant",
	/*20*/  "NoBoard",
	/*21*/  "NoShortVariations",
	/*22*/  "HalfDuplex",
	/*23*/  "PostKeyPlay",
	/*24*/  "Intelligent",
	/*25*/  "MaxTime",
	/*26*/  "NoCastling",
	/*27*/  "Quodlibet",
	/*28*/  "StopOnShortSolutions",
	/*29*/  "Beep",
	/*30*/  "SuppressGrid",
	/*31*/  "WriteGrid",
	/*32*/  "LastCapture",
    /*33*/  "CastlingMutuallyExclusive",
    /*34*/  "GoalIsEnd",
    /*35*/  "unpublishedoption"
	}
};
#endif

#if defined(WE_ARE_EXTERN)
  extern boolean  CondFlag[CondCount];
  extern char     *CondString[LanguageCount][CondCount];
#else
  boolean CondFlag[CondCount];
  char    *CondString[LanguageCount][CondCount] = {
  {
    /* French Condition Names */
    /* 0*/  "RexInclusif",
    /* 1*/  "Circe",
    /* 2*/  "CirceMalefique",
    /* 3*/  "Madrasi",
    /* 4*/  "Volage",
    /* 5*/  "Hypervolage",
    /* 6*/  "Bichromatique",
    /* 7*/  "Monochromatique",
    /* 8*/  "Grille",
    /* 9*/  "Koeko",
    /*10*/  "NoirBordCoup",
    /*11*/  "BlancBordCoup",
    /*12*/  "Leofamily",
    /*13*/  "Chinoises",
    /*14*/  "Patrouille",
    /*15*/  "CirceEchange",
    /*16*/  "SansPrises",
    /*17*/  "Immun",
    /*18*/  "ImmunMalefique",
    /*19*/  "ContactGrille",
    /*20*/  "Imitator",
    /*21*/  "CavalierMajeur",
    /*22*/  "Haan",
    /*23*/  "CirceCameleon",
    /*24*/  "CirceCouscous",
    /*25*/  "CirceEquipollents",
    /*26*/  "FileCirce",
    /*27*/  "NoirMaximum",
    /*28*/  "NoirMinimum",
    /*29*/  "BlancMaximum",
    /*30*/  "BlancMinimum",
    /*31*/  "CaseMagique",
    /*32*/  "Sentinelles",
    /*33*/  "Tibet",
    /*34*/  "DoubleTibet",
    /*35*/  "CirceDiagramme",
    /*36*/  "Trou",
    /*37*/  "NoirPriseForce",
    /*38*/  "BlancPriseForce",
    /*39*/  "RoisReflecteurs",
    /*40*/  "RoisTransmutes",
    /*41*/  "NoirPoursuite",
    /*42*/  "BlancPoursuite",
    /*43*/  "Duelliste",
    /*44*/  "CirceParrain",
    /*45*/  "SansIProm",
    /*46*/  "CirceSymetrique",
    /*47*/  "EchecsVogtlaender",
    /*48*/  "EchecsEinstein",
    /*49*/  "Bicolores",
    /*50*/  "KoekoNouveaux",
    /*51*/  "CirceClone",
    /*52*/  "AntiCirce",
    /*53*/  "FileCirceMalefique",
    /*54*/  "CirceAntipoden",
    /*55*/  "CirceCloneMalefique",
    /*56*/  "AntiCirceMalefique",
    /*57*/  "AntiCirceDiagramme",
    /*58*/  "AntiFileCirce",
    /*59*/  "AntiCirceSymetrique",
    /*60*/  "AntiFileCirceMalefique",
    /*61*/  "AntiCirceAntipoden",
    /*62*/  "AntiCirceEquipollents",
    /*63*/  "ImmunFile",
    /*64*/  "ImmunDiagramme",
    /*65*/  "ImmunFileMalefique",
    /*66*/  "ImmunSymmetrique",
    /*67*/  "ImmunAntipoden",
    /*68*/  "ImmunEquipollents",
    /*69*/  "ReversEchecsEinstein",
    /*70*/  "SuperCirce",
    /*71*/  "Degradierung",
    /*72*/  "NorskSjakk",
    /*73*/  "EchecsTraitor",
    /*74*/  "EchecsAndernach",
    /*75*/  "BlancCaseForce",
    /*76*/  "BlancCaseForceConsequent",
    /*77*/  "NoirCaseForce",
    /*78*/  "NoirCaseForceConsequent",
    /*79*/  "EchecsChameleon",
    /*80*/  "EchecsFonctionnaire",
    /*81*/  "EchecsGlasgow",
    /*82*/  "EchecsAntiAndernach",
    /*83*/  "FrischAufCirce",
    /*84*/  "CirceMalefiqueVerticale",
    /*85*/  "Isardam",
    /*86*/  "SansEchecs",
    /*87*/  "CirceDiametrale",
    /*88*/  "PromSeul",
    /*89*/  "RankCirce",
    /*90*/  "EchecsExclusif",
    /*91*/  "MarsCirce",
    /*92*/  "MarsCirceMalefique",
    /*93*/  "EchecsPhantom",
    /*94*/  "BlancRoiReflecteur",
    /*95*/  "NoirRoiReflecteur",
    /*96*/  "BlancRoiTransmute",
    /*97*/  "NoirRoiTransmute",
    /*98*/  "EchecsAntiEinstein",
    /*99*/  "CirceCouscousMalefique",
    /*100*/ "NoirCaseRoyal",
    /*101*/ "BlancCaseRoyal",
    /*102*/ "EchecsBrunner",
    /*103*/ "EchecsPlus",
    /*104*/ "CirceAssassin",
    /*105*/ "EchecsPatience",
    /*106*/ "EchecsRepublicains",
    /*107*/ "EchecsExtinction",
    /*108*/ "EchecsCentral",
    /*109*/ "ActuatedRevolvingBoard",
    /*110*/ "EchecsMessigny",
    /*111*/ "Woozles",
    /*112*/ "BiWoozles",
    /*113*/ "Heffalumps",
    /*114*/ "BiHeffalumps",
    /*115*/ "RexExclusif",
    /*116*/ "BlancCasePromotion",
    /*117*/ "NoirCasePromotion",
    /*118*/ "SansBlancPromotion",
    /*119*/ "SansNoirPromotion",
    /*120*/ "EchecsEiffel",
    /*121*/ "NoirUltraSchachZwang",
    /*122*/ "BlancUltraSchachZwang",
    /*123*/ "ActuatedRevolvingCentre",
    /*124*/ "ShieldedKings",
    /*125*/ "EchecsSting",
    /*126*/ "EchecsChameleonLigne",
    /*127*/ "BlancSansPrises",
    /*128*/ "NoirSansPrises",
    /*129*/ "EchecsAvril",
    /*130*/ "EchecsAlphabetiques",
    /*131*/ "CirceTurncoats",
    /*132*/ "CirceDoubleAgents",
    /*133*/ "AMU",
    /*134*/ "SingleBox",
    /*135*/ "MAFF",
    /*136*/ "OWU",
    /*137*/ "BlancRoisOscillant",
    /*138*/ "NoirRoisOscillant",
    /*139*/ "AntiRois",
    /*140*/ "AntiMarsCirce",
    /*141*/ "AntiMarsMalefiqueCirce",
    /*142*/ "AntiMarsAntipodeanCirce",
    /*143*/ "BlancSuperRoiTransmute",
    /*144*/ "NoirSuperRoiTransmute",
    /*145*/ "AntiSuperCirce",
    /*146*/ "UltraPatrouille",
    /*147*/ "RoisEchanges",
    /*148*/ "DynastieRoyale",
    /*149*/ "SAT",
    /*150*/ "StrictSAT",
    /*151*/ "Take&MakeEchecs",
    /*152*/ "NoirSynchronCoup",
    /*153*/ "BlancSynchronCoup",
    /*154*/ "NoirAntiSynchronCoup",
    /*155*/ "BlancAntiSynchronCoup",
    /*156*/ "Masand",
    /*157*/ "BGL" ,
    /*158*/ "NoirEchecs",
    /*159*/ "AnnanEchecs" ,
    /*160*/ "PionNormale",
    /*161*/ "Elliuortap",
    /*162*/ "VaultingKings",
    /*163*/ "BlancVaultingKing",
    /*164*/ "NoirVaultingKing",
    /*165*/ "EchecsProtee",
    /*166*/ "EchecsGeneve",
    /*167*/ "ChameleonPoursuite",
    /*168*/ "AntiKoeko",
    /*169*/ "EchecsRoque",
    /*170*/ "QuiPerdGagne",
    /*171*/ "Disparate",
    /*172*/ "EchecsGhost",
    /*173*/ "EchecsHantes",
    /*174*/ "EchecsProvacateurs",
    /*175*/ "CirceCage",
    /*176*/ "ImmunCage",
    /*177*/ "Football",
    /*178*/ "ContraParrain",
    /*179*/ "RoisKobul",
    /*180*/ "EchangeRoque",
    /*181*/ "BlancEchecsAlphabetiques",
    /*182*/ "NoirEchecsAlphabetiques",
    /*183*/ "CirceTake&Make",
    /*184*/ "Supergardes",
    /*185*/ "TrousDeVer",
    /*186*/ "EchecsMarins",
    /*187*/ "EchecsUltramarins"
  },{
    /* German Condition Names */
    /* 0*/  "RexInklusive",
    /* 1*/  "Circe",
    /* 2*/  "SpiegelCirce",
    /* 3*/  "Madrasi",
    /* 4*/  "Volage",
    /* 5*/  "Hypervolage",
    /* 6*/  "BichromesSchach",
    /* 7*/  "MonochromesSchach",
    /* 8*/  "Gitterschach",
    /* 9*/  "KoeKo",
    /*10*/  "SchwarzerRandzueger",
    /*11*/  "WeisserRandzueger",
    /*12*/  "Leofamily",
    /*13*/  "ChinesischesSchach",
    /*14*/  "PatrouilleSchach",
    /*15*/  "PlatzwechselCirce",
    /*16*/  "Ohneschlag",
    /*17*/  "Immunschach",
    /*18*/  "SpiegelImmunschach",
    /*19*/  "KontaktGitter",
    /*20*/  "Imitator",
    /*21*/  "CavalierMajeur",
    /*22*/  "HaanerSchach",
    /*23*/  "ChamaeleonCirce",
    /*24*/  "CouscousCirce",
    /*25*/  "EquipollentsCirce",
    /*26*/  "FileCirce",
    /*27*/  "SchwarzerLaengstzueger",
    /*28*/  "SchwarzerKuerzestzueger",
    /*29*/  "WeisserLaengstzueger",
    /*30*/  "WeisserKuerzestzueger",
    /*31*/  "MagischeFelder",
    /*32*/  "Sentinelles",
    /*33*/  "TibetSchach",
    /*34*/  "DoppeltibetSchach",
    /*35*/  "DiagrammCirce",
    /*36*/  "Loch",
    /*37*/  "SchwarzerSchlagzwang",
    /*38*/  "WeisserSchlagzwang",
    /*39*/  "ReflektierendeKoenige",
    /*40*/  "TransmutierendeKoenige",
    /*41*/  "SchwarzerVerfolgungszueger",
    /*42*/  "WeisserVerfolgungszueger",
    /*43*/  "Duellantenschach",
    /*44*/  "CirceParrain",
    /*45*/  "OhneIUW",
    /*46*/  "SymmetrieCirce",
    /*47*/  "VogtlaenderSchach",
    /*48*/  "EinsteinSchach",
    /*49*/  "Bicolores",
    /*50*/  "NeuKoeko",
    /*51*/  "CirceClone",
    /*52*/  "AntiCirce",
    /*53*/  "SpiegelFileCirce",
    /*54*/  "AntipodenCirce",
    /*55*/  "SpiegelCirceclone",
    /*56*/  "AntiSpiegelCirce",
    /*57*/  "AntiDiagrammCirce",
    /*58*/  "AntiFileCirce",
    /*59*/  "AntiSymmetrieCirce",
    /*60*/  "AntiSpiegelFileCirce",
    /*61*/  "AntiAntipodenCirce",
    /*62*/  "AntiEquipollentsCirce",
    /*63*/  "FileImmunSchach",
    /*64*/  "DiagrammImmunSchach",
    /*65*/  "SpiegelFileImmunSchach",
    /*66*/  "SymmetrieImmunSchach",
    /*67*/  "AntipodenImmunSchach",
    /*68*/  "EquipollentsImmunSchach",
    /*69*/  "ReversesEinsteinSchach",
    /*70*/  "SuperCirce",
    /*71*/  "Degradierung",
    /*72*/  "NorskSjakk",
    /*73*/  "TraitorChess",
    /*74*/  "AndernachSchach",
    /*75*/  "WeissesZwangsfeld",
    /*76*/  "WeissesKonsequentesZwangsfeld",
    /*77*/  "SchwarzesZwangsfeld",
    /*78*/  "SchwarzesKonsequentesZwangsfeld",
    /*79*/  "ChamaeleonSchach",
    /*80*/  "BeamtenSchach",
    /*81*/  "GlasgowSchach",
    /*82*/  "AntiAndernachSchach",
    /*83*/  "FrischAufCirce",
    /*84*/  "VertikalesSpiegelCirce",
    /*85*/  "Isardam",
    /*86*/  "OhneSchach",
    /*87*/  "DiametralCirce",
    /*88*/  "UWnur",
    /*89*/  "RankCirce",
    /*90*/  "ExklusivSchach",
    /*91*/  "MarsCirce",
    /*92*/  "MarsSpiegelCirce",
    /*93*/  "PhantomSchach",
    /*94*/  "WeisserReflektierenderKoenig",
    /*95*/  "SchwarzerReflektierenderKoenig",
    /*96*/  "WeisserTransmutierenderKoenig",
    /*97*/  "SchwarzerTransmutierenderKoenig",
    /*98*/  "AntiEinsteinSchach",
    /*99*/  "SpiegelCouscousCirce",
    /*100*/ "SchwarzesKoeniglichesFeld",
    /*101*/ "WeissesKoeniglichesFeld",
    /*102*/ "BrunnerSchach",
    /*103*/ "PlusSchach",
    /*104*/ "AssassinCirce",
    /*105*/ "PatienceSchach",
    /*106*/ "RepublikanerSchach",
    /*107*/ "AusrottungsSchach",
    /*108*/ "ZentralSchach",
    /*109*/ "ActuatedRevolvingBoard",
    /*110*/ "MessignySchach",
    /*111*/ "Woozles",
    /*112*/ "BiWoozles",
    /*113*/ "Heffalumps",
    /*114*/ "BiHeffalumps",
    /*115*/ "RexExklusive",
    /*116*/ "UWFeldWeiss",
    /*117*/ "UWFeldSchwarz",
    /*118*/ "OhneWeissUW",
    /*119*/ "OhneSchwarzUW",
    /*120*/ "EiffelSchach",
    /*121*/ "SchwarzerUltraSchachZwang",
    /*122*/ "WeisserUltraSchachZwang",
    /*123*/ "ActuatedRevolvingCentre",
    /*124*/ "SchutzKoenige",
    /*125*/ "StingSchach",
    /*126*/ "LinienChamaeleonSchach",
    /*127*/ "WeisserOhneSchlag",
    /*128*/ "SchwarzerOhneSchlag",
    /*129*/ "Aprilschach",
    /*130*/ "AlphabetischesSchach",
    /*131*/ "TurncoatCirce",
    /*132*/ "DoppelAgentenCirce",
    /*133*/ "AMU",
    /*134*/ "NurPartiesatzSteine",
    /*135*/ "MAFF",
    /*136*/ "OWU",
    /*137*/ "WeisseOszillierendeKoenige",
    /*138*/ "SchwarzeOszillierendeKoenige",
    /*139*/ "AntiKoenige",
    /*140*/ "AntiMarsCirce",
    /*141*/ "AntiMarsMalefiqueCirce",
    /*142*/ "AntiMarsAntipodeanCirce",
    /*143*/ "WeisserSuperTransmutierenderKoenig",
    /*144*/ "SchwarzerSuperTransmutierenderKoenig",
    /*145*/ "AntiSuperCirce",
    /*146*/ "UltraPatrouille",
    /*147*/ "TauschKoenige",
    /*148*/ "KoenigsDynastie",
    /*149*/ "SAT",
    /*150*/ "StrictSAT",
    /*151*/ "Take&MakeSchach",
    /*152*/ "SchwarzerSynchronZueger",
    /*153*/ "WeisserSynchronZueger",
    /*154*/ "SchwarzerAntiSynchronZueger",
    /*155*/ "WeisserAntiSynchronZueger",
    /*156*/ "Masand",
    /*157*/ "BGL",
    /*158*/ "SchwarzSchaecher",
    /*159*/ "Annanschach",
    /*160*/ "NormalBauern",
    /*161*/ "Elliuortap",
    /*162*/ "VaultingKings",
    /*163*/ "WeisserVaultingKing",
    /*164*/ "SchwarzerVaultingKing",
    /*165*/ "ProteischesSchach",
    /*166*/ "GenferSchach",
    /*167*/ "ChamaeleonVerfolgung",
    /*168*/ "AntiKoeko",
    /*169*/ "RochadeSchach",
    /*170*/ "Schlagschach",
    /*171*/ "Disparate",
    /*172*/ "Geisterschach",
    /*173*/ "Spukschach",
    /*174*/ "ProvokationSchach",
    /*175*/ "KaefigCirce",
    /*176*/ "KaefigImmunSchach",
    /*177*/ "Fussball",
    /*178*/ "KontraParrain",
    /*179*/ "KobulKoenige",
    /*180*/ "PlatzwechselRochade",
    /*181*/ "WeissesAlphabetischesSchach",
    /*182*/ "SchwarzesAlphabetischesSchach",
    /*183*/ "CirceTake&Make",
    /*184*/ "SuperDeckungen",
    /*185*/ "Wurmloecher",
    /*186*/ "MarinesSchach",
    /*187*/ "UltramarinesSchach"
  },{
    /* English Condition Names */
    /* 0*/  "RexInclusive",
    /* 1*/  "Circe",
    /* 2*/  "MirrorCirce",
    /* 3*/  "Madrasi",
    /* 4*/  "Volage",
    /* 5*/  "Hypervolage",
    /* 6*/  "BichromChess",
    /* 7*/  "MonochromChess",
    /* 8*/  "GridChess",
    /* 9*/  "KoeKo",
    /*10*/  "BlackEdgeMover",
    /*11*/  "WhiteEdgeMover",
    /*12*/  "Leofamily",
    /*13*/  "ChineseChess",
    /*14*/  "Patrouille",
    /*15*/  "PWC",
    /*16*/  "NoCapture",
    /*17*/  "ImmunChess",
    /*18*/  "MirrorImmunChess",
    /*19*/  "ContactGridChess",
    /*20*/  "Imitator",
    /*21*/  "CavalierMajeur",
    /*22*/  "HaanerChess",
    /*23*/  "ChameleonCirce",
    /*24*/  "CouscousCirce",
    /*25*/  "EquipollentsCirce",
    /*26*/  "FileCirce",
    /*27*/  "BlackMaximummer",
    /*28*/  "BlackMinimummer",
    /*29*/  "WhiteMaximummer",
    /*30*/  "WhiteMinimummer",
    /*31*/  "MagicSquares",
    /*32*/  "Sentinelles",
    /*33*/  "Tibet",
    /*34*/  "DoubleTibet",
    /*35*/  "DiagramCirce",
    /*36*/  "Hole",
    /*37*/  "BlackMustCapture",
    /*38*/  "WhiteMustCapture",
    /*39*/  "ReflectiveKings",
    /*40*/  "TransmutedKings",
    /*41*/  "BlackFollowMyLeader",
    /*42*/  "WhiteFollowMyLeader",
    /*43*/  "DuellistChess",
    /*44*/  "ParrainCirce",
    /*45*/  "NoIProm",
    /*46*/  "SymmetryCirce",
    /*47*/  "VogtlaenderChess",
    /*48*/  "EinsteinChess",
    /*49*/  "Bicolores",
    /*50*/  "NewKoeko",
    /*51*/  "CirceClone",
    /*52*/  "AntiCirce",
    /*53*/  "MirrorFileCirce",
    /*54*/  "AntipodeanCirce",
    /*55*/  "MirrorCirceClone",
    /*56*/  "MirrorAntiCirce",
    /*57*/  "DiagramAntiCirce",
    /*58*/  "FileAntiCirce",
    /*59*/  "SymmetryAntiCirce",
    /*60*/  "MirrorAntiCirce",
    /*61*/  "AntipodeanAntiCirce",
    /*62*/  "EquipollentsAntiCirce",
    /*63*/  "FileImmunChess",
    /*64*/  "DiagramImmunChess",
    /*65*/  "MirrorFileImmunChess",
    /*66*/  "SymmetryImmunChess",
    /*67*/  "AntipodeanImmunChess",
    /*68*/  "EquipollentsImmunChess",
    /*69*/  "ReversalEinsteinChess",
    /*70*/  "SuperCirce",
    /*71*/  "RelegationChess",
    /*72*/  "NorskSjakk",
    /*73*/  "TraitorChess",
    /*74*/  "AndernachChess",
    /*75*/  "WhiteForcedSquare",
    /*76*/  "WhiteConsequentForcedSquare",
    /*77*/  "BlackForcedSquare",
    /*78*/  "BlackConsequentForcedSquare",
    /*79*/  "ChameleonChess",
    /*80*/  "FunctionaryChess",
    /*81*/  "GlasgowChess",
    /*82*/  "AntiAndernachChess",
    /*83*/  "FrischAufCirce",
    /*84*/  "VerticalMirrorCirce",
    /*85*/  "Isardam",
    /*86*/  "ChecklessChess",
    /*87*/  "DiametralCirce",
    /*88*/  "PromOnly",
    /*89*/  "RankCirce",
    /*90*/  "ExclusiveChess",
    /*91*/  "MarsCirce",
    /*92*/  "MarsMirrorCirce",
    /*93*/  "PhantomChess",
    /*94*/  "WhiteReflectiveKing",
    /*95*/  "BlackReflectiveKing",
    /*96*/  "WhiteTransmutedKing",
    /*97*/  "BlackTransmutedKing",
    /*98*/  "AntiEinsteinChess",
    /*99*/  "MirrorCouscousCirce",
    /*100*/ "BlackRoyalSquare",
    /*101*/ "WhiteRoyalSquare",
    /*102*/ "BrunnerChess",
    /*103*/ "PlusChess",
    /*104*/ "CirceAssassin",
    /*105*/ "PatienceChess",
    /*106*/ "RepublicanChess",
    /*107*/ "ExtinctionChess",
    /*108*/ "CentralChess",
    /*109*/ "ActuatedRevolvingBoard",
    /*110*/ "MessignyChess",
    /*111*/ "Woozles",
    /*112*/ "BiWoozles",
    /*113*/ "Heffalumps",
    /*114*/ "BiHeffalumps",
    /*115*/ "RexExclusive",
    /*116*/ "WhitePromSquares",
    /*117*/ "BlackPromSquares",
    /*118*/ "NoWhitePromotion",
    /*119*/ "NoBlackPromotion",
    /*120*/ "EiffelChess",
    /*121*/ "BlackUltraSchachZwang",
    /*122*/ "WhiteUltraSchachZwang",
    /*123*/ "ActuatedRevolvingCentre",
    /*124*/ "ShieldedKings",
    /*125*/ "StingChess",
    /*126*/ "LineChameleonChess",
    /*127*/ "NoWhiteCapture",
    /*128*/ "NoBlackCapture",
    /*129*/ "AprilChess",
    /*130*/ "AlphabeticChess",
    /*131*/ "CirceTurncoats",
    /*132*/ "CirceDoubleAgents",
    /*133*/ "AMU",
    /*134*/ "SingleBox",
    /*135*/ "MAFF",
    /*136*/ "OWU",
    /*137*/ "WhiteOscillatingKings",
    /*138*/ "BlackOscillatingKings",
    /*139*/ "AntiKings",
    /*140*/ "AntiMarsCirce",
    /*141*/ "AntiMarsMirrorCirce",
    /*142*/ "AntiMarsAntipodeanCirce",
    /*143*/ "WhiteSuperTransmutingKing",
    /*144*/ "BlackSuperTransmutingKing",
    /*145*/ "AntiSuperCirce",
    /*146*/ "UltraPatrol",
    /*147*/ "SwappingKings",
    /*148*/ "RoyalDynasty",
    /*149*/ "SAT",
    /*150*/ "StrictSAT",
    /*151*/ "Take&MakeChess",
    /*152*/ "BlackSynchronMover",
    /*153*/ "WhiteSynchronMover",
    /*154*/ "BlackAntiSynchronMover",
    /*155*/ "WhiteAntiSynchronMover",
    /*156*/ "Masand",
    /*157*/ "BGL",
    /*158*/ "BlackChecks",
    /*159*/ "AnnanChess",
    /*160*/ "NormalPawn",
    /*161*/ "Lortap",
    /*162*/ "VaultingKings",
    /*163*/ "WhiteVaultingKing",
    /*164*/ "BlackVaultingKing",
    /*165*/ "ProteanChess",
    /*166*/ "GenevaChess",
    /*167*/ "ChameleonPursuit",
    /*168*/ "AntiKoeko",
    /*169*/ "CastlingChess",
    /*170*/ "LosingChess",
    /*171*/ "Disparate",
    /*172*/ "GhostChess",
    /*173*/ "HauntedChess",
    /*174*/ "ProvocationChess",
    /*175*/ "CageCirce",
    /*176*/ "CageImmunChess",
    /*177*/ "Football",
    /*178*/ "ContraParrain",
    /*179*/ "KobulKings",
    /*180*/ "ExchangeCastling",
    /*181*/ "WhiteAlphabeticChess",
    /*182*/ "BlackAlphabeticChess",
    /*183*/ "CirceTake&Make",
    /*184*/ "SuperGuards",
    /*185*/ "Wormholes",
    /*186*/ "MarineChess",
    /*187*/ "UltramarineChess"
  }
};
#endif

#if defined(WE_ARE_EXTERN)
  extern char *ColorString[LanguageCount][nr_colors];
  extern char **ColorTab;
#else
  char    **ColorTab;
  char    *ColorString[LanguageCount][nr_colors] =
  {
    {
       /* French */
      "Blanc",
      "Noir",
      "Neutre"
    },
    {
      /* German */
      "Weiss",
      "Schwarz",
      "Neutral"
    },
    {
      /* English */
      "White",
      "Black",
      "Neutral"
    }
  };
#endif

#if defined(WE_ARE_EXTERN)
	extern  char    *PieSpString[LanguageCount][PieSpCount-nr_sides];
	extern  char    **PieSpTab;
	extern  Flags   some_pieces_flags;
  extern  Flags   all_pieces_flags;
  extern  Flags   all_royals_flags;
#else
	Flags   some_pieces_flags;   /* used for problem-wide piecespecification */
  Flags   all_pieces_flags;
  Flags   all_royals_flags;
	char    **PieSpTab;
	char    *PieSpString[LanguageCount][PieSpCount-nr_sides] =
	{
    {
      /* French */
      "Royale",
      "Kamikaze",
      "Paralysante",
      "Chameleon",
      "Jigger",
      "Volage",
      "Fonctionnaire",
      "DemiNeutre",
      "CouleurEchangeantSautoir",
      "Protee",
      "Magique",
      "Imprenable",
      "Patrouille",
      "Frischauf"
    },
    {
      /* German */
      "Koeniglich",
      "Kamikaze",
      "Paralysierend",
      "Chamaeleon",
      "Jigger",
      "Volage",
      "Beamtet",
      "HalbNeutral",
      "SprungbockFarbeWechselnd",
      "Proteisch",
      "Magisch",
      "Unschlagbar",
      "Patrouille",
      "Frischauf"
    },
    {
      /* English */
      "Royal",
      "Kamikaze",
      "Paralysing",
      "Chameleon",
      "Jigger",
      "Volage",
      "Functionary",
      "HalfNeutral",
      "HurdleColourChanging",
      "Protean",
      "Magic",
      "Uncapturable",
      "Patrol",
      "Frischauf"
    }
  };
#endif

#if defined(WE_ARE_EXTERN)
	extern char GlobalStr[];
#else
	char GlobalStr[4*maxply];
#endif

#if defined(WE_ARE_EXTERN)
	extern char
*VariantTypeString[LanguageCount][VariantTypeCount];
#else
	char    *VariantTypeString[LanguageCount][VariantTypeCount] = {
	{
	/* French */
	/* 0*/  "TypeB",
	/* 1*/  "PionAdvers",
	/* 2*/  "Cheylan",
	/* 3*/  "Calvet",
	/* 4*/  "PionNeutre",
	/* 5*/  "MaximumNoir",
	/* 6*/  "MaximumBlanc",
    /* 7*/  "ParaSentinelles",
    /* 8*/  "MaximumTotal",
	/* 9*/  "Berolina",
	/*10*/  "Type1",
	/*11*/  "Type2",
	/*12*/  "Type3",
	/*13*/  "Voisin",
	/*14*/  "TypeC",
	/*15*/  "TypeD",
	/*16*/  "VerticalDecalage",
	/*17*/  "HorizontalDecalage",
	/*18*/  "DiagonalDecalage",
	/*19*/  "Orthogonale",
	/*20*/  "Irreguliere",
	/*21*/  "LigneGrille",
  /*22*/  "Transmute",
  /*23*/  "Blancs",
  /*24*/ "Noirs"
	},{
	/* German */
	/* 0*/  "TypB",
	/* 1*/  "PionAdvers",
	/* 2*/  "Cheylan",
	/* 3*/  "Calvet",
	/* 4*/  "PionNeutre",
	/* 5*/  "MaximumSchwarz",
	/* 6*/  "MaximumWeiss",
    /* 7*/  "ParaSentinelles",
    /* 8*/  "MaximumTotal",
	/* 9*/  "Berolina",
	/*10*/  "Typ1",
	/*11*/  "Typ2",
	/*12*/  "Typ3",
	/*13*/  "Nachbar",
	/*14*/  "TypC",
	/*15*/  "TypD",
	/*16*/  "VertikalVerschieben",
	/*17*/  "HorizontalVerschieben",
	/*18*/  "DiagonalVerschieben",
	/*19*/  "Orthogonal",
	/*20*/  "Unregelmaessig",
	/*21*/  "GitterLinie",
  /*22*/  "Transmutierende",
  /*23*/  "Weiss",
  /*24*/ "Schwarz"
	},{
	/* English */
	/* 0*/  "TypeB",
	/* 1*/  "PionAdvers",
	/* 2*/  "Cheylan",
	/* 3*/  "Calvet",
	/* 4*/  "PionNeutre",
	/* 5*/  "MaximumBlack",
	/* 6*/  "MaximumWhite",
    /* 7*/  "ParaSentinelles",
    /* 8*/  "MaximumTotal",
	/* 9*/  "Berolina",
	/*10*/  "Type1",
	/*11*/  "Type2",
	/*12*/  "Type3",
	/*13*/  "Neighbour",
	/*14*/  "TypeC",
	/*15*/  "TypeD",
	/*16*/  "VerticalShift",
	/*17*/  "HorizontalShift",
	/*18*/  "DiagonalShift",
	/*19*/  "Orthogonal",
	/*20*/  "Irregular",
	/*21*/  "GridLine",
  /*22*/  "Transmuting",
  /*23*/  "White",
  /*24*/ "Black"
	}
	};
#endif

#if defined(WE_ARE_EXTERN)
  extern boolean  ExtraCondFlag[ExtraCondCount];
	extern char *ExtraCondString[LanguageCount][ExtraCondCount];
#else
  boolean  ExtraCondFlag[ExtraCondCount];
	char    *ExtraCondString[LanguageCount][ExtraCondCount] = {
	{
	/* French */
	/* 0*/  "Maximum",
	/* 1*/  "UltraSchachZwang"
	},{
	/* German */
	/* 0*/  "Laengstzueger",
	/* 1*/  "UltraSchachZwang"
	},{
	/* English */
	/* 0*/  "Maximummer",
	/* 1*/  "UltraSchachZwang"
	}
	};
#endif


  typedef unsigned int move_diff_type;

#if defined(WE_ARE_EXTERN)
	extern move_diff_type const move_diff_code[square_h8 - square_a1 + 1];
#else
/* This are the codes for the length-difference */
/* between two squares */
/* ATTENTION: use abs(square from - square to) for indexing this table. */
/*        all move_down_codes are mapped this way to move_up_codes !    */

  move_diff_type const move_diff_code[square_h8 - square_a1 + 1]=
  {
    /* left/right   */        0,   1,   4,   9,  16,  25,  36,  49,
    /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
    /* 1 left  up   */            50,  37,  26,  17,  10,   5,   2,
    /* 1 right up   */        1,   2,   5,  10,  17,  26,  37,  50,
    /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
    /* 2 left  up   */            53,  40,  29,  20,  13,   8,   5,
    /* 2 right up   */        4,   5,   8,  13,  20,  29,  40,  53,
    /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
    /* 3 left  up   */            58,  45,  34,  25,  18,  13,  10,
    /* 3 right up   */        9,  10,  13,  18,  25,  34,  45,  58,
    /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
    /* 4 left  up   */            65,  52,  41,  32,  25,  20,  17,
    /* 4 right up   */       16,  17,  20,  25,  32,  41,  52,  65,
    /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
    /* 5 left  up   */            74,  61,  50,  41,  34,  29,  26,
    /* 5 right up   */       25,  26,  29,  34,  41,  50,  61,  74,
    /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
    /* 6 left  up   */            85,  72,  61,  52,  45,  40,  37,
    /* 6 right up   */       36,  37,  40,  45,  52,  61,  72,  85,
    /* dummies      */       UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX,  UINT_MAX, UINT_MAX,
    /* 7 left  up   */            98,  85,  74,  65,  58,  53,  50,
    /* 7 right up   */       49,  50,  53,  58,  65,  74,  85,  98
  };
#endif

#if defined(WE_ARE_EXTERN)
	extern  int move_vec_code[square_h8 - square_a1 + 1];
#else
/* This are the codes for the length-difference */
/* between two squares */
/* ATTENTION: use abs(square from - square to) for indexing this table. */
/*        all move_down_codes are mapped this way to move_up_codes !    */

	int move_vec_code[square_h8 - square_a1 + 1]= {
	/* left/right   */         0,   1,   1,   1,   1,   1,   1,   1,
	/* dummies      */        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	/* 1 left  up   */             17,  18,  19,  20,  21,  22,  23,
	/* 1 right up   */        24,  25,  26,  27,  28,  29,  30,  31,
	/* dummies      */        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	/* 2 left  up   */             41,  21,  43,  22,  45,  23,  47,
	/* 2 right up   */        24,  49,  25,  51,  26,  53,  27,  55,
	/* dummies      */        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	/* 3 left  up   */             65,  22,  67,  68,  23,  70,  71,
	/* 3 right up   */        24,  73,  74,  25,  76,  77,  26,  79,
	/* dummies      */        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	/* 4 left  up   */             89,  45,  91,  23,  93,  47,  95,
	/* 4 right up   */        24,  97,  49,  99,  25, 101,  51, 103,
	/* dummies      */        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	/* 5 left  up   */            113, 114,  23, 116, 117, 118, 119,
	/* 5 right up   */        24, 121, 122, 123, 124,  25, 126, 127,
	/* dummies      */        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	/* 6 left  up   */            137,  23, 139,  70,  47,  71, 143,
	/* 6 right up   */        24, 145,  73,  49,  74, 149,  25, 151,
	/* dummies      */        -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	/* 7 left  up   */             23, 162, 163, 164, 165, 166, 167,
	/* 7 right up   */        24, 169, 170, 171, 172, 173, 174,  25
			       };
#endif

#if defined(WE_ARE_EXTERN)
	extern  checkfunction_t *checkfunctions[PieceCount];
#else
/* This are the used checkingfunctions  */
    checkfunction_t *checkfunctions[PieceCount] = {
/*  0 */        0, /* not used */
/*  1 */        0, /* not used */
/*  2 */        roicheck,
/*  3 */        pioncheck,
/*  4 */        damecheck,
/*  5 */        cavcheck,
/*  6 */        tourcheck,
/*  7 */        foucheck,
/*  8 */        leocheck,
/*  9 */        maocheck,
/* 10 */        paocheck,
/* 11 */        vaocheck,
/* 12 */        rosecheck,
/* 13 */        scheck,
/* 14 */        ncheck,
/* 15 */        zcheck,
/* 16 */        chcheck,
/* 17 */        gicheck,
/* 18 */        rccinqcheck,
/* 19 */        bucheck,
/* 20 */        vizircheck,
/* 21 */        alfilcheck,
/* 22 */        ferscheck,
/* 23 */        dabcheck,
/* 24 */        lioncheck,
/* 25 */        nequicheck,
/* 26 */        loccheck,
/* 27 */        berolina_pawn_check,
/* 28 */        amazcheck,
/* 29 */        impcheck,
/* 30 */        princcheck,
/* 31 */        gnoucheck,
/* 32 */        antilcheck,
/* 33 */        ecurcheck,
/* 34 */        warancheck,
/* 35 */        dragoncheck,
/* 36 */        kangoucheck,
/* 37 */        cscheck,
/* 38 */        ubiubi_check,
/* 39 */        nevercheck,	/* hamster cannot check */
/* 40 */        moosecheck,
/* 41 */        eaglecheck,
/* 42 */        sparrcheck,
/* 43 */        archcheck,
/* 44 */        reffoucheck,
/* 45 */        cardcheck,
/* 46 */        nsautcheck,
/* 47 */        nevercheck,	/* dummy cannot check */
/* 48 */        camridcheck,
/* 49 */        zebridcheck,
/* 50 */        gnuridcheck,
/* 51 */        camhopcheck,
/* 52 */        zebhopcheck,
/* 53 */        gnuhopcheck,
/* 54 */        dcscheck,
/* 55 */        refccheck,
/* 56 */        equicheck,
/* 57 */        catcheck,
/* 58 */        bspawncheck,
/* 59 */        spawncheck,
/* 60 */        t_lioncheck,
/* 61 */        f_lioncheck,
/* 62 */        loccheck,               /* sirene checks like locust */
/* 63 */        tritoncheck,
/* 64 */        nereidecheck,
/* 65 */        orphancheck,
/* 66 */        edgehcheck,
/* 67 */        moacheck,
/* 68 */        rhuntcheck,
/* 69 */        bhuntcheck,
/* 70 */        maoridercheck,
/* 71 */        moaridercheck,
/* 72 */        r_hopcheck,
/* 73 */        b_hopcheck,
/* 74 */        roicheck,               /* erlking checks like king */
/* 75 */        bscoutcheck,
/* 76 */        gscoutcheck,
/* 77 */        skyllacheck,
/* 78 */        charybdischeck,
/* 79 */        contragrascheck,
/* 80 */        roselioncheck,
/* 81 */        rosehoppercheck,
/* 82 */        okapicheck,
/* 83 */        leap37check,
/* 84 */        grasshop2check,
/* 85 */        grasshop3check,
/* 86 */        leap16check,
/* 87 */        leap24check,
/* 88 */        leap35check,
/* 89 */        doublegrasshoppercheck,
/* 90 */        kinghopcheck,
/* 91 */        orixcheck,
/* 92 */        leap15check,
/* 93 */        leap25check,
/* 94 */        gralcheck,
/* 95 */        tritoncheck,
/* 96 */        nereidecheck,
/* 97 */        nightlocustcheck,
/* 98 */        tourcheck,              /* wazirrider checks like rook */
/* 99 */        foucheck,               /* fersrider checks like bishop */
/*100 */        bisoncheck,
/*101 */        elephantcheck,
/*102 */        naocheck,
/*103 */        rookmoosecheck,
/*104 */        rookeaglecheck,
/*105 */        rooksparrcheck,
/*106 */        bishopmoosecheck,
/*107 */        bishopeaglecheck,
/*108 */        bishopsparrcheck,
/*109 */        roselioncheck,  	/* rao checks like roselion */
/*110 */        scorpioncheck,
/*111 */        margueritecheck,
/*112 */        leap36check,
/*113 */        nightriderlioncheck,
/*114 */        maoriderlioncheck,
/*115 */        moariderlioncheck,
/*116 */        friendcheck,
/*117 */        dolphincheck,
/*118 */        kanglioncheck,
/*119 */        bobcheck,
/*120 */	equiengcheck,
/*121 */	equifracheck,
/*122 */	querquisitecheck,
/*123 */	bouncercheck,
/*124 */	rookbouncercheck,
/*125 */	bishopbouncercheck,
/*126 */	pchincheck,
/*127 */  radialknightcheck,
/*128 */  reversepcheck,
/*129 */  roselocustcheck,
/*130 */  zebucheck,
/*131 */  refncheck,
/*132 */  sp20check,
/*133 */  sp40check,
/*134 */  sp11check,
/*135 */  sp33check,
/*136 */  sp31check,
/*137 */  doublerookhoppercheck,
/*138 */  doublebishoppercheck,
/*139 */  norixcheck,
/*140 */  treehoppercheck,
/*141 */  leafhoppercheck,
/*142 */  greatertreehoppercheck,
/*143 */  greaterleafhoppercheck,
/*144 */  kanglioncheck,
/*145 */  knighthoppercheck,
/*146 */  knighthoppercheck,
/*147 */  shipcheck,
/*148 */  gryphoncheck,
/*149 */  marine_knight_check,
/*150 */  poseidon_check,
/*151 */  marine_pawn_check,
/*152 */  marine_ship_check,
/*153 */  huntercheck,
/*154 */  huntercheck,
/*155 */  huntercheck,
/*156 */  huntercheck,
/*157 */  huntercheck,
/*158 */  huntercheck,
/*159 */  huntercheck,
/*160 */  huntercheck,
/*161 */  huntercheck,
/*162 */  huntercheck
    };
#endif

#define hunterseparator '/'

#endif  /* not PYDATA_H */
