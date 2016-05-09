#pragma once

enum Type {
	desconhecido,
	inteiro,
	real,
	booleano
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
	_or,
	assign
};

enum UnOperation {
	_not,
	neg
};