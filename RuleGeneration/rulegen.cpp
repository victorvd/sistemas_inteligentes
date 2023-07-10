#define NR 15 // Rule sets Nrule per rule
#define NP 20 // Poblation Npop
#define Sym 14 // Number of symbols
#define Cla 3 // Number of classes
#define Ant 4 // Number of antecedents
#define Cros 10 // Crossover probability
#define Mut 4 // Mutation probability
#define Ite 1000 // Number of iterations

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

using namespace std;

#include "fuzzy.h"
#include "michigan.h"
#include "pittsburgh.h"


int main(int argc, char** argv)
{

	if (argc != 2) {
		printf("Formato: ./Classif <List_Path>\n");
		return -1;
	}
	
	//Cargar lista de valores y pasar a vector 2D------------------------------------------------------------
	vector <vector <string>> lista;
		
	lista = load_csv(argv[1]);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Crear conjuntos							|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	//Valores iniciales y finales
	double XSLmin = 4.3;
	double XSLmax = 7.91;
	double XSWmin = 2.0;
	double XSWmax = 4.41;
	double XPLmin = 1.0;
	double XPLmax = 6.91;
	double XPWmin = 0.1;
	double XPWmax = 2.51;
	
	flowSize sepLenRul, sepWidRul, petLenRul, petWidRul;
	
	//Crear e imprimir datos de entrada
	//cout<<"\nConjunto largo de sépalo: \n";
	sepLenRul.newSets(XSLmin, XSLmax, 2, 1);
	sepLenRul.newSets(XSLmin, XSLmax, 3, 0.5);
	sepLenRul.newSets(XSLmin, XSLmax, 4, 0.5);
	sepLenRul.newSets(XSLmin, XSLmax, 5, 0.5);

	//cout<<"\nConjunto ancho de sépalo: \n";
	sepWidRul.newSets(XSWmin, XSWmax, 2, 1);
	sepWidRul.newSets(XSWmin, XSWmax, 3, 0.5);
	sepWidRul.newSets(XSWmin, XSWmax, 4, 0.5);
	sepWidRul.newSets(XSWmin, XSWmax, 5, 0.5);

	//cout<<"\nConjunto largo de pétalo: \n";
	petLenRul.newSets(XPLmin, XPLmax, 2, 1);
	petLenRul.newSets(XPLmin, XPLmax, 3, 0.5);
	petLenRul.newSets(XPLmin, XPLmax, 4, 0.5);
	petLenRul.newSets(XPLmin, XPLmax, 5, 0.5);

	//cout<<"\nConjunto ancho de pétalo: \n";
	petWidRul.newSets(XPWmin, XPWmax, 2, 1);
	petWidRul.newSets(XPWmin, XPWmax, 3, 0.5);
	petWidRul.newSets(XPWmin, XPWmax, 4, 0.5);
	petWidRul.newSets(XPWmin, XPWmax, 5, 0.5);
	//cout<<endl;
	//-------------------------------------------------------------------------------

	//michigan(lista, sepLenRul, sepWidRul, petLenRul, petWidRul);
	pittsburgh(lista, sepLenRul, sepWidRul, petLenRul, petWidRul);

	return 0;

}
