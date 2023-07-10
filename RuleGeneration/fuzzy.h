#include <vector>
#include <cmath>

struct flowSize;

vector<vector<string>> load_csv(string path);

void save_csv(string path, vector <vector <auto>> &matriz);

double muValue(double x, double a, double b, int c, int d);

int rulSym(double X, flowSize &ranges);

double compGrade(vector <int> srule,
		double sepLen,
		double sepWid,
		double petLen,
		double petWid,
		flowSize* sepLenRul,
		flowSize* sepWidRul,
		flowSize* petLenRul,
		flowSize* petWidRul, int offset);

vector <int> rulgen(vector <vector <string>> &lista,
		flowSize &sepLenRul,
		flowSize &sepWidRul,
		flowSize &petLenRul,
		flowSize &petWidRul);

void display(vector<vector <int>> mat);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Clase para definir características				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct flowSize
{
	//Vectores con inicio y final de cada conjunto
	vector <double> setsVec = {};
	vector <int> setsY = {};

	void newSets(double Xmin, double Xmax, int sets, double pct)
	{
		
		if(sets == 2){
			setsVec.emplace_back(Xmin);
			setsVec.emplace_back(Xmin + (Xmax - Xmin) / (2 - pct));
			setsVec.emplace_back(Xmax - (Xmax - Xmin) / (2 - pct));
			setsVec.emplace_back(Xmax);
			
			setsY.emplace_back(1);
			setsY.emplace_back(0);
			setsY.emplace_back(0);
			setsY.emplace_back(1);
		}
		
		else if(sets > 2){
		
			double Xp = (Xmax - Xmin) / (sets - sets * pct + pct - 1); //Tamaño de cada antecedente
			
			setsVec.emplace_back(Xmin);
			setsVec.emplace_back(Xmin + 0.5 * Xp);
			
			setsY.emplace_back(1);
			setsY.emplace_back(0);
			
			int lastElem = setsVec.size();
			
			for (int i = lastElem; i < lastElem + 2 * sets - 2; i += 2) {
				setsVec.emplace_back(setsVec[i-1] - Xp * pct);
				setsVec.emplace_back(setsVec[i] + Xp);
				
				setsY.emplace_back(0);
				setsY.emplace_back(0);
			}
			
			setsVec[setsVec.size() - 1] -= 0.5 * Xp;
			setsY[setsY.size() - 1] = 1;
		}
	}
};


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Cargar archivo CSV								|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

