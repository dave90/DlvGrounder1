#include <iostream>

#include <boost/spirit/include/qi.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

using namespace std;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;



template<typename Iterator>
struct asp_grammar: qi::grammar<Iterator, ascii::space_type> {

	asp_grammar() :
			asp_grammar::base_type(statements) {

		using qi::int_;
		using qi::char_;
		using qi::lit;
		using qi::alnum;
		using qi::lexeme;

		statements = +statement;

		statement = (CONS >> -body >> DOT) | (head >> -(CONS >> -body) >> DOT)
				| (WCONS >> -body >> DOT >> SQUARE_OPEN >> weight_at_level
						>> SQUARE_CLOSE);

		head = disjunction | choice;

		choice = -(term >> binop) >> CURLY_OPEN >> choiche_elements
				>> CURLY_CLOSE >> -(binop >> term);

		choiche_elements = choiche_element % SEMICOLON;

		choiche_element = classical_litteral >> -(COLON >> naf_litterals);

		disjunction = classical_litteral % OR;

		body = (naf_litteral | aggregate) % COMMA;

		aggregate = -(term >> binop) >> aggregate_function >> CURLY_OPEN
				>> aggregate_elements >> CURLY_CLOSE >> -(binop >> term);

		aggregate_function = AGGREGATE_COUNT | AGGREGATE_MAX | AGGREGATE_MIN
				| AGGREGATE_SUM;

		aggregate_elements = aggregate_element % SEMICOLON;

		aggregate_element = terms >> -(COLON >> -naf_litterals);

		weight_at_level = term >> -(AT >> term) >> -(COMMA >> terms);

		naf_litterals = naf_litteral % COMMA;

		naf_litteral = (-NAF >> classical_litteral >> !binop) | builtin_atom;

		//BUILT e Classical iniziano con un termine.
		builtin_atom = term >> binop >> term;

		binop = EQUAL | UNEQEUAL | LESS >> !EQUAL | GREATER >> !EQUAL
				| LESS_OR_EQ | GREATER_OR_EQ;

		classical_litteral = -MINUS >> ID
				>> -(PAREN_OPEN >> terms >> PAREN_CLOSE);

		terms = term % COMMA;

		term = -MINUS
				>> (ID >> -(PAREN_OPEN >> terms >> PAREN_CLOSE) | NUMBER
						| VARIABLE | ANONYMOUS_VARIABLE) >> -arithop_term;

		arithop_term = arithop >> term;

		arithop = PLUS | MINUS | TIMES | DIV;

		COMMA = lit(",");
		PAREN_OPEN = lit("(");
		PAREN_CLOSE = lit(")");
		CONS = lit(":-");
		WCONS = lit(":~");
		MINUS = lit("-");
		ID = lexeme[char_("a-z") > *char_("a-zA-Z0-9_")];
		OR = lit("|");
		DOT = lit(".");
		EQUAL = lit("=");
		UNEQEUAL = lit("!=") | lit("<>");
		LESS = lit("<");
		GREATER = lit(">");
		LESS_OR_EQ = lit("<=");
		GREATER_OR_EQ = lit(">=");
		NAF = lit("not");
		NUMBER = int_;
		STRING = char_("\"") > +char_ - char_("\"") > char_("\"");
		VARIABLE = lexeme[char_("A-Z") > *char_("a-zA-Z0-9_")];
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
	}

	qi::rule<Iterator, ascii::space_type> program;
	qi::rule<Iterator, ascii::space_type> statements;
	qi::rule<Iterator, ascii::space_type> statement;
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
	qi::rule<Iterator, ascii::space_type> body;
	qi::rule<Iterator, ascii::space_type> naf_litteral;
	qi::rule<Iterator, ascii::space_type> naf_litterals;
	qi::rule<Iterator, ascii::space_type> builtin_atom;
	qi::rule<Iterator, ascii::space_type> binop;
	qi::rule<Iterator, ascii::space_type> classical_litteral;
	qi::rule<Iterator, ascii::space_type> terms;
	qi::rule<Iterator, ascii::space_type> arithop;
	qi::rule<Iterator, ascii::space_type> arithop_term;
	qi::rule<Iterator, ascii::space_type> term;
	qi::rule<Iterator, ascii::space_type> COMMA, PAREN_OPEN, PAREN_CLOSE, MINUS,
			ID, OR, DOT, NAF, NUMBER, SEMICOLON, EQUAL, UNEQEUAL, LESS, GREATER,
			LESS_OR_EQ, GREATER_OR_EQ, CONS, COLON, AT, VARIABLE,
			ANONYMOUS_VARIABLE, PLUS, TIMES, DIV, STRING, CURLY_OPEN,
			CURLY_CLOSE, AGGREGATE_COUNT, AGGREGATE_MAX, AGGREGATE_MIN,
			AGGREGATE_SUM, SQUARE_OPEN, SQUARE_CLOSE, WCONS;

};
