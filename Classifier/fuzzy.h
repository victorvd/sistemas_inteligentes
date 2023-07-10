#include <vector>
#include <cmath>

struct flowSize
{
	double s2 = 0;
	double l2 = 0;
	double s3 = 0;
	double m3 = 0;
	double l3 = 0;
	double s4 = 0;
	double ms4 = 0;
	double ml4 = 0;
	double l4 = 0;
	double s5 = 0;
	double ms5 = 0;
	double m5 = 0;
	double ml5 = 0;
	double l5 = 0;
	
	//Vectores con inicio y final de cada conjunto
	vector <double> setsVec = {};
	vector <int> setsY = {};
	
	//nombre de la regla
	string name;
	
	//Crear conjuntos igualmente espaciados y traslapados
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
	
	void rulVal(const vector <vector <double>> &muVal, const vector <double> &X)
	{
		for (int i = 0; i < muVal.size(); ++i) {
			if (muVal[i][0] == X[0] && muVal[i][1] == X[1]) {
				s2 = muVal[i][2];
				name = "1";
			}
			if (muVal[i][0] == X[2] && muVal[i][1] == X[3]) {
				l2 = muVal[i][2];
				name = "2";
			}
			if (muVal[i][0] == X[4] && muVal[i][1] == X[5]) {
				s3 = muVal[i][2];
				name = "3";
			}
			if (muVal[i][0] == X[6] && muVal[i][1] == X[7]) {
				m3 = muVal[i][2];
				name = "4";
			}
			if (muVal[i][0] == X[8] && muVal[i][1] == X[9]) {
				l3 = muVal[i][2];
				name = "5";
			}
			if (muVal[i][0] == X[10] && muVal[i][1] == X[11]) {
				s4 = muVal[i][2];
				name = "6";
			}
			if (muVal[i][0] == X[12] && muVal[i][1] == X[13]) {
				ms4 = muVal[i][2];
				name = "7";
			}
			if (muVal[i][0] == X[14] && muVal[i][1] == X[15]) {
				ml4 = muVal[i][2];
				name = "8";
			}
			if (muVal[i][0] == X[16] && muVal[i][1] == X[17]) {
				l4 = muVal[i][2];
				name = "9";
			}
			if (muVal[i][0] == X[18] && muVal[i][1] == X[19]) {
				s5 = muVal[i][2];
				name = "10";
			}
			if (muVal[i][0] == X[20] && muVal[i][1] == X[21]) {
				ms5 = muVal[i][2];
				name = "11";
			}
			if (muVal[i][0] == X[22] && muVal[i][1] == X[23]) {
				m5 = muVal[i][2];
				name = "12";
			}
			if (muVal[i][0] == X[24] && muVal[i][1] == X[25]) {
				ml5 = muVal[i][2];
				name = "13";
			}
			if (muVal[i][0] == X[26] && muVal[i][1] == X[27]) {
				l5 = muVal[i][2];
				name = "14";
			}
		}
	}
	
	void initialize()
	{
		s2 = 0;
		l2 = 0;
		s3 = 0;
		m3 = 0;
		l3 = 0;
		s4 = 0;
		ms4 = 0;
		ml4 = 0;
		l4 = 0;
		s5 = 0;
		ms5 = 0;
		m5 = 0;
		ml5 = 0;
		l5 = 0;
	
		name.erase();
	}
};

struct flowClass
{
	vector <double> setosa = {};
	vector <double> versicolor = {};
	vector <double> virginica = {};
	
	double resultado = 0;
	vector <double> result_temp = {0, 0, 0};
	string classification;
	
	void winRule()
	{
		if (!setosa.empty())
		{
			for (int i = 0; i < setosa.size(); ++i)
			{
				if (setosa[i] > resultado){
					resultado = setosa[i];
					classification = "Iris-setosa";
				}
			}
		}
		
		if (!versicolor.empty())
		{
			for (int i = 0; i < versicolor.size(); ++i)
			{
				if (versicolor[i] > resultado){
					resultado = versicolor[i];
					classification = "Iris-versicolor";
				}	
			}
			
		}
		
		if (!virginica.empty())
		{
			for (int i = 0; i < virginica.size(); ++i)
			{
				if (virginica[i] > resultado){
					resultado = virginica[i];
					classification = "Iris-virginica";
				}
			}			
		}
	}
	
