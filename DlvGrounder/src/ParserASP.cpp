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

#include <tclap/CmdLine.h>
#include <tclap/SwitchArg.h>

#include <boost/spirit/include/qi.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#include "StatementBuilder.h"
#include "ground/ProgramGrounder.h"
#include "utility/Timer.h"
#include "utility/Config.h"

using namespace TCLAP;
using namespace std;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

/*
 *
 * Function called from the parser
 *
 */
namespace client {

/*
 * This functions forward the action to the StatementBuilder object
 */
StatementBuilder *builder;

void addStatement() {
	builder->addStatement();
}

void addWeak() {
	builder->addWeak();
}

void addConstraint() {
	builder->addConstraint();
}

void addChoiche() {
	builder->addChoiche();
}

void addDisjunction() {
	builder->addDisjunction();
}

void addAggregate() {
	builder->addAggregate();
}

void setRuleBody() {
	builder->setRuleBody();
}

void addRule() {
	builder->addRule();
}

void addClassicalAtom() {
	builder->addClassicalAtom();
}

void addLiteral(string & name) {
	builder->addLiteral(name);
}

void setNegativeAtom() {
	builder->setNegativeAtom();
}

void setStrongNegativeAtom() {
	builder->setStrongNegativeAtom();
}

void addVariable(string &name) {
	builder->addVariable(name);
}

void addAnonymusVariable(string& name) {
	builder->addVariable(name);
}

void addId(string &name) {
	builder->addId(name);
}

void addNumber(int &name) {
	builder->addNumber(name);
}

void addNameFunction(string &name) {
	builder->addNameFunction(name);
}

void addFunctionTerm() {
	builder->addTermFunction();
}

void endFunctionTerm() {
	builder->endTermFunction();
}

void addNegativeTerm(string minus) {
	builder->setNegativeTerm();
}

void addArithTerm(string &op) {
	builder->addArithTerm(op);
}

}