vector<vector<string>> load_csv(string path)
{
	fstream file (path, ios::in);

	vector<vector<string>> lista;		//Variable para almacenar kernel
	vector<string> row;
	string line, word;
	
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();

			stringstream str(line);

			while(getline(str, word, ','))
			row.push_back(word.c_str());
			
			lista.push_back(row);
		}
		cout << "Archivo " << path << " cargado: ";
		cout << lista.size() << "x" << lista[0].size() << endl;
	}
	else{
		printf("No se puede abrir el archivo CSV\n");
		return {};
	}
	
	return lista;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Guardar archivo CSV								|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void save_csv(string path, vector <vector <auto>> &matriz)
{
	fstream file;
	file.open(path, ios_base::out);
	
	if(file.is_open())
	{
		for(int i = 0; i < matriz.size(); ++i)
		{
			for(int j = 0; j < matriz[0].size(); ++j)
			{
				file << matriz[i][j];
				
				if (j < matriz[0].size() - 1)
					file << ",";
			}
			
			file << endl;
		}

		file.close();
		
		cout << "Archivo " << path << " guardado: ";
		cout << matriz.size() << "x" << matriz[0].size() << endl;
	}
	
	else{
		printf("No se puede guardar el archivo CSV\n");
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Encontrar el valor de mu dentro un conjunto			|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
double muValue(double x, double a, double b, int c, int d)
{
	double mu;

	if (c==0 && d==0){
		if (x>=a && x<=(b+a)/2)
			mu = (2 * x - 2 * a) / (b - a);
		else if (x>(b+a)/2 && x<=b)
			mu = (2 * x - 2 * b) / (a - b);
		else
			mu = 0;
	}
	else{
		if (x>=a && x<=b)
			mu = c + (x - a) * (d - c) / (b - a);
		else
			mu = 0;

	}
		
	return mu;
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Encontrar el conjunto al que pertenecen				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int rulSym(double X, flowSize &ranges)
{
	int antec;
	double mu = 0;
	
	for (int i = 0; i < ranges.setsVec.size(); i+=2) {
		vector <double> row (2,0);
		if (X >= ranges.setsVec[i] && X < ranges.setsVec[i+1]){
			row[0] = i/2 + 1;
			row[1] = muValue(X, ranges.setsVec[i], ranges.setsVec[i+1], ranges.setsY[i], ranges.setsY[i+1]);
		}
		if (row[1] > mu){
			antec = row[0];
			mu = row[1];
		}
	}
	
	return antec;
	
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Generar antecedentes y clase al azar				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
vector <int> rulgen(vector <vector <string>> &lista,
		flowSize &sepLenRul,
		flowSize &sepWidRul,
		flowSize &petLenRul,
		flowSize &petWidRul)
{

	bool goodrule = false;
	
	vector <int> srule;

	while (!goodrule)
	{
		/*----Variables names-----
		uAqxp_CFq[0]->PrClass1
		uAqxp_CFq[1]->PrClass2
		uAqxp_CFq[2]->PrClass3
		uAqxp_CFq[3]->PrClassCq
		uAqxp_CFq[4]->PrClassNCq
		uAqxp_CFq[5]->CFq
		*/

		double uAqxp;
		vector <double> uAqxp_row (Ant,0);
		
		random_device rd;
		mt19937 gen(rd());	
		uniform_int_distribution<> disSym(0, Sym);
		
		for (int i = 0; i < Ant; ++i){
				
			srule.emplace_back(disSym(gen)); //Random rules values
		}

		for (int i = 0; i < lista.size(); ++i)
		{
			double sepLen = stof(lista[i][0]);
			double sepWid = stof(lista[i][1]);
			double petLen = stof(lista[i][2]);
			double petWid = stof(lista[i][3]);
			
			uAqxp = compGrade(srule, sepLen, sepWid, petLen, petWid,
						&sepLenRul, &sepWidRul, &petLenRul, &petWidRul, 0);
			
			uAqxp_row[0] += uAqxp;
			
			if (lista[i][4] == "Iris-setosa")
				uAqxp_row[1] += uAqxp;
			else if (lista[i][4] == "Iris-versicolor")
				uAqxp_row[2] += uAqxp;
			else if (lista[i][4] == "Iris-virginica")
				uAqxp_row[3] += uAqxp;
		}
		
		if (uAqxp_row[1] > uAqxp_row[2] && uAqxp_row[1] > uAqxp_row[3])
			srule.emplace_back(1);
		else if (uAqxp_row[2] > uAqxp_row[1] && uAqxp_row[2] > uAqxp_row[3])
			srule.emplace_back(2);
		else if (uAqxp_row[3] > uAqxp_row[1] && uAqxp_row[3] > uAqxp_row[2])
			srule.emplace_back(3);
		else
			srule.emplace_back(0);
		
		if (srule[4] == 0){
			goodrule = false;
			srule.clear();
			uAqxp_row = {0, 0, 0, 0};}
		else
			goodrule = true;
	
	} // End While

	return srule;
	
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Encontrar el grado de compatibilidad				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
double compGrade(vector <int> srule,
		double sepLen,
		double sepWid,
		double petLen,
		double petWid,
		flowSize* sepLenRul,
		flowSize* sepWidRul,
		flowSize* petLenRul,
		flowSize* petWidRul, int offset)
{
	
	int rule1 = srule[0 + offset] * 2;
	int rule2 = srule[1 + offset] * 2;
	int rule3 = srule[2 + offset] * 2;
	int rule4 = srule[3 + offset] * 2;

	double uAq1xp1, uAq2xp2, uAq3xp3, uAq4xp4;

	// Take into consideration the "don't care" antecedent
	if (srule[0 + offset] == 0)
		uAq1xp1 = 1;
	else
		uAq1xp1 = muValue(sepLen, sepLenRul->setsVec[rule1 - 2], sepLenRul->setsVec[rule1 - 1],
				 sepLenRul->setsY[rule1 - 2], sepLenRul->setsY[rule1 - 1]);
	if (srule[1 + offset] == 0)
		uAq2xp2 = 1;
	else
		uAq2xp2 = muValue(sepWid, sepWidRul->setsVec[rule2 - 2], sepWidRul->setsVec[rule2 - 1],
				 sepWidRul->setsY[rule2 - 2], sepWidRul->setsY[rule2 - 1]);
	if (srule[2 + offset] == 0)
		uAq3xp3 = 1;
	else
		uAq3xp3 = muValue(petLen, petLenRul->setsVec[rule3 - 2], petLenRul->setsVec[rule3 - 1],
				 petLenRul->setsY[rule3 - 2], petLenRul->setsY[rule3 - 1]);
	if (srule[3 + offset] == 0)
		uAq4xp4 = 1;
	else
		uAq4xp4 = muValue(petWid, petWidRul->setsVec[rule4 - 2], petWidRul->setsVec[rule4 - 1],
				 petWidRul->setsY[rule4 - 2], petWidRul->setsY[rule4 - 1]);
	
	double uAqxp = uAq1xp1 * uAq2xp2 * uAq3xp3 * uAq4xp4;
	
	return uAqxp;

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Display								|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void display(vector<vector <int>> mat)
{
	int n = mat.size();
	int m = mat[0].size();
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; ++j)
			cout << mat[i][j] << " ";
		cout << "\n";
	}
}