	void gralRule()
	{
		if (!setosa.empty())
		{
			for (int i = 0; i < setosa.size(); ++i)
			{
				result_temp[0] = result_temp[0] + setosa[i];
			}
			result_temp[0] = result_temp[0] / setosa.size();
			
			if (result_temp[0] > resultado){
				resultado = result_temp[0];
				classification = "Iris-setosa";
			}
		}
		
		if (!versicolor.empty())
		{
			for (int i = 0; i < versicolor.size(); ++i)
			{
				result_temp[1] = result_temp[1] + versicolor[i];
			}
			result_temp[1] = result_temp[1] / versicolor.size();
			
			if (result_temp[1] > resultado){
				resultado = result_temp[1];
				classification = "Iris-versicolor";
			}
		}
		
		if (!virginica.empty())
		{
			for (int i = 0; i < virginica.size(); ++i)
			{
				result_temp[2] = result_temp[2] + virginica[i];
			}
			result_temp[2] = result_temp[2] / virginica.size();
						
			if (result_temp[2] > resultado){
				resultado = result_temp[2];
				classification = "Iris-virginica";
			}
		}
	}
};


//Función para cargar archivo CSV

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
		printf("\nArchivo CSV cargado: ");
		cout<<lista.size()<<"x"<<lista[0].size()<<endl;
	}
	else{
		printf("No se puede abrir el archivo CSV\n");
		return {};
	}
	
	return lista;
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

//Encuentra los valores de mu para cada uno de los conjuntos
vector <vector <double>> muSets (vector <double> &X, vector <int> &Y, double fx) {

	vector <double> row={0,0,0};
	vector <vector <double>> mu;

	for (int i = 0; i < X.size(); i = i + 2) {
		if(fx >= X[i] && fx < X[i+1]) {
			row[0] = X[i];
			row[1] = X[i+1];
			row[2] = muValue(fx, X[i], X[i+1], Y[i], Y[i+1]);
			mu.push_back(row);
		}
	}
	
	//return mu[minElementIndex];
	return mu;
}

double tNorm(string st_norm, double rule_A, double rule_B, double rule_C, double rule_D)
{
	double tipFlor;
	
	if(st_norm == "min"){
		tipFlor = min(rule_A, min(rule_B, min(rule_C, rule_D)));
	}
	else if(st_norm == "prod_a"){
		tipFlor = rule_A * rule_B * rule_C * rule_D;
	}
	else if(st_norm == "prod_f"){
		tipFlor = rule_C + rule_D - 1;
		tipFlor = max(0.0, tipFlor);
		tipFlor = tipFlor + rule_B - 1;
		tipFlor = max(0.0, tipFlor);
		tipFlor = tipFlor + rule_A - 1;
		tipFlor = max(0.0, tipFlor);
		//tipFlor = max(0.0f, rule_A + max(0.0f, rule_B + max(0.0f, rule_C + rule_D - 1) - 1) - 1);
	}
	else if(st_norm == "prod_d"){
		if (rule_A == 1 && rule_B == 1 && rule_C == 1)
			tipFlor = rule_D;
		else if (rule_A == 1 && rule_B == 1 && rule_D == 1)
			tipFlor = rule_C;
		else if (rule_A == 1 && rule_C == 1 && rule_D == 1)
			tipFlor = rule_B;
		else if (rule_B == 1 && rule_C == 1 && rule_D == 1)
			tipFlor = rule_A;
		else
			tipFlor = 0;
	}
	else
		return -1;
		
	return tipFlor;
}

void ruleApply	(double ruleV_A, double ruleV_B, double ruleV_C, double ruleV_D, 
		flowClass &tipFlor, string setName, string st_norm, int count)
{
	if (ruleV_A && ruleV_B && ruleV_C && ruleV_D) {
		if (setName == "1") {
			tipFlor.setosa.emplace_back(tNorm(st_norm, ruleV_A,ruleV_B, ruleV_C, ruleV_D));
		}
		else if (setName == "2") {
			tipFlor.versicolor.emplace_back(tNorm(st_norm, ruleV_A,ruleV_B, ruleV_C, ruleV_D));
		}
		else if (setName == "3") {
			tipFlor.virginica.emplace_back(tNorm(st_norm, ruleV_A,ruleV_B, ruleV_C, ruleV_D));
		}
		else
			return;

		//cout<<count<<" - "<<ruleV_A<<" / "<<ruleV_B<<" / "<<ruleV_C<<" / "<<ruleV_D<<":  "<<setName<<endl;
	}
	
	return;
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
