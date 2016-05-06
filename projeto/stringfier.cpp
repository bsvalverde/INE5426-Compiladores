#include "stringfier.h"

std::string Stringfier::typeStringM(Type type) {
	switch(type) {
		case desconhecido: return "desconhecido";
		case inteiro: return "inteiro";
		case real: return "real";
		case booleano: return "booleano";
	}
}

std::string Stringfier::typeStringF(Type type) {
	switch(type) {
		case desconhecido: return "desconhecida";
		case inteiro: return "inteira";
		case real: return "real";
		case booleano: return "booleana";
	}
}

std::string Stringfier::binOpString(BinOperation op) {
	switch(op) {
		case plus: return "soma";
		case sub: return "subtracao";
		case mult: return "multiplicacao";
		case _div: return "divisao";
		case gt: return "maior";
		case lt: return "menor";
		case gte: return "maior ou igual";
		case lte: return "menor ou igual";
		case eq: return "igual";
		case neq: return "diferente";
		case _and: return "e";
		case _or: return "ou";
		case assign: return "atribuicao";
	}
}

std::string Stringfier::unOpString(UnOperation op) {
	switch(op) {
		case decl: return "declaracao";
		case _not: return "negacao";
		case neg: return "negativo";
	}
}