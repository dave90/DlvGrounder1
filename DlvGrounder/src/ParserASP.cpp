//============================================================================
// Name        : ParserASP.cpp
// Author      : Dave
// Version     :
// Copyright   : Dave Popyright
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <fstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#include "StatementBuilder.h"

using namespace std;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

/*

 Function called from the parser

 */
namespace client {

StatementBuilder builder;


void addStatement() {
	builder.addStatement();
}

void addWeak() {
	builder.addWeak();
}

void addConstraint() {
	builder.addConstraint();
}

void addChoiche() {
	builder.addChoiche();
}

void addDisjunction() {
	builder.addDisjunction();
}

void addAggregate() {
	builder.addAggregate();
}

void addLiteral() {
	builder.addLiteral();
}

void addTerm() {
	builder.addTerm();
}

void addVariable(const char &c){
	builder.addVariable(c);
}

void addAnonymusVariable(){
	builder.addVariable('_');
}

void addId(const char& c) {
	builder.addId(c);
}

void addNumber(const int n) {
	builder.addNumber(n);
}

void addFunctionTerm(){
	builder.addTermFunction();
}

void endFunctionTerm(){
	builder.endTermFunction();
}

void addNegativeTerm(){
	builder.setNegativeTerm();
}

}

/*

 Definition of the grammar

 */
template<typename Iterator>
struct asp_grammar: qi::grammar<Iterator, ascii::space_type> {

	asp_grammar() :
			asp_grammar::base_type(statements) {

		using qi::int_;
		using qi::char_;
		using qi::lit;
		using qi::alnum;
		using qi::lexeme;
		using qi::eol;

		statements = +statement[&client::addStatement];

		statement = (CONS >> -body >> DOT[&client::addConstraint])
				| (head >> -(CONS >> -body) >> DOT)
				| (WCONS >> -body >> DOT >> SQUARE_OPEN >> weight_at_level
						>> SQUARE_CLOSE[&client::addWeak]) | (optimize >> DOT)  ;


		head = disjunction[&client::addDisjunction]
				| choice[&client::addChoiche];

		choice = -(term >> binop) >> CURLY_OPEN >> choiche_elements
				>> CURLY_CLOSE >> -(binop >> term);

		choiche_elements = choiche_element % SEMICOLON;

		choiche_element = classical_literal >> -(COLON >> naf_litterals);

		disjunction = classical_literal % OR;

		//attenzione binop presente entrambi
		body = (naf_litteral | aggregate[&client::addAggregate]) % COMMA;

		aggregate = -(term >> binop) >> aggregate_function >> CURLY_OPEN
				>> aggregate_elements >> CURLY_CLOSE >> -(binop >> term);

		aggregate_function = AGGREGATE_COUNT | AGGREGATE_MAX | AGGREGATE_MIN
				| AGGREGATE_SUM;

		aggregate_elements = aggregate_element % SEMICOLON;

		aggregate_element = terms >> -(COLON >> -naf_litterals);

		weight_at_level = term >> -(AT >> term) >> -(COMMA >> terms);

		optimize = optimize_function >> CURLY_OPEN >> -optimize_elements
				>> CURLY_CLOSE;

		optimize_elements = optimize_element % SEMICOLON;

		optimize_element = weight_at_level >> -(COLON >> -naf_litterals);

		optimize_function = MAXIMIZE | MINIMIZE;

		naf_litterals = naf_litteral % COMMA;

		naf_litteral = (-NAF >> classical_literal >> !binop) | builtin_atom;

		builtin_atom = term >> binop >> term;

		binop = EQUAL | UNEQEUAL | LESS >> !EQUAL | GREATER >> !EQUAL
				| LESS_OR_EQ | GREATER_OR_EQ;

		classical_literal = -MINUS >> ID[&client::addLiteral]
				>> -(PAREN_OPEN >> terms >> PAREN_CLOSE);

		terms = term % COMMA;

		term = -MINUS[&client::addNegativeTerm]
				>> (ID>> -(PAREN_OPEN[&client::addFunctionTerm] >> terms >> PAREN_CLOSE[&client::endFunctionTerm]) | NUMBER
						| VARIABLE | ANONYMOUS_VARIABLE[&client::addAnonymusVariable] | STRING)[&client::addTerm]
				>> -arithop_term;

		arithop_term = arithop >> term;

		arithop = PLUS | MINUS | TIMES | DIV;

		COMMA = lit(",");
		PAREN_OPEN = lit("(");
		PAREN_CLOSE = lit(")");
		CONS = lit(":-");
		WCONS = lit(":~");
		MINUS = lit("-");
		ID = lexeme[char_("a-z")[&client::addId] > *char_("a-zA-Z0-9_")[&client::addId]];
		OR = lit("|");
		DOT = lit(".");
		EQUAL = lit("=");
		UNEQEUAL = lit("!=") | lit("<>");
		LESS = lit("<");
		GREATER = lit(">");
		LESS_OR_EQ = lit("<=");
		GREATER_OR_EQ = lit(">=");
		NAF = lit("not");
		NUMBER = int_[&client::addNumber];
		STRING = char_("\"")[&client::addId] > +(char_ - char_("\""))[&client::addId] > char_("\"")[&client::addId];
		VARIABLE = lexeme[char_("A-Z")[&client::addVariable] > *char_("a-zA-Z0-9_")[&client::addVariable]];
		ANONYMOUS_VARIABLE = lit("_");
		PLUS = lit("+");
		TIMES = lit("*");
		DIV = lit("/");
		CURLY_OPEN = lit("{");
		CURLY_CLOSE = lit("}");
		SEMICOLON = lit(";");
		SQUARE_OPEN = lit("[");
		SQUARE_CLOSE = lit("]");
		COLON = lit(":");
		AGGREGATE_COUNT = lit("#count");
		AGGREGATE_MAX = lit("#max");
		AGGREGATE_MIN = lit("#min");
		AGGREGATE_SUM = lit("#sum");
		AT = lit("@");
		MAXIMIZE = lit("#maximize");
		MINIMIZE = lit("#minimize");
		PERCENTAGE=lit("%");
	}

