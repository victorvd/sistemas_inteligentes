void pittsburgh(vector <vector <string>> &lista,
		flowSize sepLenRul,
		flowSize sepWidRul,
		flowSize petLenRul,
		flowSize petWidRul)
{

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Crear reglas aleatorias						|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	vector <vector <int>> rulez;
	
	random_device rd;
	
	mt19937 gen(rd());
	
	uniform_int_distribution<> disSym(0, Sym);
	uniform_int_distribution<> disRul(0, NP-1);
	uniform_int_distribution<> disCro(1, Cros);
	uniform_int_distribution<> disMut(1, Mut);
	
	for (int i = 0; i < NP; ++i){
		vector <int> bigRow; // Temporary vector for acumulating Npop
		for (int j = 0; j < NR; ++j){
			vector <int> row; // Temporary vector for acumulating Npop
			for (int k = 0; k < Ant; ++k){
				row = rulgen(lista, sepLenRul, sepWidRul, petLenRul, petWidRul);
			}
			//Concantenar resultados en un vector más grande
			bigRow.insert(bigRow.end(), row.begin(), row.end());
		}
		rulez.emplace_back(bigRow);
	}
	
	save_csv("results/PittsRules.csv", rulez);

int count = 1;
	
while (count <= Ite)
{

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Genereación de reglas y razonamiento difuso			|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	vector <vector <double>> uAqxp_CFq;
	vector <vector <double>> uAwxpCFw;
	
	for (int i = 0; i < NP; ++i)
	{
		vector <double> CFq_row (6,0); // Variable temporal para almacenar los valores de probabilidad por regla
		vector <double> uAqxp_row (Ant,0); // Variable temporal para acumular los valores de aciertos por clase
		
		for (int k = 0; k < NR; ++k)
		{
			for (int j = 0; j < lista.size(); ++j)
			{
				double uAqxp;
				
				double sepLen = stof(lista[j][0]);
				double sepWid = stof(lista[j][1]);
				double petLen = stof(lista[j][2]);
				double petWid = stof(lista[j][3]);
				
				//uAqxp = uAq1xp1 * uAq2xp2 * uAq3xp3 * uAq4xp4;
				uAqxp = compGrade(rulez[i], sepLen, sepWid, petLen, petWid,
						&sepLenRul, &sepWidRul, &petLenRul, &petWidRul, k * 5);
				
				uAqxp_row[0] += uAqxp;
				
				if (lista[j][4] == "Iris-setosa")
					uAqxp_row[1] += uAqxp;
				else if (lista[j][4] == "Iris-versicolor")
					uAqxp_row[2] += uAqxp;
				else if (lista[j][4] == "Iris-virginica")
					uAqxp_row[3] += uAqxp;
			}
		}
		
		//Fuzzy conditional probability for each class
		CFq_row[0] = uAqxp_row[1] / uAqxp_row[0]; //PrClass1 setosa
		CFq_row[1] = uAqxp_row[2] / uAqxp_row[0]; //PrClass2 versicolor
		CFq_row[2] = uAqxp_row[3] / uAqxp_row[0]; //PrClass3 virginica
		
		//Consequent Class Cq of the fuzzy rule Rq
		CFq_row[3] = max(CFq_row[0], max(CFq_row[1], CFq_row[2])); //PrClassCq
		
		//Complement of the consequent Class Cq
		CFq_row[4] = CFq_row[0] + CFq_row[1] + CFq_row[2] - CFq_row[3]; // PrClassNCq
		
		//Certainty Factor (Rule weight)		
		CFq_row[5] = CFq_row[3] - CFq_row[4]; //CFq
		
		uAqxp_CFq.emplace_back(CFq_row);	//PrClass1, PrClass2, PrClass3, PrClassCq, PrClassNCq, CFq

	}
	
	//display(uAqxp_CFq);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Evaluación de reglas						|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//cout << "Evaluación... ";
	
	for (int i = 0; i < lista.size(); ++i)
	{
		double sepLen = stof(lista[i][0]);
		double sepWid = stof(lista[i][1]);
		double petLen = stof(lista[i][2]);
		double petWid = stof(lista[i][3]);
		
		vector <double> uAwxpCFw_bigrow;
		
		for (int j = 0; j < NP; ++j)
		{
			vector <double> uAwxpCFw_row (6,0);
			
			for (int k = 0; k < NR; ++k)
			{
				double uAqxp;
				
				// Compatibility grade of each training pattern xp and antecedent part Aq
				uAqxp = compGrade(rulez[j], sepLen, sepWid, petLen, petWid,
						&sepLenRul, &sepWidRul, &petLenRul, &petWidRul, k * 5);

				if (uAwxpCFw_row[0] < uAqxp * uAqxp_CFq[j][5]){ //uAqxp_CFq[5]->CFq Certainty Factor
					uAwxpCFw_row[0] = uAqxp * uAqxp_CFq[j][5]; // Rule weight o certainty factor
					uAwxpCFw_row[1] = rulez[j][0 + k * 5]; // Antecedent 1
					uAwxpCFw_row[2] = rulez[j][1 + k * 5]; // Antecedent 2
					uAwxpCFw_row[3] = rulez[j][2 + k * 5]; // Antecedent 3
					uAwxpCFw_row[4] = rulez[j][3 + k * 5]; // Antecedent 4
					uAwxpCFw_row[5] = rulez[j][4 + k * 5]; // Class
				}
			}
			//Concantenar resultados en un vector más grande
			uAwxpCFw_bigrow.insert(uAwxpCFw_bigrow.end(), uAwxpCFw_row.begin(), uAwxpCFw_row.end());
		}
		
		uAwxpCFw.emplace_back(uAwxpCFw_bigrow);
	}
		//cout << "done!\n";

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Aptitud (fitness) de reglas					|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	//cout << "Aptitud... ";
		
	vector <int> fitness (NP,0);
	
	int aciertos = 0;
	
	for (int i = 0; i < lista.size(); ++i)
	{
		int clase;
		
		if (lista[i][4] == "Iris-setosa")
			clase = 1;
		else if (lista[i][4] == "Iris-versicolor")
			clase = 2;
		else if (lista[i][4] == "Iris-virginica")
			clase = 3;
		
		for (int j = 0; j < NP; ++j) {
			for (int k = 0; k < NR; ++k) {
				if (uAwxpCFw[i][5 + k * 6] == clase){
					//++aciertos;
					if ((uAwxpCFw[i][1 + k * 6] == rulez[j][0 + k * 5] || rulez[j][0 + k * 5] == 0) && 
						(uAwxpCFw[i][2 + k * 6] == rulez[j][1 + k * 5] || rulez[j][1 + k * 5] == 0) && 
						(uAwxpCFw[i][3 + k * 6] == rulez[j][2 + k * 5] || rulez[j][2 + k * 5] == 0) && 
						(uAwxpCFw[i][4 + k * 6] == rulez[j][3 + k * 5] || rulez[j][3 + k * 5] == 0) && 
						uAwxpCFw[i][5 + k * 6] == rulez[j][4 + k * 5]){
						fitness[j] += 1;
						++aciertos;}
				}
			}
		}
	}
	//Imprimir cantidad de aciertos por regla
	cout << "Aciertos: " << aciertos << endl;
	
	for (int i = 0; i < fitness.size(); ++i)
	{
		cout << fitness[i] << " ";
	}
	cout << endl;
	//cout << "done!\n";

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Npop - 1 rule sets generated				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	vector <vector<int>> newRuleSet;

	// variables temporales para guardar los índices de los elementos de las matrices
	int t1, t2;
		
	while (newRuleSet.size() < NP - 1){

	vector <int> parent1;
	vector <int> parent2;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
	|	Mating pool y selección por torneo				|
	\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
			
		//cout << "Mating pool... ";
		
		// Creación de mating pool
		vector <vector<int>> matiPool;
		
		while (matiPool.size() < NP){
			t1 = disRul(gen);
			t2 = disRul(gen);
			
			while (t2 == t1)
				t2 = disRul(gen);
			
			vector<int> row;

			for (int j = 0; j < 5*NR; ++j){
				// Selección por torneo binario
				if (fitness[t1] > fitness[t2])	
					row.emplace_back(rulez[t1][j]);
				else
					row.emplace_back(rulez[t2][j]);
			}
			matiPool.emplace_back(row);
		}
		
		//cout << "done!\n";
		//save_csv("results/PmatingPool.csv", matiPool);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
	|	Crossover							|
	\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		//cout << "Crossover... ";
		
		t1 = disRul(gen);
		t2 = disRul(gen);
		
		vector<bool> mask = {0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1};
		
		// Se eligen 2 parejas de la mating pool y se cruzan se la probabilidad de crossover
		int crossProb = disCro(gen);
		
		if (crossProb < Cros){ //Crossover probability: 0.9
			for (int i = 0; i < NR; ++i){
				for (int j = 0; j < 5; ++j){
					if (mask[i]){				
						parent1.emplace_back(matiPool[t1][j + i * 5]);
						parent2.emplace_back(matiPool[t2][j + i * 5]);
					}
					else{
						parent1.emplace_back(matiPool[t2][j + i * 5]);
						parent2.emplace_back(matiPool[t1][j + i * 5]);
					}
				}
			}
		}
		else{
			for (int i = 0; i < 5*NR; ++i){
				parent1.emplace_back(matiPool[t1][i]);
				parent2.emplace_back(matiPool[t2][i]);
			}
		}
		/*cout<<"Fitness: " << fitness[t1] << " " << "Parent 1: ";
		display({matiPool[t1]});
			cout << endl;
		cout<<"Fitness: " << fitness[t2] << " " << "Parent 2: ";
		display({matiPool[t2]});
			cout << endl;
		cout<<"Fitness: " << fitness[t1] << " " << "Offspring 1: ";
		display({parent1});
			cout << endl;
		cout<<"Fitness: " << fitness[t2] << " " << "Offspring 2: ";
		display({parent2});
			cout << endl;
		
		//cout << "done!\n";*/

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
	|	Mutation							|
	\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		
		//cout << "Mutation... ";
		
		for (int i = 0; i < 5*NR; ++i){//Mutate the n antecedents (n-dimensional patterns) but not the class
			int mutProb = disMut(gen);
			if(mutProb < 2){//Mutation Probability: 1/n = 1/4
				if ((i+1)%5 != 0)
					parent1[i] = disSym(gen);
			}
			
			mutProb = disMut(gen);
			if(mutProb < 2){//Mutation Probability: 1/n = 1/4
				if ((i+1)%5 != 0)
					parent2[i] = disSym(gen);
			}
		}
		/*display({parent1});
			cout << endl;
		display({parent2});
			cout << endl;*/
		//cout << "done!\n";
		
		newRuleSet.emplace_back(parent1);
		newRuleSet.emplace_back(parent2);
	
	} // While end (New Rule Set)
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Replace offsprings in mating pool				|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	//Encontrar las reglas con el mayor fitness
	t1 = 0;
	for(int i = 0; i < NP; ++i){
		if (fitness[i] > fitness[t1])
			t1 = i;
	}

	vector <int> row;
	
	for(int i = 0; i < 5*NR; ++i){
		row.emplace_back(rulez[t1][i]);
	}
	
	// Aegurar que el número de conjuntos en newRuleSet siempre sea menor que NP (NP-1)
	if (newRuleSet.size() == NP)
		newRuleSet.pop_back();
	
	newRuleSet.emplace_back(row);
	newRuleSet.erase(newRuleSet.begin());
	newRuleSet.emplace_back(row);
	newRuleSet.erase(newRuleSet.begin());
	newRuleSet.emplace_back(row);
	newRuleSet.erase(newRuleSet.begin());
	newRuleSet.emplace_back(row);
	
	rulez.clear();
	
	rulez = newRuleSet;
	
	++count;
	
} // While end (Iterations)

	save_csv("results/PittsRulesB.csv", rulez);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*\
|	Aplicar fitness para encontrar el mejor set			|
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	vector <vector <double>> uAqxp_CFq;
	vector <vector <double>> uAwxpCFw;
	
	for (int i = 0; i < NP; ++i)
	{
		vector <double> CFq_row (6,0);
		vector <double> uAqxp_row (Ant,0);
		
		for (int k = 0; k < NR; ++k)
		{
			for (int j = 0; j < lista.size(); ++j)
			{
				double uAqxp;
				
				double sepLen = stof(lista[j][0]);
				double sepWid = stof(lista[j][1]);
				double petLen = stof(lista[j][2]);
				double petWid = stof(lista[j][3]);
				
				uAqxp = compGrade(rulez[i], sepLen, sepWid, petLen, petWid,
						&sepLenRul, &sepWidRul, &petLenRul, &petWidRul, k * 5);
				
				uAqxp_row[0] += uAqxp;
				
				if (lista[j][4] == "Iris-setosa")
					uAqxp_row[1] += uAqxp;
				else if (lista[j][4] == "Iris-versicolor")
					uAqxp_row[2] += uAqxp;
				else if (lista[j][4] == "Iris-virginica")
					uAqxp_row[3] += uAqxp;
			}
		}
		
		//Fuzzy conditional probability for each class
		CFq_row[0] = uAqxp_row[1] / uAqxp_row[0];
		CFq_row[1] = uAqxp_row[2] / uAqxp_row[0];
		CFq_row[2] = uAqxp_row[3] / uAqxp_row[0];
		
		//Consequent Class Cq of the fuzzy rule Rq
		CFq_row[3] = max(CFq_row[0], CFq_row[1]);
		CFq_row[3] = max(CFq_row[1], CFq_row[2]);
		
		//Complement of the consequent Class Cq
		CFq_row[4] = CFq_row[0] + CFq_row[1] + CFq_row[2] - CFq_row[3];
		
		//Certainty Factor (Rule weight)		
		if (rulez[i][4] == 0)
			CFq_row[5] = 0;
		else
			CFq_row[5] = CFq_row[3] - CFq_row[4];
		
		uAqxp_CFq.emplace_back(CFq_row);

	}	
	
	
	for (int i = 0; i < lista.size(); ++i)
	{
		double sepLen = stof(lista[i][0]);
		double sepWid = stof(lista[i][1]);
		double petLen = stof(lista[i][2]);
		double petWid = stof(lista[i][3]);
		
		vector <double> uAwxpCFw_bigrow;
		
		for (int j = 0; j < NP; ++j)
		{
			vector <double> uAwxpCFw_row (6,0);
			
			for (int k = 0; k < NR; ++k)
			{
				double uAqxp;
				
				// Compatibility grade of each training pattern xp and antecedent part Aq
				uAqxp = compGrade(rulez[j], sepLen, sepWid, petLen, petWid,
						&sepLenRul, &sepWidRul, &petLenRul, &petWidRul, k * 5);

				if (uAwxpCFw_row[0] < uAqxp * uAqxp_CFq[j][5]){
					uAwxpCFw_row[0] = uAqxp * uAqxp_CFq[j][5];
					uAwxpCFw_row[1] = rulez[j][0 + k * 5];
					uAwxpCFw_row[2] = rulez[j][1 + k * 5];
					uAwxpCFw_row[3] = rulez[j][2 + k * 5];
					uAwxpCFw_row[4] = rulez[j][3 + k * 5];
					uAwxpCFw_row[5] = rulez[j][4 + k * 5];
				}
			}
			//Concantenar resultados en un vector más grande
			uAwxpCFw_bigrow.insert(uAwxpCFw_bigrow.end(), uAwxpCFw_row.begin(), uAwxpCFw_row.end());
		}
		
		uAwxpCFw.emplace_back(uAwxpCFw_bigrow);
	}
		
	vector <int> fitness (NP,0);
	
	for (int i = 0; i < lista.size(); ++i)
	{
		int clase;
		
		if (lista[i][4] == "Iris-setosa")
			clase = 1;
		else if (lista[i][4] == "Iris-versicolor")
			clase = 2;
		else if (lista[i][4] == "Iris-virginica")
			clase = 3;
		
		for (int j = 0; j < NP; ++j) {
			for (int k = 0; k < NR; ++k) {
				if (uAwxpCFw[i][5 + k * 6] == clase){
					if (uAwxpCFw[i][1 + k * 6] == rulez[j][0 + k * 5] && 
						uAwxpCFw[i][2 + k * 6] == rulez[j][1 + k * 5] &&
						uAwxpCFw[i][3 + k * 6] == rulez[j][2 + k * 5] &&
						uAwxpCFw[i][4 + k * 6] == rulez[j][3 + k * 5] && 
						uAwxpCFw[i][5 + k * 6] == rulez[j][4 + k * 5]){
						fitness[j] += 1;}
				}
			}
		}
	}
	
	int index;
	vector <vector <int>> winset;
	
	for (int i = 1; i < NP; i+=2) {
		if (fitness[i-1] < fitness[i])
			index = i;
		else
			index = i-1;
	}
	
	for (int i = 0; i < 5*NR; i+=5) {
		vector <int> row;
		for (int j = 0; j < 5; ++j){
			row.emplace_back(rulez[index][j + i]);
		}
		winset.emplace_back(row);
	}
	
	save_csv("results/PittsWinSet.csv", winset);

	return;

}
