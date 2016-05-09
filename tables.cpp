#include "main.h"

#include <map>

using namespace std;

map<feature,string> featNames={
    {F_NULL,"NULL"},
    {F_LETTER,"l"},
    {F_PNCT,"pnct"},
    {F_SENTENCE,"s"},
    {F_WORD,"w"},
    {F_LEMMAID,"LEMMA_ID"},
    {F_PNCTL,"pnctL"},
    {F_PNCTR,"pnctR"},
    {F_TREEDEP,"dep"},
    {F_POST,"pos"},
    {F_ANIM,"anim"},
    {F_GNDR,"gndr"},
    {F_NMBR,"nmbr"},
    {F_CASE,"case"},
    {F_ASPC,"aspc"},
    {F_TRNS,"trns"},
    {F_PERS,"pers"},
    {F_TENS,"tens"},
    {F_MOOD,"mood"},
    {F_INVL,"invl"},
    {F_VOIC,"voic"},
    {F_UNCERTAIN,"UNCERTAIN"}
};

map <string, feature> opcorpFeatMap={
	{ "NOUN" , F_POST },
	{ "ADJF" , F_POST },
	{ "ADJS" , F_POST },
	{ "COMP" , F_POST },
	{ "VERB" , F_POST },
	{ "INFN" , F_POST },
	{ "PRTF" , F_POST },
	{ "PRTS" , F_POST },
	{ "GRND" , F_POST },
	{ "NUMR" , F_POST },
	{ "ADVB" , F_POST },
	{ "NPRO" , F_POST },
	{ "PRED" , F_POST },
	{ "PREP" , F_POST },
	{ "CONJ" , F_POST },
	{ "PRCL" , F_POST },
	{ "INTJ" , F_POST },
	{ "anim" , F_ANIM },
	{ "inan" , F_ANIM },
	{ "masc" , F_GNDR },
	{ "femn" , F_GNDR },
	{ "neut" , F_GNDR },
	{ "Ms-f" , F_GNDR },
	{ "sing" , F_NMBR },
	{ "plur" , F_NMBR },
	{ "nomn" , F_CASE },
	{ "gent" , F_CASE },
	{ "datv" , F_CASE },
	{ "accs" , F_CASE },
	{ "ablt" , F_CASE },
	{ "loct" , F_CASE },
	{ "voct" , F_CASE },
	{ "gen1" , F_CASE },
	{ "gen2" , F_CASE },
	{ "acc2" , F_CASE },
	{ "loc1" , F_CASE },
	{ "loc2" , F_CASE },
	{ "perf" , F_ASPC },
	{ "impf" , F_ASPC },
	{ "tran" , F_TRNS },
	{ "intr" , F_TRNS },
	{ "1per" , F_PERS },
	{ "2per" , F_PERS },
	{ "3per" , F_PERS },
	{ "pres" , F_TENS },
	{ "past" , F_TENS },
	{ "futr" , F_TENS },
	{ "indc" , F_MOOD },
	{ "impr" , F_MOOD },
	{ "incl" , F_INVL },
	{ "excl" , F_INVL },
	{ "actv" , F_VOIC },
	{ "pssv" , F_VOIC },
	{ "PNCT" , F_PNCT }

};