	qi::rule<Iterator, ascii::space_type> program;
	qi::rule<Iterator, ascii::space_type> statements;
	qi::rule<Iterator, ascii::space_type> statement;
	qi::rule<Iterator, ascii::space_type> comment;
	qi::rule<Iterator, ascii::space_type> head;
	qi::rule<Iterator, ascii::space_type> disjunction;
	qi::rule<Iterator, ascii::space_type> choice;
	qi::rule<Iterator, ascii::space_type> choiche_elements;
	qi::rule<Iterator, ascii::space_type> choiche_element;
	qi::rule<Iterator, ascii::space_type> aggregate;
	qi::rule<Iterator, ascii::space_type> aggregate_function;
	qi::rule<Iterator, ascii::space_type> aggregate_elements;
	qi::rule<Iterator, ascii::space_type> aggregate_element;
	qi::rule<Iterator, ascii::space_type> weight_at_level;
	qi::rule<Iterator, ascii::space_type> optimize;
	qi::rule<Iterator, ascii::space_type> optimize_elements;
	qi::rule<Iterator, ascii::space_type> optimize_element;
	qi::rule<Iterator, ascii::space_type> optimize_function;
	qi::rule<Iterator, ascii::space_type> body;
	qi::rule<Iterator, ascii::space_type> naf_litteral;
	qi::rule<Iterator, ascii::space_type> naf_litterals;
	qi::rule<Iterator, ascii::space_type> builtin_atom;
	qi::rule<Iterator, ascii::space_type> binop;
	qi::rule<Iterator, ascii::space_type> classical_literal;
	qi::rule<Iterator, ascii::space_type> terms;
	qi::rule<Iterator, ascii::space_type> arithop;
	qi::rule<Iterator, ascii::space_type> arithop_term;
	qi::rule<Iterator, ascii::space_type> term;
	qi::rule<Iterator, ascii::space_type> COMMA, PAREN_OPEN, PAREN_CLOSE, MINUS,
			ID, OR, DOT, NAF, NUMBER, SEMICOLON, EQUAL, UNEQEUAL, LESS, GREATER,
			LESS_OR_EQ, GREATER_OR_EQ, CONS, COLON, AT, VARIABLE,
			ANONYMOUS_VARIABLE, PLUS, TIMES, DIV, STRING, CURLY_OPEN,
			CURLY_CLOSE, AGGREGATE_COUNT, AGGREGATE_MAX, AGGREGATE_MIN,
			AGGREGATE_SUM, SQUARE_OPEN, SQUARE_CLOSE, WCONS, MAXIMIZE, MINIMIZE,PERCENTAGE;

};



struct Term_hash
    : std::unary_function<VariableTerm, std::size_t>
{
    std::size_t operator()(Term * const& p) const
    {

    	long index=p->getIndex();
        std::size_t seed = index;
        return seed;
    }
};

int main() {

	ifstream ifs("test.txt");
	string str((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

	string::const_iterator iter = str.begin();
	string::const_iterator end = str.end();

	asp_grammar<string::const_iterator> parser;
	bool r = phrase_parse(iter, end, parser, ascii::space);
	if (r && iter == end) {
		cout << "-------------------------\n";
		cout << "Parsing succeeded\n";
		cout << "-------------------------\n";
	} else {
		string rest(iter, end);
		cout << "-------------------------\n";
		cout << "Parsing failed\n";
		cout << "stopped at: \": " << rest << "\"\n";
		cout << "-------------------------\n";
	}

	client::builder.printStats();

	return 0;
}
