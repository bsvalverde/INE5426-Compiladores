//classe genérica para declaração dos enums utilizados
#pragma once

enum Type {
	desconhecido,
	inteiro,
	real,
	booleano,
	composto
};

enum BinOperation {
	plus,
	sub,
	mult,
	_div,
	gt,
	lt,
	gte,
	lte,
	eq,
	neq,
	_and,
	_or
};

enum UnOperation {
	_not,
	neg
};