/*
 *
 * Definition of the grammar
 *
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

		statement = (CONS[&client::setRuleBody] >> -body >> DOT[&client::addRule])
				| (head >> -(CONS[&client::setRuleBody] >> -body) >> DOT[&client::addRule])
				| (WCONS >> -body >> DOT >> SQUARE_OPEN >> weight_at_level
						>> SQUARE_CLOSE[&client::addWeak]) | (optimize >> DOT);

		head = disjunction[&client::addDisjunction] | choice[&client::addChoiche];

		choice = -(term >> binop) >> CURLY_OPEN >> choiche_elements >> CURLY_CLOSE
				>> -(binop >> term);

		choiche_elements = choiche_element % SEMICOLON;

		choiche_element = classical_literal >> -(COLON >> naf_litterals);

		disjunction = classical_literal % OR;

		body = (naf_litteral | aggregate[&client::addAggregate]) % COMMA;

		aggregate = -(term >> binop) >> aggregate_function >> CURLY_OPEN >> aggregate_elements
				>> CURLY_CLOSE >> -(binop >> term);

		aggregate_function = AGGREGATE_COUNT | AGGREGATE_MAX | AGGREGATE_MIN | AGGREGATE_SUM;

		aggregate_elements = aggregate_element % SEMICOLON;

		aggregate_element = terms >> -(COLON >> -naf_litterals);

		weight_at_level = term >> -(AT >> term) >> -(COMMA >> terms);

		optimize = optimize_function >> CURLY_OPEN >> -optimize_elements >> CURLY_CLOSE;

		optimize_elements = optimize_element % SEMICOLON;

		optimize_element = weight_at_level >> -(COLON >> -naf_litterals);

		optimize_function = MAXIMIZE | MINIMIZE;

		naf_litterals = naf_litteral % COMMA;

		naf_litteral = (-NAF[&client::setNegativeAtom] >> classical_literal >> !binop)
				| builtin_atom;

		builtin_atom = term >> binop >> term;

		binop = EQUAL | UNEQEUAL | LESS >> !EQUAL | GREATER >> !EQUAL | LESS_OR_EQ | GREATER_OR_EQ;

		classical_literal = -MINUS[&client::setStrongNegativeAtom] >> ID[&client::addLiteral]
				>> (-(PAREN_OPEN >> terms >> PAREN_CLOSE))[&client::addClassicalAtom];

		terms = term % COMMA;

		term = -MINUS[&client::addNegativeTerm]
				>> ((ID[&client::addNameFunction] >> PAREN_OPEN[&client::addFunctionTerm] >> terms
						>> PAREN_CLOSE[&client::endFunctionTerm]) | ID[&client::addId]
						| NUMBER[&client::addNumber] | VARIABLE[&client::addVariable]
						| ANONYMOUS_VARIABLE[&client::addAnonymusVariable] | STRING[&client::addId])
				>> -arithop_term;

		arithop_term = arithop >> term;

		arithop = PLUS[&client::addArithTerm] | MINUS[&client::addArithTerm]
				| TIMES[&client::addArithTerm] | DIV[&client::addArithTerm];

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
		STRING = char_("\"") > +(char_ - char_("\"")) > char_("\"");
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
		MAXIMIZE = lit("#maximize");
		MINIMIZE = lit("#minimize");
		PERCENTAGE = lit("%");
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
	qi::rule<Iterator, string(), ascii::space_type> COMMA, PAREN_OPEN, PAREN_CLOSE, MINUS, ID, OR,
			DOT, NAF, SEMICOLON, EQUAL, UNEQEUAL, LESS, GREATER, LESS_OR_EQ, GREATER_OR_EQ, CONS,
			COLON, AT, VARIABLE, ANONYMOUS_VARIABLE, PLUS, TIMES, DIV, STRING, CURLY_OPEN,
			CURLY_CLOSE, AGGREGATE_COUNT, AGGREGATE_MAX, AGGREGATE_MIN, AGGREGATE_SUM, SQUARE_OPEN,
			SQUARE_CLOSE, WCONS, MAXIMIZE, MINIMIZE, PERCENTAGE;
	qi::rule<Iterator, int(), ascii::space_type> NUMBER;
};

typedef string::const_iterator string_const_it;
typedef boost::spirit::istream_iterator iter_file;
typedef asp_grammar<string_const_it> asp_parser;

bool parseArgs(int argc, char* argv[], string& filename) {
	try {

		// Define the command line object.
		CmdLine cmd("Command description message", ' ', "0.9");

		ValueArg<string> termTableArg("t", "termTable", "Term Table Type", false, "STL", "string");
		cmd.add(&termTableArg);

		ValueArg<string> hashArg("a", "hash", "Hash type", false, "STL_HASH", "string");
		cmd.add(&hashArg);

		SwitchArg parseArgs("p", "print", "Print parser result", false);
		cmd.add(&parseArgs);

		SwitchArg dependencyArgs("d", "dependency", "Print dependency graph", false);
		cmd.add(&dependencyArgs);

		SwitchArg componentArgs("c", "component", "Print component graph", false);
		cmd.add(&componentArgs);

		SwitchArg statisticArgs("s", "statistic", "Print statistic", false);
		cmd.add(&statisticArgs);

		ValueArg<string> fileGraphArg("f", "fileGraph", "File to print graphs", false, "",
				"string");
		cmd.add(&fileGraphArg);

		SwitchArg stdInArgs("1", "stdin", "Read input from stdin (if file not specified true)",
				false);
		cmd.add(&stdInArgs);

		//MUST be last
		UnlabeledValueArg<string> fileArg("fileName", "program file", false, "", "file");
		cmd.add(fileArg);

		// Parse the args.
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		string termTable = termTableArg.getValue();
		string hash = hashArg.getValue();
		bool parser = parseArgs.getValue();
		bool dependency = dependencyArgs.getValue();
		bool component = componentArgs.getValue();
		bool statistic = statisticArgs.getValue();
		string fileGraph = fileGraphArg.getValue();
		filename = fileArg.getValue();
		if (stdInArgs.getValue())
			filename = "";

		Config::getInstance()->setTermTableType(termTable);
		Config::getInstance()->setHashType(hash);
		Config::getInstance()->setParser(parser);
		Config::getInstance()->setDependency(dependency);
		Config::getInstance()->setComponent(component);
		Config::getInstance()->setStatistic(statistic);
		Config::getInstance()->setFileGraph(fileGraph);

	} catch (ArgException &e)  // catch any exceptions
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
		return false;
	}
	return true;
}

int main(int argc, char* argv[]) {
	string nameFile;

	if (!parseArgs(argc, argv, nameFile))
		return 0;

	client::builder = new StatementBuilder;

	string_const_it iter;
	string_const_it end;
	string* str;
	if (strcmp(nameFile.c_str(),"")==0) {
		str=new string;
		std::string buffer;
		while (std::cin >> buffer)*str+=buffer;
		iter = str->begin();
		end = str->end();

	} else {
		ifstream ifs(nameFile);
		str=new string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		iter = str->begin();
		end = str->end();
	}
	Timer::getInstance()->start("Parse time");

	asp_parser parser;
	bool r = phrase_parse(iter, end, parser, ascii::space);
	if (r && iter == end) {

		if (Config::getInstance()->isParser()) {

			cout << "-------------------------\n";
			cout << "Parsing succeeded\n";
			cout << "-------------------------\n";

		}
		delete str;

		ProgramGrounder grounder(client::builder->getPredicateTable(),
				client::builder->getInstanceTable(), client::builder->getStatementDependency(),
				client::builder->getTermTable());

		grounder.ground();

		grounder.print();

	} else {
		string rest(iter, end);
		cout << "-------------------------\n";
		cout << "Parsing failed\n";
//		cout << "stopped at: \": " << rest << "\"\n";
		cout << "-------------------------\n";

		delete str;
	}

	Timer::getInstance()->end();

	client::builder->printStats();

	delete client::builder;

	return 0;
}
