#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

#include "fuzzy.h"

int main(int argc, char** argv)
{

	if (argc != 4) {
		printf("Formato: ./Classif <List_Path> <Rule_Path> <T_norm>\n");
		return -1;
	}
	
	//Cargar lista de valores y pasar a vector 2D------------------------------------------------------------
	vector <vector <string>> lista;
	vector <vector <string>> reglas;
		
	lista = load_csv(argv[1]);
	
	reglas = load_csv(argv[2]);
	
	string st_norm = argv[3];
	
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

	
	/*//Crear e imprimir datos de entrada
	cout<<"\nConjunto largo de sépalo: ";
	sepLenRul.newSets(XSLmin, XSLmax, 5, 0.25);
	cout<<"Conjunto ancho de sépalo: ";
	sepWidRul.newSets(XSWmin, XSWmax, 5, 0.25);
	cout<<"Conjunto largo de pétalo: ";
	petLenRul.newSets(XPLmin, XPLmax, 5, 0.25);
	cout<<"Conjunto ancho de pétalo: ";
	petWidRul.newSets(XPWmin, XPWmax, 5, 0.25);
	cout<<endl;*/
		
	for (int j = 0; j < lista.size(); j++)
	{		
		flowClass tipFlor;
		
		sepLenRul.initialize();
		sepWidRul.initialize();
		petLenRul.initialize();
		petWidRul.initialize();
	
		double sepLen = stof(lista[j][0]);
		double sepWid = stof(lista[j][1]);
		double petLen = stof(lista[j][2]);
		double petWid = stof(lista[j][3]);
		
		/*cout<<sepLen<<endl;
		cout<<sepWid<<endl;
		cout<<petLen<<endl;
		cout<<petWid<<endl;*/
		
		//Encontrar los valores de mu para cada uno de los conjuntos
		vector <vector <double>> muSepLen = muSets (sepLenRul.setsVec, sepLenRul.setsY, sepLen);
		vector <vector <double>> muSepWid = muSets (sepWidRul.setsVec, sepLenRul.setsY, sepWid);
		vector <vector <double>> muPetLen = muSets (petLenRul.setsVec, sepLenRul.setsY, petLen);
		vector <vector <double>> muPetWid = muSets (petWidRul.setsVec, sepLenRul.setsY, petWid);
		
		sepLenRul.rulVal(muSepLen, sepLenRul.setsVec);
		
		sepWidRul.rulVal(muSepWid, sepWidRul.setsVec);
		
		petLenRul.rulVal(muPetLen, petLenRul.setsVec);
	
		petWidRul.rulVal(muPetWid, petWidRul.setsVec);
		
		// Fuzzy Rules
		for (int k = 0; k < reglas.size(); ++k)
		{
		
			double aux_sepLenRul = 0;
			double aux_sepWidRul = 0;
			double aux_petLenRul = 0;
			double aux_petWidRul = 0;
			
			int cond = stoi(reglas[k][0]);
			
			switch (cond) {
				case 0:
					aux_sepLenRul = 1;
					break;
				case 1:
					aux_sepLenRul = sepLenRul.s2;
					break;
				case 2:
					aux_sepLenRul = sepLenRul.l2;
					break;
				case 3:
					aux_sepLenRul = sepLenRul.s3;
					break;
				case 4:
					aux_sepLenRul = sepLenRul.m3;
					break;
				case 5:
					aux_sepLenRul = sepLenRul.l3;
					break;
				case 6:
					aux_sepLenRul = sepLenRul.s4;
					break;
				case 7:
					aux_sepLenRul = sepLenRul.ms4;
					break;
				case 8:
					aux_sepLenRul = sepLenRul.ml4;
					break;
				case 9:
					aux_sepLenRul = sepLenRul.l4;
					break;
				case 10:
					aux_sepLenRul = sepLenRul.s5;
					break;
				case 11:
					aux_sepLenRul = sepLenRul.ms5;
					break;
				case 12:
					aux_sepLenRul = sepLenRul.m5;
					break;
				case 13:
					aux_sepLenRul = sepLenRul.ml5;
					break;
				case 14:
					aux_sepLenRul = sepLenRul.l5;
					break;
				default:
					aux_sepLenRul = sepLenRul.l5;
					break;
			}
			cond = stoi(reglas[k][1]);
			
			switch (cond) {			
				case 0:
					aux_sepWidRul = 1;
					break;
				case 1:
					aux_sepWidRul = sepWidRul.s2;
					break;
				case 2:
					aux_sepWidRul = sepWidRul.l2;
					break;
				case 3:
					aux_sepWidRul = sepWidRul.s3;
					break;
				case 4:
					aux_sepWidRul = sepWidRul.m3;
					break;
				case 5:
					aux_sepWidRul = sepWidRul.l3;
					break;
				case 6:
					aux_sepWidRul = sepWidRul.s4;
					break;
				case 7:
					aux_sepWidRul = sepWidRul.ms4;
					break;
				case 8:
					aux_sepWidRul = sepWidRul.ml4;
					break;
				case 9:
					aux_sepWidRul = sepWidRul.l4;
					break;
				case 10:
					aux_sepWidRul = sepWidRul.s5;
					break;
				case 11:
					aux_sepWidRul = sepWidRul.ms5;
					break;
				case 12:
					aux_sepWidRul = sepWidRul.m5;
					break;
				case 13:
					aux_sepWidRul = sepWidRul.ml5;
					break;
				case 14:
					aux_sepWidRul = sepWidRul.l5;
					break;
				default:
					aux_sepWidRul = sepWidRul.l5;
					break;
			}			
			cond = stoi(reglas[k][2]);
						
			switch (cond) {			
				case 0:
					aux_petLenRul = 1;
					break;
				case 1:
					aux_petLenRul = petLenRul.s2;
					break;
				case 2:
					aux_petLenRul = petLenRul.l2;
					break;
				case 3:
					aux_petLenRul = petLenRul.s3;
					break;
				case 4:
					aux_petLenRul = petLenRul.m3;
					break;
				case 5:
					aux_petLenRul = petLenRul.l3;
					break;
				case 6:
					aux_petLenRul = petLenRul.s4;
					break;
				case 7:
					aux_petLenRul = petLenRul.ms4;
					break;
				case 8:
					aux_petLenRul = petLenRul.ml4;
					break;
				case 9:
					aux_petLenRul = petLenRul.l4;
					break;
				case 10:
					aux_petLenRul = petLenRul.s5;
					break;
				case 11:
					aux_petLenRul = petLenRul.ms5;
					break;
				case 12:
					aux_petLenRul = petLenRul.m5;
					break;
				case 13:
					aux_petLenRul = petLenRul.ml5;
					break;
				case 14:
					aux_petLenRul = petLenRul.l5;
					break;
				default:
					aux_petLenRul = petLenRul.l5;
					break;
			}
			cond = stoi(reglas[k][3]);
			
			switch (cond) {			
				case 0:
					aux_petWidRul = 1;
					break;
				case 1:
					aux_petWidRul = petWidRul.s2;
					break;
				case 2:
					aux_petWidRul = petWidRul.l2;
					break;
				case 3:
					aux_petWidRul = petWidRul.s3;
					break;
				case 4:
					aux_petWidRul = petWidRul.m3;
					break;
				case 5:
					aux_petWidRul = petWidRul.l3;
					break;
				case 6:
					aux_petWidRul = petWidRul.s4;
					break;
				case 7:
					aux_petWidRul = petWidRul.ms4;
					break;
				case 8:
					aux_petWidRul = petWidRul.ml4;
					break;
				case 9:
					aux_petWidRul = petWidRul.l4;
					break;
				case 10:
					aux_petWidRul = petWidRul.s5;
					break;
				case 11:
					aux_petWidRul = petWidRul.ms5;
					break;
				case 12:
					aux_petWidRul = petWidRul.m5;
					break;
				case 13:
					aux_petWidRul = petWidRul.ml5;
					break;
				case 14:
					aux_petWidRul = petWidRul.l5;
					break;
				default:
					aux_petWidRul = petWidRul.l5;
					break;
			}
			ruleApply (aux_sepLenRul, aux_sepWidRul, aux_petLenRul, aux_petWidRul, tipFlor, reglas[k][4], st_norm, j);
		}
		/*else
			cout<<"No es posible clasificar."<<endl;*/
			
		tipFlor.gralRule();
		
		cout<<j<<":  "<<tipFlor.resultado<<"  "<<tipFlor.classification<<endl;

	}
		
	return 0;
}
