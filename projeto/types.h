#pragma once

enum Type {
	inteiro,
	real,
	booleano
};

class TypeStringfier{
public:
	static std::string typeStringM(Type type){
		std::string retorno = "";
		switch(type){
			case inteiro:
				retorno = "inteiro";
				break;
			case real:
				retorno = "real";
				break;
			case booleano:
				retorno = "booleano";
				break;
		}
		return retorno;
	}

	static std::string typeStringF(Type type){
		std::string retorno = "";
		switch(type){
			case inteiro:
				retorno = "inteira";
				break;
			case real:
				retorno = "real";
				break;
			case booleano:
				retorno = "booleana";
				break;
		}
		return retorno;
	}
};