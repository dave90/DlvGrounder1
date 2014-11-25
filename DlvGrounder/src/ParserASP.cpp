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


void addChoiche() {
	builder->addChoiche();
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

void setBinop(string& binop){
	builder->setBinop(binop);
}

void addBuiltinAtom(){
	builder->addBuiltinAtom();
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

void addNumber(unsigned int &name) {
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

void addNegativeTerm(string& minus) {
	builder->setNegativeTerm();
}

void addArithTerm() {
	builder->addArithTerm();
}

void addArithTermAndPopTerm(){
	builder->removeLastTerm();
	builder->addArithTerm();
}

void setArithOperator(string& op) {
	builder->setArithOperator(op);
}

void endArithTerm() {
	builder->endArithTerm();
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

		using qi::uint_;
		using qi::char_;
		using qi::lit;
		using qi::alnum;
		using qi::lexeme;
		using qi::eol;

		statements = *statement[&client::addStatement];

		statement = (CONS[&client::setRuleBody] >> -body >> DOT[&client::addRule])
				| (head >> -(CONS[&client::setRuleBody] >> -body) >> DOT[&client::addRule])
				| (WCONS >> -body >> DOT >> SQUARE_OPEN >> weight_at_level
						>> SQUARE_CLOSE[&client::addWeak]) | (optimize >> DOT) | ( PERCENTAGE >> COMMENT);

		head = disjunction | choice[&client::addChoiche];

		choice = -(term_arithTerm >> binop) >> CURLY_OPEN >> choiche_elements >> CURLY_CLOSE
				>> -(binop >> term_arithTerm);

		choiche_elements = choiche_element % SEMICOLON;

		choiche_element = classical_literal >> -(COLON >> naf_litterals);

		disjunction = classical_literal % OR;

		body = (naf_litteral | -NAF >> aggregate[&client::addAggregate]) % COMMA;

		aggregate = -(term_arithTerm >> binop) >> aggregate_function >> CURLY_OPEN >> aggregate_elements
				>> CURLY_CLOSE >> -(binop >> term_arithTerm);

		aggregate_function = AGGREGATE_COUNT | AGGREGATE_MAX | AGGREGATE_MIN | AGGREGATE_SUM;

		aggregate_elements = aggregate_element % SEMICOLON;

		aggregate_element = terms >> -(COLON >> -naf_litterals);

		weight_at_level = term_arithTerm >> -(AT >> term_arithTerm) >> -(COMMA >> term_arithTerm);

		optimize = optimize_function >> CURLY_OPEN >> -optimize_elements >> CURLY_CLOSE;

		optimize_elements = optimize_element % SEMICOLON;

		optimize_element = weight_at_level >> -(COLON >> -naf_litterals);

		optimize_function = MAXIMIZE | MINIMIZE;

		naf_litterals = naf_litteral % COMMA;

		naf_litteral = (-NAF[&client::setNegativeAtom] >> classical_literal >> !binop)
				| builtin_atom;

		builtin_atom = (term_arithTerm >> binop >> term_arithTerm )[&client::addBuiltinAtom];

		binop = EQUAL[&client::setBinop] | UNEQEUAL1[&client::setBinop] | UNEQEUAL2[&client::setBinop]| LESS[&client::setBinop] >> !EQUAL | GREATER[&client::setBinop] >> !EQUAL | LESS_OR_EQ[&client::setBinop] | GREATER_OR_EQ[&client::setBinop];

		classical_literal = -MINUS[&client::setStrongNegativeAtom] >> ID[&client::addLiteral]
				>> (-(PAREN_OPEN >> terms >> PAREN_CLOSE))[&client::addClassicalAtom];

		terms = term_arithTerm % COMMA;

		term_arithTerm = (term >> !not_arithop[&client::addArithTermAndPopTerm] | arithTerm[&client::endArithTerm] );

		arithTerm = ( term >> *(arithop >> term )   );

		term = -MINUS[&client::addNegativeTerm]
				>> ((ID[&client::addNameFunction] >> PAREN_OPEN[&client::addFunctionTerm] >> terms
						>> PAREN_CLOSE[&client::endFunctionTerm]) | ID[&client::addId]
						| NUMBER[&client::addNumber] | VARIABLE[&client::addVariable]
						| ANONYMOUS_VARIABLE[&client::addAnonymusVariable] | STRING[&client::addId]);


		arithop = PLUS[&client::setArithOperator] | MINUS[&client::setArithOperator]
				| TIMES[&client::setArithOperator] | DIV[&client::setArithOperator];

		not_arithop = PLUS | MINUS	| TIMES | DIV;


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
		UNEQEUAL1 = lit("!=");
		UNEQEUAL2 = lit("<>");
		LESS = lit("<");
		GREATER = lit(">");
		LESS_OR_EQ = lit("<=");
		GREATER_OR_EQ = lit(">=");
		NAF = lexeme[lit("not ")] | lexeme[lit("not") > qi::eol];
		NUMBER = uint_;
		STRING = lexeme[char_("\"") > +(char_ - char_("\"")) > char_("\"")];
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
		COMMENT = lexeme[+(char_ - qi::eol) > qi::eol];
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
	qi::rule<Iterator, ascii::space_type> term_arithTerm;
	qi::rule<Iterator, ascii::space_type> terms;
	qi::rule<Iterator, ascii::space_type> arithop;
	qi::rule<Iterator, ascii::space_type> not_arithop;
	qi::rule<Iterator, ascii::space_type> arithTerm;
	qi::rule<Iterator, ascii::space_type> term;
	qi::rule<Iterator, string(), ascii::space_type> COMMA, PAREN_OPEN, PAREN_CLOSE, MINUS, ID, OR,
			DOT, NAF, SEMICOLON, EQUAL, UNEQEUAL1, UNEQEUAL2, LESS, GREATER, LESS_OR_EQ, GREATER_OR_EQ, CONS,
			COLON, AT, VARIABLE, ANONYMOUS_VARIABLE, PLUS, TIMES, DIV, STRING, CURLY_OPEN,
			CURLY_CLOSE, AGGREGATE_COUNT, AGGREGATE_MAX, AGGREGATE_MIN, AGGREGATE_SUM, SQUARE_OPEN,
			SQUARE_CLOSE, WCONS, MAXIMIZE, MINIMIZE, PERCENTAGE,COMMENT;
	qi::rule<Iterator, unsigned int(), ascii::space_type> NUMBER;
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

		ValueArg<string> indexingTypeArg("z", "indexType", "Index type", false, "DEFAULT", "string");
		cmd.add(&indexingTypeArg);

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

		SwitchArg printRulesArgs("r", "printRules", "Print the rules of the program",
				false);
		cmd.add(&printRulesArgs);

		ValueArg<string> indexingPreferencesArg("i", "index", "Set on which term predicates have to be indexed", false, "", "string");
		cmd.add(&indexingPreferencesArg);

		SwitchArg disabeSimpleArgs("m", "noSImpli", "Disable the simplifiation", false);
		cmd.add(&disabeSimpleArgs);


		//MUST be last
		UnlabeledValueArg<string> fileArg("fileName", "program file", false, "", "file");
		cmd.add(fileArg);

		// Parse the args.
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		string termTable = termTableArg.getValue();
		string hash = hashArg.getValue();
		string indexType=indexingTypeArg.getValue();
		bool parser = parseArgs.getValue();
		bool dependency = dependencyArgs.getValue();
		bool component = componentArgs.getValue();
		bool statistic = statisticArgs.getValue();
		string fileGraph = fileGraphArg.getValue();
		bool printRule=printRulesArgs.getValue();
		string indexing=indexingPreferencesArg.getValue();
		bool disableSimplification=disabeSimpleArgs.getValue();
		filename = fileArg.getValue();
		if (stdInArgs.getValue())
			filename = "";

		Config::getInstance()->setTermTableType(termTable);
		Config::getInstance()->setHashType(hash);
		Config::getInstance()->setIndexType(indexType);
		Config::getInstance()->setParser(parser);
		Config::getInstance()->setDependency(dependency);
		Config::getInstance()->setComponent(component);
		Config::getInstance()->setStatistic(statistic);
		Config::getInstance()->setFileGraph(fileGraph);
		Config::getInstance()->setPrintRules(printRule);
		Config::getInstance()->setIndexingPreferences(indexing);
		Config::getInstance()->setDisableSimplification(disableSimplification);

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
		while (std::getline(std::cin , buffer)){if(buffer[0]!='%')*str+=buffer;}
		// Add end line for a comment
		*str+='\n';
		iter = str->begin();
		end = str->end();
	} else {
		ifstream ifs(nameFile);
		if(!ifs.is_open()){cout<<"File "<<nameFile<<" not exist"<<endl; return 0;}
		str=new string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		// Add end line for a comment
		*str+='\n';
		iter = str->begin();
		end = str->end();
	}
	Timer::getInstance()->start("Parse time");
	asp_parser parser;
	bool r = phrase_parse(iter, end, parser, ascii::space);
	Timer::getInstance()->end();

	if (r && iter == end) {

		if (Config::getInstance()->isParser()) {

			cout << "-------------------------\n";
			cout << "Parsing succeeded\n";
			cout << "-------------------------\n";

		}

		delete str;
		ProgramGrounder grounder(client::builder->getPredicateTable(),
				client::builder->getInstanceTable(), client::builder->getStatementDependency());

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


	client::builder->printStats();

	delete client::builder;

	return 0;
}
